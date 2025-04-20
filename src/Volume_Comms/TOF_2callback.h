
#ifndef TOF_2callback_H_
#define TOF_2callback_H_
#endif

#include "../VL53L4CD/user/uld-driver/VL53L4CD_api.h"
#include "./thread_controller_class/thread_controller.h"
#include <mutex>
#include <math.h>

class TOF_2callback : public VL53L4CD_API::VL53L4CD_Callback_Interface {
    virtual void hasVL53L4CDSample(uint16_t v) override {

    switch(sensors_running){

        case 0:
            printf("hello\n");
            thread_controller->release();
        
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
    }
};

    friend class Volume_Comms;

    public:       

    private:
        int total_distance = 125;

        std::atomic<int>* variable_tof_1;
        std::atomic<int>* variable_tof_2;
        float* volume;
        bool sensors_running = 1;

        Thread_Controller* thread_controller;

        

};

