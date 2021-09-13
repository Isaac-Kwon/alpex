#ifndef __TTHRESHOLDSCAN__
#define __TTHRESHOLDSCAN__ 

#include "TROOT.h"
#include "TString.h"
#include "TH2D.h"
#include "TTree.h"

#include "deque"
#include "thread"
#include "mutex"

class TH1D;
// class TTree;
class TH1F;
class TFile;

struct MeasureEntry{
    int col=-1;
    int address=-1;
    std::deque<float> x;
    std::deque<float> value;
};

class TThresholdScan{
    public:
    TThresholdScan(TString infile, TString outfile="result_threshold.root");

    void AnalyzeFile();
    void AnalyzeFileMT();
    
    // TH2D*  GetThresholdMap(){return h21;}
    // TH1D*  GetThresholdDistribution(){return h11;}
    TTree* GetTree(){return tree;}

    private:

    Bool_t GetThreshold(Double_t *thresh,Double_t *noise,Double_t *chi2);
    Bool_t GetThresholdMT(MeasureEntry* entry, Double_t *thresh, Double_t *noise, Double_t *chi2, const int n=0);
    void ResetData();
    void ProcessPixel(int col, int row);

    static Double_t erf(Double_t *xx, Double_t *par);

    static const Int_t nInj = 50;

    void ThreadInjector();
    void ThreadAnalyzer(const int n);

    std::thread* fInjector;
    std::deque<std::thread*> fAnalyzers;
    std::deque<MeasureEntry*> fEntry;

    std::mutex fMutexInjector;
    std::mutex fMutexTree;
    std::mutex fMutexFitter;

    TH2D *h2thres, *h2noise, *h2chi2;

    TString fInfileName;
    TString fOutfileName = "result_threshold.root";

    TFile* fOutfile;
    TH1D *h1thres, *h1noise, *h1chi2;

    TTree * tree;

    Double_t maxchi2 = 10;

    Float_t data[256];
    Float_t x   [256];
    Int_t   NPoints;

    Float_t ELECTRONS_PER_DAC = 10.;

    Int_t NPixels;

    Int_t NNostart;
    Int_t NChisq;

    Float_t FindStart();
    Float_t FindStartMT(MeasureEntry* entry);
    int AddressToColumn (int ARegion, int ADoubleCol, int AAddress);
    int AddressToRow    (int ARegion, int ADoubleCol, int AAddress);

    UShort_t tvX;
    UShort_t tvY;
    Float_t  tvThreshold;
    Float_t  tvNoise;
    Float_t  tvChi2;

    Bool_t fInjectorRun = kFALSE;

    
};

#endif