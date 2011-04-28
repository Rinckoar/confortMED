#include "control.c"
#include "calibration.c"

/*********************************************
 *  		MANAGER INIT		     *
 ********************************************/		

//		INIT INTERNAL VALUES 
//input:
//	s	Sensor structure.
//output:
//	0 or 1	Return 0 if alL values were 
//		correctly loaded. Otherwise
//		1 is returned.
//description:
//	This function set all the sensor
//	initial values and then it checks if
//	all values were correctly loaded.
//-------------------------------------------	
static int1 set_sensor_values(sensor s)
{
	s[0].x=20;
	s[0].y=10;
	s[0].rx=64;
	s[0].ry=12;
	s[0].oldr=0;
	if(s[0].x != 20 || s[0].y != 10 || s[0].rx != 64 || s[0].ry != 12)
		return 1;

	s[1].x=0;
	s[1].y=35;
	s[1].rx=44;
	s[1].ry=32;
	s[1].oldr=0;
	if(s[1].x != 0 || s[1].y != 35 || s[1].rx != 44 || s[1].ry != 32)
		return 1;

	s[2].x=75;
	s[2].y=50;
	s[2].rx=64;
	s[2].ry=52;
	s[2].oldr=0;
	if(s[2].x != 75 || s[2].y != 50 || s[2].rx != 64 || s[2].ry != 52)
		return 1;

	s[3].x=90;
	s[3].y=20;
	s[3].rx=84;
	s[3].ry=32;
	s[3].oldr=0;
	if(s[3].x != 90 || s[3].y != 20 || s[3].rx != 84 || s[3].ry != 32)
		return 1;

	read_from_eeprom(s);

	return 0;
}

//		MANAGER INIT 
//input:
//	s	Sensor structure.
//output:
//	0 or 1	Return 0 if the menu manager 
//		were correctly initialized.
//		Otherwise 1 is returned.
//description:
//	This function set the ADC and also
//	load the initial values for all 
//	sensors.
//-------------------------------------------	
int1 manager_init(sensor s)
{
	set_adc();
	if(set_sensor_values(s)){
		return 1;
	}
	
	return 0;
}

//		MAIN MENU 
//input:
//	s	Sensor structure.
//	units	Pressure units (mmHg or kPa).
//description:
//	main menu is active when the device
//	is in MAIN_ON mode and call the 
//	functions to read the ADC and print 
//	the vales in the GLCD. 
//-------------------------------------------	
void main_menu(sensor s,unsigned int units)
{
	char u[9];

	get_adc(s);
	show_values(s,units);
	if(units==KPA)
		sprintf(u,"kPa"); 
	else
		sprintf(u,"mmHg"); 
	
	glcd_rect(5,54,30,62,YES,OFF);
	glcd_text57(5,54, u,1,ON); 
}

//		CALIBRATION MENU 
//input:
//	s	Sensor structure.
//description:
//	calib. menu is active when the device
//	is in MAIN_CALIB mode and call the
//	two calibration options 
//	"Evaluar sensores" and 
//	"Calibrar sensores".
//-------------------------------------------	
unsigned int calib_menu(sensor s)
{
	char text[50];
	unsigned int op;
	int1 x;

intro:
	disable_interrupts(int_EXT);				// calib. menu intro.
	disable_interrupts(int_RB);
	glcd_init(ON);
	sprintf(text,"Menu de calibracion");
	glcd_text57(10,10, text,1,ON); 
	sprintf(text,"1-> Siguiente.");
	glcd_text57(15,30, text,1,ON); 
	sprintf(text,"3-> Salir."); 
	glcd_text57(15,40, text,1,ON);
	op=0;
	x=1;

command:							// code to attend the user request. 
	delay_ms(200);
	while(op==0){
		op=swap( PORTB & 0b00110001);	
	}

	if(op==2)
		goto exit;
	if(op==16){
		x=~x;
		if(x)
			goto two;
	}
	if(op==1){
		if(x)
			goto calib_sen;
		else
			goto calib_test;
	}

one:	
	glcd_fillScreen(OFF);   				// screen option test sensors.
	sprintf(text,"Evaluar Sensores");
	glcd_text57(10,10, text,1,ON); 
	sprintf(text,"1-> Siguiente.");
	glcd_text57(15,30, text,1,ON); 
	sprintf(text,"2-> Ejecutar."); 
	glcd_text57(15,40, text,1,ON);
	sprintf(text,"3-> Salir."); 
	glcd_text57(15,50, text,1,ON);
	op=0;
	goto command;
								// screen option calibrate sensors.
two:								
	glcd_fillScreen(OFF);   
	sprintf(text,"Calibrar Sensores");
	glcd_text57(10,10, text,1,ON); 
	sprintf(text,"1-> Siguiente.");
	glcd_text57(15,30, text,1,ON); 
	sprintf(text,"2-> Ejecutar."); 
	glcd_text57(15,40, text,1,ON);
	sprintf(text,"3-> Salir."); 
	glcd_text57(15,50, text,1,ON);
	op=0;
	goto command;

calib_test:							// call the function to test the sensors.

	if(test_calib(s)){
		x=0;
		goto one;
	}
	else{
		x=1;
		goto two;
	}

calib_sen:
			
	sensor_calibration(s);					// call the funtion to calib. the sensors.
	goto two;
   
exit:								// end of the calib. menu.
	glcd_fillScreen(OFF);   
	sprintf(text,"Fin del Menu");
	glcd_text57(30,30, text,1,ON);
	delay_ms(1000);
	enable_interrupts(int_RB);				
	enable_interrupts(int_EXT);
	glcd_init(OFF);
	return MAIN_OFF;
}
