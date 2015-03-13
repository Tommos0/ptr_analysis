#ifndef __THITCOLLECTION_H__
#define __THITCOLLECTION_H__
#include "TObjArray.h"
#include "THit.h"

class THitCollection : public TObjArray {
public:
    THitCollection();
    virtual ~THitCollection();
    virtual void Add(TObject *obj);
    Int_t MaxY(); 
    Int_t MinY(); 
    THit* GetHit(Int_t i);
    void CutZ(Int_t minz, Int_t maxz);
    void CutY(Int_t miny, Int_t maxy);
    void CutYZ(Int_t miny, Int_t maxy, Int_t minz, Int_t maxz);
    ClassDef(THitCollection,1)
};
#endif //__THITCOLLECTION_H__
