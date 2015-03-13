#ifndef __TVIEWER_H__
#define __TVIEWER_H__
#include "TObject.h"
#include "THitCollection.h"
#include "TCanvas.h"
#include "TGraph2D.h"
#include "TTrack.h"
class TViewer : public TObject {
private:
    TGraph2D *g1;
    TGraph2D *ghits;
public:
    TCanvas *c0;
    TViewer();
    TViewer(TString id, TString name);
    ~TViewer();

    void AddHits(THitCollection *h,Int_t color);
    void AddTrack(TTrack *track,Int_t color);
    ClassDef(TViewer,1)
};

#endif //__TVIEWER_H__
