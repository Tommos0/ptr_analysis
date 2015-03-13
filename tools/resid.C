#include "TGraphErrors.h"
#include "TCanvas.h"
#include "TGraph.h"
#include "TAxis.h"
#include "TF1.h"
#include <iostream>
//#include "THitCollection.h"
TH2F *dist;
TH2F *distz;
void resid() {
    dist = new TH2F("residual","residual",500,0,32,500,-2,2);
    distz  = new TH2F("residualz","residualz",500,0,32,500,-8,8);

    Double_t vdrift = 2.0; //(cm/us)
    Double_t zmax = 30.; //z at anode
    Double_t zstart = 20.;
    
    Double_t dtrans = 201.480; //transverse diffusion um * cm**-0.5 
    Double_t dlong = 165.751; //longitudinal diffusion
    gInterpreter->GenerateDictionary("vector<THitCollection *>","THitCollection.h;vector");
    //gROOT->Reset();
    gROOT->ProcessLine(".L THit.cxx+");
    gROOT->ProcessLine(".L THitCollection.cxx+");
    gROOT->ProcessLine(".L TRawFile.cxx+");
    gROOT->ProcessLine(".L TTrack.cxx+");
    gROOT->ProcessLine(".L TClusterFinder.cxx+");
    const Int_t nbins=100;
    TString run = "run3";
//    char* rawfiles[10] = { "run14","run13","run10","run15","run16","run17","run18","run19","run20","run21"};
    char* rawfiles[5] = { "run12","run13","run14","run15","run16"};
    //char* rawfiles[2] = { "run20","run21"};

    for (int k=0;k<5;k++) {
        cout << "processing " << rawfiles[k] << endl;
        TString rawfilename,fitfilename;
       // rawfilename.Form("/data/detrd/ProtonTherapy/testbeam_KVI_mei2014/data/%s.root",rawfiles[k]);
        rawfilename.Form("/data/testbeam_KVI_mei2014/data/%s.root",rawfiles[k]);
        cout << rawfilename << endl;
        fitfilename.Form("%s_processed.root",rawfiles[k]);


        TRawFile *traw = new TRawFile(rawfilename,"data");
        TFile *ffit = new TFile(fitfilename);
        TTree *tfit = ffit->Get("data");
        UInt_t id;
        TTrack *track;
        tfit->SetBranchAddress("id",&id);
        tfit->SetBranchAddress("track2",&track);
   //     TH1F *resids = new TH1F("residuals","residuals",100,-10,10);
   //     TProfile *residy = new TProfile("residualsy","residualsy",256,0,14,-2,2);
   //     TProfile2D *prof2 = new TProfile2D("2dprofile","2dprofile",100,0,28,50,0,14,-2,2);
        for (Int_t i=0;i<tfit->GetEntries()-1;i++) {
//            cout << "track " << i << " / " << tfit->GetEntries() << endl;
            tfit->GetEntry(i);
            if (track->a != track->a) { continue; }
            
            traw->Select(id);
            THitCollection *c1hits = traw->GetCam2Hits();

            //c1hits->CutZ(0,32);
            c1hits->CutZ(0,280);
            c1hits->Sort();
//            TClusterFinder *cf1 = new TClusterFinder(c1hits);
//            cf1->FindByX(15,4,100);
//            std::vector<THitCollection*> clusters1 = cf1->GetClusters();
//            THitCollection *hc = clusters1[0];
            for (Int_t j=0;j<c1hits->GetEntries();j++) {
                //if (j>10) exit();
                THit *h = c1hits->GetHit(j);
		h->SetPars(vdrift,zmax,zstart,dtrans,dlong);
                Double_t x=h->xmm();
                Double_t y=h->ymm();
                Double_t z=h->zmm(); 
                //Double_t residual = x - track->b - track->a * y;
                //Double_t residual = x-track->a*y-track->b+track->a*track->ycogxy;
                //cout << residual;
                Double_t cogx = track->b;
                Double_t cogy = track->ycogxy;
                Double_t a = track->a;
                Double_t s = (1./(a*a+1))* ( (cogx-x)*a + (cogy-y));
                Double_t dx = TMath::Sqrt( (cogx-x-s*a)**2 + (cogy-y-s)**2  );
                if (cogx-x-s*a > 0) { dx = -dx; }
                Double_t residual = dx;
    //            cout << j << " " << residual << endl;
                Double_t sigx = h->xerr();
                //sigz = TMath::Sqrt(( vdrift*vdrift/12. + z * dlong * dlong * 1e-7));
                Double_t error = TMath::Sqrt((TMath::Power(sigx,2) + track->a*track->a*TMath::Power(sigx,2)));
     //           resids->Fill(residual);
                dist->Fill(track->d,residual);
		distz->Fill(track->d,z-track->d);
//		cout << z-track->d << endl;
      //          prof2->Fill(track->b,y,residual,1);
       //         if ((track->b > 26) && (track->b>16)) { residy->Fill(y,residual,1); } 
    //            Double_t residual = ()
                
            }
            //delete hc;
            //delete cf1;
         //   delete h;
         //   delete c1hits;
            //delete clusters1;
        }
        //delete track;
        //delete traw;
        //delete tfit;
       // delete ffit;

    }
/*
    TCanvas *c5 = new TCanvas("c5","c5");
    dist->Draw();
    TH1D *slice = dist->ProjectionY("slice",100,110);
    TCanvas *c6 = new TCanvas("c6","c6");
    slice->Draw();
	
}*/
   TCanvas *c8 = new TCanvas("c8","x residual vs z");
   dist->Draw();
   TCanvas *c6 = new TCanvas("c6","z residual vs z");
   distz->Draw();
   TCanvas *c7 = new TCanvas("c7","dummy");



    TGraphErrors *sigmas = new TGraphErrors(50);
    TGraphErrors *sigmasz = new TGraphErrors(50);
    for (Int_t i=0;i<49;i++) {
        TH1D *slice = dist->ProjectionY("slice",i*10,(i+1)*10-1);
//        cout << i << " " << slice->GetEntries() << endl;
        if (slice->GetEntries()>0) {
            slice->Fit("gaus","q");
	        TF1 *fit = slice->GetFunction("gaus");
		sigmas->SetPointError(i,0,5/(TMath::Sqrt(slice->GetEntries())));
	        sigmas->SetPoint(i,(i+.5)*32./50.,fit->GetParameter(2));
        }

        TH1D *slicez = distz->ProjectionY("slicez",i*10,(i+1)*10-1);

	if (i==47) {
		TCanvas *c4 = new TCanvas("c4","c4");
        	TH1D *kslice = distz->ProjectionY("kslice",100,109);
		kslice->Draw();
		
		TCanvas *c5 = new TCanvas("c5","c5");
	}
	
        if (slicez->GetEntries()>0) {
            slicez->Fit("gaus","q");
	        TF1 *fitz = slicez->GetFunction("gaus");
		sigmasz->SetPointError(i,0,5/(TMath::Sqrt(slicez->GetEntries())));
	        sigmasz->SetPoint(i,(i+.5)*32./50.,fitz->GetParameter(2));
        }
//        if (i>32) break;
    }
//    sigmas->SetMarkerStyle(20);
//    sigmas->SetMarkerSize(2);
    TCanvas *c1 = new TCanvas("c1","Sigma_x vs z");
    sigmas->Draw("ap");   
    TCanvas *c2 = new TCanvas("c2","Sigma_z vs z");
    sigmasz->Draw("ap");
    //TCanvas *c1 = new TCanvas("c1","c1");
//    c1->Divide(1,2);
//    c1->cd(1);
//    resids->Draw();
//    c1->cd(2);
   // dist->Draw();
//    residy->Draw();
//    TCanvas *c2 = new TCanvas("c2","c2");
//    prof2->Draw("LEGO2 0");

}


