#ifndef STATE_H
#define STATE_H
#include <Arduino.h>
#include <motor.h>
#include <motorData.h>
class State
{
  public:
    State ();
    State(const char * name, uint8_t id, uint8_t nextState);
    void setStateId(uint8_t id);
    uint8_t getStateId();
    uint8_t getNextStateId();
    void setNextStateId(uint8_t nextStateId);
    uint8_t run(float currentData);
    bool getisCompeleted();
    char* getName();
    void reset();

  private:
    uint8_t stateId;
    uint8_t nextStateId;
    char name[16];
    void moveToNextOperation();
    Motordata_t enterydata, rundata, exitdata;
    MotorOperation *enterOperation;
    MotorOperation *runOperation;
    MotorOperation *exitOperation;
    MotorOperation *activeOperation;
    uint8_t previousOperation;
    bool isComplete;
    uint8_t currentState;
    

};

#endif