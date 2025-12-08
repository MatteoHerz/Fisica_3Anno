#include <TFile.h>
#include <TCanvas.h>
#include <TH1F.h>
#include <TH2F.h>
#include <TTree.h>
#include <iostream>

void treereader() {

    TH2F *h2d = new TH2F("h2d","Momentum Y vs Momentum X",20,0,6,20,0,6);
    TH1F *h1d = new TH1F("h1d","Energy Distribution",100,0,20);
    TH1F *h1dsel = new TH1F("h1dsel","Energy Distribution for selected events",100,0,20);

    TFile *file = new TFile("outfile.root", "READ");

    TTree *tree = (TTree*)file->Get("mytree");

    // Declare variables to hold the branch data
    int pN;
    float E;
    float Mom[3];

    // Set the branch addresses so that ROOT knows where to store the data
    tree->SetBranchAddress("PartNum", &pN);
    tree->SetBranchAddress("Energy", &E);
    tree->SetBranchAddress("Momentum", Mom);

    // Loop over all the entries in the TTree
    int nEntries = tree->GetEntries();
    for (int i = 0; i < nEntries; i++) {

        tree->GetEntry(i);
        // correlation between px and py
        h2d->Fill(Mom[0],Mom[1]);
        h1d->Fill(E);
        // energy values when px and py are lower than 3
        if(Mom[0]<3 && Mom[1]<3) h1dsel->Fill(E);

    }

    // Close the file
    file->Close();

    TCanvas *c1 = new TCanvas("c1","Momentum Y vs Momentum X",1600,800);
    c1->Divide(2,1);

    c1->cd(1);
    h2d->Draw("COLZ");

    c1->cd(2);
    h1d->SetLineColor(kRed);
    h1dsel->SetLineColor(kBlue);
    h1d->Draw("");
    h1dsel->Draw("SAME");

    c1->SaveAs("CorrAndSel.png");

    return;
}
