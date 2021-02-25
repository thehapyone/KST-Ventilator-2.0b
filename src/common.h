#ifndef DATATYPES_H

#define DATATYPES_H

#include <Arduino.h>
#include <string.h>


#define OPERATION_ERROR_NO_ERROR 11
#define OPERATION_ERROR_TIME_OUT OPERATION_ERROR_NO_ERROR+1
#define OPERATION_ERROR_UNKNOWN  OPERATION_ERROR_TIME_OUT+1
#define PRESSURE_READ_ERROR OPERATION_ERROR_UNKNOWN+1


#define DEBUG_INFO 1
#define DEBUG_ERROR DEBUG_INFO+1
#define DEBUG_WARNING  DEBUG_ERROR+1


#define UNKNOW_MODE 0xA0
#define ENTER_INHALE_MODE UNKNOW_MODE + 1
#define RUN_INHALE_MODE ENTER_INHALE_MODE + 1
#define EXIT_INHALE_MODE RUN_INHALE_MODE + 1
#define ENTER_EXHALE_MODE EXIT_INHALE_MODE + 1
#define RUN_EXHALE_MODE ENTER_EXHALE_MODE + 1
#define EXIT_EXHALE_MODE RUN_EXHALE_MODE + 1
#define IDEL_MODE EXIT_EXHALE_MODE + 1
#define SUB_STATE_START IDEL_MODE + 1
#define SUB_STATE_REPEAT SUB_STATE_START + 1
#define SUB_STATE_LEAVE SUB_STATE_REPEAT + 1


//void debug(uint8_t messageType, const char* name, const char* messge, uint8_t errorCode);
void debug(uint8_t messageType, const char* task,  const char* subtask,const char* message, uint8_t errorCode);
#endif