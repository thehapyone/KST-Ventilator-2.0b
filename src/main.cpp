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
#define kpa_mmh20 101.9716213  // Conversion from kpa to mmh20
#define kpa_cmh20 10.19716213  // Conversion from kpa to cmh20


#define pre_sensitivity 0.450 // Sensor sensitivity - 450mv/kPa

  /*  for our sensor the transfer function from the specification PDF is
   *  Vout = Vs * (0.09 * P + 0.04)
   *  Vs = 5 V
   *  P = (Vout/Vs - 0.04)/0.09 = (Vout/5-0.04)/0.09
   *  Vout = sensorVal
   *  P = (sensorVal/5-0.04)/0.09
   */

float preasureReading = 0; // The detected presure 
float preasureReading2 = 0; // The detected presure 2
float sensorOffset_2 = 0.2;
float refVs = 5.00;  // The reference voltage
void setup() {
  // put your setup code here, to run once:
  // Configuring the external reference voltage
  analogReference(DEFAULT);
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
      float valueA0 = (float)analogRead(A0);
  float toVolt = (valueA0 * refVs) / 1023.00;

  
  Serial.print("Raw voltage - ");
  Serial.println(toVolt);


  if (toVolt < sensorOffset_2 )
    toVolt = sensorOffset_2;

  // Calculate the presure reading in kPa
  preasureReading = ((toVolt / refVs) - 0.04 ) / 0.09;
  preasureReading2 = (1 / pre_sensitivity) * (toVolt - sensorOffset_2);

  float sensorValue = ((valueA0 * ADC_mV - SensorOffset) / sensitivity * mmh2O_kpa);     // result in kPa
  // print out the value you read:

  Serial.print("Raw voltage - ");
  Serial.println(toVolt);

    Serial.print("Air Pressure: ");  
  Serial.print(sensorValue,2);
  Serial.println(" kPa");


  Serial.print("Air Pressure Opt2 : ");  
  Serial.print(preasureReading, 2);
  Serial.println(" kPa");


  Serial.print("Air Pressure Opt3 : ");  
  Serial.print(preasureReading2, 2);
  Serial.println(" kPa");

  //Serial.print("Raw voltage ADC - 5V - ");
  //Serial.println(valueA1);
  
  delay(2000);        // delay in between reads for stability
}

