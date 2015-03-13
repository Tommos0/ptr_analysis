#ifndef __THIT_H__
#define __THIT_H__
#include "TObject.h"
#include "TROOT.h"
#include "TMath.h"
class THit: public TObject {
private:
    void init();
public:
    Double_t vdrift;
    Double_t zmax;
    Double_t zstart;
    Double_t dtrans;
    Double_t dlong;
    Double_t (*errFuncX)(Double_t);
    Double_t (*errFuncY)(Double_t);
    THit();
    THit(Int_t xvalue, Int_t yvalue, Int_t zvalue);
    void SetPars(Double_t _vdrift,Double_t _zmax,Double_t _zstart,Double_t _dtrans,Double_t _dlong);
    virtual ~THit();

    virtual Int_t Compare(const TObject *obj) const;
    virtual Bool_t IsSortable() const;
    Double_t xmm();
    Double_t ymm();
    Double_t zmm();
    Double_t xerr();
    Double_t yerr();
    Double_t zerr();

    Int_t x;
    Int_t y;
    Int_t z;
    ClassDef(THit, 5) // THit
};
#endif /* __THIT_H__ */
