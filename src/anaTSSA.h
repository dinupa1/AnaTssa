/*
functions used in the anaTSSA framework
*/

R__LOAD_LIBRARY(../RooUnfold/libRooUnfold)

#include <TFile.h>
#include <TTree.h>
#include <TH1D.h>
#include <TF1.h>
#include <TCanvas.h>
#include <RooUnfoldResponse.h>
#include <RooUnfoldBayes.h>
#include <Riostream.h>
#include <tuple>

using namespace std;

RooUnfoldResponse* train_matrix(TString file_name, TString matrix_name, double pt_min, double pt_max, double phi_min, double phi_max, double phi_bins)
{
	cout << "read from file : " << file_name.Data() << endl;

	TFile* file = TFile::Open(file_name.Data(), "READ");
	TTree* tree = (TTree*)file->Get("tree");
	int n = tree->GetEntries();

	int reco_stat;
	double weight, true_pt, true_phi, reco_phi, reco_mass;

	tree->SetBranchAddress("reco_stat", &reco_stat);
	tree->SetBranchAddress("weight", &weight);
	tree->SetBranchAddress("true_pt", &true_pt);
	tree->SetBranchAddress("true_phi", &true_phi);
	tree->SetBranchAddress("reco_phi", &reco_phi);
	tree->SetBranchAddress("reco_mass", &reco_mass);

	RooUnfoldResponse* matrix = new RooUnfoldResponse(phi_bins, phi_min, phi_max, matrix_name.Data());

	cout << "train response matrix : " << matrix_name.Data() << endl;

	for(int i = 0; i < n; i++)
	{
		tree->GetEntry(i);
		if(pt_min < true_pt && true_pt < pt_max)
		{
			if(reco_stat == 0)
			{
				//if(2.5 < reco_mass && reco_mass < 4.0){matrix->Fill(reco_phi, true_phi);}
                matrix->Fill(reco_phi, true_phi);

			}
		}
	}

	return matrix;
}

TH1D* make_histo(TString file_name, TString hist_name, double pt_min, double pt_max, double phi_min, double phi_max, double mass_min, double mass_max, double mass_bins, double int_lumi, double exp_lumi)
{
	cout << "read from file : " << file_name.Data() << endl;
	cout << "total integrated luminocity : " << int_lumi << endl;
	cout << "expected luminicity : " << exp_lumi << endl;

	TFile* file = TFile::Open(file_name.Data(), "READ");
	TTree* tree = (TTree*)file->Get("tree");
	int n = tree->GetEntries();

	int reco_stat;
	double weight, reco_pt, reco_phi, reco_mass;

	tree->SetBranchAddress("reco_stat", &reco_stat);
	tree->SetBranchAddress("weight", &weight);
	tree->SetBranchAddress("reco_pt", &reco_pt);
	tree->SetBranchAddress("reco_phi", &reco_phi);
	tree->SetBranchAddress("reco_mass", &reco_mass);

	TH1D* hist = new TH1D(hist_name.Data(), "; mass(GeV/c^{2}); Yield", mass_bins, mass_min, mass_max);

	//cout << "fill histo : " << hist_name.Data() << endl;

	for(int i = 0; i < n; i++)
	{
		tree->GetEntry(i);
		if(reco_stat == 0)
		{
			if(pt_min < reco_pt && reco_pt < pt_max)
			{
				if(phi_min < reco_phi && reco_phi < phi_max)
				{
					//cout << "pt_min : " << pt_min << " pt_max : " << pt_max << " phi_min : " << phi_min << " phi_max : " << phi_max << endl;
					hist->Fill(reco_mass, weight);
				}
			}
		}
	}

	hist->Scale(exp_lumi/int_lumi);

	return hist;
}

void plot_mass(TH1D* h1, TH1D* h2)
{
	h1->SetMarkerColor(8);
	h1->SetMarkerStyle(8);

	h2->SetFillColor(2);
	h2->SetFillStyle(3001);

	TCanvas* can = new TCanvas(h1->GetName()); can->SetGrid();
	h1->Draw("E1 P0");
	h2->Draw("E4 SAME");
	can->Draw();
}

void plot_matrix(RooUnfoldResponse* matrix)
{
	auto M = matrix->Mresponse();
	TCanvas* can = new TCanvas();can->SetGrid();
	M.Draw("COL TEXT");
	can->Draw();
}

void plot_hist(TH1D* hist)
{
	hist->SetMarkerColor(8);
	hist->SetMarkerStyle(8);
	TCanvas* can = new TCanvas(hist->GetName());can->SetGrid();
	hist->Draw("E1 P0");
	can->Draw();
}

tuple<double, double> integrate(TH1D* hist, double min, double max)
{
	double error;
	double bin1 = hist->FindBin(min);
	double bin2 = hist->FindBin(max);
	double count = hist->IntegralAndError(bin1, bin2, error);

	return {count, error};
}

TH1D* unfold(RooUnfoldResponse* matrix, TH1D* hist, TString hist_name, int niter)
{
	RooUnfoldBayes H1(matrix, hist, niter);
	TH1D* hist1 = (TH1D*)H1.Hreco();
	hist1->SetNameTitle(hist_name.Data(), "; #phi(rad); Yield");
	return hist1;
}

TH1D* asymmetry(TH1D* hist1, TH1D* hist2, TString hist_name)
{
	TH1D* hist = (TH1D*)hist1->GetAsymmetry(hist2);
	hist->SetNameTitle(hist_name.Data(), "; #phi(rad); A_{N}");
	return hist;
}

void plot_asym(TH1D* hist, TF1* f1)
{
	TCanvas* can = new TCanvas(hist->GetName()); can->SetGrid();

	hist->SetMarkerColor(8);
	hist->SetMarkerStyle(8);
	hist->Fit(f1);
	hist->Draw("E1 P0");

	can->Draw();
}