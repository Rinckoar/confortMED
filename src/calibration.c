#include <Math.h>
#include <stdlibm.h>

float *voltage,*load;
char test[50];

float counter(float count)
{
	if(INCR){
		delay_ms(20);
		if(INCR)
			count+=1;
	}
	if(DECR){
		delay_ms(20);
		if(DECR)
			count-=1;
	}
	if(count<0)
		count=0;
	
	return count;

}
/*
void set_calib_values(sensor s,unsigned int index)
{
	unsigned int j;
	float v,w;

	for(j=0;j<NKVAL;j++){

		w=0.0;
		v=0.0;
		while(OK==0){	
			get_adc(s);
			v=(float) s[index].adc*5/1023;
			w=counter(w);
			glcd_rect(0,10,90,30,YES,OFF);
			sprintf(test,"X[%d]= %.1f V",j+1,v);
			glcd_text57(0,10, test,1,ON);
			sprintf(test,"Y[%d]= %.1f gr ",j+1,w);
			glcd_text57(0,20, test,1,ON);
		}
		load[j]=w;
		voltage[j]=v;
		delay_ms(600);
	}
}
*/
void set_calib_values(sensor s,unsigned int index)
{
	load[0]=198;
	load[1]=186;
	load[2]=116;
	load[3]=89;
	load[4]=120;
	load[5]=109;
	load[6]=28;
	load[7]=58;
	load[8]=34;
	load[9]=31;

	voltage[0]=22.7;
	voltage[1]=16.6;
	voltage[2]=15.9;
	voltage[3]=12.5;
	voltage[4]=10.2;
	voltage[5]=6.8;
	voltage[6]=6.8;
	voltage[7]=4.0;
	voltage[8]=2.7;
	voltage[9]=2.8;

}


void linear_reg(sensor s,unsigned int index)
{

	float Xm,Ym,Sx,Sy,Sxy,r,b0,b1;
	int i;

	Xm=0;
	Ym=0;
	for(i=0;i<NKVAL;i++){
		Xm = Xm + load[i];
		Ym = Ym + Voltage[i];
	}
	Xm=Xm/NKVAL;
	Ym=Ym/NKVAL;

	Sx=0;
	Sy=0;
	Sxy=0;
	for(i=0;i<NKVAl;i++){
		Sx= Sx + pow(load[i]-Xm,2);
		Sy= Sy + pow(voltage[i]-Ym,2);
		Sxy = Sxy + (load[i]-Xm)*(voltage[i]-Ym);
	}

	b1=Sxy/Sx;
	b0=Ym-b1*Xm;
	r=Sxy/(sqrt(Sx)*sqrt(Sy));

	s[index].m=b1;
	s[index].b=b0;

	glcd_fillScreen(OFF);  
	sprintf(test,"Sensor # %d",index+1);
	glcd_text57(0,0, test,1,ON); 
	sprintf(test,"R=%.3f",r);
	glcd_text57(20,30, test,1,ON); 
	sprintf(test,"m=%.3f",s[index].m);
	glcd_text57(20,40, test,1,ON); 
	sprintf(test,"b=%.3f",s[index].b);
	glcd_text57(20,50, test,1,ON); 
	delay_ms(5000);
	
	write_2_eeprom(s,index);
}

signed int sensor_calibration(sensor s)
{
	unsigned int k=0,op=0;

	voltage=(float *)malloc(NKVAL*sizeof(float));
	load=(float *)malloc(NKVAL*sizeof(float));
	if((voltage==NULL) || (load==NULL))	
		goto exit;

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
	sprintf(test,"Sensor # 1");
	glcd_text57(0,0, test,1,ON); 
	sprintf(test,"1-> Incre.");
	glcd_text57(0,35, test,1,ON); 
	sprintf(test,"2-> Decre."); 
	glcd_text57(0,45, test,1,ON);
	sprintf(test,"3-> Acep."); 
	glcd_text57(0,55, test,1,ON);
	delay_ms(300);
	
	set_calib_values(s,0);
	linear_reg(s,0);
	if(k<NCH)
		goto done;

s1:
	glcd_fillScreen(OFF);   
	sprintf(test,"Sensor # 2");
	glcd_text57(0,0, test,1,ON); 
	sprintf(test,"1-> Incre.");
	glcd_text57(0,35, test,1,ON); 
	sprintf(test,"2-> Decre."); 
	glcd_text57(0,45, test,1,ON);
	sprintf(test,"3-> Acep."); 
	glcd_text57(0,55, test,1,ON);
	delay_ms(300);
	
	set_calib_values(s,1);
	linear_reg(s,1);
	if(k<NCH)
		goto done;

s2:
	glcd_fillScreen(OFF);   
	sprintf(test,"Sensor # 3");
	glcd_text57(0,0, test,1,ON); 
	sprintf(test,"1-> Incre.");
	glcd_text57(0,35, test,1,ON); 
	sprintf(test,"2-> Decre."); 
	glcd_text57(0,45, test,1,ON);
	sprintf(test,"3-> Acep."); 
	glcd_text57(0,55, test,1,ON);
	delay_ms(300);
	
	set_calib_values(s,2);
	linear_reg(s,2);
	if(k<NCH)
		goto done;

s3:
	glcd_fillScreen(OFF);   
	sprintf(test,"Sensor # 4");
	glcd_text57(0,0, test,1,ON); 
	sprintf(test,"1-> Incre.");
	glcd_text57(0,35, test,1,ON); 
	sprintf(test,"2-> Decre."); 
	glcd_text57(0,45, test,1,ON);
	sprintf(test,"3-> Acep."); 
	glcd_text57(0,55, test,1,ON);
	delay_ms(300);
	
	set_calib_values(s,3);
	linear_reg(s,3);
	if(k<NCH)
		goto done;

done:
	free(voltage);
	free(load);
	return 0;

exit:

	return -1;




}




