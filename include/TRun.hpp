#ifndef __TRUN__ 
#define __TRUN__

#include "map"
#include "string"
#include "TTree.h"

#include "TROOT.h"
#include "TNamed.h"

class TRunBulk;
// class TTree;

class TRun{
    private:
    std::map<std::string, TTree *> fTrees;
    std::map<std::string, int> fValues;
    std::map<std::string, std::string> fStrings;
    std::map<std::string, Float_t> fFloat;
    TRunBulk* fRunBulk = nullptr;
    TString fname;
    TString ftitle;
    protected:
    public:
    TRun();
    TRun(const char * name, const char * title);
    TString     GetName(){return fname;}
    TString     GetTitle(){return ftitle;}
    void        SetValue(std::string index, int value){fValues[index]=value;}
    int         GetValue(std::string index){return fValues[index];}
    void        SetString(std::string index, std::string value);
    std::string GetString(std::string index){return fStrings[index];}
    void        SetTree(std::string index, TTree * tree){fTrees[index]=tree;}
    TTree     * GetTree(std::string index){return fTrees[index];}
    void        SetFloat(std::string index, Float_t float_){fFloat[index]=float_;}
    Float_t     GetFloat(std::string index){return fFloat[index];}
    TRunBulk  * GetBulk(){return fRunBulk;}
    void        Print();

    friend class TRunBulk;

    // ClassDef(TRun,1);
};

// TRun::TRun(): TNamed(){

// }

// TRun::TRun(const char * name, const char * title): TNamed(name, title){

// }

// void TRun::Print(){
//     TNamed::Print();

//     std::cout<<"Trees"<<std::endl;
//     for(std::map<std::string, TTree *>::iterator it = fTrees.begin();
//         it != fTrees.end(); it++){
//         std::cout<<"Key: "<<it->first <<std::endl;
//         it->second->Print();
//     }

//     std::cout<<"Values (int)"<<std::endl;
//     for(std::map<std::string, int>::iterator it = fValues.begin();
//         it != fValues.end(); it++){
//         std::cout<<"["<<it->first<<"] -> "<<it->second<<std::endl;
//     }

//     std::cout<<"Strings"<<std::endl;
//     for(std::map<std::string, std::string>::iterator it = fStrings.begin();
//         it != fStrings.end(); it++){
//         std::cout<<"["<<it->first<<"] -> \""<<it->second<<"\""<<std::endl;
//     }

//     std::map<std::string, int> fValues;
//     std::map<std::string, std::string> fStrings;

    
    
// }

#endif