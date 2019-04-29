#include <iostream>
#include "TFile.h"
#include "TTree.h"
#include <fstream>
using namespace std;

void eff_run(int run_i, int run_f, int ch, float x_i, float x_f, float y_i, float y_f, float thres, int isvme)
{
 
  //Warning: photek cut is hardcoded
 
  TH1F* h_num = new TH1F("h_num","", run_f - run_i + 1, run_i, run_f+1);
  TH1F* h_num1 = new TH1F("h_num1","", run_f - run_i + 1, run_i, run_f+1);
  TH1F* h_num2= new TH1F("h_num2","", run_f - run_i + 1, run_i, run_f+1);
  TH1F* h_num3 = new TH1F("h_num3","", run_f - run_i + 1, run_i, run_f+1);
  TH1F* h_num4 = new TH1F("h_num4","", run_f - run_i + 1, run_i, run_f+1);
  
  TH1F* h_numfake = new TH1F("h_numfake","", run_f - run_i + 1, run_i, run_f+1);
  TH1F* h_num_end = new TH1F("h_num_end","", run_f - run_i + 1, run_i, run_f+1);
  TH1F* h_num_start = new TH1F("h_num_start","", run_f - run_i + 1, run_i, run_f+1);
 

  TH1F* h_den = new TH1F("h_den", "", run_f - run_i + 1, run_i, run_f+1);
  TH1F* h_den1 = new TH1F("h_den1", "", run_f - run_i + 1, run_i, run_f+1);
  TH1F* h_den2 = new TH1F("h_den2", "", run_f - run_i + 1, run_i, run_f+1);
  TH1F* h_den3 = new TH1F("h_den3", "", run_f - run_i + 1, run_i, run_f+1);
  TH1F* h_den4 = new TH1F("h_den4", "", run_f - run_i + 1, run_i, run_f+1);
  

  TH1F* h_denfake = new TH1F("h_denfake", "", run_f - run_i + 1, run_i, run_f+1);
  TH1F* h_den_end = new TH1F("h_den_end", "", run_f - run_i + 1, run_i, run_f+1);
  TH1F* h_den_start = new TH1F("h_den_start", "", run_f - run_i + 1, run_i, run_f+1);
  
  TH1F* h_dummy = new TH1F("h_dummy", "", run_f - run_i + 1, run_i, run_f+1);
  TH1F* h_skipEventsC = new TH1F("h_skipEventsC", "", run_f - run_i + 1, run_i, run_f+1);
  TH1F* h_skipEventsD = new TH1F("h_skipEventsD", "", run_f - run_i + 1, run_i, run_f+1);

  vector<float> no_pulse;
  vector<float> no_rootfile;
  vector<float> no_tracks;
  vector<float> bad_tracks;
  vector<float> def_bad_tracks;
  vector<float> good_run;
  float x_dut;
  float y_dut;
  Float_t x_corr[5];
  for (int i=run_i; i < run_f + 1; i++)
    {
      if(i==3171) continue;
      TString root_filename = Form("/Users/rheller/tb_tmp/v9.2_skipC/VME_RECO_Run%i_2x2mm_2x8_W6_bv360.root", i);
     // TString root_filename = Form("/data/TestBeam/2018_11_November_CMSTiming/RECO/v5/DataVMETiming_Run%i.root", i);
      //TString root_filename = Form("/Users/rheller/cernbox/ETL/tb/W6_2x8_bv360/DataVMETiming_Run%i_2x2mm_2x8_W6_bv360.root", i);                                 
      //if(isvme==0) TString root_filename = Form("/data/TestBeam/2018_11_November_CMSTiming/DT5742/RECO/v6/DT5742_Run%i.root", i);                                
      //if(isvme==0) root_filename = Form("/Users/rheller/cernbox/ETL/tb/W6_2x8_bv360/DT5742_Run%i_2x2mm_2x8_W6_bv360.root", i); 
      //TString root_filename = Form("/eos/uscms/store/group/cmstestbeam/BTL_ETL/2018_11/data/DT5742/RECO/v6/backup/DT5742_Run%i_2x2mm_2x8_W6_bv360.root", i);
      //TString root_filename = Form("/eos/uscms/store/group/cmstestbeam/BTL_ETL/2018_11/data/DT5742/RECO/v6/DT5742_Run%i.root", i);                               
      //TString root_filename = Form("/eos/uscms/store/group/cmstestbeam/BTL_ETL/2018_11/data/VME/RECO/v6/DataVMETiming_Run%i_2x2mm_2x8_W6_bv360.root", i);         
      TFile *f = TFile::Open(root_filename);
      // if (!f){
      //   root_filename = Form("/eos/uscms/store/user/cmstestbeam/BTL_ETL/2018_11_recover/RECO/v7/recover/DataVMETiming_Run%i.root", i);
      //   f = TFile::Open(root_filename);
      // }
      if(f) //file exist                                                                                                                                            
        {
          TTree *pulse = (TTree*)f->Get("pulse");
          if(pulse) //pulse exist                                                                                                                                   
            {
              TBranch * b_x_dut = pulse->Branch("x_dut",&x_dut,"x_dut/F");
              TBranch * b_y_dut = pulse->Branch("y_dut",&y_dut,"y_dut/F");

              TBranch * b_x_corr = pulse->Branch("x_corr[5]",&x_corr,"x_corr[5]/F");
              if(!b_x_corr) cout<<"Run "<<i<<" missing x_corr[2]"<<endl;
              int n_with_tracks = pulse->GetEntries("x_dut[2]>-999.");
              if(n_with_tracks>0) //track exist      (This doesn't actually check anything)                                                                                                                       
                {
                  //TH1F * h1 = new TH1F("h1","",100,-400,400); ,"",  20, -0.004, 0.004                                                                                              
		  TH1F * x = new TH1F("x","",120,-0.005,0.005);
                  TH1F * y = new TH1F("y","",120,-0.005,0.005);
                  x->StatOverflows();
                  y->StatOverflows();
                  pulse->Project("x","xSlope","xSlope!=-999");
                  pulse->Project("y","ySlope","ySlope!=-999");
                  int binmax1 = x->GetMaximumBin(); float xMean = x->GetBinCenter(binmax1);
                  float xMeanl = x->GetBinLowEdge(binmax1);
                  float xMeanr = x->GetBinLowEdge(binmax1+1);
                  int binmax2 = y->GetMaximumBin(); float yMean = y->GetBinCenter(binmax2);
                  float yMeanl = y->GetBinLowEdge(binmax2);
                  float yMeanr = y->GetBinLowEdge(binmax2+1);
		  //std::cout<<xMean<<endl;
		  //std::cout<<yMean<<endl;
                  //float xMean = x->GetMean();                                                                                                                     
                  //float yMean = y->GetMean();

                  //TString den_cmd = Form("ntracks==1&&chi2<3&&x_corr>%.2f&&x_corr<%.2f&&y_corr>%.2f&&y_corr<%.2f)", x_i, x_f, y_i, y_f);                                                                                                                                                                                                                                                                                              
                  //TString num_cmd = Form("chidx[%i]>=0&&amp[chidx[%i]]>%.2f&&%s", ch,ch,thres,den_cmd.Data());                                                                                                                                                                                                                                                                                                                        
                  //TString den_cmd = Form("ntracks==1&&chi2<3&&x_dut[2]>%.2f&&x_dut[2]<%.2f&&y_dut[2]>%.2f&&y_dut[2]<%.2f", x_i, x_f, y_i, y_f);
                  //TString num_cmd = Form("amp[%i]>%.2f&&%s", ch,thres,den_cmd.Data());                                                                                                                                                                                                                                                                                                                                                
                  //TString den_cmd = Form("ntracks==1&&chi2<3&&xSlope<%f&&xSlope>%f&&ySlope<%f&&ySlope>%f&&x_dut[2]>%.2f&&x_dut[2]<%.2f&&y_dut[2]>%.2f&&y_dut[2]<%.2f", xMeanr, xMeanl, yMeanr, yMeanl, x_i, x_f, y_i, y_f);




                  TString den_cmd = Form("ntracks==1&&chi2<15&&x_corr>%.2f&&x_corr<%.2f&&y_corr>%.2f&&y_corr<%.2f",  x_i, x_f, y_i, y_f);
                  TString den_cmd1 = Form("ntracks==1&&chi2<15&&nplanes>=15&&x_corr[2]+1.2>%.2f&&x_corr[2]+1.2<%.2f&&y_corr[2]-0.3>%.2f&&y_corr[2]-0.3<%.2f",  0.9, 2.2, 18.3, 19.6);
                  TString den_cmd2 = Form("ntracks==1&&chi2<15&&nplanes>=15&&x_corr[2]+1.2>%.2f&&x_corr[2]+1.2<%.2f&&y_corr[2]-0.3>%.2f&&y_corr[2]-0.3<%.2f",  0.9, 2.2, 20.3, 21.6);
                  TString den_cmd3 = Form("ntracks==1&&chi2<15&&nplanes>=15&&x_corr[2]+1.2>%.2f&&x_corr[2]+1.2<%.2f&&y_corr[2]-0.3>%.2f&&y_corr[2]-0.3<%.2f",  3.05, 4.35, 18.3, 19.6);
                  TString den_cmd4 = Form("ntracks==1&&chi2<15&&nplanes>=15&&x_corr[2]+1.2>%.2f&&x_corr[2]+1.2<%.2f&&y_corr[2]-0.3>%.2f&&y_corr[2]-0.3<%.2f",  3.05, 4.35, 20.3, 21.6);
                 

                  int entries = pulse->GetEntries();
                  int lastN = entries-751;
                  TString den_cmdfake ="ntracks==1&&chi2<15&&nplanes>=15&&(x_corr[2]+1.2<0||y_corr[2]-0.3>22.5||y_corr[2]-0.3<17.4)";
                  
                  TString den_cmd_end = Form("i_evt>%i && ((%s) ||(%s) ||(%s) ||(%s)) ",lastN,den_cmd1.Data(),den_cmd2.Data(),den_cmd3.Data(),den_cmd4.Data());
                  TString den_cmd_start = Form("i_evt<%i && ((%s) ||(%s) ||(%s) ||(%s)) ",lastN,den_cmd1.Data(),den_cmd2.Data(),den_cmd3.Data(),den_cmd4.Data());

                  TString num_cmd = Form("amp[%i]>%.2f&&%s", ch,thres,den_cmd.Data());
                  TString num_cmd_end = Form("(amp[chidx[6]]>15 || amp[chidx[7]]>15 ||amp[chidx[8]]>15 ||amp[chidx[9]]>15) && %s",den_cmd_end.Data());
                  TString num_cmd_start = Form("(amp[chidx[6]]>15 || amp[chidx[7]]>15 ||amp[chidx[8]]>15 ||amp[chidx[9]]>15) && %s",den_cmd_start.Data());

                  TString num_cmd1 = Form("(amp[chidx[6]]>15 || amp[chidx[7]]>15 ||amp[chidx[8]]>15 ||amp[chidx[9]]>15) && %s",den_cmd1.Data());
                  TString num_cmd2 = Form("(amp[chidx[6]]>15 || amp[chidx[7]]>15 ||amp[chidx[8]]>15 ||amp[chidx[9]]>15) && %s",den_cmd2.Data());
                  TString num_cmd3 = Form("(amp[chidx[6]]>15 || amp[chidx[7]]>15 ||amp[chidx[8]]>15 ||amp[chidx[9]]>15) && %s",den_cmd3.Data());
                  TString num_cmd4 = Form("(amp[chidx[6]]>15 || amp[chidx[7]]>15 ||amp[chidx[8]]>15 ||amp[chidx[9]]>15) && %s",den_cmd4.Data());

                  TString num_cmdfake = Form("(amp[chidx[6]]>15 || amp[chidx[7]]>15 ||amp[chidx[8]]>15 ||amp[chidx[9]]>15) && %s",den_cmdfake.Data());

                  float num_entries1 = pulse->GetEntries(num_cmd1);
                  float den_entries1 = pulse->GetEntries(den_cmd1);
                  float num_entries2 = pulse->GetEntries(num_cmd2);
                  float den_entries2 = pulse->GetEntries(den_cmd2);
                  float num_entries3 = pulse->GetEntries(num_cmd3);
                  float den_entries3 = pulse->GetEntries(den_cmd3);
                  float num_entries4 = pulse->GetEntries(num_cmd4);
                  float den_entries4 = pulse->GetEntries(den_cmd4);

                  float num_entriesfake = pulse->GetEntries(num_cmdfake);
                  float den_entriesfake = pulse->GetEntries(den_cmdfake);
                  float num_entries_end = pulse->GetEntries(num_cmd_end);
                  float den_entries_end = pulse->GetEntries(den_cmd_end);
                  float num_entries_start = pulse->GetEntries(num_cmd_start);
                  float den_entries_start = pulse->GetEntries(den_cmd_start);


                  float num_entries = num_entries1+num_entries2+num_entries3+num_entries4;
                  float den_entries = den_entries1+den_entries2+den_entries3+den_entries4;

                  h_num->SetBinContent(h_num->FindBin(i), num_entries);
                  h_den->SetBinContent(h_den->FindBin(i), den_entries);

                  h_num1->SetBinContent(h_num1->FindBin(i), num_entries1);
                  h_den1->SetBinContent(h_den1->FindBin(i), den_entries1);
                  h_num2->SetBinContent(h_num2->FindBin(i), num_entries2);
                  h_den2->SetBinContent(h_den2->FindBin(i), den_entries2);
                  h_num3->SetBinContent(h_num3->FindBin(i), num_entries3);
                  h_den3->SetBinContent(h_den3->FindBin(i), den_entries3);
                  h_num4->SetBinContent(h_num4->FindBin(i), num_entries4);
                  h_den4->SetBinContent(h_den4->FindBin(i), den_entries4) ;
                  h_numfake->SetBinContent(h_numfake->FindBin(i), num_entriesfake);
                  h_denfake->SetBinContent(h_denfake->FindBin(i), den_entriesfake); 
                  h_num_end->SetBinContent(h_num_end->FindBin(i), num_entries_end);
                  h_den_end->SetBinContent(h_den_end->FindBin(i), den_entries_end);
                  h_num_start->SetBinContent(h_num_start->FindBin(i), num_entries_start);
                  h_den_start->SetBinContent(h_den_start->FindBin(i), den_entries_start);

                  int skipEventsC = pulse->GetMaximum("skippedEventsC");
                  int skipEventsD = pulse->GetMaximum("skippedEvents");
                  h_skipEventsD->SetBinContent(h_skipEventsD->FindBin(i),skipEventsD);
                  h_skipEventsC->SetBinContent(h_skipEventsC->FindBin(i),skipEventsC);
                  //cout<<Form("Run %i, skipD %i, skipC %i",i,skipEventsD,skipEventsC)<<endl;
                  float effi; 

                  if(den_entries>0) effi = num_entries/den_entries;
//                  cout<<"effi2 is "<<effi2<<" "<<num_entries_2<<" "<<den_entries_2<<endl;

                  //std::cout<<effi_num<<endl;                                                                                                                      
                  //std::cout<<effi_den<<endl;                                                                                                                      
                  //std::cout<<effi<<endl;                                                                                                                          
                  if (effi<0.7) bad_tracks.push_back(i); //bad efficiency    
                  // if (effi < 0.7 && effi2<0.7) def_bad_tracks.push_back(i);              
                  else good_run.push_back(i);
                  // h_num->SetBinContent(h_num->FindBin(i), num_entries);
                  // h_den->SetBinContent(h_den->FindBin(i), den_entries);

                }
              else no_tracks.push_back(i);
            }
          else no_pulse.push_back(i);
          f->Close();
        }
      else no_rootfile.push_back(i);
    }

  TCanvas *c1 = new TCanvas("c1", "c1",900,900);
  c1->Divide(2,3);
  c1->cd(1);
  h_num->Draw();
  c1->cd(2);
  h_den->Draw();
  c1->cd(3);
  TGraphAsymmErrors* h_eff1 = new TGraphAsymmErrors(); h_eff1->SetLineColor(kBlack);

  TGraphAsymmErrors* h_eff2 = new TGraphAsymmErrors(); h_eff2->SetLineColor(kRed);
  TGraphAsymmErrors* h_eff3 = new TGraphAsymmErrors(); h_eff3->SetLineColor(kGreen);
  TGraphAsymmErrors* h_eff4 = new TGraphAsymmErrors(); h_eff4->SetLineColor(kBlue);
  h_eff1->Divide(h_num1,h_den1,"cl=0.683 b(1,1) mode");
  h_eff2->Divide(h_num2,h_den2,"cl=0.683 b(1,1) mode");
  h_eff3->Divide(h_num3,h_den3,"cl=0.683 b(1,1) mode");
  h_eff4->Divide(h_num4,h_den4,"cl=0.683 b(1,1) mode");
  h_dummy->Draw();
  // h_eff1->Draw("EP");
  // h_eff2->Draw("EP same");
  // h_eff3->Draw("EP same");
  // h_eff4->Draw("EP same");

  TGraphAsymmErrors* h_start = new TGraphAsymmErrors(); h_start->SetLineColor(kBlack);
  h_start->Divide(h_num_start,h_den_start,"cl=0.683 b(1,1) mode");
  h_dummy->Draw();
  h_start->Draw("EP");

  c1->cd(4);
  TGraphAsymmErrors* h_eff = new TGraphAsymmErrors(); h_eff->SetLineColor(kBlack);
  h_eff->Divide(h_num,h_den,"cl=0.683 b(1,1) mode");

  TGraphAsymmErrors* h_fake = new TGraphAsymmErrors(); h_fake->SetLineColor(kRed);
  h_fake->Divide(h_numfake,h_denfake,"cl=0.683 b(1,1) mode");
 
  h_dummy->Draw();
  h_fake->Draw("EP");
  h_eff->Draw("EP same");
  

  c1->cd(5);
  TGraphAsymmErrors* h_end = new TGraphAsymmErrors(); h_end->SetLineColor(kBlack);
  h_end->Divide(h_num_end,h_den_end,"cl=0.683 b(1,1) mode");
  h_dummy->Draw();
  h_end->Draw("EP");

  c1->cd(6);
  //h_dummy->Draw();
  h_skipEventsD->SetLineColor(kRed);
  h_skipEventsD->SetFillColor(kRed);
  h_skipEventsD->SetFillStyle(1001.);
  h_skipEventsC->SetLineColor(kBlue);
  h_skipEventsC->SetFillColor(kBlue);
  h_skipEventsC->SetFillStyle(1001);
  THStack *skip = new THStack();
  skip->Add(h_skipEventsC);
  skip->Add(h_skipEventsD);
  skip->SetMaximum(3);skip->SetMinimum(-3);
  skip->Draw();



  TString filename = Form("plots/eff_run_run%i_to%i_ch%i.pdf",run_i,run_f,ch);
  c1->Print(filename);

  for (int i=0;i<no_rootfile.size();i++) std::cout<<"Missing root file "<<no_rootfile[i]<<endl;
  for (int i=0;i<no_pulse.size();i++) std::cout<<"Missing pulse "<<no_pulse[i]<<endl;
  for (int i=0;i<no_tracks.size();i++) std::cout<<"Missing tracks "<<no_tracks[i]<<endl;
  for (int i=0;i<bad_tracks.size();i++) std::cout<<"Bad tracks "<<bad_tracks[i]<<endl;
  for (int i=0;i<def_bad_tracks.size();i++) std::cout<<"Definitely bad tracks "<<def_bad_tracks[i]<<endl;
  for (int i=0;i<good_run.size();i++) std::cout<<good_run[i]<<endl;


}
 

