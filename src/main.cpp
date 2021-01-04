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

/************ Pin Definitions *****************************/
Servo motor;

#define motorPin 3 // Motor pin for PWM control - new motor based on brushless motor
#define motorMaxSpeed 180
#define motorMinSpeed 0
/************* Breathing Settings *************************/
#define breathPerMin 12 // The Breath per minute
#define inhaleRatio 0.4 // Percentage of inhale time. Currently using a ratio of 2:3 (Inhale:Exhale)
#define toMillsecs 1000 // Converting Seconds to Millseconds
#define INHALE_MODE 1
#define EXHALE_MODE 2

uint16_t breathDuration = 0;  // Breath duration in seconds - Inhale + Exhale
uint16_t inhaleTime = 0;  // Inspiratory time
uint16_t exhaleTime = 0;  // Expiratory time

uint8_t inhaleSpeed = 180; // This is the equivalent pressure value in speed that is sent to the motor
uint8_t exhaleSpeed = 80; 

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
EasyNex nextion(Serial); // Should we use SoftwareSerial or HardwareSerial
#define displayRefresh 100 // In millisecs
uint8_t currentPageId = 0; // Saves the current page id

/************* Function declaration *************************/
void initialize(void);
void stopMotor(Servo&);
void startMotor(Servo&);
void setMotorSpeed(Servo&, uint8_t);
void changePage(String);

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
 * Here we initialize the startup device parameters
 * @param (None)
 * @returns None
 **/
void initialize(void)
{
  // Initialize the breathing parameters
  breathDuration = (60.0/breathPerMin) * toMillsecs;
  inhaleTime = inhaleRatio * breathDuration; 
  exhaleTime = breathDuration - inhaleTime; 

  // start the motor
  startMotor(motor);
  // Enable screen communication
  nextion.begin(115200);

  // set the starting breathing mode
  breathingMode = EXHALE_MODE;

}

/**
 * Here we send a command to change the screen of the Nextion display
 * @param (String) - The page name we want to change to.
 * @returns None
 **/

void changePage(String pageName)
{
  // send a command to change the screen
  
  nextion.writeStr("page "+pageName);

}

void homeScreenUpdate()
{
  // Allows for updating the values on the homeScreen.
  // Here, we update the latest IPAP, EPAP, BPM and Inhale Time values
}

void setup() {
  initialize(); // Initialize device parameters

  // start the pressure sensor
  pressureSensor.begin();

  delay(2000);

  // Send a start command to the screen
  changePage("start");
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

    Serial.println(pressure2, 2);
    //Serial.println(" cmH20");
  }


  delay(50);
}

