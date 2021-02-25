#include <motor.h>
#include <common.h>
#include <motorData.h>

MotorOperation::MotorOperation(uint8_t opId, const char* name_, Motordata_t *motorData_)
{
    
    MotorOperation::motorData.desiredOutput = motorData_->desiredOutput; 
    MotorOperation::motorData.motorSpeed = motorData_->motorSpeed;  
    MotorOperation::motorData.maxRunTimeInMs = motorData_->maxRunTimeInMs;  
    strncpy(MotorOperation::name, name_, sizeof(MotorOperation::name)-1);
    MotorOperation::opId = opId;
    // Serial.print(name_);
    // Serial.print(": ");
    // Serial.print(MotorOperation::motorData.motorSpeed);
    // Serial.print(": ");
    // Serial.print(MotorOperation::motorData.desiredOutput);
    // Serial.print(": ");
    // Serial.println(MotorOperation::motorData.maxRunTimeInMs);
}
void MotorOperation::printStat(const char* name_)
{
    Serial.print(name_);
    Serial.print(": ");
    Serial.print(MotorOperation::name);
    Serial.print(": motorSpeed");
    Serial.print(MotorOperation::motorData.motorSpeed);
    Serial.print(": desiredOutput");
    Serial.print(MotorOperation::motorData.desiredOutput);
    Serial.print(": maxRunTimeInMs");
    Serial.println(MotorOperation::motorData.maxRunTimeInMs);
}
uint8_t MotorOperation::run(uint8_t stateId, float currentData)
{
    uint32_t currentDataInt = (uint32_t)(currentData * 1000);
    
  
    Serial.print("S:");
    Serial.print(stateId);
    Serial.print(",SS:");
    Serial.print(opId); 
    Serial.print(",T:");
    Serial.print(MotorOperation::motorData.desiredOutput);
    Serial.print(",C:");
    Serial.print(currentDataInt);
    Serial.println(";");
    if (MotorOperation::isRunning() != true)
        start();
    /*
    if (currentData > motorData.desiredOutput)
    {
        exit(OPERATION_ERROR_NO_ERROR);
        return OPERATION_ERROR_NO_ERROR;     
    }*/
    if ((millis() - MotorOperation::startTime) > motorData.maxRunTimeInMs)
    {
        exit(OPERATION_ERROR_TIME_OUT);
        return OPERATION_ERROR_TIME_OUT;
    }
   
    return 0;
}
void MotorOperation::start()
{
    MotorOperation::startTime = millis();
    MotorOperation::running = true;    
}
void MotorOperation::exit(uint8_t errorCode)
{
    MotorOperation::running = false;
}
bool MotorOperation::isRunning()
{
    return MotorOperation::running;
}
char* MotorOperation::getName()
{
    return MotorOperation::name;
}
uint8_t MotorOperation::getopId()
{
    return MotorOperation::opId;
}
