


#ifndef Volume_Comms_H_
#define Volume_Comms_H_

#include "../uld-driver/VL53L4CD_api.h"
#include "../Control_pins/Pin_Control_Wrapper.h"
#include "../motor/Parallax_INC4_Servo_Motor.h"
#include "./thread_controller_class/thread_controller.h"

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

        float total_volume = 0;

    private:

        void Dispensing_Controller(Thread_Controller& thread_controller);

        void motor_controller();

        VL53L4CD_API TOF_1;
        VL53L4CD_API TOF_2;

        Pin_Control x_shut_6;
        Pin_Control x_shut_5;

        Parallax_Motor motor;

        std::atomic<int> last_TOF_1_Sample;
        std::atomic<int> last_TOF_2_Sample;

        TOF_1callback tof_1_callback;
        TOF_2callback tof_2_callback;

        struct gpiod_line *pinDRDY = nullptr;
	    struct gpiod_chip *chipDRDY = nullptr;

        std::thread thr;

        bool measureing_volume = true;

        //thread_controller to managed thread communications
        Thread_Controller thread_controller;


        


};
#endif

