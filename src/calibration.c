
#include <Math.h>



signed int sensor_calibration()
{
	unsigned int k=0,op=0;
	char test[50];

intro:	
	glcd_fillScreen(OFF);   
	sprintf(test,"Elegir Sensor");
	glcd_text57(10,0, test,1,ON); 
	sprintf(test,"1-> Siguiente.");
	glcd_text57(10,30, test,1,ON); 
	sprintf(test,"2-> Ejecutar."); 
	glcd_text57(10,40, test,1,ON);
	sprintf(test,"3-> Atras."); 
	glcd_text57(10,50, test,1,ON);
	if(k<NCH){
		sprintf(test,"Sensor %d",k+1);
		glcd_text57(20,15, test,1,ON); 
	}
	else{
		sprintf(test,"Todos",);
		glcd_text57(20,15, test,1,ON);
	}


command:	
	
	delay_ms(500);
	while(op==0){
		op=swap( PORTB & 0b00110001);	
	}
	if (op==2)
		goto exit;
	if((op==16)){
		k++;
		op=0;
		goto intro;
	}
	if((op==1)){
		switch (k){

			case 0: {	goto s0;
					break;
				}

			case 1: {	goto s1;
					break;
				}

			case 2: {	goto s2;
					break;
				}

			case 3: {	goto s3;
					break;
				}
			case 4: {	goto s0;
					break;
				}
		}
	}


s0:
	glcd_fillScreen(OFF);   
	sprintf(test,"s0");
	glcd_text57(10,10, test,1,ON);
	delay_ms(1000);
	if(k<NCH)
		goto done;

s1:

	glcd_fillScreen(OFF);   
	sprintf(test,"s1");
	glcd_text57(10,10, test,1,ON); 
	delay_ms(1000);
	if(k<NCH)
		goto done;
s2:
	glcd_fillScreen(OFF);   
	sprintf(test,"s2");
	glcd_text57(10,10, test,1,ON); 
	delay_ms(1000);
	if(k<NCH)
		goto done;

s3:
	glcd_fillScreen(OFF);   
	sprintf(test,"s3");
	glcd_text57(10,10, test,1,ON); 
	delay_ms(1000);

done:
	return 0;

exit:

	return -1;




}




