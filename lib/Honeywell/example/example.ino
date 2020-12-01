/**
 * An example code for communicating with the Honeywell SPI Pressure sensors
 * For this Example, I will be using the : ABPDRRV060MGSA3
 **/

#include <Arduino.h>
#include "Honeywell.h"


/************* Pressure Sensor Config *************************/
const uint8_t sensorPin = 10; // The slave select pin
Honeywell pressureSensor(sensorPin, 0.0, 60.0); //create instance of the sensor. It requires the SS pin, Min Pressure of sensor, and Max Pressure of sensor.

const float tocmH20 = 1.0197162129779;
void setup()
{
    Serial.begin(9600);
    // start the pressure sensor
    pressureSensor.begin();
    delay(1000);
}

void loop()
{
    // here we read the pressure sensor as well
    pressureSensor.update();
    float currentPressure = pressureSensor.readPressure();
    uint8_t sensorStatus = pressureSensor.readStatus();
    float pressure2 = currentPressure * tocmH20;

    Serial.print("Current Pressure : ");  
    Serial.print(currentPressure, 2);
    Serial.print(" mbar | ");
 
    Serial.print(pressure2, 2);
    Serial.println(" cmH20");

    delay(500);

}
