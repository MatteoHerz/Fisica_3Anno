#include <TFile.h>
#include <TCanvas.h>
#include <TH1D.h>
#include <TH1F.h>
#include <TF1.h>
#include <TTree.h>
#include <TAxis.h>
#include <TMath.h>
#include <TSystem.h>
#include <cmath>
#include <iostream>
using namespace std; 

int sturgesRule(int n) {
    return TMath::CeilNint(1 + 3.322 * TMath::Log10(n));
}

void selectionMass(){

    int bin1, bin2, n=0;

    TFile *file = new TFile("dimuontree.root","READ"); 
    TTree *masstree = (TTree*)file->Get("tree");

    float mass, dcaMuons, dcaPV; 
    masstree->SetBranchAddress("mass", &mass);
    masstree->SetBranchAddress("dcaMuons", &dcaMuons);
    masstree->SetBranchAddress("dcaPV", &dcaPV);

    int nEntries = masstree->GetEntries();
    for(int i=0; i<nEntries; i++)
    {
        masstree->GetEntry(i);
        if(abs(dcaMuons)<=0.3 && abs(dcaPV)<=0.3){
            n++; 
        }
    }

    bin1 = sturgesRule(nEntries);
    bin2 = sturgesRule(n);

    //cout<<"\nnEntries = "<<nEntries<<endl;; 
    //cout<<"bin1 = "<<bin1<<endl; 
    //cout<<"bin1 = "<<bin2<<endl; 

    TH1F *h1fsel = new TH1F("h1dsel","Selected Mass Distribution",750,0,7);
    TH1F *h1f = new TH1F("h1d","Total Mass Distribution",700,0,7);

    for(int i=0; i<nEntries; i++)
    {
        masstree->GetEntry(i);
        h1f->Fill(mass); 
        if(abs(dcaMuons)<0.3 && abs(dcaPV)<0.3){
            h1fsel->Fill(mass);
        }
    }

    cout << "Eventi nell'istogramma totale: " << h1f->GetEntries() << endl;
    cout << "Eventi nell'istogramma selezionato: " << h1fsel->GetEntries() <<endl<<endl;
    
    TCanvas *c1 = new TCanvas("c1", "Mass Distributions", 1600, 900);

    c1->Divide(2,1); 

    c1->cd(1);

    h1f->SetLineColor(kBlue);
    h1f->SetFillColorAlpha(kBlue, 0.3);
    h1fsel->SetLineColor(kRed);
    h1fsel->SetFillColorAlpha(kRed, 0.3);
    gPad->SetGrid(1, 1);
    gPad->SetLogy(1);  // Utile per vedere piccoli segnali su fondo grande

    h1f->GetXaxis()->SetTitle("Inv. Mass [GeV/c^{2}]");
    h1f->GetYaxis()->SetTitle("Bin counts");
    h1f->Draw("HIST");
    h1fsel->Draw("HIST SAME");
    h1f->GetXaxis()->SetRangeUser(1.9, 5);  // Solo regione J/ψ

    c1->cd(2);
    gPad->SetGrid(1, 1);
    gPad->SetLogy(0);

    TF1 *fitFunc = new TF1("fitFunc", "gaus(0) + gaus(3) + expo(6)", 2, 5);
    fitFunc->SetParameters(600, 3.1, 0.1, 150, 3.6, 0.1, 8.0, -1.5);
    fitFunc->SetParLimits(1,2.8,3.4);
    fitFunc->SetParLimits(4,3.5,3.7);
    fitFunc->SetParLimits(7, -3.0, 0.0);

    for (int i = 0; i < 5; i++)
    {
        h1fsel->Fit(fitFunc,"U0N");
    }

    fitFunc->SetLineColor(kBlue);
    fitFunc->SetLineWidth(2);
    h1fsel->SetLineColor(kRed);
    h1fsel->SetFillColorAlpha(kRed, 0.3);
    
    h1fsel->GetXaxis()->SetTitle("Inv. Mass Fitting [GeV/c^{2}]");
    h1fsel->GetYaxis()->SetTitle("Bin counts");
    h1fsel->Draw("HIST");
    fitFunc->Draw("SAME");
    h1fsel->GetXaxis()->SetRangeUser(1.9, 5.1);
    
    c1->Update();
    c1->SetGrid();
    c1->Draw();
    
    // Mantieni il canvas aperto
    c1->WaitPrimitive();
    
    file->Close();
    
    return; 
}