#include "TFit.h"
#include "THitCollection.h"
#include "TGraphErrors.h"
#include "THit.h"
#include "TCanvas.h"
#include "TGraph.h"
#include "TAxis.h"
#include "TF1.h"
#include "TTrack.h"
#include <iostream>

ClassImp(TFit);

TFit::TFit() { init(); }
TFit::TFit(THitCollection *hc) {
    init();
    hits = hc;
}

void TFit::init() {
    vdrift = 1.9; //cm / micro sec
    zmax = 30.; //drift height in mm
    zstart = 20.; //start of z in bins (@ cathode)
    dtrans = 201.480; //micro m / sqrt cm
    dlong = 165.751;
    xErrp1 = 0;
    xErrp2 = 0;
    zErrp1 = 0;
    zErrp2 = 0;
}

TFit::~TFit() {}

/*FitTGraph(): Fit using ROOT's TGraphErrors*/
TTrack * TFit::FitTGraph() {
    Int_t entries = hits->GetEntriesFast();
    THit *h;
    TGraphErrors *geyx = new TGraphErrors(entries);
    TGraphErrors *geyz = new TGraphErrors(entries);
        
    for (Int_t i=0;i<entries;i++) {
        h = (THit *)hits->At(i);
        geyx->SetPoint(i,h->y,h->x);
        geyx->SetPointError(i,5,5);
        geyz->SetPoint(i,h->y,h->z);
        geyz->SetPointError(i,5,5);
    }
    TF1 *linear = new TF1("linear", "x ++ 1", 0, 256);
    geyx->Fit(linear,"q");
    TF1 *fit1 = geyx->GetFunction("linear");
    geyz->Fit(linear,"q");
    TF1 *fit2 = geyz->GetFunction("linear");
    TTrack * track = new TTrack(fit1->GetParameter(0),fit1->GetParameter(1),fit2->GetParameter(0),fit2->GetParameter(1));
    track->aErr = fit1->GetParError(0);
    track->bErr = fit1->GetParError(1);
    track->cErr = fit2->GetParError(0);
    track->dErr = fit2->GetParError(1);
    track->chi2yx = fit1->GetChisquare();
    track->chi2yz = fit2->GetChisquare();
    track->nhits = entries;
    delete fit2;
    delete fit1;
    delete linear;
    delete geyz;
    delete geyx;
    return track;
}

void TFit::YorkMethod(Int_t nhits, Double_t *x, Double_t *y, Double_t *sigx, Double_t *sigy, Double_t *resa, Double_t *resb, Double_t *aerr, Double_t *berr, Double_t *xcog) {
    Double_t *wx = new Double_t[nhits];
    Double_t *wy = new Double_t[nhits];
    Double_t *W = new Double_t[nhits];
    Double_t *U = new Double_t[nhits];
    Double_t *V = new Double_t[nhits];
    Double_t *B = new Double_t[nhits];
    Double_t b=0;
    Double_t avX=0;
    Double_t avY=0;
    Double_t avB=0;
    Double_t sumWBV = 0;
    Double_t sumWBU = 0;
    Double_t sumW = 0; 
    for (Int_t i=0;i<nhits;i++) {
          wx[i] = 1./(sigx[i]*sigx[i]);
          wy[i] = 1./(sigy[i]*sigy[i]);
    }
    for (Int_t j=0;j<20;j++) {
        sumW=0;
        avX=0;
        avY=0;
        sumWBV=0;
        sumWBU=0;
        for (Int_t i=0;i<nhits;i++) {
            W[i] = (wx[i]*wy[i])/(wx[i] + b*b*wy[i]);
            sumW += W[i];
            avX += W[i]*x[i];
            avY += W[i]*y[i];
        }
        avX = avX/sumW;
        avY = avY/sumW;
        for (Int_t i=0;i<nhits;i++) {
            U[i]=x[i]-avX;
            V[i]=y[i]-avY;
            B[i] = W[i] * (U[i]/wy[i] + b*V[i]/wx[i]);
            sumWBV += W[i]*B[i]*V[i];
            sumWBU += W[i]*B[i]*U[i];
        }
        b = sumWBV/sumWBU;
    }
//    Double_t a = avY - b * avX;
    Double_t a = avY;
    //a,b found, calculate errors
    Double_t *xadj = new Double_t[nhits];
    Double_t avgxadj = 0;
    for (Int_t i=0;i<nhits;i++) {
        xadj[i] = avX + B[i];
        avgxadj += W[i] * xadj[i];
    }
    avgxadj = avgxadj / sumW;
    Double_t *u = new Double_t[nhits];
    *berr = 0;
    for (Int_t i=0;i<nhits;i++) {
        u[i] = xadj[i] - avgxadj;
        *berr += W[i]*u[i]*u[i];
    }
    *berr = TMath::Sqrt(1./(*berr));
    //*aerr = TMath::Sqrt(1./sumW + avgxadj*avgxadj * (*berr) * (*berr));
    *aerr = TMath::Sqrt(1./sumW);
    *resa = a;
    *resb = b;
    *xcog = avX;
    delete wx;
    delete wy;
    delete W;
    delete U;
    delete V;
    delete B;
    delete xadj;
    delete u;

}



/*FitYork: Fit using York's iterative method
 * Converts all units to mm
 * */
TTrack * TFit::FitYork() {
    Int_t entries = hits->GetEntriesFast();
    THit *h;
    Double_t *x = new Double_t[entries];
    Double_t *y = new Double_t[entries];
    Double_t *z = new Double_t[entries];
    Double_t *sigx = new Double_t[entries]; //==sigy
    Double_t *sigz = new Double_t[entries];
    for (Int_t i=0;i<entries;i++) {
        //calculate all in millimeter
        h = (THit *)hits->At(i);
        h->vdrift = vdrift;
        h->zmax = zmax;
        h->zstart = zstart;
        h->dtrans = dtrans;
        h->dlong = dlong;
        y[i] = h->ymm();
        x[i] = h->xmm();
        z[i] = h->zmm(); 
        if (xErrp1>0) {
            sigx[i] = max(0.001,xErrp1 + xErrp2*TMath::Sqrt(h->zmm()));
            sigz[i] = max(0.001,zErrp1 + zErrp2*TMath::Sqrt(h->zmm())); 
        }
        else {
            sigx[i] = h->xerr();
            sigz[i] = h->zerr();
        }
    }
    /*TGraphErrors *tg = new TGraphErrors(entries,y,z,sigx,sigz);
    tg->SetMarkerColor(4);
    tg->SetMarkerStyle(1);
    tg->Draw("AP");
    tg->GetXaxis()->SetLimits(0,15);
    tg->GetYaxis()->SetRangeUser(0,40);
    */
    TTrack * track = new TTrack();
    //swapped x and y because then slope is near 0
    YorkMethod(entries,y,x,sigx,sigx,&(track->b),&(track->a),&(track->bErr),&(track->aErr),&(track->ycogxy));
    YorkMethod(entries,y,z,sigx,sigz,&(track->d),&(track->c),&(track->dErr),&(track->cErr),&(track->ycogzy));
    track->nhits = entries;
    track->chi2yx = 0;
    track->chi2yz = 0;
    for (Int_t i=0;i<entries;i++) {
           // cout << x[i] - track->b - track->a * y[i]  << " / " << TMath::Sqrt(TMath::Power(sigx[i],2) + track->a*track->a*TMath::Power(sigx[i],2)) << endl;      
            track->chi2yx += TMath::Power(x[i] - track->b - track->a * y[i] , 2) / (TMath::Power(sigx[i],2) + track->a*track->a*TMath::Power(sigx[i],2));
            track->chi2yz += TMath::Power(z[i] - track->d - track->c * y[i] , 2) / (TMath::Power(sigz[i],2) + track->c*track->c*TMath::Power(sigx[i],2));
    }
    //cout << "hits: " << track->nhits << " " << "Chi2 yz = " << track->chi2yz << endl;
    delete x;
    delete y;
    delete z;
    delete sigx;
    delete sigz;
    return track;
}
