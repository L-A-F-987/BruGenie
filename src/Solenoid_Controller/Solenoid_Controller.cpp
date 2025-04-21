
#include "Solenoid_Controller.h"

#include <unistd.h>


void Solenoid_Controller::start(){

    solenoid_1.start(default_solenoid_1_pin);
    solenoid_2.start(default_solenoid_2_pin);

};

void Solenoid_Controller::stop(){

    solenoid_1.off();
    solenoid_2.off();

    solenoid_1.stop();
    solenoid_2.stop();
};

//defining functions with a switch case to switch on/off based on the value of solenoid state boolean
void Solenoid_Controller::Open_solenoid_1(){

    switch(solenoid_2_state){

    case 0:

        solenoid_1.on();

        solenoid_1_state = 1;
    
    case 1:

        solenoid_2.off();

        //small microsleep added as both run off the same power supply and don't want to have both lines open at the same time
        usleep(1000);

        solenoid_1.on();

        solenoid_1_state = 1;
    }
};

void Solenoid_Controller::Open_solenoid_2(){

    switch(solenoid_1_state){

        case 0:
    
            solenoid_2.on();

            solenoid_2_state = 1;
        
        case 1:
    
            solenoid_1.off();
    
            //small microsleep added as both run off the same power supply and don't want to have both lines open at the same time
            usleep(1000);
    
            solenoid_2.on();

            solenoid_2_state = 1;
        }

};

//functions to close solenoid to stop fluid flow

void Solenoid_Controller::Close_solenoid_1(){

    solenoid_1.off();

    solenoid_1_state = 0;

};

void Solenoid_Controller::Close_solenoid_2(){

    solenoid_2.off();

    solenoid_2_state = 0;

};


