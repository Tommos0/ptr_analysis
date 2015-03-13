int max(unsigned short * const data, int const length) {
    int max=data[0];
    for (int i=1;i<length;i++)
        if (data[i]>max) max = data[i];
    return max;
}
int avg(unsigned short * const data, int const length) {
    int avg=0;
    for (int i=0;i<length;i++)
        avg+=data[i];
    return avg/length;
}
int calib() {
    int runs[10] = { 1,2,3,4,5,6,7,8,9,10 };
    Double_t energy[11] = { 150, 139.4, 130.1, 120.3, 110.6, 100.2, 90.8, 80.5, 71.0, 60.6 };
    TH1I * hist[10];
    TH1I * hist2[10];
    //Fill the histograms
    for (Int_t i=0;i<10;i++) {
        TString filename = Form("/data/detrd/ProtonTherapy/testbeam_KVI_mei2014/data/run%d.root",runs[i]);
        TString treename = "data";
//        TString filename2 = Form("/project/detrd/ptr/kvi2014/run42_%d.root",runs[i]);
        hist[i] = new TH1I(Form("run%d",runs[i]),Form("run%d",runs[i]),100,0,2400);
        hist2[i] = new TH1I(Form("run2_%d",runs[i]),Form("run2_%d",runs[i]),100,0,2400);
        TFile *f = new TFile(filename);
        TTree *t = f->Get(treename);
  //      TFile *f2 = new TFile(filename2);
   //     TTree *t2 = f2->Get(treename);
        unsigned short calo_data[1602];
    //    unsigned short calo_data2[1602];
        t->SetBranchAddress("calo_data",calo_data);
    //    t2->SetBranchAddress("calo_data",calo_data2);
        cout << i << endl;
        for (int j=0;j<t->GetEntries();j++) {
            t->GetEntry(j);         
            hist[i]->Fill(max(calo_data,1602));
        }
    //    for (int j=0;j<t2->GetEntries();j++) {
    //        t2->GetEntry(j);         
    //        hist2[i]->Fill(max(calo_data2,1602));
    //    }
        f->Close();
     //   f2->Close();
    }
    //Plot histograms
//    gStyle->SetOptStat(0);
    gStyle->SetOptFit(1);
    TCanvas *c = new TCanvas("c","Energy Histogram",0,0,800,800);
 //   c->Divide(1,2);
    TLegend *leg = new TLegend(0.1,0.7,0.48,0.9);
    c->cd(1);
    for (int i=0;i<10;i++) {
        if (i==10) hist[i]->SetFillColor(11);
        else hist[i]->SetFillColor(i+1);
        if (i==0) hist[i]->Draw();
        else hist[i]->Draw("SAME");
        leg->AddEntry(hist[i],Form("Run %d - %4.1f MeV",runs[i],energy[i]),"f");
    }
    leg->Draw();
 /*   c->cd(2);
    for (int i=0;i<10;i++) {
        if (i==10) hist2[i]->SetFillColor(11);
        else hist2[i]->SetFillColor(i+1);
        if (i==0) hist2[i]->Draw();
        else hist2[i]->Draw("SAME");
    }
   // leg->SetHeader("The Legend Title");
    leg->Draw();
   */ 
    //Fit histograms -> plot energy calibration
    TCanvas *c3 = new TCanvas("c3","Energy Calibration",0,0,800,800);
    //TGraph * g = new TGraph(11);
    TGraphErrors * g2 = new TGraphErrors(11);
    c->cd(1);
    for (int i=0;i<10;i++){
        //hist[i]->Fit("gaus","","",hist[i]->GetMean()-200,hist[i]->GetMean()+350);
        Double_t bincenter = hist[i]->GetBinCenter(hist[i]->GetMaximumBin());
        hist[i]->Fit("gaus","","",bincenter-100,bincenter+100);
        TF1 *f1 = hist[i]->GetFunction("gaus");
        g2->SetPoint(i,f1->GetParameter(1),energy[i]);
        g2->SetPointError(i,f1->GetParameter(2),0);
    }
    c3->cd();
    g2->Draw("A*");
//    TGraph * g2 = new TGraph(11);
 //   c->cd(2);
 //   for (int i=0;i<10;i++){
        //hist[i]->Fit("gaus","","",hist[i]->GetMean()-200,hist[i]->GetMean()+350);
 //       Double_t bincenter = hist2[i]->GetBinCenter(hist2[i]->GetMaximumBin());
 //      hist2[i]->Fit("gaus","0","",bincenter-100,bincenter+100);
 //       TF1 *f1 = hist2[i]->GetFunction("gaus");
 //       g2->SetPoint(i,f1->GetParameter(1),energy[i]);
 //   }
  //  g2->SetMarkerColor(kRed);
  //  c3->cd();
 //   g2->Draw("* same");
    return 0;
}
