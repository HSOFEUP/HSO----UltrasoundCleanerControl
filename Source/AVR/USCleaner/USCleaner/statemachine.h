/*
 * statemachine.h
 *
 * Created: 26.03.2016 14:48:41
 *  Author: HSO
 */ 


#ifndef STATEMACHINE_H_
#define STATEMACHINE_H_

#define STATE_IDLE 0
#define STATE_WASHING 1
#define STATE_DRYING 2
#define STATE_EMPTY 3
#define STATE_BEEP 4
#define STATE_FILLING 5
#define STATE_FINISHED 6
#define STATE_READY 7
#define STATE_UNFILLING 8

#define START_BUTTON 1
#define ENTER_BUTTON 2
#define ESC_BUTTON 3
#define UP_BUTTON 4
#define DOWN_BUTTON 5

#define PRG_BUTTON 6


#define EMPTY_SENSOR 2
#define FILLED_SENSOR 3
#define DISK_SENSOR 4

#define SENSOR_ACTIVE 0



#define PRG_STATE_RUN 1
#define PRG_STATE_DRY_SET_MAX 2
#define PRG_STATE_DRY_SET_MIN 3
#define PRG_STATE_WAS_SET_MAX 4
#define PRG_STATE_WAS_SET_MIN 5
#define PRG_STATE_ROT_SET_MAX 6
#define PRG_STATE_ROT_SET_MIN 7
#define PRG_STATE_SAVE_PARAMS 8



extern void stateMachine(uint8_t sig);
extern void decodeOutputs(uint8_t state);
extern void stateMachineProgram(uint8_t sig);


#endif /* STATEMACHINE_H_ */