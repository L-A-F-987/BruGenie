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

void VL53L4CD_API::boot_sensor()
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

	printf("interrupt setting register:%x\n\n",i2c_read_Byte(0x87));
	//setting up sensor configuration
	for (Addr = (uint16_t)0x2D; Addr <= (uint16_t)0x87; Addr++)
	{	
		I2C_WrByte(Addr,
				VL53L4CD_DEFAULT_CONFIGURATION[
                                  Addr - (uint16_t)0x2D]);
	
	//printf("16bit:%X\n8bit:%X\n",Addr,(uint8_t)Addr);
	}

	printf("interrupt setting register:%x\n\n",i2c_read_Byte(0x87));
	
	running = true;
	
	//continue_loop = (uint8_t)0;

	//}while(continue_loop == (uint8_t)1);

	printf("first start %x and interrupt%x\n",i2c_read_Byte(VL53L4CD_SYSTEM_START),i2c_read_Byte(VL53L4CD_SYSTEM__INTERRUPT_CLEAR));

	I2C_WrByte(0x87, (uint8_t)0x40);

	printf("first start %x and interrupt%x\n",i2c_read_Byte(VL53L4CD_SYSTEM_START),i2c_read_Byte(VL53L4CD_SYSTEM__INTERRUPT_CLEAR));	

	//waiting for data ready before moving on

	//adding a gpio event type to wait on 

	I2C_WrByte(0x86, 0x01);

	int ret = gpiod_line_request_falling_edge_events(pinDRDY,"Consumer");
	const struct timespec ts = {1,0};
	gpiod_line_event_wait(pinDRDY, &ts);
	struct gpiod_line_event event;
	gpiod_line_event_read(pinDRDY, &event);

	//Clearing System Interrupt
	I2C_WrByte(0x86, 0x01);

	//stopping ranging
	I2C_WrByte(0x87, (uint8_t)0x80);

	//setting ????
	I2C_WrByte(VL53L4CD_VHV_CONFIG__TIMEOUT_MACROP_LOOP_BOUND,(uint8_t)0x09);

	I2C_WrByte(0x0B, (uint8_t)0);

	I2C_WrWord(0x0024, (uint8_t)0x500);

	VL53L4CD_SetRangeTiming_RealTime(200,0);
}


void VL53L4CD_API::start_recording_data(){


	uint8_t tmp;

	tmp = i2c_read_conversion(VL53L4CD_INTERMEASUREMENT_MS);
	
	///* Sensor runs in continuous mode */
	if(tmp == (uint8_t)0){

		I2C_WrByte(0x87, (uint8_t)0x21);
	
	}
	/* Sensor runs in autonomous mode */
	else{
		I2C_WrByte(0x87, (uint8_t)0x40);
	}


	I2C_WrByte(0x86, 0x01);
	thr = std::thread(&VL53L4CD_API::worker,this);

}

void VL53L4CD_API::DataReady(){
	//	printf("I Have Data For You\n");
	uint16_t v = i2c_read_conversion(VL53L4CD_RESULT__DISTANCE);
	//for(auto &cb: adsCallbackInterface){
	//	cb -> hasVL53L4CDSample(v);
	//}

	printf("%u\n",v);
}


void VL53L4CD_API::stop_recording_data(){
	if(!running) return;
	running = false;
	thr.join();
	gpiod_line_release(pinDRDY);
	gpiod_chip_close(chipDRDY);
	close(fd_i2c);
}


void VL53L4CD_API::worker()
{	
	printf("i'm here");
	I2C_WrByte(0x86, 0x01);
	int ret = gpiod_line_request_falling_edge_events(pinDRDY,"Consumer");
	while(running) {

		//timespec with constant timeout duration
		I2C_WrByte(0x86, 0x01);
		const struct timespec ts = {1,0};
		gpiod_line_event_wait(pinDRDY, &ts);
		struct gpiod_line_event event;
		gpiod_line_event_read(pinDRDY, &event);
		DataReady();
	}
	
}

void VL53L4CD_API::stop_sensor_ranging(){

	I2C_WrByte(0x87, (uint8_t)0x80);

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


	uint16_t written_value = i2c_read_conversion(reg);

	if(written_value != (uint16_t)value){

		printf("oh this word is wrong\n");

	}




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

	uint16_t written_value = i2c_read_conversion(reg);

}

uint8_t VL53L4CD_API::I2C_WrByte(uint16_t reg, uint8_t value)
{	


	//uint8_t original_value = i2c_read_Byte(reg);
	
	uint8_t data_write[3];
	data_write[0] = (reg >> 8) & 0xFF;
	data_write[1] = reg & 0xFF;
	data_write[2] = value;
	long int r = write(fd_i2c,&data_write,3);
	if(r < 0){
		printf("%i",r);
	#ifdef DEBUG
                fprintf(stderr,"Could not read word from %02x. ret=%d.\n",address,r);
	#endif
             throw "Could not read from i2c.";
	}


	//uint8_t written_value = i2c_read_Byte(reg);


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
		I2C_WrWord(VL53L4CD_INTERMEASUREMENT_MS, 0);
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
		I2C_WrWord(VL53L4CD_INTERMEASUREMENT_MS,
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

void VL53L4CD_API::VL53L4CD_SetI2CAddress(
		uint8_t new_address)
{

	I2C_WrByte(VL53L4CD_I2C_SLAVE__DEVICE_ADDRESS,
			(uint8_t)(new_address));


	address =  new_address;


}


/*end of added by BruGenie*/


