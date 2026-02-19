#include <TFile.h>
#include <TTree.h>
#include <TRandom3.h>

void buildTree() {

  TRandom3 gRandom(0);

  // (re)creating a new file
  TFile *file = new TFile("outfile.root", "RECREATE");

  // tree constructor
  TTree *mytree = new TTree("mytree", "Particles tree");

  // local variables used to fill the TTree
  int partNum;
  float energy;
  float momentum[3];

  // Set the tree branches 
  mytree->Branch("PartNum", &partNum, "PartNum/I");
  mytree->Branch("Energy", &energy, "Energy/F");
  mytree->Branch("Momentum", momentum, "Momentum[3]/F");
  
  // OR (for multiple leaves)
  //mytree->Branch("Momentum", momentum, "Px/F:Py/F:Pz/F");
  
  // OR, for single-leaf separated branches
  // float px, py, pz;
  // mytree->Branch("Px", px, "Px/F");
  // mytree->Branch("Py", py, "Py/F");
  // mytree->Branch("Pz", pz, "Pz/F");

  // Filling in a loop
  for (int i = 0; i < 1000; i++) {
    
    partNum = i;
    energy = gRandom.Gaus(10,1);
    momentum[0] = gRandom.Exp(2.4);
    momentum[1] = gRandom.Exp(2.2);
    momentum[2] = gRandom.Exp(6.9);

    // actual filling
    mytree->Fill();
  }

  // writing on the file
  mytree->Write();

  // closing the file
  file->Close();

}

int main(){
	buildTree(); 
	return 0; 
}
