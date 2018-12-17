#include <iostream>
#include "TFile.h"
#include "TTree.h"
#include <fstream>
using namespace std;

void eff_run(int run_i, int run_f, int ch, float x_i, float x_f, float y_i, float y_f, float thres, int isvme)
{
 
  //Warning: photek cut is hardcoded
 
  TH1F* h_num = new TH1F("h_num","", run_f - run_i + 1, run_i, run_f);
  TH1F* h_den = new TH1F("h_den", "", run_f - run_i + 1, run_i, run_f);
  TH1F* h_dummy = new TH1F("h_dummy", "", run_f - run_i + 1, run_i, run_f);
  vector<float> no_pulse;
  vector<float> no_rootfile;
  vector<float> no_tracks;
  vector<float> bad_tracks;
  float x_dut;
  for (int i=run_i; i < run_f + 1; i++)
    {
      //TString root_filename = Form("/data/TestBeam/2018_11_November_CMSTiming/RECO/v6/DataVMETiming_Run%i.root", i);
      TString root_filename = Form("/Users/rheller/cernbox/ETL/tb/W6_2x8_bv360/DataVMETiming_Run%i_2x2mm_2x8_W6_bv360.root", i);
      //if(isvme==0) TString root_filename = Form("/data/TestBeam/2018_11_November_CMSTiming/DT5742/RECO/v6/DT5742_Run%i.root", i);
      if(isvme==0) root_filename = Form("/Users/rheller/cernbox/ETL/tb/W6_2x8_bv360/DT5742_Run%i_2x2mm_2x8_W6_bv360.root", i);
      TFile *f = TFile::Open(root_filename);
      if(f) //file exist
	{
	  TTree *pulse = (TTree*)f->Get("pulse");                                                                               
	  if(pulse) //pulse exist
	    {
	      TBranch * b_x_dut = pulse->Branch("x_dut",&x_dut,"x_dut/F");
	      if(b_x_dut) //track exist
		{
		  TString den_cmd = Form("ntracks==1&&chi2<3&&x_corr>%.2f&&x_corr<%.2f&&y_corr>%.2f&&y_corr<%.2f&&((amp[4]>10&&amp[4]<50&&run<1766)||(amp[0]>70&&amp[0]<300&&run>=1766))", x_i, x_f, y_i, y_f);
		  TString num_cmd = Form("chidx[%i]>=0&&amp[chidx[%i]]>%.2f&&%s", ch,ch,thres,den_cmd.Data());
		  
      float effi;
      float den_entries = pulse->GetEntries(den_cmd);
      if(den_entries>0) effi = pulse->GetEntries(num_cmd)/den_entries;
		  if (effi<0.8) bad_tracks.push_back(i); //bad efficiency
		  h_num->SetBinContent(h_num->FindBin(i), pulse->GetEntries(num_cmd));
		  h_den->SetBinContent(h_den->FindBin(i), pulse->GetEntries(den_cmd));
		}
	      else no_tracks.push_back(i);
	    }
	  else no_pulse.push_back(i);
	  f->Close();
	}
      else no_rootfile.push_back(i);
    }

  TCanvas *c1 = new TCanvas("c1", "c1",900,900);
  c1->Divide(1,3);
  c1->cd(1);
  h_num->Draw();
  c1->cd(2);
  h_den->Draw();
  c1->cd(3);
  TGraphAsymmErrors* h_eff = new TGraphAsymmErrors();
  h_dummy->Draw();
  h_eff->Divide(h_num,h_den,"cl=0.683 b(1,1) mode");    
  h_eff->Draw("EP same");

  for (int i=0;i<no_rootfile.size();i++) std::cout<<"Missing root file"<<no_rootfile[i]<<endl;
  for (int i=0;i<no_pulse.size();i++) std::cout<<"Missing pulse"<<no_pulse[i]<<endl;
  for (int i=0;i<no_tracks.size();i++) std::cout<<"Missing tracks"<<no_tracks[i]<<endl;
  for (int i=0;i<bad_tracks.size();i++) std::cout<<"Bad tracks"<<bad_tracks[i]<<endl;
  
} 






