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
#include <TMatrixD.h>
#include <TMath.h>
#include <tuple>

using namespace std;


RooUnfoldResponse* pt_matrix(TString file_name, TString matrix_name, double pt_min, double pt_max, double phi_min, double phi_max, double phi_bins)
{
	/*cout << "read from file : " << file_name.Data() << endl;*/

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

	RooUnfoldResponse* matrix = new RooUnfoldResponse(phi_bins, phi_min, phi_max, matrix_name.Data(), "; true #phi(rad); reco. #phi(rad)");

	/*cout << "train response matrix : " << matrix_name.Data() << endl;*/

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

RooUnfoldResponse* xf_matrix(TString file_name, TString matrix_name, double xf_min, double xf_max, double phi_min, double phi_max, double phi_bins)
{
	/*cout << "read from file : " << file_name.Data() << endl;*/

	TFile* file = TFile::Open(file_name.Data(), "READ");
	TTree* tree = (TTree*)file->Get("tree");
	int n = tree->GetEntries();

	int reco_stat;
	double weight, true_xf, true_phi, reco_phi, reco_mass;

	tree->SetBranchAddress("reco_stat", &reco_stat);
	tree->SetBranchAddress("weight", &weight);
	tree->SetBranchAddress("true_xf", &true_xf);
	tree->SetBranchAddress("true_phi", &true_phi);
	tree->SetBranchAddress("reco_phi", &reco_phi);
	tree->SetBranchAddress("reco_mass", &reco_mass);

	RooUnfoldResponse* matrix = new RooUnfoldResponse(phi_bins, phi_min, phi_max, matrix_name.Data(), "; true #phi(rad); reco. #phi(rad)");

	/*cout << "train response matrix : " << matrix_name.Data() << endl;*/

	for(int i = 0; i < n; i++)
	{
		tree->GetEntry(i);
		if(xf_min < true_xf && true_xf < xf_max)
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

TH1D* pt_histo(TString file_name, TString hist_name, double pt_min, double pt_max, double phi_min, double phi_max, double mass_min, double mass_max, double mass_bins, double int_lumi, double exp_lumi)
{
	/*cout << "read from file : " << file_name.Data() << endl;
	cout << "total integrated luminocity : " << int_lumi << endl;
	cout << "expected luminicity : " << exp_lumi << endl;*/

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

	TString hist_title = Form("%f < pT < %f & %f < #phi < %f; mass(GeV/c^{2}); Yield", pt_min, pt_max, phi_min, phi_max);

	TH1D* hist = new TH1D(hist_name.Data(), hist_title.Data(), mass_bins, mass_min, mass_max);

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

TH1D* xf_histo(TString file_name, TString hist_name, double xf_min, double xf_max, double phi_min, double phi_max, double mass_min, double mass_max, double mass_bins, double int_lumi, double exp_lumi)
{
	/*cout << "read from file : " << file_name.Data() << endl;
	cout << "total integrated luminocity : " << int_lumi << endl;
	cout << "expected luminicity : " << exp_lumi << endl;*/

	TFile* file = TFile::Open(file_name.Data(), "READ");
	TTree* tree = (TTree*)file->Get("tree");
	int n = tree->GetEntries();

	int reco_stat;
	double weight, reco_xf, reco_phi, reco_mass;

	tree->SetBranchAddress("reco_stat", &reco_stat);
	tree->SetBranchAddress("weight", &weight);
	tree->SetBranchAddress("reco_xf", &reco_xf);
	tree->SetBranchAddress("reco_phi", &reco_phi);
	tree->SetBranchAddress("reco_mass", &reco_mass);

	TString hist_title = Form("%f < xF < %f & %f < #phi < %f; mass(GeV/c^{2}); Yield", xf_min, xf_max, phi_min, phi_max);

	TH1D* hist = new TH1D(hist_name.Data(), hist_title.Data(), mass_bins, mass_min, mass_max);

	//cout << "fill histo : " << hist_name.Data() << endl;

	for(int i = 0; i < n; i++)
	{
		tree->GetEntry(i);
		if(reco_stat == 0)
		{
			if(xf_min < reco_xf && reco_xf < xf_max)
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
	//M.SetStats(0);
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
	double count = hist->IntegralAndError(bin1, bin2, error, "width");

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

	gStyle->SetOptFit();
	hist->SetMarkerColor(8);
	hist->SetMarkerStyle(8);
	hist->Fit(f1);
	hist->Draw("E1 P0");

	can->Draw();
}


void pre_analysis(TString data1, TString data2, double int_lumi1, double int_lumi2, double exp_lumi)
{
	double pi = TMath::Pi();

	TFile* file1 = TFile::Open(data1.Data(), "READ");
	TTree* tree1 = (TTree*)file1->Get("tree");
	int n1 = tree1->GetEntries();

	TFile* file2 = TFile::Open(data2.Data(), "READ");
	TTree* tree2 = (TTree*)file2->Get("tree");
	int n2 = tree2->GetEntries();

	TH1D* h1 = new TH1D("h1", "; mass(GeV/c^{2}); Yield", 30, 1.0, 6.0);
	TH1D* h2 = new TH1D("h2", "; mass(GeV/c^{2}); Yield", 30, 1.0, 6.0);

	TH1D* h3 = new TH1D("h3", "; pT(GeV/c); Yield", 5, 0.0, 5.0);
	TH1D* h4 = new TH1D("h4", "; pT(GeV/c); Yield", 5, 0.0, 5.0);

	TH1D* h5 = new TH1D("h5", "; xF(GeV/c); Yield", 5, 0.0, 0.5);
	TH1D* h6 = new TH1D("h6", "; xF(GeV/c); Yield", 5, 0.0, 0.5);

	tree1->Project("h1", "reco_mass", "weight*(reco_stat == 0)");
	tree2->Project("h2", "reco_mass", "weight*(reco_stat == 0)");

	tree1->Project("h3", "reco_pt", "weight*(reco_stat == 0)");
	tree2->Project("h4", "reco_pt", "weight*(reco_stat == 0)");

	tree1->Project("h5", "reco_xf", "weight*(reco_stat == 0)");
	tree2->Project("h6", "reco_xf", "weight*(reco_stat == 0)");

	h1->Scale(exp_lumi/int_lumi1);
	h2->Scale(exp_lumi/int_lumi2);
	h1->Add(h2);


	h3->Scale(exp_lumi/int_lumi1);
	h4->Scale(exp_lumi/int_lumi2);
	h3->Add(h4);

	h5->Scale(exp_lumi/int_lumi1);
	h6->Scale(exp_lumi/int_lumi2);
	h5->Add(h6);

	//plot_hist(h1);
	plot_hist(h3);
	plot_hist(h5);

}