#include "TGraphErrors.h"
#include "TCanvas.h"
#include "TGraph.h"
#include "TAxis.h"
#include "TF1.h"
#include <iostream>
//#include "THitCollection.h"
void resolz() {
    gInterpreter->GenerateDictionary("vector<THitCollection *>","THitCollection.h;vector");
    //gROOT->Reset();
    gROOT->ProcessLine(".L THit.cxx+");
    gROOT->ProcessLine(".L THitCollection.cxx+");
    gROOT->ProcessLine(".L TRawFile.cxx+");
    gROOT->ProcessLine(".L TTrack.cxx+");
    gROOT->ProcessLine(".L TClusterFinder.cxx+");
    char* rawfiles[11] = { "run1","run2","run3","run4","run5","run6","run7","run8","run9","run10","run42"};
    //char* rawfiles[2] = { "run20","run21"};
    TChain *c = new TChain("chain");
    TH2D *dist = new TH2D("zvres","zvres",500,0,32,100,-1,1);
    for (int k=0;k<11;k++) {
        TString fitfilename;
        fitfilename.Form("%s_processed.root/data",rawfiles[k]);
        c->Add(fitfilename);
    }
    cout << c->GetEntries() << endl;
        TTrack *track1,*track2;
//        c->SetBranchAddress("id",&id);
        c->SetBranchAddress("track1",&track1);
        c->SetBranchAddress("track2",&track2);
        for (Int_t i=0;i<c->GetEntries()-1;i++) {
            c->GetEntry(i);
//            cout << track1->c - track2->c << endl;
            //dist->Fill(track1->d,track1->a);
            dist->Fill(track1->d,(track1->c-track2->c)/2.);
        }
    //(dist->ProfileX())->Draw("prof");   
    TGraphErrors *sigmas = new TGraphErrors(50);
    for (Int_t i=0;i<49;i++) {
        TH1D *slice = dist->ProjectionY("slice",i*10,(i+1)*10-1);
//        if (i==22) {slice->Draw(); break; }
//        cout << i << " " << slice->GetEntries() << endl;
        cout << i << " " << slice->GetEntries() << endl;
        if (slice->GetEntries()>1) {
            slice->Fit("gaus","q");
	        TF1 *fit = slice->GetFunction("gaus");
		sigmas->SetPointError(i,0,0.04/(TMath::Sqrt(slice->GetEntries())));
	        sigmas->SetPoint(i,(i+.5)*32./50.,fit->GetParameter(2));
        }

    }
    TF1 *tf1 = new TF1("t1","sqrt(12*pow(0.549+0.04*sqrt(x),2)/((70-0.84*x)*10*10))",0,30);

    sigmas->Draw("ap");
    tf1->Draw("same");
}


