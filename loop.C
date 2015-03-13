void loop() {
    for (Int_t i=12;i<17;i++) {
        gROOT->Reset();
        gROOT->LoadMacro("process.C");
        //TString filename=Form("/data/detrd/ProtonTherapy/testbeam_KVI_mei2014/data/run%i.root",i);
        TString filename=Form("/data/testbeam_KVI_mei2014/data/run%i.root",i);
        process(filename,"data",Form("run%i_processed.root",i));
    }

}
