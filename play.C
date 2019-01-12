{
  gROOT->Reset();
  TChain c("pulse");

  c.Add("/Users/rheller/cernbox/ETL/tb/W6_2x8_bv360/DT*.root");
//  c.Add("/eos/uscms/store/group/cmstestbeam/BTL_ETL/2018_11/data/DT5742/RECO/v6/backup/DT5742_Run177*bv360.root");
  cout<<"Loaded "<<c.GetEntries()<<" events"<<endl;
  int i=9; 
  // c.Draw(Form("channel[chidx[%i]]:time[0]",i),Form("amp[chidx[%i]]>15 && amp[ptkidx[%i]]>20 && amp[ptkidx[%i]]<300 &&(y_dut[2]-y0+22)<20",i,i,i),"l");
  // TCanvas c1;
  // c.Draw("y_dut[2]+y0-25:x_dut[2]+x0-40>>h1(100,0,10,100,20,30)",Form("ntracks==1 &&chidx[%i]>0 &&amp[chidx[%i]]>15 && amp[ptkidx[%i]]>20 && amp[ptkidx[%i]]<300",i,i,i,i),"colz");
  // TCanvas c2;
  // c.Dra(x_dut[2]-x0)>-27w("y_dut[2]+y0-25:x_dut[2]+x0-40>>h2(100,0,10,100,20,30)",Form("ntracks==1 &&chidx[%i]>0 &&amp[chidx[%i]]>15 && amp[ptkidx[%i]]>20 && amp[ptkidx[%i]]<300&& risetime[chidx[%i]]>0.1",i,i,i,i,i),"colz");


  TString bad_cut = Form("chidx[%i]>=0 && ntracks ==1 && run!=1776 && LP2_20[chidx[%i]]-LP2_20[ptkidx[%i]] <-2 && amp[chidx[%i]]>15 && risetime[chidx[%i]]>0.1 && amp[ptkidx[%i]]>70 && amp[ptkidx[%i]]<300",i,i,i,i,i,i,i);
  bad_cut += "&& y_corr < 18";

  TString good_cut = Form("chidx[%i]>=0 && ntracks == 1 && run!=1776 && LP2_20[chidx[%i]]-LP2_20[ptkidx[%i]] <-2 && amp[chidx[%i]]>15 && risetime[chidx[%i]]>0.1 && amp[ptkidx[%i]]>70 && amp[ptkidx[%i]]<300",i,i,i,i,i,i,i);
  good_cut += "&& (x_corr)>5.3 && (x_corr) < 6.5  && y_corr > 19.2 && y_corr < 20.6";

  vector<TString> vars;
  vars.push_back(Form("amp[chidx[%i]]",i));
  // vars.push_back(Form("baseline[chidx[%i]]",i));
  // vars.push_back(Form("noise[chidx[%i]]",i));
  vars.push_back(Form("LP2_20[chidx[%i]]",i));
  vars.push_back(Form("LP2_20[ptkidx[%i]]",i));
  vars.push_back(Form("LP2_20[chidx[%i]]-LP2_20[ptkidx[%i]]",i,i));
  vars.push_back(Form("amp[ptkidx[%i]]",i));
  vars.push_back(Form("integral[chidx[%i]]",i));
  vars.push_back(Form("decaytime[chidx[%i]]",i));
  vars.push_back(Form("risetime[chidx[%i]]",i));
  vars.push_back("i_evt");
  vars.push_back("run");
  vars.push_back("xSlope");
  vars.push_back("ySlope");
  //  vars.push_back("ntracks");
  cout<<"N good events "<<c.GetEntries(good_cut)<<endl;
  cout<<"N bad events "<<c.GetEntries(bad_cut)<<endl;
  TCanvas c1;
  c1.Divide(3,4);
  for (int j=0; j<vars.size(); j++){
    c1.cd(j+1);
    c.SetLineWidth(2);
    c.SetLineColor(kRed);
    c.Draw(vars[j],bad_cut,"norm");
    c.SetLineColor(kGreen+1);
    c.Draw(vars[j],good_cut,"same norm");
  }
}
