


#ifndef Volume_Comms_H_
#define Volume_Comms_H_

#include "../uld-driver/VL53L4CD_api.h"
#include "../Control_pins/Pin_Control_Wrapper.h"

class Volume_Comms{

    public:
        int Start_sensor_1();

    private:

        VL53L4CD_API TOF_1;
        VL53L4CD_API TOF_2;

       Pin_Control x_shut_6;


};
#endif

