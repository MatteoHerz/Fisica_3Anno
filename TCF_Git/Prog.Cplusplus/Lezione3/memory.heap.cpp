#include <iostream>
using namespace std; 

  int * doubleptr(int val){

    int *result = new int(2*val);

    std::cout << "inside func - pointer: " << result << std::endl;

    std::cout << "inside func - doubleresult: " << *result << std::endl;
    
    return result;
  }

  int main(){
    
    int num = 7;
    
    int *doublenum = doubleptr(num); 
    
    std::cout << "Returned pointer: " << doublenum << std::endl;
    
    std::cout << "Returned value: " << *doublenum << std::endl;

    std::cout << " \n... Deleting doublenum ..." <<std::endl;  
    std::cout << "... Initializing it to a nullptr ...\n" <<std::endl;

    delete doublenum;
    
    doublenum = nullptr;

    std::cout << "Final pointer: " << doublenum << std::endl;

    return 0;
  }
