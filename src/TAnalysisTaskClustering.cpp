#include "TAnalysisTaskClustering.hpp"

#include "TH2D.h"
#include "TFile.h"
#include "TEvent.hpp"
#include "TTimeStamp.h"

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

// TAnalysisTaskClustering::~TAnalysisTaskClustering(){
//     // thit->remo();
//     // tevent->remo();
//     // tcluster->remo();
//     TAnalysisTask::~TAnalysisTask();

// }

void TAnalysisTaskClustering::Clear(){
    tevent->Delete();
    tcluster->Delete();
    finput->Clear();
    foutput->Clear();
    h1->Delete();
    h2->Delete();
    h3->Delete();
    h4->Delete();
    if (finfile){
        finfile->Close("R");
        delete finfile;
        std::cout<<"TAnalysisTaskClustering::Clear - Close Infile"<<std::endl;
    }
    if (foutputfile){
        foutputfile->Close("R");
        delete foutputfile;
        std::cout<<"TAnalysisTaskClustering::Clear - Close Outfile"<<std::endl;
    }
}

void TAnalysisTaskClustering::Init(){

    finput = new TObjArray();//("indir","TDirectory for Clustering Analysis (InputFile)");

    //======================================================================//
    // Input Treeing
    finfile = new TFile(finputfname, "READ");
    thit = (TTree*) finfile->Get("hit");
    thit->SetBranchAddress("ChipID",    &read.chipid);
    thit->SetBranchAddress("TimeStamp", &read.t);
    thit->SetBranchAddress("X",         &read.x);
    thit->SetBranchAddress("Y",         &read.y);
    
    finput->Add(thit);


    foutputfile = new TFile(foutputfname, "RECREATE");
    foutputfile->cd();
    //======================================================================//
    // Output Treeing
    RecordingEvent = new TEvent();

    std::cout<<"Construct ClusterTree"<<std::endl;
    tcluster = new TTree("tcluster", "Tree for Clusters");
    tcluster->Branch("t", &(clusterdump.t), "t/I");
    tcluster->Branch("npixel", &(clusterdump.n), "npixel/s");
    tcluster->Branch("pixelx", clusterdump.x, "x[npixel]/s");
    tcluster->Branch("pixely", clusterdump.y, "y[npixel]/s");

    tcluster->Branch("cluster", &clusterdump, "centerx/s:centery/s:sizex/s:sizey/s:coreless/O");


    std::cout<<"Construct EventTree"<<std::endl;
    tevent = new TTree("tevent", "Tree for Events");
    tevent->Branch("t", &(eventdump.t), "t/I");
    tevent->Branch("npixel", &(eventdump.npixel), "npixel/s");
    tevent->Branch("ncluster", &(eventdump.ncluster), "ncluster/s");
    tevent->Branch("pixelx", eventdump.x, "x[npixel]/s");
    tevent->Branch("pixely", eventdump.y, "y[npixel]/s");
    tevent->Branch("clusterx", eventdump.centerx, "centerx[ncluster]/s");
    tevent->Branch("clustery", eventdump.centery, "centery[ncluster]/s");
    
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
    foutput->Add(tevent);
    foutput->Add(h1);
    foutput->Add(h2);
    foutput->Add(h3);
    foutput->Add(h4);
}

void TAnalysisTaskClustering::Exec(Int_t verbose, Int_t ratio){
    thit->GetEntry(0);
    UInt_t t_prev = read.t;
    TEvent * event = RecordingEvent;
    Int_t nentries = thit->GetEntries() / ratio;
    TTimeStamp starttime = TTimeStamp();

    std::cout << "Started at " << starttime.AsString() << std::endl;

    for(Int_t i=0; i<nentries; i++){
        thit->GetEntry(i);
        if(t_prev != read.t || i==nentries-1){
            if(i==nentries-1){
                event->Append(TPixel(read.x, read.y, read.t));
            }

            event->Clustering(fDistance);
            RecordEvent(event);
            
        }
        t_prev = read.t;
        event->Append(TPixel(read.x, read.y, read.t));
        if(verbose>=1 && i%verbose==0){
            TTimeStamp nowtime = TTimeStamp();
            Double_t timeelp = nowtime - starttime;
            Double_t ratio = (Float_t(i)/Float_t(nentries));
            Double_t timeest = timeelp/ratio;
            std::cout<< i << "\t/\t" << nentries <<
                "  ("<<TString::Format("%.2f", Float_t(i*100)/nentries)<<")\tt=" << read.t <<"\t Now n=\t" << tcluster->GetEntries() << " | " << timeelp <<" s. / " << timeest << " s. est. :: " << timeest - timeelp << " sec remained." << std::endl;
        }
    }

    TTimeStamp endtime = TTimeStamp();
    std::cout << "Finished at " << endtime.AsString() <<std::endl;
    std::cout << "Total elapsed time : " << starttime - endtime << " sec." << std::endl;

    tcluster->Draw("y:x >> h1", "", "goff");
    tcluster->Draw("cluster.centery:cluster.centerx >> h2", "", "goff");
    tcluster->Draw("cluster.centery:cluster.centerx >> h4", "", "goff");
    // tevent->Print();
    // tcluster->Print();
}

void TAnalysisTaskClustering::RecordEvent(TEvent * event){
    // std::cout<<"TEventClustering::RecordEvent at ptr\t"<<RecordingEvent<<std::endl;
    // RecordingEvent->Print();
    // std::cout<<"DumpCluster"<<std::endl;
    // if(RecordingEvent==nullptr){
    //     event->Clear();
    //     std::cout<<"TAnalysisTaskClustering::RecordEvent - RecordingEvent is nullptr, ignored"<<std::endl;
    //     return;
    // }
    
    for(Int_t i=0; i<RecordingEvent->GetNCluster(); i++){
        // std::cout<<"Cluster N\'\t"<<i<<std::endl;
        RecordingEvent->GetCluster(i)->Dump(clusterdump);
        tcluster->Fill();
    }
    // std::cout<<"DumpEvent"<<std::endl;
    RecordingEvent->Dump(eventdump);

    tevent->Fill();
    event->Clear();
}

