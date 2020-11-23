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
 **/

#include <Arduino.h>

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

uint8_t inhaleSpeed = 150; // This is the equivalent pressure value in speed that is sent to the motor
uint8_t exhaleSpeed = 50; 

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
void setMotorDirection(uint8_t);
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
 * Sets the Motor Direction
 * @param (byte) 1 - Forward and 0 - Reverse
 * @returns None
 **/
void setMotorDirection(uint8_t direction)
{
  if (direction)
  {
      digitalWrite(12, HIGH);
  }
  else
    digitalWrite(12, HIGH);
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
  setMotorDirection(1);

  delay(2000);

  // get the current time
  timePrev = millis();

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
    break;

  default:
    break;
  }

  motorCurrent = analogRead(motorSensingPin);
  Serial.println(motorCurrent);
  Serial.print(" ");

  delay(10);
}

