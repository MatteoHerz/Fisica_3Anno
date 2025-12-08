#include <TFile.h>
#include <TTree.h>
#include <iostream>

void treereader() {

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

        // Print the values of the variables for the current event
        std::cout << "particle = " << pN << ", "
                  << "Energy = " << E << ", "
                  << "Momentum = (" << Mom[0] << ", "
                  << Mom[1] << ", " << Mom[2] << ")" << std::endl;
    }

    // Close the file
    file->Close();
}

int main(){
	treereader(); 
	return 0; 	
}
