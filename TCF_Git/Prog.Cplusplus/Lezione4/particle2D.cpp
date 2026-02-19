#include <iostream>
#include "particle2D.h"
using namespace std; 

int main(){

    //particle2D p(0.0, 0.0, 1.0, 1.0, 1.0);
    //p.printState(); 
    //or
    //particle2D *p2 = new particle2D(0,0,2,2,13);

    double timedelay, initpos_y, initpos_x, initvel_y, initvel_x; 

    cout << "\nInitial x_position for the particle: ";
    cin >> initpos_x;
    cout << "\nInitial y_position for the particle: "; 
    cin >> initpos_y;

    cout << "\nInitial x_velocity for the particle: ";
    cin >> initvel_x;
    cout << "\nInitial y_velocity for the particle: "; 
    cin >> initvel_y;

    particle2D p(initpos_x, initpos_y, initvel_x, initvel_y, 1.0);

    cout << "\n\nInitial state of the particle: ";
    p.printState();

    cout << "\nTime delay you want to use to check conditions: "; 
    cin >> timedelay;

    p.update(timedelay);
    p.printState(); 
    cout <<endl; 

    return 0;
}