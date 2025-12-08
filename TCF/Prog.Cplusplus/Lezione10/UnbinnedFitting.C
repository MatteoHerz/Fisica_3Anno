#include <TFile.h>
#include <TCanvas.h>
#include <TH1F.h>
#include <TH2F.h>
#include <TF1.h>
#include <TTree.h>

void treefitter() {

    TCanvas *canvas = new TCanvas("canvas", "Unbinned Fit", 800, 800);

    TF1 *gaussian = new TF1("gaussian", "gaus(0)", 0, 20);  // Gaussian function
    gaussian->SetParameters(100, 10, 1); // Initial parameters: amplitude, mean, sigma
    gaussian->SetNpx(1000);
    
    TFile *file = new TFile("outfile.root", "READ");

    TTree *tree = (TTree*)file->Get("mytree");
    
    canvas->cd();
    
    tree->Fit("gaussian","Energy","","U0N"); // Option U performs unbinned fitting

    tree->Draw("Energy");
    gaussian->Draw("SAME");

    // Print the fit results
    gaussian->Print();
    
    return;
}
