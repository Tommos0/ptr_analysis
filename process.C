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
    cout << "processing " << filename << endl;
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
            fit1->zErrp1 = 0.55;
            fit1->zErrp2 = 0.0340;

            fit2->vdrift = 1.7; //cm / micro sec
            fit2->zmax = 30.; //drift height in mm
            fit2->zstart = 20.; //start of z in bins (@ cathode)
            fit2->dtrans = 201.480; //micro m / sqrt cm
            fit2->dlong = 165.751;
            fit2->xErrp1 = 0.0197;
            fit2->xErrp2 = 0.0661;
            fit2->zErrp1 = 0.501;
            fit2->zErrp2 = 0.0450;

            track1 = fit1->FitYork();
            track2 = fit2->FitYork();

	    //bootstrap start
		
            for (int j=0;j<200;j++) {
                THitCollection* hcnew = TFit::redraw(clusters1[0]);
		hcnew->SetOwner(kFALSE);
                TFit *bsfit1 = new TFit(hcnew);
                bsfit1->vdrift = 2.0; //cm / micro sec
                bsfit1->zmax = 30.; //drift height in mm
                bsfit1->zstart = 40.; //start of z in bins (@ cathode)
                bsfit1->dtrans = 201.480; //micro m / sqrt cm
                bsfit1->dlong = 165.751;
                bsfit1->xErrp1 = 0.0456;
                bsfit1->xErrp2 = 0.0552;
                bsfit1->zErrp1 = 0.55;
                bsfit1->zErrp2 = 0.0340;
                TTrack *bstrack1 = bsfit1->FitYork();
                track1->aErrBS+=(bstrack1->a - track1->a)*(bstrack1->a - track1->a);
                track1->cErrBS+=(bstrack1->c - track1->c)*(bstrack1->c - track1->c);
                delete bstrack1; 
                delete bsfit1;
		delete hcnew;
            }
            track1->aErrBS = sqrt(track1->aErrBS/200.);
            track1->cErrBS = sqrt(track1->cErrBS/200.);

            for (int j=0;j<200;j++) {
                THitCollection* hcnew = TFit::redraw(clusters2[0]);
		hcnew->SetOwner(kFALSE);
                TFit *bsfit1 = new TFit(hcnew);
                bsfit1->vdrift = 1.7; //cm / micro sec
                bsfit1->zmax = 30.; //drift height in mm
                bsfit1->zstart = 20.; //start of z in bins (@ cathode)
                bsfit1->dtrans = 201.480; //micro m / sqrt cm
                bsfit1->dlong = 165.751;
                bsfit1->xErrp1 = 0.0197;
                bsfit1->xErrp2 = 0.0661;
                bsfit1->zErrp1 = 0.501;
                bsfit1->zErrp2 = 0.045;
                TTrack *bstrack1 = bsfit1->FitYork();
                track2->aErrBS+=(bstrack1->a - track2->a)*(bstrack1->a - track2->a);
                track2->cErrBS+=(bstrack1->c - track2->c)*(bstrack1->c - track2->c);
                delete bstrack1; 
                delete bsfit1;
		delete hcnew;
            }
            track2->aErrBS = sqrt(track2->aErrBS/200.);
            track2->cErrBS = sqrt(track2->cErrBS/200.);


	    //bootstrap end

	    outTree->Fill();
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
