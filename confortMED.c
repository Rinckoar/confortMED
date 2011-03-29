/******************************************************************************
 *                                                                            *
 *	      Juan Felipe Restrepo <jf.restrepo.rinckoar@gmail.com>           *
 *----------------------------------------------------------------------------*
 *				                                              *
 *	PORTABLE DEVICE TO MESSURE PRESSURE PATTERNS IN MODELED SEATS         *
 *				                                              *
 *				ConfortMED                                    *
 *                                                                            *
 *	AUTHOR:		Juan Felipe Restrepo Rinckoar.                        *
 *	ADVISOR:	Robinson Torrez Ph.D.                                 *
 *	                                                                      *
 *		Escuela de Ingenieria de Antioquia-Universidad CES            *
 *			                                                      *
 *			   MEDELLIN-COLOMBIA                                  *
 *			       	  2010                                        *
 *---------------------------------------------------------------------------*/

#include "confortMED.h"
#include "manager.c"
#include <stdlibm.h>

sensor s;
unsigned int main_mode,units;



/*********************************************
 *  		INTERRUPTS		     *
 ********************************************/		

//		EXT INTERRUPT 
//-------------------------------------------	
#int_EXT
void ext()
{ 
	if(main_mode==MAIN_ON){
		main_mode=MAIN_PAUSE;
		PORTE=MAIN_PAUSE;
		return;
	}
	if(main_mode==MAIN_PAUSE){
		main_mode=MAIN_ON;
		PORTE=MAIN_ON;
		return;
	}
}

//		RB INTERRUPT 
//-------------------------------------------	
#INT_RB
void Interrupcion_RB()
{
	int button;
	delay_ms(50);						
	button=swap( PORTB & 0b00110000);			
	
	switch (button){
		
		case 1:{						
			       	main_mode=set_lcd(main_mode);
				PORTE=main_mode;
			      	break;
		      }
		case 2:{					
				if(main_mode==MAIN_ON){
					units=~units;
					break;
				}
				if(main_mode==MAIN_OFF){
					main_mode=MAIN_CALIB;
					PORTE=main_mode;
					break;
				}
		      }

	}
  	#asm movf PORTB,0 #endasm				
}

/*********************************************
 *  		  MAIN CODE		     *
 ********************************************/		

//		MAIN INIT 
//-------------------------------------------	
static signed int main_init()
{

	signed int check=0;	
			
	port_b_pullups(ON);					
	set_tris_b(0xFF);					
	set_tris_e(0x00);
	set_tris_a(0xFF);
	PORTE=0xFF;
	PORTB=0x00;

	s=(sensor)malloc(4*sizeof(_sensor));
	if(s==NULL){	
		check=-1;
		goto exit;
	}
	if(manager_init(s)){
		check=-2;
		goto exit;
	}
exit:

	if (check<0){
		return check;
	}
	else{
		PORTE=0;
		delay_ms(1000);
		enable_interrupts(int_RB);				
		ext_int_edge(L_TO_H);	
		enable_interrupts(int_EXT);
		enable_interrupts(global);
		main_mode=MAIN_OFF;
		units=KPA;
		PORTE=MAIN_OFF;
		return check;
	}

}

//		MAIN ERROR DISPLAY
//-------------------------------------------	
static void main_error(int er)
{
	char H[]="ALLOCATION ERROR";
	char H1[]="CHARGING INTERNAL \n     VALUES ERROR";

	glcd_init(ON);
	switch(er){
		case -1:{	
				glcd_text57(20,20,H, 1, ON);
				break;
			}
		case -2:{	
				glcd_text57(20,20, H1, 1, ON);
				break;
			}
	}
	delay_ms(5000);
	glcd_fillScreen(OFF);
}

//		MAIN 
//-------------------------------------------	
void main(void)
{
	signed int er;	
	er=main_init();
	if(er<0)
		goto error;
	
	while(1){
		
		if(main_mode==MAIN_ON)
			main_menu(s,units);
		
		while(main_mode==MAIN_PAUSE){
		}
		while(main_mode==MAIN_CALIB){
			main_mode=calib_menu(s);
			PORTE=main_mode;
		}

	}




error:
	main_error(er);
}

