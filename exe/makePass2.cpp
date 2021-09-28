#include "TROOT.h"
#include "TFile.h"
#include "TString.h"
#include "TTree.h"
#include "TH2D.h"

#include "iostream"
#include "stdlib.h"
#include "getopt.h"

#include "TCluster.hpp"
#include "TEvent.hpp"

// #include "TAnalysisTaskClustering.hpp"

struct Arguments{
    TString ifilename = "";
    TString ofilename = "";
    Int_t   minNpixel = 1;


    Bool_t  b_ifilename = kFALSE;
    Bool_t  b_ofilename = kFALSE;
    Bool_t  b_npixel  = kFALSE;

    Arguments(): ifilename(""),       ofilename(""),       minNpixel(0.),
                 b_ifilename(kFALSE), b_ofilename(kFALSE), b_npixel(kFALSE){}

};

TFile * Analysis(TFile * ifile, TFile * ofile=nullptr, Int_t npix=0){
    if(ofile == nullptr){
        std::cout<<"WARNING: output file is not defined. make new file; result_pass0.root"<<std::endl;
        ofile = new TFile("result_pass0.root", "RECREATE");
    }
    ofile->cd();

    TTree * treei = (TTree*) ifile->Get("tcluster");
    TTree * treeo = new TTree("tcluster", "Tree of Clusters pass2");
    

    TClusterDump clusterdump;

    std::cout<<"pass2 part 0"<<std::endl;
    treei->SetBranchAddress("t", &(clusterdump.t));
    treei->SetBranchAddress("npixel", &(clusterdump.n));
    treei->SetBranchAddress("pixelx", clusterdump.x);
    treei->SetBranchAddress("pixely", clusterdump.y);
    treei->SetBranchAddress("noise",  clusterdump.noise);
    treei->SetBranchAddress("cluster", &clusterdump);

    std::cout<<"pass2 part 1"<<std::endl;
    treeo->Branch("t", &(clusterdump.t), "t/I");
    treeo->Branch("npixel", &(clusterdump.n), "npixel/s");
    treeo->Branch("pixelx", clusterdump.x, "x[npixel]/s");
    treeo->Branch("pixely", clusterdump.y, "y[npixel]/s");
    treeo->Branch("noise",  clusterdump.noise, "noise[npixel]/s");
    treeo->Branch("cluster", &clusterdump, "centerx/s:centery/s:sizex/s:sizey/s:coreless/O");
    std::cout<<"pass2 part 2"<<std::endl;

    treeo->SetAlias("noiselevel", "Sum$(noise)/npixel");

    Int_t nentries = treei->GetEntries();
    for(Int_t i=0; i<nentries; i++){
        treei->GetEntry(i);
        
        if(clusterdump.n <= npix) continue;

        treeo->Fill();
    }

    TH2D* h1 = new TH2D("h1", "Hitmap", 1024, 0, 1024, 512, 0, 512);
    h1->GetXaxis()->SetTitle("PixelX");
    h1->GetYaxis()->SetTitle("PixelY");
    TH2D* h2 = new TH2D("h2", "CluterCenter", 1024, 0, 1024, 512, 0, 512);
    h2->GetXaxis()->SetTitle("PixelX");
    h2->GetYaxis()->SetTitle("PixelY");
    TH2D* h4 = new TH2D("h4", "CluterCenter", 16, 0, 1024, 8, 0, 512);
    h4->GetXaxis()->SetTitle("PixelX");
    h4->GetYaxis()->SetTitle("PixelY");

    ofile->Add(treeo);
    ofile->Add(h1);
    ofile->Add(h2);
    ofile->Add(h4);

    treeo->Draw("y:x >> h1", "", "goff");
    treeo->Draw("cluster.centery:cluster.centerx >> h2", "", "goff");
    treeo->Draw("cluster.centery:cluster.centerx >> h4", "", "goff");


    return ofile;

}

int main(int argc, char** argv){
  char c; // option
  Arguments args;

  static struct option long_options[] =
    {
      {"input"    , required_argument, 0, 'i'},
      {"output"   , required_argument, 0, 'o'},
      {"npuxel"   , required_argument, 0, 'n'},
      {"help"     , no_argument,       0, 'h'},
      {0, 0, 0, 0}
    };

    int option_index = 0;
    while( (c = getopt_long(argc, argv, "i:o:n:h", long_options, &option_index)) != -1){
        switch(c){
            case 'i':
                args.ifilename  = TString(optarg);
                args.b_ifilename = kTRUE;
                break;
            break;
            case 'o':
                args.ofilename  = TString(optarg);
                args.b_ofilename = kTRUE;
                break;
            case 'n':
                args.minNpixel = Int_t(std::stoi(optarg));
                args.b_npixel = kTRUE;
                break;
            case 'h':
            std::cout<<"Remove less pixel cluster"<<std::endl;
            std::cout<<"-i \t input  \t input file(root) " <<std::endl;
            std::cout<<"-o \t output \t output file" <<std::endl;
            std::cout<<"-n \t npixel \t minimum number of pixel in a cluster" <<std::endl;
            std::cout<<"-h \t help   \t this message" <<std::endl;
                return 0;
            case '?':
                printf("Unknown flag : %c", optopt);
                return 1;
        }
    }

    if (!(args.b_ifilename && args.b_ofilename)){
        std::cout<<"Input file name and output file name are necessary!"<<std::endl;
        return 1;
    }

    TFile * ifile = new TFile(args.ifilename, "READ");
    TFile * ofile = new TFile(args.ofilename, "RECREATE");
    Analysis(ifile, ofile, args.minNpixel);

    ofile->Write();
    // ifile->Close("R");
    // ofile->Close("R");
    ofile->Delete();

    // delete ifile;
    // delete ofile;

    return 0;
}