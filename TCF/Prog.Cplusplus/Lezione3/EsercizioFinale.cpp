#include <iostream>
#include <cmath>
using namespace std; 

double calc_acc(double k, double m, double pos);
void update_vel (double &velocity, double acceleration, double dt);
void update_pos (double *position, double velocity, double dt);

int main(){

    double m = 2, k = 10, initialpos, initialvel, timedelay; 
    int nsteps; 
    
    cout << "\nSteps you want to simulate: "; 
    cin >> nsteps;

    cout << "\nTime-delay you want to use: "; 
    cin >> timedelay;

    cout << "\nInitial position: "; 
    cin >> initialpos;

    cout << "\nInitial velocity: "; 
    cin >> initialvel;
    cout << endl; 
    
    double * acceleration = new double [nsteps];
    double * velocity = new double [nsteps];
    double * position = new double [nsteps];

    for(int i=0; i<nsteps; i++) { 
        if(i == 0) { 
            acceleration[i] = calc_acc(k,m, initialpos); 
            velocity[i] = initialvel; 
            position[i] = initialpos;
        } else { 
            update_vel(velocity[i], acceleration[i-1], timedelay); 
            velocity[i] += velocity[i-1];

            update_pos(&position[i], velocity[i-1], timedelay); 
            position[i] += position[i-1];
            
            acceleration[i] = calc_acc(k,m, position[i]); 
        } 
    }

    for (int i = 0; i < nsteps; i++) 
    {
        double t = i * timedelay; 
        cout << "t = " << t
             << " s | x = " << position[i]
             << " m | v = " << velocity[i]
             << " m/s | a = " << acceleration[i]
             << " m/s^2\n";
    }

    //cout <<"\nVelocity values over time:  \n"<<endl;
    //for (int i = 0; i < nsteps; i++) cout << velocity[i] << "  ";

    //cout <<"\n\nPosition values over time:  \n"<<endl;
    //for (int i = 0; i < nsteps; i++) cout << position[i] << "  ";

    delete [] acceleration;
    delete [] velocity;
    delete [] position;

    return 0;
}

double calc_acc(double k, double m, double pos){
    return - (k/m) * pos;
}

void update_vel(double &velocity, double acceleration, double dt){
    velocity = acceleration*dt;
    return;
}

void update_pos(double *position, double velocity, double dt){
    *position = velocity*dt;
    return;
}