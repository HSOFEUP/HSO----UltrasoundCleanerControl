/*
 * statemachine.c
 *
 * Created: 26.03.2016 14:48:24
 *  Author: HSO
 */ 

#include <inttypes.h>
#include "statemachine.h"
#include "pwm.h"
#include "iodefines.h"
#include "libs/utils/bit_tools.h"
#include "libs/usart/usart.h"

#define DEBUG_STATE


/************************************************************************/
/* Globals VARIABLES                                                                     */
/************************************************************************/
extern uint16_t washingMaxTime;
extern uint16_t washingMinTime;
extern uint16_t drywingMaxTime;
extern uint16_t drywingMinTime;

extern uint16_t rotationMax;
extern uint16_t rotationMin;


extern uint16_t washingTime;
extern uint16_t dryingTime;
extern uint8_t rotatioSpeedPercentage;

extern uint16_t washingCurrent;
extern uint16_t dryingCurrent;




extern uint8_t flagBeep;
extern uint8_t flagSaveParams;


extern uint8_t diskInSensor;
extern uint8_t filledTankSensor;
extern uint8_t emptyReservoirSensor;
extern uint8_t emptyTankSensor;


extern uint8_t stateMode;
extern uint8_t stateProgram;
/************************************************************************/
/* State Machine Process                                                                    */
/************************************************************************/
void stateMachine(uint8_t sig){
	
	static uint16_t time;
	switch(stateMode){
		
		case STATE_IDLE:
			#ifdef DEBUG_STATE
			USART1_sendStr("STATE: STATE_IDLE\n\r");
			#endif
			
			washingCurrent=washingTime;
			dryingCurrent =dryingTime;
			
			if(emptyReservoirSensor==SENSOR_ACTIVE){
				if(diskInSensor == SENSOR_ACTIVE){
					stateMode=STATE_READY;
				}
			}else{
				 stateMode = STATE_EMPTY;
			}
		break;
		
		
		case STATE_READY:
			#ifdef DEBUG_STATE
			USART1_sendStr("STATE: STATE_READY\n\r");
			#endif
			washingCurrent=washingTime;
			dryingCurrent =dryingTime;
			
			if(!diskInSensor == SENSOR_ACTIVE){
				stateMode = STATE_IDLE;
			
			}
			else if(sig == START_BUTTON && diskInSensor == SENSOR_ACTIVE){
				stateMode = STATE_FILLING;
			}
		
		break;
		
		case STATE_FILLING:
			#ifdef DEBUG_STATE
			USART1_sendStr("STATE: STATE_FILLING\n\r");
			#endif
			
			if(filledTankSensor==SENSOR_ACTIVE){
				
				stateMode = STATE_WASHING;
			}
		break;
		
		case STATE_WASHING:
			#ifdef DEBUG_STATE
			USART1_sendStr("STATE: STATE_WASHING\n\r");
			#endif
			
			washingCurrent--;
			if(washingCurrent <= 0){
				stateMode = STATE_UNFILLING;
				
			}
		break;
		
		case STATE_UNFILLING:
			#ifdef DEBUG_STATE
			USART1_sendStr("STATE: STATE_UNFILLING\n\r");
			#endif
			if(!emptyTankSensor == SENSOR_ACTIVE){
				if(dryingTime < drywingMinTime){
					stateMode = STATE_IDLE;
				}else{
					dryingCurrent = dryingTime;
					stateMode = STATE_DRYING;	
				}
				
			}
		break;
		
		case STATE_DRYING:
			#ifdef DEBUG_STATE
			USART1_sendStr("STATE: STATE_DRYING\n\r");
			#endif
			
			dryingCurrent--;
			if(dryingCurrent <= 0){
				stateMode = STATE_FINISHED;
			}
		break;
		
		case STATE_EMPTY:
			#ifdef DEBUG_STATE
			USART1_sendStr("STATE: STATE_EMPTY\n\r");
			#endif

			// activate beep
			flagBeep=1;
			if(!emptyReservoirSensor==SENSOR_ACTIVE){
				stateMode = STATE_IDLE;
				flagBeep=0;	
			} // SET CLEAR
		break;
		
		case STATE_FINISHED:
			#ifdef DEBUG_STATE
			USART1_sendStr("STATE: STATE_FINISHED\n\r");
			#endif
		
			if(!diskInSensor==SENSOR_ACTIVE){
				stateMode = STATE_IDLE;
			}
		break;
		
		default:
			stateMode = STATE_IDLE;
		break;
	}
	
}





/************************************************************************/
/* @State Machine for Program params                                                                     */
/************************************************************************/
void stateMachineProgram(uint8_t sig){
	
	switch(stateProgram){
		
		case PRG_STATE_RUN:
			

			if(sig == PRG_BUTTON) stateProgram = PRG_STATE_ROT_SET_MAX;
		break;
		
		case PRG_STATE_ROT_SET_MAX:
			if(sig == ENTER_BUTTON) stateProgram = PRG_STATE_ROT_SET_MIN;
			if(sig == UP_BUTTON); // SET UP
			if(sig == DOWN_BUTTON); // SET DOWN
			if(sig == ESC_BUTTON) stateProgram = PRG_STATE_SAVE_PARAMS;
		break;
		
		case PRG_STATE_ROT_SET_MIN:
			if(sig == ENTER_BUTTON) stateProgram = PRG_STATE_WAS_SET_MAX;
			if(sig == UP_BUTTON); // SET UP
			if(sig == DOWN_BUTTON); // SET DOWN
			if(sig == ESC_BUTTON) stateProgram = PRG_STATE_SAVE_PARAMS;
		break;
		
		case PRG_STATE_WAS_SET_MAX:
			if(sig == ENTER_BUTTON) stateProgram = PRG_STATE_WAS_SET_MIN;
			if(sig == UP_BUTTON); // SET UP
			if(sig == DOWN_BUTTON); // SET DOWN
			if(sig == ESC_BUTTON) stateProgram = PRG_STATE_SAVE_PARAMS;
		break;
		
		
		case PRG_STATE_WAS_SET_MIN:
			if(sig == ENTER_BUTTON) stateProgram = PRG_STATE_DRY_SET_MAX;
			if(sig == UP_BUTTON); // SET UP
			if(sig == DOWN_BUTTON); // SET DOWN
			if(sig == ESC_BUTTON) stateProgram = PRG_STATE_SAVE_PARAMS;
		break;
		
		case PRG_STATE_DRY_SET_MAX:
			if(sig == ENTER_BUTTON) stateProgram = PRG_STATE_WAS_SET_MIN;
			if(sig == UP_BUTTON); // SET UP
			if(sig == DOWN_BUTTON); // SET DOWN
			if(sig == ESC_BUTTON) stateProgram = PRG_STATE_SAVE_PARAMS;
		break;
		
		
		case PRG_STATE_DRY_SET_MIN:
			if(sig == ENTER_BUTTON) stateProgram = PRG_STATE_SAVE_PARAMS;
			if(sig == UP_BUTTON); // SET UP
			if(sig == DOWN_BUTTON); // SET DOWN
			if(sig == ESC_BUTTON) stateProgram = PRG_STATE_SAVE_PARAMS;
		break;
		
		
		case PRG_STATE_SAVE_PARAMS:
			flagSaveParams++;
			stateProgram = PRG_STATE_RUN;
		break;
		
		default:
			stateProgram= PRG_STATE_RUN;
		break;
	}
}






/************************************************************************/
/* Set Outputs                                                                      */
/************************************************************************/
void decodeOutputs(uint8_t state){
	
	switch(stateMode){
		
		case STATE_IDLE:
			setMotorSpeed(0);
			// DISABLE ALL
			MOTOR_OFF;
			PUMP_OFF;
			FAN_OFF;
			VALVE_ON;
			
			ULTRA_OFF;
			
			
			
		break;
		
		case STATE_FILLING:
			setMotorSpeed(0);
			// DISABLE ALL
			MOTOR_OFF;
			PUMP_ON;
			FAN_OFF;
			VALVE_OFF;
			ULTRA_OFF;
		break;
		
		case STATE_WASHING:
			
			setMotorSpeed(rotatioSpeedPercentage);
			// DISABLE ALL
			MOTOR_ON;
			PUMP_OFF;
			FAN_OFF;
			VALVE_OFF;
			ULTRA_ON;
		break;
		
		case STATE_UNFILLING:
			setMotorSpeed(0);
			// DISABLE ALL
			MOTOR_OFF;
			PUMP_OFF;
			FAN_OFF;
			VALVE_ON;
			ULTRA_OFF;
		break;
		
		case STATE_DRYING:
			setMotorSpeed(rotatioSpeedPercentage);
			// DISABLE ALL
			MOTOR_ON;
			PUMP_OFF;
			FAN_ON;
			VALVE_OFF;
			ULTRA_OFF;
		break;
		
		case STATE_EMPTY:
			setMotorSpeed(0);
			// DISABLE ALL
			MOTOR_ON;
			PUMP_OFF;
			FAN_ON;
			VALVE_ON;
			ULTRA_OFF;
		break;
		
		case STATE_FINISHED:
			setMotorSpeed(0);
			// DISABLE ALL
			MOTOR_ON;
			PUMP_OFF;
			FAN_ON;
			VALVE_OFF;
			ULTRA_OFF;
		break;
		
	}
	
}