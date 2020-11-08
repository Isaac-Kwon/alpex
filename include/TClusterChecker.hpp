#include "TROOT.h"
#include "TCanvas.h"
#include "TPad.h"
#include "TTree.h"
#include "TH2D.h"
#include "iostream"
#include "sstream"
#include "TSystem.h"

class TClusterChecker: public TNamed{
    private:
    TTree * tcluster;
    TCanvas * c1;
    TH2D *hs[5];
    protected:
    
    public:
    TClusterChecker();
    TClusterChecker(const char* name, const char* title);
    void Init();
    void SetTree(TTree * ttree){tcluster = ttree;}
    void SetRangeUser(Int_t x1, Int_t x2, Int_t y1, Int_t y2);
    void SetTimeStamp(Int_t time1, Int_t time2=-1);

    void DrawCanvas();
    void UpdateCanvas(){c1->Modified(); c1->Update();}

    void Draw(const char * varexp, const char * selection);
    void Draw(const char * varexp, const TCut & selection);
    void Draw(const char * selection){Draw("y:x", selection);}
    void Draw(const TCut & selection){Draw("y:x", selection);}

    void StartInterpreter();
    

    ClassDef(TClusterChecker,10);
};

TClusterChecker::TClusterChecker(): TNamed("cc1", "TClusterChecker1"){
    // Init();
}

TClusterChecker::TClusterChecker(const char* name, const char* title): TNamed(name, title){
    // Init();
}

void TClusterChecker::Init(){
    gROOT->cd();
    for(Int_t i=0; i<5; i++){
        hs[i] = new TH2D(TString::Format("%s_h%d", GetName(), i), TString::Format("%s_h%d", GetName(), i), 1024, 0, 1024, 512, 0, 512);
    }
}

void TClusterChecker::DrawCanvas(){
    c1 = new TCanvas(TString::Format("%s_%s", GetName(), "c1"), TString::Format("%s_%s", GetTitle(), "c1"), 1800, 800);
    c1->Divide(1,2);
    c1->cd(1)->Divide(2,1);
    c1->cd(2)->Divide(3,1);
    
    c1->cd(1)->cd(1);
    hs[0]->Draw("COLZ");
    c1->cd(1)->cd(2);
    hs[1]->Draw("COLZ");
    c1->cd(2)->cd(1);
    hs[2]->Draw("COLZ");
    c1->cd(2)->cd(2);
    hs[3]->Draw("COLZ");
    c1->cd(2)->cd(3);
    hs[4]->Draw("COLZ");

}

void TClusterChecker::SetRangeUser(Int_t x1, Int_t x2, Int_t y1, Int_t y2){
    for(Int_t i=1; i<5; i++){
        hs[i]->GetXaxis()->SetRangeUser(x1, x2);
        hs[i]->GetYaxis()->SetRangeUser(y1, y2);
    }
}

void TClusterChecker::Draw(const char * varexp, const char * selection){
    gROOT->cd();
    for(Int_t i=0; i<5; i++){
        tcluster->Draw(TString::Format("%s>>%s",varexp,hs[i]->GetName()),selection, "goff");
    }
}

void TClusterChecker::Draw(const char * varexp, const TCut & selection){
    gROOT->cd();
    for(Int_t i=0; i<5; i++){
        tcluster->Draw(TString::Format("%s>>%s",varexp,hs[i]->GetName()),selection, "goff");
    }
}

void TClusterChecker::StartInterpreter(){

    // p200/120     : SetRange X=[190,210] Y=[110,130] 
    // x10/20       : SetRangeX [10,20]
    // t12500       : Set Specific time 12500
    // t+1  or t-1  : timeframe +1 / -1
    // q            : Finish Interpreter
    std::cout<<"Start Interpreter of TClusterChecker " << GetName() <<std::endl;
    std::string line;
    Bool_t continuer=true;
    TTimer *timer = new TTimer();
    // timer->Connect("Timeout()", "TCanvas", c1, "Update()");
    while(continuer){
        gSystem->ProcessEvents();
        // {c1->WaitPrimitive();}
        std::cout<<"CCInterpreter >> ";
        std::getline(std::cin, line);
        std::stringstream linestream(line);
        std::string word;
        if(std::cin.eof()){
            std::cout<<"EOF (CTRL+D)"<<std::endl;
            break;
        }
        for(int i=0; std::getline(linestream, word); i++){
            timer->Start(10, kFALSE);
            if(word.at(0) == 'p'){
                std::cout<<"SetRangeXY"<<std::endl;
                word.erase(0,1);
            }else if(word.at(0) == 'x'){
                std::cout<<"SetRangeX"<<std::endl;
                word.erase(0,1);
            }else if(word.at(0) == 'y'){
                std::cout<<"SetRangeY"<<std::endl;
                word.erase(0,1);
            }else if(word.at(0) == 't'){
                std::cout<<"SetTimeFrame"<<std::endl;
                word.erase(0,1);
                if(word.at(0) == '+'){
                    std::cout<<"front"<<std::endl;
                    word.erase(0,1);
                }else if(word.at(0) == '-'){
                    std::cout<<"front"<<std::endl;
                    word.erase(0,1);
                }else{
                    std::cout<<"specific time"<<std::endl;
                    word.erase(0,1);
                }
            }else if(word.at(0) == 's'){
                std::cout<<"Selection"<<std::endl;
                word.erase(0,1);
            }else if(word.at(0) == 'q'){
                std::cout<<"Quit Interpreter"<<std::endl;
                continuer = false;
            }else if(word.at(0) == 'h'){
                std::cout<< "p[n1]/[n2] : Set Range X = [n1-10, n1+10] and Y = [n2-10, n2+10]"<<std::endl;
                std::cout<< "x[n1]/[n2] : Set Range X = [n1, n2]"<<std::endl;
                std::cout<< "y[n1]/[n2] : Set Range Y = [n1, n2]"<<std::endl;
                std::cout<< "t[n]       : Set Timestamp [n]"<<std::endl;
                std::cout<< "t+[n]      : Go time after [n]"<<std::endl;
                std::cout<< "t+[n]      : Go time before [n]"<<std::endl;
            }else{
                std::cout<<"Cannot recognize the order \'" << word.at(0) <<"\' in " << word <<std::endl;
            }
            if(!continuer) break;
        }
    }




}