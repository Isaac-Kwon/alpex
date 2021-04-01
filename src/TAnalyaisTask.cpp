#include "TAnalysisTask.hpp"

#include "TDirectory.h"
#include "TObjArray.h"
#include "TFile.h"
#include "TKey.h"
#include "TTask.h"

// #if !defined(__CLING__)
// ClassImp(TAnalysisTask)
// #endif

TAnalysisTask::TAnalysisTask()
{}

TAnalysisTask::TAnalysisTask(const char *name, const char *title)
{
    fname = TString(name);
    ftitle = TString(title);
}

TAnalysisTask::TAnalysisTask(const TAnalysisTask &task)
{
    fname = task.fname;
    ftitle = task.ftitle;
}

TAnalysisTask::~TAnalysisTask(){
    Clear();
}

void TAnalysisTask::Clear(){
    foutputfile->Close();
    finput->Delete();
    // foutput->Delete();
}

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

