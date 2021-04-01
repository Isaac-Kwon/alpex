#include "TRun.hpp"

#include "TObject.h"
#include "TTree.h"
#include "TString.h"
#include "iostream"

// #if !defined(__CLING__)
// ClassImp(TRun)
// #endif

TRun::TRun():
      fname(),
      ftitle(){
}

TRun::TRun(const char * name, const char * title):
      fname(name),
      ftitle(title){

}

void TRun::Print(){
    // TNamed::Print();

    std::cout<<"Trees"<<std::endl;
    for(std::map<std::string, TTree *>::iterator it = fTrees.begin();
        it != fTrees.end(); it++){
        std::cout<<"Key: "<<it->first <<std::endl;
        it->second->Print();
    }

    std::cout<<"Values (int)"<<std::endl;
    for(std::map<std::string, int>::iterator it = fValues.begin();
        it != fValues.end(); it++){
        std::cout<<"["<<it->first<<"] -> "<<it->second<<std::endl;
    }

    std::cout<<"Strings"<<std::endl;
    for(std::map<std::string, std::string>::iterator it = fStrings.begin();
        it != fStrings.end(); it++){
        std::cout<<"["<<it->first<<"] -> \""<<it->second<<"\""<<std::endl;
    }

    std::map<std::string, int> fValues;
    std::map<std::string, std::string> fStrings;

    
    
}