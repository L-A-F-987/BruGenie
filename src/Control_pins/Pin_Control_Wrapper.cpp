
#include <unistd.h>
#include "Pin_Control_Wrapper.h"
#include <gpiod.h>



int Pin_Control::start(int pin_number){

    //storing the chip and pins
    chipGPIO = gpiod_chip_open_by_number(chip_used);
    pinGPIO= gpiod_chip_get_line(chipGPIO,pin_number);

    int ret = gpiod_line_request_output(pinGPIO,"consumer",0);

    
    int x = 0;

    return 0;
}


int Pin_Control::on(){
    // function to switch on GPIO pin for the solenoid valve to start water flow
    printf("solenoid_on\n");

    gpiod_line_set_value(pinGPIO,1); 

    return 0;

}

int Pin_Control::off(){
    // function to switch off GPIO pin for the solenoid valve to stop water flow
    printf("solenoid_off\n");

    int ret = gpiod_line_set_value(pinGPIO,0);

    return 0;


}

int Pin_Control::stop(){

    //releasing the pin
    gpiod_line_release(pinGPIO);
	gpiod_chip_close(chipGPIO);

    return 0;

}


