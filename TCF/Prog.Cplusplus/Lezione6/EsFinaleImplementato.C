#include "TH1F.h"
#include <TCanvas.h>
#include <TApplication.h>
#include <TGraph.h>
#include <TAxis.h>
#include <iostream>
#include <cmath>
#include <vector>
#include <fstream>

using namespace std;

// Funzione per leggere i dati dal file
vector<double> readData(const string& filename) {
    vector<double> data;
    ifstream in_file(filename);
    
    if (!in_file.is_open()) {
        cerr << "Errore: impossibile aprire il file " << filename << endl;
        return data;
    }
    
    double value;
    while (in_file >> value) {
        data.push_back(value);
    }
    in_file.close();
    
    cout << "Letti " << data.size() << " dati dal file." << endl;
    return data;
}

// Funzione per calcolare lambda da intervalli definiti dall'utente
vector<double> calculateLambdasFromRanges(const vector<double>& data, 
                                          const vector<pair<double, double>>& ranges,
                                          double a, double d) {
    vector<double> lambdas;
    
    for (size_t i = 0; i < ranges.size(); i++) {
        double sum = 0.0;
        int count = 0;
        
        // Calcola la media dei valori nell'intervallo
        for (double value : data) {
            if (value >= ranges[i].first && value < ranges[i].second) {
                sum += value;
                count++;
            }
        }
        
        if (count > 0) {
            double mean = sum / count;
            double lambda = (mean * 1e-3 * a) / (d * (i + 1)); // converti mm in m
            lambdas.push_back(lambda);
            cout << "Massimo ordine " << i+1 << ": x_mean = " << mean 
                 << " mm, lambda = " << lambda * 1e9 << " nm" << endl;
        } else {
            cout << "Attenzione: nessun dato nell'intervallo [" 
                 << ranges[i].first << ", " << ranges[i].second << "]" << endl;
        }
    }
    
    return lambdas;
}

int main() {
    // Parametri fisici
    const double a = 3e-6;  // Distanza tra le fenditure (m)
    const double d = 1.0;   // Distanza schermo (m)
    
    // Definisci gli intervalli per i tre picchi (in mm)
    vector<pair<double, double>> ranges = {
        {1.5, 5.0},   // Primo massimo
        {5.0, 8.0},   // Secondo massimo
        {8.0, 10.0}   // Terzo massimo
    };
    
    // Leggi i dati UNA SOLA VOLTA
    vector<double> all_data = readData("InterfPattern.txt");
    
    if (all_data.empty()) {
        cerr << "Nessun dato disponibile!" << endl;
        return 1;
    }
    
    // Calcola il numero ottimale di bin
    int N = all_data.size();
    int optimal_bins = 1 + 3.322 * log10(N);
    
    cout << "\nNumero di dati: " << N << endl;
    cout << "Numero ottimale di bins: " << optimal_bins << endl;
    
    // Crea l'istogramma e riempilo
    TH1D *photons = new TH1D("photons", "Distribuzione Fotoni - Doppia Fenditura", optimal_bins, -10, 10);
    photons->GetXaxis()->SetTitle("Posizione x (mm)");
    photons->GetYaxis()->SetTitle("Numero di fotoni");
    photons->SetLineColor(kBlue);
    photons->SetLineWidth(2);
    photons->SetFillColor(kCyan-10);
    
    for (double value : all_data) {
        photons->Fill(value);
    }
    
    // Setup grafico
    TApplication app("app", 0, 0);
    TCanvas *can = new TCanvas("can", "Analisi Doppia Fenditura", 1200, 600);
    can->Divide(2, 1);
    
    // Disegna l'istogramma principale
    can->cd(1);
    gPad->SetGrid();
    photons->Draw();
    
    // Calcola lambda usando gli intervalli predefiniti
    cout << "\n=== CALCOLO LUNGHEZZA D'ONDA ===" << endl;
    vector<double> lambdas = calculateLambdasFromRanges(all_data, ranges, a, d);
    
    // Calcola e mostra la media
    if (!lambdas.empty()) {
        double lambda_mean = 0.0;
        for (double l : lambdas) {
            lambda_mean += l;
        }
        lambda_mean /= lambdas.size();
        
        cout << "\n=== RISULTATO FINALE ===" << endl;
        cout << "Lambda media = " << lambda_mean * 1e9 << " nm" << endl;
        
        // Determina il colore
        double lambda_nm = lambda_mean * 1e9;
        string color;
        if (lambda_nm < 380) color = "Ultravioletto (UV)";
        else if (lambda_nm < 450) color = "Violetto";
        else if (lambda_nm < 495) color = "Blu";
        else if (lambda_nm < 570) color = "Verde";
        else if (lambda_nm < 590) color = "Giallo";
        else if (lambda_nm < 620) color = "Arancione";
        else if (lambda_nm < 750) color = "Rosso";
        else color = "Infrarosso";
        
        cout << "Colore della luce: " << color << endl;
        
        // Grafico delle lambda calcolate
        can->cd(2);
        gPad->SetGrid();
        
        TGraph *gr = new TGraph();
        for (size_t i = 0; i < lambdas.size(); i++) {
            gr->SetPoint(i, i+1, lambdas[i] * 1e9);
        }
        gr->SetTitle("Lunghezza d'onda per ordine di interferenza");
        gr->GetXaxis()->SetTitle("Ordine del massimo");
        gr->GetYaxis()->SetTitle("#lambda (nm)");
        gr->SetMarkerStyle(21);
        gr->SetMarkerSize(1.5);
        gr->SetMarkerColor(kRed);
        gr->SetLineColor(kRed);
        gr->SetLineWidth(2);
        gr->Draw("APL");
        
        // Aggiungi una linea per la media
        TGraph *grMean = new TGraph();
        grMean->SetPoint(0, 0.5, lambda_mean * 1e9);
        grMean->SetPoint(1, lambdas.size() + 0.5, lambda_mean * 1e9);
        grMean->SetLineColor(kGreen+2);
        grMean->SetLineWidth(2);
        grMean->SetLineStyle(2);
        grMean->Draw("L SAME");
    }
    
    can->Update();
    app.Run();
    
    return 0;
}