/*
 * lcdmessages.c
 *
 * Created: 28.03.2016 14:57:33
 *  Author: HSO
 */ 

#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#include <stdio.h>
#include <avr/delay.h>
#include "libs/lcds/hd774x.h"
#include "statemachine.h"



/************************************************************************/
/* Globals VARIABLES                                                                     */
/************************************************************************/
extern uint16_t washingMaxTime;
extern uint16_t washingMinTime;
extern uint16_t drywingMaxTime;
extern uint16_t drywingMinTime;

extern uint16_t rotationMax;
extern uint16_t rotationMin;

extern uint16_t beepTime;

extern uint16_t washingTime;
extern uint16_t dryingTime;
extern uint8_t rotatioSpeedPercentage;
extern uint8_t temperature;


extern uint16_t washingCurrent;
extern uint16_t dryingCurrent;




/************************************************************************/
/* LCD STUFF                                                                     */
/************************************************************************/
void showLcdSplash(void){
	
	LCD_clr();
	_delay_ms(200);
	LCD_gotoXY(4,0);
	LCD_sendString("Ultrasound");
	
	LCD_gotoXY(6,1);
	LCD_sendString("Cleaner");
	
	LCD_gotoXY(3,2);
	LCD_sendString("HSO & Valente");
	
	
	LCD_gotoXY(4,3);
	LCD_sendString("Version: 1.0");
	
	_delay_ms(2000); // to show the message
	
	LCD_clr();
	
}


/*****s*******************************************************************/
/* @ lcd messages                                                                      */
/************************************************************************/
void showLcdMessagesVars(uint8_t state){
	
	char lcdBuffer[20];
	
	LCD_gotoXY(0,0);
	LCD_sendString("STATUS: ");
	LCD_gotoXY(8,0);
	
	switch(state){
		
		case STATE_IDLE:
			LCD_sendString("IDLE      ");
		break;
		
		case STATE_READY:
			LCD_sendString("READY     ");
		break;
		
		case STATE_FILLING:
			LCD_sendString("FILLING   ");
		break;
		
		
		case STATE_WASHING:
			LCD_sendString("WASHING   ");
		break;
		
		case STATE_UNFILLING:
			LCD_sendString("DRAINING  ");
		break;
		
		
		case STATE_DRYING:
			LCD_sendString("DRYING    ");
		break;
		
		case STATE_EMPTY:
			LCD_sendString("EMPTY     ");
		break;
		
		default:
			
		break;
	}
	
	
	// program mode
	
	
	// convert to minutes
	LCD_gotoXY(0,1);
	sprintf(lcdBuffer,"Wash Time: %2d:%d",washingCurrent/60, washingCurrent%60);
	LCD_sendString(lcdBuffer);
	
	// CHECK TIME
	if(dryingTime >= drywingMinTime){
		LCD_gotoXY(0,2);
		sprintf(lcdBuffer,"Dry Time: %2d:%d   ",dryingCurrent/60, dryingCurrent%60);
		LCD_sendString(lcdBuffer);
			
	}else{
		LCD_gotoXY(0,2);
		sprintf(lcdBuffer,"Dry Time: DISABLED");
		LCD_sendString(lcdBuffer);
		
	}
	
	LCD_gotoXY(0,3);
	sprintf(lcdBuffer,"Speed: %2d%% T: %2d",rotatioSpeedPercentage,temperature);
	LCD_sendString(lcdBuffer);
	
	
	
	
}