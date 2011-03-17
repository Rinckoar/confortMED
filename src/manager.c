


/*********************************************
 *  		MANAGER INIT		     *
 ********************************************/		

//		INIT INTERNAL VALUES 
//-------------------------------------------	

static int1 set_sensor_values(sensor s)
{
	int1 retval=0;

	s[0].x=42;
	s[0].y=0;
	s[0].rx=64;
	s[0].ry=18;
	if(s[0].x != 45 || s[0].y != 0 || s[0].rx != 64 || s[0].ry != 18){
		retval=1;
		goto exit;
	}
	
	s[1].x=45;
	s[1].y=54;
	s[1].rx=64;
	s[1].ry=42;
	if(s[1].x != 45 || s[1].y != 54 || s[1].rx != 64 || s[1].ry != 42){
		retval=1;
		goto exit;
	}

	s[2].x=0;
	s[2].y=16;
	s[2].rx=20;
	s[2].ry=36;
	if(s[2].x != 0 || s[2].y != 16 || s[2].rx != 20 || s[2].ry != 36){
		retval=1;
		goto exit;
	}

	s[3].x=86;
	s[3].y=16;
	s[3].rx=110;
	s[3].ry=36;
	if(s[3].x != 86 || s[3].y != 16 || s[3].rx != 110 || s[3].ry != 36){
		retval=1;
		goto exit;
	}

	read_eeprom(s);
exit:
	return retval;
}

//		MAIN MANAGER INIT 
//-------------------------------------------	
int1 manager_init(sensor s)
{

	int1 retval=0;

	set_adc();
	
	if(set_sensor_values(s)){
		retval=1;
	}

	return retval;
}




