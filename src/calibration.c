#include <Math.h>
#include <stdlibm.h>

float *voltage,*load;
char text[50];

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
			sprintf(text,"X[%d]= %.1f gr",j+1,w);
			glcd_text57(0,10, text,1,ON);
			sprintf(text,"Y[%d]= %.1f V ",j+1,v);
			glcd_text57(0,20, text,1,ON);
		}
		load[j]=w;
		voltage[j]=v;
		delay_ms(600);
	}
}
*/

void set_calib_values(sensor s,unsigned int index)
{
/*	load[0]=198;
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
	voltage[9]=2.8;*/

	load[0]=0;
	load[1]=10;
	load[2]=20;
	load[3]=30;
	load[4]=40;
	load[5]=50;

	voltage[0]=0;
	voltage[1]=1;
	voltage[2]=2;
	voltage[3]=3;
	voltage[4]=4;
	voltage[5]=5;

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
	sprintf(text,"Sensor # %d",index+1);
	glcd_text57(0,0, text,1,ON); 
	sprintf(text,"R=%.3f",r);
	glcd_text57(20,30, text,1,ON); 
	sprintf(text,"m=%.3f",s[index].m);
	glcd_text57(20,40, text,1,ON); 
	sprintf(text,"b=%.3f",s[index].b);
	glcd_text57(20,50, text,1,ON); 
	delay_ms(5000);
	
	write_2_eeprom(s,index);
}

void sensor_calibration(sensor s)
{
	unsigned int k=0,op=0;

	voltage=(float *)malloc(NKVAL*sizeof(float));
	load=(float *)malloc(NKVAL*sizeof(float));
	if((voltage==NULL) || (load==NULL))	
		goto exit;
	

intro:	
	glcd_fillScreen(OFF);   
	sprintf(text,"Elegir Sensor");
	glcd_text57(10,0, text,1,ON); 
	sprintf(text,"1-> Siguiente.");
	glcd_text57(10,30, text,1,ON); 
	sprintf(text,"2-> Ejecutar."); 
	glcd_text57(10,40, text,1,ON);
	sprintf(text,"3-> Atras."); 
	glcd_text57(10,50, text,1,ON);
	if(k<NCH){
		sprintf(text,"Sensor %d",k+1);
		glcd_text57(20,15, text,1,ON); 
	}
	else{
		sprintf(text,"Todos");
		glcd_text57(20,15, text,1,ON);
	}
	op=0;

command:	
	
	delay_ms(200);
	while(op==0){
		op=swap( PORTB & 0b00110001);	
	}
	if (op==2)
		goto exit;
	if((op==16)){
		k++;
		if(k>NCH)
			k=0;
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
	sprintf(text,"Sensor # 1");
	glcd_text57(0,0, text,1,ON); 
	sprintf(text,"1-> Incre.");
	glcd_text57(0,35, text,1,ON); 
	sprintf(text,"2-> Decre."); 
	glcd_text57(0,45, text,1,ON);
	sprintf(text,"3-> Acep."); 
	glcd_text57(0,55, text,1,ON);
	delay_ms(300);
	
	set_calib_values(s,0);
	linear_reg(s,0);
	if(k<NCH){
		k=0;
		goto intro;
	}
s1:
	glcd_fillScreen(OFF);   
	sprintf(text,"Sensor # 2");
	glcd_text57(0,0, text,1,ON); 
	sprintf(text,"1-> Incre.");
	glcd_text57(0,35, text,1,ON); 
	sprintf(text,"2-> Decre."); 
	glcd_text57(0,45, text,1,ON);
	sprintf(text,"3-> Acep."); 
	glcd_text57(0,55, text,1,ON);
	delay_ms(300);
	
	set_calib_values(s,1);
	linear_reg(s,1);
	if(k<NCH){
		k=0;
		goto intro;
	}

s2:
	glcd_fillScreen(OFF);   
	sprintf(text,"Sensor # 3");
	glcd_text57(0,0, text,1,ON); 
	sprintf(text,"1-> Incre.");
	glcd_text57(0,35, text,1,ON); 
	sprintf(text,"2-> Decre."); 
	glcd_text57(0,45, text,1,ON);
	sprintf(text,"3-> Acep."); 
	glcd_text57(0,55, text,1,ON);
	delay_ms(300);
	
	set_calib_values(s,2);
	linear_reg(s,2);
	if(k<NCH){
		k=0;
		goto intro;
	}
s3:
	glcd_fillScreen(OFF);   
	sprintf(text,"Sensor # 4");
	glcd_text57(0,0, text,1,ON); 
	sprintf(text,"1-> Incre.");
	glcd_text57(0,35, text,1,ON); 
	sprintf(text,"2-> Decre."); 
	glcd_text57(0,45, text,1,ON);
	sprintf(text,"3-> Acep."); 
	glcd_text57(0,55, text,1,ON);
	delay_ms(300);
	
	set_calib_values(s,3);
	linear_reg(s,3);
	k=0;
	goto intro;
exit:
	free(voltage);
	free(load);
}

unsigned int test_calib(sensor s)
{

	unsigned int op,k,i,j;
	unsigned int status[4]={0 0 0 0};
	float Vx,error;

intro:	
	glcd_fillScreen(OFF);   
	sprintf(text,"Conectar todos\n\r los sensores");
	glcd_text57(9,0, text,1,ON); 
	sprintf(text,"2-> Ejecutar."); 
	glcd_text57(10,40, text,1,ON);
	sprintf(text,"3-> Atras."); 
	glcd_text57(10,50, text,1,ON);
	op=0;
	j=0;

command:
	delay_ms(200);
	while(op==0){
		op=swap( PORTB & 0b00110000);	
	}
	if(op==2)
		goto exit;
	if(op==1){
		if(j)
			goto done;
	}
	
test:
	delay_ms(2000);
	get_adc(s);
	for(i=0;i<NCH;i++){
		Vx = (float) s[i].adc*5.0/1023.0;
		error=fabs(Vx - s[i].b)/5.0; 

/*		glcd_fillScreen(OFF);   
		sprintf(text,"Vx=%f",Vx); 
		glcd_text57(0,10, text,1,ON);
		sprintf(text,"Error=%f",error); 
		glcd_text57(0,20, text,1,ON);

		
		delay_ms(5000);
*/
		if(error >= MAXERROR)
			status[i]=1;
	}

	glcd_fillScreen(OFF);   
	sprintf(text,"Recalibrar Sensores\a");
	glcd_text57(0,0, text,1,ON); 
	sprintf(text,"2-> Calibrar."); 
	glcd_text57(10,40, text,1,ON);
	sprintf(text,"3-> Atras."); 
	glcd_text57(10,50, text,1,ON);
	
	k=0;
	for(i=0;i<NCH;i++){
		if(status[i]){
			sprintf(text,"S%d",i+1);
			glcd_text57(10+k,20, text,1,ON); 
			k+=20;
		}
	}
	j=1;
	op=0;

	goto command;
	
done:
	return 0;

exit:
	return 1;
}


