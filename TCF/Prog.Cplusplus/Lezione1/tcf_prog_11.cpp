#include <iostream>

int main (int argc, char ** argv)
  {
    int inputnumber = 0;
    
    std::cout << "Please input your favourite number: ";
    
    std::cin >> inputnumber;
    
    std::cout << "You dumb. "<<inputnumber<<" is just a number!"<<std::endl;
    
    return 0 ;
  }  
