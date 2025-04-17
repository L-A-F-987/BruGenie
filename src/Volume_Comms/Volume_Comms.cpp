
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>

#include "Volume_Comms.h"


int Volume_Comms::Start_sensor_1()
{

	x_shut_6.start(6);
	x_shut_6.on();
	x_shut_6.stop();
	
	TOF_1.boot_sensor((uint8_t)0x28, 14);
	TOF_2.boot_sensor((uint8_t)0x29, 27);

	//TOF_2.VL53L4CD_SetI2CAddress((uint8_t)0x28);
	//TOF_2.VL53L4CD_SetI2CAddress((uint8_t)0x29);
	TOF_1.start_recording_data();
	TOF_2.start_recording_data();
	//VL53L4CD.VL53L4CD_SetI2CAddress((uint8_t)0x28);

	/*
	getchar();
	getchar();
	usleep(1000);
	TOF_1.stop_recording_data();
	TOF_2.stop_recording_data();

	*/
	getchar();
	getchar();
	usleep(1000);
	TOF_1.stop_recording_data();
	TOF_2.stop_recording_data();

	


	printf("End of ULD demo\n");
};
