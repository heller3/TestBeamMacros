#include <iostream>
#include "TFile.h"
#include "TTree.h"
#include <fstream>

using namespace std;

void move_zombies(int run_i, int run_f){
	for (int i=run_i; i < run_f + 1; i++)
	{
		cout<<"Run "<<i<<endl;
		TString root_filename = Form("/eos/uscms/store/user/cmstestbeam/BTL_ETL/2018_11_recover/RECO/v7/DataVMETiming_Run%i.root", i);
		TFile *f = TFile::Open(root_filename);
		if(f){
			if(f->IsZombie()){
				cout<<"Run "<<i<<" is a zombie. Moving it.."<<endl;
				rename(root_filename, "/eos/uscms/store/user/cmstestbeam/BTL_ETL/2018_11_recover/RECO/v7/zombie/DataVMETiming_Run%i.root");
				continue;
			}
			TTree *pulse = (TTree*)f->Get("pulse");
			if(pulse){
				if(f->TestBit(TFile::kRecovered))
				{
					cout<<"Copying recovered tree, run "<<i<<endl;
					TFile * copyfile = new TFile(Form("/eos/uscms/store/user/cmstestbeam/BTL_ETL/2018_11_recover/RECO/v7/recover/DataVMETiming_Run%i.root",i),"recreate");
					int nentries= pulse->GetEntries();
					TTree * out_tree= pulse->CopyTree(Form("i_evt<%i",nentries-2));
					out_tree->Write();
					copyfile->Close();
				}
				f->Close();
			}
		}
	}
}
