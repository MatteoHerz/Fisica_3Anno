#include <iostream>
using namespace std; 

int main(){
	int myarray[] = {7, 5, 3};
	cout <<"Indirizzo del primo elemento di myarray: " << myarray << endl;
	cout << myarray[0] << " is equal to " << *myarray << endl;

	//Possiamo navigare nei valori successivi al prima dell'array usando l'aògebra degli array, ecco un esempio: 
	cout << myarray[1] << " is equal to " << *(myarray+1) << endl;
	cout << myarray[2] << " is equal to " << *(myarray+2) << endl;
}
