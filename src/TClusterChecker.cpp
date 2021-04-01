#include "TClusterChecker.hpp"
#include "TCanvas.h"
#include "TString.h"
#include "TPad.h"
#include "TCut.h"
#include "TTimer.h"
#include "TTree.h"
#include "TH2D.h"
#include "iostream"
#include "TSystem.h"
#include "deque"
#include "TCluster.hpp"
#include "TEventList.h"
#include "TCluster.hpp"
#include "sstream"

// #if !defined(__CLING__)
// ClassImp(TClusterChecker)
// #endif

TClusterChecker::TClusterChecker(): TNamed("cc1", "TClusterChecker1"){
    Init();
}

TClusterChecker::TClusterChecker(const char* name, const char* title, TTree *tree): TNamed(name, title){
    SetTree(tree);
    Init();
    
}

void TClusterChecker::Init(){
    gROOT->cd();
    for(Int_t i=0; i<5; i++){
        hs[i] = new TH2D(TString::Format("%s_h%d", GetName(), i), TString::Format("%s_h%d", GetName(), i), 1024, 0, 1024, 512, 0, 512);
    }
    if(tcluster != 0) InitDraw();
    hs[0]->SetTitle("Whole Time");
    hs[1]->SetTitle("Whole Time (S.pos)");
    hs[2]->SetTitle("Time  -  1");
    hs[3]->SetTitle("Time +/- 0");
    hs[4]->SetTitle("Time  +  1");

}

void TClusterChecker::InitDraw(){
    for(Int_t i=0; i<5; i++){
        tcluster->Draw(TString::Format("%s>>%s","y:x",hs[i]->GetName()),"", "goff");
    }
}

void TClusterChecker::DrawCanvas(){
    c1 = new TCanvas(TString::Format("%s_%s", GetName(), "c1"), TString::Format("%s_%s", GetTitle(), "c1"), 1800, 800);
    c1->Divide(1,2);
    c1->cd(1)->Divide(2,1);
    c1->cd(2)->Divide(3,1);
    pads[0] = (TPad*) c1->cd(1)->cd(1);
    pads[1] = (TPad*) c1->cd(1)->cd(2);
    pads[2] = (TPad*) c1->cd(2)->cd(1);
    pads[3] = (TPad*) c1->cd(2)->cd(2);
    pads[4] = (TPad*) c1->cd(2)->cd(3);

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

void TClusterChecker::UpdateCanvas(){
    for(int i=0; i<5; i++){
        pads[i]->Modified();
        pads[i]->Update();
    }

    c1->Modified(); c1->Update();
}

void TClusterChecker::SetRangeXY(Int_t x1, Int_t x2, Int_t y1, Int_t y2){
    ranges[0] = x1;
    ranges[1] = x2;
    ranges[2] = y1;
    ranges[3] = y2;

    for(Int_t i=1; i<5; i++){
        hs[i]->GetXaxis()->SetRangeUser(x1, x2);
        hs[i]->GetYaxis()->SetRangeUser(y1, y2);
    }
}

void TClusterChecker::SetRangeX(Int_t x1, Int_t x2){
    ranges[0] = x1;
    ranges[1] = x2;
    for(Int_t i=1; i<5; i++){
        hs[i]->GetXaxis()->SetRangeUser(x1, x2);
    }
}

void TClusterChecker::SetRangeY(Int_t y1, Int_t y2){
    ranges[2] = y1;
    ranges[3] = y2;
    for(Int_t i=1; i<5; i++){
        hs[i]->GetYaxis()->SetRangeUser(y1, y2);
    }
}

void TClusterChecker::SetTree(TTree * tree){
    tcluster = tree;
    tcluster->SetBranchAddress("id"      , &read.chipid   );
    tcluster->SetBranchAddress("t"       , &read.t        );
    tcluster->SetBranchAddress("npixel"  , &read.npixel   );
    tcluster->SetBranchAddress("ncluster", &read.ncluster );
    tcluster->SetBranchAddress("Xcenter" , &read.x_center );
    tcluster->SetBranchAddress("Ycenter" , &read.y_center );
    tcluster->SetBranchAddress("x"       ,  read.x        );
    tcluster->SetBranchAddress("y"       ,  read.y        );
}

void TClusterChecker::SetTime(Int_t time){
    if(time <= 0){
        tcut[0] = TCut();
        tcut[1] = TCut();
        tcut[2] = TCut();
        Draw();
        timestamp=-1;
        return;
    }
    std::cout<<"Set Time to "<<time<<std::endl;
    timestamp = time;
    tcut[0] = TCut(TString::Format("t==%d", time-1));
    tcut[1] = TCut(TString::Format("t==%d", time));
    tcut[2] = TCut(TString::Format("t==%d", time+1));
    Draw();
}

void TClusterChecker::Draw(){
    Draw(ncut, ncut, ncut&&tcut[0],ncut&&tcut[1], ncut&&tcut[2]);
}

void TClusterChecker::Draw(const char * varexp, const char * selection){
    for(Int_t i=0; i<5; i++){
        tcluster->Draw(TString::Format("%s>>%s",varexp,hs[i]->GetName()),selection, "goff");
    }
    SetRangeXY(ranges[0], ranges[1], ranges[2], ranges[3]);
}

void TClusterChecker::Draw(const char * varexp, const TCut & selection){
    for(Int_t i=0; i<5; i++){
        tcluster->Draw(TString::Format("%s>>%s",varexp,hs[i]->GetName()),selection, "goff");
    }
    SetRangeXY(ranges[0], ranges[1], ranges[2], ranges[3]);
}

void TClusterChecker::Draw(const TCut cut1, const TCut cut2, const TCut cut3, const TCut cut4, const TCut cut5){
    tcluster->Draw(TString::Format("y:x>>%s",hs[0]->GetName()),cut1, "goff");
    tcluster->Draw(TString::Format("y:x>>%s",hs[1]->GetName()),cut2, "goff");
    tcluster->Draw(TString::Format("y:x>>%s",hs[2]->GetName()),cut3, "goff");
    tcluster->Draw(TString::Format("y:x>>%s",hs[3]->GetName()),cut4, "goff");
    tcluster->Draw(TString::Format("y:x>>%s",hs[4]->GetName()),cut5, "goff");
    SetRangeXY(ranges[0], ranges[1], ranges[2], ranges[3]);
}

//Scope between X=[x1,x2] Y=[y1,y2]
std::deque<TCluster> TClusterChecker::SearchCluster(Int_t x1,Int_t x2,Int_t y1,Int_t y2, Float_t share, TCut cut){
    std::deque<TCluster> ans;
    if(x1>x2){ Int_t xtemp=x2; x2 = x1; x1 = xtemp;}
    if(y1>y2){ Int_t ytemp=y2; y2 = y1; y1 = ytemp;}

    TEventList * evlist = new TEventList("evlist");
    tcluster->Draw(">>evlist", cut);
    for(int i=0; i<evlist->GetN(); i++){
        tcluster->GetEntry(evlist->GetEntry(i));
        int npixel = read.npixel;
        int pixelshare = 0;
        for(int ipix=0; ipix<npixel; ipix++){
            if(! (x1 < read.x[ipix] && read.x[ipix] < x2)) continue;
            if(! (y1 < read.y[ipix] && read.y[ipix] < y2)) continue;
            pixelshare++;
            // std::cout<< "[" << read.x[ipix] << ", " << read.y[ipix] << "]\t" << pixelshare << " in " << npixel <<std::endl;
        }
        if(pixelshare >= share*npixel){
            std::deque<TPixel> pixdeque;
            for(int ipix=0; ipix<npixel; ipix++){
                TPixel pix = TPixel(read.x[ipix],read.y[ipix],read.t);
                pixdeque.emplace_back(pix);
            }
            ans.emplace_back(TCluster(pixdeque));
        }
    }
    evlist->Delete();
    return ans;
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
    TTimer *timer = new TTimer(c1, 500);
    timer->Connect("Timeout()", "TCanvas", c1, "Modified()");
    UpdateCanvas();
    while(continuer){
        UpdateCanvas();
        // {c1->WaitPrimitive();}
        std::cout<<std::endl<<"CCInterpreter >> ";
        std::getline(std::cin, line);
        std::stringstream linestream(line);
        std::string word;
        if(std::cin.eof()){
            std::cout<<"EOF (CTRL+D)"<<std::endl;
            break;
        }
        for(int i=0; std::getline(linestream, word); i++){
            // timer->Start();
            int length = word.length();
            char order = word.at(0);
            Int_t         val1 = -1;
            Int_t         val2 = -1;
            char addi;
            if(length>1){
                addi  = word.at(1);
                word.erase(0,1);
                if(addi == '+' || addi == '-') word.erase(0,1);

                std::string s_val1, s_val2;
                Int_t         dpos = word.find('/');
                if(dpos == word.npos){
                    val1 = std::atoi(word.c_str());
                }else{
                    s_val1 = word.substr(0,dpos);
                    s_val2 = word.substr(dpos+1,word.npos);
                    val1 = std::atoi(s_val1.c_str());
                    val2 = std::atoi(s_val2.c_str());
                }
            }

            if(order == 'p'){
                //SetRangeXY
                std::cout<<"SetRangeXY"<<std::endl;
                std::cout<<">> X=["<< val1-10 << ","<<val1+10 << "]" << "\t Y=[" << val2-10 << "," << val2+10 <<"]" << std::endl;
                SetRangeXY(val1-20, val1+20, val2-20, val2+20);
                //=============================================//
            }else if(order == 'x'){
                //SetRangeX
                std::cout<<"SetRangeX"<<std::endl;
                std::cout<<">> X=["<< val1 <<","<< val2 <<"]"<< std::endl;
                SetRangeX(val1, val2);
                //=============================================//
            }else if(order == 'y'){
                //SetRangeY
                std::cout<<"SetRangeY"<<std::endl;
                std::cout<<">> Y=["<< val1 <<","<< val2 <<"]"<< std::endl;
                SetRangeY(val1, val2);
                //=============================================//
            }else if(order == 't'){
                //Time Operations
                std::cout<<"SetTimeFrame"<<std::endl;
                if(addi == '+'){
                    std::cout<<"> Forwards"<<std::endl; //Should Update
                    std::cout<<">> "<< val1 << std::endl;
                    SetTimeFoward(val1);
                }else if(addi == '-'){
                    std::cout<<"> Backward"<<std::endl; //Should Update
                    std::cout<<">> "<< val1 << std::endl;
                    SetTimeBackward(val1);
                }else{
                    // if(val1<=0) SetTime();
                    std::cout<<"> SpecificTime"<<std::endl; //Should Update
                    std::cout<<">> "<< val1 << std::endl;
                    SetTime(val1);
                }
                //=============================================//
            }else if(order == 'f'){
                std::cout<<"Find Cluster This Scope \t" << (ncut && tcut[1]).GetTitle() <<std::endl;
                std::cout<<"in " << "X = [" << ranges[0] <<"," << ranges[1] <<"] Y = [" << ranges[2] <<","<< ranges[3]<<"]" <<std::endl;
                std::deque<TCluster> clusters = SearchCluster(ranges[0], ranges[1], ranges[2], ranges[3], 1.0, ncut && tcut[1]);
                std::cout<< "> " << clusters.size() << " clusters found"<<std::endl;
                for(int i=0; i<clusters.size(); i++){
                    std::cout<<"Cluster # "<<i<<std::endl;
                    clusters.at(i).Print();
                }
                //=============================================//
            }else if(order == 'n'){
                std::cout<<"npixel Cut in each cluster"<<std::endl;
                if(addi == '+'){
                    std::cout<<"> Larger (and eq) than"<<std::endl;
                    std::cout<<">> "<< val1 << std::endl;
                    ncut = TCut(TString::Format("npixel >= %d", val1));
                }else if(addi == '-'){
                    std::cout<<"> Less (and eq) than"<<std::endl;
                    std::cout<<">> " << val1 << std::endl;
                    ncut = TCut(TString::Format("npixel <= %d", val1));
                }else{
                    if(val2>0){
                        std::cout<<"Between"<<std::endl;
                        std::cout<< ">> " << val1 << "\t" << val2 <<std::endl;
                        ncut = TCut(TString::Format("npixel >= %d && npixel <= %d", val1, val2));
                    }else{
                        std::cout<<"Specific Number"<<std::endl;
                        std::cout<< ">> " << val1 << std::endl;
                        ncut = TCut(TString::Format("npixel == %d", val1));
                        if(val1==-1){ncut=TCut(); Draw();}
                    }
                }
                Draw();
                //=============================================//
            }else if(order == 'u'){
                std::cout<<"Update Canvas"<<std::endl;
                c1->Modified();
                c1->Update();
                gSystem->ProcessEvents();
                //=============================================//
            }else if(order == 'q'){
                std::cout<<"Quit Interpreter"<<std::endl;
                continuer = false;
            }else if(order == 'h'){
                std::cout<< "p[n1]/[n2]\t: Set Range X = [n1-10, n1+10] and Y = [n2-10, n2+10]"<<std::endl;
                std::cout<< "x[n1]/[n2]\t: Set Range X = [n1, n2]"<<std::endl;
                std::cout<< "y[n1]/[n2]\t: Set Range Y = [n1, n2]"<<std::endl;
                std::cout<<std::endl;
                std::cout<< "t\t:Time Operation" << std::endl;
                std::cout<< "t[n]\t: Set Timestamp [n]"<<std::endl;
                std::cout<< "t+[n]\t: Go time after [n]"<<std::endl;
                std::cout<< "t+[n]\t: Go time before [n]"<<std::endl;
                std::cout<< std::endl;
                std::cout<< "n\t: Cut with npixel in cluster" << std::endl;
                std::cout<< "n+[n]\t: npixel larger(and eq) than n" <<std::endl;
                std::cout<< "n-[n]\t: npixel less(and eq) than n" <<std::endl;
                std::cout<< "n[n]`\t: Specific npixel n" <<std::endl;
                std::cout<< "n[n1]/[n2]\t: npixel in range [n1, n2]"  <<std::endl;
            }else{
                std::cout<<"Cannot recognize the order \'" << order <<"\' in " << order<<word <<std::endl;
            }
            if(!continuer) break;
        }
        UpdateCanvas();
        gSystem->ProcessEvents();

        // c1->WaitPrimitive();
    }
}