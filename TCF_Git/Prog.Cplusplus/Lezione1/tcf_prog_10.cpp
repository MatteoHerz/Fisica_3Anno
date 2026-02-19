#include <iostream>

int main (int argc, char ** argv)
  {
    std::cout << "Input parameters from command line:" << std::endl ;

    std::cout << "You decided to pass a string of "<< argc <<" parameters: "<< std::endl ;

    std::cout << "Parameter 1: "<< argv[0]<< std::endl;

    std::cout << "Parameter 2: "<< argv[1]<< std::endl;

    std::cout << "note that argv["<< argc <<"] is a null pointer"<< std::endl;
    
    return 0 ;
  }    

