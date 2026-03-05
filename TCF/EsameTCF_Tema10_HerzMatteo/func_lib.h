#ifndef func_lib_h
#define func_lib_h

#include <vector>
#include "TH2D.h"
#include "TCanvas.h"
#include "TF2.h"
#include "TLegend.h"

using namespace std;

/*
Dopo una visualizzazioen iniziale degli istogrammi / mappe di colore si
sono stabiliti i limiti geometrici del cuneo
*/
const int FIT_X_MIN = 30, FIT_X_MAX = 80;
const int FIT_Y_MIN = 30, FIT_Y_MAX = 70;

struct Materiali {
    const char* name;
    double rho, rho_err;
    double dmax, dmax_err;
};

vector<Materiali> setMateriali();
void caricamentoFile(const char *filename, TH2D *h2_sup, TH2D *h2_inf, TH2D *h2_T);
TF2 *fitIntensita(TH2D *h2_sup, TH2D *h2_inf);
TF2 *fitTrasmissione(TH2D *h2_T);
void calcoloDmax(vector<Materiali>& materiali, double coeff, double coeff_err);
void graficoIntensita(TCanvas *c, TH2D* h2_inf, TH2D *h2_T, TF2 *fit_inf, TF2 *fit_T);
void graficoMateriali(TCanvas *c, vector<Materiali>& materiali, double coeff);
void printFinale(double coeff, double coeff_err, double kT, double kT_err, vector<Materiali> &materiali);

#endif 