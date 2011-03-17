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
#bit  B0 = 	PORTB.0


#elif defined(__PCH__)
#include <18F452.h>
#device ADC=10
#fuses HS,NOWDT,NOPROTECT,NOLVP
#use delay(clock=20000000)
#Byte PORTA = 	0xF80								
#Byte PORTB = 	0xF81
#Byte PORTE = 	0xF84
#bit  B0 = 	PORTB.0
//#define FAST_GLCD    			// Try commenting this out to see the differences
#endif

#define NCH	4

#define MMHG 	1
#define KPA 	0

#define MAIN_OFF	6
#define MAIN_ON		5
#define MAIN_PAUSE	3

#define KPA2MMHG	7.50061561	
#define AREA		71.480343488

typedef struct {
	int x,y,rx,ry;
	long adc;
	float m,b;
}_sensor;

typedef _sensor* sensor;

