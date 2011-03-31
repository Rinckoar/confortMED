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

float adc_2_pressure(long adc,float m,float b,unsigned int units)
{
	float p;

	p=(adc*5/1023)*m+b;
	if(units==KPA)
		return p;
	
	return p*KPA2MMHG;
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
	}
	else{
		x=ON;
		mode=MAIN_ON;
	}
	
	glcd_init(x);
	return mode;
}

//	PRINT VALUES IN GLCD 
//-------------------------------------------	
void show_values(sensor s,unsigned int units)
{
	
	char value[9];
	float temp=0;
	
   	unsigned int i,r;	
	for(i=0;i<NCH;i++){

		
		
		sprintf(value, "%.2f",adc_2_pressure(s[i].adc,s[i].m,s[i].b,units)); 
		
		temp= (float) s[i].adc/1023.0;
		if(temp < 0.25)
			r=2;
		if((temp >= 0.25) & (temp < 0.5))
			r=4;
		if((temp >= 0.5) & (temp < 0.75))
			r=6;
		if(temp >= 0.75)
			r=8;
		
		glcd_rect(s[i].x,s[i].y,s[i].x + 35,s[i].y + 7,YES,OFF);
		glcd_circle(s[i].rx,s[i].ry,s[i].oldr,YES,OFF);
		glcd_circle(s[i].rx,s[i].ry,r,YES,ON);
		glcd_text57(s[i].x, s[i].y, value,1,ON); 
		s[i].oldr=r;
	}
}




/*********************************************
 *  		EEPROM CONTROL		     *
 ********************************************/		

//		READ VALUES FROM EEPROM
//-------------------------------------------	

void read_from_eeprom(sensor s)
{
	unsigned int i;

	for(i=0;i<NCH;i++){
		s[i].m=read_float_eeprom(8*i);
		s[i].b=read_float_eeprom(8*i+4);	
	}
}

//		WRITE VALUES TO EEPROM
//-------------------------------------------	
void write_2_eeprom(sensor s,unsigned int index)
{
	write_float_eeprom(8*index,s[index].m);
	write_float_eeprom(8*index+4,s[index].b);	
}


