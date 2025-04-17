
#include <unistd.h>
#include <signal.h>
#include <dlfcn.h>

#include <stdio.h>
#include <string.h>

#include "../../user/uld-driver/VL53L4CD_api.h"

#include "../Volume_Comms.h"

int exit_main_loop = 0;

void sighandler(int signal)
{
	printf("SIGNAL Handler called, signal = %d\n", signal);
	exit_main_loop  = 1;
}

int main(int argc, char ** argv)
{

    Volume_Comms v;
	char choice[20];
	int status;

	//printf("WHYY");

	//v.Start_sensor_1();

	//do {
		//printf("----------------------------------------------------------------------------------------------------------\n");
		//printf(" VL53L4CD uld driver test example menu \n");
		//printf(" ------------------ Ranging menu ------------------\n");
		//printf(" 1 : basic ranging\n");



	int i = 0;
	if (i ==0){
		printf("BruGenie");
		printf("----------------------------------------------------------------------------------------------------------\n");

		printf("Your choice ?\n ");
		scanf("%s", choice);

		if (strcmp(choice, "1") == 0) {
			printf("Starting Test 1\n");
			v.Start_sensor_1();
			printf("\n");
		}
	}
	//} while (!exit_main_loop);

	//VL53L4CD_comms_close(Dev);

	return 0;
}
