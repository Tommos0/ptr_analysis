#include "TRawFile.h"
#include "THitCollection.h"
#include "THit.h"
#include <iostream>
#include "TString.h"

ClassImp(TRawFile);

int TRawFile::max(unsigned short * const data, int const length) {
    int maxvalue=data[0];
    for (int i=1;i<length;i++) {
        if (data[i]>maxvalue) maxvalue = data[i];
    }
    return maxvalue;
}
int TRawFile::avg(unsigned short * const data, int const length) {
    int avgvalue=0;
    for (int i=0;i<length;i++)
        avgvalue+=data[i];
    return avgvalue/length;
}
Double_t TRawFile::caltoenergy(int calvalue) {
    Double_t energy = 3.2e-8 * calvalue*calvalue*calvalue - 8.4e-5 * calvalue*calvalue + 0.12 * calvalue - 0.014;
    return energy;
}
TRawFile::TRawFile() { }
TRawFile::TRawFile(TString filename, TString treename) {
    f = new TFile(filename);
    t = (TTree *)f->Get(treename);
    t->SetBranchAddress("cam1_hits",&eventhits1);
    t->SetBranchAddress("cam1_x",x1);
    t->SetBranchAddress("cam1_y",y1);
    t->SetBranchAddress("cam1_z",z1);
    t->SetBranchAddress("cam2_hits",&eventhits2);
    t->SetBranchAddress("cam2_x",x2);
    t->SetBranchAddress("cam2_y",y2);
    t->SetBranchAddress("cam2_z",z2);
    t->SetBranchAddress("calo_data",calo_data);
}
TRawFile::~TRawFile() { 
    f->Close();
    delete f;
}

void TRawFile::Select(Int_t eventnr) {
    t->GetEntry(eventnr);
}
Int_t TRawFile::GetEntries() {
    return t->GetEntries();
}
TTree *TRawFile::GetTree() {
    return t;
}
Double_t TRawFile::GetEnergy() {
   return caltoenergy(max(calo_data,1602)); 
}
THitCollection* TRawFile::GetCam1Hits() {
    THitCollection *hc = new THitCollection();
    for (unsigned int i=0;i<eventhits1;i++) { 
      hc->Add(new THit(x1[i],y1[i],z1[i]));  
    }
    return hc;
}
THitCollection* TRawFile::GetCam2Hits() {
    THitCollection *hc = new THitCollection();
    for (unsigned int i=0;i<eventhits2;i++) 
      hc->Add(new THit(x2[i],y2[i],z2[i]));  
    return hc;
}

