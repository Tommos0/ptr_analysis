/* *TClusterFinder
 * Finds clusters of hits, each cluster should be a track
 * Input THitCollection, output vector of THitCollection.
 * */

#include "TClusterFinder.h"
#include "THit.h"
#include "THitCollection.h"
#include <iostream>
ClassImp(TClusterFinder);

TClusterFinder::TClusterFinder() {}
TClusterFinder::TClusterFinder(THitCollection *h) {
    rawhits = h;
}
TClusterFinder::~TClusterFinder() { 
   //for (Int_t i=0;i<clusters.size();i++) delete clusters[i];
   clusters.clear();
}

std::vector<THitCollection*> TClusterFinder::GetClusters() {
    return clusters;
}

void TClusterFinder::FindByX(Int_t minhits,Int_t maxoffset, Int_t ydif) {
    rawhits->Sort();
    Int_t entries = rawhits->GetEntriesFast();
    if (entries<5) return;

    THit *lastHit = (THit *)rawhits->At(0);
    THit *nextHit;

    THitCollection *currentCluster = new THitCollection();
    currentCluster->Add(lastHit);
    
    for (Int_t i=1;i<entries;i++) {
        nextHit = (THit *)rawhits->At(i);
        if ((nextHit->x - lastHit->x) > maxoffset)  { //new cluster
            if ((currentCluster->GetEntriesFast() > minhits) && ((currentCluster->MaxY()-currentCluster->MinY())>ydif)) 
               clusters.push_back(currentCluster); 
            else { delete currentCluster; }
            currentCluster = new THitCollection();
        }
        currentCluster->Add(nextHit);
        lastHit = nextHit;
    }
    if (currentCluster->GetEntriesFast() > minhits) clusters.push_back(currentCluster);
    else { delete currentCluster; }
}

