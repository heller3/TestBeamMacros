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
#include "TTree.h"
#include "TChain.h"
#include "TMath.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TH3F.h"
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

bool debug=false;

int run_boundaries[2] ={1765,1766};// {3150,3150};
const int nchan=36;
const int nlgad=16;
float hitThres=15.;
TChain * t;
TFile * outRootFile;

Float_t amp[nchan];
Float_t risetime[nchan];
Float_t LP2_30[nchan];
Float_t gaus_mean[nchan];
Int_t ptkidx[nlgad];
Int_t chidx[nlgad];
Float_t y_dut[3];
Float_t x_dut[3];
Float_t y_corr;
Float_t x_corr;
Int_t run;
Float_t chi2;
Int_t ntracks;

vector<TH3F*> v_h_eff;
vector<TH3F*> v_h_amp;
vector<TH3F*> v_h_time;
vector<TH3F*> v_h_run;

vector<TH2F*> v_map_eff;
vector<TH2F*> v_map_nhits;
vector<TH2F*> v_map_amp;
vector<TH2F*> v_map_deltat;
vector<TH2F*> v_map_sigmat;

TH2F * cosmetic_map;
TH2F * cosmetic_map_scaled;

vector<TH1F*> v_x_eff_top;
vector<TH1F*> v_x_nhits_top;
vector<TH1F*> v_x_amp_top;
vector<TH1F*> v_x_deltat_top;
vector<TH1F*> v_x_sigmat_top;

vector<TH1F*> v_x_eff_bot;
vector<TH1F*> v_x_nhits_bot;
vector<TH1F*> v_x_amp_bot;
vector<TH1F*> v_x_deltat_bot;
vector<TH1F*> v_x_sigmat_bot;

vector<TH1F*> v_y_eff;
vector<TH1F*> v_y_nhits;
vector<TH1F*> v_y_amp;
vector<TH1F*> v_y_deltat;
vector<TH1F*> v_y_sigmat;

vector<TH1D*> v_amp_dists;
vector<TH1D*> v_run_dists;

vector<float> box_x_lo = {2.4,10.8};
vector<float> box_x_hi = {3.39,11.79};

vector<float> box_y_lo = {18.6,20.6}; 
vector<float> box_y_hi = {19.59,21.59}; 

//efficiency
int nbinsX=140;
int nbinsY=60;
int gainbinfactor=2; //coarser binning for gain maps


//gain
// int nbinsX=70;
// int nbinsY=30;


int minX=-2;
int maxX=19;
float minY=15.5;
float maxY=24.5;


float nbinsAmp=40;
float nbinsTime=40;
float minAmp=0;
float maxAmp=200;
float minTime=-5;
float maxTime=5;


pair<int,int> nLGADHitsAndChannel();
bool GoodPhotekHit();
void InitBranches();
void ConvertMap(TH3F * h3, TH2F * h2, int type);
void Convert1D(TH3F * h3, TH1F * h1, int type, int slice);
void FillBox(TH3F * h3, TH1D * h1, int ibox);
pair<float,float> GetMPV(TH1F * h);
void CleanMap(TH2F * map, float xmin, float xmax, float ymin, float ymax, bool scale);
pair<float,float> Rotate(float x0, float y0, float angle);
void CosmeticMap(TH2F * map, TString zaxis);
void DrawCMS();
// g++ -o plot_maps plot_maps.C  `root-config --cflags --glibs`
void plot_maps(TString chainPath,TString tag){

t = new TChain("pulse");
t->Add(chainPath);
InitBranches();
uint nentries= t->GetEntries();
cout<<"Loaded trees with  "<<nentries<<" events."<<endl;

vector<TString> base_sel = {"ntracks==1&&chi2<3&&(amp[0]>150&&amp[0]<200&&run>=1766)","first","last"};
vector<TString> eff_sel = {""};
vector<TString> amp_sel = {""};
vector<TString> time_sel = {"",""};

//vector<TString> ptk_sel = {"((amp[4]>10&&amp[4]<50&&run<1766)||(amp[0]>150&&amp[0]<200&&run>=1766))","(amp[4]>10&&amp[4]<50&&run<1766)","(amp[0]>150&&amp[0]<200&&run>=1766)","(amp[27]>40&&amp[27]<120&&run<1766)"};
TString single_chan_sel = "(chidx[dummy]>=0 && (amp[chidx[dummy]]>15 && risetime[chidx[dummy]]>0.15))";

vector<int> lgad_channels = {1,2,3,5,6,7,11,13,14,15,16};


// TString num_sel = "(";
// for(int i=0;i<nchan;i++){
//   TString buf(single_chan_sel);
//   buf.ReplaceAll("dummy",Form("%i",i));
//   if(i>0) num_sel+= " || ";
//   num_sel+= buf;
// }
// num_sel+=")";


// TString amp_num = "MaxIf$(amp,";
// for(uint i=0;i<lgad_channels.size();i++){
// 	if(i>0) amp_num+="||"
// 	amp_num+=Form("Iteration$==",lgad_channels[i]);	
// }
// amp_num+=")";

// TString time_num = "MinIf$(LP2_30-gaus_mean)";
// for(uint i=0;i<lgad_channels.size();i++){
// 	if(i>0) time_num+="||"
// 	time_num+=Form("Iteration$==",lgad_channels[i]);	
// }
// time_num+=")";


outRootFile = new TFile(Form("%s.root",tag.Data()),"recreate");

for(uint ib = 0; ib < base_sel.size(); ib++){
	for(uint ie = 0; ie < eff_sel.size(); ie++){
		TString name = Form("h3_eff_%i_%i",ib,ie);
		v_h_eff.push_back(new TH3F(name,name,nbinsX,minX,maxX,nbinsY,minY,maxY,2,0,2));
		name = Form("h_map_eff_%i_%i",ib,ie);
		v_map_eff.push_back(new TH2F(name,name,nbinsX,minX,maxX,nbinsY,minY,maxY));
		name = Form("h_map_nhits_%i_%i",ib,ie);
		v_map_nhits.push_back(new TH2F(name,name,nbinsX,minX,maxX,nbinsY,minY,maxY));

		name = Form("h_x_eff_top_%i_%i",ib,ie);
		v_x_eff_top.push_back(new TH1F(name,name,nbinsX,minX,maxX));
		name = Form("h_x_nhits_top_%i_%i",ib,ie);
		v_x_nhits_top.push_back(new TH1F(name,name,nbinsX,minX,maxX));

		name = Form("h_x_eff_bot_%i_%i",ib,ie);
		v_x_eff_bot.push_back(new TH1F(name,name,nbinsX,minX,maxX));
		name = Form("h_x_nhits_bot_%i_%i",ib,ie);
		v_x_nhits_bot.push_back(new TH1F(name,name,nbinsX,minX,maxX));

		name = Form("h_y_eff_%i_%i",ib,ie);
		v_y_eff.push_back(new TH1F(name,name,nbinsY,minY,maxY));
		name = Form("h_y_nhits_%i_%i",ib,ie);
		v_y_nhits.push_back(new TH1F(name,name,nbinsY,minY,maxY));

	}
	for(uint ia = 0; ia < amp_sel.size(); ia++){
		TString name = Form("h3_amp_%i_%i",ib,ia);
		v_h_amp.push_back(new TH3F(name,name,nbinsX/gainbinfactor,minX,maxX,nbinsY/gainbinfactor,minY,maxY,nbinsAmp,minAmp,maxAmp));
		name = Form("h_map_amp_%i_%i",ib,ia);
		v_map_amp.push_back(new TH2F(name,name,nbinsX/gainbinfactor,minX,maxX,nbinsY/gainbinfactor,minY,maxY));
		
		name = Form("h_x_amp_top_%i_%i",ib,ia);
		v_x_amp_top.push_back(new TH1F(name,name,nbinsX/gainbinfactor,minX,maxX));
		name = Form("h_x_amp_bot_%i_%i",ib,ia);
		v_x_amp_bot.push_back(new TH1F(name,name,nbinsX/gainbinfactor,minX,maxX));
		name = Form("h_y_amp_%i_%i",ib,ia);
		v_y_amp.push_back(new TH1F(name,name,nbinsY/gainbinfactor,minY,maxY));


		name = Form("h3_run_%i_%i",ib,ia);
		v_h_run.push_back(new TH3F(name,name,nbinsX,minX,maxX,nbinsY,minY,maxY,40,2900,3700));

	}

	for(uint ibox=0;ibox<box_x_lo.size();ibox++){
		TString name = Form("dist_amp_%i_%i",ib,ibox);
		v_amp_dists.push_back(new TH1D(name,name,nbinsAmp,minAmp,maxAmp));
		name = Form("run_amp_%i_%i",ib,ibox);
		v_run_dists.push_back(new TH1D(name,name,40,2900,3700));
	}


	for(uint it = 0; it < time_sel.size(); it++){
		TString name = Form("h3_time_%i_%i",ib,it);
		v_h_time.push_back(new TH3F(name,name,nbinsX,minX,maxX,nbinsY,minY,maxY,nbinsTime,minTime,maxTime));
		name = Form("h_map_deltat_%i_%i",ib,it);
		v_map_deltat.push_back(new TH2F(name,name,nbinsX,minX,maxX,nbinsY,minY,maxY));
		name = Form("h_map_sigmat_%i_%i",ib,it);
		v_map_sigmat.push_back(new TH2F(name,name,nbinsX,minX,maxX,nbinsY,minY,maxY));
		
		name = Form("h_x_deltat_top_%i_%i",ib,it);
		v_x_deltat_top.push_back(new TH1F(name,name,nbinsX,minX,maxX));
		name = Form("h_x_sigmat_top_%i_%i",ib,it);
		v_x_sigmat_top.push_back(new TH1F(name,name,nbinsX,minX,maxX));

		name = Form("h_x_deltat_bot_%i_%i",ib,it);
		v_x_deltat_bot.push_back(new TH1F(name,name,nbinsX,minX,maxX));
		name = Form("h_x_sigmat_bot_%i_%i",ib,it);
		v_x_sigmat_bot.push_back(new TH1F(name,name,nbinsX,minX,maxX));

		name = Form("h_y_deltat_%i_%i",ib,it);
		v_y_deltat.push_back(new TH1F(name,name,nbinsY,minY,maxY));
		name = Form("h_y_sigmat_%i_%i",ib,it);
		v_y_sigmat.push_back(new TH1F(name,name,nbinsY,minY,maxY));
	}
}



int ngoodevents=0;
if(debug) nentries=15;
for(int i=0;i<nentries;i++){
	t->GetEntry(i);
	if (i % 1000 == 0) {
	  fprintf(stdout, "\r  Processed events: %8d of %8d ", i, nentries);
	}
	fflush(stdout);
	//if(run<1766) continue;

	//Skip events without exactly one track
	if(ntracks!=1 || chi2 > 7) continue;

	//Require good photek hit
	if( run<2900 && !GoodPhotekHit()) continue;

	//Require 0 or 1 LGAD hits to veto showers
	pair<int,int> nhits_and_channel =nLGADHitsAndChannel();
	int nhits= nhits_and_channel.first;
	int channel = nhits_and_channel.second;
	if(nhits>1) continue;
	ngoodevents++;
	if(debug){ 
		cout<<"Good entry, "<<i<<endl;
		cout<<"N hits: "<<nhits<<endl;
		for(int j=0;j<nlgad;j++){
			cout<<chidx[j]<<" ";
		}
		cout<<endl;
		for(int j=0;j<nchan;j++){
			cout<<amp[j]<<" "<<risetime[j]<<" ";
		}
		cout<<endl;
		}
	//Get amplitude, time of real hit
	//Fill hists

	float x_adjust = x_corr+1.2;
	float y_adjust = y_corr-0.3;

	pair<float,float> rotated = Rotate(x_adjust,y_adjust,0.28);
	x_adjust = rotated.first;
	y_adjust = rotated.second;


	if(run<2000){
		x_adjust-= 4; y_adjust-=1.;
		if(run<1766) y_adjust+=0.8;
		if(run>=1766) y_adjust+=0.4;
	}

	v_h_eff[0]->Fill(x_adjust,y_adjust,nhits);
	if(run<run_boundaries[0]) v_h_eff[1]->Fill(x_adjust,y_adjust,nhits);
	if(run>run_boundaries[1]) v_h_eff[2]->Fill(x_adjust,y_adjust,nhits);

	if(nhits==0) continue;
	if(channel<0) cout<<"Channel value doesn't make sense"<<endl;
	int index = chidx[channel];
	int ptkindex = ptkidx[channel];

	v_h_amp[0]->Fill(x_adjust,y_adjust,amp[index]);
	v_h_run[0]->Fill(x_adjust,y_adjust,run);
	if(run<run_boundaries[0]) {
		v_h_amp[1]->Fill(x_adjust,y_adjust,amp[index]);
		v_h_run[1]->Fill(x_adjust,y_adjust,run);}
	if(run>run_boundaries[1]) {
		v_h_amp[2]->Fill(x_adjust,y_adjust,amp[index]);
		v_h_run[2]->Fill(x_adjust,y_adjust,run);}

	if(ptkindex>=0){
		float delta_t = -LP2_30[index]+gaus_mean[ptkindex];
		if(delta_t<0) continue;
		v_h_time[0]->Fill(x_adjust,y_adjust,delta_t);
		if(run<run_boundaries[0]) v_h_time[1]->Fill(x_adjust,y_adjust,delta_t);
		if(run>run_boundaries[1]) v_h_time[2]->Fill(x_adjust,y_adjust,delta_t);
		//if (amp[index] < 80)
			//v_h_time[1]->Fill(x_adjust,y_adjust,delta_t);
	}

}

cout<<"Processed "<<nentries<<" events, "<<ngoodevents<<" good events."<<endl;
	//Rebin gain plots:
	//Doesn't work correctly for 3D hist?
	// for(uint ie = 0; ie < v_h_eff.size(); ie++){
	// 	v_h_amp[ie]->RebinX();
	// 	v_h_amp[ie]->RebinY();
	// }

	//Construct maps
	for(uint ie = 0; ie < v_h_eff.size(); ie++){
		ConvertMap(v_h_eff[ie],v_map_eff[ie],4);
		ConvertMap(v_h_eff[ie],v_map_nhits[ie],2);

		Convert1D(v_h_eff[ie],v_x_eff_top[ie],4,0);
		Convert1D(v_h_eff[ie],v_x_nhits_top[ie],2,0);

		Convert1D(v_h_eff[ie],v_x_eff_bot[ie],4,1);
		Convert1D(v_h_eff[ie],v_x_nhits_bot[ie],2,1);

		Convert1D(v_h_eff[ie],v_y_eff[ie],4,2);
		Convert1D(v_h_eff[ie],v_y_nhits[ie],2,2);
		v_h_eff[ie]->Write();
		v_map_eff[ie]->Write();
		v_map_nhits[ie]->Write();
		v_x_eff_top[ie]->Write();
		v_x_nhits_top[ie]->Write();
		v_x_eff_bot[ie]->Write();
		v_x_nhits_bot[ie]->Write();
		v_y_eff[ie]->Write();
		v_y_nhits[ie]->Write();
	}


	cout<<"Finished efficiency maps"<<endl;
	for(uint ie = 0; ie < v_h_amp.size(); ie++){
		ConvertMap(v_h_amp[ie],v_map_amp[ie],3);
		Convert1D(v_h_amp[ie],v_x_amp_top[ie],3,0);
		Convert1D(v_h_amp[ie],v_x_amp_bot[ie],3,1);
		Convert1D(v_h_amp[ie],v_y_amp[ie],3,2);
		v_h_amp[ie]->Write();
		v_map_amp[ie]->Write();
		v_x_amp_top[ie]->Write();
		v_x_amp_bot[ie]->Write();
		v_y_amp[ie]->Write();
	}
	cout<<"Finished amp maps"<<endl;
	for(uint ie = 0; ie < v_h_time.size(); ie++){
		v_h_time[ie]->Write();
		ConvertMap(v_h_time[ie],v_map_deltat[ie],0);
		v_map_deltat[ie]->Write();
		ConvertMap(v_h_time[ie],v_map_sigmat[ie],1);
		v_map_sigmat[ie]->Write();
		Convert1D(v_h_time[ie],v_x_deltat_top[ie],0,0);
		v_x_deltat_top[ie]->Write();
		Convert1D(v_h_time[ie],v_x_sigmat_bot[ie],1,1);
		v_x_sigmat_bot[ie]->Write();

		Convert1D(v_h_time[ie],v_y_deltat[ie],0,3);
		v_y_deltat[ie]->Write();
		Convert1D(v_h_time[ie],v_y_sigmat[ie],1,3);
		v_y_sigmat[ie]->Write();

	}
	cout<<"Finished time maps"<<endl;

	//Fill distributions for boxes 
	for(uint ie = 0; ie < v_h_amp.size(); ie++){
		for(uint ibox=0;ibox<box_x_lo.size();ibox++ ){
			FillBox(v_h_amp[ie],v_amp_dists[ie*box_x_lo.size()+ibox], ibox);
			v_amp_dists[ie*box_x_lo.size()+ibox]->Write();
			FillBox(v_h_run[ie],v_run_dists[ie*box_x_lo.size()+ibox], ibox);
			v_run_dists[ie*box_x_lo.size()+ibox]->Write();
		}
	}

	TH2F* gain_map = (TH2F*)v_map_amp[0]->Clone("amp_cosmetic");
	CleanMap(gain_map,-0.26,16.45,17.9,22.,false);
	CosmeticMap(gain_map,"Most probable value [mV]");
	gain_map->SetMinimum(20.);


	cosmetic_map = (TH2F*)v_map_eff[0]->Clone("cosmetic");
	cosmetic_map_scaled = (TH2F*)v_map_eff[0]->Clone("cosmetic_scale");

	CleanMap(cosmetic_map,-0.26,16.65,17.9,22.2,false);
	CosmeticMap(cosmetic_map,"Hit efficiency");

	CleanMap(cosmetic_map_scaled,-0.26,16.65,17.9,22.2,true);
	CosmeticMap(cosmetic_map_scaled,"Hit efficiency");
	cosmetic_map_scaled->SetMaximum(1.);

	TCanvas c1("","",1100,500);
	c1.SetLeftMargin(0.07);
	c1.SetRightMargin(0.15);
	c1.SetBottomMargin(0.13);


	cosmetic_map->Write();
	cosmetic_map->Draw("colz");
	DrawCMS();
	c1.Print("efficiency_map_unscaled.pdf");

	TCanvas c2("","",1100,500);
	c2.SetLeftMargin(0.07);
	c2.SetRightMargin(0.15);
	c2.SetBottomMargin(0.13);

	cosmetic_map_scaled->Write();
	cosmetic_map_scaled->Draw("colz");
	DrawCMS();
	c2.Print("efficiency_map_scaled.pdf");


	TCanvas c3("","",1100,500);
	c3.SetLeftMargin(0.07);
	c3.SetRightMargin(0.15);
	c3.SetBottomMargin(0.13);

	gain_map->Write();
	gain_map->Draw("colz");
	DrawCMS();
	c3.Print("gain_map.pdf");



outRootFile->Close();
}

void CosmeticMap(TH2F * map, TString zaxis){
		map->SetStats(0);
		map->SetTitle("");
		map->SetTitle(Form(";x [mm];y [mm];%s",zaxis.Data()));
	//	map->GetXaxis()->SetTitleOffset(0.95);
		map->GetXaxis()->SetTitleSize(0.05);
		map->GetYaxis()->SetTitleOffset(0.65);
		map->GetZaxis()->SetTitleOffset(0.8);
		map->GetYaxis()->SetTitleSize(0.05);
		map->GetZaxis()->SetTitleSize(0.055);
		map->GetXaxis()->SetLabelSize(0.045);
		map->GetYaxis()->SetLabelSize(0.045);
		map->GetZaxis()->SetLabelSize(0.045);


}

void DrawCMS(){
	TLatex * tla = new TLatex();
    tla->SetTextSize(0.05);
	tla->DrawLatexNDC(0.07,0.92,"#font[62]{CMS} #scale[0.8]{#font[52]{Preliminary}}");
}

void CleanMap(TH2F * map, float xmin, float xmax, float ymin, float ymax, bool scale){
	for(uint ix=0;ix<map->GetNbinsX();ix++){
		float thisx = map->GetXaxis()->GetBinLowEdge(ix);
		for(uint iy=0;iy<map->GetNbinsY();iy++){
			float thisy = map->GetYaxis()->GetBinLowEdge(iy);
			if(thisx<xmin || thisx>xmax || thisy<ymin || thisy>ymax){
				map->SetBinContent(ix,iy,0);
			}
			else{
				float this_content = map->GetBinContent(ix,iy);
				if (scale) map->SetBinContent(ix,iy,this_content/0.9);
			}
		}
	}
}

pair<float,float> Rotate(float x0, float y0, float angle){
	float rad_angle = angle*3.14159/180.;
 	float x_rot = x0*cos(rad_angle) + y0*sin(rad_angle);
 	float y_rot = y0*cos(rad_angle) - x0*sin(rad_angle);
 	return pair<float,float>{x_rot,y_rot};
}


void FillBox(TH3F * h3, TH1D * h1, int ibox){
	int xi = h3->GetXaxis()->FindBin(box_x_lo[ibox]);
	int xf = h3->GetXaxis()->FindBin(box_x_hi[ibox]);
	int yi = h3->GetYaxis()->FindBin(box_y_lo[ibox]);
	int yf = h3->GetYaxis()->FindBin(box_y_hi[ibox]);

	TString histname = h1->GetName();
	h3->ProjectionZ(histname,xi,xf,yi,yf);

}
void Convert1D(TH3F * h3, TH1F * h1, int type, int slice){
	bool isX = true;
	if(slice>=2) isX=false;

	int yi,yf,xi,xf;
	if(slice==0){ //1mm across top row
		yi = h3->GetYaxis()->FindBin(20.6);
		yf = h3->GetYaxis()->FindBin(21.59);
	}
	else if(slice==1){ //1mm across bottom row
		yi = h3->GetYaxis()->FindBin(18.6);
		yf = h3->GetYaxis()->FindBin(19.59);
	}
	else if(slice==2){
		xi = h3->GetXaxis()->FindBin(10.8);
		xf = h3->GetXaxis()->FindBin(11.79);
	}

	int nbins=0;
	if (isX) nbins = h3->GetNbinsX();
	else nbins = h3->GetNbinsY();

	for(int ix=0;ix<nbins;ix++){
		TH1F * h;
		if (isX) h = (TH1F *) h3->ProjectionZ("temp",ix,ix,yi,yf);
		else h = (TH1F *) h3->ProjectionZ("temp",xi,xf,ix,ix);

		if(type==0){
			if(h->GetEntries() > 10){
				h1->SetBinContent(ix,h->GetMean());
				h1->SetBinError(ix,h->GetMeanError());
			}
		}
		if(type==1){
			if(h->GetEntries() > 10){
				h1->SetBinContent(ix,h->GetRMS());
				h1->SetBinError(ix,h->GetRMSError());
			}
		}
			//Count yields from efficiency numerator
		if(type==2){
			h1->SetBinContent(ix,h->GetBinContent(2));
		}
			//Landau MPV
		if(type==3){
			if(h->GetEntries()>10){
				pair<float,float> mpv_and_err = GetMPV(h);
				if(mpv_and_err.first < 15 || mpv_and_err.first > 100 || mpv_and_err.second/mpv_and_err.first > 0.2) continue;
				h1->SetBinContent(ix,mpv_and_err.first);
				h1->SetBinError(ix,mpv_and_err.second);
			}
		}
			//Efficiency calculation
		if(type==4){
			if(h->GetEntries()>10){
				float eff = h->GetBinContent(2)/h->GetEntries();
				float unc = sqrt(eff*(1-eff)*h->GetEntries())/h->GetEntries();
				h1->SetBinContent(ix,eff);
				h1->SetBinError(ix,unc);
			}
		}

			h->Delete();
		}
	
}

void ConvertMap(TH3F * h3, TH2F * h2, int type){
	for(uint ix=0;ix<h3->GetNbinsX();ix++){
		for(uint iy=0;iy<h3->GetNbinsY();iy++){
			TH1F * h = (TH1F *) h3->ProjectionZ("temp",ix,ix,iy,iy);
			if(type==0){
				if(h->GetEntries() > 10){
					h2->SetBinContent(ix,iy,h->GetMean());
					h2->SetBinError(ix,iy,h->GetMeanError());
				}
			}
			if(type==1){
				if(h->GetEntries() > 10){
					h2->SetBinContent(ix,iy,h->GetRMS());
					h2->SetBinError(ix,iy,h->GetRMSError());
				}
			}
			if(type==2){
				h2->SetBinContent(ix,iy,h->GetBinContent(2));
			}
			//Landau MPV
			if(type==3){
				if(h->GetEntries()>10){
					pair<float,float> mpv_and_err = GetMPV(h);
					if(mpv_and_err.first < 15 || mpv_and_err.first > 100 || mpv_and_err.second/mpv_and_err.first > 0.2) continue;
					h2->SetBinContent(ix,iy,mpv_and_err.first);
					h2->SetBinError(ix,iy,mpv_and_err.second);
				}
			}
			//Efficiency calculation
			if(type==4){
				if(h->GetEntries()>10){
					float eff = h->GetBinContent(2)/h->GetEntries();
					float unc = sqrt(eff*(1-eff)*h->GetEntries())/h->GetEntries();
					h2->SetBinContent(ix,iy,eff);
					h2->SetBinError(ix,iy,unc);
				}
			}

			h->Delete();
		}
	}
}

pair<float,float> GetMPV(TH1F * h){

	TF1* f1  = new TF1("f1","landau",15,100);
	h->Fit("f1","RQ");
	float mpv = f1->GetParameter(1);
	float e_mpv = f1->GetParError(1);
	float chi2pdf = f1->GetChisquare()/f1->GetNDF();
	f1->Delete();

	return pair<float,float> {mpv,e_mpv};

}

pair<int,int> nLGADHitsAndChannel(){
	int nhits=0;
	int ch=-1;
	for(int j=0;j<nlgad;j++){
		if(chidx[j]>=0){
			if(amp[chidx[j]] > hitThres && risetime[chidx[j]] !=0 /*risetime[chidx[j]] > 0.1*/){ nhits++; ch=j;} 
		}
	}
	return pair<int,int> {nhits,ch};
}

bool GoodPhotekHit(){
	//vector<TString> ptk_sel = {"((amp[4]>10&&amp[4]<50&&run<1766)||(amp[0]>150&&amp[0]<200&&run>=1766))","(amp[4]>10&&amp[4]<50&&run<1766)","(amp[0]>150&&amp[0]<200&&run>=1766)","(amp[27]>40&&amp[27]<120&&run<1766)"};

	if(run>=1766 && (amp[0]>150&&amp[0]<200)) return true;
	else if(run<1766 && (amp[4]>10 && amp[4]<50)) return true;
	else return false;

}

void InitBranches(){


  t->SetBranchStatus("*", 0);
  t->SetBranchStatus("amp", 1);
  // t->SetBranchStatus("x_dut", 1);
  // t->SetBranchStatus("y_dut", 1);
  t->SetBranchStatus("y_corr", 1);
  t->SetBranchStatus("x_corr", 1);
  t->SetBranchStatus("ntracks", 1);
  t->SetBranchStatus("run", 1);
  t->SetBranchStatus("ptkidx", 1);
  t->SetBranchStatus("chidx", 1);
  t->SetBranchStatus("risetime", 1);
	t->SetBranchStatus("LP2_30", 1);	
	t->SetBranchStatus("gaus_mean", 1);
	t->SetBranchStatus("chi2", 1);


    t->SetBranchAddress("amp", &amp);
	t->SetBranchAddress("risetime", &risetime);
    t->SetBranchAddress("LP2_30", &LP2_30);	
    t->SetBranchAddress("gaus_mean", &gaus_mean);   
    // t->SetBranchAddress("y_dut", &y_dut);    
    // t->SetBranchAddress("x_dut", &x_dut);   	
    t->SetBranchAddress("x_corr", &x_corr);   	
    t->SetBranchAddress("y_corr", &y_corr);   	
   	t->SetBranchAddress("chi2", &chi2);
	t->SetBranchAddress("ntracks", &ntracks);
	t->SetBranchAddress("run", &run);
	t->SetBranchAddress("ptkidx", &ptkidx);
	t->SetBranchAddress("chidx", &chidx);

}

# ifndef __CINT__
int main(int argc, char **argv)
{
	
    if (argc == 3) plot_maps(argv[1], argv[2]);
    else cout << "Please give data path and tag for output name." << endl;

}
# endif

