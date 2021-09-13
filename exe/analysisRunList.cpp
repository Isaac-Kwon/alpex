#include "TROOT.h"
#include "TFile.h"
#include "TH1D.h"
#include "TTree.h"
#include "TCanvas.h"
#include "TH2D.h"
#include "TStyle.h"
#include "TGraphErrors.h"
#include "TMultiGraph.h"
#include "TColor.h"
#include "TLegend.h"

#include "iostream"
#include "fstream"
#include "string"

#include "deque"

class RUN{
    public:
    int filei;
    std::string THSfname;
    std::string FHRfname;
    std::string configfname;
    int VCASN;
    int VCASN2;
    int ITHR;
    TFile * THSfile;
    TFile * FHRfile;
    TH1D * hThreshold;
    TH1D * hNoise;
    TTree * tThreshold;
    TTree * tHit;
    TH2D * hHitmap;
    TH2D * hTHSmap;

    RUN(int i, std::string fname1, std::string fname2, std::string fname3, int ITHR_, int VCASN_, int VCASN2_):
    THSfname(fname1), FHRfname(fname2), configfname(fname3), VCASN(VCASN_), VCASN2(VCASN2_), ITHR(ITHR_), filei(i)
    {}
};

Color_t colors[] = {kBlack, kGray, kRed, kGreen, kCyan, kViolet, kBlue, kYellow, kOrange};

void analysisRun2_0(){
    TString rundirname = "../analyzed/Run2/";
    std::ifstream ifs(rundirname + "runlist.txt");
    std::deque<RUN*> runs;
    
    for(int i=0; ifs.good(); i++){
        std::string f1, f2, f3;
        int i1, i2, i3;

        ifs >> f1 >> f2 >> f3 >> i1 >> i2 >> i3;

        std::cout<<"Import\t" << f1 <<"\t"<< f2 <<"\t"<< f3 <<"\t"<< i1 <<"\t"<< i2 <<"\t"<< i3 << std::endl;
        
        runs.emplace_back(new RUN(i, f1, f2, f3, i1, i2, i3));
    }

    for(auto run : runs){
        // if(run->filei == 12) continue;
        // if(run->filei == 13) continue;
        // // if(run->filei > 20) break;

        run->THSfile = TFile::Open(rundirname+run->THSfname.c_str());
        run->FHRfile = TFile::Open(rundirname+run->FHRfname.c_str());
        run->tThreshold = (TTree *) run->THSfile->Get("tree");
        run->tHit = (TTree *) run->FHRfile->Get("hit");
        
        std::cout<<"open\t"<< rundirname+run->THSfname.c_str()<<std::endl;
        std::cout<<"open\t"<< rundirname+run->FHRfname.c_str()<<std::endl;
        std::cout<<"Threshold File \t" << run->THSfile  << std::endl;
        std::cout<<"Hit File \t" << run->FHRfile   << std::endl;
        std::cout<<"Threshold Tree \t" << run->tThreshold  << std::endl;
        std::cout<<"Hit Tree \t" << run->tHit   << std::endl;

        run->hThreshold     = new TH1D(TString::Format("h1_threshold_%d" , run->filei), TString::Format("Thres VCASN=%d ITHR=%d",run->VCASN,run->ITHR), 1000, 0, 50);
        run->hNoise         = new TH1D(TString::Format("h1_noise_%d"     , run->filei), TString::Format("Noise VCASN=%d ITHR=%d",run->VCASN,run->ITHR), 500, 0, 10);
        run->tThreshold->Draw(TString::Format("thres >> h1_threshold_%d" , run->filei),"","goff");
        run->tThreshold->Draw(TString::Format("noise >> h1_noise_%d"     , run->filei),"","goff");
        run->hTHSmap = (TH2D*) run->THSfile->Get("hThreshold");

        run->hHitmap = new TH2D(TString::Format("h2_hitmap_%d" , run->filei), TString::Format("Hitmap VCASN=%d ITHR=%d",run->VCASN,run->ITHR), 1024, 0, 1024, 512, 0, 512);
        run->tHit->Draw(TString::Format("y:x >> h2_hitmap_%d" , run->filei),"x!=638 && x!=639 && x!=640 && x!=641","goff",run->tHit->GetEntries());
        
        TCanvas * c1 = new TCanvas("c1", "c1", 1200, 800);
        c1->Draw();
        gStyle->SetOptStat(111111);
        c1->Update();
        run->hThreshold->Draw("COLZ");
        c1->SaveAs(TString::Format("../analyzed/Run2/plot/Threshold_%d.png",run->filei));
        // gStyle->SetOptStat(0);
        c1->Update();
        run->hHitmap->Draw("COLZ");
        c1->SaveAs(TString::Format("../analyzed/Run2/plot/HitMap_%d.png",run->filei));
        run->hTHSmap->Draw("COLZ");
        c1->SaveAs(TString::Format("../analyzed/Run2/plot/ThresholdMap_%d.png",run->filei));
    }
}