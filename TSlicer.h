#ifndef __TSLICER_H__
#define __TSLICER_H__
#include "TGraphErrors.h"
#include "TH2D.h"
#include "TH1D.h"
namespace TSlicer {
    TGraphErrors *slice(TH2D *dist,int nslices) {
        TGraphErrors* tge = new TGraphErrors(nslices);
        cout << dist->GetXaxis()->GetXmin() << " " << dist->GetXaxis()->GetXmax() << endl;
        for (int i=0;i<nslices;i++) {
            TH1D *slice = dist->ProjectionY("slice",i*dist->GetNbinsX()/nslices,(i+1)*dist->GetNbinsX()/nslices);
            cout << "slice: " << i*dist->GetNbinsX()/nslices << " " << (i+1)*dist->GetNbinsX()/nslices << endl;
            if (slice->GetEntries()>0) {
                slice->Fit("gaus","q");
                TF1 *fit = slice->GetFunction("gaus");
                tge->SetPointError(i,0,fit->GetParError(2));
                Double_t range = dist->GetXaxis()->GetXmax() - dist->GetXaxis()->GetXmin();
                tge->SetPoint(i,dist->GetXaxis()->GetXmin() + (i+.5)*range/nslices   ,fit->GetParameter(2));
            }
            //delete slice;
            //delete fit;
        }
        return tge; 
    }

}
#endif //__TSLICER_H__
