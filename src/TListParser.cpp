#include "TListParser.hpp"

#include "iostream"
#include "fstream"
#include "sstream"
#include "algorithm"

#include "TFile.h"
#include "TTree.h"
#include "TFolder.h"
#include "TSystem.h"

TListParser::TListParser(){
}

void TListParser::AddColumn(TString key, Int_t typenumber){
    if(fdata.size()>0){
        std::cerr << "Data is not empty, data before this operation can have missing value for this key" <<std::endl;
    }
    fkeys.emplace_back(key, typenumber);
}

std::map<TString, TPData> TListParser::Get(Int_t i){
    return fdata.at(i);
}

Int_t TListParser::GetN(){
    return fdata.size();
}

Int_t TListParser::ParsingFile(TString filename, Bool_t firstlabel){
    //Open File
    std::ifstream ifs;
    ifs.open(filename.Data());
    if(!(ifs.is_open())){
        std::cout<<"TListParser::ParsingFile - Failed to open file. filename = " << filename << std::endl;
        return -1;
    }

    if(firstlabel){
        // Parsing first line
        std::string line, text;
        std::getline(ifs, line);
        std::stringstream line_stream(line);

        for(int i=0; std::getline(line_stream,text,'\t'); i++){
            std::stringstream text_stream(text);
            std::string svalue;
            
            TPColumn col;
            for(int j=0; std::getline(text_stream,svalue,'/'); j++){
                TListParser::removespace(svalue);
                switch (j)
                {
                    case 0:
                        col.fname = svalue;
                        break;
                    case 1:
                        {
                            Bool_t brk=kFALSE;
                            for(const auto& [num, chr] : ftypenumber){
                                if(TString(svalue) == chr){
                                    col.ftype=num;
                                    brk=kTRUE;
                                }
                                if(brk) break;
                            }
                        }
                        break;
                    case 2:
                        if(col.ftype!=0) break;
                        col.fobjname = svalue;
                    default:
                        break;
                }
            }
            fkeys.emplace_back(col);
        }
    // std::cout<<i<<"\t"<<pair.index<<"\t"<<pair.datatype<<"\t"<<pair.name<<std::endl;
    
    }else{
        if(fkeys.size()<1){
            std::cerr << "TListParser::ParsingFile - key is empty, return" << std::endl;
            return -1;
        }
    }

    {
        TSystem sys;
        TFile * file;
        TTree * tree;
        std::string line, text;
        // std::getline(ifs, line);
        // std::cout<<"line\t"<<line<<std::endl;
        for(int i=0; std::getline(ifs, line); i++){
            std::stringstream text_stream(line);
            std::string text;
            fdata.emplace_back();
            for(int j=0; std::getline(text_stream, text, '\t'); j++){
                TPColumn column = fkeys.at(j);
                switch (column.ftype){
                    case 0: //TTree
                    file = TFile::Open(TString(sys.DirName(filename))+"/"+TString(text.c_str()),"READ");
                    // std::cout<<"Open File : "<<TString(sys.DirName(filename))+"/"+TString(text.c_str()) << " :: " << "\"" << column.fobjname.Data() <<"\""<<std::endl;
                    tree = (TTree *) (file->Get(column.fobjname.Data()));
                    // std::cout<<"Tree Loaded : "<< tree<<std::endl;


                    fdata[i][column.fname].dTTree = tree;
                    break;
                    //
                    case 1: //Int
                    fdata[i][column.fname].dInt = std::atoi(text.c_str());
                    break;
                    //
                    case 2: //textstring
                    fdata[i][column.fname].dTString = text;
                    break;
                    //
                    case 3:
                    fdata[i][column.fname].dFloat = atof(text.c_str());
                    break;
                    //
                    default:
                    break;
                }
            }
        }
    }
}

void TListParser::Print(){
    
    std::cout<<"Keys"<<std::endl;
    for(auto it=fkeys.begin(); it!=fkeys.end(); it++){
        TString fname = it->fname;
        Int_t   ftype = it->ftype;
        TString fobjname = it->fobjname;
        std::cout<<fname<<"\t";
        std::cout<<ftype<<"("<<ftypenumber_Human.at(ftype)<<")";
        if(fobjname != ""){
            std::cout<<"\t"<<fobjname;
        }
        std::cout<<std::endl;
        // std::cout<<(((*it).fobjname==TString("")) ? "" : "/" ) << (*it).fobjname << std::endl;
    }

    std::cout<<std::endl;

    Int_t i=0;
    for(auto it2=fdata.begin(); it2!=fdata.end(); it2++){
        std::cout<<i++<<"\t";
        for(auto it=fkeys.begin(); it!=fkeys.end(); it++){
            TPData obj = it2->operator[](it->fname);
            switch(it->ftype){
                case 0:
                std::cout << obj.dTTree->GetDirectory()->GetName() + TString("/") + obj.dTTree->GetName();
                break;
                case 1:
                std::cout << obj.dInt;
                break;
                case 2:
                std::cout << obj.dTString;
                break;
                case 3:
                std::cout << obj.dFloat;
                break;
            }
            std::cout<<"\t";
        }
        std::cout<<std::endl;
    }

    return;

}