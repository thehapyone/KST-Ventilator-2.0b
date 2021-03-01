#include <pressure.h>


#define MOCK_PRESSURE_READING
Pressure::Pressure(Honeywell *pressureSensor)
{
    Pressure::pressureSensor = pressureSensor;
}
float Pressure::getCurrentValue()
{
    return Pressure::currentValue;
}
float Pressure::getPreviousValue()
{
    return Pressure::previousValue;
}
bool Pressure::isPressureValid()
{
    uint32_t timeElapsed = millis() - Pressure::lastSuccessfulReadTime;
    if (timeElapsed < MAX_TIME_IN_MS_PRESSURE_READING_IS_VALID)
        return true;
    return false;
}
uint8_t Pressure::getErrorCode()
{
    return Pressure::errorCode;
}
float Pressure::read()
{
#ifdef MOCK_PRESSURE_READING
      return mock();
#else
    return sensorRead();
#endif
}
float Pressure::sensorRead()
{
  Pressure::pressureSensor->update();
  float currentPressure = Pressure::pressureSensor->readPressure();
  float pressure2 = currentPressure * tocmH20;
  Pressure::errorCode = Pressure::pressureSensor->readStatus();
  uint32_t timeElaspsed = millis() - lastSuccessfulReadTime;
  if (Pressure::errorCode == HONEYWELL_OK)
  {    
    lastSuccessfulReadTime = millis();
    previousValue = pressure2;
    return pressure2;
  }
 
  if (timeElaspsed < MAX_TIME_IN_MS_PRESSURE_READING_IS_VALID)
  {
    return previousValue;

  }
  //Here is a good time to reset the pressure sensor  
  return (float)PRESSURE_READ_ERROR;
}
void Pressure::updatePressure(uint32_t pressure)
{
  
  lastSuccessfulReadTime = millis();
  previousValue = (float)(pressure / 1000.0);
}

float Pressure::mock()
{
  

  uint32_t timeElaspsed = millis() - lastSuccessfulReadTime;
  
 
  if (timeElaspsed < MAX_TIME_IN_MS_PRESSURE_READING_IS_VALID)
  {
    return previousValue;

  }
  //Here is a good time to reset the pressure sensor  
  return (float)PRESSURE_READ_ERROR;
}
