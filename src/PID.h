#ifndef PID_H
#define PID_H
#include <Arduino.h>
#include <common.h>
#include <motorData.h>
#include <PID_v1.h>
class PIDWrapper{
public:
    PIDWrapper();
    void compute();
    void SetTunings(double consKp, double consKi, double consKd);
    double getOutput();
    void setInput(float Input_);
    double getInput();
    void setSetPoint(float newSetPoint);
private:
    double aggKp=4;
    double aggKi=0.2;
    double aggKd=1;
    double consKp=1;
    double consKi=0.05;
    double consKd=0.25;
    double Setpoint = 0;
    double Input = 0;
    double Output = 0;
    //double aggKp, aggKi, aggKd;
    //double consKp, consKi, consKd;
    PID myPID;
};
#endif