#include <vector>
#include "TMath.h"
int process(TString filename, TString treename, TString outFilename) {
    gInterpreter->GenerateDictionary("vector<THitCollection *>","THitCollection.h;vector");
    gROOT->Reset();
    gROOT->ProcessLine(".L THit.cxx+");
    gROOT->ProcessLine(".L THitCollection.cxx+");
    gROOT->ProcessLine(".L TRawFile.cxx+");
    gROOT->ProcessLine(".L TViewer.cxx+");
    gROOT->ProcessLine(".L TClusterFinder.cxx+");
    gROOT->ProcessLine(".L TTrack.cxx+");
    gROOT->ProcessLine(".L TFit.cxx+");
    Int_t rejectX = 0;
    Int_t rejectErr = 0;
    Int_t rejectHits = 0;
    Int_t noTrack1 = 0;
    Int_t noTrack2 = 0;
    Int_t multiTrack1 = 0;
    Int_t multiTrack2 = 0;

    TViewer *tv = new TViewer();
    //TGraph2D *tf = new TGraph2D(1);
    TRawFile *f = new TRawFile(filename,treename);
    TTree *tree = f->GetTree();
    
    TFile* outFile = new TFile(outFilename, "RECREATE");
    TTree* outTree = new TTree(treename,treename);
    Double_t energy;
    UInt_t id;
    TTrack *track1;
    TTrack *track2;
    outTree->Branch("id",&id,"id/i");
    outTree->Branch("energy",&energy,"energy/D");
    outTree->Branch("track1","TTrack",&track1);
    outTree->Branch("track2","TTrack",&track2);
    for (Int_t i=0;i<f->GetEntries();i++) {
        f->Select(i);
        id = i;
        energy = f->GetEnergy();
        THitCollection *c1hits = f->GetCam1Hits();
        THitCollection *c2hits = f->GetCam2Hits();
        c1hits -> SetOwner(true);
        c2hits -> SetOwner(true);
        c1hits->CutZ(0,280);
        //c1hits->CutYZ(50,230,0,21);
        c2hits->CutZ(20,280);
        TClusterFinder *cf1 = new TClusterFinder(c1hits);
        TClusterFinder *cf2 = new TClusterFinder(c2hits);
        cf1->FindByX(15,4,100);
        cf2->FindByX(15,4,100);
        std::vector<THitCollection*> clusters1 = cf1->GetClusters();
        std::vector<THitCollection*> clusters2 = cf2->GetClusters();
        if (clusters1.size()>1) multiTrack1++;
        if (clusters2.size()>1) multiTrack2++;
        if (clusters1.size()==0) noTrack1++; 
        if (clusters2.size()==0) {
            noTrack2++;
            //cout << i << " no track2, but " << c2hits->GetEntries() << " hits " << endl;
        }
        if (clusters1.size()==1 && clusters2.size()==1) {
            TFit *fit1 = new TFit(clusters1[0]);
            TFit *fit2 = new TFit(clusters2[0]);
            fit1->vdrift = 2.0; //cm / micro sec
            fit1->zmax = 30.; //drift height in mm
            fit1->zstart = 40.; //start of z in bins (@ cathode)
            fit1->dtrans = 201.480; //micro m / sqrt cm
            fit1->dlong = 165.751;
            fit1->xErrp1 = 0.0456;
            fit1->xErrp2 = 0.0552;
            fit1->zErrp1 = 0.545;
            fit1->zErrp2 = 0.0410;

            fit2->vdrift = 1.7; //cm / micro sec
            fit2->zmax = 30.; //drift height in mm
            fit2->zstart = 20.; //start of z in bins (@ cathode)
            fit2->dtrans = 201.480; //micro m / sqrt cm
            fit2->dlong = 165.751;
            fit2->xErrp1 = 0.0197;
            fit2->xErrp2 = 0.0661;
            fit2->zErrp1 = 0.501;
            fit2->zErrp2 = 0.0450;

            //track1 = fit1->FitTGraph();
            //track2 = fit2->FitTGraph();
            track1 = fit1->FitYork();
            track2 = fit2->FitYork();
            if (track2->a != track2->a) { cout << "track2.a nan" << endl; }
            if /* (track1->chi2yx / track1->nhits > 4) ||
               (track2->chi2yx / track2->nhits > 4) ||
               (abs(track1->a)>1) ||
               (abs(track1->c)>1) ||
               (abs(track2->a)>1) ||
               (abs(track2->c)>1)
               )*/
            (false) { }
            /* 
                ((track1->aErr>0.1) || (track2->aErr>0.1) ||
                (track1->cErr>0.2) || (track2->cErr>0.2))
                { rejectErr++; }

            */
            else {
                outTree->Fill();
                if (i==52) {
                    cout << "123123" << endl;
                 //   tv->AddHits(clusters1[0],kBlue);
                 //   tv->AddTrack(track1,1);
                }
            }
            delete track1; 
            delete track2;
            delete fit1;
            delete fit2;
        }
        else {
            rejectX++;
        }
      //  delete clusters1;
       // delete clusters2;
        delete cf1;
        delete cf2;
        delete c1hits;
        delete c2hits;
        if (i>0 && i%500==0) {
            cout << "progress: " << i << "/" << f->GetEntries() << endl;
            outFile->Write();
        }
    }
    outFile->Write();
    outFile->Close();
        //f->Delete(); //messes up stuff somehow
    cout << "done.. Rejected: " << endl;
    cout << "No Track (2): " << noTrack2 << endl;
    cout << "No Track (1): " << noTrack1 << endl;
    cout << "Multitrack (2): " << multiTrack2 << endl;
    cout << "Multitrack (1): " << multiTrack1 << endl;
    cout << "Rejected on error: " << rejectErr << endl;
    return 0;
}
