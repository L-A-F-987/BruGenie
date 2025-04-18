
#include <unistd.h>
#include <signal.h>
#include <dlfcn.h>

#include <stdio.h>
#include <string.h>

#include "../../user/uld-driver/VL53L4CD_api.h"

#include "../Volume_Comms.h"

int main(int argc, char ** argv)
{

    Volume_Comms v;

	v.Boot_sensors();
	//getchar();
	v.start_sensors();
	getchar();
	v.stop_sensors();


	return 0;
}
