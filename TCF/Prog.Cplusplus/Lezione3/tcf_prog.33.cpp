#include <iostream>
using namespace std; 

int main(){
	int num1 = 13;

	int * point_to_num = &num1;

	cout << "Pointer: "<<point_to_num<<" value: "<<*point_to_num<<" variable: "<<num1<<endl; 

	int num2 = 15;

	point_to_num = &num2;

	cout << "Pointer: "<<point_to_num<<" value: "<<*point_to_num<<" variable: "<<num2<<endl; 

	*point_to_num = 14;

	cout << "Pointer: "<<point_to_num<<" value: "<<*point_to_num<<" variable num1: "<<num1<<" variable num2: "<<num2<<endl;
}
