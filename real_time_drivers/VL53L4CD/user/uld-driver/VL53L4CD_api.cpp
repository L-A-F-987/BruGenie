/**
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */


/**
 * @file  vl53l4cd_api.c
 * @brief Functions implementation
 */

#include <string.h>
#include <math.h>
#include <thread>
#include <stdio.h>
#include "VL53L4CD_api.h"
#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>



static const uint8_t VL53L4CD_DEFAULT_CONFIGURATION[] = {
	#ifdef VL53L4CD_I2C_FAST_MODE_PLUS
	0x12, /* 0x2d : set bit 2 and 5 to 1 for fast plus mode (1MHz I2C),
	 else don't touch */
	#else
	0x00, /* 0x2d : set bit 2 and 5 to 1 for fast plus mode (1MHz I2C),
	 else don't touch */
	#endif
	0x00, /* 0x2e : bit 0 if I2C pulled up at 1.8V, else set bit 0 to 1
	 (pull up at AVDD) */
	0x00, /* 0x2f : bit 0 if GPIO pulled up at 1.8V, else set bit 0 to 1
	(pull up at AVDD) */
	0x11, /* 0x30 : set bit 4 to 0 for active high interrupt and 1 for active low
	(bits 3:0 must be 0x1), use SetInterruptPolarity() */
	0x02, /* 0x31 : bit 1 = interrupt depending on the polarity,
	use CheckForDataReady() */
	0x00, /* 0x32 : not user-modifiable */
	0x02, /* 0x33 : not user-modifiable */
	0x08, /* 0x34 : not user-modifiable */
	0x00, /* 0x35 : not user-modifiable */
	0x08, /* 0x36 : not user-modifiable */
	0x10, /* 0x37 : not user-modifiable */
	0x01, /* 0x38 : not user-modifiable */
	0x01, /* 0x39 : not user-modifiable */
	0x00, /* 0x3a : not user-modifiable */
	0x00, /* 0x3b : not user-modifiable */
	0x00, /* 0x3c : not user-modifiable */
	0x00, /* 0x3d : not user-modifiable */
	0xff, /* 0x3e : not user-modifiable */
	0x00, /* 0x3f : not user-modifiable */
	0x0F, /* 0x40 : not user-modifiable */
	0x00, /* 0x41 : not user-modifiable */
	0x00, /* 0x42 : not user-modifiable */
	0x00, /* 0x43 : not user-modifiable */
	0x00, /* 0x44 : not user-modifiable */
	0x00, /* 0x45 : not user-modifiable */
	0x20, /* 0x46 : interrupt configuration 0->level low detection, 1-> level high,
	2-> Out of window, 3->In window, 0x20-> New sample ready , TBC */
	0x0b, /* 0x47 : not user-modifiable */
	0x00, /* 0x48 : not user-modifiable */
	0x00, /* 0x49 : not user-modifiable */
	0x02, /* 0x4a : not user-modifiable */
	0x14, /* 0x4b : not user-modifiable */
	0x21, /* 0x4c : not user-modifiable */
	0x00, /* 0x4d : not user-modifiable */
	0x00, /* 0x4e : not user-modifiable */
	0x05, /* 0x4f : not user-modifiable */
	0x00, /* 0x50 : not user-modifiable */
	0x00, /* 0x51 : not user-modifiable */
	0x00, /* 0x52 : not user-modifiable */
	0x00, /* 0x53 : not user-modifiable */
	0xc8, /* 0x54 : not user-modifiable */
	0x00, /* 0x55 : not user-modifiable */
	0x00, /* 0x56 : not user-modifiable */
	0x38, /* 0x57 : not user-modifiable */
	0xff, /* 0x58 : not user-modifiable */
	0x01, /* 0x59 : not user-modifiable */
	0x00, /* 0x5a : not user-modifiable */
	0x08, /* 0x5b : not user-modifiable */
	0x00, /* 0x5c : not user-modifiable */
	0x00, /* 0x5d : not user-modifiable */
	0x01, /* 0x5e : not user-modifiable */
	0xcc, /* 0x5f : not user-modifiable */
	0x07, /* 0x60 : not user-modifiable */
	0x01, /* 0x61 : not user-modifiable */
	0xf1, /* 0x62 : not user-modifiable */
	0x05, /* 0x63 : not user-modifiable */
	0x00, /* 0x64 : Sigma threshold MSB (mm in 14.2 format for MSB+LSB),
	 use SetSigmaThreshold(), default value 90 mm  */
	0xa0, /* 0x65 : Sigma threshold LSB */
	0x00, /* 0x66 : Min count Rate MSB (MCPS in 9.7 format for MSB+LSB),
	 use SetSignalThreshold() */
	0x80, /* 0x67 : Min count Rate LSB */
	0x08, /* 0x68 : not user-modifiable */
	0x38, /* 0x69 : not user-modifiable */
	0x00, /* 0x6a : not user-modifiable */
	0x00, /* 0x6b : not user-modifiable */
	0x00, /* 0x6c : Intermeasurement period MSB, 32 bits register,
	 use SetIntermeasurementInMs() */
	0x00, /* 0x6d : Intermeasurement period */
	0x0f, /* 0x6e : Intermeasurement period */
	0x89, /* 0x6f : Intermeasurement period LSB */
	0x00, /* 0x70 : not user-modifiable */
	0x00, /* 0x71 : not user-modifiable */
	0x00, /* 0x72 : distance threshold high MSB (in mm, MSB+LSB),
	 use SetD:tanceThreshold() */
	0x00, /* 0x73 : distance threshold high LSB */
	0x00, /* 0x74 : distance threshold low MSB ( in mm, MSB+LSB),
	 use SetD:tanceThreshold() */
	0x00, /* 0x75 : distance threshold low LSB */
	0x00, /* 0x76 : not user-modifiable */
	0x01, /* 0x77 : not user-modifiable */
	0x07, /* 0x78 : not user-modifiable */
	0x05, /* 0x79 : not user-modifiable */
	0x06, /* 0x7a : not user-modifiable */
	0x06, /* 0x7b : not user-modifiable */
	0x00, /* 0x7c : not user-modifiable */
	0x00, /* 0x7d : not user-modifiable */
	0x02, /* 0x7e : not user-modifiable */
	0xc7, /* 0x7f : not user-modifiable */
	0xff, /* 0x80 : not user-modifiable */
	0x9B, /* 0x81 : not user-modifiable */
	0x00, /* 0x82 : not user-modifiable */
	0x00, /* 0x83 : not user-modifiable */
	0x00, /* 0x84 : not user-modifiable */
	0x01, /* 0x85 : not user-modifiable */
	0x00, /* 0x86 : clear interrupt, use ClearInterrupt() */
	0x00  /* 0x87 : start ranging, use StartRanging() or StopRanging(),
	 If you want an automatic start after VL53L4CD_init() call,
	  put 0x40 in location 0x87 */
};

/*added by BruGenie*/

//Interrupt Based Callback

void VL53L4CD_API::start_recording_data()
{	
	interrupt_pin = Default_interrupt_pin;
	chip = drdy_chip;

	uint8_t Addr;

	//assigning pin and chip for interrupt
	chipDRDY = gpiod_chip_open_by_number(chip);
	pinDRDY = gpiod_chip_get_line(chipDRDY,interrupt_pin);

	//setting the I2C bus being used
	char gpioFilename[19] ="/dev/i2c-1";

	fd_i2c = open(gpioFilename, O_RDWR);

	if(fd_i2c <0 ){
		char i2copen[] = "failed to open i2c\n";

	//checking if the device opened
	#ifdef DEBUG
		fprintf(stderr,i2copen);
	#endif 
		throw i2copen;
	}
	
	if (ioctl(fd_i2c,I2C_SLAVE,address)){
		
		char i2cslave[] = "Could not access I2C address!";

	//throwing error if the address could not be accessed
	#ifdef DEBUG
		fprintf(stderr,i2cslave);
	#endif

		throw i2cslave;
	}

	#ifdef DEBUG
	fprintf(stderr,"Init.\n");
	#endif

	//adding a while loop to check if the sensor has booted 
	
	uint8_t continue_loop = (uint8_t)1;
	uint8_t tmp;
	uint16_t i = 0;

	do{
		tmp = i2c_read_Byte(
				VL53L4CD_FIRMWARE__SYSTEM_STATUS);

		if(tmp == (uint8_t)0x3) //Sensor booted
		{
			continue_loop = (uint8_t)0;
			printf("sensor booted\n");
			printf("sensor boot value:%x\n",tmp);
		}
		else if(i < (uint16_t)1000)       //Wait for boot
		{
			i++;
			printf("waiting for sensor boot\n");
		}
		else // Timeout 1000ms reached
		{
			continue_loop = (uint8_t)0;
		}
		usleep(1000);
	}while(continue_loop == (uint8_t)1);


	//setting up sensor configuration
	for (Addr = (uint16_t)0x2D; Addr <= (uint16_t)0x87; Addr++)
	{	
		I2C_WrByte(Addr,
				VL53L4CD_DEFAULT_CONFIGURATION[
                                  Addr - (uint16_t)0x2D]);
	
	//printf("16bit:%X\n8bit:%X\n",Addr,(uint8_t)Addr);
	}

	
	
	running = true;
	
	//continue_loop = (uint8_t)0;

	//}while(continue_loop == (uint8_t)1);

	I2C_WrByte(VL53L4CD_SYSTEM_START, (uint8_t)0x40);

	//Clearing System Interrupt
	I2C_WrByte(VL53L4CD_SYSTEM__INTERRUPT_CLEAR, (uint8_t)0x01);

	//waiting for data ready before moving on

	//adding a gpio event type to wait on 
	
	int ret = gpiod_line_request_rising_edge_events(pinDRDY,"Consumer");

	const struct timespec ts = {1,0};
	gpiod_line_event_wait(pinDRDY, &ts);

	printf("I have risen :)\n\n");

	//Clearing System Interrupt
	I2C_WrByte(VL53L4CD_SYSTEM__INTERRUPT_CLEAR, (uint8_t)0x01);

	//stopping ranging
	I2C_WrByte(VL53L4CD_SYSTEM_START, (uint8_t)0x80);

	//setting ????
	I2C_WrByte(VL53L4CD_VHV_CONFIG__TIMEOUT_MACROP_LOOP_BOUND,(uint8_t)0x09);

	I2C_WrByte(0x0B, (uint8_t)0);

	I2C_WrWord(0x0024, (uint8_t)0x500);


	VL53L4CD_SetRangeTiming_RealTime(50, 0);
	

	//setting the sensor to begin recording data
	//I2C_WrByte(VL53L4CD_SYSTEM_START, (uint8_t)0x40);

	tmp = i2c_read_conversion(VL53L4CD_INTERMEASUREMENT_MS);
	
	/* Sensor runs in continuous mode */
	if(tmp == (uint8_t)0){

		I2C_WrByte(VL53L4CD_SYSTEM_START, (uint8_t)0x21);
	
	}
	/* Sensor runs in autonomous mode */
	else{
		I2C_WrByte(VL53L4CD_SYSTEM_START, (uint8_t)0x40);
	}

	I2C_WrByte(VL53L4CD_SYSTEM__INTERRUPT_CLEAR, (uint16_t)0x01);

	for(int i=0;i<50;i++){
		worker();
		printf("i'm here");
		usleep(100000);
	}

	//thr = std::thread(&VL53L4CD_API::worker,this);

	
	
	//worker();

}

uint8_t VL53L4CD_API::I2C_WrWord(uint16_t reg, uint16_t value)
{	

	uint8_t original_value = i2c_read_conversion(reg);
	
	uint8_t data_write[4];
	data_write[0] = (reg >> 8) & 0xFF;
	data_write[1] = reg & 0xFF;
	data_write[2] = (value >> 8) & 0xFF;
	data_write[3] = value & 0xFF;
	long int r = write(fd_i2c,&data_write,4);
	if(r < 0){
		printf("%i",r);
	#ifdef DEBUG
                fprintf(stderr,"Could not read word from %02x. ret=%d.\n",address,r);
	#endif
             throw "Could not read from i2c.";
	}


	//printf("Register:%x\nValue:%x\n",reg,value);
	

	//printf("datawrite[1]:%x\ndatawrite[2]:%x\n",data_write[1],data_write[2]);

	uint16_t written_value = i2c_read_conversion(reg);

	if(written_value != (uint16_t)value){

		printf("oh this word is wrong\n");

	}

	//if (written_value != original_value){


	//	printf("we have a printer\n");
	//}


}

uint8_t VL53L4CD_API::I2C_Wr_four_bytes(uint16_t reg, uint32_t value)
{	

	uint8_t original_value = i2c_read_conversion(reg);
	
	uint8_t data_write[6];
	data_write[0] = (reg >> 8) & 0xFF;
	data_write[1] = reg & 0xFF;
	data_write[2] = (value >> 24) & 0xFF;
	data_write[3] = (value >> 16) & 0xFF;
	data_write[4] = (value >> 8) & 0xFF;
	data_write[5] = value & 0xFF;
	long int r = write(fd_i2c,&data_write,6);

	//printf("you are writing a 32 bit number of; %x\n\n",value);
	if(r < 0){
		printf("%i",r);
	#ifdef DEBUG
                fprintf(stderr,"Could not read word from %02x. ret=%d.\n",address,r);
	#endif
             throw "Could not read from i2c.";
	}


	//printf("Register:%x\nValue:%x\n",reg,value);
	

	//printf("datawrite[1]:%x\ndatawrite[2]:%x\n",data_write[1],data_write[2]);

	uint16_t written_value = i2c_read_conversion(reg);

	if(written_value != (uint32_t)value){

		printf("oh these 4 bytes are wrong\n");

	}

	//if (written_value != original_value){


	//	printf("we have a 4 byte printer\n");
	//}


}

uint8_t VL53L4CD_API::I2C_WrByte(uint16_t reg, uint8_t value)
{	


	uint8_t original_value = i2c_read_Byte(reg);
	
	uint8_t data_write[3];
	data_write[0] = (reg >> 8) & 0xFF;
	data_write[1] = reg & 0xFF;
	data_write[2] = value & 0xFF;
	long int r = write(fd_i2c,&data_write,3);
	if(r < 0){
		printf("%i",r);
	#ifdef DEBUG
                fprintf(stderr,"Could not read word from %02x. ret=%d.\n",address,r);
	#endif
             throw "Could not read from i2c.";
	}

	//printf("Register:%x\nValue:%x\n",reg,value);


	uint8_t written_value = i2c_read_Byte(reg);

	if(written_value != value){

		printf("\noh this byte could be wrong:%x\n",reg);
		printf("you wanted to write:%x\nI read:%x\n",value,written_value);
		printf("here's what I have stored in array element[0],[1].[2] :%x,%x,%x\n\n",data_write[0],data_write[1],data_write[2]);

	}

	//if (written_value != original_value){


	//	printf("we have a printer\n");
	//}


}


void VL53L4CD_API::DataReady(){
	//	printf("I Have Data For You\n");
	uint16_t v = i2c_read_conversion(VL53L4CD_RESULT__DISTANCE);
	for(auto &cb: adsCallbackInterface){
		cb -> hasVL53L4CDSample(v);
	}

	printf("%u\n",v);
}

void VL53L4CD_API::stop_recording_data(){
	if(!running) return;
	running = false;
	//thr.join();
	gpiod_line_release(pinDRDY);
	gpiod_chip_close(chipDRDY);
	close(fd_i2c);
}

void VL53L4CD_API::worker()
{
	//while(running) {

		//timespec with constant timeout duration
		const struct timespec ts = {1,0};
		gpiod_line_event_wait(pinDRDY, &ts);
		struct gpiod_line_event event;
		gpiod_line_event_read(pinDRDY, &event);
		DataReady();
	//}
}

int VL53L4CD_API::i2c_read_conversion(uint8_t reg){

	uint8_t data_array[2];
	data_array[0] = (reg>>8) & 0xFF;
	data_array[1] = reg & 0xFF;
	write(fd_i2c,data_array,2);


	uint8_t read_data[2];
    long int r = read(fd_i2c, read_data, 2);


	if(r < 0){
	#ifdef DEBUG
               fprintf(stderr,"Could not read word from %02x. ret=%d.\n",address,r);
	#endif
                throw "Could not read from i2c.";
	}

	return (((unsigned)(read_data[0])) << 8) | ((unsigned)(read_data[1]));

};

int VL53L4CD_API::i2c_read_Byte(uint16_t reg){

	uint8_t data_array[2];
	data_array[0] = (reg>>8) & 0xFF;
	data_array[1] = reg & 0xFF;
	write(fd_i2c,data_array,2);


	uint8_t read_data[1];
    long int r = read(fd_i2c, read_data, 1);


	if(r < 0){
	#ifdef DEBUG
               fprintf(stderr,"Could not read word from %02x. ret=%d.\n",address,r);
	#endif
                throw "Could not read from i2c.";
	}

	return ((uint8_t)(read_data[0]));



};

void VL53L4CD_API::VL53L4CD_SetRangeTiming_RealTime(
		uint32_t timing_budget_ms,
		uint32_t inter_measurement_ms)
{
	uint16_t clock_pll, osc_frequency, ms_byte;
	uint32_t macro_period_us = 0, timing_budget_us = 0, ls_byte, tmp;
	float_t inter_measurement_factor = (float_t)1.055;

	if(osc_frequency != (uint16_t)0)
	{
		timing_budget_us = timing_budget_ms*(uint32_t)1000;
		macro_period_us = (uint32_t)((uint32_t)2304 *
		((uint32_t)0x40000000 / (uint32_t)osc_frequency)) >> 6;
	}
	else
	{
		printf("\nRead Conversion Fail\n");
	}

	//Timing budget check validity 
	if ((timing_budget_ms < (uint32_t)10) 
			|| (timing_budget_ms > (uint32_t)200))
	{
		printf("\nTiming Budget Fail\n");
	}
	//Sensor runs in continuous mode
	else if(inter_measurement_ms == (uint32_t)0)
	{
		I2C_Wr_four_bytes(VL53L4CD_INTERMEASUREMENT_MS, 0);
		printf("you have selected continuous mode\n");
		timing_budget_us -= (uint32_t)2500;
	}
	//Sensor runs in autonomous low power mode
	else if(inter_measurement_ms > timing_budget_ms)
	{	
		printf("you have selected autonomous low power mode\n");
		clock_pll = i2c_read_conversion(
				VL53L4CD_RESULT__OSC_CALIBRATE_VAL);
		clock_pll = clock_pll & (uint16_t)0x3FF;
				inter_measurement_factor = inter_measurement_factor
				  * (float_t)inter_measurement_ms
				  * (float_t)clock_pll;
		I2C_Wr_four_bytes(VL53L4CD_INTERMEASUREMENT_MS,
				(uint32_t)inter_measurement_factor);

		timing_budget_us -= (uint32_t)4300;
		timing_budget_us /= (uint32_t)2;

	}

		ms_byte = 0;
		timing_budget_us = timing_budget_us << 12;
		tmp = macro_period_us*(uint32_t)16;
		ls_byte = ((timing_budget_us + ((tmp >> 6)>>1)) /(tmp>> 6))
			- (uint32_t)1;

		while ((ls_byte & 0xFFFFFF00U) > 0U) {
					 ls_byte = ls_byte >> 1;
					 ms_byte++;
			}
		ms_byte = (uint16_t)(ms_byte << 8)
		+ (uint16_t) (ls_byte & (uint32_t)0xFF);
		I2C_WrWord(VL53L4CD_RANGE_CONFIG_A,ms_byte);

		ms_byte = 0;
		tmp = macro_period_us*(uint32_t)12;
		ls_byte = ((timing_budget_us + ((tmp >> 6)>>1)) /(tmp>> 6))
			- (uint32_t)1;

		while ((ls_byte & 0xFFFFFF00U) > 0U) {
				 ls_byte = ls_byte >> 1;
				 ms_byte++;
		}
		ms_byte = (uint16_t)(ms_byte << 8)
		+ (uint16_t) (ls_byte & (uint32_t)0xFF);
		I2C_WrWord(VL53L4CD_RANGE_CONFIG_B,ms_byte);

}


/*end of added by BruGenie*/

/*
VL53L4CD_Error VL53L4CD_API::VL53L4CD_GetSWVersion(
		VL53L4CD_Version_t *p_Version)
{
	VL53L4CD_Error Status = VL53L4CD_ERROR_NONE;

	p_Version->major = VL53L4CD_IMPLEMENTATION_VER_MAJOR;
	p_Version->minor = VL53L4CD_IMPLEMENTATION_VER_MINOR;
	p_Version->build = VL53L4CD_IMPLEMENTATION_VER_BUILD;
	p_Version->revision = VL53L4CD_IMPLEMENTATION_VER_REVISION;
	return Status;
}

VL53L4CD_Error VL53L4CD_API::VL53L4CD_SetI2CAddress(
		Dev_t dev,
		uint8_t new_address)
{
	VL53L4CD_Error status = VL53L4CD_ERROR_NONE;

	status |= VL53L4CD_WrByte(dev, VL53L4CD_I2C_SLAVE__DEVICE_ADDRESS,
			(uint8_t)(new_address >> (uint8_t)1));
	return status;
}

VL53L4CD_Error VL53L4CD_API::VL53L4CD_GetSensorId(
		Dev_t dev,
		uint16_t *p_id)
{
	VL53L4CD_Error status = VL53L4CD_ERROR_NONE;

	status |= VL53L4CD_RdWord(dev, VL53L4CD_IDENTIFICATION__MODEL_ID, p_id);
	return status;
}
*/
/*
VL53L4CD_Error VL53L4CD_API::VL53L4CD_SensorInit(
		Dev_t dev)
{
	VL53L4CD_Error status = VL53L4CD_ERROR_NONE;
	uint8_t Addr, tmp;
	uint8_t continue_loop = 1;
	uint16_t i = 0;

	do{
		status |= VL53L4CD_RdByte(dev,
				VL53L4CD_FIRMWARE__SYSTEM_STATUS, &tmp);

		if(tmp == (uint8_t)0x3) //Sensor booted
		{
			continue_loop = (uint8_t)0;
		}
		else if(i < (uint16_t)1000)       //Wait for boot
		{
			i++;
		}
		else // Timeout 1000ms reached
		{
			continue_loop = (uint8_t)0;
			status |= (uint8_t)VL53L4CD_ERROR_TIMEOUT;
		}
		VL53L4CD_WaitMs(dev, 1);
	}while(continue_loop == (uint8_t)1);

	// Load default configuration
	for (Addr = (uint8_t)0x2D; Addr <= (uint8_t)0x87; Addr++)
	{
		status |= VL53L4CD_WrByte(dev, Addr,
				VL53L4CD_DEFAULT_CONFIGURATION[
                                  Addr - (uint8_t)0x2D]);
	}

	///Start VHV 
	status |= VL53L4CD_WrByte(dev, VL53L4CD_SYSTEM_START, (uint8_t)0x40);
	i  = (uint8_t)0;
	continue_loop = (uint8_t)1;
	do{
		status |= VL53L4CD_CheckForDataReady(dev, &tmp);
		if(tmp == (uint8_t)1) // Data ready 
		{
			continue_loop = (uint8_t)0;
		}
		else if(i < (uint16_t)1000)       // Wait for answer
		{
			i++;
		}
		else // Timeout 1000ms reached
		{
			continue_loop = (uint8_t)0;
			status |= (uint8_t)VL53L4CD_ERROR_TIMEOUT;
		}
		VL53L4CD_WaitMs(dev, 1);
	}while(continue_loop == (uint8_t)1);

	status |= VL53L4CD_ClearInterrupt(dev);
	status |= VL53L4CD_StopRanging(dev);
	status |= VL53L4CD_WrByte(dev,
			VL53L4CD_VHV_CONFIG__TIMEOUT_MACROP_LOOP_BOUND, 
                        (uint8_t)0x09);
	status |= VL53L4CD_WrByte(dev, 0x0B, (uint8_t)0);
	status |= VL53L4CD_WrWord(dev, 0x0024, 0x500);

	status |= VL53L4CD_SetRangeTiming(dev, 50, 0);

	return status;
}
*/
/*
VL53L4CD_Error VL53L4CD_API::VL53L4CD_ClearInterrupt(
		Dev_t dev)
{
	VL53L4CD_Error status = VL53L4CD_ERROR_NONE;

	status |= VL53L4CD_WrByte(dev, VL53L4CD_SYSTEM__INTERRUPT_CLEAR, 0x01);
	return status;
}
*/
/*
VL53L4CD_Error VL53L4CD_API::VL53L4CD_StartRanging(
		Dev_t dev)
{
	VL53L4CD_Error status = VL53L4CD_ERROR_NONE;
	uint32_t tmp;

	status |= VL53L4CD_RdDWord(dev, VL53L4CD_INTERMEASUREMENT_MS, &tmp);

	//Sensor runs in continuous mode
	if(tmp == (uint32_t)0)
	{
		status |= VL53L4CD_WrByte(dev, VL53L4CD_SYSTEM_START, 0x21);
	}
	//Sensor runs in autonomous mode
	else
	{
		status |= VL53L4CD_WrByte(dev, VL53L4CD_SYSTEM_START, 0x40);
	}

	return status;
}

VL53L4CD_Error VL53L4CD_API::VL53L4CD_StopRanging(
		Dev_t dev)
{
	VL53L4CD_Error status = VL53L4CD_ERROR_NONE;

	status |= VL53L4CD_WrByte(dev, VL53L4CD_SYSTEM_START, 0x80);
	return status;
}

VL53L4CD_Error VL53L4CD_API::VL53L4CD_CheckForDataReady(
		Dev_t dev,
		uint8_t *p_is_data_ready)
{
	VL53L4CD_Error status = VL53L4CD_ERROR_NONE;
	uint8_t temp;
	uint8_t int_pol;

	status |= VL53L4CD_RdByte(dev, VL53L4CD_GPIO_HV_MUX__CTRL, &temp);
	temp = temp & (uint8_t)0x10;
	temp = temp >> 4;

	if (temp == 	(uint8_t)1)
	{
		int_pol = (uint8_t)0;
	}
	else
	{
		int_pol = (uint8_t)1;
	}

	status |= VL53L4CD_RdByte(dev, VL53L4CD_GPIO__TIO_HV_STATUS, &temp);

	if ((temp & (uint8_t)1) == int_pol)
	{
		*p_is_data_ready = (uint8_t)1;
	}
	else
	{
		*p_is_data_ready = (uint8_t)0;
	}

	return status;
}

*/

/*
VL53L4CD_Error VL53L4CD_API::VL53L4CD_SetRangeTiming(
		Dev_t dev,
		uint32_t timing_budget_ms,
		uint32_t inter_measurement_ms)
{
	VL53L4CD_Error status = VL53L4CD_ERROR_NONE;
	uint16_t clock_pll, osc_frequency, ms_byte;
	uint32_t macro_period_us = 0, timing_budget_us = 0, ls_byte, tmp;
	float_t inter_measurement_factor = (float_t)1.055;

	status |= VL53L4CD_RdWord(dev, 0x0006, &osc_frequency);
	if(osc_frequency != (uint16_t)0)
	{
		timing_budget_us = timing_budget_ms*(uint32_t)1000;
		macro_period_us = (uint32_t)((uint32_t)2304 *
		((uint32_t)0x40000000 / (uint32_t)osc_frequency)) >> 6;
	}
	else
	{
		status |= (uint8_t)VL53L4CD_ERROR_INVALID_ARGUMENT;
	}

	//Timing budget check validity 
	if ((timing_budget_ms < (uint32_t)10) 
			|| (timing_budget_ms > (uint32_t)200) || (status != (uint8_t)0))
	{
		status |= VL53L4CD_ERROR_INVALID_ARGUMENT;
	}
	//Sensor runs in continuous mode
	else if(inter_measurement_ms == (uint32_t)0)
	{
		status |= VL53L4CD_WrDWord(dev,VL53L4CD_INTERMEASUREMENT_MS, 0);
		timing_budget_us -= (uint32_t)2500;
	}
	//Sensor runs in autonomous low power mode
	else if(inter_measurement_ms > timing_budget_ms)
	{
		status |= VL53L4CD_RdWord(dev,
				VL53L4CD_RESULT__OSC_CALIBRATE_VAL, &clock_pll);
		clock_pll = clock_pll & (uint16_t)0x3FF;
				inter_measurement_factor = inter_measurement_factor
				  * (float_t)inter_measurement_ms
				  * (float_t)clock_pll;
		status |= VL53L4CD_WrDWord(dev, VL53L4CD_INTERMEASUREMENT_MS,
				(uint32_t)inter_measurement_factor);

		timing_budget_us -= (uint32_t)4300;
		timing_budget_us /= (uint32_t)2;

	}
	//Invalid case
	else
	{
		status |= (uint8_t)VL53L4CD_ERROR_INVALID_ARGUMENT;
	}

	if(status != (uint8_t)VL53L4CD_ERROR_INVALID_ARGUMENT)
	{
				ms_byte = 0;
				timing_budget_us = timing_budget_us << 12;
				tmp = macro_period_us*(uint32_t)16;
				ls_byte = ((timing_budget_us + ((tmp >> 6)>>1)) /(tmp>> 6))
				  - (uint32_t)1;

				while ((ls_byte & 0xFFFFFF00U) > 0U) {
						 ls_byte = ls_byte >> 1;
						 ms_byte++;
				}
				ms_byte = (uint16_t)(ms_byte << 8)
			+ (uint16_t) (ls_byte & (uint32_t)0xFF);
				status |= VL53L4CD_WrWord(dev, VL53L4CD_RANGE_CONFIG_A,ms_byte);

				ms_byte = 0;
				tmp = macro_period_us*(uint32_t)12;
				ls_byte = ((timing_budget_us + ((tmp >> 6)>>1)) /(tmp>> 6))
				  - (uint32_t)1;

				while ((ls_byte & 0xFFFFFF00U) > 0U) {
						 ls_byte = ls_byte >> 1;
						 ms_byte++;
				}
				ms_byte = (uint16_t)(ms_byte << 8)
			+ (uint16_t) (ls_byte & (uint32_t)0xFF);
				status |= VL53L4CD_WrWord(dev, VL53L4CD_RANGE_CONFIG_B,ms_byte);
	}

	return status;
}
*/

/*
VL53L4CD_Error VL53L4CD_API::VL53L4CD_GetRangeTiming(
		Dev_t dev,
		uint32_t *p_timing_budget_ms,
		uint32_t *p_inter_measurement_ms)
{
	VL53L4CD_Error status = VL53L4CD_ERROR_NONE;
	uint16_t osc_frequency = 1, range_config_macrop_high, clock_pll = 1;
	uint32_t tmp, ls_byte, ms_byte, macro_period_us;
	float_t clock_pll_factor = (float_t)1.065;

	// Get InterMeasurement 
	status |= VL53L4CD_RdDWord(dev, VL53L4CD_INTERMEASUREMENT_MS, &tmp);
	status |= VL53L4CD_RdWord(dev,
			VL53L4CD_RESULT__OSC_CALIBRATE_VAL, &clock_pll);
	clock_pll = clock_pll & (uint16_t)0x3FF;
	clock_pll_factor = clock_pll_factor * (float_t)clock_pll;
	clock_pll = (uint16_t)clock_pll_factor;
	*p_inter_measurement_ms = (uint16_t)(tmp/(uint32_t)clock_pll);

	//Get TimingBudget 
	status |= VL53L4CD_RdWord(dev, 0x0006, &osc_frequency);
	status |= VL53L4CD_RdWord(dev, VL53L4CD_RANGE_CONFIG_A,
		&range_config_macrop_high);

	macro_period_us = (uint32_t)((uint32_t)2304 * ((uint32_t)0x40000000
			/ (uint32_t)osc_frequency)) >> 6;
	ls_byte = (range_config_macrop_high & (uint32_t)0x00FF) << 4;
	ms_byte = (range_config_macrop_high & (uint32_t)0xFF00) >> 8;
	ms_byte = (uint32_t)0x04 - (ms_byte - (uint32_t)1) - (uint32_t)1;

	macro_period_us = macro_period_us * (uint32_t)16;
	*p_timing_budget_ms = (((ls_byte + (uint32_t)1)*(macro_period_us>> 6))
		 - ((macro_period_us>> 6)>>1)) >> 12;

	if(ms_byte < (uint8_t)12)
	{
		  *p_timing_budget_ms = (uint32_t)(*p_timing_budget_ms
				   >> (uint8_t)ms_byte);
	}
	 
	 //Mode continuous
	 if(tmp == (uint32_t)0)
	 {
		*p_timing_budget_ms += (uint32_t)2500;
	 }
	 // Mode autonomous
	 else
	 {
	      *p_timing_budget_ms *= (uint32_t)2;
	      *p_timing_budget_ms += (uint32_t)4300;
	 }

	 *p_timing_budget_ms = *p_timing_budget_ms/(uint32_t)1000;

	return status;
} */

/*

VL53L4CD_Error VL53L4CD_API::VL53L4CD_GetResult(
		Dev_t dev,
		VL53L4CD_ResultsData_t *p_result)
{
	VL53L4CD_Error status = VL53L4CD_ERROR_NONE;
	uint16_t temp_16;
	uint8_t temp_8;
	uint8_t status_rtn[24] = { 255, 255, 255, 5, 2, 4, 1, 7, 3,
			0, 255, 255, 9, 13, 255, 255, 255, 255, 10, 6,
			255, 255, 11, 12 };

	status |= VL53L4CD_RdByte(dev, VL53L4CD_RESULT__RANGE_STATUS,
		&temp_8);
	temp_8 = temp_8 & (uint8_t)0x1F;
	if (temp_8 < (uint8_t)24)
	{
		temp_8 = status_rtn[temp_8];
	}
	p_result->range_status = temp_8;

	status |= VL53L4CD_RdWord(dev, VL53L4CD_RESULT__SPAD_NB,
		&temp_16);
	p_result->number_of_spad = temp_16 / (uint16_t) 256;

	status |= VL53L4CD_RdWord(dev, VL53L4CD_RESULT__SIGNAL_RATE,
		&temp_16);
	p_result->signal_rate_kcps = temp_16 * (uint16_t) 8;

	status |= VL53L4CD_RdWord(dev, VL53L4CD_RESULT__AMBIENT_RATE,
		&temp_16);
	p_result->ambient_rate_kcps = temp_16 * (uint16_t) 8;

	status |= VL53L4CD_RdWord(dev, VL53L4CD_RESULT__SIGMA,
		&temp_16);
	p_result->sigma_mm = temp_16 / (uint16_t) 4;

	status |= VL53L4CD_RdWord(dev, VL53L4CD_RESULT__DISTANCE,
		&temp_16);
	p_result->distance_mm = temp_16;

	p_result->signal_per_spad_kcps = p_result->signal_rate_kcps
			/p_result->number_of_spad;
	p_result->ambient_per_spad_kcps = p_result->ambient_rate_kcps
			/p_result->number_of_spad;

	return status;
}*/

/*

VL53L4CD_Error VL53L4CD_API::VL53L4CD_SetOffset(
		Dev_t dev,
		int16_t OffsetValueInMm)
{
	VL53L4CD_Error status = VL53L4CD_ERROR_NONE;
	uint16_t temp;

	temp = (uint16_t)((uint16_t)OffsetValueInMm*(uint16_t)4);

	status |= VL53L4CD_WrWord(dev, VL53L4CD_RANGE_OFFSET_MM, temp);
	status |= VL53L4CD_WrWord(dev, VL53L4CD_INNER_OFFSET_MM, (uint8_t)0x0);
	status |= VL53L4CD_WrWord(dev, VL53L4CD_OUTER_OFFSET_MM, (uint8_t)0x0);
	return status;
}

VL53L4CD_Error  VL53L4CD_API::VL53L4CD_GetOffset(
		Dev_t dev,
		int16_t *p_offset)
{
	VL53L4CD_Error status = VL53L4CD_ERROR_NONE;
	uint16_t temp;

	status |= VL53L4CD_RdWord(dev,VL53L4CD_RANGE_OFFSET_MM, &temp);

	temp = temp<<3;
	temp = temp>>5;
	*p_offset = (int16_t)(temp);

	if(*p_offset > 1024)
	{
		*p_offset = *p_offset - 2048;
	}

	return status;
}

VL53L4CD_Error VL53L4CD_API::VL53L4CD_SetXtalk(
		Dev_t dev,
		uint16_t XtalkValueKcps)
{
	VL53L4CD_Error status = VL53L4CD_ERROR_NONE;

	status |= VL53L4CD_WrWord(dev,
		VL53L4CD_XTALK_X_PLANE_GRADIENT_KCPS, 0x0000);
	status |= VL53L4CD_WrWord(dev,
		VL53L4CD_XTALK_Y_PLANE_GRADIENT_KCPS, 0x0000);
	status |= VL53L4CD_WrWord(dev,
		VL53L4CD_XTALK_PLANE_OFFSET_KCPS,
		(XtalkValueKcps<<9));
        
	return status;
}

VL53L4CD_Error VL53L4CD_API::VL53L4CD_GetXtalk(
		Dev_t dev,
		uint16_t *p_xtalk_kcps)
{
	VL53L4CD_Error status = VL53L4CD_ERROR_NONE;
	float_t tmp_xtalk;

	status |= VL53L4CD_RdWord(dev,
		VL53L4CD_XTALK_PLANE_OFFSET_KCPS, p_xtalk_kcps);
		
	tmp_xtalk = (float_t)*p_xtalk_kcps / (float_t)512.0;
	*p_xtalk_kcps = (uint16_t)(round(tmp_xtalk));

	return status;
}

VL53L4CD_Error VL53L4CD_API::VL53L4CD_SetDetectionThresholds(
		Dev_t dev,
		uint16_t distance_low_mm,
		uint16_t distance_high_mm,
		uint8_t window)
{
	VL53L4CD_Error status = VL53L4CD_ERROR_NONE;

	status |= VL53L4CD_WrByte(dev, VL53L4CD_SYSTEM__INTERRUPT, window);
	status |= VL53L4CD_WrWord(dev, VL53L4CD_THRESH_HIGH, distance_high_mm);
	status |= VL53L4CD_WrWord(dev, VL53L4CD_THRESH_LOW, distance_low_mm);
	return status;
}

VL53L4CD_Error VL53L4CD_API::VL53L4CD_GetDetectionThresholds(Dev_t dev,
		uint16_t *p_distance_low_mm,
		uint16_t *p_distance_high_mm,
		uint8_t *p_window)
{
	VL53L4CD_Error status = VL53L4CD_ERROR_NONE;

	status |= VL53L4CD_RdWord(dev, VL53L4CD_THRESH_HIGH,p_distance_high_mm);
	status |= VL53L4CD_RdWord(dev, VL53L4CD_THRESH_LOW, p_distance_low_mm);
	status |= VL53L4CD_RdByte(dev, VL53L4CD_SYSTEM__INTERRUPT, p_window);
	*p_window = (*p_window & (uint8_t)0x7);

	return status;
}

VL53L4CD_Error VL53L4CD_API::VL53L4CD_SetSignalThreshold(
		Dev_t dev,
		uint16_t signal_kcps)
{
	VL53L4CD_Error status = VL53L4CD_ERROR_NONE;

	status |= VL53L4CD_WrWord(dev,
			VL53L4CD_MIN_COUNT_RATE_RTN_LIMIT_MCPS,signal_kcps>>3);
	return status;
}

VL53L4CD_Error VL53L4CD_API::VL53L4CD_GetSignalThreshold(
		Dev_t dev,
		uint16_t 	*p_signal_kcps)
{
	VL53L4CD_Error status = VL53L4CD_ERROR_NONE;
	uint16_t tmp = 0;

	status |= VL53L4CD_RdWord(dev,
			VL53L4CD_MIN_COUNT_RATE_RTN_LIMIT_MCPS, &tmp);
	*p_signal_kcps = tmp <<3;

	return status;
}

VL53L4CD_Error VL53L4CD_API::VL53L4CD_SetSigmaThreshold(
		Dev_t dev,
		uint16_t 	sigma_mm)
{
	VL53L4CD_Error status = VL53L4CD_ERROR_NONE;

	if(sigma_mm>(uint16_t)((uint16_t)0xFFFF>>2))
	{
		status |= (uint8_t)VL53L4CD_ERROR_INVALID_ARGUMENT;
	}
	else
	{
		status |= VL53L4CD_WrWord(dev,
			VL53L4CD_RANGE_CONFIG__SIGMA_THRESH, sigma_mm<<2);
	}

	return status;
}

VL53L4CD_Error VL53L4CD_API::VL53L4CD_GetSigmaThreshold(
		Dev_t dev,
		uint16_t 	*p_sigma_mm)
{
	VL53L4CD_Error status = VL53L4CD_ERROR_NONE;

	status += VL53L4CD_RdWord(dev,
			VL53L4CD_RANGE_CONFIG__SIGMA_THRESH, p_sigma_mm);
	*p_sigma_mm = *p_sigma_mm >> 2;

	return status;
}

VL53L4CD_Error VL53L4CD_API::VL53L4CD_StartTemperatureUpdate(
		Dev_t dev)
{
	VL53L4CD_Error status = VL53L4CD_ERROR_NONE;
	uint8_t tmp = 0, continue_loop = 1;
	uint16_t i = 0;

	status |= VL53L4CD_WrByte(dev,
		VL53L4CD_VHV_CONFIG__TIMEOUT_MACROP_LOOP_BOUND, (uint8_t)0x81);
	status |= VL53L4CD_WrByte(dev, 0x0B, (uint8_t)0x92);
	status |= VL53L4CD_StartRanging(dev);

	do{
			status |= VL53L4CD_CheckForDataReady(dev, &tmp);
			if(tmp == (uint8_t)1) //Data ready
			{
					continue_loop = (uint8_t)0;
			}
			else if(i < (uint16_t)1000)       //Wait for answer
			{
					i++;
			}
			else // Timeout 1000ms reached 
			{
					continue_loop = (uint8_t)0;
					status = (uint8_t)VL53L4CD_ERROR_TIMEOUT;
			}
			VL53L4CD_WaitMs(dev, 1);
	}while(continue_loop == (uint8_t)1);

	status |= VL53L4CD_ClearInterrupt(dev);
	status |= VL53L4CD_StopRanging(dev);

	status += VL53L4CD_WrByte(dev,
		//VL53L4CD_VHV_CONFIG__TIMEOUT_MACROP_LOOP_BOUND, 0x09);
	status += VL53L4CD_WrByte(dev, 0x0B, 0);
	return status;
}

*/
