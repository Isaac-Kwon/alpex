#ifndef __TANALYSISTASKCLUSTERING__
#define __TANALYSISTASKCLUSTERING__ 1

#include "TROOT.h"
#include "TString.h"
#include "TTree.h"

#include "TAnalysisTask.hpp"

#include "TEvent.hpp"
#include "TPixel.hpp"
#include "TCluster.hpp"

#include "TMap2Base.hpp"

class TFile;
class TTree;
class TEvent;
class TH2D;

typedef TMap2Base<Short_t, Int_t> ChipMap;

class TAnalysisTaskClustering: public TAnalysisTask{
    private:
    TString finputfname;
    TString foutputfname;
    // TFile * finfile = 0;
    // TFile * foutfile = 0;
    Bool_t  finputSelf = kFALSE;
    Bool_t  foutputSelf = kFALSE;

    TTree * thit = 0;

    TTree * tevent = 0; //Output 1
    TTree * tcluster = 0; //Output 2
    
    TEventDump   eventdump;
    TClusterDump clusterdump;

    Double_t fDistance=0;

    TEvent * RecordingEvent; // Event Structure
    TH2D * h1 = 0;
    TH2D * h2 = 0;
    TH2D * h3 = 0;
    TH2D * h4 = 0;
    TH2D * h5 = 0;

    TH2D * h_pre0 = 0;
    struct RStruct{
        UChar_t  chipid;
        UInt_t   t;
        UShort_t x;
        UShort_t y;
    };
    RStruct read;

    ChipMap* fNoiseMap = nullptr;
    Int_t fNoiseLevel=10;

    void    RecordEvent(TEvent * event);

    protected:
    void preAnalysis();
    public:
    TAnalysisTaskClustering(const char * name, const char * title, const char * inputfname, const char * outputfname);
    TAnalysisTaskClustering(const char * name, const char * title, TFile* inputfile, TFile* outputfile);
    TAnalysisTaskClustering(const char * inputfname, const char * outputfname);
    TAnalysisTaskClustering(TFile* inputfile, TFile* outputfile);
    // ~TAnalysisTaskClustering();
    void Clear();
    virtual void Init();
    virtual void Exec(Int_t verbose=-1, Int_t ratio=1);
    void SetDistance(Double_t distance){fDistance=distance;}
    virtual void Save(const char *fname, Bool_t closing);
};

#endif