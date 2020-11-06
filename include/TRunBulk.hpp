#ifndef TRUNBULK_H
#define TRUNBULK_H 1;

#include "iostream"
#include "fstream"
#include "string"
#include "sstream"
#include "deque"
#include "cctype"
#include "algorithm"
#include "cstdlib"

#include "TROOT.h"
#include "TString.h"
#include "TList.h"
#include "TTree.h"

#include "TRun.hpp"

class TRunBulk: public TNamed{
    private:
    TList fRuns;
    protected:
    public:
    TRunBulk();
    TRunBulk(const char * name, const char * title);
    // TString          Print();
    virtual Bool_t   LoadRuns(const char * runlistFilename);
    TRun * Get(int i){return (TRun*) fRuns.At(i);}
    TRun * At(int i){return Get(i);}
    Int_t  GetSize(){return fRuns.GetSize();}
    void   Print();

    ClassDef(TRunBulk,10);
};

TRunBulk::TRunBulk() : TNamed(), fRuns(){

}

TRunBulk::TRunBulk(const char * name, const char * title) : TNamed(name, title), fRuns(){

}

// Bool_t TRunBulk::AddRun(TRun * run){
//     if(fRuns==nullptr){
//         std::cout<<"TRunBulk::AddRun - List of run is not initialized."<<std::endl;
//         return kFALSE;
//     }
//     fRuns->Add(run);
//     return kTRUE;
// }

void removespace(std::string & str){
    str.erase(std::remove_if(str.begin(), str.end(), ::isspace), str.end());
}

Bool_t TRunBulk::LoadRuns(const char * runlistFilename){
    std::ifstream ifs;

    ifs.open(runlistFilename);
    if(!(ifs.is_open())){
        std::cout<<"TRunBulk::LoadRuns - Failed to open runlist file. filename = " << runlistFilename << std::endl;
        return kFALSE;
    }


    struct columnPair{
        std::string index;
        std::string name = 0;
        int datatype=-1;
        // 0: TTree {tree}
        // 1: DAC Value (int) {dac}
        // 2: text {text}
        columnPair(): index(""), datatype(-1), name("") {}
        columnPair(std::string index_, int datatype__, std::string name_="")
                 : index(index_), datatype(datatype__), name(name_) {}
        columnPair(const columnPair & pair)
                 : index(pair.index), datatype(pair.datatype), name(pair.name) {}
    };

    std::deque<columnPair> columns;

    std::string line, text;  // line >> line_stream >> text
    std::getline(ifs, line);

    std::cout<<"Loaded line"<<std::endl<< line << std::endl; //Dummy
    std::stringstream line_stream(line);

    for(int i=0; std::getline(line_stream,text,'\t'); i++){
        std::stringstream text_stream(text);
        std::string svalue;
        columnPair pair = columnPair();
        for(int j=0; std::getline(text_stream,svalue,'/'); j++){
            removespace(svalue);
            switch (j)
            {
                case 0:
                    pair.index = svalue;
                    break;
                case 1:
                    if(svalue == "tree"){
                        pair.datatype = 0;
                    }else if(svalue == "dac"){
                        pair.datatype = 1; 
                    }else if(svalue == "text"){
                        pair.datatype = 2; 
                    }else
                case 2:
                    if(pair.datatype!=0) break;
                    pair.name = svalue;
                default:
                    break;
            }
        }
        // std::cout<<i<<"\t"<<pair.index<<"\t"<<pair.datatype<<"\t"<<pair.name<<std::endl;
        columns.emplace_back(pair);
    }
    std::cout << std::endl;

    // Recognize Column Information (Initializa column)
    // Write Column Like following.
    // THR/tree/treename   FHR/tree/treename  Config/text  ITHR/dac    VCASN/dac ...

    for(int i=0; std::getline(ifs, line); i++){
        std::stringstream text_stream(line);
        std::string text;
        TRun * run = new TRun(TString::Format("%s_%d", TNamed::GetName(), i), TString::Format("%s_run_%d", TNamed::GetTitle(), i));
        fRuns.AddLast(run);
        run->fRunBulk = this;
        for(int j=0; std::getline(text_stream, text, '\t'); j++){
            columnPair pair = columns.at(j);
            switch (pair.datatype)
            {
                case 0: //TTree
                run->fTrees[pair.index] = (TTree *) (TFile::Open(text.c_str(),"READ")->Get(pair.name.c_str()));
                break;
                //
                case 1: //dac
                run->fValues[pair.index] = std::atoi(text.c_str());
                break;
                //
                case 2: //textstring
                run->fStrings[pair.index] = text;
                break;
                //
                default:
                break;
            }
        }
    }


    // Read Columns
    
    return kTRUE;
}

void TRunBulk::Print(){
    TNamed::Print();
    for(Int_t i=0; i<fRuns.GetSize(); i++){
        std::cout<<"N' of run = "<<i<<std::endl;
        Get(i)->Print();
        std::cout<<std::endl;
    }
}

#endif