#include "THit.h"
#include <iostream>
ClassImp(THit);


THit::THit(): TObject() { init(); }
THit::THit(Int_t xvalue, Int_t yvalue, Int_t zvalue):TObject() {
    init();
    x = xvalue;
    y = yvalue;
    z = zvalue;
}

void THit::init() { //set default values
    vdrift = 1.9; //cm / micro sec
    zmax = 30.; //drift height in mm
    zstart = 20.; //start of z in bins (@ cathode)
    dtrans = 201.480; //micro m / sqrt cm
    dlong = 165.751;
}
void THit::SetPars(Double_t _vdrift,Double_t _zmax,Double_t _zstart,Double_t _dtrans,Double_t _dlong) {
	vdrift=_vdrift;
	zmax=_zmax;
	zstart=_zstart;
	dtrans=_dtrans;
	dlong=_dlong;

}

Int_t THit::Compare(const TObject *obj) const { //used for ordering
    if (x<((THit *)obj)->x) return -1;
    else if (x>((THit *)obj)->x) return 1;
    else return 0;
}
Bool_t THit::IsSortable() const {
    return kTRUE;
}
Double_t THit::xmm() {
    return x * 55. * 1.e-3;
}
Double_t THit::ymm() {
    return y * 55. * 1.e-3;
}

Double_t THit::zmm() {
//  return zmax - (z - zstart) * vdrift; 
    return zmax - (z - zstart) * vdrift/10.; 
}
Double_t THit::xerr() {
/*   Double_t transerr = zmm() * dtrans*dtrans*0.1;
   if (transerr < 0) { transerr = 0; }
   return TMath::Sqrt(( 55. * 55. / 12. ) + transerr) * 1.e-3;
*/
//	return 0.0521+0.0540*TMath::Sqrt(zmm()); //iter. 1
	return 0.0460+0.0552*TMath::Sqrt(zmm()); //iter. 2

}
Double_t THit::yerr() {
   return xerr();
//   return TMath::Sqrt(( 55. * 55. / 12. ) + zmm() * dtrans * dtrans * 0.1) * 1.e-3;
}
Double_t THit::zerr() {
//   Double_t longerr = zmm() * dlong*dlong*1.e-7;
//   if (longerr<0) { longerr = 0; }
//   return TMath::Sqrt( 0.64*0.64 + 0.01*vdrift*vdrift/12. + longerr);
//     return 0.598+0.0280*TMath::Sqrt(zmm()); //iter. 1
     return 0.549+0.0400*TMath::Sqrt(zmm()); //iter. 2
}
THit::~THit() { 
}
