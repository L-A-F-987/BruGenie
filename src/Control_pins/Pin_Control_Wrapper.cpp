
#include <unistd.h>
#include "Pin_Control_Wrapper.h"
#include <gpiod.h>



int Pin_Control::start(int pin_number){

    //storing the chip and pins
    chipGPIO = gpiod_chip_open_by_number(chip_used);
    pinGPIO= gpiod_chip_get_line(chipGPIO,pin_number);

    int ret = gpiod_line_request_output(pinGPIO,"consumer",0);
    
    int x = 0;

    return 1;
}


int Pin_Control::on(){

    printf("solenoid_on\n");

    gpiod_line_set_value(pinGPIO,1);


}

int Pin_Control::off(){

    printf("solenoid_off\n");

    gpiod_line_set_value(pinGPIO,0);


}

int Pin_Control::stop(){

    //releasing the pin
    gpiod_line_release(pinGPIO);
	gpiod_chip_close(chipGPIO);


}
