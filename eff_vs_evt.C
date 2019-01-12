{
  gROOT->Reset();
  TChain c("pulse");

  c.Add("/Users/rheller/cernbox/ETL/tb/W6_2x8_bv360/DT*.root");
//  c.Add("/eos/uscms/store/group/cmstestbeam/BTL_ETL/2018_11/data/DT5742/RECO/v6/backup/DT5742_Run177*bv360.root");
  cout<<"Loaded "<<c.GetEntries()<<" events"<<endl;
  int run=1782;  
  cout<<"Entries this run "<<c.GetEntries(Form("run==%i",run))<<endl;
  int i=9; 
 
  TString den = Form("run==%i&&chidx[%i]>=0 &&ntracks==1 &&chi2<3 && (x_corr)>5.3 && (x_corr) < 6.5  && y_corr > 19.2 && y_corr < 20.6 && amp[ptkidx[%i]]>70 && amp[ptkidx[%i]]<300",run,i,i,i);
  TString hit = Form("%s && amp[chidx[%i]]>15 && risetime[chidx[%i]]>0.1",den.Data(),i,i);

  nEvents=15000;
  TH1F * hden = new TH1F("den","",10,0,nEvents);
  TH1F * hnum = new TH1F("num","",10,0,nEvents);
  TH1F * h_dummy = new TH1F("h_dummy","",10,0,nEvents);

  c.Project("den","i_evt",den);
  c.Project("num","i_evt",hit);

  TGraphAsymmErrors* h_eff = new TGraphAsymmErrors();
  h_dummy->Draw();
  h_eff->Divide(hnum,hden,"cl=0.683 b(1,1) mode");    
  h_eff->Draw("EP same");

}
