
// Inluding the standard library 
#include <iostream>

// The Actual Function Goes here
void Print_Out_Names(int n_prints){


    /*
    Simple for loop, note how i is declared  as int i
    This specifies that a variable of i is initialised 
    as an integer, in c++ you must manually declare the variable 
    type for everything and it is as far as I know fixed.
    */

    // set initial value; test_case ; increment
    for(int i = 0;i<n_prints;i++ ){
        
        //Std = iostream library , cout = print command
        std::cout<<"Hello Team :)\n";


    }

    /*
    Another key note, anything declared in a c++ for loop only 
    exists in the loop notice how i can be reused without complaint
    */

    for(int i = 0; i<1;  i++){

        std::cout<<"Man, I sure Hope That Worked\n\n";

    }

// git testing taniya laptop


}





