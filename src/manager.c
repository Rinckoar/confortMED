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
	char test[50];
	unsigned int op,j;
	signed int retval;
intro:
	disable_interrupts(int_EXT);	
	disable_interrupts(int_RB);
	glcd_init(ON);
	sprintf(test,"Menu de calibracion");
	glcd_text57(10,10, test,1,ON); 
	sprintf(test,"1-> Siguiente.");
	glcd_text57(15,30, test,1,ON); 
	sprintf(test,"3-> Salir."); 
	glcd_text57(15,40, test,1,ON);
	op=0;
	j=0;

command:	
	delay_ms(500);
	while(op==0){
		op=swap( PORTB & 0b00110001);	
	}
	j++;
	if (op==2)
		goto exit;
	if(op==16){
		switch(j){
			case 2: {	goto two;
					break;
				}
			case 3: {	goto three;
					break;
				}
		}
	}
	if(op==1){
		switch(j){
			case 2: {	goto calib_sen;
					break;
				}
			case 3: {	goto calib_off;
					break;
				}
		}
	}


one:	
	glcd_fillScreen(OFF);   
	sprintf(test,"Calibrar Sensores");
	glcd_text57(10,10, test,1,ON); 
	sprintf(test,"1-> Siguiente.");
	glcd_text57(15,30, test,1,ON); 
	sprintf(test,"2-> Ejecutar."); 
	glcd_text57(15,40, test,1,ON);
	sprintf(test,"3-> Salir."); 
	glcd_text57(15,50, test,1,ON);
	op=0;
	goto command;
	
two:
	glcd_fillScreen(OFF);   
	sprintf(test,"Calibrar Offset");
	glcd_text57(10,10, test,1,ON); 
	sprintf(test,"1-> Siguiente.");
	glcd_text57(15,30, test,1,ON); 
	sprintf(test,"2-> Ejecutar."); 
	glcd_text57(15,40, test,1,ON);
	sprintf(test,"3-> Salir."); 
	glcd_text57(15,50, test,1,ON);
	op=0;
	goto command;

three:
	glcd_fillScreen(OFF);   
	sprintf(test,"Fin del Menu");
	glcd_text57(10,10, test,1,ON);
	delay_ms(1000);
	goto exit;




calib_sen:

	if(sensor_calibration()<0)
		goto intro;
	else
		goto three;


calib_off:

	glcd_fillScreen(OFF);   
	sprintf(test,"LOCO");
	glcd_text57(10,10, test,1,ON); 
	goto command;



exit:
	enable_interrupts(int_RB);				
	enable_interrupts(int_EXT);
	glcd_init(OFF);
	return MAIN_OFF;





}







