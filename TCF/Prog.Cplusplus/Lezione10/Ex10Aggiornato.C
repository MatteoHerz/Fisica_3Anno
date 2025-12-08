#include <TFile.h>
#include <TCanvas.h>
#include <TH1D.h>
#include <TH1F.h>
#include <TTree.h>
#include <TMath.h>
#include <cmath>
#include <TLegend.h>
#include <iostream>

int sturgesRule1(int n) {
    return TMath::CeilNint(1 + 3.322 * TMath::Log10(n));
}

void selection(){
    int bin1, bin2, n = 0;

    TFile *file = new TFile("dimuontree.root","READ"); 
    if (!file || file->IsZombie()) {
        std::cout << "Errore: impossibile aprire il file dimuontree.root" << std::endl;
        return;
    }

    TTree *masstree = (TTree*)file->Get("tree");
    if (!masstree) {
        std::cout << "Errore: impossibile trovare l'albero 'tree'" << std::endl;
        file->Close();
        return;
    }

    float mass, dcaMuons, dcaPV; 
    masstree->SetBranchAddress("mass", &mass);
    masstree->SetBranchAddress("dcaMuons", &dcaMuons);
    masstree->SetBranchAddress("dcaPV", &dcaPV);

    // PRIMA: trova il range effettivo delle masse
    float minMass = 1e6, maxMass = -1e6;
    int nEntries = masstree->GetEntries();
    
    for(int i = 0; i < nEntries; i++) {
        masstree->GetEntry(i);
        if(mass < minMass) minMass = mass;
        if(mass > maxMass) maxMass = mass;
        if(abs(dcaMuons) <= 0.3 && abs(dcaPV) <= 0.3) {
            n++; 
        }
    }
    
    std::cout << "Range massa: " << minMass << " - " << maxMass << std::endl;
    std::cout << "Numero di eventi totali: " << nEntries << std::endl;
    std::cout << "Numero di eventi selezionati: " << n << std::endl;

    // Usa un range fisso appropriato per masse di dimuoni
    // o usa il range trovato con un piccolo margine
    double histMin = 0;
    double histMax = 12; // range tipico per masse di dimuoni
    
    // Oppure usa il range automatico:
    // double histMin = minMass - 0.1;
    // double histMax = maxMass + 0.1;

    bin1 = TMath::Min(100, sturgesRule1(nEntries)); // limita a max 100 bin
    bin2 = TMath::Min(100, sturgesRule1(n));

    TH1F *h1fsel = new TH1F("h1dsel", "Selected Mass Distribution", bin2, histMin, histMax);
    TH1F *h1f = new TH1F("h1d", "Total Mass Distribution", bin1, histMin, histMax);

    // RIempi gli istogrammi
    for(int i = 0; i < nEntries; i++) {
        masstree->GetEntry(i);
        h1f->Fill(mass); 
        if(abs(dcaMuons) <= 0.3 && abs(dcaPV) <= 0.3) {
            h1fsel->Fill(mass);
        }
    }

    TCanvas *c1 = new TCanvas("c1", "Mass Distributions", 1600, 800);
    
    h1f->SetLineColor(kBlue);
    h1f->SetFillColorAlpha(kBlue, 0.3);
    h1f->GetXaxis()->SetTitle("Mass (GeV/c^{2})");
    h1f->GetYaxis()->SetTitle("Events");
    
    h1fsel->SetLineColor(kRed);
    h1fsel->SetFillColorAlpha(kRed, 0.3);
    h1fsel->SetLineWidth(2);

    // Disegna prima l'istogramma con più eventi
    if (h1f->GetEntries() > 0) {
        h1f->Draw("HIST");
        h1fsel->Draw("HIST SAME");
        
        // Aggiungi legenda
        TLegend *leg = new TLegend(0.7, 0.7, 0.9, 0.9);
        leg->AddEntry(h1f, "All events", "f");
        leg->AddEntry(h1fsel, "Selected events", "f");
        leg->Draw();
        
        std::cout << "Istogramma totale: " << h1f->GetEntries() << " eventi" << std::endl;
        std::cout << "Istogramma selezionato: " << h1fsel->GetEntries() << " eventi" << std::endl;
    } else {
        std::cout << "ATTENZIONE: Nessun evento negli istogrammi!" << std::endl;
    }

    file->Close();
}