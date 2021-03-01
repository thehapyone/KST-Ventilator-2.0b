#include <PID.h>

PIDWrapper::PIDWrapper(): myPID(&Input, &Output, &Setpoint, consKp, consKi, consKd, DIRECT)
{
    aggKp=4;
    aggKi=0.2;
    aggKd=1;
    consKp=1;
    consKi=0.05;
    consKd=0.25;
    //PID myPID(&Input, &Output, &Setpoint, consKp, consKi, consKd, DIRECT);
    myPID.SetMode(AUTOMATIC);
    myPID.SetOutputLimits(0, 100);

}

void PIDWrapper::compute()
{
    myPID.Compute();
}

void PIDWrapper::SetTunings(double consKp, double consKi, double consKd)
{
    myPID.SetTunings(consKp, consKi, consKd);
}
double PIDWrapper::getOutput()
{
    return Output;
}
double PIDWrapper::getInput()
{
    return Input;
}
void PIDWrapper::setInput(float newInput)
{
    Input = newInput;
}
void PIDWrapper::setSetPoint(float newSetPoint)
{
    Setpoint = newSetPoint;
}