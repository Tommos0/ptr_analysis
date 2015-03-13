#ifndef __TCLUSTERFINDER_H__
#define __TCLUSTERFINDER_H__
#include "TObject.h"
#include "THitCollection.h"

class TClusterFinder: public TObject {
private:
    THitCollection *rawhits;
    std::vector<THitCollection*> clusters;
public:
    TClusterFinder();
    TClusterFinder(THitCollection *h);
    virtual ~TClusterFinder();
      
    std::vector<THitCollection*> GetClusters();
    void FindByX(Int_t minhits=20, Int_t maxoffset=4,Int_t ydif=0);

    ClassDef(TClusterFinder,2) // TClusterFinder


};


#endif //__TCLUSTERFINDER_H__
