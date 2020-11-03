#ifndef TANALYSISTASK_H
#define TANALYSISTASK_H 1;

#include "TROOT.h"
#include "TTask.h"
#include "TDirectory.h"
#include "TObjArray.h"
#include "TFile.h"
#include "TKey.h"

#include "TCluster.hpp"
#include "TEvent.hpp"
#include "TPixel.hpp"

class TAnalysisTask : public TTask{
    private:

    protected:
    TObjArray * finput;
    TObjArray * foutput;

    TFile * foutputfile;

    public:
    TAnalysisTask();
    TAnalysisTask(const char *name, const char *title);
    TAnalysisTask(const TAnalysisTask &task); 

    //=====================================================================
    // === OVERLOAD THIS AND CREATE YOUR OUTPUT OBJECTS (HISTOGRAMS,DATA) HERE
    virtual void Init(){;}
    virtual void Exec(Option_t *option){;}
    virtual void Save(const char *fname=0, Bool_t closing=kTRUE);
};

TAnalysisTask::TAnalysisTask()
{}

TAnalysisTask::TAnalysisTask(const char *name, const char *title)
              :TTask(name, title)
{}

TAnalysisTask::TAnalysisTask(const TAnalysisTask &task)
              :TTask(task)
{}

void TAnalysisTask::Save(const char *fname, Bool_t closing){
    
    foutput->Print();

    if(foutputfile==nullptr){
        if(fname == 0){
            foutputfile = new TFile("taskresult.root", "RECREATE");
        }else{
            foutputfile = new TFile(fname, "RECREATE");
        }
    }

    foutputfile->cd();
    foutputfile->Add(foutput->Clone(),kTRUE);
    foutputfile->Write();
};

#endif