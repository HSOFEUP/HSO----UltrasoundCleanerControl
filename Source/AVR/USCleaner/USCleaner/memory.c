/*
 * memory.c
 *
 * Created: 26.03.2016 14:17:13
 *  Author: HSO
 */ 

#include <avr/io.h>
#include <avr/eeprom.h>
#include "memory.h"

eestruct_t EEMEM eestruct_eemem;
eestruct_t eestruct_var;


extern uint16_t washingMaxTime;
extern uint16_t washingMinTime;
extern uint16_t drywingMaxTime;
extern uint16_t drywingMinTime;

extern uint16_t rotationMax;
extern uint16_t rotationMin;


/************************************************************************/
/* load default parameters                                                                     */
/************************************************************************/
void paramLoadDefaultParameters(void){
	
	// STATES DEFAULT
	washingMaxTime = WASHING_MAX_TIME_DEFAULT;
	washingMinTime = WASHING_MIN_TIME_DEFAULT;
	drywingMaxTime = DRYING_MAX_TIME_DEFAULT;
	drywingMinTime = DRYING_MIN_TIME_DEFAULT;
	
	rotationMax = ROTATION_MAX_DEFAULT;
	rotationMin = ROTATION_MIN_DEFAULT;
	
}


/************************************************************************/
/* @ set initial values to eeprom  if nothin there yet                                                                   */
/************************************************************************/
void eepromSetDefaultParameters(){
	eestruct_var.initEeprom=1; // emprom init
	
	// STATUS
	eestruct_var.washingMaxTime = WASHING_MAX_TIME_DEFAULT;
	eestruct_var.washingMinTime = WASHING_MIN_TIME_DEFAULT;
	eestruct_var.drywingMaxTime = DRYING_MAX_TIME_DEFAULT;
	eestruct_var.drywingMinTime = DRYING_MIN_TIME_DEFAULT;
	
	eestruct_var.rotationMax = ROTATION_MAX_DEFAULT;
	eestruct_var.rotationMin = ROTATION_MIN_DEFAULT;
	
	
	eeprom_write_block((const void*)&eestruct_var,(void*)&eestruct_eemem,sizeof(eestruct_t));
	
}


/************************************************************************/
/* @restore to EEPROM                                                                     */
/************************************************************************/
void paramLoadFromEeprom(){
	//uint8_t temp=0;
	// read from emprom
	eeprom_read_block((void*)&eestruct_var, (const void*)&eestruct_eemem,sizeof(eestruct_t));
	
	// test the fits field to check if it was written else use default and load
	if((eestruct_var.initEeprom &0xFF) ==0xFF){
		eepromSetDefaultParameters();
		paramLoadDefaultParameters();
		
	}
	else{
		// write to the global variables
		washingMaxTime = eestruct_var.washingMaxTime;
		washingMinTime = eestruct_var.washingMinTime;
		drywingMaxTime = eestruct_var.drywingMaxTime;
		drywingMinTime = eestruct_var.drywingMinTime;
		
		rotationMax = eestruct_var.rotationMax;
		rotationMin = eestruct_var.rotationMin;
		
	}
	
	
	
}

/************************************************************************/
/* @read from EEPROM                                                                     */
/************************************************************************/
void paramSavetoEeprom(){
	
	
	// save paramenetrs on the run
	eestruct_var.initEeprom=1; // emprom init
	
	eestruct_var.washingMaxTime = washingMaxTime;
	eestruct_var.washingMinTime = washingMinTime;
	eestruct_var.drywingMaxTime = drywingMaxTime;
	eestruct_var.drywingMinTime = drywingMinTime;
	
	eestruct_var.rotationMax = rotationMax;
	eestruct_var.rotationMin = rotationMin;
	
	eeprom_write_block((const void*)&eestruct_var,(void*)&eestruct_eemem,sizeof(eestruct_t));
	
	
}
