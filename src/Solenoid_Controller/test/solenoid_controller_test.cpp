#include "../Solenoid_Controller.h"

#include <unistd.h>



int main(){


    Solenoid_Controller solenoid;

    solenoid.start();

    usleep(10000);

    solenoid.Open_solenoid_2();

    usleep(5000000);

    solenoid.Open_solenoid_1();

    usleep(5000000);

    solenoid.Open_solenoid_2();
    
    usleep(5000000);
    
    solenoid.Close_solenoid_1();

    solenoid.stop();

}