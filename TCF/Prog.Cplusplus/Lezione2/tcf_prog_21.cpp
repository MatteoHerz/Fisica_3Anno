#include <iostream>
#include <array>
#include <cmath>
#include "datanalysis.h"
using namespace std; 

int main (){
	//array <double, 3> values, array<double, 3> errors;
	double array[3], errors[3], results[2]; 

	for(int i=0; i<3; i++){
		cout<<"\nInserire il "<<i+1<<" valore: ";
		cin >> values[i];
		cout<<"\nInserire l'errore: ";
		cin >> errors[i]; 
	}

	//array<double, 2> results = wavg(6, values, errors);  
	
	results[2]=wavg(6,values,errors);

	cout<<"Media pesata: "<<results[0]<<"+-"<<results[1]<<endl; 

	return 0; 
}
