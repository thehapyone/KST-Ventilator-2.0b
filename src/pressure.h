#ifndef PRESSURE_H
#define PRESSURE_H
#include <Arduino.h>
#include <common.h>
#include <Honeywell.h>


#define MAX_TIME_IN_MS_PRESSURE_READING_IS_VALID  1000 //this defines the amount of time a pressure sensor reading is considered a valid
#define tocmH20 1.0197162129779

class Pressure{
public:
    Pressure(Honeywell* pressureSensor);
    
    float getCurrentValue();
    float getPreviousValue();
    bool isPressureValid();
    uint8_t getErrorCode();
    float read();
    void updatePressure(uint32_t pressure);
private:
    uint32_t lastSuccessfulReadTime;
    float previousValue;
    float currentValue;
    uint8_t errorCode;
    Honeywell *pressureSensor;
    float sensorRead();
    
    float mock();
};

#endif