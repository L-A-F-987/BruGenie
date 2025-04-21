//test to boot a sensor and test he I2C lines

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "../uld-driver/VL53L4CD_api.h"

int example2()
{

	VL53L4CD_API VL53L4CD;

	VL53L4CD.boot_sensor(0x29,14);
	VL53L4CD.VL53L4CD_SetI2CAddress((uint8_t)0x28);
	
  usleep(100000);
	VL53L4CD.stop_recording_data();



	printf("End of test demo\n");

}
