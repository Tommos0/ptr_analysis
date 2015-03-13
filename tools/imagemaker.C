

TH2D *getImage(TChain *t) {
    gROOT->ProcessLine(".L TTrack.cxx+");
    const int numbinsx = 60;
    const int numbinsy = 60;
    const Double_t eLow = -10;
    const Double_t eHigh = 30;
    const Double_t xLow = -2;
    const Double_t xHigh = 30;
    const Double_t yLow = -2;
    const Double_t yHigh = 30;
    TH1D * binEnergy[numbinsx][numbinsy];
    for (Int_t i=0;i<numbinsx;i++)
        for (Int_t j=0;j<numbinsy;j++) {
            TString name;
            name.Form("bin%i.%i",i,j);
            binEnergy[i][j] = new TH1D(name,name,100,eLow,eHigh);
            binEnergy[i][j]->Sumw2();
        }
    TTrack *track1;
    TTrack *track2;
    Double_t energy;
    Int_t entries = t->GetEntries();
    t->SetBranchAddress("track1",&track1);
    t->SetBranchAddress("track2",&track2);
    t->SetBranchAddress("energy",&energy);
    cout << "entries: " << entries << endl;
    TH2D * bincounts = new TH2D("bincounts","",numbinsx,0,numbinsx,numbinsy,0,numbinsy);
    TH1D *dist = new TH1D("dist","dist",100,-50,50);
        TRandom a;
    Double_t avgx1x2a=0;
    Double_t avgx1x2b=0;
    int n=0;
    for (int i=0; i < entries-1; i++) {
        if (i%10000==0) cout << "progress: " << i << endl;
        t->GetEntry(i);
//        if (track1->aErr > 0.01) continue; 
        if (track1->a>0.2267 || track1->a<0.0533 || track2->a>0.2267 || track2->a<0.0533 || track1->c>0.2174 || track1->c < -0.184 ||  track2->c>0.2174 || track2->c < -0.184) continue; 
        if (track1->a != track1->a || track2->a != track2->a) continue;
        n++;
        Double_t T1 = TMath::ATan(track1->a)-0.138;
        Double_t T2 = TMath::ATan(track2->a)-0.136;
        Double_t S1 = TMath::ATan(track1->c);
        Double_t S2 = TMath::ATan(track2->c);
        avgx1x2a+=(track1->b-track2->b);
    //    cout << track1->b - track2->b << " "; 
        aErr1 = track1->aErr/(1+tan(T1)*tan(T1));
        cErr1 = track1->cErr/(1+tan(S1)*tan(S1));
        aErr2 = track2->aErr/(1+tan(T1)*tan(T1));
        cErr2 = track2->cErr/(1+tan(S1)*tan(S1));
        T1 = T1/(1.+aErr1*aErr1/(0.0071*0.0071));
        T2 = T2/(1.+aErr2*aErr2/(0.0071*0.0071));
        S1 = S1/(1.+cErr1*cErr1/(0.0071*0.0071));
        S2 = S2/(1.+cErr2*cErr2/(0.0071*0.0071));
       // T1 = 0;
        //T1 = 0;
        /*T1+=a.Gaus(0,0.02);
        T2+=a.Gaus(0,0.02);
        S1 = 0;*/
        //S1 = 0;
        /*
        S1 += a.Gaus(0,0.02);
        S2 += a.Gaus(0,0.02);*/
        Double_t dTPCy = 229;
        Double_t dTPCx = -1.13;
        Double_t dTPCz = 0;
        Double_t yplane = 115;

        TVector2 xycog1(track1->b,track1->ycogxy);
        xycog1 = xycog1.Rotate(0.138);
        Double_t x1 = xycog1.X() + (yplane - xycog1.Y())*TMath::Tan(T1);

        TVector2 xycog2(track2->b,track2->ycogxy);
        xycog2 = xycog2.Rotate(0.136);
        xycog2.Set(xycog2.X()+dTPCx,xycog2.Y()+dTPCy);
        Double_t x2 = xycog2.X() - (xycog2.Y() - yplane)*TMath::Tan(T2);
//        cout << T2 << " " << x1 << " " << x2 << endl;
        //TVector2 zycog1(track1->d,track1->ycogzy);
        TVector2 zycog1(track1->d,xycog1.Y());
        Double_t z1 = zycog1.X() + (yplane - zycog1.Y())*TMath::Tan(S1);
        
        TVector2 zycog2(track2->d,xycog2.Y());
        zycog2.Set(zycog2.X()+dTPCz,zycog2.Y()+dTPCy);
        
        Double_t z2 = zycog2.X() - (zycog2.Y() - yplane)*TMath::Tan(S2);


        avgx1x2b+=(x1-x2);
      //  cout << z1 << " " << z2 << endl;


    //    dist->Fill(z1-z2);

        Double_t x = (x1+x2)/2.;
        //x = x1;
        Double_t y = (z1+z2)/2.;
/*        if (track1->aErr > 0.01) {
            //x=(xycog1.X()+xycog2.X())/2.;
            x=xycog1.X();
        }*/
//        cout << "x1 " << xycog1.X() << " x2 " << xycog2.X() << endl; 
            //x = ((track1->b + 8.2*track1->a) + (track2->b - 6.8*track2->a))/2.;
        //y = ((track1->d + 8.2*track1->c) + (track2->d - 6.8*track2->c))/2.;
//        y = (zycog1.X() + zycog2.X())/2.;
//        y = (zycog1.X());
        //Double_t x = (track2->b + (track1->a * 256 + track1->b))/2;
        //Double_t y = (track2->d + (track1->c * 256 + track1->d))/2;
        Int_t xbin =  (numbinsx/(xHigh-xLow))*(x-xLow);
        Int_t ybin =  (numbinsy/(yHigh-yLow))*(y-yLow);
        if (xbin>0 && xbin<numbinsx && ybin>0 && ybin<numbinsy) {
            binEnergy[xbin][ybin]->Fill(150-energy,1./track1->aErr);
            bincounts->Fill(xbin,ybin);
        }

 /*       if (i==2) {
            TCanvas *c4 = new TCanvas("c5","c5");
            TLine *l = new TLine(0,0,10,10);
            l->DrawLine(0,0,10,10);
            l->Draw();
        }
*/
    }
    cout << avgx1x2a/((Double_t)n) << " " << avgx1x2b/((Double_t)n) << endl;
    bincounts->SetMaximum(70);
    bincounts->SetMinimum(0);
  //  gStyle->SetNumberContours(255);
    TH2D * hist = new TH2D("calo","",numbinsx,xLow,xHigh,numbinsy,yLow,yHigh);
    //TH2D * hist = new TH2D("calo","",numbinsx,xLow,xHigh,numbinsy,yLow,yHigh);
    for (Int_t i=0;i<numbinsx;i++)
        for (Int_t j=0;j<numbinsy;j++) {
            if (binEnergy[i][j]->GetEntries()>15) {
                
                /*******************************
                 * Find max from gauss density *
                 *******************************/
                
                TString dname;
                dname.Form("density%i.%i",i,j);
                TH1D *dens = new TH1D(dname,dname,100,eLow,eHigh);
                for (Int_t k=0;k<100;k++) {
                    Double_t d=0;
                    for (Int_t l=0;l<100;l++) 
                         d=d+ binEnergy[i][j]->GetBinContent(l) * TMath::Exp((-1. * (Double_t )((k-l)*(k-l))) / (2.*6.*6.));
                    dens->SetBinContent(k,d);
                }
                hist->SetBinContent(i,j,dens->GetMaximumBin()*((eHigh-eLow)/100)+eLow ); 
                delete dens;
                

                /***********************
                 * Fit bin with landau *
                 ***********************/
/*               
                binEnergy[i][j]->Fit("landau","0Lq");
                TF1 *fit = binEnergy[i][j]->GetFunction("landau");
                Double_t e = fit->GetParameter(1);
               // if (fit->GetParError(1)/fit->GetParameter(1) < 0.5)
                    hist->SetBinContent(i,j,fit->GetParameter(1));
//                hist->SetBinContent(i,j,0); 
//                
                delete fit;
  */              
            
            
            }
            else {
                hist->SetBinContent(i,j,0);
            }
          //  delete binEnergy[i][j];
        }
    return hist;
}
