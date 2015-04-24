#ifndef __TTRACK_H__
#define __TTRACK_H__
#include "TObject.h"
class TTrack:public TObject {
public:
    Double_t a;//slope y,x

    Double_t b;//x center of gravity
    Double_t c;//slope y,z
    Double_t d;//z center of gravity
    Double_t ycogxy; //y cog xy plane
    Double_t ycogzy; //y cog zy plane
    Double_t aErr;
    Double_t aErrBS;
    Double_t bErr;
    Double_t cErr;
    Double_t cErrBS;
    Double_t dErr;
    Double_t chi2yx;
    Double_t chi2yz;
    Int_t nhits; 
    TTrack();
    TTrack(Double_t a1,Double_t b1,Double_t c1,Double_t d1);
    TTrack(Double_t a1, Double_t errA, Double_t b1, Double_t errB, Double_t c1, Double_t errC, Double_t d1, Double_t errD);
    virtual ~TTrack();



    ClassDef(TTrack,1);

};
#endif //__TTRACK_H__
