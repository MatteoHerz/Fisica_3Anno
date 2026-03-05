#include "func_lib.h"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <cmath>
#include "TF1.h"
#include "TGraphErrors.h"
#include "TLatex.h"
#include "TLine.h"
#include "TLegend.h"

using namespace std;

vector<Materiali> setMateriali() {
    vector<Materiali> materiali;
    /*
    I materiali ed i relativi valori sono stati presi da una tabella online 
    (https://www.samaterials.it/content/density-measurement-and-common-materials.html).
    */
    const char* nomi[22] = {
        "Alluminio", "Rame", "Acciaio al carbonio", "Ghisa", "Titanio",
        "Acciaio inox (304)", "Ottone", "Piombo", "Tungsteno", "Nichel",
        "Oro", "Argento", "Legno (quercia)", "Calcestruzzo", "Vetro",
        "Polietilene", "Legno (pino)", "Fibra di carbonio", "Fibra di vetro",
        "Gomma", "Silicio", "Berillio"
    };
    double rho[22] = {
        2.70, 8.96, 7.85, 6.90, 4.43, 7.93, 8.50, 11.34, 19.25, 8.90,
        19.32, 10.49, 0.80, 2.30, 2.60, 0.935, 0.575, 1.80, 2.60, 1.35,
        2.33, 1.85
    };
    double rho_err[22] = {
        0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
        0.0, 0.0, 0.05, 0.0, 0.20, 0.025, 0.075, 0.20, 0.10, 0.25,
        0.0, 0.0
    };

    for (int i = 0; i<22; i++) {
        Materiali m = {nomi[i], rho[i], rho_err[i], 0.0, 0.0};
        materiali.push_back(m);
    }
    
    return materiali;
}


void caricamentoFile(const char *filename, TH2D *h2_sup, TH2D *h2_inf, TH2D *h2_T) {
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Errore apertura file: " << filename << endl;
        return;
    }
    
    /*
    Riempiamo gli istogrammi del primo rivelatore: h2_sup, del
    secondo rivelatore h2_inf e l'istogramma della trasmissione I/I0
    */
    double x, y, I0, I;
    while (file >> x >> y >> I0 >> I) {
        h2_sup->Fill(x, y, I0);
        h2_inf->Fill(x, y, I);
        h2_T->Fill(x, y, I/I0);
    }
    file.close();
}

/*
Vista la forte correlazione tra dmax e rho li inseriamo come un unico parametro [1] 
che chiameremo coeff
*/
TF2 *fitIntensita(TH2D *h2_sup, TH2D *h2_inf) {
    double I0_media = h2_sup->GetMean(3);
    
    TF2 *fit = new TF2("fit_I", "[0] * exp(-0.3 * [1] * (x - 30)/50)", FIT_X_MIN, FIT_X_MAX, FIT_Y_MIN, FIT_Y_MAX);
    fit->SetParameters(I0_media, 2);
    fit->SetLineWidth(3);
    fit->SetLineColor(kRed);
    h2_inf->Fit(fit, "SQR");
    
    return fit;
}

TF2 *fitTrasmissione(TH2D *h2_T) {
    TF2 *fit = new TF2("fit_T", "exp(-0.3 * [0] * (x - 30)/50)", FIT_X_MIN, FIT_X_MAX, FIT_Y_MIN, FIT_Y_MAX);
    fit->SetParameter(0, 2.0);
    fit->SetLineWidth(3);
    fit->SetLineColor(kRed);
    h2_T->Fit(fit, "SQR");
    
    return fit;
}

/*
Trovato il coefficiente dal fit possiamo trovare un possibile valore dello
spessore massimo del cuneo dall'andamento dmax*rho = coeff
*/
void calcoloDmax(vector<Materiali>& materiali, double coeff, double coeff_err) {
    for (int i = 0; i < materiali.size(); i++) {
        materiali[i].dmax = coeff / materiali[i].rho;
        materiali[i].dmax_err = materiali[i].dmax * sqrt(pow(coeff_err/coeff, 2) + pow(materiali[i].rho_err/materiali[i].rho, 2));
    }
}

void graficoIntensita(TCanvas *c, TH2D *h2_inf, TH2D *h2_T, TF2 *fit_inf, TF2 *fit_T) {
    c->Divide(2, 2);
    
    c->cd(1);
    h2_inf->SetTitle("Fit Intensita': I(x,y); x; y");
    h2_inf->Draw("LEGO");
    TLegend *leg1 = new TLegend(0.07, 0.9, 0.40, 0.88);
    leg1->SetBorderSize(0);
    leg1->SetFillColor(0);
    leg1->SetTextFont(42);
    leg1->SetTextSize(0.04);
    leg1->AddEntry(fit_inf, "Fit teorico", "l");
    leg1->Draw();

    c->cd(2);
    h2_inf->Draw("COLZ");
    
    c->cd(3);
    h2_T->SetTitle("Fit Trasmissione: T(x,y) = I/I_{0}; x; y");
    h2_T->Draw("LEGO");
    TLegend *leg2 = new TLegend(0.07, 0.9, 0.40, 0.88);
    leg2->SetBorderSize(0);
    leg2->SetFillColor(0);
    leg2->SetTextFont(42);
    leg2->SetTextSize(0.04);
    leg2->AddEntry(fit_T, "Fit teorico", "l");
    leg2->Draw();
    
    c->cd(4);
    h2_T->Draw("COLZ");
}

void graficoMateriali(TCanvas *c, vector<Materiali> &materiali, double coeff) {
    c->Divide(1, 1);
    c->cd(1);

    int n = materiali.size();
    vector<double> rho(n), dmax(n), rho_err(n), dmax_err(n);
    
    for (int i=0; i<n; i++) {
        rho[i] = materiali[i].rho;
        dmax[i] = materiali[i].dmax;
        rho_err[i] = materiali[i].rho_err;
        dmax_err[i] = materiali[i].dmax_err;
    }
    
    TGraphErrors *ge = new TGraphErrors(n, rho.data(), dmax.data(), rho_err.data(), dmax_err.data());
    ge->SetTitle("Spessore massimo vs Densita'; #rho [g/cm^{3}]; d_{max} [cm]");
    ge->SetMarkerStyle(20);
    ge->SetMarkerSize(1.0);
    ge->SetMarkerColor(kBlue);
    ge->Draw("AP");

    TF1 *fit_curve = new TF1("fit_curve", "[0]/x", 0.1, 25);
    fit_curve->SetParameter(0, coeff);
    fit_curve->SetLineWidth(2);
    fit_curve->SetLineColor(kRed);
    fit_curve->Draw("SAME");

    // Legend
    TLegend *leg = new TLegend(0.6, 0.7, 0.8, 0.8);
    leg->SetBorderSize(0);
    leg->SetFillColor(0);
    leg->SetTextFont(42);
    leg->SetTextSize(0.02);
    leg->AddEntry(ge, "Materiali", "lep");
    leg->AddEntry(fit_curve, "Fit: d = coeff/#rho", "l");
    leg->Draw();
    
    // Etichette per materiali selezionati
    TLatex latex;
    latex.SetTextFont(42);
    latex.SetTextColor(kBlack);
    
    double x_start = 5.0;
    double x_spacing = 2.6;
    double y_start = 14.0;
    double y_spacing = 2.5;
    
    int materiali_scelti[4] = {19, 0, 4, 5};
    
    for (int i = 0; i < 4; i++) {
        int index = materiali_scelti[i];
        double x_pos = x_start + i * x_spacing;
        double y_pos = y_start - i * y_spacing;
        double interlinea = 0.8;
        
        latex.SetTextSize(0.030);
        latex.SetTextFont(62);
        latex.DrawLatex(x_pos, y_pos, materiali[index].name);
        
        latex.SetTextSize(0.026);
        latex.SetTextFont(42);
        latex.DrawLatex(x_pos , y_pos - interlinea, Form("#rho = %.2f #pm %.2f g/cm^{3}", materiali[index].rho, materiali[index].rho_err));
        latex.DrawLatex(x_pos, y_pos - 2*interlinea, Form("d_{max} = %.2f #pm %.2f cm", materiali[index].dmax, materiali[index].dmax_err));
        
        TLine *line = new TLine(materiali[index].rho, materiali[index].dmax, x_pos, y_pos);
        line->SetLineStyle(2);
        line->SetLineColor(kGreen+1);
        line->Draw();
    }
}

/*
Stampa a schermo dei risultati dei fit e della tabella con tutte i possibili materiali
di cui potrebbe essere composto il cuneo
*/
void printFinale(double coeff, double coeff_err, double kT, double kT_err, vector<Materiali> &materiali) {
    cout << "\n===== RISULTATI ANALISI =====" << endl;
    cout << "Fit Intensita' I:         k = rho*dmax = " << coeff << " ± " << coeff_err << " [g/cm²]" << endl;
    cout << "Fit Trasmissione I/I_0:   k = rho*dmax = " << kT << " ± " << kT_err << " [g/cm²]" << endl;

    cout << "\n===== TUTTE LE POSSIBILI COPPIE DI VALORI RHO-DMAX =====\n" << endl;
    cout << left << setw(20) << "Materiale" << setw(20) << "rho (g/cm^3)" << setw(20) << "dmax (cm)" << endl;
    cout << left << setw(50) << "-------------------------------------------------" << endl;
    for(int i=0; i<materiali.size(); i++) {
        cout << setprecision(3) << left << setw(20) << materiali[i].name
                                        << setw(20) << materiali[i].dmax
                                        << setw(20) << materiali[i].rho  << endl;
    }
}