/**
 * Library for communicating with the Honeywell SPI Pressure sensors
 *
 **/

#ifndef Honeywell_h
#define Honeywell_h

#include <stdint.h>


// Status Bit Information
/*
Status Bits Definition
S1 S0 
0 0 normal operation, valid data
0 1 device in command mode*
1 0 stale data
1 1 diagnostic condition
*/

#define HONEYWELL_OK 0 // normal operation and data is valid
#define HONEYWELL_STALE 2 // Old data already read my master
#define HONEYWELL_COMMAND 1 // Ideally you will not received this one
#define HONEYWELL_DIAGNOSTIC 3

// Sensor output parameters 
#define SENSOR_MAX 14746.0 // 90% of 2^14 counts or 0x3999
#define SENSOR_MIN 1638.0 // 10% of 2^14 counts or 0x0666

#define SENSOR_SPI_SPEED 700000


class Honeywell
{
private:
    /* data */
    uint8_t _pin; 
    float _pressureMax;
    float _pressureMin;
    uint8_t _statusBit = 0;
    uint16_t _outputBytes = 0;
    
    float _transferFunc(uint16_t); // Converts the SPI output to Pressure.
    
public:
    Honeywell(uint8_t pin, float pressureMin, float pressureMax);
    ~Honeywell();

    void begin(void);
    void update(void); // Calls for every pressure and status read

    float readPressure(void); // Method for reading the device pressure
    uint8_t readStatus(void); // Reads the device status
};



#endif