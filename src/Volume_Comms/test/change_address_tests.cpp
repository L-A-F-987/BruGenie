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
	usleep(10000);
	v.stop_sensors();
    v.reset_address();

	return 0;
}