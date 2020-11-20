/***
 * KST Ventilator Project - XXX
 * Version: 2.0
 * Author: Knightec KST Team - Tomislav Galeta, Ayo Ayibiowu
 * @desc: some text
 * 
 * Change Logs ----
 * v0.1 #######################################
 * - Simple breathing patten using constant motor speed value and cpu blocking (delay)
 * - No support for exhale pressure assist (Motor is disabled during expiratory)
 * v0.1 #######################################
 * - 
 **/

#include <Arduino.h>

/********** Presure Sensor Parameters *********************/
#define SensorOffset 102.0     // in mV taken from datasheet
#define ADC_mV 4.8828125       // convesion multiplier from Arduino ADC value to voltage in mV
#define sensitivity 4.413      // in mV/mmH2O taken from datasheet
#define mmh2O_cmH2O 10         // divide by this figure to convert mmH2O to cmH2O
#define mmh2O_kpa 0.00981      // convesion multiplier from mmH2O to kPa

/************ Pin Definitions *****************************/
#define motorPin 3 // Motor pin for PWM control
#define motorChannelA 12 // Motor channel A
#define brakeChannelA 9 // Motor Barke Channel A
#define MOTOR_BRAKE_USED 1 // For setting if the motor shield brake funtionality will be used
#define motorSensingPin A0 // Current pin for the motor

/************* Breathing Settings *************************/
#define breathPerMin 12 // The Breath per minute
#define inhaleRatio 0.4 // Percentage of inhale time. Currently using a ratio of 2:3 (Inhale:Exhale)
#define toMillsecs 1000 // Converting Seconds to Millseconds
#define INHALE_MODE 1
#define EXHALE_MODE 2

uint16_t breathDuration = 0;  // Breath duration in seconds - Inhale + Exhale
uint16_t inhaleTime = 0;  // Inspiratory time
uint16_t exhaleTime = 0;  // Expiratory time

uint8_t inhaleSpeed = 255;
uint8_t exhaleSpeed = 100; 

uint8_t breathingMode; // Breathing mode sething - Inhale or exhale
/************* Timer Variables *************************/
uint32_t timeNow = 0;
uint32_t timePrev = 0;
uint32_t timeDiff = 0;

/************* Other Variables *************************/
uint16_t motorCurrent = 0;


/************* Function declaration *************************/
void initialize(void);
void stopMotor(void);
void startMotor(void);
void setMotorSpeed(uint8_t);

/**
 * Stops the motor. Requries startMotor() to work again
 * @param (None)
 * @returns None
 **/
void stopMotor(void)
{
  analogWrite(motorPin, 0);
  if (MOTOR_BRAKE_USED)
    digitalWrite(brakeChannelA, HIGH);
}

/**
 * Starts the motor. Only needs to be called once until stopMotor() is called again
 * @param (None)
 * @returns None
 **/
void startMotor(void)
{
  // Disengage the brake if activated
  if (MOTOR_BRAKE_USED)
    digitalWrite(brakeChannelA, LOW);
  analogWrite(motorPin, 0);
}

/**
 * Sets the Motor speed
 * @param (byte) A PWM Speed value between 0-255
 * @returns None
 **/
void setMotorSpeed(uint8_t speed)
{
  analogWrite(motorPin, speed);
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
  inhaleTime = 0.4 * breathDuration; 
  exhaleTime = breathDuration - inhaleTime; 

  Serial.begin(9600);     // initialize serial communication at 9600 bits per second:

  // Configure the output pins
  pinMode(motorChannelA, OUTPUT); //Initiates Motor Channel A 
  pinMode(brakeChannelA, OUTPUT); //Initiates Brake Channel A

  // set the starting breathing mode
  breathingMode = EXHALE_MODE;

}


void setup() {
  initialize(); // Initialize device parameters

  // start the motor
  startMotor();

  // get the current time
  timePrev = millis();

  delay(2000);
}


void loop() {

  // Check the breathing mode
  switch (breathingMode)
  {
  case INHALE_MODE:
    // activate the inhale pressure
    setMotorSpeed(inhaleSpeed);
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
    setMotorSpeed(exhaleSpeed);
    // check if its time to swtich mode
    timeDiff = millis() - timePrev;

    if (timeDiff >= exhaleTime)
    {
      breathingMode = INHALE_MODE; // change mode    break;
      timePrev = millis();
    }
  default:
    break;
  }

  motorCurrent = analogRead(motorSensingPin);
  Serial.println(motorCurrent);
  Serial.print(" ");

  delay(100);
}

