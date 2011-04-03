#include "control.c"
#include "calibration.c"

/*********************************************
 *  		MANAGER INIT		     *
 ********************************************/		

//		INIT INTERNAL VALUES 
//-------------------------------------------	

static int1 set_sensor_values(sensor s)
{
	int1 retval=0;


	s[0].x=20;
	s[0].y=10;
	s[0].rx=64;
	s[0].ry=12;
	s[0].oldr=0;
	if(s[0].x != 20 || s[0].y != 10 || s[0].rx != 64 || s[0].ry != 12){
		retval=1;
		goto exit;
	}

	s[1].x=0;
	s[1].y=35;
	s[1].rx=44;
	s[1].ry=32;
	s[1].oldr=0;
	if(s[1].x != 0 || s[1].y != 35 || s[1].rx != 44 || s[1].ry != 32){
		retval=1;
		goto exit;
	}

	s[2].x=75;
	s[2].y=50;
	s[2].rx=64;
	s[2].ry=52;
	s[2].oldr=0;
	if(s[2].x != 75 || s[2].y != 50 || s[2].rx != 64 || s[2].ry != 52){
		retval=1;
		goto exit;
	}

	s[3].x=90;
	s[3].y=20;
	s[3].rx=84;
	s[3].ry=32;
	s[3].oldr=0;
	if(s[3].x != 90 || s[3].y != 20 || s[3].rx != 84 || s[3].ry != 32){
		retval=1;
		goto exit;
	}

	read_from_eeprom(s);

exit:
	return retval;
}

//		MAIN MANAGER INIT 
//-------------------------------------------	
int1 manager_init(sensor s)
{

	int1 retval=0;
	int i;

	set_adc();
	
	for(i=0;i<NCH;i++){
		write_float_eeprom(8*i,1);
		write_float_eeprom(8*i+4,0);	
	}
	
	if(set_sensor_values(s)){
		retval=1;
	}

	return retval;
}

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

unsigned int calib_menu(sensor s)
{
	char text[50];
	unsigned int op;
	int1 x;


intro:
	disable_interrupts(int_EXT);	
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

command:	
	delay_ms(200);
	while(op==0){
		op=swap( PORTB & 0b00110001);	
	}

select:
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
	glcd_fillScreen(OFF);   
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

calib_sen:

	sensor_calibration(s);
	goto two;

calib_test:

	if(test_calib(s)){
		x=0;
		goto one;
	}
	else{
		x=1;
		goto two;
	}
   
exit:
	glcd_fillScreen(OFF);   
	sprintf(text,"Fin del Menu");
	glcd_text57(30,30, text,1,ON);
	delay_ms(1000);
	enable_interrupts(int_RB);				
	enable_interrupts(int_EXT);
	glcd_init(OFF);
	return MAIN_OFF;





}







