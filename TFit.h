#ifndef __TFIT_H__
#define __TFIT_H__
#include "TObject.h"
#include "THitCollection.h"
#include "TCanvas.h"
#include "TTrack.h"
#include "TMath.h"

class TFit : public TObject {
private:
    THitCollection *hits;
    TCanvas *c1;
    Double_t chi2yx;
    Double_t chi2yz;
    void init();
public:
    Double_t vdrift;
    Double_t zmax;
    Double_t zstart;
    Double_t dtrans;
    Double_t dlong;
    Double_t xErrp1;
    Double_t xErrp2;
    Double_t zErrp1;
    Double_t zErrp2;
    TFit();
    TFit(THitCollection *hc);
    virtual ~TFit();
    TTrack * FitTGraph();
    TTrack * FitYork();
    void YorkMethod(Int_t , Double_t *, Double_t *, Double_t *, Double_t *, Double_t *, Double_t *, Double_t *, Double_t *,Double_t *);
    ClassDef(TFit,1);
};

#endif //__TFIT_H__
