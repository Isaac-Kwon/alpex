#ifndef TRUN_H 
#define TRUN_H 1;

#include "map"

#include "TROOT.h"
#include "TObject.h"
#include "TNamed.h"
#include "TMap.h"
#include "TTree.h"
#include "TString.h"

class TRunBulk;

class TRun: public TNamed{
    private:
    std::map<std::string, TTree *> fTrees;
    std::map<std::string, int> fValues;
    std::map<std::string, std::string> fStrings;
    protected:
    public:
    TRun();
    TRun(const char * name, const char * title);
    Bool_t      SetValue(std::string name, int value) {return }
    int         GetValue(std::string name);
    Bool_t      SetString(std::string name, std::string value);
    std::string GetString(std::string name);
    Bool_t      AddTree(std::string name, TTree * tree) {fTrees[name] = }
    TTree     * GetTree(std::string name);
    TRunBulk  * GetBulk();
    TString     Print();

    friend class TRunBulk;

    ClassDef(TRun,10);
};

TRun::TRun(): TNamed(){

}

TRun::TRun(const char * name, const char * title): TNamed(name, title){

}

#endif