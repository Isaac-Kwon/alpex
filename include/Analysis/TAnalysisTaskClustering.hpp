#ifndef TANALYSISTASKCLUSTERING_H
#define TANALYSISTASKCLUSTERING_H 1;

#include "TROOT.h"
#include "TString.h"
#include "TTree.h"
#include "TH2D.h"
#include "TCut.h"
#include "TFile.h"

#include "TAnalysisTask.hpp"

#include "TPixel.hpp"
#include "TCluster.hpp"
#include "TEvent.hpp"

class TAnalysisTaskClustering: public TAnalysisTask{
    private:
    TString finputfname;
    TString foutputfname;
    TTree * thit;
    TTree * tcluster;
    TH2D * h1;
    TH2D * h2;
    TH2D * h3;
    TH2D * h4;
    struct RStruct{
        UChar_t  chipid;
        UInt_t   t;
        UShort_t x;
        UShort_t y;
    };
    RStruct read;

    struct WStruct{
        UChar_t  chipid=0;
        UInt_t   t;
        UShort_t npixel;
        UShort_t ncluster;
        UShort_t x_center;
        UShort_t y_center;
        UShort_t x[100];
        UShort_t y[100];
    };
    WStruct write;
    void    RecordEvent(TEvent * event);

    protected:
    public:
    TAnalysisTaskClustering(const char * name, const char * title, const char * inputfname, const char * outputfname);
    TAnalysisTaskClustering(const char * inputfname, const char * outputfname);
    virtual void Init();
    virtual void Exec();
    void Save2();

};

TAnalysisTaskClustering::TAnalysisTaskClustering(const char * name, const char * title, const char * inputfname, const char * outputfname)
                         :TAnalysisTask(name,title),
                          finputfname(inputfname),
                          foutputfname(outputfname)
{
    Init();
}

TAnalysisTaskClustering::TAnalysisTaskClustering(const char * inputfname, const char * outputfname)
                        :TAnalysisTask("task","analysis task for clustering"),
                         finputfname(inputfname),
                         foutputfname(outputfname)
{
    Init();
}


void TAnalysisTaskClustering::Init(){

    finput = new TObjArray();//("indir","TDirectory for Clustering Analysis (InputFile)");

    //======================================================================//
    // Input Treeing
    TFile * infile = TFile::Open(finputfname, "READ");
    thit = (TTree*) infile->Get("hit");
    thit->SetBranchAddress("ChipID",    &read.chipid);
    thit->SetBranchAddress("TimeStamp", &read.t);
    thit->SetBranchAddress("X",         &read.x);
    thit->SetBranchAddress("Y",         &read.y);
    
    finput->Add(thit->Clone());

    foutputfile = new TFile(foutputfname, "RECREATE");
    foutputfile->cd();
    //======================================================================//
    // Output Treeing
    tcluster = new TTree("tcluster", "Tree for Clusters");
    tcluster->Branch("id"      , &write.chipid  , "id/b");
    tcluster->Branch("t"       , &write.t       , "t/i");
    tcluster->Branch("npixel"  , &write.npixel  , "npixel/s");
    tcluster->Branch("ncluster", &write.ncluster, "ncluster/s");
    tcluster->Branch("Xcenter" , &write.x_center, "xcenter/s");
    tcluster->Branch("Ycenter" , &write.y_center, "ycenter/s");
    tcluster->Branch("x"       ,  write.x       , "x[npixel]/s");
    tcluster->Branch("y"       ,  write.y       , "y[npixel]/s");
    
    h1               = new TH2D("h1", "Hitmap", 1024, 0, 1024, 512, 0, 512);
    h1->GetXaxis()->SetTitle("PixelX");
    h1->GetYaxis()->SetTitle("PixelY");
    h2               = new TH2D("h2", "CluterCenter", 1024, 0, 1024, 512, 0, 512);
    h2->GetXaxis()->SetTitle("PixelX");
    h2->GetYaxis()->SetTitle("PixelY");
    h3               = new TH2D("h3", "CluterShape", 10, -5, 5, 10, -5, 5);
    h3->GetXaxis()->SetTitle("PixelX - Mean(PixelX s)");
    h3->GetYaxis()->SetTitle("PixelY - Mean(PixelY s)");
    h4               = new TH2D("h4", "CluterCenter", 16, 0, 1024, 8, 0, 512);
    h4->GetXaxis()->SetTitle("PixelX");
    h4->GetYaxis()->SetTitle("PixelY");

    foutput = new TObjArray(); //new TDirectory("outdir","TDirectory for Clustering Analysis (OutputFile)");
    foutput->Add(tcluster);
    foutput->Add(h1);
    foutput->Add(h2);
    foutput->Add(h3);
    foutput->Add(h4);

    
}

void TAnalysisTaskClustering::Exec(){
    thit->GetEntry(0);
    UInt_t t_prev = read.t;
    TEvent * event = new TEvent();
    for(Int_t i=0; i<thit->GetEntries(); i++){
        thit->GetEntry(i);
        if(t_prev != read.t){
            // std::cout<<"NOW i = " << i << " t= " << t_prev <<std::endl;
            event->Clustering();
            // std::cout<<"\t Finished"<<std::endl;
            RecordEvent(event);
        }
        t_prev = read.t;
        event->AddPixel(TPixel(read.x, read.y, read.t));
    }

    TCut cut = TCut("npixel>=1");
    tcluster->Draw("y:x >> h1", cut, "goff");
    tcluster->Draw("ycenter:xcenter >> h2", cut, "goff");
    tcluster->Draw("y-ycenter:x-xcenter >> h3", cut, "goff");
    tcluster->Draw("ycenter:xcenter >> h4", cut, "goff");
}

void TAnalysisTaskClustering::RecordEvent(TEvent * event){
    // event->Print();
    for(Int_t i=0; i<event->GetNCluster(); i++){
        TCluster cluster = event->GetCluster(i);
        
        write.t = cluster.GetPixel(0).GetT();
        write.npixel = cluster.GetNPixel();
        write.ncluster = event->GetNCluster();
        
        UInt_t sumx=0;
        UInt_t sumy=0;
        
        for(Int_t i=0; i<cluster.GetNPixel(); i++){
            TPixel pix = cluster.GetPixel(i);
            sumx += pix.GetX();
            sumy += pix.GetY();
            write.x[i] = UShort_t(pix.GetX());
            write.y[i] = UShort_t(pix.GetY());
        }
        write.x_center = UShort_t(sumx/cluster.GetNPixel());
        write.y_center = UShort_t(sumy/cluster.GetNPixel());
        tcluster->Fill();
    }
    event->Clear();
}


#endif