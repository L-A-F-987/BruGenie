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

#include "VL53L4CD_api.h"

#include "examples.h"

int exit_main_loop = 0;

void sighandler(int signal)
{
	printf("SIGNAL Handler called, signal = %d\n", signal);
	exit_main_loop  = 1;
}

int main(int argc, char ** argv)
{
	char choice[20];
	int status;
	VL53L4CD_LinuxDev LinuxDev;
	Dev_t Dev = &LinuxDev;
	VL53L4CD_Version_t TOF_SW_Version;

	/*********************************/
	/*   Power on sensor and init    */
	/*********************************/

	/* Initialize channel com */
	status = VL53L4CD_comms_init(Dev);
	if(status)
	{
		printf("VL53L4CD comms init failed\n");
		return -1;
	}
	
	status = VL53L4CD_GetSWVersion(&TOF_SW_Version);
	printf("Starting examples of VL53L4CD driver (version %u.%u.%u.%u)\n",
		TOF_SW_Version.major,
		TOF_SW_Version.minor,
		TOF_SW_Version.build,
		TOF_SW_Version.revision);

	do {
		printf("----------------------------------------------------------------------------------------------------------\n");
		printf(" VL53L4CD uld driver test example menu \n");
		printf(" ------------------ Ranging menu ------------------\n");
		printf(" 1 : basic ranging\n");
		printf(" 2 : low power\n");
		printf(" 3 : high accuracy\n");
		printf(" 4 : fast ranging\n");
		printf(" 5 : calibrate offset and Xatlk\n");
		printf(" 6 : detection thresholds - need to catch GPIO1 interrupt for this example\n");
		printf(" 7 : exit\n");
		printf("----------------------------------------------------------------------------------------------------------\n");

		printf("Your choice ?\n ");
		scanf("%s", choice);

		if (strcmp(choice, "1") == 0) {
			printf("Starting Test 1\n");
			status = example1(Dev);
			printf("\n");
		}
		else if (strcmp(choice, "2") == 0) {
			printf("Starting Test 2\n");
			status = example2(Dev);
			printf("\n");
		}
		else if (strcmp(choice, "3") == 0) {
			printf("Starting Test 3\n");
			status = example3(Dev);
			printf("\n");
		}
		else if (strcmp(choice, "4") == 0) {
			printf("Starting Test 4\n");
			status = example4(Dev);
			printf("\n");
		}
		else if (strcmp(choice, "5") == 0) {
			printf("Starting Test 5\n");
			status = example5(Dev);
			printf("\n");
		}
		else if (strcmp(choice, "6") == 0) {
			printf("Starting Test 6\n");
			status = example6(Dev);
			printf("\n");
		}
		else if (strcmp(choice, "7") == 0){
			exit_main_loop = 1;
		}
		
		else{
			printf("Invalid choice\n");
		}

	} while (!exit_main_loop);

	VL53L4CD_comms_close(Dev);

	return 0;
}
