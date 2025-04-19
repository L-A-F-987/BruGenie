
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <math.h>

#include "Volume_Comms.h"


int Volume_Comms::Boot_sensors()
{

	x_shut_6.start(6);
	x_shut_5.start(5);

	x_shut_5.off();
	x_shut_6.off();

	x_shut_5.on();
	x_shut_6.on();

	TOF_2.boot_sensor((uint8_t)0x29, 27);

	TOF_2.VL53L4CD_SetI2CAddress((uint8_t)0x28);

	x_shut_5.on();
	x_shut_6.off();
	usleep(1000);
	x_shut_6.on();
	

	x_shut_5.stop();
	x_shut_6.stop();

	usleep(1000);
	TOF_2.start_recording_data();
	TOF_2.stop_recording_data();

	TOF_1.boot_sensor(TOF_1_address, TOF_1_interrupt);
	TOF_2.boot_sensor(TOF_2_address, TOF_2_interrupt);

	//configuring interrupt to match TOF to allow for blocking of the volume calculation thread
	chipDRDY = gpiod_chip_open_by_number(rpi_chip);
	pinDRDY = gpiod_chip_get_line(chipDRDY,TOF_2_interrupt);
	


};


void Volume_Comms::start_sensors(){

	motor.start();

	tof_1_callback.variable = &last_TOF_1_Sample;
	tof_2_callback.variable = &last_TOF_2_Sample;
	TOF_1.registerCallback(&tof_1_callback);
	TOF_2.registerCallback(&tof_2_callback);

	TOF_1.start_recording_data();
	TOF_2.start_recording_data();

	
	//thr(std::ref(last_TOF_1_Sample));
	thr = std::thread(&Volume_Comms::Volume_Tracker,this, std::ref(last_TOF_1_Sample), std::ref(last_TOF_2_Sample));


	motor.Set_motor_downwards();

}

void Volume_Comms::stop_sensors(){


	TOF_1.stop_recording_data();
	TOF_2.stop_recording_data();
	motor.stop();
	measureing_volume = false;

	thr.join();

	printf("%f\n",total_volume);


	printf("End of ULD demo\n");

	//printf("Last TOF 1 Sample: %i\nLast TOF 2 Sample: %i\n",last_TOF_1_Sample,last_TOF_2_Sample);

	gpiod_line_release(pinDRDY);
	gpiod_chip_close(chipDRDY);

}

void Volume_Comms::Volume_Tracker(std::atomic<int>&last_TOF_1_Sample, std::atomic<int>&last_TOF_2_Sample){

	const struct timespec ts = {1,0};
	gpiod_line_event_wait(pinDRDY, &ts);
	struct gpiod_line_event event;
	gpiod_line_event_read(pinDRDY, &event);

	usleep(10000000);

	int width = total_distance - last_TOF_1_Sample.load() - last_TOF_2_Sample.load();

	//printf("width:%i\nTOF_1:%i\n,TOF_2:%i\n",width);
	printf("width %i\n",width);
	printf("TOF1 %i\n",last_TOF_1_Sample.load());
	total_volume += (M_PI*std::pow((float)width,2))/2;

}


