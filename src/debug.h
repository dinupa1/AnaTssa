#include <TFile.h>
#include <TTree.h>
#include <TH1D.h>
#include <TCanvas.h>

using namespace std;

void plot_histo(TH1D* hist)
{
	TCanvas* can = new TCanvas(hist->GetName()); can->SetGrid();
	hist->SetMarkerStyle(8);
	hist->SetMarkerColor(8);

	hist->Draw("E1 P0");
	can->Draw();
}

TH1D* true_histo(TString file_name, double mass_min, double mass_max, double phi_bins, double phi_min, double phi_max, double int_lumi, double exp_lumi, TString hist_name)
{
	cout << "read from file : " << file_name.Data() << endl;
	TFile* file = TFile::Open(file_name.Data(), "READ");
	TTree* tree = (TTree*)file->Get("tree");
	int n = tree->GetEntries();

	cout << "entries : " << n << endl;

	int reco_stat;
	double weight, true_phi, true_mass;

	TString hist_title = Form("%f < m < %f; #phi(rad); Yield", mass_min, mass_max);
	TH1D* hist = new TH1D(hist_name.Data(), hist_title.Data(), phi_bins, phi_min, phi_max);


	tree->SetBranchAddress("reco_stat", &reco_stat);
	tree->SetBranchAddress("weight", &weight);
	tree->SetBranchAddress("true_phi", &true_phi);
	tree->SetBranchAddress("true_mass", &true_mass);

	for(int i = 0; i < n; i++)
	{
		tree->GetEntry(i);
		if(mass_min < true_mass && true_mass < mass_max){hist->Fill(true_phi, weight);}
	}

	hist->Scale(exp_lumi/int_lumi);

	return hist;
}

TH1D* reco_histo(TString file_name, double mass_min, double mass_max, double phi_bins, double phi_min, double phi_max, double int_lumi, double exp_lumi, TString hist_name)
{
	cout << "read from file : " << file_name.Data() << endl;
	TFile* file = TFile::Open(file_name.Data(), "READ");
	TTree* tree = (TTree*)file->Get("tree");
	int n = tree->GetEntries();

	cout << "entries : " << n << endl;

	int reco_stat;
	double weight, reco_phi, reco_mass;

	TString hist_title = Form("%f < m < %f; #phi(rad); Yield", mass_min, mass_max);
	TH1D* hist = new TH1D(hist_name.Data(), hist_title.Data(), phi_bins, phi_min, phi_max);


	tree->SetBranchAddress("reco_stat", &reco_stat);
	tree->SetBranchAddress("weight", &weight);
	tree->SetBranchAddress("reco_phi", &reco_phi);
	tree->SetBranchAddress("reco_mass", &reco_mass);

	for(int i = 0; i < n; i++)
	{
		tree->GetEntry(i);
		if(reco_stat == 0)
		{
			if(mass_min < reco_mass && reco_mass < mass_max){hist->Fill(reco_phi, weight);}
		}
	}

	hist->Scale(exp_lumi/int_lumi);

	return hist;
}

TH1D* asymmetry(TH1D* hist1, TH1D* hist2, TString hist_name)
{
	TH1D* hist = (TH1D*)hist1->GetAsymmetry(hist2);
	//TString hist_title = Form("%s ; #phi(rad); A_{N}", title.Data());
	hist->SetNameTitle(hist_name.Data(), "; #phi(rad); A_{N}");

	return hist;
}