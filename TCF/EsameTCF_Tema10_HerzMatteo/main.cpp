#include "func_lib.h"
#include <iostream>
#include "TH2D.h"
#include "TCanvas.h"
#include "TApplication.h"
#include "TLegend.h"

using namespace std;

int main(int argc, char** argv) {
    TApplication app("app", &argc, argv);
    
    // Inizializza istogrammi vuoti
    TH2D h2_sup("h2_sup", "", 100, 0, 100, 100, 0, 100);
    TH2D h2_inf("h2_inf", "", 100, 0, 100, 100, 0, 100);
    TH2D h2_T("h2_T", "", 100, 0, 100, 100, 0, 100);
    
    // Carica dati
    caricamentoFile("data.txt", &h2_sup, &h2_inf, &h2_T);
    
    // Esegui fit
    TF2* fit_intensita = fitIntensita(&h2_sup, &h2_inf);
    TF2* fit_trasmissione = fitTrasmissione(&h2_T);
    
    // Estrai parametri
    double coeff = fit_intensita->GetParameter(1);
    double coeff_err = fit_intensita->GetParError(1);
    double coeffT = fit_trasmissione->GetParameter(0);
    double coeffT_err = fit_trasmissione->GetParError(0);
    
    // Analisi dmax per ogni materiale
    vector<Materiali> materiali = setMateriali();
    calcoloDmax(materiali, coeff, coeff_err);
    
    // Cout finale
    printFinale(coeff, coeff_err, coeffT, coeffT_err, materiali);
    
    // Grafici
    TCanvas c1("c1", "Grafici Intensita", 1200, 900);
    TCanvas c2("c2", "Analisi Materiali", 1200, 900);
    
    graficoIntensita(&c1, &h2_inf, &h2_T, fit_intensita, fit_trasmissione);
    graficoMateriali(&c2, materiali, coeff);
    
    c1.Update();
    c2.Update();
    app.Run();

    // pulizia
    delete fit_intensita;
    delete fit_trasmissione;
    
    return 0;
}