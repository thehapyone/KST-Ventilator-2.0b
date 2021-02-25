#include <state.h>
#include <string.h>
#include <common.h>
#include <motorData.h>
State::State() {
       
}
char* State::getName() {
       return name;
}
void State::reset() {
       State::isComplete = false;
       currentState = SUB_STATE_START;
       
}
State::State(const char *name_, uint8_t id, uint8_t nextState) {
    
    setStateId(id);
    setNextStateId(nextState);
    MotorDataReader motorDataReader;
    motorDataReader.read(id, SUB_STATE_START, &(State::enterydata));

    motorDataReader.read(id, SUB_STATE_REPEAT, &(State::rundata));
    motorDataReader.read(id, SUB_STATE_LEAVE, &(State::exitdata));
    State::enterOperation = new MotorOperation(SUB_STATE_START, "init", &(State::enterydata));
    
    State::exitOperation = new MotorOperation(SUB_STATE_LEAVE, "leave", &(State::exitdata));
    State::runOperation = new MotorOperation( SUB_STATE_REPEAT, "repeat", &(State::rundata));
    reset();
    strncpy(State::name, name_, sizeof(State::name)-1);
    
}
void State::moveToNextOperation()
{
    
}
/*void State::logMessage(const char*task, const char* subTask, const char*message, uint8_t errorCode)
{
    
}*/
uint8_t State::run(float currentData) {
   //debug(0, name, activeOperation->getName(), "running", 0);
   uint8_t status;
   
   switch (currentState)
   {
   case SUB_STATE_START/* constant-expression */:
       
       status = State::enterOperation->run(stateId, currentData);
       if (!State::enterOperation->isRunning())
            currentState = SUB_STATE_REPEAT;
       break;
   case SUB_STATE_REPEAT/* constant-expression */:
       
       status = State::runOperation->run(stateId, currentData);
       if (!State::runOperation->isRunning())
            currentState = SUB_STATE_LEAVE;
       break;
   case SUB_STATE_LEAVE/* constant-expression */:
       
       status = State::exitOperation->run(stateId, currentData);
       if (!State::exitOperation->isRunning()){
           currentState = SUB_STATE_START;
           isComplete = true;
       }
            
       break; 
   default:
       break;
   }
   return status;
}
bool State::getisCompeleted(){
  return State::isComplete;
}
uint8_t State::getStateId(){
    return stateId;
}
uint8_t State::getNextStateId(){
    return nextStateId;
}
void State::setStateId(uint8_t id)
{
    stateId = id;
}
void State::setNextStateId(uint8_t nextState)
{
    nextStateId = nextState; 
}