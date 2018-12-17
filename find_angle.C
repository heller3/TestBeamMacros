#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <iomanip> // for setw()
#include <time.h>
//#include <sys/stat.h>
//#include <sys/types.h>


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


/*void find_center_of_valley(TH1F * h, float xmin, float xmax){
  slices[i]->GetXaxis()->SetRangeUser(27,29);
  float this_y_min = slices[i]->GetBinCenter(slices[i]->GetMinimumBin());
  
  
  // valley->SetBinContent(this_x_bin,this_y_min);

}
*/
void find_angle(){
TChain * chain = new TChain("pulse");
 vector<float> xvals ={-1.7,-1.4,-1,-0.7,-0.2,0.4,0.7,1,1.4,1.75,2.4,2.7,3,3.3,3.6,3.8,4.6,5.0,5.2,5.8};

chain->Add("/eos/uscms/store/group/cmstestbeam/BTL_ETL/2018_11/data/DT5742/RECO/v6/DT5742_Run176[6-9]*bv360.root");
chain->Add("/eos/uscms/store/group/cmstestbeam/BTL_ETL/2018_11/data/DT5742/RECO/v6/DT5742_Run17[7-9]*bv360.root");

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

