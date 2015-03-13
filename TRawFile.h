#ifndef __TRAWFILE_H__
#define __TRAWFILE_H__

#include "TObject.h"
#include "THitCollection.h"
#include "TTree.h"
#include "TFile.h"

class TRawFile : public TObject {
private:
    TFile * f;
    TTree * t;
    unsigned short calo_data[1602];
    unsigned short x1[512*256];
    unsigned short y1[512*256];
    unsigned short z1[512*256];
    unsigned short x2[512*256];
    unsigned short y2[512*256];
    unsigned short z2[512*256];
    unsigned int eventhits1;
    unsigned int eventhits2;
    int max(unsigned short * const data, int const length); 
    int avg(unsigned short * const data, int const length);
    Double_t caltoenergy(int calvalue);
public:
    TRawFile();
    TRawFile(TString filename, TString treename);
    ~TRawFile();
    void Select(Int_t eventnr);
    TTree *GetTree();
    Int_t GetEntries();
    Double_t GetEnergy(); 
    THitCollection *GetCam1Hits();
    THitCollection *GetCam2Hits();
    ClassDef(TRawFile, 1);
};
#endif //__TRAWFILE_H__
