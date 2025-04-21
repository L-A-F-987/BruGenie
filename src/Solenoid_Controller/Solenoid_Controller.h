
#ifndef Solenoid_Control_H_
#define Solenoid_Control_H_

#define default_solenoid_1_pin 24
#define default_solenoid_2_pin 16

#include "../Control_pins/Pin_Control_Wrapper.h"


class Solenoid_Controller{


    public:
    // declaring functions to open and close the solenoid valves in response to writing the respective pins to be high or low

        void start();

        void Open_solenoid_1();

        void Open_solenoid_2();

        void Close_solenoid_1();

        void Close_solenoid_2();

        void stop();

    


    private:


        //solenoid used for milk
        Pin_Control solenoid_1;

        //solenoid used for water
        Pin_Control solenoid_2;

        // declaring booleans to use for switch case to switch between code blocks in the cpp file
        bool solenoid_1_state;
        bool solenoid_2_state;

};

#endif