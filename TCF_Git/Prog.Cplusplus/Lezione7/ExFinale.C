#include "TH1F.h"
#include "TF1.h"
#include <TCanvas.h>
#include <TApplication.h>
#include <TGraph.h>
#include <TAxis.h>
#include <iostream>
#include <cmath>
#include <vector>
#include <fstream>

using namespace std;

int main(){

    TApplication app("app",0,0);

    TH1D *hist = new TH1D("hist","Invariant mass distribution",100,1.3,5);

    double readvalue;
    ifstream infile("jpsimass.txt");

    while(infile >> readvalue){
        hist->Fill(readvalue);
    }

    TF1 *modelfit = new TF1("modelfit","gaus(0)+gaus(5)+pol1(3)",1,5);
    modelfit->SetNpx(5000);
    modelfit->SetParameters(7e3,3.1,0.14,4.8e4,-1.6e3,1500,3.6,0.14);
    modelfit->SetParLimits(6,3.4,3.7); 
    
    for(int i=0; i<4; i++){
        hist->Fit(modelfit,"0");
    }

    TCanvas *can = new TCanvas("can","can",1000,1000);
    can->SetMargin(0.15,0.05,0.1,0.1);
    can->cd();

    hist->GetXaxis()->SetTitle("m_{#mu^{+}#mu^{-}}");
    hist->GetYaxis()->SetTitle("counts");
    hist->Draw("E");
    modelfit->Draw("same");

    app.Run();
    
    return 0;
}

