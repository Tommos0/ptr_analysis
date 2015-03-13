int radiograph() {
    gROOT->ProcessLine(".L TTrack.cxx+");
    gROOT->ProcessLine(".L imagemaker.C");
    const int numbinsx = 60;
    const int numbinsy = 60;
    const Double_t eLow = -10;
    const Double_t eHigh = 100;
    const Double_t xLow = -2;
    const Double_t xHigh = 30;
    const Double_t yLow = -2;
    const Double_t yHigh = 30;
    //TFile *f = new TFile(filename);
    //TTree *t = f->Get(treename);
    TChain *t = new TChain("chain");
      t->Add("run12_processed.root/data");
      t->Add("run13_processed.root/data");
      t->Add("run14_processed.root/data");
      t->Add("run15_processed.root/data");
      t->Add("run16_processed.root/data");
/*      t->Add("run17_processed.root/data");
      t->Add("run18_processed.root/data");
      t->Add("run19_processed.root/data");
      t->Add("run20_processed.root/data");
      t->Add("run21_processed.root/data");
      t->Add("run22_processed.root/data");
      */
      //t->Add("run23_processed.root/data");
/*      t->Add("run36_processed.root/data");
      t->Add("run37_processed.root/data");
      t->Add("run38_processed.root/data");
      t->Add("run39_processed.root/data");
      t->Add("run40_processed.root/data");
      t->Add("run41_processed.root/data");*/
/*      t->Add("run29_processed.root/data");
      t->Add("run30_processed.root/data");
      t->Add("run31_processed.root/data");
      t->Add("run32_processed.root/data");
      t->Add("run33_processed.root/data");
      t->Add("run34_processed.root/data");
      t->Add("run35_processed.root/data");*/
      //t->Add("run23_processed.root/data");
      //t->Add("run40_processed.root/data");
    //  t->Add("run41_processed.root/data");





    TH2D *hist = getImage(t);
    hist->SetMaximum(60);
    hist->SetMinimum(-5);
  //  gStyle->SetNumberContours(255);
   TCanvas *c1 = new TCanvas("c1", "c1",10,32,700,502);
   gStyle->SetOptStat(0);
   c1->Range(-1.020454,-1.24578,1.020454,1.24578);
   TView *view = TView::CreateView(1);
   view->SetRange(-1.4,-1.4,0,1.4,1.4,21);
   c1->SetFillColor(0);
   c1->SetBorderMode(0);
   c1->SetBorderSize(2);
   c1->SetTheta(59.36441);
   c1->SetPhi(133.9655);
   c1->SetFrameBorderMode(0);
   c1->SetFrameBorderMode(0);
    gStyle->SetPalette(1);
    gStyle->SetOptStat(0);


   TH2D *aver_xyEloss=hist;
   aver_xyEloss->SetMinimum(0);
   aver_xyEloss->SetMaximum(20);
   aver_xyEloss->SetEntries(31844.53);
   aver_xyEloss->SetContour(20);
   aver_xyEloss->SetContourLevel(0,0);
   aver_xyEloss->SetContourLevel(1,1);
   aver_xyEloss->SetContourLevel(2,2);
   aver_xyEloss->SetContourLevel(3,3);
   aver_xyEloss->SetContourLevel(4,4);
   aver_xyEloss->SetContourLevel(5,5);
   aver_xyEloss->SetContourLevel(6,6);
   aver_xyEloss->SetContourLevel(7,7);
   aver_xyEloss->SetContourLevel(8,8);
   aver_xyEloss->SetContourLevel(9,9);
   aver_xyEloss->SetContourLevel(10,10);
   aver_xyEloss->SetContourLevel(11,11);
   aver_xyEloss->SetContourLevel(12,12);
   aver_xyEloss->SetContourLevel(13,13);
   aver_xyEloss->SetContourLevel(14,14);
   aver_xyEloss->SetContourLevel(15,15);
   aver_xyEloss->SetContourLevel(16,16);
   aver_xyEloss->SetContourLevel(17,17);
   aver_xyEloss->SetContourLevel(18,18);
   aver_xyEloss->SetContourLevel(19,19);

   Int_t ci;   // for color index setting
   ci = TColor::GetColor("#000099");
   aver_xyEloss->SetLineColor(ci);
   aver_xyEloss->GetXaxis()->SetTitle("X [cm]");
   aver_xyEloss->GetXaxis()->Set(60,-1.4,1.4);
   aver_xyEloss->GetYaxis()->Set(60,-1.4,1.4);
   aver_xyEloss->GetXaxis()->CenterTitle(true);
   aver_xyEloss->GetXaxis()->SetLabelFont(42);
   aver_xyEloss->GetXaxis()->SetLabelOffset(0.001);
   aver_xyEloss->GetXaxis()->SetLabelSize(0.05);
   aver_xyEloss->GetXaxis()->SetTitleSize(0.06);
   aver_xyEloss->GetXaxis()->SetTitleOffset(1.3);
   aver_xyEloss->GetXaxis()->SetTitleFont(42);
   aver_xyEloss->GetYaxis()->SetTitle("Y [cm]");
   aver_xyEloss->GetYaxis()->CenterTitle(true);
   aver_xyEloss->GetYaxis()->SetLabelFont(42);
   aver_xyEloss->GetYaxis()->SetLabelOffset(-0.003);
   aver_xyEloss->GetYaxis()->SetLabelSize(0.05);
   aver_xyEloss->GetYaxis()->SetTitleSize(0.06);
   aver_xyEloss->GetYaxis()->SetTitleOffset(1.3);
   aver_xyEloss->GetYaxis()->SetTitleFont(42);
   aver_xyEloss->GetZaxis()->SetTitle("Energy loss [MeV]");
   aver_xyEloss->GetZaxis()->CenterTitle(true);
   aver_xyEloss->GetZaxis()->SetNdivisions(509);
   aver_xyEloss->GetZaxis()->SetLabelFont(42);
   aver_xyEloss->GetZaxis()->SetLabelOffset(0.008);
   aver_xyEloss->GetZaxis()->SetLabelSize(0.05);
   aver_xyEloss->GetZaxis()->SetTitleSize(0.06);
   aver_xyEloss->GetZaxis()->SetTitleOffset(0.75);
   aver_xyEloss->GetZaxis()->SetTitleFont(42);
   aver_xyEloss->Draw("LEGO2 ");
   
   TPaveText *pt = new TPaveText(0.4252874,0.934322,0.5732759,0.9957627,"blNDC");
   pt->SetName("title");
   pt->SetBorderSize(0);
   pt->SetFillColor(0);
   pt->SetTextFont(42);
   TText *text = pt->AddText("xyEloss");
   pt->Draw();
   c1->Modified();
   c1->cd();
   c1->SetSelected(c1);







    //hist->Smooth(1,"k3a");
    return 0;
}
