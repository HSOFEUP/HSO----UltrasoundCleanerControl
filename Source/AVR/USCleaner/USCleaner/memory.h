/*
 * memory.h
 *
 * Created: 26.03.2016 14:16:59
 *  Author: HSO
 */ 


#ifndef MEMORY_H_
#define MEMORY_H_


/************************************************************************/
/* EEPROM AREA                                                                     */
/************************************************************************/
// pid variables
typedef struct{
	uint8_t initEeprom;
	// Modes
	uint16_t washingMaxTime;
	uint16_t washingMinTime;
	uint16_t drywingMaxTime;
	uint16_t drywingMinTime;
	
	uint16_t rotationMax;
	uint16_t rotationMin;
	
}eestruct_t;


// TIMES
#define WASHING_MAX_TIME_DEFAULT 180
#define WASHING_MIN_TIME_DEFAULT 20
#define DRYING_MAX_TIME_DEFAULT 180
#define DRYING_MIN_TIME_DEFAULT 10

#define ROTATION_MAX_DEFAULT 2000
#define ROTATION_MIN_DEFAULT 200


extern void paramLoadDefaultParameters(void);
extern void eepromSetDefaultParameters(void);
extern void paramLoadFromEeprom(void);
extern void paramSavetoEeprom(void);


#endif /* MEMORY_H_ */