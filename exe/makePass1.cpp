#include "TROOT.h"
#include "TFile.h"
#include "TString.h"

#include "iostream"
#include "stdlib.h"
#include "getopt.h"

#include "TAnalysisTaskClustering.hpp"

struct Arguments{
    TString ifilename = "";
    TString ofilename = "";
    Double_t distance = 0.;


    Bool_t  b_ifilename = kFALSE;
    Bool_t  b_ofilename = kFALSE;
    Bool_t  b_distance  = kFALSE;

    Arguments(): ifilename(""),       ofilename(""),       distance(0.),
                 b_ifilename(kFALSE), b_ofilename(kFALSE), b_distance(kFALSE){}

};

TFile * Analysis(TFile * ifile, TFile * ofile=nullptr, Double_t distance=0.0){
    if(ofile == nullptr){
        std::cout<<"WARNING: output file is not defined. make new file; result_pass0.root"<<std::endl;
        ofile = new TFile("result_pass0.root", "RECREATE");
    }

    TAnalysisTaskClustering * t = new TAnalysisTaskClustering("task", "taskClustering", ifile, ofile);
    t->SetDistance(distance);
    t->Exec(1000, 1);
    t->Save(0, kFALSE);
    t->Clear();

    delete t;

    return ofile;

}

int main(int argc, char** argv){
  char c; // option
  Arguments args;

  static struct option long_options[] =
    {
      {"input"            , required_argument, 0, 'i'},
      {"output"           , required_argument, 0, 'o'},
      {"distance"         , required_argument, 0, 'd'},
      {"help"             , no_argument,       0, 'h'},
      {0, 0, 0, 0}
    };

    int option_index = 0;
    while( (c = getopt_long(argc, argv, "i:o:d:h", long_options, &option_index)) != -1){
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
            case 'd':
                args.distance = Double_t(std::stod(optarg));
                args.b_distance = kTRUE;
                break;
            case 'h':
            std::cout<<"Single Run Analysis"<<std::endl;
            std::cout<<"-i \t input    \t input file(root) " <<std::endl;
            std::cout<<"-o \t output   \t output file" <<std::endl;
            std::cout<<"-d \t distance \t clustering distance" <<std::endl;
            std::cout<<"-h \t help     \t this message" <<std::endl;
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
    Analysis(ifile, ofile, args.distance);

    ifile->Close("R");
    ofile->Close("R");

    delete ifile;
    delete ofile;

    return 0;
}