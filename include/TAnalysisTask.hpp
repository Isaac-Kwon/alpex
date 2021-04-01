#ifndef __TANALYSISTASK__
#define __TANALYSISTASK__

#include "TROOT.h"
#include "TTask.h"
#include "TObject.h"
#include "TFile.h"

class TFile;
class TObjArray;

class TAnalysisTask{
    private:

    protected:
    TObjArray * finput;
    TObjArray * foutput;

    TFile * foutputfile;

    TString fname, ftitle;
    public:
    TAnalysisTask();
    TAnalysisTask(const char *name, const char *title);
    TAnalysisTask(const TAnalysisTask &task); 
    virtual ~TAnalysisTask(); 

    //=====================================================================
    // === OVERLOAD THIS AND CREATE YOUR OUTPUT OBJECTS (HISTOGRAMS,DATA) HERE
    virtual void Init(){;}
    virtual void Exec(){;}
    virtual void Clear();
    virtual void Save(const char *fname=0, Bool_t closing=kTRUE);

    // ClassDef(TAnalysisTask,1);
};

// TAnalysisTask::TAnalysisTask()
// {}

// TAnalysisTask::TAnalysisTask(const char *name, const char *title)
//               :TTask(name, title)
// {}

// TAnalysisTask::TAnalysisTask(const TAnalysisTask &task)
//               :TTask(task)
// {}

// TAnalysisTask::~TAnalysisTask(){
//     Clear();
// }

// void TAnalysisTask::Clear(){
//     foutputfile->Close();
//     finput->Delete();
//     // foutput->Delete();
// }

// void TAnalysisTask::Save(const char *fname, Bool_t closing){
    
//     foutput->Print();

//     if(foutputfile==nullptr){
//         if(fname == 0){
//             foutputfile = new TFile("taskresult.root", "RECREATE");
//         }else{
//             foutputfile = new TFile(fname, "RECREATE");
//         }
//     }

//     foutputfile->cd();
//     foutputfile->Add(foutput->Clone(),kTRUE);
//     foutputfile->Write();
// };


#endif