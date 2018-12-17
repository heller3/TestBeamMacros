#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <iomanip> // for setw()
#include <time.h>

#include "TSystem.h"
#include "TRandom.h"
#include "TROOT.h"
#include "TF1.h"
#include "TMath.h"
#include "TFile.h"
#include "TVirtualFFT.h"
#include "TTree.h"
#include "TChain.h"
#include "TMath.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TGraphErrors.h"
#include "TCanvas.h"
#include "TDirectory.h"
#include "TBranch.h"
#include "TString.h"
#include "TStyle.h"
#include "TPaveStats.h"
#include "TLatex.h"
#include "TSystemDirectory.h"
#include "TSystemFile.h"
#include "TGaxis.h"
#include "TLegend.h"
#include "TColor.h"

using namespace std;
void plot_eff(){

TChain * dt = new TChain("pulse");
TChain * vme = new TChain("pulse");

dt->Add("/Users/rheller/cernbox/ETL/tb/W6_2x8_bv360/DT*.root");
vme->Add("/Users/rheller/cernbox/ETL/tb/W6_2x8_bv360/DataVME*.root");

//plot efficiency map for 3 datasets
int nbinsX=400;
int nbinsY=100;
int minX=1;
int maxX=21;
int minY=16;
int maxY=26;

vector<TH2F *> num_dt;
vector<TH2F *> num_dt_day;
vector<TH2F *> num_dt_night;
vector<TH2F *> num_vme;
vector<TH2F *> den_dt;
vector<TH2F *> den_dt_day;
vector<TH2F *> den_dt_night;
vector<TH2F *> den_vme;
vector<TH2F *> eff_dt; 
vector<TH2F *> eff_dt_day;
vector<TH2F *> eff_dt_night;
vector<TH2F *> eff_vme;

vector<TH1F *> num_x_dt;
vector<TH1F *> num_x_dt_day;
vector<TH1F *> num_x_dt_night;
vector<TH1F *> num_x_vme;
vector<TH1F *> den_x_dt;
vector<TH1F *> den_x_dt_day;
vector<TH1F *> den_x_dt_night;
vector<TH1F *> den_x_vme;
vector<TH1F *> eff_x_dt; 
vector<TH1F *> eff_x_dt_day;
vector<TH1F *> eff_x_dt_night;
vector<TH1F *> eff_x_vme;


vector<TH2F *> num_gain_dt;
vector<TH2F *> num_gain_dt_day;
vector<TH2F *> num_gain_dt_night;
vector<TH2F *> num_gain_vme;
vector<TH2F *> den_gain_dt;
vector<TH2F *> den_gain_dt_day;
vector<TH2F *> den_gain_dt_night;
vector<TH2F *> den_gain_vme;
vector<TH2F *> eff_gain_dt; 
vector<TH2F *> eff_gain_dt_day;
vector<TH2F *> eff_gain_dt_night;
vector<TH2F *> eff_gain_vme;

vector<TH1F *> num_gain_x_dt;
vector<TH1F *> num_gain_x_dt_day;
vector<TH1F *> num_gain_x_dt_night;
vector<TH1F *> num_gain_x_vme;
vector<TH1F *> den_gain_x_dt;
vector<TH1F *> den_gain_x_dt_day;
vector<TH1F *> den_gain_x_dt_night;
vector<TH1F *> den_gain_x_vme;
vector<TH1F *> eff_gain_x_dt; 
vector<TH1F *> eff_gain_x_dt_day;
vector<TH1F *> eff_gain_x_dt_night;
vector<TH1F *> eff_gain_x_vme;

vector<TH2F *> num_deltaT_dt;
vector<TH2F *> num_deltaT_dt_day;
vector<TH2F *> num_deltaT_dt_night;
vector<TH2F *> num_deltaT_vme;
vector<TH2F *> den_deltaT_dt;
vector<TH2F *> den_deltaT_dt_day;
vector<TH2F *> den_deltaT_dt_night;
vector<TH2F *> den_deltaT_vme;
vector<TH2F *> eff_deltaT_dt; 
vector<TH2F *> eff_deltaT_dt_day;
vector<TH2F *> eff_deltaT_dt_night;
vector<TH2F *> eff_deltaT_vme;

vector<TH1F *> num_deltaT_x_dt;
vector<TH1F *> num_deltaT_x_dt_day;
vector<TH1F *> num_deltaT_x_dt_night;
vector<TH1F *> num_deltaT_x_vme;
vector<TH1F *> den_deltaT_x_dt;
vector<TH1F *> den_deltaT_x_dt_day;
vector<TH1F *> den_deltaT_x_dt_night;
vector<TH1F *> den_deltaT_x_vme;
vector<TH1F *> eff_deltaT_x_dt; 
vector<TH1F *> eff_deltaT_x_dt_day;
vector<TH1F *> eff_deltaT_x_dt_night;
vector<TH1F *> eff_deltaT_x_vme;

gStyle->SetOptStat(0);

int nchan=16;

TString yx = "y_corr:x_corr";

int nslices=2;
vector<TString> y_slice = {"(y_corr>19.2&&y_corr<20.6)","(y_corr>21.2&&y_corr<22.6)"};
float lboundx = 2.6;
float rboundx = 19.6;
float uboundy = 23;
float lboundy = 18.7;
int nsels=1;
vector<TString> den_sel = {/*"ntracks==1",*/"ntracks==1&&chi2<3"/*,Form("ntracks==1&&chi2<3&&x_corr>%f&&x_corr<%f&&y_corr>%f&&y_corr<%f",lboundx,rboundx,lboundy,uboundy)*/,"ntracks==1&&chi2<3&&ySlope>0.00015&&ySlope<0.00045&&xSlope>0.00005&&xSlope<0.0004"};

vector<TString> ptk_sel = {"((amp[4]>10&&amp[4]<50&&run<1766)||(amp[0]>70&&amp[0]<300&&run>=1766))","(amp[4]>10&&amp[4]<50&&run<1766)","(amp[0]>70&&amp[0]<300&&run>=1766)","(amp[27]>40&&amp[27]<120&&run<1766)"};
TString single_chan_sel = "(chidx[dummy]>=0 && (amp[chidx[dummy]]>15 && risetime[chidx[dummy]]>0.15))";



TString num_sel = "(";
for(int i=0;i<nchan;i++){
  TString buf(single_chan_sel);
  buf.ReplaceAll("dummy",Form("%i",i));
  if(i>0) num_sel+= " || ";
  num_sel+= buf;
}
num_sel+=")";
cout<<num_sel<<endl<<endl<<endl<<endl<<endl;

for(int i=0;i<nsels;i++){
  num_dt.push_back(new TH2F(Form("num_dt%i",i),Form("num DT%i",i),nbinsX,minX,maxX,nbinsY,minY,maxY));
  num_dt_day.push_back(new TH2F(Form("num_dt_day%i",i),Form("num DT_day%i",i),nbinsX,minX,maxX,nbinsY,minY,maxY));
  num_dt_night.push_back(new TH2F(Form("num_dt_night%i",i),Form("num DT_night%i",i),nbinsX,minX,maxX,nbinsY,minY,maxY));
  num_vme.push_back(new TH2F(Form("num_vme%i",i),Form("num vme%i",i),nbinsX,minX,maxX,nbinsY,minY,maxY));

  den_dt.push_back(new TH2F(Form("den_dt%i",i),Form("den DT%i",i),nbinsX,minX,maxX,nbinsY,minY,maxY));
  den_dt_day.push_back(new TH2F(Form("den_dt_day%i",i),Form("den DT_day%i",i),nbinsX,minX,maxX,nbinsY,minY,maxY));
  den_dt_night.push_back(new TH2F(Form("den_dt_night%i",i),Form("den DT_night%i",i),nbinsX,minX,maxX,nbinsY,minY,maxY));
  den_vme.push_back(new TH2F(Form("den_vme%i",i),Form("den vme%i",i),nbinsX,minX,maxX,nbinsY,minY,maxY));

  num_gain_dt.push_back(new TH2F(Form("num_gain_dt%i",i),Form("num_gain DT%i",i),nbinsX,minX,maxX,nbinsY,minY,maxY));
  num_gain_dt_day.push_back(new TH2F(Form("num_gain_dt_day%i",i),Form("num_gain DT_day%i",i),nbinsX,minX,maxX,nbinsY,minY,maxY));
  num_gain_dt_night.push_back(new TH2F(Form("num_gain_dt_night%i",i),Form("num_gain DT_night%i",i),nbinsX,minX,maxX,nbinsY,minY,maxY));
  num_gain_vme.push_back(new TH2F(Form("num_gain_vme%i",i),Form("num_gain vme%i",i),nbinsX,minX,maxX,nbinsY,minY,maxY));

  den_gain_dt.push_back(new TH2F(Form("den_gain_dt%i",i),Form("den_gain DT%i",i),nbinsX,minX,maxX,nbinsY,minY,maxY));
  den_gain_dt_day.push_back(new TH2F(Form("den_gain_dt_day%i",i),Form("den_gain DT_day%i",i),nbinsX,minX,maxX,nbinsY,minY,maxY));
  den_gain_dt_night.push_back(new TH2F(Form("den_gain_dt_night%i",i),Form("den_gain DT_night%i",i),nbinsX,minX,maxX,nbinsY,minY,maxY));
  den_gain_vme.push_back(new TH2F(Form("den_gain_vme%i",i),Form("den_gain vme%i",i),nbinsX,minX,maxX,nbinsY,minY,maxY));

  num_deltaT_dt.push_back(new TH2F(Form("num_deltaT_dt%i",i),Form("num_deltaT DT%i",i),nbinsX,minX,maxX,nbinsY,minY,maxY));
  num_deltaT_dt_day.push_back(new TH2F(Form("num_deltaT_dt_day%i",i),Form("num_deltaT DT_day%i",i),nbinsX,minX,maxX,nbinsY,minY,maxY));
  num_deltaT_dt_night.push_back(new TH2F(Form("num_deltaT_dt_night%i",i),Form("num_deltaT DT_night%i",i),nbinsX,minX,maxX,nbinsY,minY,maxY));
  num_deltaT_vme.push_back(new TH2F(Form("num_deltaT_vme%i",i),Form("num_deltaT vme%i",i),nbinsX,minX,maxX,nbinsY,minY,maxY));

  den_deltaT_dt.push_back(new TH2F(Form("den_deltaT_dt%i",i),Form("den_deltaT DT%i",i),nbinsX,minX,maxX,nbinsY,minY,maxY));
  den_deltaT_dt_day.push_back(new TH2F(Form("den_deltaT_dt_day%i",i),Form("den_deltaT DT_day%i",i),nbinsX,minX,maxX,nbinsY,minY,maxY));
  den_deltaT_dt_night.push_back(new TH2F(Form("den_deltaT_dt_night%i",i),Form("den_deltaT DT_night%i",i),nbinsX,minX,maxX,nbinsY,minY,maxY));
  den_deltaT_vme.push_back(new TH2F(Form("den_deltaT_vme%i",i),Form("den_deltaT vme%i",i),nbinsX,minX,maxX,nbinsY,minY,maxY));


  for(int j=0;j<nslices;j++){
    num_x_dt.push_back(new TH1F(Form("num_x_dt%i_slice%i",i,j),Form("num_x DT%i_slice%i",i,j),nbinsX,minX,maxX));
    num_x_dt_day.push_back(new TH1F(Form("num_x_dt_day%i_slice%i",i,j),Form("num_x DT_day%i_slice%i",i,j),nbinsX,minX,maxX));
    num_x_dt_night.push_back(new TH1F(Form("num_x_dt_night%i_slice%i",i,j),Form("num_x DT_night%i_slice%i",i,j),nbinsX,minX,maxX));
    num_x_vme.push_back(new TH1F(Form("num_x_vme%i_slice%i",i,j),Form("num_x vme%i_slice%i",i,j),nbinsX,minX,maxX));

    den_x_dt.push_back(new TH1F(Form("den_x_dt%i_slice%i",i,j),Form("den_x DT%i_slice%i",i,j),nbinsX,minX,maxX));
    den_x_dt_day.push_back(new TH1F(Form("den_x_dt_day%i_slice%i",i,j),Form("den_x DT_day%i_slice%i",i,j),nbinsX,minX,maxX));
    den_x_dt_night.push_back(new TH1F(Form("den_x_dt_night%i_slice%i",i,j),Form("den_x DT_night%i_slice%i",i,j),nbinsX,minX,maxX));
    den_x_vme.push_back(new TH1F(Form("den_x_vme%i_slice%i",i,j),Form("den_x vme%i_slice%i",i,j),nbinsX,minX,maxX));

    num_gain_x_dt.push_back(new TH1F(Form("num_gain_x_dt%i_slice%i",i,j),Form("num_gain_x DT%i_slice%i",i,j),nbinsX,minX,maxX));
    num_gain_x_dt_day.push_back(new TH1F(Form("num_gain_x_dt_day%i_slice%i",i,j),Form("num_gain_x DT_day%i_slice%i",i,j),nbinsX,minX,maxX));
    num_gain_x_dt_night.push_back(new TH1F(Form("num_gain_x_dt_night%i_slice%i",i,j),Form("num_gain_x DT_night%i_slice%i",i,j),nbinsX,minX,maxX));
    num_gain_x_vme.push_back(new TH1F(Form("num_gain_x_vme%i_slice%i",i,j),Form("num_gain_x vme%i_slice%i",i,j),nbinsX,minX,maxX));

    den_gain_x_dt.push_back(new TH1F(Form("den_gain_x_dt%i_slice%i",i,j),Form("den_gain_x DT%i_slice%i",i,j),nbinsX,minX,maxX));
    den_gain_x_dt_day.push_back(new TH1F(Form("den_gain_x_dt_day%i_slice%i",i,j),Form("den_gain_x DT_day%i_slice%i",i,j),nbinsX,minX,maxX));
    den_gain_x_dt_night.push_back(new TH1F(Form("den_gain_x_dt_night%i_slice%i",i,j),Form("den_gain_x DT_night%i_slice%i",i,j),nbinsX,minX,maxX));
    den_gain_x_vme.push_back(new TH1F(Form("den_gain_x_vme%i_slice%i",i,j),Form("den_gain_x vme%i_slice%i",i,j),nbinsX,minX,maxX));

    num_deltaT_x_dt.push_back(new TH1F(Form("num_deltaT_x_dt%i_slice%i",i,j),Form("num_deltaT_x DT%i_slice%i",i,j),nbinsX,minX,maxX));
    num_deltaT_x_dt_day.push_back(new TH1F(Form("num_deltaT_x_dt_day%i_slice%i",i,j),Form("num_deltaT_x DT_day%i_slice%i",i,j),nbinsX,minX,maxX));
    num_deltaT_x_dt_night.push_back(new TH1F(Form("num_deltaT_x_dt_night%i_slice%i",i,j),Form("num_deltaT_x DT_night%i_slice%i",i,j),nbinsX,minX,maxX));
    num_deltaT_x_vme.push_back(new TH1F(Form("num_deltaT_x_vme%i_slice%i",i,j),Form("num_deltaT_x vme%i_slice%i",i,j),nbinsX,minX,maxX));

    den_deltaT_x_dt.push_back(new TH1F(Form("den_deltaT_x_dt%i_slice%i",i,j),Form("den_deltaT_x DT%i_slice%i",i,j),nbinsX,minX,maxX));
    den_deltaT_x_dt_day.push_back(new TH1F(Form("den_deltaT_x_dt_day%i_slice%i",i,j),Form("den_deltaT_x DT_day%i_slice%i",i,j),nbinsX,minX,maxX));
    den_deltaT_x_dt_night.push_back(new TH1F(Form("den_deltaT_x_dt_night%i_slice%i",i,j),Form("den_deltaT_x DT_night%i_slice%i",i,j),nbinsX,minX,maxX));
    den_deltaT_x_vme.push_back(new TH1F(Form("den_deltaT_x_vme%i_slice%i",i,j),Form("den_deltaT_x vme%i_slice%i",i,j),nbinsX,minX,maxX));

  }
}

cout<<"Finished declaring histograms"<<endl;
for(int i=0;i<nsels;i++){
  cout<<den_sel[i]+"&&"+ptk_sel[0]<<endl<<endl;

  dt->Project(Form("den_dt%i",i),yx,den_sel[i]+"&&"+ptk_sel[0]);
  dt->Project(Form("den_dt_day%i",i),yx,den_sel[i]+"&&"+ptk_sel[1]);
  dt->Project(Form("den_dt_night%i",i),yx,den_sel[i]+"&&"+ptk_sel[2]);
  vme->Project(Form("den_vme%i",i),yx,den_sel[i]+"&&"+ptk_sel[3]);

  cout<<num_sel+"&&"+den_sel[i]+"&&"+ptk_sel[0]<<endl<<endl;

  dt->Project(Form("num_dt%i",i),yx,num_sel+"&&"+den_sel[i]+"&&"+ptk_sel[0]);
  dt->Project(Form("num_dt_day%i",i),yx,num_sel+"&&"+den_sel[i]+"&&"+ptk_sel[1]);
  dt->Project(Form("num_dt_night%i",i),yx,num_sel+"&&"+den_sel[i]+"&&"+ptk_sel[2]);
  vme->Project(Form("num_vme%i",i),yx,num_sel+"&&"+den_sel[i]+"&&"+ptk_sel[3]);

  for(int j=0;j<nslices;j++){
    int this_index = nslices*i + j;

    dt->Project(Form("den_x_dt%i_slice%i",i,j),"x_corr",den_sel[i]+"&&"+ptk_sel[0]+"&&"+y_slice[j]);
    dt->Project(Form("den_x_dt_day%i_slice%i",i,j),"x_corr",den_sel[i]+"&&"+ptk_sel[1]+"&&"+y_slice[j]);
    dt->Project(Form("den_x_dt_night%i_slice%i",i,j),"x_corr",den_sel[i]+"&&"+ptk_sel[2]+"&&"+y_slice[j]);
    vme->Project(Form("den_x_vme%i_slice%i",i,j),"x_corr",den_sel[i]+"&&"+ptk_sel[3]+"&&"+y_slice[j]);

    dt->Project(Form("num_x_dt%i_slice%i",i,j),"x_corr",num_sel+"&&"+den_sel[i]+"&&"+ptk_sel[0]+"&&"+y_slice[j]);
    dt->Project(Form("num_x_dt_day%i_slice%i",i,j),"x_corr",num_sel+"&&"+den_sel[i]+"&&"+ptk_sel[1]+"&&"+y_slice[j]);
    dt->Project(Form("num_x_dt_night%i_slice%i",i,j),"x_corr",num_sel+"&&"+den_sel[i]+"&&"+ptk_sel[2]+"&&"+y_slice[j]);
    vme->Project(Form("num_x_vme%i_slice%i",i,j),"x_corr",num_sel+"&&"+den_sel[i]+"&&"+ptk_sel[3]+"&&"+y_slice[j]);


    cout<<"Numerator is "<<num_sel+"&&"+den_sel[i]+"&&"+ptk_sel[2]+"&&"+y_slice[j]<<endl<<endl;
    cout<<"Denominator is "<< den_sel[i]+"&&"+ptk_sel[2]+"&&"+y_slice[j]<<endl<<endl;

    eff_x_dt.push_back((TH1F*)num_x_dt[this_index]->Clone());
    eff_x_dt_day.push_back((TH1F*)num_x_dt_day[this_index]->Clone());
    eff_x_dt_night.push_back((TH1F*)num_x_dt_night[this_index]->Clone());
    eff_x_vme.push_back((TH1F*)num_x_vme[this_index]->Clone());

    eff_x_dt[this_index]->Divide(den_x_dt[this_index]);
    eff_x_dt_day[this_index]->Divide(den_x_dt_day[this_index]);
    eff_x_dt_night[this_index]->Divide(den_x_dt_night[this_index]);
    eff_x_vme[this_index]->Divide(den_x_vme[this_index]);

    float start=5.3; float end= 6.5;
    float avg_eff_pad_10 = num_x_dt_night[this_index]->Integral(num_x_dt_night[this_index]->FindBin(start),num_x_dt_night[this_index]->FindBin(end));
    avg_eff_pad_10 /= den_x_dt_night[this_index]->Integral(den_x_dt_night[this_index]->FindBin(start),den_x_dt_night[this_index]->FindBin(end));
    
    start= 11.3; end = 12.5;
    float avg_eff_pad_4_vme= num_x_vme[this_index]->Integral(num_x_vme[this_index]->FindBin(start),num_x_vme[this_index]->FindBin(end));
    avg_eff_pad_4_vme /= den_x_vme[this_index]->Integral(den_x_vme[this_index]->FindBin(start),den_x_vme[this_index]->FindBin(end));
    
    float avg_eff_pad_12_dt_day = num_x_dt_day[this_index]->Integral(num_x_dt_day[this_index]->FindBin(start),num_x_dt_day[this_index]->FindBin(end));
    avg_eff_pad_12_dt_day /= den_x_dt_day[this_index]->Integral(den_x_dt_day[this_index]->FindBin(start),den_x_dt_day[this_index]->FindBin(end));

    cout<<"Selection "<<i<<", slice "<<j<<endl<<"Efficiency pad 10: "<<avg_eff_pad_10<<", 4: "<<avg_eff_pad_4_vme<<", 12: "<<avg_eff_pad_12_dt_day<<endl;
    // new TCanvas();
    // num_x_dt_night[this_index]->Draw("hist");
    // new TCanvas();
    // den_x_dt_night[this_index]->Draw("hist");


    // new TCanvas();
    // eff_x_dt[this_index]->Draw("hist");
    // new TCanvas();
    // eff_x_dt_day[this_index]->Draw("hist");
    new TCanvas();
    eff_x_dt_night[this_index]->Draw("hist");
    // new TCanvas();
    // eff_x_vme[this_index]->Draw("hist");

  }


  eff_dt.push_back((TH2F*)num_dt[i]->Clone());
  eff_dt_day.push_back((TH2F*)num_dt_day[i]->Clone());
  eff_dt_night.push_back((TH2F*)num_dt_night[i]->Clone());
  eff_vme.push_back((TH2F*)num_vme[i]->Clone());

  eff_dt[i]->Divide(den_dt[i]);
  eff_dt_day[i]->Divide(den_dt_day[i]);
  eff_dt_night[i]->Divide(den_dt_night[i]);
  eff_vme[i]->Divide(den_vme[i]);


  //Define pad selections in xy
  //Plot amplitude, delta T for each one

  //Plot map of average gain (fill num with weight = amp, den has risetime sel)
  //Plot map of average delta T (fill num with weight = delta T, den has risetime sel and amp cut)

  // new TCanvas();
  // den_dt[i]->Draw("colz");
  // new TCanvas();
  // den_dt_day[i]->Draw("colz");
  // new TCanvas();
  // den_dt_night[i]->Draw("colz");
  // new TCanvas();
  // den_vme[i]->Draw("colz");

  // new TCanvas();
  // num_dt[i]->Draw("colz");
  // 
  // new TCanvas();
  // num_dt_night[i]->Draw("colz");
  // new TCanvas();
  // num_vme[i]->Draw("colz");

  // new TCanvas();
  // eff_dt[i]->Draw("colz");
  new TCanvas(Form("day%i",i),"",958,510);
  eff_dt_day[i]->Draw("colz");
  new TCanvas(Form("night%i",i),"",958,510);
  eff_dt_night[i]->Draw("colz");
  new TCanvas(Form("vme%i",i),"",958,510);;
  eff_vme[i]->Draw("colz");

  num_dt[i]->Add(num_vme[i]);
  new TCanvas(Form("total%i",i),"",958,510);
  num_dt[i]->Draw("colz");

}





/*

// nums.push_back(new TH2F(Form("num%i",it),Form("by hand theta = %f",180.*thetas[it]/3.14159),200,10,30,100,20,30));
// dens.push_back(new TH2F(Form("den%i",it),Form("theta = %f",thetas[it]),200,10,30,100,20,30));

// vector<TH2F*> nums;
// vector<TH2F*> dens;
// vector<TH2F*> effs2;

//TH3F * holder = new TH3F("holder","",100,-4,16,200,23,33,10,0,2);
 TH3F * holder = new TH3F("holder","",100,0,20,200,15,25,10,0,2);
 TH3F * holder_eps = new TH3F("holder_eps","",100,0,20,200,15,25,10,0,2);
 float th=15;
 chain->Project("holder",Form("((chidx[0]>=0&&(amp[chidx[0]]>%f && risetime[chidx[0]]>0.1))||(chidx[1]>=0&&(amp[chidx[1]]>%f && risetime[chidx[1]]>0.1))||(chidx[2]>=0&&(amp[chidx[2]]>%f && risetime[chidx[2]]>0.1))||(chidx[3]>=0&&(amp[chidx[3]]>%f && risetime[chidx[3]]>0.1))||(chidx[4]>=0&&(amp[chidx[4]]>%f && risetime[chidx[4]]>0.1))||(chidx[5]>=0&&(amp[chidx[5]]>%f && risetime[chidx[5]]>0.1))||(chidx[6]>=0&&(amp[chidx[6]]>%f && risetime[chidx[6]]>0.1))||(chidx[7]>=0&&(amp[chidx[7]]>%f && risetime[chidx[7]]>0.1))||(chidx[8]>=0&&(amp[chidx[8]]>%f && risetime[chidx[8]]>0.1))||(chidx[9]>=0&&(amp[chidx[9]]>%f && risetime[chidx[9]]>0.1))||(chidx[10]>=0&&(amp[chidx[10]]>%f && risetime[chidx[10]]>0.1))||(chidx[11]>=0&&(amp[chidx[11]]>%f && risetime[chidx[11]]>0.1))||(chidx[12]>=0&&(amp[chidx[12]]>%f && risetime[chidx[12]]>0.1))||(chidx[13]>=0&&(amp[chidx[13]]>%f && risetime[chidx[13]]>0.1))||(chidx[14]>=0&&(amp[chidx[14]]>%f && risetime[chidx[14]]>0.1))||(chidx[15]>=0&&(amp[chidx[15]]>%f && risetime[chidx[15]]>0.1))):y_corr:x_corr",th,th,th,th,th,th,th,th,th,th,th,th,th,th,th,th),"ntracks==1&&amp[0]>70&&amp[0]<300");

 float theta=0.2 * 3.14159/180;
 vector<TH3F*> holders;
 vector<TH2F*> nums;
 vector<TH2F*> dens;
 vector<TH2F*> effs2;
 vector<TProfile2D*> effs;
 vector<float> thetas = {0.,1.95 * 3.14159/180,2.07 * 3.14159/180,2.15 * 3.14159/180};
 for(int it=0;it<thetas.size();it++){
   holders.push_back(new TH3F(Form("holder%i",it),Form("theta = %f",thetas[it]),200,10,30,100,20,30,10,0,2));
   nums.push_back(new TH2F(Form("num%i",it),Form("by hand theta = %f",180.*thetas[it]/3.14159),200,10,30,100,20,30));
   dens.push_back(new TH2F(Form("den%i",it),Form("theta = %f",thetas[it]),200,10,30,100,20,30));

   chain->Project(Form("holder%i",it),Form("((chidx[0]>=0&&(amp[chidx[0]]>%f && risetime[chidx[0]]>0.1))||(chidx[1]>=0&&(amp[chidx[1]]>%f && risetime[chidx[1]]>0.1))||(chidx[2]>=0&&(amp[chidx[2]]>%f && risetime[chidx[2]]>0.1))||(chidx[3]>=0&&(amp[chidx[3]]>%f && risetime[chidx[3]]>0.1))||(chidx[4]>=0&&(amp[chidx[4]]>%f && risetime[chidx[4]]>0.1))||(chidx[5]>=0&&(amp[chidx[5]]>%f && risetime[chidx[5]]>0.1))||(chidx[6]>=0&&(amp[chidx[6]]>%f && risetime[chidx[6]]>0.1))||(chidx[7]>=0&&(amp[chidx[7]]>%f && risetime[chidx[7]]>0.1))||(chidx[8]>=0&&(amp[chidx[8]]>%f && risetime[chidx[8]]>0.1))||(chidx[9]>=0&&(amp[chidx[9]]>%f && risetime[chidx[9]]>0.1))||(chidx[10]>=0&&(amp[chidx[10]]>%f && risetime[chidx[10]]>0.1))||(chidx[11]>=0&&(amp[chidx[11]]>%f && risetime[chidx[11]]>0.1))||(chidx[12]>=0&&(amp[chidx[12]]>%f && risetime[chidx[12]]>0.1))||(chidx[13]>=0&&(amp[chidx[13]]>%f && risetime[chidx[13]]>0.1))||(chidx[14]>=0&&(amp[chidx[14]]>%f && risetime[chidx[14]]>0.1))||(chidx[15]>=0&&(amp[chidx[15]]>%f && risetime[chidx[15]]>0.1))):-x_dut[2]*sin(%f) + y_dut[2]*cos(%f):x_dut[2]*cos(%f) + y_dut[2]*sin(%f)",th,th,th,th,th,th,th,th,th,th,th,th,th,th,th,th,thetas[it],thetas[it],thetas[it],thetas[it]),"ntracks==1&&amp[0]>70&&amp[0]<300");
   effs.push_back(holders[it]->Project3DProfile("yx"));
   new TCanvas();
   effs[it]->Draw("colz");

   chain->Project(Form("den%i",it),Form("-x_dut[2]*sin(%f) + y_dut[2]*cos(%f):x_dut[2]*cos(%f) + y_dut[2]*sin(%f)",thetas[it],thetas[it],thetas[it],thetas[it]),"ntracks==1&&amp[0]>70&&amp[0]<300");

   chain->Project(Form("num%i",it),Form("-x_dut[2]*sin(%f) + y_dut[2]*cos(%f):x_dut[2]*cos(%f) + y_dut[2]*sin(%f)",thetas[it],thetas[it],thetas[it],thetas[it]),Form("ntracks==1&&amp[0]>70&&amp[0]<300 && ((chidx[0]>=0&&(amp[chidx[0]]>%f && risetime[chidx[0]]>0.1))||(chidx[1]>=0&&(amp[chidx[1]]>%f && risetime[chidx[1]]>0.1))||(chidx[2]>=0&&(amp[chidx[2]]>%f && risetime[chidx[2]]>0.1))||(chidx[3]>=0&&(amp[chidx[3]]>%f && risetime[chidx[3]]>0.1))||(chidx[4]>=0&&(amp[chidx[4]]>%f && risetime[chidx[4]]>0.1))||(chidx[5]>=0&&(amp[chidx[5]]>%f && risetime[chidx[5]]>0.1))||(chidx[6]>=0&&(amp[chidx[6]]>%f && risetime[chidx[6]]>0.1))||(chidx[7]>=0&&(amp[chidx[7]]>%f && risetime[chidx[7]]>0.1))||(chidx[8]>=0&&(amp[chidx[8]]>%f && risetime[chidx[8]]>0.1))||(chidx[9]>=0&&(amp[chidx[9]]>%f && risetime[chidx[9]]>0.1))||(chidx[10]>=0&&(amp[chidx[10]]>%f && risetime[chidx[10]]>0.1))||(chidx[11]>=0&&(amp[chidx[11]]>%f && risetime[chidx[11]]>0.1))||(chidx[12]>=0&&(amp[chidx[12]]>%f && risetime[chidx[12]]>0.1))||(chidx[13]>=0&&(amp[chidx[13]]>%f && risetime[chidx[13]]>0.1))||(chidx[14]>=0&&(amp[chidx[14]]>%f && risetime[chidx[14]]>0.1))||(chidx[15]>=0&&(amp[chidx[15]]>%f && risetime[chidx[15]]>0.1)))",th,th,th,th,th,th,th,th,th,th,th,th,th,th,th,th));

   chain->Project(Form("holder%i",it),Form("((chidx[0]>=0&&(amp[chidx[0]]>%f && risetime[chidx[0]]>0.1))||(chidx[1]>=0&&(amp[chidx[1]]>%f && risetime[chidx[1]]>0.1))||(chidx[2]>=0&&(amp[chidx[2]]>%f && risetime[chidx[2]]>0.1))||(chidx[3]>=0&&(amp[chidx[3]]>%f && risetime[chidx[3]]>0.1))||(chidx[4]>=0&&(amp[chidx[4]]>%f && risetime[chidx[4]]>0.1))||(chidx[5]>=0&&(amp[chidx[5]]>%f && risetime[chidx[5]]>0.1))||(chidx[6]>=0&&(amp[chidx[6]]>%f && risetime[chidx[6]]>0.1))||(chidx[7]>=0&&(amp[chidx[7]]>%f && risetime[chidx[7]]>0.1))||(chidx[8]>=0&&(amp[chidx[8]]>%f && risetime[chidx[8]]>0.1))||(chidx[9]>=0&&(amp[chidx[9]]>%f && risetime[chidx[9]]>0.1))||(chidx[10]>=0&&(amp[chidx[10]]>%f && risetime[chidx[10]]>0.1))||(chidx[11]>=0&&(amp[chidx[11]]>%f && risetime[chidx[11]]>0.1))||(chidx[12]>=0&&(amp[chidx[12]]>%f && risetime[chidx[12]]>0.1))||(chidx[13]>=0&&(amp[chidx[13]]>%f && risetime[chidx[13]]>0.1))||(chidx[14]>=0&&(amp[chidx[14]]>%f && risetime[chidx[14]]>0.1))||(chidx[15]>=0&&(amp[chidx[15]]>%f && risetime[chidx[15]]>0.1))):-x_dut[2]*sin(%f) + y_dut[2]*cos(%f):x_dut[2]*cos(%f) + y_dut[2]*sin(%f)",th,th,th,th,th,th,th,th,th,th,th,th,th,th,th,th,thetas[it],thetas[it],thetas[it],thetas[it]),"ntracks==1&&amp[0]>70&&amp[0]<300");

   effs2.push_back((TH2F*)nums[it]->Clone());
   effs2[it]->Divide(dens[it]);

   new TCanvas();
   effs2[it]->Draw("colz");



 }
 //chain->Draw("((chidx[0]>=0&&amp[chidx[0]]>15)||(chidx[1]>=0&&amp[chidx[1]]>15)||(chidx[2]>=0&&amp[chidx[2]]>15)||(chidx[3]>=0&&amp[chidx[3]]>15)||(chidx[4]>=0&&amp[chidx[4]]>15)||(chidx[5]>=0&&amp[chidx[5]]>15)||(chidx[6]>=0&&amp[chidx[6]]>15)||(chidx[7]>=0&&amp[chidx[7]]>15)||(chidx[8]>=0&&amp[chidx[8]]>15)||(chidx[9]>=0&&amp[chidx[9]]>15)||(chidx[10]>=0&&amp[chidx[10]]>15)||(chidx[11]>=0&&amp[chidx[11]]>14)||(chidx[12]>=0&&amp[chidx[12]]>15)||(chidx[13]>=0&&amp[chidx[13]]>15)||(chidx[14]>=0&&amp[chidx[14]]>15)||(chidx[15]>=0&&amp[chidx[15]]>15)):y_dut[2]-22+y0:x_dut[2]-44+x0>>hp4(200,-4,16,10,23,33,10,0,2)","ntracks==1&&amp[0]>70&&amp[0]<300","profcolz");

vector<TCanvas*> cans;

TProfile2D * eff = holder->Project3DProfile("yx");//(TProfile2D*) gDirectory->Get("hp4");
TProfile2D * eff2 = holder_eps->Project3DProfile("yx");//(TProfile2D*) gDirectory->Get("hp4");

TCanvas *c1 = new TCanvas;
eff->Draw("colz");
TAxis *xaxis = eff->GetXaxis();
 new TCanvas;
 eff2->Draw("colz");
vector<TH1D*> slices;
TH1F * valley = new TH1F("valley","y-position of valley",100,-4,16);
 for(int i=0;i<xvals.size();i++){
  float this_x= xvals[i];//-1.7+0.11*i;
  int this_x_bin = xaxis->FindBin(this_x);
  cout<<this_x<<" "<<this_x_bin<<endl;
  slices.push_back(eff->ProjectionY(Form("py%i",i),this_x_bin,this_x_bin));
  slices[i]->SetTitle(Form("x = %f",this_x));
  cans.push_back(new TCanvas());
  slices[i]->Draw("hist");
  slices[i]->GetXaxis()->SetRangeUser(20,22);
  float this_y_min = slices[i]->GetBinCenter(slices[i]->GetMinimumBin());
  valley->SetBinContent(this_x_bin,this_y_min);
}
new TCanvas;
valley->SetMarkerStyle(20);
valley->Draw("p");

 TF1 * fitline =  new TF1("fitline","pol1");
 valley->Fit(fitline);
 float slope = fitline->GetParameter(1);
 float intercept = fitline->GetParameter(0);
 cout<<slope<<" "<<intercept<<endl;
 */
 /* TLine line = TLine();
 line.SetLineColor(kRed);
 line.SetLineWidth(4);
 c1->cd();
 line.DrawLine(-3.,intercept+ (-3.*slope), 12., intercept + 12.*slope);
 line.SetLineColor(kBlack);
 line.SetLineWidth(2);
 for(int i=0;i<xvals.size();i++){
   line.DrawLine(xvals[i],20,xvals[i],22);
 }
 */




}

