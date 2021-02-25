#include <motorData.h>
#include <common.h>
#include <config.h>

MotorDataReader::MotorDataReader()
{

}
void MotorDataReader::setData(uint8_t stateId, uint8_t subStateId, uint8_t desiredOutput, uint8_t motorSpeed, uint16_t maxRunTimeInMs,Motordata_t *data)
{
    
    data->desiredOutput = desiredOutput;
    data->motorSpeed = motorSpeed;
    data->maxRunTimeInMs = maxRunTimeInMs;
   
}
void MotorDataReader::read(uint8_t stateId, uint8_t subStateId, Motordata_t *data)
{
    
    switch (stateId)
    {
        case ENTER_INHALE_MODE:
            switch (subStateId)
                {
                    case SUB_STATE_START:
                       
                        setData(stateId, subStateId, ENTER_INHALE_MODE_STARTDESIRED_OUTPUT, ENTER_INHALE_MODE_STARTMAXMOTORSPEED, ENTER_INHALE_MODE_START_DURATION_IN_MS, data);
                        break;
                    case SUB_STATE_REPEAT:
                        setData(stateId, subStateId, ENTER_INHALE_MODE_REPEATDESIRED_OUTPUT, ENTER_INHALE_MODE_REPEATMAXMOTORSPEED, ENTER_INHALE_MODE_REPEAT_DURATION_IN_MS, data);

                        break;
                    case SUB_STATE_LEAVE:
                        setData(stateId, subStateId, ENTER_INHALE_MODE_LEAVEDESIRED_OUTPUT, ENTER_INHALE_MODE_LEAVEMAXMOTORSPEED, ENTER_INHALE_MODE_LEAVE_DURATION_IN_MS, data);
                        break;
                    default:
                        break;
                };
            break;
        case RUN_INHALE_MODE:
            switch (subStateId)
                {
                    case SUB_STATE_START:
                        data->desiredOutput = RUN_INHALE_MODE_STARTDESIRED_OUTPUT;
                        data->motorSpeed = RUN_INHALE_MODE_STARTMAXMOTORSPEED;
                        data->maxRunTimeInMs = RUN_INHALE_MODE_START_DURATION_IN_MS;
                        break;
                    case SUB_STATE_REPEAT:
                        data->desiredOutput = RUN_INHALE_MODE_REPEATDESIRED_OUTPUT;
                        data->motorSpeed = RUN_INHALE_MODE_REPEATMAXMOTORSPEED;
                        data->maxRunTimeInMs = RUN_INHALE_MODE_REPEAT_DURATION_IN_MS;
                        break;
                    case SUB_STATE_LEAVE:
                        data->desiredOutput = RUN_INHALE_MODE_LEAVEDESIRED_OUTPUT;
                        data->motorSpeed = RUN_INHALE_MODE_LEAVEMAXMOTORSPEED;
                        data->maxRunTimeInMs = RUN_INHALE_MODE_LEAVE_DURATION_IN_MS;
                        break;
                    default:
                        break;
                };
            break;
        case EXIT_INHALE_MODE:
            switch (subStateId)
                {
                    case SUB_STATE_START:
                        data->desiredOutput = EXIT_INHALE_MODE_STARTDESIRED_OUTPUT;
                        data->motorSpeed = EXIT_INHALE_MODE_STARTMAXMOTORSPEED;
                        data->maxRunTimeInMs = EXIT_INHALE_MODE_START_DURATION_IN_MS;
                        break;
                    case SUB_STATE_REPEAT:
                        data->desiredOutput = EXIT_INHALE_MODE_REPEATDESIRED_OUTPUT;
                        data->motorSpeed = EXIT_INHALE_MODE_REPEATMAXMOTORSPEED;
                        data->maxRunTimeInMs = EXIT_INHALE_MODE_REPEAT_DURATION_IN_MS;
                        break;
                    case SUB_STATE_LEAVE:
                        data->desiredOutput = EXIT_INHALE_MODE_LEAVEDESIRED_OUTPUT;
                        data->motorSpeed = EXIT_INHALE_MODE_LEAVEMAXMOTORSPEED;
                        data->maxRunTimeInMs = EXIT_INHALE_MODE_LEAVE_DURATION_IN_MS;
                        break;
                    default:
                        break;
                };
            break;
        case ENTER_EXHALE_MODE:
            switch (subStateId)
                {
                    case SUB_STATE_START:
                        data->desiredOutput = ENTER_EXHALE_MODE_STARTDESIRED_OUTPUT;
                        data->motorSpeed = ENTER_EXHALE_MODE_STARTMAXMOTORSPEED;
                        data->maxRunTimeInMs = ENTER_EXHALE_MODE_START_DURATION_IN_MS;
                        break;
                    case SUB_STATE_REPEAT:
                        data->desiredOutput = ENTER_EXHALE_MODE_REPEATDESIRED_OUTPUT;
                        data->motorSpeed = ENTER_EXHALE_MODE_REPEATMAXMOTORSPEED;
                        data->maxRunTimeInMs = ENTER_EXHALE_MODE_REPEAT_DURATION_IN_MS;
                        break;
                    case SUB_STATE_LEAVE:
                        data->desiredOutput = ENTER_EXHALE_MODE_LEAVEDESIRED_OUTPUT;
                        data->motorSpeed = ENTER_EXHALE_MODE_LEAVEMAXMOTORSPEED;
                        data->maxRunTimeInMs = ENTER_EXHALE_MODE_LEAVE_DURATION_IN_MS;
                        break;
                    default:
                        break;
                };
            break;
        case RUN_EXHALE_MODE:
            switch (subStateId)
                {
                    case SUB_STATE_START:
                        data->desiredOutput = RUN_EXHALE_MODE_STARTDESIRED_OUTPUT;
                        data->motorSpeed = RUN_EXHALE_MODE_STARTMAXMOTORSPEED;
                        data->maxRunTimeInMs = RUN_EXHALE_MODE_START_DURATION_IN_MS;
                        break;
                    case SUB_STATE_REPEAT:
                        data->desiredOutput = RUN_EXHALE_MODE_REPEATDESIRED_OUTPUT;
                        data->motorSpeed = RUN_EXHALE_MODE_REPEATMAXMOTORSPEED;
                        data->maxRunTimeInMs = RUN_EXHALE_MODE_REPEAT_DURATION_IN_MS;
                        break;
                    case SUB_STATE_LEAVE:
                        data->desiredOutput = RUN_EXHALE_MODE_LEAVEDESIRED_OUTPUT;
                        data->motorSpeed = RUN_EXHALE_MODE_LEAVEMAXMOTORSPEED;
                        data->maxRunTimeInMs = RUN_EXHALE_MODE_LEAVE_DURATION_IN_MS;
                        break;
                    default:
                        break;
                };
            break;
        case EXIT_EXHALE_MODE:
            switch (subStateId)
                {
                    case SUB_STATE_START:
                        data->desiredOutput = EXIT_EXHALE_MODE_STARTDESIRED_OUTPUT;
                        data->motorSpeed = EXIT_EXHALE_MODE_STARTMAXMOTORSPEED;
                        data->maxRunTimeInMs = EXIT_EXHALE_MODE_START_DURATION_IN_MS;
                        break;
                    case SUB_STATE_REPEAT:
                        data->desiredOutput = EXIT_EXHALE_MODE_REPEATDESIRED_OUTPUT;
                        data->motorSpeed = EXIT_EXHALE_MODE_REPEATMAXMOTORSPEED;
                        data->maxRunTimeInMs = EXIT_EXHALE_MODE_REPEAT_DURATION_IN_MS;
                        break;
                    case SUB_STATE_LEAVE:
                        data->desiredOutput = EXIT_EXHALE_MODE_LEAVEDESIRED_OUTPUT;
                        data->motorSpeed = EXIT_EXHALE_MODE_LEAVEMAXMOTORSPEED;
                        data->maxRunTimeInMs = EXIT_EXHALE_MODE_LEAVE_DURATION_IN_MS;
                        break;
                    default:
                        break;
                };
            break;
        case IDEL_MODE:
            /* code */
            break;
        default:
            break;
    };
}