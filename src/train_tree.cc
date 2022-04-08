/*
A simple program to read the tree structure created by AnaSimDst and make a simple tree for off-line analysis
02-11-2022
*/
R__LOAD_LIBRARY(libana_sim_dst)

#include <TFile.h>
#include <TTree.h>
#include <TString.h>
#include <TLorentzVector.h>
#include <iostream>

using namespace std;

const double pi = TMath::Pi(), P = 1.0, D = 1.0;

void train_tree(TString in_name, TString out_name, TString tree_name)
{
	// read the tree created by AnaSimDst
	cout << "read from file : " << in_name.Data() << endl;
	TFile* in_file = TFile::Open(in_name.Data());
	TTree* in_tree = (TTree*)in_file->Get("tree");
	int n = in_tree->GetEntries();

	cout << "entries : " << n << endl;

	EventData* ed = new EventData();
	DimuonList* true_dimu = new DimuonList();
	DimuonList* reco_dimu = new DimuonList();
	
	in_tree->SetBranchAddress("evt", &ed);
	in_tree->SetBranchAddress("dim_true", &true_dimu);
	in_tree->SetBranchAddress("dim_reco", &reco_dimu);

	int reco_stat;
	double weight, true_mass, true_phi, true_pt, true_x1, true_x2, true_xf, reco_mass, reco_phi, reco_pt, reco_x1, reco_x2, reco_xf;

	cout << "create file : " << out_name.Data() << endl;
	TFile* out_file = new TFile(out_name.Data(), "RECREATE"); // note that spin up is 0 and spin down is 1
	TTree* out_tree = new TTree("tree", tree_name.Data());

	out_tree->Branch("reco_stat", &reco_stat, "reco_stat/I");
	out_tree->Branch("weight", &weight, "weight/D");

	out_tree->Branch("true_mass", &true_mass, "true_mass/D");
	out_tree->Branch("true_phi", &true_phi, "true_phi/D");
	out_tree->Branch("true_pt", &true_pt, "true_pt/D");
	out_tree->Branch("true_x1", &true_x1, "true_x1/D");
	out_tree->Branch("true_x2", &true_x2, "true_x2/D");
	out_tree->Branch("true_xf", &true_xf, "true_xf/D");

	out_tree->Branch("reco_mass", &reco_mass, "reco_mass/D");
	out_tree->Branch("reco_phi", &reco_phi, "reco_phi/D");
	out_tree->Branch("reco_pt", &reco_pt, "reco_pt/D");
	out_tree->Branch("reco_x1", &reco_x1, "reco_x1/D");
	out_tree->Branch("reco_x2", &reco_x2, "reco_x2/D");
	out_tree->Branch("reco_xf", &reco_x2, "reco_xf/D");

	for(int i = 0; i < n; i++)
	{
		in_tree->GetEntry(i);
		reco_stat = ed->rec_stat;
		true_mass = true_dimu->at(0).mom.M();
		true_phi = true_dimu->at(0).mom.Phi();
		//double phi_weight = 1 + A_N* P* sin(true_phi + spin); // 0 or pi for spin up/down case -> weight make the histogram
		weight = ed->weight;
		true_pt = true_dimu->at(0).mom.Perp();
		true_x1 = true_dimu->at(0).x1;
		true_x2 = true_dimu->at(0).x2;
		true_xf = true_x1 - true_x2;
		reco_mass = reco_dimu->at(0).mom.M();
		reco_phi = reco_dimu->at(0).mom.Phi();
		reco_pt = reco_dimu->at(0).mom.Perp();
		reco_x1 = reco_dimu->at(0).x1;
		reco_x2 = reco_dimu->at(0).x2;
		reco_xf = reco_x1 - reco_x2;
		out_tree->Fill();
	}
	out_tree->Write();
	//out_file->Write();
	out_file->Close();
	cout << "***				***" << endl;
}