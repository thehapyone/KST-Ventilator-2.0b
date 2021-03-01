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
int MotorOperation::calcDeviation(float currentData)
{
    int deviation = ((currentData * 100) / (MotorOperation::motorData.desiredOutput));
    deviation -= 100;
  
    return deviation;
}
uint8_t MotorOperation::run(uint8_t stateId, float currentData, PIDWrapper *myPid)
{
    
    myPid->setSetPoint(MotorOperation::motorData.desiredOutput);
    myPid->setInput(currentData);
    myPid->compute();
    uint32_t currentDataInt = (uint32_t)(myPid->getInput() * 1000);
    uint32_t currentOutInt = (uint32_t)(myPid->getOutput() * 1000);
    uint8_t status = 0;
    double aggKp=4, aggKi=0.2, aggKd=1;
    double consKp=1, consKi=0.05, consKd=0.25;
    if (MotorOperation::isRunning() != true)
        start();
    
    int deviation = calcDeviation(currentData);
    if (deviation > 10)
        myPid->SetTunings(aggKp, aggKi, aggKd);
    if (deviation < 10)
        myPid->SetTunings(aggKp, aggKi, aggKd);
    if (deviation > 0)
        myPid->SetTunings(consKp, consKi, consKd);
    if (deviation < 0)
        myPid->SetTunings(consKp, consKi, consKd);
    if ((millis() - MotorOperation::startTime) > motorData.maxRunTimeInMs)
    {
        exit(OPERATION_ERROR_TIME_OUT);
        status = OPERATION_ERROR_TIME_OUT;
    }
    Serial.print("S:");
    Serial.print(stateId);
    Serial.print(",SS:");
    Serial.print(opId); 
    Serial.print(",T:");
    Serial.print(MotorOperation::motorData.desiredOutput);
    Serial.print(",C:");
    Serial.print(currentDataInt);
    Serial.print(",O:");
    Serial.print(currentOutInt);
    Serial.println(";");
    return status;
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
