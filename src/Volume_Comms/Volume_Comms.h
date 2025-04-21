


#ifndef Volume_Comms_H_
#define Volume_Comms_H_

#include "../uld-driver/VL53L4CD_api.h"
#include "../Control_pins/Pin_Control_Wrapper.h"
#include "../motor/Parallax_INC4_Servo_Motor.h"
#include "./thread_controller_class/thread_controller.h"
#include "../Solenoid_Controller/Solenoid_Controller.h"

#include <atomic>
#include <mutex>
#include <condition_variable>

#include "TOF_1callback.h"
#include "TOF_2callback.h"

#define rpi_chip 4
#define TOF_1_interrupt 14
#define TOF_2_interrupt 27

#define TOF_1_address (uint8_t)0x28
#define TOF_2_address (uint8_t)0x29


class Volume_Comms{

    //making tof_callback 2 and this class friends to share an std::mutex
    friend class TOF_2callback;

    public:

        int Boot_sensors();

        void start_sensors();

        void stop_sensors();

        void reset_address();

        float total_volume = 0;

    private:

        void Dispensing_Controller();

        void motor_controller();

        VL53L4CD_API TOF_1;
        VL53L4CD_API TOF_2;

        Pin_Control x_shut_6;
        Pin_Control x_shut_5;

        Parallax_Motor motor;

        Solenoid_Controller solenoids;

        std::atomic<int> last_TOF_1_Sample;
        std::atomic<int> last_TOF_2_Sample;

        TOF_1callback tof_1_callback;
        TOF_2callback tof_2_callback;
        std::thread thr;

        bool measureing_volume = true;

        //thread_controller to managed thread communications
        Thread_Controller thread_controller;

        int flowrate = 30;

        float coffee_ratio = 0.9;
        float milk_ratio = 1.0 - coffee_ratio;


        


};
#endif

