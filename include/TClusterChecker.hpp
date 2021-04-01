#ifndef __TCLUSTECHECKER__
#define __TCLUSTECHECKER__

#include "TROOT.h"

#include "deque"
#include "TCut.h"
#include "TPad.h"
#include "TH2D.h"
#include "TCanvas.h"


class TCluster;
class TNamed;
class TTree;

class TClusterChecker: public TNamed{
    private:
    TTree * tcluster=0;
    TCanvas * c1;
    TH2D *hs[5];
    TPad *pads[5];
    Int_t npads = 5;
    TCut ncut;
    TCut tcut[3];

    Int_t ranges[4] = {0,1024, 0, 512};
    Int_t timestamp = 0;
    struct TreeStructure{
        UChar_t  chipid=0;
        UInt_t   t;
        UShort_t npixel;
        UShort_t ncluster;
        UShort_t x_center;
    UShort_t y_center;
        UShort_t x[100];
        UShort_t y[100];
    };

    protected:
    TreeStructure read;
    
    public:
    TClusterChecker();
    TClusterChecker(const char* name, const char* title, TTree * tree=0);
    void Init();
    void InitDraw();
    void SetTree(TTree * ttree);
    void SetRangeXY(Int_t x1, Int_t x2, Int_t y1, Int_t y2);
    void SetRangeX(Int_t x1, Int_t x2);
    void SetRangeY(Int_t y1, Int_t y2);
    void SetTime(Int_t time=-1);
    void SetTimeFoward(Int_t dt){SetTime(timestamp+dt);}
    void SetTimeBackward(Int_t dt){SetTime(timestamp-dt);}
    std::deque<TCluster> SearchCluster(Int_t x1, Int_t x2, Int_t y1, Int_t y2, Float_t share=1.0, TCut cut = TCut());


    void DrawCanvas();
    void UpdateCanvas();

    void Draw();
    void Draw(const char * varexp, const char * selection);
    void Draw(const char * varexp, const TCut & selection);
    void Draw(const char * selection){Draw("y:x", selection);}
    void Draw(const TCut & selection){Draw("y:x", selection);}
    void Draw(const TCut cut1, const TCut cut2, const TCut cut3, const TCut cut4, const TCut cut5);

    void StartInterpreter();
    
};

#endif