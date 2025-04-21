
#ifndef TOF_2callback_H_
#define TOF_2callback_H_
#endif

#include "../VL53L4CD/user/uld-driver/VL53L4CD_api.h"
#include "./thread_controller_class/thread_controller.h"
#include <mutex>
#include <math.h>

class TOF_2callback : public VL53L4CD_API::VL53L4CD_Callback_Interface {
    virtual void hasVL53L4CDSample(uint16_t v) override {

    printf("%i\n",v);
    switch(sensors_running){

        case 0:
            printf("I'm Ready Total Volume is: %f\n",*volume);
            //thread_controller->release();
            //(*TOF_1_Pointer).stop_recording_data();
            //TOF_2_Pointer->stop_recording_data();

            sensors_running = 2;
        
        case 1:
            if (variable_tof_2) {
                variable_tof_2 -> store(v);
                
                int width = total_distance -((*variable_tof_1).load() + (*variable_tof_2).load());


                if(width < 1){ 
                    sensors_running = 0;
                }

                else{
                    *volume +=  (M_PI*std::pow((float)width,2))/2;
                }
            }
        
        case 2:
        
            sensors_running = 2;
    }
};

    friend class Volume_Comms;

    public:       

    private:
        int total_distance = 125;

        std::atomic<int>* variable_tof_1;
        std::atomic<int>* variable_tof_2;
        float* volume;
        int sensors_running = 1;

        Thread_Controller* thread_controller;

        VL53L4CD_API* TOF_1_Pointer;
        VL53L4CD_API* TOF_2_Pointer;


        

};

