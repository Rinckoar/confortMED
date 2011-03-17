

#include <internal_eeprom.c>
#include "confortmed_glcd.c"
#include <graphics.c>

/*********************************************
 *  		ADC CONTROL		     *
 ********************************************/		

//		INIT ADC 
//-------------------------------------------	
void set_adc()
{
	setup_adc(ADC_CLOCK_INTERNAL);
	setup_adc_ports(AN0_AN1_AN2_AN3_AN4);
}

//	GET ADC VALUES FOR EACH CHANNEL 
//-------------------------------------------	
void get_adc(sensor s)
{
	unsigned int i;
	for(i=0;i<NCH;i++){
		set_adc_channel(i);
		while(!adc_done()){
		}
		s[i].adc=read_adc();
	}
}

/*********************************************
 *  		GLCD CONTROL		     *
 ********************************************/		

//		SET GLCD ON/OFF
//-------------------------------------------	
unsigned int set_lcd(unsigned int mode)
{
	int1 x;

	if((mode == MAIN_ON) || (mode == MAIN_PAUSE)){
		x=OFF;
		mode=MAIN_OFF;
		glcd_fillScreen(OFF);
		disable_interrupts(INT_EXT);
	}
	else{
		x=ON;
		mode=MAIN_ON;
		enable_interrupts(INT_EXT);
	}
	
	glcd_init(x);
	return mode;
}

//	PRINT VALUES IN GLCDSET LCD ON/OFF
//-------------------------------------------	
void show_values(sensor s)
{
	
	char voltage[5];
   	int i;	
	for(i=0;i<NCH;i++){
	sprintf(voltage, "%.2f",(float) s[i].adc*5.0/1023.0); // Converts adc to text
	glcd_rect(s[i].x,s[i].y,s[i].x+15,s[i].y+10, YES, OFF);
	glcd_text57(s[i].x, s[i].y, voltage, 1, ON);            // Write the new voltage

	}
}

/*********************************************
 *  		EEPROM CONTROL		     *
 ********************************************/		

//		READ VALUES FROM EEPROM
//-------------------------------------------	

void read_eeprom(sensor s)
{
	unsigned int i;

	for(i=0;i<NCH;i++){
		s[i].m=read_float_eeprom(8*i);
		s[i].b=read_float_eeprom(8*i+4);	
	}
}

void write_eeprom(sensor s)
{
	unsigned int i;

	for(i=0;i<NCH;i++){
		write_float_eeprom(8*i,s[i].m);
		write_float_eeprom(8*i+4,s[i].b);	
	}
}


