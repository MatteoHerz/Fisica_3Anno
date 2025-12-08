#include <TFile.h>
#include <TCanvas.h>
#include <TH1D.h>
#include <TH1F.h>
#include <TF1.h>
#include <TTree.h>
#include <TAxis.h>
#include <TLegend.h>
#include <TMath.h>
#include <TSystem.h>
#include <cmath>
#include <iostream>
using namespace std; 

int sturgesRule(int n) {
    // Usa ceil invece di CeilNint per evitare problemi di linking
    return (int)ceil(1 + 3.322 * log10(n));
}

void selectionMass(){
    int bin1, bin2, n=0;
    
    TFile *file = new TFile("dimuontree.root","READ"); 
    if (!file || file->IsZombie()) {
        cerr << "Errore: impossibile aprire il file!" << endl;
        return;
    }
    
    TTree *masstree = (TTree*)file->Get("tree");
    if (!masstree) {
        cerr << "Errore: tree non trovato!" << endl;
        file->Close();
        return;
    }
    
    float mass, dcaMuons, dcaPV; 
    masstree->SetBranchAddress("mass", &mass);
    masstree->SetBranchAddress("dcaMuons", &dcaMuons);
    masstree->SetBranchAddress("dcaPV", &dcaPV);
    
    int nEntries = masstree->GetEntries();
    
    // Primo loop: conta eventi selezionati
    for(int i=0; i<nEntries; i++) {
        masstree->GetEntry(i);
        if(abs(dcaMuons)<=0.3 && abs(dcaPV)<=0.3){
            n++; 
        }
    }
    
    bin1 = sturgesRule(nEntries);
    bin2 = sturgesRule(n);
    
    // Istogrammi con binning ragionevole
    TH1F *h1fsel = new TH1F("h1dsel","Selected Mass Distribution",200,2.0,5.0);
    TH1F *h1f = new TH1F("h1d","Total Mass Distribution",200,2.0,5.0);
    
    // Secondo loop: riempi istogrammi
    for(int i=0; i<nEntries; i++) {
        masstree->GetEntry(i);
        h1f->Fill(mass); 
        if(abs(dcaMuons)<=0.3 && abs(dcaPV)<=0.3){
            h1fsel->Fill(mass);
        }
    }
    
    cout << "\n========== STATISTICHE ==========" << endl;
    cout << "Eventi totali: " << h1f->GetEntries() << endl;
    cout << "Eventi selezionati: " << h1fsel->GetEntries() << endl;
    cout << "Efficienza: " << (100.0*n/nEntries) << "%" << endl;
    
    // Stili
    h1f->SetLineColor(kBlue);
    h1f->SetFillColorAlpha(kBlue, 0.3);
    h1fsel->SetLineColor(kBlack);
    h1fsel->SetLineWidth(2);
    h1fsel->SetMarkerStyle(20);
    h1fsel->SetMarkerSize(0.5);
    
    TCanvas *c1 = new TCanvas("c1", "Mass Distributions", 1600, 900);
    c1->Divide(2,1); 
    
    // Pannello 1: Confronto totale vs selezionato
    c1->cd(1);
    gPad->SetGrid(1, 1);
    gPad->SetLogy(1);
    h1f->GetXaxis()->SetTitle("Inv. Mass [GeV/c^{2}]");
    h1f->GetYaxis()->SetTitle("Events");
    h1f->GetXaxis()->SetRangeUser(2.0, 5.0);
    h1f->Draw("HIST");
    h1fsel->Draw("HIST SAME");
    
    TLegend *leg1 = new TLegend(0.6, 0.7, 0.88, 0.88);
    leg1->AddEntry(h1f, "Total", "f");
    leg1->AddEntry(h1fsel, "Selected", "l");
    leg1->Draw();
    
    // Pannello 2: Fit con due gaussiane + fondo esponenziale
    c1->cd(2);
    gPad->SetGrid(1, 1);
    gPad->SetLogy(0);
    
    // Funzione di fit: 2 gaussiane + fondo esponenziale decrescente
    // gaus(0): A1 * exp(-(x-mean1)^2/(2*sigma1^2))  -> parametri [0],[1],[2]
    // gaus(3): A2 * exp(-(x-mean2)^2/(2*sigma2^2))  -> parametri [3],[4],[5]
    // expo(6): exp([6] + [7]*x)                      -> parametri [6],[7]
    
    TF1 *fitFunc = new TF1("fitFunc", "gaus(0) + gaus(3) + expo(6)", 2.5, 4.5);
    
    // Stima iniziale dei parametri guardando l'istogramma
    double maxBin1 = h1fsel->GetMaximum() * 0.3;  // Altezza picco J/psi (circa 30% del max)
    double maxBin2 = h1fsel->GetMaximum() * 0.08; // Altezza picco psi(2S) (circa 8% del max)
    
    // Parametri iniziali
    // J/psi (picco principale attorno a 3.1 GeV)
    fitFunc->SetParameter(0, maxBin1);     // Ampiezza J/psi
    fitFunc->SetParameter(1, 3.1);         // Media J/psi
    fitFunc->SetParameter(2, 0.05);        // Sigma J/psi
    
    // psi(2S) (picco secondario attorno a 3.7 GeV)
    fitFunc->SetParameter(3, maxBin2);     // Ampiezza psi(2S)
    fitFunc->SetParameter(4, 3.7);         // Media psi(2S)
    fitFunc->SetParameter(5, 0.05);        // Sigma psi(2S)
    
    // Fondo esponenziale decrescente
    fitFunc->SetParameter(6, 8.0);         // Intercetta
    fitFunc->SetParameter(7, -1.5);        // Pendenza negativa
    
    // Limiti sui parametri per guidare il fit
    fitFunc->SetParLimits(1, 3.0, 3.2);    // Media J/psi
    fitFunc->SetParLimits(2, 0.01, 0.15);  // Sigma J/psi
    fitFunc->SetParLimits(4, 3.6, 3.8);    // Media psi(2S)
    fitFunc->SetParLimits(5, 0.01, 0.15);  // Sigma psi(2S)
    fitFunc->SetParLimits(7, -3.0, 0.0);   // Pendenza deve essere negativa
    
    // Nomi dei parametri per output leggibile
    fitFunc->SetParName(0, "A_{J/#psi}");
    fitFunc->SetParName(1, "#mu_{J/#psi}");
    fitFunc->SetParName(2, "#sigma_{J/#psi}");
    fitFunc->SetParName(3, "A_{#psi(2S)}");
    fitFunc->SetParName(4, "#mu_{#psi(2S)}");
    fitFunc->SetParName(5, "#sigma_{#psi(2S)}");
    fitFunc->SetParName(6, "Bkg p0");
    fitFunc->SetParName(7, "Bkg p1");
    
    // Esegui il fit con più iterazioni
    cout << "\n========== ESECUZIONE FIT ==========" << endl;
    for (int i = 0; i < 3; i++) {
        h1fsel->Fit(fitFunc, "RMQ+");  // R=range, M=improve fit, Q=quiet, +=add to list
    }
    
    // Disegna
    h1fsel->GetXaxis()->SetTitle("Inv. Mass [GeV/c^{2}]");
    h1fsel->GetYaxis()->SetTitle("Events");
    h1fsel->GetXaxis()->SetRangeUser(2.5, 4.5);
    h1fsel->Draw("E");  // E = error bars
    
    // Disegna la funzione totale
    fitFunc->SetLineColor(kRed);
    fitFunc->SetLineWidth(2);
    fitFunc->Draw("SAME");
    
    // Crea funzioni separate per visualizzare le componenti
    TF1 *gauss1 = new TF1("gauss1", "gaus", 2.5, 4.5);
    gauss1->SetParameters(fitFunc->GetParameter(0), fitFunc->GetParameter(1), fitFunc->GetParameter(2));
    gauss1->SetLineColor(kBlue);
    gauss1->SetLineStyle(2);
    gauss1->SetLineWidth(2);
    gauss1->Draw("SAME");
    
    TF1 *gauss2 = new TF1("gauss2", "gaus", 2.5, 4.5);
    gauss2->SetParameters(fitFunc->GetParameter(3), fitFunc->GetParameter(4), fitFunc->GetParameter(5));
    gauss2->SetLineColor(kGreen+2);
    gauss2->SetLineStyle(2);
    gauss2->SetLineWidth(2);
    gauss2->Draw("SAME");
    
    TF1 *background = new TF1("background", "expo", 2.5, 4.5);
    background->SetParameters(fitFunc->GetParameter(6), fitFunc->GetParameter(7));
    background->SetLineColor(kMagenta);
    background->SetLineStyle(3);
    background->SetLineWidth(2);
    background->Draw("SAME");
    
    // Legenda
    TLegend *leg2 = new TLegend(0.55, 0.55, 0.88, 0.88);
    leg2->SetBorderSize(1);
    leg2->AddEntry(h1fsel, "Data", "lep");
    leg2->AddEntry(fitFunc, "Total Fit", "l");
    leg2->AddEntry(gauss1, "J/#psi", "l");
    leg2->AddEntry(gauss2, "#psi(2S)", "l");
    leg2->AddEntry(background, "Background", "l");
    leg2->Draw();
    
    c1->Update();
    
    // Stampa risultati del fit
    cout << "\n========== RISULTATI FIT ==========" << endl;
    cout << "Chi2/NDF = " << fitFunc->GetChisquare() << "/" << fitFunc->GetNDF() 
         << " = " << fitFunc->GetChisquare()/fitFunc->GetNDF() << endl;
    cout << "\nJ/psi:" << endl;
    cout << "  Mass = " << fitFunc->GetParameter(1) << " +/- " << fitFunc->GetParError(1) << " GeV/c^2" << endl;
    cout << "  Sigma = " << fitFunc->GetParameter(2) << " +/- " << fitFunc->GetParError(2) << " GeV/c^2" << endl;
    cout << "  Width (FWHM) = " << 2.355*fitFunc->GetParameter(2) << " GeV/c^2" << endl;
    cout << "\npsi(2S):" << endl;
    cout << "  Mass = " << fitFunc->GetParameter(4) << " +/- " << fitFunc->GetParError(4) << " GeV/c^2" << endl;
    cout << "  Sigma = " << fitFunc->GetParameter(5) << " +/- " << fitFunc->GetParError(5) << " GeV/c^2" << endl;
    cout << "  Width (FWHM) = " << 2.355*fitFunc->GetParameter(5) << " GeV/c^2" << endl;
    
    c1->WaitPrimitive();

    file->Close();
}