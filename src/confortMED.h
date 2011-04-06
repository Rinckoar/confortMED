#ifndef CONFORTMED_H
#define CONFORTMED_H
#endif

#if defined(__PCM__)
#include <16F877a.h>
#device ADC=10
#fuses HS,NOWDT,NOPROTECT,NOLVP
#use delay(clock=20000000)
#Byte PORTA = 	0x05								
#Byte PORTB = 	0x06
#Byte PORTE = 	0x09
#bit  INCR = 	PORTB.0
#bit  DECR =	PORTB.4
#bit  OK =	PORTB.5

#elif defined(__PCH__)
#include <18F452.h>
#device ADC=10
#fuses HS,NOWDT,NOPROTECT,NOLVP
#use delay(clock=20000000)
#Byte PORTA = 	0xF80								
#Byte PORTB = 	0xF81
#Byte PORTE = 	0xF84
#bit  INCR = 	PORTB.0
#bit  DECR =	PORTB.4
#bit  OK =	PORTB.5
//#define FAST_GLCD    			// Try commenting this out to see the differences
#endif

#define NCH	4			// Number of sensors.
#define NKVAL 	6			// Number of point for calibration.

#define MMHG 	1			// Display values in mmHg.
#define KPA 	0

#define MAIN_OFF	6		// Constants to define the current program state.
#define MAIN_ON		5
#define MAIN_PAUSE	3
#define MAIN_CALIB	0

#define MAXERROR	0.1		// Maximum full scale error allowed to test the sensors.

#define KPA2MMHG	7.50061561	// constant to change Kpa to mmHg.
#define GR2KPA		9.807		// constant to change grf/mm^2 to Kpa.
#define AREA		71.480343488 	// Sensor active area.

typedef struct {			// sensor struct: 	x-> 	x coordinate to print the numeric pressure value. 
	int x,y,rx,ry,oldr;		//			y-> 	y coordinate to print the numeric pressure value.
	long adc;			//			rx->	x cordinate for the center of the circle.
	float m,b;			//			ry->	y cordinate for the center of the circle.
}_sensor;				// 			oldr->	value of the last circle drawed, this value is used to delete the past circle.
					//			adc->	analog/digital value.
					//			m-> 	value of the slope.
					//			b->	intercept with the y-axis.

typedef _sensor* sensor;		// define struct _sensor pointer.


