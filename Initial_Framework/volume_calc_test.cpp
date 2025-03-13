#include <iostream>
#include <iomanip>
//including the library I made  
#include "./header_files/Ultrasound_Sensors.h"

int main(){



    int n_slices = 5;
    double thickness[5];
    double max_height = 10.5;

    for(int i = 0;i<n_slices;i++){
        thickness[i] = 2.3;
    }

    //Volume_calculation(5,max_height,thickness);

    if(Volume_calculation(5,max_height,thickness)!= 43.6249409859113){


        std::cout<<std::setprecision(15) <<Volume_calculation(5,max_height,thickness)<<"\n";
        //throw "Volume_calculation_test_failed";

    }


}