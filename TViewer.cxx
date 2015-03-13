#include "TViewer.h"
#include "THit.h"
#include "TGraph2D.h"
#include "TString.h"
#include "TPolyLine3D.h"
#include "TTrack.h"
#include <iostream>
ClassImp(TViewer);
TViewer::TViewer() { TViewer("v1", "TViewer"); } 
TViewer::TViewer(TString id,TString name) {
    c0 = new TCanvas(id,name,200,10,700,500);
    g1 = new TGraph2D(3);
   g1->SetName("g1");
    g1->SetPoint(0,0,0,0);
    g1->SetPoint(1,30,15,32);
     g1->SetMarkerStyle(20);
     g1->SetMarkerColor(kBlue);
     g1->SetMarkerSize(.3);
     g1->Draw("p");
     //c0->Modified(); c0->Update(); 
}
TViewer::~TViewer() {}
void TViewer::AddHits(THitCollection *h,Int_t color) {
     Int_t entries = h->GetEntries();
     if (entries<1) return;
     Double_t *x = new Double_t[entries];
     Double_t *y = new Double_t[entries];
     Double_t *z = new Double_t[entries];
     THit* hit;
     for (Int_t i=0;i<entries;i++){
       hit = (THit *)h->At(i);
        x[i] = hit->xmm();
        y[i] = hit->ymm();
        z[i] = hit->zmm();
     }
     ghits = new TGraph2D(entries,x,y,z);

     ghits->SetMarkerStyle(20);
     ghits->SetMarkerSize(0.6);
     ghits->SetMarkerColor(color);
     ghits->Draw("p same");
     TGraph2D *test = new TGraph2D(1);
     test->SetPoint(0,20,20,20);
     test->Draw("p same"); //doesn't draw without the test thing...

}
void TViewer::AddTrack(TTrack *track, Int_t color) {
    TPolyLine3D *l = new TPolyLine3D(2);
    l->SetPoint(0,track->b-track->a*track->ycogxy,0,track->d-track->c*track->ycogzy);
    l->SetPoint(1,15*track->a + track->b-track->a*track->ycogxy,15,15*track->c + track->d-track->c*track->ycogzy);
    l->SetLineColor(color);
    l->SetLineWidth(1);
    l->Draw("same");
}
