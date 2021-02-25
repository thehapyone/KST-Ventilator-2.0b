#ifndef MOTOR_H
#define MOTOR_H
#include <Arduino.h>
#include <common.h>
#include <motorData.h>
class MotorOperation{
public:
    MotorOperation();
    MotorOperation(uint8_t opId, const char* name_, Motordata_t *motorData);
    uint8_t run(uint8_t stateId, float currentData); 
    
    uint8_t getrunTimeInMs();
    uint8_t getMotorSpeed();  
    uint8_t getdesiredOutput();  
    uint8_t getLastErrorCode();
    char* getName();
    void setedesiredOutput(uint8_t runTimeInMs_);
    void seterunTimeInMs(uint8_t runTimeInMs_);
    void setMotorSpeed(uint8_t motorSpeed_);
    bool getRunningState();
    void setRunningState(bool isRunning);
    uint8_t getopId(); 
    void printStat(const char* name_);
    bool isRunning();
    
private:
    bool running;
    char name[16];
    uint8_t lastErrorCode;
    Motordata_t motorData;
    uint32_t startTime;
    void start();
    uint8_t opId;
    void exit(uint8_t errorCode);

};
#endif