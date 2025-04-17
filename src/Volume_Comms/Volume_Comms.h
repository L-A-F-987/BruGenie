#ifndef Volume_Comms_H_
#define Volume_Comms_H_


#include "../VL53L4CD/user/uld-driver/VL53L4CD_api.h"
#include "../Control_pins/Pin_Control_Wrapper.h"

class Volume_Comms{

    public: 

    void start();

    void stop();
    
    void volume_tracker();

    private:

    //variable to track volume
    int total_estimated_volume;

    //the two TOF Sensors 
    VL53L4CD_API TOF_1;

    //VL53L4CD_API TOF_2; 

    //xshut pins
    //Pin_Control x_shut5;
    //Pin_Control x_shut6;


};

#endif