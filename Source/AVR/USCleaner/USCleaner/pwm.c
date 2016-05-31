/*
 * pwm.c
 *
 * Created: 26.03.2016 14:55:59
 *  Author: HSO
 */ 
#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#include "pwm.h"
#include <avr/io.h>


/************************************************************************/
/* PWM ZONE                                                                */
/************************************************************************/
void configPWM(void){
	//The formula for Fast PWM
	//F(PWM) = F(Clock)/(N*(1+TOP)
	// max 245 Hz at 16mhz with full top
	
	// fast pwm, mode 14, noninverted output, prescaler 1024 //32
	TCCR1A = (1<<WGM11)|(1<<COM1A1)|(1<<COM1B1);
	//TCCR1B = (1<<WGM12)|(1<<WGM13)|(1<<CS10)|(1<<CS12);
	//TCCR1B = (1<<WGM12)|(1<<WGM13)|(1<<CS01) |  (1<<CS00);
	TCCR1B = (1<<WGM12)|(1<<WGM13)|(1<<CS00);
	
	//ICR1 = (F_CPU[MHz] * periode[us] / prescaler) - 1
	ICR1   = MAX_PWM_VALUE;           // periode cca 0.5s / 8 MHz
	
	DDRB   = (1<<5);//|(1<<6);  // output pins OC1A, OC1B   Mega16,32
	
	OCR1A = 0;          // duty cycle = 20 percent
	//OCR1B = 0;         // duty cycle = 80 percent
	
	
	
}


/************************************************************************/
/* @ set motor Speed                                                                     */
/************************************************************************/
void setMotorSpeed(uint8_t duty){
	
	uint16_t pwmVal;
	if(duty >100){
		pwmVal = MAX_PWM_VALUE;
	}else{
		pwmVal = (uint16_t)((MAX_PWM_VALUE*(uint16_t)duty)/(uint16_t) 100);
	}

	OCR1A =pwmVal;
	
}
