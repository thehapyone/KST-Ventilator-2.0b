/***
 * KST Ventilator Project - XXX
 * Version: 2.0
 * Author: Knightec KST Team - Tomislav Galeta, Ayo Ayibiowu
 * @desc: some text
 * 
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
  // read the input on analog pin 0:
  float sensorValue = ((analogRead(A0) * ADC_mV - SensorOffset) / sensitivity * mmh2O_kpa);     // result in kPa
  // print out the value you read:
  Serial.print("Air Pressure: ");  
  Serial.print(sensorValue,2);
  Serial.println(" kPa");
  
  delay(1000);        // delay in between reads for stability
}

