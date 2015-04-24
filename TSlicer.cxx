/* TTrack.cxx
 * a  = slope of track in y,x plane
 * b  = x coordinate of center of gravity
 * c  = slope of track in y,z plane
 * d  = z coordinate of center of gravity
 * Err: the errors from fitting
 * ycogxy = y coordinate center of gravity y,x plane
 * ycogzy = y coordinate cog z,y plane
 */
#include "TObject.h"
#include "TTrack.h"

ClassImp(TTrack);

TTrack::TTrack() {
    a=0;
    b=0;
    c=0;
    d=0;
    aErr=0;
    bErr=0;
    cErr=0;
    dErr=0;
    ycogxy=0;
    ycogzy=0;
}
TTrack::TTrack(Double_t a1,Double_t b1, Double_t c1, Double_t d1) : a(a1), b(b1), c(c1), d(d1) { }
TTrack::TTrack(Double_t a1, Double_t errA, Double_t b1, Double_t errB, Double_t c1, Double_t errC, Double_t d1, Double_t errD) : a(a1), b(b1), c(c1), d(d1),aErr(errA), bErr(errB), cErr(errC), dErr(errD) { }
TTrack::~TTrack() { }   
