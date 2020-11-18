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
 * 
 **/

#include <Arduino.h>

/********** Presure Sensor Parameters *********************/
#define SensorOffset 102.0     // in mV taken from datasheet
#define ADC_mV 4.8828125       // convesion multiplier from Arduino ADC value to voltage in mV
#define sensitivity 4.413      // in mV/mmH2O taken from datasheet
#define mmh2O_cmH2O 10         // divide by this figure to convert mmH2O to cmH2O
#define mmh2O_kpa 0.00981      // convesion multiplier from mmH2O to kPa


void setup() {
  // put your setup code here, to run once:
  pinMode(12, OUTPUT); 
  //Initiates Motor Channel A 
  pinMode(9, OUTPUT);
  //Initiates Brake Channel A pin
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
// put your main code here, to run repeatedly: forward @ full speed
    digitalWrite(12, HIGH); 
    //Establishes forward direction of Channel A
    digitalWrite(9, LOW);  
    //Disengage the Brake for Channel 
    analogWrite(3, 255);   
    //Spins the motor on Channel A at full speed
    delay(1500);
    digitalWrite(9, HIGH); //Eengage the Brake for Channel A
    delay(1800);
}

