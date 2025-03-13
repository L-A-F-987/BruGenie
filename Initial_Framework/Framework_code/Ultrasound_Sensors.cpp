
// Inluding the standard library 
#include <iostream>
#include <math.h>


// The Actual Function Goes here
double Volume_calculation(int n_slices,double max_height,double* measured_diameter_array){

    double Volume = 0.0;

    double slice_thickness = max_height/n_slices;

    double circular_area;

    for(int i = 0; i<n_slices; i++){

        circular_area = (std::pow(measured_diameter_array[i],2)*M_PI)/4;

        Volume += slice_thickness * circular_area;

    }

    //std::cout<<Volume;

    return{Volume};



}





