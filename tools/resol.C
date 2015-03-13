#include "TGraphErrors.h"
#include "TCanvas.h"
#include "TGraph.h"
#include "TAxis.h"
#include "TF1.h"
#include <iostream>
//#include "THitCollection.h"

TH2D *errs = new TH2D("errs","errs",100,0,32,100,0,0.2);
TH2D *errs2 = new TH2D("errs2","errs2",100,0,32,100,0,0.2);

void resol() {
    const double theta1=0.1347;
    const double theta2=0.1308;
    gInterpreter->GenerateDictionary("vector<THitCollection *>","THitCollection.h;vector");
    //gROOT->Reset();
    gROOT->ProcessLine(".L THit.cxx+");
    gROOT->ProcessLine(".L THitCollection.cxx+");
    gROOT->ProcessLine(".L TRawFile.cxx+");
    gROOT->ProcessLine(".L TTrack.cxx+");
    gROOT->ProcessLine(".L TClusterFinder.cxx+");
    //char* rawfiles[11] = { "run1","run2","run3","run4","run5","run6","run7","run8","run9","run10","run42"};
    char* rawfiles[5] = { "run12","run13","run14","run15","run16"};
    //char* rawfiles[2] = { "run20","run21"};
    TChain *c = new TChain("chain");
    TH2D *dist1 = new TH2D("xyres","xyres",500,0,32,100,-0.25,0.25);
    TH2D *dist2 = new TH2D("xyres2","xyres2",500,0,32,100,-0.25,0.25);
    for (int k=0;k<5;k++) {
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

//            cout << track1->a - track2->a << endl;
            //dist->Fill(track1->d,track1->a);
            //dist->Fill(track1->d,(track1->a-track2->a)/2);
            
            double x1 = cos(theta1)*track1->b - sin(theta1)*track1->ycogxy;
            double x2 = cos(theta2)*track2->b - sin(theta2)*track2->ycogxy;
            x2-=1.471;

            double y1 = sin(theta1)*track1->b + cos(theta1)*track1->ycogxy;
            double y2 = sin(theta2)*track2->b + cos(theta2)*track2->ycogxy;

            y2+=229.;

            double angle=atan((x2-x1)/(y2-y1));
            dist1->Fill(track1->d,angle-atan(track1->a)+theta1);
            dist2->Fill(track2->d,angle-atan(track2->a)+theta2);
            errs->Fill(track1->d,track1->aErr*track1->aErr);
            errs2->Fill(track2->d,track2->aErr);
        }
    //(dist->ProfileX())->Draw("prof");   
    TGraphErrors *sigmas1 = new TGraphErrors(50);
    TGraphErrors *sigmas2 = new TGraphErrors(50);
    for (Int_t i=0;i<49;i++) {
        TH1D *slice1 = dist1->ProjectionY("slice1",i*10,(i+1)*10-1);
        TH1D *slice2 = dist2->ProjectionY("slice2",i*10,(i+1)*10-1);
//        cout << i << " " << slice->GetEntries() << endl;
//        cout << i << " " << slice->GetEntries() << endl;
//        if (i==1) {
//            slice->Draw(); TCanvas *c2 = new TCanvas("c2"); break;
//        }
        if (slice1->GetEntries()>1) {
            slice1->Fit("gaus","q");
	        TF1 *fit = slice1->GetFunction("gaus");
		sigmas1->SetPointError(i,0,0.04/(TMath::Sqrt(slice1->GetEntries())));
	        sigmas1->SetPoint(i,(i+.5)*32./50.,sqrt(pow(fit->GetParameter(2),2) - 0.007*0.007));
        }

        if (slice2->GetEntries()>1) {
            slice2->Fit("gaus","q");
	        TF1 *fit = slice2->GetFunction("gaus");
		sigmas2->SetPointError(i,0,0.04/(TMath::Sqrt(slice2->GetEntries())));
	        sigmas2->SetPoint(i,(i+.5)*32./50.,sqrt(pow(fit->GetParameter(2),2) - 0.007*0.007));
        }

    }
    TF1 *tf1 = new TF1("t1","12*pow(0.046+0.0552*sqrt(x),2)/((70-0.84*x)*10*10)",0,30);
    TF1 *tf2 = new TF1("t2","12*pow(0.0197+0.0661*sqrt(x),2)/((90-2*x)*10*10)",0,30);

    sigmas1->Draw("ap");
    tf1->Draw("same");
    TProfile *pro = errs->ProfileX();
    pro->Draw("same");
    TCanvas *c2 = new TCanvas("c2");
    sigmas2->Draw("ap");
    tf2->Draw("same");
    TProfile *pro2 = errs2->ProfileX();
    pro2->Draw("same");
}


