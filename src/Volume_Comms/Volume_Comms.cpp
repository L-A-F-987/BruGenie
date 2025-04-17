
#include "Volume_Comms.h"



void Volume_Comms::start(){

    //setting x_shutpins

    /*
    x_shut5.start(5);
    x_shut6.start(6);
    x_shut5.on();
    x_shut6.off();
    */

    //changing the I2C Address of the VL53L4CD Sensor to allow for 2 sensors to run on a single I2C line
    TOF_1.boot_sensor();
    //TOF_1.VL53L4CD_SetI2CAddress((uint8_t)0x28);
    //TOF_1.stop_recording_data();

    //starting TOFs back up 

    //TOF_1.boot_sensor();
    //TOF_2.boot_sensor();

    TOF_1.start_recording_data();
    //TOF_2.start_recording_data();


};

void Volume_Comms::stop(){

    //releasing the x_shut pins
    //x_shut6.stop();
    //x_shut5.stop();

    //stopping the sensors internally to save power
    TOF_1.stop_sensor_ranging();
    //TOF_2.stop_sensor_ranging();

    //shutting the I2C line for both TOF sensors
    TOF_1.stop_recording_data();
    //TOF_2.stop_recording_data();

}

void Volume_Comms::volume_tracker(){

    


}
