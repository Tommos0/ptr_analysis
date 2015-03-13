/* THitCollection
 * Collection of THits, extends ROOT's TObjArray
 * Methods for cutting on min/max Z and Y
 */

#include "THitCollection.h"
#include "THit.h"
#include <iostream>
#include "TString.h"

ClassImp(THitCollection);
THitCollection::THitCollection() : TObjArray() { }
THitCollection::~THitCollection() {
}
void THitCollection::Add(TObject *obj) {
    if (TString(obj->ClassName()).EqualTo("THit")) {
        AddLast(obj);
    }
}
THit * THitCollection::GetHit(Int_t i) {
    return (THit*)At(i);
}

Int_t THitCollection::MaxY() {
    Int_t maxy = 0;
    for (Int_t i=0;i<GetEntriesFast();i++) {
        Int_t y=((THit *)At(i))->y;
        if (y > maxy) maxy=y;
    }
    return maxy;
}
Int_t THitCollection::MinY() {
    Int_t miny = ((THit *)At(0))->y;
    for (Int_t i=0;i<GetEntriesFast();i++) {
        Int_t y=((THit *)At(i))->y;
        if (y < miny) miny=y;
    }
    return miny;
}
void THitCollection::CutZ(Int_t minz, Int_t maxz) {
    for (Int_t i=0;i<GetEntriesFast();i++) {
        THit *hit = (THit *)At(i);
        Int_t z = hit->z;
        if (z<minz || z>maxz) {
            Remove(hit);
        }   

    }
    
}
void THitCollection::CutY(Int_t miny, Int_t maxy) {
    for (Int_t i=0;i<GetEntriesFast();i++) {
        Int_t y = ((THit *)At(i))->y;
        if (y<miny || y>maxy) {
            RemoveAt(i);
        }   

    }
    
}
void THitCollection::CutYZ(Int_t miny, Int_t maxy, Int_t minz, Int_t maxz) {
    for (Int_t i=0;i<GetEntriesFast();i++) {
        Int_t y = ((THit *)At(i))->y;
        Int_t z = ((THit *)At(i))->z;
        if (y<miny || y>maxy || z<minz || z>maxz) {
            RemoveAt(i);
        }   

    }
    

}
