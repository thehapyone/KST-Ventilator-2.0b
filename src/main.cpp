/***
 * KST Ventilator Project - XXX
 * Version: 2.0
 * Author: Knightec KST Team - Ayo Ayibiowu, Tomislav Galeta
 * Contact Email: ayodeji.ayibiow@knightec.se
 * @desc: The goal is to be able to create a low cost ventilator device with
 * the ability to changing breathing parameters like breaths per minutes and others.
 * 
 * Change Logs ----
 * v0.1 #######################################
 * - Simple breathing patten using constant motor speed value and cpu blocking (delay)
 * - No support for exhale pressure assist (Motor is disabled during expiratory)
 * v0.2 #######################################
 * - CPU blocking removed.
 * - Support for setting the breaths per minute
 * - Support for setting the inhale and exhale duration
 * - Support for setting the inhale and exhale speed
 * - Support for measuring motor current usage
 * v0.21 ########################################
 * - Addition of pressure sensor support.
 * v0.22 ########################################
 * - Migrate code to support new motor : Brushless DC motor with an ESC
 **/

#include <Arduino.h>
#include <Honeywell.h>
#include <EasyNextionLibrary.h>
#include <Servo.h>
#include <SoftwareSerial.h>

SoftwareSerial mySerial(5, 6); // RX, TX


/************ Pin Definitions *****************************/
Servo motor;

#define motorPin 3 // Motor pin for PWM control - new motor based on brushless motor
#define motorMaxSpeed 180
#define motorMinSpeed 0
/************* Breathing Settings *************************/
uint8_t breathPerMin = 12; // The Breath per minute
double inhaleRatio = 0.4; // Percentage of inhale time. Currently using a ratio of 2:3 (Inhale:Exhale)
#define toMillsecs 1000 // Converting Seconds to Millseconds
#define INHALE_MODE 1
#define EXHALE_MODE 2

uint16_t breathDuration = 0;  // Breath duration in seconds - Inhale + Exhale
uint16_t inhaleTime = 0;  // Inspiratory time
uint16_t exhaleTime = 0;  // Expiratory time

uint8_t inhaleSpeed = 180; // This is the equivalent pressure value in speed that is sent to the motor
uint8_t exhaleSpeed = 80; 

double ipap = 12; // Pressure values in mmH20
double epap = 5; // pressure values in mmH20

uint8_t breathingMode; // Breathing mode sething - Inhale or exhale

/************* Timer Variables *************************/
uint32_t timeNow = 0;
uint32_t timePrev = 0;
uint32_t timeDiff = 0;

/************* Other Variables *************************/
uint16_t motorCurrent = 0;


/************* Pressure Sensor Config *************************/
const uint8_t sensorPin = 10;
Honeywell pressureSensor(sensorPin, 0.0, 60.0); //create instance of the sensor
#define tocmH20 1.0197162129779

/************* Nextion Display Configuration ********************/

EasyNex nextion(mySerial); // Should we use SoftwareSerial or HardwareSerial
#define displayRefresh 100 // In millisecs

uint8_t currentPageId = 0; // Saves the current page id

// For tracking the page ids
#define SETTINGS_PAGE_ID 3
#define SAVE_PAGE_ID 4
#define HOME_PAGE_ID 2
#define START_PAGE_ID 1
#define WELCOME_PAGE_ID 0


/************* Function declaration *************************/
void initialize(void);
void stopMotor(Servo&);
void startMotor(Servo&);
void setMotorSpeed(Servo&, uint8_t);
void changePage(uint8_t);

/**
 * Stops the motor. Requries startMotor() to work again
 * @param (None)
 * @returns None
 **/
void stopMotor(Servo& motor)
{
  motor.write(0);
}

/**
 * Starts the motor. Only needs to be called once until stopMotor() is called again
 * @param (Servo object)
 * @returns None
 **/
void startMotor(Servo& motor)
{
  motor.attach(motorPin,1000,2000); // (pin, min pulse width, max pulse width in microseconds) 
  //"Initializing ESC");
  motor.write(motorMaxSpeed);
  delay(2200);
  motor.write(motorMinSpeed);
  delay(3000);
}

/**
 * Sets the Motor speed
 * @param (byte) A PWM Speed value between 0-255
 * @returns None
 **/
void setMotorSpeed(Servo& motor, uint8_t speed)
{
  motor.write(speed);
}

/**
 * Here we calculate the breathing information
 * @param (None)
 * @returns None
 **/
void updateBreathingParameters(void)
{
  // update the breathing parameters
  breathDuration = (60.0/breathPerMin) * toMillsecs;
  inhaleTime = inhaleRatio * breathDuration; 
  exhaleTime = breathDuration - inhaleTime; 
}


/**
 * Here we initialize the startup device parameters
 * @param (None)
 * @returns None
 **/
void initialize(void)
{
  // Enable screen communication
  nextion.begin(9600);
  changePage(WELCOME_PAGE_ID);

  // update the breathing parameters
  updateBreathingParameters();

  // start the motor
  startMotor(motor);


  // set the starting breathing mode
  breathingMode = EXHALE_MODE;

}

/**
 * Here we send a command to change the screen of the Nextion display
 * @param (String) - The page name we want to change to.
 * @returns None
 **/

void changePage(uint8_t pageName)
{
  // send a command to change the screen
  
  nextion.writeStr("page "+String(pageName));

}

void homeScreenUpdate()
{
  // Allows for updating the values on the homeScreen.
  // Here, we update the latest IPAP, EPAP, BPM and Inhale Time values
}

/**
 * Here we update the variables value when the save button is pressed
 * We update the latest IPAP, EPAP, BPM and Inhale Time values
 * @param None
 * @returns None
 **/

void saveSettingsUpdate()
{
  // Here we update the variables value when the save button is pressed
  // Here, we update the latest IPAP, EPAP, BPM and Inhale Time values
  Serial.println("Save btn fired");
  // Get the BPM
  const int tempBPM = nextion.readNumber("bpm.val");
  if (tempBPM != 777777)
    breathPerMin = (uint8_t)tempBPM;

  // Get the Inhale ratio
  const uint32_t tempInhaleRate = nextion.readNumber("inhale.val");
  if (tempInhaleRate != 777777)
    // convert to ratio values
    inhaleRatio = (double)tempInhaleRate / 100.0;
  
  // Get the IPAP values
  const uint32_t ipapVal = nextion.readNumber("ipap.val");
  if (ipapVal != 777777)
    ipap = (double)ipapVal / 10.0;

  // Get the EPAP values
  const uint32_t epapVal = nextion.readNumber("epap.val");
  if (epapVal != 777777)
    epap = (double)epapVal / 10.0;

  // Update the breathing parameters
  updateBreathingParameters();

  // wait until we are in the save screen
  delay(500);
  // ack
  if (tempBPM == 777777 || tempInhaleRate == 777777 || ipapVal == 777777 || epapVal == 777777)
    // send back a failed save request.
    nextion.writeNum("savestate.val", 2);
  else
  {
    nextion.writeNum("savestate.val", 1);
  }

  Serial.print("new values are: ");
  const String p1=";";
  Serial.println(breathPerMin + p1 + inhaleRatio + p1 + ipap + p1 + epap);
}

/**
 * Send the current values to the screen
 * @param None
 * @returns None
 **/

void settingsScreenUpdate()
{
  // here we update the default or last save values when this is called
  //BPm
  Serial.println("in settings");
  //delay(50);
  if(nextion.currentPageId != SETTINGS_PAGE_ID)
    changePage(SETTINGS_PAGE_ID);
  
  Serial.print("values are: ");
  const String p1=";";
  Serial.println(breathPerMin + p1 + inhaleRatio + p1 + ipap + p1 + epap);

  nextion.writeNum("bpm.val", (uint32_t) breathPerMin);
  //inhaleRatio and exhaleRatio
  uint32_t myInhaleRate = inhaleRatio * 100;
  nextion.writeNum("inhale.val", myInhaleRate);
  nextion.writeNum("exhale.val", 100-myInhaleRate);
  //iPAP and ePAP  
  uint32_t myIpap = ipap * 10;
  nextion.writeNum("ipap.val", myIpap);
  uint32_t myEpap = epap * 10;
  nextion.writeNum("epap.val", myEpap);
}

/**
 * Here we cancel the pending values update.
 * @param None
 * @returns None
 **/
void cancelSettingsSave()
{
  // it basically calls the settings screen update again.
  settingsScreenUpdate();
}

// Trigger mapping functions

 void trigger0()
 {
   saveSettingsUpdate();
 }

 void trigger1()
 {
   cancelSettingsSave();
 }

  void trigger2()
 {
   settingsScreenUpdate();
 }

void setup() {
  Serial.begin(9600);
  Serial.println("Booting Up");

  initialize(); // Initialize device parameters

  // start the pressure sensor
  pressureSensor.begin();

  delay(2000);
  Serial.println("changing screen to start");
  // Send a start command to the screen
  changePage(START_PAGE_ID);
  // get the current time
  timePrev = millis();

}




void loop() {
  // Enable screen listener
  nextion.NextionListen();

  // Check the breathing mode
  switch (breathingMode)
  {
  case INHALE_MODE:
    // activate the inhale pressure
    setMotorSpeed(motor, inhaleSpeed);
    //Serial.print("Current screen - ");
    //Serial.println(nextion.currentPageId);
    // check if its time to swtich mode
    timeDiff = millis() - timePrev;
    if (timeDiff >= inhaleTime)
    {
      breathingMode = EXHALE_MODE; // change mode
      timePrev = millis();
    }
    break;
  
  case EXHALE_MODE:
    // activate the exhale pressure
    setMotorSpeed(motor, exhaleSpeed);
    // check if its time to swtich mode
    timeDiff = millis() - timePrev;

    if (timeDiff >= exhaleTime)
    {
      breathingMode = INHALE_MODE; // change mode    break;
      timePrev = millis();
    }
    break;

  default:
    break;
  }

  // motorCurrent = analogRead(motorSensingPin);
  // Serial.println(motorCurrent);
  // Serial.print(" ");

  // here we read the pressure sensor as well
  pressureSensor.update();
  float currentPressure = pressureSensor.readPressure();
  float pressure2 = currentPressure * tocmH20;

  if (pressureSensor.readStatus() == HONEYWELL_OK)
  {
    // Serial.print("Current Pressure : ");  
    // Serial.print(currentPressure, 2);
    // Serial.print(" mbar | ");

    //Serial.println(pressure2, 2);
    //Serial.println(" cmH20");
  }
  /*
  int currentpage = nextion.currentPageId;
  //Serial.println(currentpage);
  if (currentpage == 3) {
  
  unsigned long xHake = nextion.readNumber("bpm.val"); // Store to x the value of numeric box n0
  Serial.print("bpm val: ");
  Serial.println(xHake);
  Serial.println("----");
  }
  */

  delay(20);
}

