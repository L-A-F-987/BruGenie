
#ifndef TOF_2callback_H_
#define TOF_2callback_H_
#endif

#include "../VL53L4CD/user/uld-driver/VL53L4CD_api.h"
#include <mutex>
#include <math.h>

class TOF_2callback : public VL53L4CD_API::VL53L4CD_Callback_Interface {
    virtual void hasVL53L4CDSample(uint16_t v) override {
    

    printf("%i\n",v);

    switch(sensors_running){

        case 0:
            printf("i'm done bro\n");

        case 1:
            if (variable_tof_2) {
                variable_tof_2 -> store(v);
                
                usleep(1000);
                int width = total_distance -((*variable_tof_1).load() + (*variable_tof_2).load());

                if(width < 1){ 
                    printf("volume: %i\n",*volume);
                    sensors_running = false;
                }

                else{
                    *volume +=  (M_PI*std::pow((float)width,2))/2;
                }
            }
    }
};




    public:

        std::atomic<int>* variable_tof_1;
        std::atomic<int>* variable_tof_2;
        float* volume;
        bool sensors_running = true;           

    private:
        int total_distance = 125;

        

};

