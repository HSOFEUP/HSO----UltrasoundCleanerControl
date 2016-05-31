/*
 * pwm.h
 *
 * Created: 28.03.2016 14:51:05
 *  Author: HSO
 */ 


#ifndef PWM_H_
#define PWM_H_

#include <inttypes.h>


#define MAX_PWM_VALUE 2560




extern void configPWM(void);
extern void setMotorSpeed(uint8_t duty);


#endif /* PWM_H_ */