
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include "../uld-driver/VL53L4CD_api.h"


int example1()
{

	VL53L4CD_API VL53L4CD;

	VL53L4CD.boot_sensor();
	VL53L4CD.start_recording_data();
	getchar();
	getchar();
	usleep(1000); //short sleep to ensure that sensor readings have come in before values are accessed
	VL53L4CD.stop_recording_data();

	printf("End of ULD demo\n");
}
