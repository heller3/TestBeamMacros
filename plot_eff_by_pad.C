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


#ifdef __MAKECINT__
#pragma link C++ class std::vector < std::vector<int> >+;
#pragma link C++ class std::vector < std::vector<float> >+;
#pragma link C++ class std::vector < std::vector<TH1F*> >+;
#endif


using namespace std;
void plot_eff_by_pad(){

TChain * dt = new TChain("pulse");
TChain * vme = new TChain("pulse");

dt->Add("/Users/rheller/cernbox/ETL/tb/W6_2x8_bv360/DT5742_Run176[6-9]*360.root");
dt->Add("/Users/rheller/cernbox/ETL/tb/W6_2x8_bv360/DT5742_Run17[7-9]*360.root");
//vme->Add("/Users/heller/ETL/tb/W6_2x8_bv360/DataVME*.root");

bool DT_night_only =true;

//plot efficiency map for 3 datasets
int nbinsX=400;
int nbinsY=100;
int minX=1;
int maxX=21;
int minY=17;
int maxY=27;

vector< vector<TH2F *> > num_dt;
vector< vector<TH2F *> > num_dt_day;
vector< vector<TH2F *> > num_dt_night;
vector< vector<TH2F *> > num_vme;
vector< vector<TH2F *> > den_dt;
vector< vector<TH2F *> > den_dt_day;
vector< vector<TH2F *> > den_dt_night;
vector< vector<TH2F *> > den_vme;
vector< vector<TH2F *> > eff_dt; 
vector< vector<TH2F *> > eff_dt_day;
vector< vector<TH2F *> > eff_dt_night;
vector< vector<TH2F *> > eff_vme;

vector< vector<TH1F *> > num_x_dt;
vector< vector<TH1F *> > num_x_dt_day;
vector< vector<TH1F *> > num_x_dt_night;
vector< vector<TH1F *> > num_x_vme;
vector< vector<TH1F *> > den_x_dt;
vector< vector<TH1F *> > den_x_dt_day;
vector< vector<TH1F *> > den_x_dt_night;
vector< vector<TH1F *> > den_x_vme;
vector< vector<TH1F *> > eff_x_dt; 
vector< vector<TH1F *> > eff_x_dt_day;
vector< vector<TH1F *> > eff_x_dt_night;
vector< vector<TH1F *> > eff_x_vme;


vector< vector<TH2F *> > num_gain_dt;
vector< vector<TH2F *> > num_gain_dt_day;
vector< vector<TH2F *> > num_gain_dt_night;
vector< vector<TH2F *> > num_gain_vme;
vector< vector<TH2F *> > den_gain_dt;
vector< vector<TH2F *> > den_gain_dt_day;
vector< vector<TH2F *> > den_gain_dt_night;
vector< vector<TH2F *> > den_gain_vme;
vector< vector<TH2F *> > eff_gain_dt; 
vector< vector<TH2F *> > eff_gain_dt_day;
vector< vector<TH2F *> > eff_gain_dt_night;
vector< vector<TH2F *> > eff_gain_vme;

vector< vector<TH1F *> > num_gain_x_dt;
vector< vector<TH1F *> > num_gain_x_dt_day;
vector< vector<TH1F *> > num_gain_x_dt_night;
vector< vector<TH1F *> > num_gain_x_vme;
vector< vector<TH1F *> > den_gain_x_dt;
vector< vector<TH1F *> > den_gain_x_dt_day;
vector< vector<TH1F *> > den_gain_x_dt_night;
vector< vector<TH1F *> > den_gain_x_vme;
vector< vector<TH1F *> > eff_gain_x_dt; 
vector< vector<TH1F *> > eff_gain_x_dt_day;
vector< vector<TH1F *> > eff_gain_x_dt_night;
vector< vector<TH1F *> > eff_gain_x_vme;

vector< vector<TH2F *> > num_deltaT_dt;
vector< vector<TH2F *> > num_deltaT_dt_day;
vector< vector<TH2F *> > num_deltaT_dt_night;
vector< vector<TH2F *> > num_deltaT_vme;
vector< vector<TH2F *> > den_deltaT_dt;
vector< vector<TH2F *> > den_deltaT_dt_day;
vector< vector<TH2F *> > den_deltaT_dt_night;
vector< vector<TH2F *> > den_deltaT_vme;
vector< vector<TH2F *> > eff_deltaT_dt; 
vector< vector<TH2F *> > eff_deltaT_dt_day;
vector< vector<TH2F *> > eff_deltaT_dt_night;
vector< vector<TH2F *> > eff_deltaT_vme;

vector< vector<TH1F *> > num_deltaT_x_dt;
vector< vector<TH1F *> > num_deltaT_x_dt_day;
vector< vector<TH1F *> > num_deltaT_x_dt_night;
vector< vector<TH1F *> > num_deltaT_x_vme;
vector< vector<TH1F *> > den_deltaT_x_dt;
vector< vector<TH1F *> > den_deltaT_x_dt_day;
vector< vector<TH1F *> > den_deltaT_x_dt_night;
vector< vector<TH1F *> > den_deltaT_x_vme;
vector< vector<TH1F *> > eff_deltaT_x_dt; 
vector< vector<TH1F *> > eff_deltaT_x_dt_day;
vector< vector<TH1F *> > eff_deltaT_x_dt_night;
vector< vector<TH1F *> > eff_deltaT_x_vme;


int nchan=16;

TString yx = "y_corr:x_corr";

int nslices=2;
vector<TString> y_slice = {"(y_corr>19.4&&y_corr<20.6)","(y_corr>19.7&&y_corr<20.3)"};

//int nsels=3;
//vector<TString> den_sel = {"ntracks==1","ntracks==1&&chi2<3","ntracks==1&&chi2<3&&ySlope>0.00015&&ySlope<0.00045&&xSlope>0.00005&&xSlope<0.0004"};
int nsels=1;
vector<TString> den_sel = {"ntracks==1&&chi2<3"};


vector<TString> ptk_sel = {"((amp[4]>10&&amp[4]<50&&run<1766)||(amp[0]>70&&amp[0]<300&&run>=1766))","(amp[4]>10&&amp[4]<50&&run<1766)","(amp[0]>70&&amp[0]<300&&run>=1766)","(amp[27]>40&&amp[27]<120&&run<1766)"};
TString single_chan_sel = "(chidx[dummy]>=0 && (amp[chidx[dummy]]>15 && risetime[chidx[dummy]]>0.15))";

TString single_chan_gain_den = "(chidx[dummy]>=0 && risetime[chidx[dummy]]>0.15)";
TString single_chan_gain_weight = "(chidx[dummy]>=0)*amp[chidx[dummy]]";
TString single_chan_deltaT_weight = "(chidx[dummy]>=0)*(LP2_20[chidx[dummy]] - LP2_20[ptkidx[dummy]])";

//Plot map of average gain (fill num with weight = amp, den has risetime sel)
//Plot map of average delta T (fill num with weight = delta T, den has risetime sel and amp cut)
//  TString gain_den = ""
//TString weight_gain = "";

//Make OR selection
TString num_sel = "(";
for(int i=0;i<nchan;i++){
  TString buf(single_chan_sel);
  buf.ReplaceAll("dummy",Form("%i",i));
  if(i>0) num_sel+= " || ";
  num_sel+= buf;
}
num_sel+=")";

//Make single channel selection. (0th element is OR)
vector<TString> indiv_chan_sel;
vector<TString> indiv_chan_gain_den;
vector<TString> indiv_chan_gain_weight;
vector<TString> indiv_chan_deltaT_weight;

indiv_chan_sel.push_back(num_sel);
indiv_chan_gain_den.push_back("0");
indiv_chan_gain_weight.push_back("0");
indiv_chan_deltaT_weight.push_back("0");
for(int i=0;i<nchan;i++){
  TString buf(single_chan_sel);
  TString buf_den(single_chan_gain_den);
  TString buf_gain_weight(single_chan_gain_weight);
  TString buf_deltaT_weight(single_chan_deltaT_weight);
  buf.ReplaceAll("dummy",Form("%i",i));
  buf_den.ReplaceAll("dummy",Form("%i",i));
  buf_gain_weight.ReplaceAll("dummy",Form("%i",i));
  buf_deltaT_weight.ReplaceAll("dummy",Form("%i",i));
  indiv_chan_sel.push_back(buf);
  indiv_chan_gain_den.push_back(buf_den);
  indiv_chan_gain_weight.push_back(buf_gain_weight);
  indiv_chan_deltaT_weight.push_back(buf_deltaT_weight);
}
//cout<<num_sel<<endl<<endl<<endl<<endl<<endl;

for(int i=0;i<nsels;i++){
  vector<TH2F*> this_num_dt, this_num_dt_day,this_num_dt_night,this_num_vme;
  vector<TH2F*> this_den_dt, this_den_dt_day,this_den_dt_night,this_den_vme;
  vector<TH2F*> this_num_gain_dt, this_num_gain_dt_day,this_num_gain_dt_night,this_num_gain_vme;
  vector<TH2F*> this_den_gain_dt, this_den_gain_dt_day,this_den_gain_dt_night,this_den_gain_vme;
  vector<TH2F*> this_num_deltaT_dt, this_num_deltaT_dt_day,this_num_deltaT_dt_night,this_num_deltaT_vme;
  vector<TH2F*> this_den_deltaT_dt, this_den_deltaT_dt_day,this_den_deltaT_dt_night,this_den_deltaT_vme;

  vector<TH1F*> this_num_x_dt, this_num_x_dt_day,this_num_x_dt_night,this_num_x_vme;
  vector<TH1F*> this_den_x_dt, this_den_x_dt_day,this_den_x_dt_night,this_den_x_vme;
  vector<TH1F*> this_num_gain_x_dt, this_num_gain_x_dt_day,this_num_gain_x_dt_night,this_num_gain_x_vme;
  vector<TH1F*> this_den_gain_x_dt, this_den_gain_x_dt_day,this_den_gain_x_dt_night,this_den_gain_x_vme;
  vector<TH1F*> this_num_deltaT_x_dt, this_num_deltaT_x_dt_day,this_num_deltaT_x_dt_night,this_num_deltaT_x_vme;
  vector<TH1F*> this_den_deltaT_x_dt, this_den_deltaT_x_dt_day,this_den_deltaT_x_dt_night,this_den_deltaT_x_vme;



  for(int ch=0;ch<nchan+1;ch++){
  this_num_dt.push_back(new TH2F(Form("num_dt%i_pad%i",i,ch),Form("num DT%i_pad%i",i,ch),nbinsX,minX,maxX,nbinsY,minY,maxY));
  this_num_dt_day.push_back(new TH2F(Form("num_dt_day%i_pad%i",i,ch),Form("num DT_day%i_pad%i",i,ch),nbinsX,minX,maxX,nbinsY,minY,maxY));
  this_num_dt_night.push_back(new TH2F(Form("num_dt_night%i_pad%i",i,ch),Form("num DT_night%i_pad%i",i,ch),nbinsX,minX,maxX,nbinsY,minY,maxY));
  this_num_vme.push_back(new TH2F(Form("num_vme%i_pad%i",i,ch),Form("num vme%i_pad%i",i,ch),nbinsX,minX,maxX,nbinsY,minY,maxY));

  this_den_dt.push_back(new TH2F(Form("den_dt%i_pad%i",i,ch),Form("den DT%i_pad%i",i,ch),nbinsX,minX,maxX,nbinsY,minY,maxY));
  this_den_dt_day.push_back(new TH2F(Form("den_dt_day%i_pad%i",i,ch),Form("den DT_day%i_pad%i",i,ch),nbinsX,minX,maxX,nbinsY,minY,maxY));
  this_den_dt_night.push_back(new TH2F(Form("den_dt_night%i_pad%i",i,ch),Form("den DT_night%i_pad%i",i,ch),nbinsX,minX,maxX,nbinsY,minY,maxY));
  this_den_vme.push_back(new TH2F(Form("den_vme%i_pad%i",i,ch),Form("den vme%i_pad%i",i,ch),nbinsX,minX,maxX,nbinsY,minY,maxY));

  this_num_gain_dt.push_back(new TH2F(Form("num_gain_dt%i_pad%i",i,ch),Form("num_gain DT%i_pad%i",i,ch),nbinsX,minX,maxX,nbinsY,minY,maxY));
  this_num_gain_dt_day.push_back(new TH2F(Form("num_gain_dt_day%i_pad%i",i,ch),Form("num_gain DT_day%i_pad%i",i,ch),nbinsX,minX,maxX,nbinsY,minY,maxY));
  this_num_gain_dt_night.push_back(new TH2F(Form("num_gain_dt_night%i_pad%i",i,ch),Form("num_gain DT_night%i_pad%i",i,ch),nbinsX,minX,maxX,nbinsY,minY,maxY));
  this_num_gain_vme.push_back(new TH2F(Form("num_gain_vme%i_pad%i",i,ch),Form("num_gain vme%i_pad%i",i,ch),nbinsX,minX,maxX,nbinsY,minY,maxY));

  this_den_gain_dt.push_back(new TH2F(Form("den_gain_dt%i_pad%i",i,ch),Form("den_gain DT%i_pad%i",i,ch),nbinsX,minX,maxX,nbinsY,minY,maxY));
  this_den_gain_dt_day.push_back(new TH2F(Form("den_gain_dt_day%i_pad%i",i,ch),Form("den_gain DT_day%i_pad%i",i,ch),nbinsX,minX,maxX,nbinsY,minY,maxY));
  this_den_gain_dt_night.push_back(new TH2F(Form("den_gain_dt_night%i_pad%i",i,ch),Form("den_gain DT_night%i_pad%i",i,ch),nbinsX,minX,maxX,nbinsY,minY,maxY));
  this_den_gain_vme.push_back(new TH2F(Form("den_gain_vme%i_pad%i",i,ch),Form("den_gain vme%i_pad%i",i,ch),nbinsX,minX,maxX,nbinsY,minY,maxY));
 
  this_num_deltaT_dt.push_back(new TH2F(Form("num_deltaT_dt%i_pad%i",i,ch),Form("num_deltaT DT%i_pad%i",i,ch),nbinsX,minX,maxX,nbinsY,minY,maxY));
  this_num_deltaT_dt_day.push_back(new TH2F(Form("num_deltaT_dt_day%i_pad%i",i,ch),Form("num_deltaT DT_day%i_pad%i",i,ch),nbinsX,minX,maxX,nbinsY,minY,maxY));
  this_num_deltaT_dt_night.push_back(new TH2F(Form("num_deltaT_dt_night%i_pad%i",i,ch),Form("num_deltaT DT_night%i_pad%i",i,ch),nbinsX,minX,maxX,nbinsY,minY,maxY));
  this_num_deltaT_vme.push_back(new TH2F(Form("num_deltaT_vme%i_pad%i",i,ch),Form("num_deltaT vme%i_pad%i",i,ch),nbinsX,minX,maxX,nbinsY,minY,maxY));

  this_den_deltaT_dt.push_back(new TH2F(Form("den_deltaT_dt%i_pad%i",i,ch),Form("den_deltaT DT%i_pad%i",i,ch),nbinsX,minX,maxX,nbinsY,minY,maxY));
  this_den_deltaT_dt_day.push_back(new TH2F(Form("den_deltaT_dt_day%i_pad%i",i,ch),Form("den_deltaT DT_day%i_pad%i",i,ch),nbinsX,minX,maxX,nbinsY,minY,maxY));
  this_den_deltaT_dt_night.push_back(new TH2F(Form("den_deltaT_dt_night%i_pad%i",i,ch),Form("den_deltaT DT_night%i_pad%i",i,ch),nbinsX,minX,maxX,nbinsY,minY,maxY));
  this_den_deltaT_vme.push_back(new TH2F(Form("den_deltaT_vme%i_pad%i",i,ch),Form("den_deltaT vme%i_pad%i",i,ch),nbinsX,minX,maxX,nbinsY,minY,maxY));

  



  for(int j=0;j<nslices;j++){
    this_num_x_dt.push_back(new TH1F(Form("num_x_dt%i_pad%i_slice%i",i,ch,j),Form("num_x DT%i_pad%i_slice%i",i,ch,j),nbinsX,minX,maxX));
    this_num_x_dt_day.push_back(new TH1F(Form("num_x_dt_day%i_pad%i_slice%i",i,ch,j),Form("num_x DT_day%i_pad%i_slice%i",i,ch,j),nbinsX,minX,maxX));
    this_num_x_dt_night.push_back(new TH1F(Form("num_x_dt_night%i_pad%i_slice%i",i,ch,j),Form("num_x DT_night%i_pad%i_slice%i",i,ch,j),nbinsX,minX,maxX));
    this_num_x_vme.push_back(new TH1F(Form("num_x_vme%i_pad%i_slice%i",i,ch,j),Form("num_x vme%i_pad%i_slice%i",i,ch,j),nbinsX,minX,maxX));

    this_den_x_dt.push_back(new TH1F(Form("den_x_dt%i_pad%i_slice%i",i,ch,j),Form("den_x DT%i_pad%i_slice%i",i,ch,j),nbinsX,minX,maxX));
    this_den_x_dt_day.push_back(new TH1F(Form("den_x_dt_day%i_pad%i_slice%i",i,ch,j),Form("den_x DT_day%i_pad%i_slice%i",i,ch,j),nbinsX,minX,maxX));
    this_den_x_dt_night.push_back(new TH1F(Form("den_x_dt_night%i_pad%i_slice%i",i,ch,j),Form("den_x DT_night%i_pad%i_slice%i",i,ch,j),nbinsX,minX,maxX));
    this_den_x_vme.push_back(new TH1F(Form("den_x_vme%i_pad%i_slice%i",i,ch,j),Form("den_x vme%i_pad%i_slice%i",i,ch,j),nbinsX,minX,maxX));

    this_num_gain_x_dt.push_back(new TH1F(Form("num_gain_x_dt%i_pad%i_slice%i",i,ch,j),Form("num_gain_x DT%i_pad%i_slice%i",i,ch,j),nbinsX,minX,maxX));
    this_num_gain_x_dt_day.push_back(new TH1F(Form("num_gain_x_dt_day%i_pad%i_slice%i",i,ch,j),Form("num_gain_x DT_day%i_pad%i_slice%i",i,ch,j),nbinsX,minX,maxX));
    this_num_gain_x_dt_night.push_back(new TH1F(Form("num_gain_x_dt_night%i_pad%i_slice%i",i,ch,j),Form("num_gain_x DT_night%i_pad%i_slice%i",i,ch,j),nbinsX,minX,maxX));
    this_num_gain_x_vme.push_back(new TH1F(Form("num_gain_x_vme%i_pad%i_slice%i",i,ch,j),Form("num_gain_x vme%i_pad%i_slice%i",i,ch,j),nbinsX,minX,maxX));

    this_den_gain_x_dt.push_back(new TH1F(Form("den_gain_x_dt%i_pad%i_slice%i",i,ch,j),Form("den_gain_x DT%i_pad%i_slice%i",i,ch,j),nbinsX,minX,maxX));
    this_den_gain_x_dt_day.push_back(new TH1F(Form("den_gain_x_dt_day%i_pad%i_slice%i",i,ch,j),Form("den_gain_x DT_day%i_pad%i_slice%i",i,ch,j),nbinsX,minX,maxX));
    this_den_gain_x_dt_night.push_back(new TH1F(Form("den_gain_x_dt_night%i_pad%i_slice%i",i,ch,j),Form("den_gain_x DT_night%i_pad%i_slice%i",i,ch,j),nbinsX,minX,maxX));
    this_den_gain_x_vme.push_back(new TH1F(Form("den_gain_x_vme%i_pad%i_slice%i",i,ch,j),Form("den_gain_x vme%i_pad%i_slice%i",i,ch,j),nbinsX,minX,maxX));

    this_num_deltaT_x_dt.push_back(new TH1F(Form("num_deltaT_x_dt%i_pad%i_slice%i",i,ch,j),Form("num_deltaT_x DT%i_pad%i_slice%i",i,ch,j),nbinsX,minX,maxX));
    this_num_deltaT_x_dt_day.push_back(new TH1F(Form("num_deltaT_x_dt_day%i_pad%i_slice%i",i,ch,j),Form("num_deltaT_x DT_day%i_pad%i_slice%i",i,ch,j),nbinsX,minX,maxX));
    this_num_deltaT_x_dt_night.push_back(new TH1F(Form("num_deltaT_x_dt_night%i_pad%i_slice%i",i,ch,j),Form("num_deltaT_x DT_night%i_pad%i_slice%i",i,ch,j),nbinsX,minX,maxX));
    this_num_deltaT_x_vme.push_back(new TH1F(Form("num_deltaT_x_vme%i_pad%i_slice%i",i,ch,j),Form("num_deltaT_x vme%i_pad%i_slice%i",i,ch,j),nbinsX,minX,maxX));

    this_den_deltaT_x_dt.push_back(new TH1F(Form("den_deltaT_x_dt%i_pad%i_slice%i",i,ch,j),Form("den_deltaT_x DT%i_pad%i_slice%i",i,ch,j),nbinsX,minX,maxX));
    this_den_deltaT_x_dt_day.push_back(new TH1F(Form("den_deltaT_x_dt_day%i_pad%i_slice%i",i,ch,j),Form("den_deltaT_x DT_day%i_pad%i_slice%i",i,ch,j),nbinsX,minX,maxX));
    this_den_deltaT_x_dt_night.push_back(new TH1F(Form("den_deltaT_x_dt_night%i_pad%i_slice%i",i,ch,j),Form("den_deltaT_x DT_night%i_pad%i_slice%i",i,ch,j),nbinsX,minX,maxX));
    this_den_deltaT_x_vme.push_back(new TH1F(Form("den_deltaT_x_vme%i_pad%i_slice%i",i,ch,j),Form("den_deltaT_x vme%i_pad%i_slice%i",i,ch,j),nbinsX,minX,maxX));

  }
}
  num_dt.push_back(this_num_dt);
  num_dt_day.push_back(this_num_dt_day);
  num_dt_night.push_back(this_num_dt_night);
  num_vme.push_back(this_num_vme);

  den_dt.push_back(this_den_dt);
  den_dt_day.push_back(this_den_dt_day);
  den_dt_night.push_back(this_den_dt_night);
  den_vme.push_back(this_den_vme);

  num_gain_dt.push_back(this_num_gain_dt);
  num_gain_dt_day.push_back(this_num_gain_dt_day);
  num_gain_dt_night.push_back(this_num_gain_dt_night);
  num_gain_vme.push_back(this_num_gain_vme);

  den_gain_dt.push_back(this_den_gain_dt);
  den_gain_dt_day.push_back(this_den_gain_dt_day);
  den_gain_dt_night.push_back(this_den_gain_dt_night);
  den_gain_vme.push_back(this_den_gain_vme);

  num_deltaT_dt.push_back(this_num_deltaT_dt);
  num_deltaT_dt_day.push_back(this_num_deltaT_dt_day);
  num_deltaT_dt_night.push_back(this_num_deltaT_dt_night);
  num_deltaT_vme.push_back(this_num_deltaT_vme);

  den_deltaT_dt.push_back(this_den_deltaT_dt);
  den_deltaT_dt_day.push_back(this_den_deltaT_dt_day);
  den_deltaT_dt_night.push_back(this_den_deltaT_dt_night);
  den_deltaT_vme.push_back(this_den_deltaT_vme);

  num_x_dt.push_back(this_num_x_dt);
  num_x_dt_day.push_back(this_num_x_dt_day);
  num_x_dt_night.push_back(this_num_x_dt_night);
  num_x_vme.push_back(this_num_x_vme);

  den_x_dt.push_back(this_den_x_dt);
  den_x_dt_day.push_back(this_den_x_dt_day);
  den_x_dt_night.push_back(this_den_x_dt_night);
  den_x_vme.push_back(this_den_x_vme);

  num_gain_x_dt.push_back(this_num_gain_x_dt);
  num_gain_x_dt_day.push_back(this_num_gain_x_dt_day);
  num_gain_x_dt_night.push_back(this_num_gain_x_dt_night);
  num_gain_x_vme.push_back(this_num_gain_x_vme);

  den_gain_x_dt.push_back(this_den_gain_x_dt);
  den_gain_x_dt_day.push_back(this_den_gain_x_dt_day);
  den_gain_x_dt_night.push_back(this_den_gain_x_dt_night);
  den_gain_x_vme.push_back(this_den_gain_x_vme);

  num_deltaT_x_dt.push_back(this_num_deltaT_x_dt);
  num_deltaT_x_dt_day.push_back(this_num_deltaT_x_dt_day);
  num_deltaT_x_dt_night.push_back(this_num_deltaT_x_dt_night);
  num_deltaT_x_vme.push_back(this_num_deltaT_x_vme);

  den_deltaT_x_dt.push_back(this_den_deltaT_x_dt);
  den_deltaT_x_dt_day.push_back(this_den_deltaT_x_dt_day);
  den_deltaT_x_dt_night.push_back(this_den_deltaT_x_dt_night);
  den_deltaT_x_vme.push_back(this_den_deltaT_x_vme);


}

cout<<"Finished declaring histograms"<<endl;
for(int i=0;i<nsels;i++){
  cout<<den_sel[i]+"&&"+ptk_sel[0]<<endl<<endl;
  for(int ch=0;ch<nchan+1;ch++){
    if(ch>12||ch<6) continue;
    dt->Project(Form("den_dt%i_pad%i",i,ch),yx,den_sel[i]+"&&"+ptk_sel[0]);
    dt->Project(Form("den_dt_day%i_pad%i",i,ch),yx,den_sel[i]+"&&"+ptk_sel[1]);
    dt->Project(Form("den_dt_night%i_pad%i",i,ch),yx,den_sel[i]+"&&"+ptk_sel[2]);
    vme->Project(Form("den_vme%i_pad%i",i,ch),yx,den_sel[i]+"&&"+ptk_sel[3]);


    dt->Project(Form("den_gain_dt%i_pad%i",i,ch),yx,indiv_chan_gain_den[ch] +"&&" + den_sel[i]+"&&"+ptk_sel[0]);
    dt->Project(Form("den_gain_dt_day%i_pad%i",i,ch),yx,indiv_chan_gain_den[ch] +"&&" + den_sel[i]+"&&"+ptk_sel[1]);
    dt->Project(Form("den_gain_dt_night%i_pad%i",i,ch),yx,indiv_chan_gain_den[ch] +"&&" + den_sel[i]+"&&"+ptk_sel[2]);
    vme->Project(Form("den_gain_vme%i_pad%i",i,ch),yx,indiv_chan_gain_den[ch] +"&&" + den_sel[i]+"&&"+ptk_sel[3]);

    dt->Project(Form("den_deltaT_dt%i_pad%i",i,ch),yx,indiv_chan_sel[ch] +"&&"+ den_sel[i]+"&&"+ptk_sel[0]);
    dt->Project(Form("den_deltaT_dt_day%i_pad%i",i,ch),yx,indiv_chan_sel[ch] +"&&"+ den_sel[i]+"&&"+ptk_sel[1]);
    dt->Project(Form("den_deltaT_dt_night%i_pad%i",i,ch),yx,indiv_chan_sel[ch] +"&&"+ den_sel[i]+"&&"+ptk_sel[2]);
    vme->Project(Form("den_deltaT_vme%i_pad%i",i,ch),yx,indiv_chan_sel[ch] +"&&"+ den_sel[i]+"&&"+ptk_sel[3]);

    cout<<num_sel+"&&"+den_sel[i]+"&&"+ptk_sel[0]<<endl<<endl;

    dt->Project(Form("num_dt%i_pad%i",i,ch),yx,indiv_chan_sel[ch]+"&&"+den_sel[i]+"&&"+ptk_sel[0]);
    dt->Project(Form("num_dt_day%i_pad%i",i,ch),yx,indiv_chan_sel[ch]+"&&"+den_sel[i]+"&&"+ptk_sel[1]);
    dt->Project(Form("num_dt_night%i_pad%i",i,ch),yx,indiv_chan_sel[ch]+"&&"+den_sel[i]+"&&"+ptk_sel[2]);
    vme->Project(Form("num_vme%i_pad%i",i,ch),yx,indiv_chan_sel[ch]+"&&"+den_sel[i]+"&&"+ptk_sel[3]);
    
    dt->Project(Form("num_gain_dt%i_pad%i",i,ch),yx, indiv_chan_gain_weight[ch] +"*(" + indiv_chan_sel[ch]+"&&"+den_sel[i]+"&&"+ptk_sel[0]+")");
    dt->Project(Form("num_gain_dt_day%i_pad%i",i,ch),yx, indiv_chan_gain_weight[ch] +"*(" + indiv_chan_sel[ch]+"&&"+den_sel[i]+"&&"+ptk_sel[1]+")");
    dt->Project(Form("num_gain_dt_night%i_pad%i",i,ch),yx, indiv_chan_gain_weight[ch] +"*(" + indiv_chan_sel[ch]+"&&"+den_sel[i]+"&&"+ptk_sel[2]+")");
    vme->Project(Form("num_gain_vme%i_pad%i",i,ch),yx, indiv_chan_gain_weight[ch] +"*(" + indiv_chan_sel[ch]+"&&"+den_sel[i]+"&&"+ptk_sel[3]+")");
    
    dt->Project(Form("num_deltaT_dt%i_pad%i",i,ch),yx,indiv_chan_deltaT_weight[ch] +"*(" +indiv_chan_sel[ch]+"&&"+den_sel[i]+"&&"+ptk_sel[0]+")");
    dt->Project(Form("num_deltaT_dt_day%i_pad%i",i,ch),yx,indiv_chan_deltaT_weight[ch] +"*(" +indiv_chan_sel[ch]+"&&"+den_sel[i]+"&&"+ptk_sel[1]+")");
    dt->Project(Form("num_deltaT_dt_night%i_pad%i",i,ch),yx,indiv_chan_deltaT_weight[ch] +"*(" +indiv_chan_sel[ch]+"&&"+den_sel[i]+"&&"+ptk_sel[2]+")");
    vme->Project(Form("num_deltaT_vme%i_pad%i",i,ch),yx,indiv_chan_deltaT_weight[ch] +"*(" +indiv_chan_sel[ch]+"&&"+den_sel[i]+"&&"+ptk_sel[3]+")");
    

  for(int j=0;j<nslices;j++){
    int this_index = nslices*i + j;

    dt->Project(Form("den_x_dt%i_pad%i_slice%i",i,ch,j),"x_corr",den_sel[i]+"&&"+ptk_sel[0]+"&&"+y_slice[j]);
    dt->Project(Form("den_x_dt_day%i_pad%i_slice%i",i,ch,j),"x_corr",den_sel[i]+"&&"+ptk_sel[1]+"&&"+y_slice[j]);
    dt->Project(Form("den_x_dt_night%i_pad%i_slice%i",i,ch,j),"x_corr",den_sel[i]+"&&"+ptk_sel[2]+"&&"+y_slice[j]);
    vme->Project(Form("den_x_vme%i_pad%i_slice%i",i,ch,j),"x_corr",den_sel[i]+"&&"+ptk_sel[3]+"&&"+y_slice[j]);

    dt->Project(Form("num_x_dt%i_pad%i_slice%i",i,ch,j),"x_corr",indiv_chan_sel[ch]+"&&"+den_sel[i]+"&&"+ptk_sel[0]+"&&"+y_slice[j]);
    dt->Project(Form("num_x_dt_day%i_pad%i_slice%i",i,ch,j),"x_corr",indiv_chan_sel[ch]+"&&"+den_sel[i]+"&&"+ptk_sel[1]+"&&"+y_slice[j]);
    dt->Project(Form("num_x_dt_night%i_pad%i_slice%i",i,ch,j),"x_corr",indiv_chan_sel[ch]+"&&"+den_sel[i]+"&&"+ptk_sel[2]+"&&"+y_slice[j]);
    vme->Project(Form("num_x_vme%i_pad%i_slice%i",i,ch,j),"x_corr",indiv_chan_sel[ch]+"&&"+den_sel[i]+"&&"+ptk_sel[3]+"&&"+y_slice[j]);

    cout<<"Numerator is "<<num_sel+"&&"+den_sel[i]+"&&"+ptk_sel[2]+"&&"+y_slice[j]<<endl<<endl;
    cout<<"Denominator is "<< den_sel[i]+"&&"+ptk_sel[2]+"&&"+y_slice[j]<<endl<<endl;

  
    // float start=5.3; float end= 6.5;
    // float avg_eff_pad_10 = num_x_dt_night[this_index]->Integral(num_x_dt_night[this_index]->FindBin(start),num_x_dt_night[this_index]->FindBin(end));
    // avg_eff_pad_10 /= den_x_dt_night[this_index]->Integral(den_x_dt_night[this_index]->FindBin(start),den_x_dt_night[this_index]->FindBin(end));
    
    // cout<<"Selection "<<i<<", efficiency "<<avg_eff_pad_10<<endl;
    // new TCanvas();
    // num_x_dt_night[this_index]->Draw("hist");
    // new TCanvas();
    // den_x_dt_night[this_index]->Draw("hist");


    // new TCanvas();
    // eff_x_dt[this_index]->Draw("hist");
    // new TCanvas();
    // eff_x_dt_day[this_index]->Draw("hist");

    // new TCanvas();
    // eff_x_vme[this_index]->Draw("hist");
  }
  }

  vector<TH2F*> this_eff_dt, this_eff_dt_day,this_eff_dt_night,this_eff_vme;
  vector<TH2F*> this_eff_gain_dt, this_eff_gain_dt_day,this_eff_gain_dt_night,this_eff_gain_vme;
  vector<TH2F*> this_eff_deltaT_dt, this_eff_deltaT_dt_day,this_eff_deltaT_dt_night,this_eff_deltaT_vme;
  vector<TH1F*> this_eff_x_dt, this_eff_x_dt_day,this_eff_x_dt_night,this_eff_x_vme;
  
  for(int ch=0;ch<nchan+1;ch++){
    this_eff_dt.push_back((TH2F*)num_dt[i][ch]->Clone());
    this_eff_dt_day.push_back((TH2F*)num_dt_day[i][ch]->Clone());
    this_eff_dt_night.push_back((TH2F*)num_dt_night[i][ch]->Clone());
    this_eff_vme.push_back((TH2F*)num_vme[i][ch]->Clone());

    this_eff_gain_dt.push_back((TH2F*)num_gain_dt[i][ch]->Clone());
    this_eff_gain_dt_day.push_back((TH2F*)num_gain_dt_day[i][ch]->Clone());
    this_eff_gain_dt_night.push_back((TH2F*)num_gain_dt_night[i][ch]->Clone());
    this_eff_gain_vme.push_back((TH2F*)num_gain_vme[i][ch]->Clone());

    this_eff_deltaT_dt.push_back((TH2F*)num_deltaT_dt[i][ch]->Clone());
    this_eff_deltaT_dt_day.push_back((TH2F*)num_deltaT_dt_day[i][ch]->Clone());
    this_eff_deltaT_dt_night.push_back((TH2F*)num_deltaT_dt_night[i][ch]->Clone());
    this_eff_deltaT_vme.push_back((TH2F*)num_deltaT_vme[i][ch]->Clone());

    this_eff_dt[ch]->Divide(den_dt[i][ch]);
    this_eff_dt_day[ch]->Divide(den_dt_day[i][ch]);
    this_eff_dt_night[ch]->Divide(den_dt_night[i][ch]);
    this_eff_vme[ch]->Divide(den_vme[i][ch]);

    this_eff_gain_dt[ch]->Divide(den_gain_dt[i][ch]);
    this_eff_gain_dt_day[ch]->Divide(den_gain_dt_day[i][ch]);
    this_eff_gain_dt_night[ch]->Divide(den_gain_dt_night[i][ch]);
    this_eff_gain_vme[ch]->Divide(den_gain_vme[i][ch]);

    this_eff_deltaT_dt[ch]->Divide(den_deltaT_dt[i][ch]);
    this_eff_deltaT_dt_day[ch]->Divide(den_deltaT_dt_day[i][ch]);
    this_eff_deltaT_dt_night[ch]->Divide(den_deltaT_dt_night[i][ch]);
    this_eff_deltaT_vme[ch]->Divide(den_deltaT_vme[i][ch]);

    this_eff_x_dt.push_back((TH1F*)num_x_dt[i][ch]->Clone());
    this_eff_x_dt_day.push_back((TH1F*)num_x_dt_day[i][ch]->Clone());
    this_eff_x_dt_night.push_back((TH1F*)num_x_dt_night[i][ch]->Clone());
    this_eff_x_vme.push_back((TH1F*)num_x_vme[i][ch]->Clone());

    this_eff_x_dt[ch]->Divide(den_x_dt[i][ch]);
    this_eff_x_dt_day[ch]->Divide(den_x_dt_day[i][ch]);
    this_eff_x_dt_night[ch]->Divide(den_x_dt_night[i][ch]);
    this_eff_x_vme[ch]->Divide(den_x_vme[i][ch]);

    new TCanvas();
    this_eff_dt_night[ch]->Draw("colz");

    new TCanvas();
    this_eff_gain_dt_night[ch]->Draw("colz");

    new TCanvas();
    this_eff_deltaT_dt_night[ch]->Draw("colz");

    //new TCanvas();
   // eff_x_dt_night[this_index]->Draw("hist");

  }

  eff_dt.push_back(this_eff_dt);
  eff_dt_day.push_back(this_eff_dt_day);
  eff_dt_night.push_back(this_eff_dt_night);
  eff_vme.push_back(this_eff_vme);

  eff_gain_dt.push_back(this_eff_gain_dt);
  eff_gain_dt_day.push_back(this_eff_gain_dt_day);
  eff_gain_dt_night.push_back(this_eff_gain_dt_night);
  eff_gain_vme.push_back(this_eff_gain_vme);

  eff_deltaT_dt.push_back(this_eff_deltaT_dt);
  eff_deltaT_dt_day.push_back(this_eff_deltaT_dt_day);
  eff_deltaT_dt_night.push_back(this_eff_deltaT_dt_night);
  eff_deltaT_vme.push_back(this_eff_deltaT_vme);

  eff_x_dt.push_back(this_eff_x_dt);
  eff_x_dt_day.push_back(this_eff_x_dt_day);
  eff_x_dt_night.push_back(this_eff_x_dt_night);
  eff_x_vme.push_back(this_eff_x_vme);

  //Define pad selections in xy
  //Plot amplitude, delta T for each one


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
  // new TCanvas();
  // num_dt_day[i]->Draw("colz");
  // new TCanvas();
  // num_dt_night[i]->Draw("colz");
  // new TCanvas();
  // num_vme[i]->Draw("colz");

  // new TCanvas();
  // eff_dt[i]->Draw("colz");
  // new TCanvas();
  // eff_dt_day[i]->Draw("colz");
  new TCanvas();
  eff_dt_night[i][0]->Draw("colz");
  // new TCanvas();
  // eff_vme[i]->Draw("colz");

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

