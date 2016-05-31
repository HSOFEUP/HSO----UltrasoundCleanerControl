/*
 * USCleaner.c
 *
 * Created: 25.03.2016 14:16:21
 *  Author: HSO
 */ 


#define F_CPU 16000000UL



#include <avr/io.h>
#include <stdlib.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <inttypes.h>


#include "libs/adc/adc_analog.h"
#include "libs/usart/usart.h"
#include "libs/timer/timer_utils.h"
#include "libs/utils/my_utils.h"
#include "libs/utils/bit_tools.h"
#include "libs/lcds/hd774x.h"

#include "pwm.h"
#include "memory.h"
#include "lcdmessages.h"
#include "statemachine.h"



/************************************************************************/
/* GLOBAL VARIABLES                                                                     */
/************************************************************************/
volatile uint16_t washingMaxTime = 200;
volatile uint16_t washingMinTime = 20;
volatile uint16_t drywingMaxTime = 150;
volatile uint16_t drywingMinTime = 10;

volatile uint16_t rotationMax;
volatile uint16_t rotationMin;


volatile uint16_t washingTime = 150;
volatile uint16_t dryingTime = 150;
volatile uint8_t rotatioSpeedPercentage = 50;
volatile uint8_t temperature = 20;

volatile uint16_t washingCurrent;
volatile uint16_t dryingCurrent;
volatile uint16_t rotationCurrent;


volatile uint8_t stateMode;
volatile uint8_t stateProgram;
volatile uint8_t flagBeep;
volatile uint8_t flagSaveParams;

volatile uint8_t diskInSensor;
volatile uint8_t filledTankSensor;
volatile uint8_t emptyReservoirSensor;
volatile uint8_t emptyTankSensor;



/************************************************************************/
/* SCHEDULER ZONE                                                                   */
/************************************************************************/
// Timer setup for control loop
// freq = (FCPU /prescaler) /timerscale
// timerscale timer0 8bits = 256
//
//#define TIMER0_TICK 0.001
#define TIMER0_SCHED_PRESC TIMER0_PRESC128
#define TIMER0_SCHED_RELOAD 125//// 125 timer inc=1ms

//#define SAMPLING_PERIOD 0.002 // 1ms base time
volatile uint16_t schedulerMaxCount=5000;

//Global Vars
volatile uint8_t flagTaskReadButtons = 0;
volatile uint8_t flagTaskReadAnalog = 0;
volatile uint8_t flagTaskStateUpdate=0;
volatile uint8_t flagTaskLcd=0;
volatile uint8_t flagTaskUpdateOutputs =0;


// Task periodicity 1ms base tick
volatile uint16_t taskReadButtonsPeriod = 100; // 100ms
volatile uint16_t taskReadAnalogPeriod = 1500;
volatile uint16_t taskStateUpdatePeriod =1000; // 1 seconds
volatile uint16_t taskLcdPeriod =500; // 1/5 seconds
volatile uint16_t taskUpdateOutputPeriod =100;


/************************************************************************/
/* @Scheduler Init                                                                     */
/************************************************************************/
void schedulerInit(void){
	
	TCCR0 |= TIMER0_SCHED_PRESC;
	TCCR0 |= TIMER0_WAVEFORM_MODE_CTC;
	OCR0  = TIMER0_SCHED_RELOAD; // timer count reload
	TIMSK |= (1<< OCIE0); // Enable timer compare interrupt
	
}



/************************************************************************/
/* @Config GPIO                                                                     */
/************************************************************************/
void configGPIO(void){
	
	
	
	DDRE =0xFF; // all outputs
	PORTF=0x00;
	
	DDRA = 0xF0; // low nible input
	PORTA = 0x0F; // enable pullpups
	
	DDRB = 0xD0; // low nible input
	PORTB = 0x1F; // enable pullpups
	
}



/////////////////////////////////////////////////////////////
////////////////// GIO ZONE /////////////////////////////////
/////////////////////////////////////////////////////////////
/************************************************************************/
/* @read the buttons and complement and clean                                                                     */
/************************************************************************/
uint8_t readButtons(void){

	return (~PINB) &0x00011111; // read the low nibble
}




/************************************************************************/
/* @decode Button                                                                     */
/************************************************************************/
uint8_t decodeButton(uint8_t button){
	button &=0x1F; // ensure
	switch (button){
		
		case 1: return START_BUTTON;
		break;
		
		case 2: return ESC_BUTTON;
		break;
		
		case 4: return ENTER_BUTTON;
		break;
		
		case 8: return UP_BUTTON;
		break;
		
		case 16: return DOWN_BUTTON;
		break;
		
		case 3: return PRG_BUTTON; // entering program
		break;
		
		default: return 0;
		break;
	}
}


/************************************************************************/
/* NUMBER CYCLES DEBOUNCE                                                                     */
/************************************************************************/
#define N_DEBOUNCE 3
/************************************************************************/
/* @debounce function                                                                     */
/************************************************************************/
uint8_t debounceKey(uint8_t codeNew){
	uint8_t key =0; // by default
	static uint8_t codeOld;
	static uint8_t keyCount;
	
	// ALREADY SOMETHIN PRESSED
	if(keyCount != 0){
		
		// IF SAME KEY and inside debounce times save
		if(codeNew == codeOld && keyCount <N_DEBOUNCE){ // ONLY IF EQUAL AND DEBOUNCE AVAILABLE
			codeOld =codeNew;
			keyCount++;
			// Reached debounce value and valid key
			if (keyCount == N_DEBOUNCE){
				key = codeNew; // ONLY HERE key is changed;
				
			}
		}
		
	}

	
	// INITIAL CONDITION
	if (keyCount == 0){
		codeOld = codeNew;
		keyCount++;
	}
	
	// if pressed key different reset (user must release the key for new run)
	if(codeNew != codeOld){
		codeOld =codeNew;
		keyCount =1;
	}
	return key;
}




/************************************************************************/
/* SENSOR PIN DEFINITION                                                                     */
/************************************************************************/
#define SENSORS_PORT PINA

#define PIN_SENSOR_START 0
#define PIN_SENSOR_TANK_FILLED 1
#define PIN_SENSOR_RESERVOIR_EMPTY 2
#define PIN_SENSOR_TANK_EMPTY 3


/************************************************************************/
/* @ read sensors and activate sensor flags vars                                                                     */
/************************************************************************/
int readSensors(void){

	diskInSensor = bit_read(SENSORS_PORT,PIN_SENSOR_START);
	filledTankSensor = bit_read(SENSORS_PORT,PIN_SENSOR_TANK_FILLED);
	emptyReservoirSensor = bit_read(SENSORS_PORT,PIN_SENSOR_RESERVOIR_EMPTY);
	emptyTankSensor = bit_read(SENSORS_PORT,PIN_SENSOR_TANK_EMPTY);
	
	return (1);
}


/************************************************************************/
/* @ Read All analogs channels                                                                     */
/************************************************************************/
void readAnalogs(void){
	
	washingTime = ADC_readAndWAIT(0);
	washingTime = (uint16_t) washingMaxTime * ((double)washingTime/(double)1024);
	dryingTime = ADC_readAndWAIT(1);
	dryingTime = (uint16_t) drywingMaxTime * ((double)dryingTime/(double)1024);
	// need conversiton
	//rotatioSpeedPercentage = ADC_readAndWAIT(3);
	
}



/* TODO:
	Control tempretrature
	Define max times
	Use pressure sensor and calibration procedure
*/


int main(void){
	
	USART1_config(USART1_MY_UBBRN,USART_DATA_FORMAT_8BITS|USART_STOP_BITS_1,USART_TRANSMIT_ENABLE|USART_RECEIVE_ENABLE| USART_INTERRUPT_ENABLE);
	//Config IO Ports
	configGPIO();
	//Init Scheduler
	schedulerInit();
	configPWM();
	setMotorSpeed(0);
	//Init ADC
	ADC_init(ADC_ENABLE,ADC_REF_VCC,ADC_MAX_FREQ); //CHECK PARAMS TO FILL IN
	
	//Init LCD
	LCD_init();
	LCD_clr();
	//LCD_sendString(&temp);
	
	showLcdSplash(); // show splash
	
	paramLoadFromEeprom(); // load parameters
	
	//enable interrupts
	sei();
	
	USART1_sendStr("HELLO\n\r");
	
	uint8_t codeSaved=0;
	uint8_t portVal = 0;
	uint8_t code =0;
    while(1){
		
        //TODO:: Please write your application code 
		
		// READ ANALOGS
		if(flagTaskReadAnalog){
			readAnalogs();
			
			flagTaskReadAnalog=0;	
		}
		
		// READ BUTTONS
		if(flagTaskReadButtons){
			readSensors();
			portVal = readButtons();
			code = decodeButton(portVal);
			code = debounceKey(code);
			
			if(code !=0x00){
				codeSaved = code;
				
			}
			
			stateMachineProgram(code);// lcd stuff
			flagTaskReadButtons=0;
		}
		
		// MACHINE STATE
		if(flagTaskStateUpdate){
			
			
			stateMachine(codeSaved);
			codeSaved =0x00;
			
			flagTaskStateUpdate=0;
		}
		
		// SET OUTPUS
		if(flagTaskUpdateOutputs){
			
			decodeOutputs(stateMode);
			flagTaskUpdateOutputs=0;
		}
		
		// SET LCD
		if(flagTaskLcd){
			
			showLcdMessagesVars(stateMode);
			flagTaskLcd=0;
		}
		
		
		// Beep the buzzer
		if(flagBeep){
			
			
			
		}
		
		
		
		// Save Parameters
		if(flagSaveParams){
			paramSavetoEeprom();
			flagSaveParams=0;
		}
    }
}