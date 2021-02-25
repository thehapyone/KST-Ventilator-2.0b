

#ifndef MOTORDATA_H
#define MOTORDATA_H
#include <Arduino.h>


struct Motordata_t
{
    uint16_t maxRunTimeInMs;
    uint8_t motorSpeed;
    uint8_t desiredOutput;
};
class MotorDataReader{
public:
    MotorDataReader();
    void read(uint8_t stateId, uint8_t subStateId, Motordata_t *data);
    void setData(uint8_t stateId, uint8_t subStateId, uint8_t desiredOutput, uint8_t motorSpeed, uint16_t maxRunTimeInMs,Motordata_t *data);
};
#endif