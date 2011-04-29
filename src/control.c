#include <internal_eeprom.c>
#include "confortmed_glcd.c"
#include <graphics.c>

/*********************************************
 *  		ADC CONTROL		     *
 ********************************************/		

//		INIT ADC 
//description:
//	This function set the ADC to use the 
//	internal clock and also set the port
//	A pins A0,A1,A2,A3 and A4 as analog 
//	inputs.
//-------------------------------------------	
void set_adc()
{
	setup_adc(ADC_CLOCK_INTERNAL);
	setup_adc_ports(AN0_AN1_AN2_AN3_AN4);
}

//	GET ADC VALUES FOR EACH CHANNEL 
//input: 
//	Structure sensor
//description:
//	Get the ADC value for each sensor.
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

//		COMPUTE PRESSURE VALUE
//Inputs: 
//	adc	Analog to digital conv. value.
//	m	Slope of the calibration curve.
//	b	y-axes intrc. of the calib curve.
//	units	Pressure units (mmHg or kPa).
//output:
//	Pressure value in kPa or mmHg.
//description:
//	First the ADC value is changed into 
//	voltage value, then the weight is 
//	computed using the parameters of
//	the calibration curve (m & b), 
//	then the pressure is calculated 
//	dividing the weigth by the sensing 
//	area and multipliying it by the 
//	factor grf->kPa. finaly the pressure 
//	value is returned in kPa or mmHg 
//	depending of the value of the variable
//	"units".
//-------------------------------------------	
float adc_2_pressure(long adc,float m,float b,unsigned int units)
{
	float p,v,w;

	v=(float) adc*5.0/1023.0;
	w= (v-b)/m;
	p=(w/AREA)*GR2KPA;	
	if(units==KPA)
		return p;
	
	return p*KPA2MMHG;
}

/*********************************************
 *  		GLCD CONTROL		     *
 ********************************************/		

//		SET GLCD ON/OFF
//input:
//	mode	Option to set the GLCD ON/OFF.
//output:
//	mode	The new GLCD state.
//description:
//	The function switch between the values
//	on ON and OFF. If the input is ON or
//	PAUSE states is changed into OFF, and if
//	the input is OFF, it is cahnged into ON.
//-------------------------------------------
unsigned int set_lcd(unsigned int mode)
{

	if((mode == MAIN_ON) || (mode == MAIN_PAUSE)){
		glcd_init(OFF);
		mode=MAIN_OFF;
		glcd_fillScreen(OFF);
	}
	else{
		glcd_init(ON);
		mode=MAIN_ON;
	}
	
	return mode;
}

//		PRINT VALUES IN GLCD 
//input:
//	s	Sensor structure.
//	units	Pressure units (mmHg or kPa).
//description:
//	Print in the GLCD the numeric and 
//	graphic pressure values for each 
//	sensor. First the pressure value 
//	(float) is cahnged into char, then
//	the rate ADC/MaxADC (ADC/1023) is 
//	calculated and based in this value
//	the circle radius is set.
//	Finaly the over-pressure warning "!"
//	is printed if the read pressure is
//	bigger than MAXPRESSURE.
//-------------------------------------------	
void show_values(sensor s,unsigned int units)
{
	char text[9];
	float temp=0,pressure=0;
   	unsigned int i,r;

	for(i=0;i<NCH;i++){

		pressure=adc_2_pressure(s[i].adc,s[i].m,s[i].b,units);			// calculate the read pressure.
		sprintf(text,"%.2f",pressure); 						// charge in "value" (char) the value of pressure. 
		

		temp= (float) s[i].adc/1023.0;
		if(temp < 0.25)								// set the circle radius.
			r=2;
		if((temp >= 0.25) & (temp < 0.5))
			r=4;
		if((temp >= 0.5) & (temp < 0.75))
			r=6;
		if(temp >= 0.75)
			r=8;
		
		glcd_rect(s[i].x,s[i].y,s[i].x + 35,s[i].y + 7,YES,OFF);		// erase past numeric value.
		glcd_circle(s[i].rx,s[i].ry,s[i].oldr,YES,OFF);				// erase past circle.
		glcd_circle(s[i].rx,s[i].ry,r,YES,ON);					// print new circle.
		glcd_text57(s[i].x, s[i].y, text,1,ON); 				// print new numeric value.
		s[i].oldr=r;
		
		if(units == KPA){							// print warning message for over-pressure.
			if((pressure >= MAXPRESSURE) && (r > 6)){
				sprintf(text,"!");
				glcd_text57(s[i].rx-5, s[i].ry-7, text,2,OFF);
			}
		}
		else{
			if(pressure >= MAXPRESSURE*KPA2MMHG && (r > 6)){
				sprintf(text,"!");
				glcd_text57(s[i].rx-5, s[i].ry-7, text,2,OFF);
			}
		}
	}
}

/*********************************************
 *  		EEPROM CONTROL		     *
 ********************************************/		

//		READ VALUES FROM EEPROM
//input:
//	s	Sensor structure. 
//description:
//	Read from the the EEPROM the value of
//	of the calibration curve parameters 
//	for each sensor.
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
//input:
//	s	Sensor structure.
//	index	Pointer to the EEPROM memory
//		and the sensor number.
//description:
//	Write to the EEPROM the value of
//	of the calibration curve parameters 
//	for the sensor pointed by the variable
//	"index".
//-------------------------------------------	
void write_2_eeprom(sensor s,unsigned int index)
{
	write_float_eeprom(8*index,s[index].m);
	write_float_eeprom(8*index+4,s[index].b);	
}
