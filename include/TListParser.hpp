#ifndef __TLISTPARSER__
#define __TLISTPARSER__

#include "map"
#include "deque"
#include "string"

#include "TROOT.h"
#include "TString.h"
#include "TTree.h"

struct TPColumn{
    TString fname;
    Int_t   ftype;
    TString fobjname;
    TPColumn(): fname(), ftype(-1), fobjname(){;}
    TPColumn(TString name, Int_t type): fname(name), ftype(type){;}
    TPColumn(const TPColumn& other): fname(other.fname), ftype(other.ftype), fobjname(other.fobjname){;}
};

struct TPData{
    Int_t   ftype=-1;
    TTree*  dTTree = nullptr;
    Int_t   dInt   = -1;
    TString dTString  = "";
    Float_t dFloat = -1;

    TPData(): ftype(-1), dTTree(nullptr), dInt(-1), dTString(), dFloat(-1) {;}
    TPData(const TPData&  other):
        ftype(other.ftype), dTTree(other.dTTree), dInt(other.dInt),
        dTString(other.dTString), dFloat(other.dFloat){;}
};

class TListParser{
    private:
    std::deque<TPColumn> fkeys;
    std::deque<std::map<TString, TPData>> fdata;

    const std::map<Int_t, TString> ftypenumber_Human = {
        {0, "TTree"},
        {1, "Int_t"},
        {2, "TString"},
        {3, "Float_t"}
    };

    const std::map<Int_t, TString> ftypenumber = {
        {0, "T"},
        {1, "I"},
        {2, "S"},
        {3, "F"}
    };

    public:
    TListParser();

    std::map<TString, TPData> Get(Int_t i);
    Int_t GetN();
    void AddColumn(TString key, Int_t typenumber);

    Int_t ParsingFile(TString filename, Bool_t firstlabel=kTRUE);

    void Print();

    static void removespace(std::string & str){
        str.erase(std::remove_if(str.begin(), str.end(), ::isspace), str.end());
    }
    

};

#endif