#include<iostream>
using namespace std; 

int main  (int argc, char ** argv)
  {

    int num = 5 ;

    cout << "num value:    " << num << endl ;

    cout << "num address:  " << & num << endl ;  
    //l'operatore & ritorna il valore della cella di memoria della variabile num
      
    return 0 ; 
}
