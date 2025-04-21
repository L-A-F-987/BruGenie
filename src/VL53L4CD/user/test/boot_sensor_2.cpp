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


#include <unistd.h>
#include <signal.h>
#include <dlfcn.h>

#include <stdio.h>
#include <string.h>

#include "../uld-driver/VL53L4CD_api.h"

int main()
{
	VL53L4CD_API VL53L4CD;

	VL53L4CD.boot_sensor(0x29,14);
	VL53L4CD.start_recording_data();
	usleep(100000); 
	VL53L4CD.stop_recording_data();

	printf("End of boot test\n");

	return 0;
}
