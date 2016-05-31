/*
 * iodefines.h
 *
 * Created: 29.03.2016 19:53:16
 *  Author: HSO
 */ 


#ifndef IODEFINES_H_
#define IODEFINES_H_

#include <avr/io.h>
/************************************************************************/
/* @Define outputs                                                                     */
/************************************************************************/

#define RELAY_PORT PORTF

#define PIN_MOTOR 0
#define PIN_PUMP 1

#define PIN_FAN 2
#define PIN_VALVE 3

#define PIN_AUX1 4
#define PIN_AUX2 5
#define PIN_AUX3 6
#define PIN_AUX4 7

/************************************************************************/
/* @ set relays states                                                                     */
/************************************************************************/
#define MOTOR_ON do{bit_set(RELAY_PORT,PIN_MOTOR);}while(0)
#define MOTOR_OFF do{bit_clear(RELAY_PORT,PIN_MOTOR);}while(0)

#define PUMP_ON do{bit_set(RELAY_PORT,PIN_PUMP);}while(0)
#define PUMP_OFF do{bit_clear(RELAY_PORT,PIN_PUMP);}while(0)

#define FAN_ON bit_set(RELAY_PORT,PIN_FAN)
#define FAN_OFF bit_clear(RELAY_PORT,PIN_FAN)

#define VALVE_ON bit_set(RELAY_PORT,PIN_VALVE)
#define VALVE_OFF bit_clear(RELAY_PORT,PIN_VALVE)

#define ULTRA_ON bit_set(RELAY_PORT,PIN_AUX1)
#define ULTRA_OFF bit_clear(RELAY_PORT,PIN_AUX1)

#define AUX2_ON bit_set(RELAY_PORT,PIN_AUX2)
#define AUX2_OFF bit_clear(RELAY_PORT,PIN_AUX2)

#define AUX3_ON bit_set(RELAY_PORT,PIN_AUX3)
#define AUX3_OFF bit_clear(RELAY_PORT,PIN_AUX3)

#define AUX4_ON bit_set(RELAY_PORT,PIN_AUX4)
#define AUX4_OFF bit_clear(RELAY_PORT,PIN_AUX4)




#endif /* IODEFINES_H_ */