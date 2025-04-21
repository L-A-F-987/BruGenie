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


/***********************************/
/*     VL53L4CD ULD low power      */
/***********************************/
/*
* This example shows an example of low power usage. It initializes the VL53L4CD
* ULD, configure the sensor and starts a ranging to capture 200 frames.
*
*/

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "../uld-driver/VL53L4CD_api.h"

int example2()
{

	VL53L4CD_API VL53L4CD;

	VL53L4CD.boot_sensor(0x29,14);
	VL53L4CD.VL53L4CD_SetI2CAddress((uint8_t)0x28);
	

	//getchar();
	//VL53L4CD.stop_sensor_ranging();
	VL53L4CD.stop_recording_data();


	//status = VL53L4CD.VL53L4CD_StopRanging(dev);
	printf("End of ULD demo\n");
	//return status;

}
