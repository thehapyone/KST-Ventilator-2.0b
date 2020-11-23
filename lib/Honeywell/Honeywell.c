#include "Honeywell.h"
#include <Arduino.h>
#include <SPI.h>


Honeywell::Honeywell(uint8_t pin)
{
    // Update the private pin
    _pin = pin;
    // Update the max and min pressure of the sensor
    _pressureMin = pressureMin;
    _pressureMax = pressureMax;

}

Honeywell::~Honeywell()
{
}


void Honeywell::begin()
{        
    // SPI COnfigure
    SPI.begin();

    // Configure the Slave pin as output
    pinMode(_pin, OUTPUT)

    // Activate the pin to High to disable the pin
    digitalWrite(_pin, HIGH);

}


float Honeywell::_transferFunc(uint16_t dataIn)
{
    // Calculate the resulting pressure
    float pressureOut = 0;
    pressureOut = ((((float)dataIn - SENSOR_MAX) * (_pressureMax - _pressureMin)) / (SENSOR_MAX - SENSOR_MIN)) + _pressureMin;
    return pressureOut;

}

float Honeywell::update(void)
{
    SPI.beginTransaction(SPISettings(SENSOR_SPI_SPEED, MSBFIRST, SPI_MODE0));
    
    // activate the slave device
    digitalWrite(_pin, LOW);

    uint8_t firstByte, secondByte;

    // read the data
    firstByte = SPI.transfer(0x00); // first byte
    secondByte = SPI.transfer(0x00); // Second byte

    // disable the slave device
    digitalWrite(_pin, HIGH);

    // end SPI
    SPI.endTransaction();

    // To get the status bit
    _statusBit = (firstByte >> 6);

    // To get the pressure data - Flip away the status bit
    uint8_t pressureIn_1 = (firstByte & 63);
    _outputBytes = ((pressureIn_1 << 8) | secondByte);

}

float Honeywell::readPressure(void)
{
    return _transferFunc(_outputBytes);
}

uint8_t Honeywell::readStatus(void)
{
    return _statusBit;
}

