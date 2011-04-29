/******************************************************************************
 *                                                                            *
 *	2011-04-28  Juan Felipe Restrepo <jf.restrepo.rinckoar@gmail.com>     *
 *----------------------------------------------------------------------------*
 *				                                              *
 *	PORTABLE DEVICE TO MEASSURE PRESSURE PATTERNS IN MODELED SEATS         *
 *				                                              *
 *				ConfortMED                                    *
 *                                                                            *
 *	AUTHOR:		Juan Felipe Restrepo Rinckoar.                        *
 *	ADVISOR:	Robinson Torres Ph.D.                                 *
 *	                                                                      *
 *		Escuela de Ingenieria de Antioquia-Universidad CES            *
 *			                                                      *
 *			   MEDELLIN-COLOMBIA                                  *
 *			       	  2011                                        *
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
	if(main_mode==MAIN_ON){					// switch between the MAIN_ON mode and
		main_mode=MAIN_PAUSE;				// the MAIN_PAUSE mode.
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
#int_RB
void Interrupcion_RB()
{
	int button;
	delay_ms(50);						
	button=swap( PORTB & 0b00110000);			
	
	switch (button){
		
		case 1:{						
			       	main_mode=set_lcd(main_mode);	// switch between the MAIN_ON mode and 
				PORTE=main_mode;		// the MAIN_OFF mode.
			      	break;
		      }
		case 2:{					
				if(main_mode==MAIN_ON){		// switch the unints (kPa or mmHg).	
					units=~units;
					break;
				}
				if(main_mode==MAIN_OFF){	// enter to the calibration mode.	
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
//output:
//	check	Varieble used to check if 
//		everything was correctly 
//		initialized or not.
//description:
//	function to initized the device, it
//	allocates in memory the sensors 
//	structure array, set the ports
//	function and call the fuction to
//	init. the menu manager.
//-------------------------------------------	
static signed int main_init()
{
	char text[]="";
	signed int check=0;	
			
	port_b_pullups(ON);					
	set_tris_b(0xFF);					
	set_tris_e(0x00);
	set_tris_a(0xFF);
	PORTE=0xFF;
	PORTB=0x00;

	s=(sensor)malloc(NCH*sizeof(_sensor));
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
		glcd_init(ON);
		PORTE=0;
		sprintf(text,"ConfortMED");
		glcd_text57(10,5,text,2,1);
		sprintf(text,"Juan Felipe Restrepo");
		glcd_text57(6,30,text,1,1);	
		sprintf(text,"KIRON-EIA");
		glcd_text57(40,45,text,1,1);
		sprintf(text,"v.1.0");
		glcd_text57(96,56,text,1,1);
#ifdef FAST_GLCD
		glcd_update();
#endif	
		delay_ms(5000);
		glcd_fillScreen(OFF);
#ifdef FAST_GLCD
		glcd_update();
#endif	
		glcd_init(OFF);
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
//input:
//	er	Variable to select the kind
//		of error.
//description:
//	Print in the GLCD an error message
//	if the init. routine fail.
//-------------------------------------------	
static void main_error(int er)
{
	char text[]="";
	
	glcd_init(ON);
	switch(er){
		case -1:{	
				sprintf(text,"ALLOCATION ERROR");
				glcd_text57(20,20,text, 1, ON);
				break;
			}
		case -2:{	
				sprintf(text,"LOADING INTERNAL \n     VALUES ERROR");
				glcd_text57(20,20, text, 1, ON);
				break;
			}
	}
#ifdef FAST_GLCD
	glcd_update();
#endif	
	delay_ms(5000);
	glcd_fillScreen(OFF);
#ifdef FAST_GLCD
	glcd_update();
#endif	
}

//		MAIN 
//description:
//	main routine.
//-------------------------------------------	
void main(void)
{
	signed int er;	
			
	er=main_init();						// call the init. function code.
	if(er<0)						// checks if there was any error during the init.
		goto error;
	
	while(1){
		
		if(main_mode==MAIN_ON)				// call the function to attend the MAIN_ON operation mode.
			main_menu(s,units);
		
		while(main_mode==MAIN_PAUSE){			// attend the MAIN_PAUSE mode.
		}

		while(main_mode==MAIN_CALIB){			// call the function to attend the MAIN_CALIB mode.
			main_mode=calib_menu(s);
			PORTE=main_mode;
		}
	
	}

error:								// call the function to attend the init errors.
	main_error(er);
}
