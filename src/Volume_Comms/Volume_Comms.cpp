
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
};


void Volume_Comms::start_sensors(){

	motor.start();

	tof_1_callback.variable = &last_TOF_1_Sample;
	tof_2_callback.variable_tof_2 = &last_TOF_2_Sample;
	tof_2_callback.variable_tof_1 = &last_TOF_1_Sample;
	tof_2_callback.volume = &total_volume;
	TOF_1.registerCallback(&tof_1_callback);
	TOF_2.registerCallback(&tof_2_callback);

	//providing callback 2 a copy of the tof so it can call stop for them 
	tof_2_callback.TOF_1_Pointer = &TOF_1;
	tof_2_callback.TOF_2_Pointer = &TOF_2;

	TOF_1.start_recording_data();
	TOF_2.start_recording_data();

	motor.Set_motor_upwards();
	

	
	//thr = std::thread(&Volume_Comms::Dispensing_Controller,this);

	getchar();

	Dispensing_Controller();
}

void Volume_Comms::stop_sensors(){


	TOF_1.stop_recording_data();
	TOF_2.stop_recording_data();
	motor.stop();
	measureing_volume = false;

	//thr.join();

	printf("%f\n",total_volume);


	printf("End of ULD demo\n");

}

void Volume_Comms::Dispensing_Controller(){


	int dispensing_volume = 0.9*((total_volume)/1000);

	printf("dispensing_volume:%i\n",dispensing_volume);

	motor.stop();
	TOF_1.stop_recording_data();
	TOF_2.stop_recording_data();

	solenoids.start();

	float solenoid_one_time = (dispensing_volume*coffee_ratio)/flowrate;
	float solenoid_two_time = (dispensing_volume*milk_ratio)/flowrate;

	int solenoid_one_time_micro = solenoid_one_time*1000000;

	int solenoid_two_time_micro = solenoid_two_time*1000000;

	printf("solenoid_one_time:%i\n",solenoid_one_time_micro);
	solenoids.Open_solenoid_1();

	usleep(solenoid_one_time_micro);

	solenoids.Open_solenoid_2();

	usleep(solenoid_two_time_micro);

	solenoids.Close_solenoid_2();

	solenoids.stop();


	


	
}


void Volume_Comms::motor_controller(){
	//block thread until the cup has been measured
	
	//begin dispensing the liquid
	int dispensing_volume = 0.9*total_volume;

	
}

void Volume_Comms::reset_address(){
	
	//setting both xshuts high and low to reset the address
	x_shut_6.start(6);
	x_shut_5.start(5);

	x_shut_5.off();
	x_shut_6.off();

	x_shut_5.on();
	x_shut_6.on();

	x_shut_6.stop();
	x_shut_5.stop();
	
}




