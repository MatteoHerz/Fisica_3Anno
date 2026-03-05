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

int main(){
	double a = 3e-6, d = 1, value = 0;
	vector<double> xmeans;
	vector<double> all_data;


	ifstream in_file("InterfPattern.txt");
	
	while(in_file >> value) {
   	 all_data.push_back(value);
	}
	in_file.close();

	int N = all_data.size();

	int optimal_bins = 1 + 3.322 * log10(N); // Sturges rules for # of bins


	TH1D *photons = new TH1D("photons","Photons Collector",300,-10, 10);
	ifstream in_file0;
	in_file0.open("InterfPattern.txt", ios::in);

	while(true){
		in_file0 >> value; 
		if(in_file0.eof()==true) break;
		photons->Fill(value); 
	}

	TApplication app("app",0,0);
	TCanvas *can = new TCanvas("","",800,800);
	can->SetGrid();
	can->cd();
	photons->Draw();

	//Inizializza la classe in_file
	in_file.clear(); 
	in_file.seekg(0, ios::beg); 

	//
		TH1D *mean1 = new TH1D("mean1","Photons Collector",30, 1.5, 5);

		ifstream in_file1;
		in_file1.open("InterfPattern.txt", ios::in);

		while(true){
			in_file1 >> value; 
			if(in_file1.eof()==true) break;
			mean1->Fill(value); 
		}
		xmeans.push_back((mean1->GetMean())*1e-3);
		in_file1.close();
	//
	
	//
		TH1D *mean2 = new TH1D("mean2","Photons Collector",30, 5, 8);

		ifstream in_file2;
		in_file2.open("InterfPattern.txt", ios::in);

		while(true){
			in_file2 >> value; 
			if(in_file2.eof()==true) break;
			mean2->Fill(value); 
		}
		xmeans.push_back((mean2->GetMean())*1e-3);
		in_file2.close();
	//
	
	//
		TH1D *mean3 = new TH1D("mean3","Photons Collector",30, 8, 10);

		ifstream in_file3;
		in_file3.open("InterfPattern.txt", ios::in);

		while(true){
			in_file3 >> value; 
			if(in_file3.eof()==true) break;
			mean3->Fill(value); 
		}
		xmeans.push_back((mean3->GetMean())*1e-3);
		in_file3.close();
	//

	double lambda = 0.0; 
	
	for(int i=0; i<xmeans.size(); i++)
	{
		lambda += xmeans[i]*a/(d*(i+1));
		cout <<"\nLambda "<<i+1<<" = "<< xmeans[i]*a/(d*(i+1));
	}

	cout <<"\nFinal Lambda = " <<lambda/3<<endl;

	app.Run();

	return 0; 
}
