#include "datanalysis.h"
#include <cmath>
#include <array>
using namespace std;

double wavg(int L, double & centrval[L/2], double & uncert[L/2]){

    double sigma, x; 

    for (int i=0; i<L/2; i++){
        sigma =+ 1/pow(uncert[i],2);
	x =+ (1/pow(uncert[i],2))*centrval[i]; 
    }

    double results[2] = {x/sigma, 1/sqrt(sigma)}; 

    return results[];
}
