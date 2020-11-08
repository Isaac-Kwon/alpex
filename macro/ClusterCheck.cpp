#include "TROOT.h"
#include "TClusterChecker.hpp"
#include "TFile.h"
#include "TTree.h"

void ClusterCheck(){

    gROOT->cd();
    TClusterChecker* cc = new TClusterChecker();
    cc->Print();
    TFile * file = new TFile("./testroot.root", "READ");
    TTree * tree = (TTree*) file->Get("tcluster");
    cc->Init();

    cc->SetTree(tree);
    cc->Draw("");

    cc->DrawCanvas();
    cc->UpdateCanvas();
    cc->StartInterpreter();

    //Checking Clusters
    // 1.1 Whole Hitmap, Partial Hitmap
    // 1.2 Partial Hitmap. (on timeframe and not)
    // 2.1-3 Timeframe Hitmap in partial(-1, 0, +1)

    

}