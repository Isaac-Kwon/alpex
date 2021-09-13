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


TAnalysisTaskClustering::TAnalysisTaskClustering(const char * name, const char * title, TFile* inputfile, TFile* outputfile)
                         :TAnalysisTask(name,title)
{
    finputfile = inputfile;
    foutputfile = outputfile;
    Init();
}

TAnalysisTaskClustering::TAnalysisTaskClustering(TFile* inputfile, TFile* outputfile)
                         :TAnalysisTaskClustering("task","analysis task for clustering", inputfile, outputfile)
{;}

void TAnalysisTaskClustering::Clear(){
    tevent->Delete();
    tcluster->Delete();
    finput->Clear();
    foutput->Clear();
    h1->Delete();
    h2->Delete();
    h3->Delete();
    h4->Delete();
    if (finputfile && finputSelf){
        finputfile->Close("R");
        delete finputfile;
        std::cout<<"TAnalysisTaskClustering::Clear - Close Infile"<<std::endl;
    }
    if (foutputfile && foutputSelf){
        foutputfile->Close("R");
        delete foutputfile;
        std::cout<<"TAnalysisTaskClustering::Clear - Close Outfile"<<std::endl;
    }
}

void TAnalysisTaskClustering::Init(){

    finput = new TObjArray();//("indir","TDirectory for Clustering Analysis (InputFile)");

    //======================================================================//
    // Input Treeing
    if(finputfile==nullptr){
        finputfile = new TFile(finputfname, "READ");
        finputSelf = kTRUE;
    }
    thit = (TTree*) finputfile->Get("hit");
    thit->SetBranchAddress("ChipID",    &read.chipid);
    thit->SetBranchAddress("TimeStamp", &read.t);
    thit->SetBranchAddress("X",         &read.x);
    thit->SetBranchAddress("Y",         &read.y);
    
    finput->Add(thit);


    if(foutputfile==nullptr){
        foutputfile = new TFile(foutputfname, "RECREATE");
        foutputSelf = kTRUE;
    }
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
    tcluster->Branch("noise",  clusterdump.noise, "noise[npixel]/s");
    tcluster->Branch("cluster", &clusterdump, "centerx/s:centery/s:sizex/s:sizey/s:coreless/O");

    tcluster->SetAlias("noiselevel", "Sum$(noise)/npixel");


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

    h5               = new TH2D("h5", "Noise Map", 16, 0, 1024, 8, 0, 512);

    foutput = new TObjArray(); //new TDirectory("outdir","TDirectory for Clustering Analysis (OutputFile)");
    foutput->Add(tcluster);
    foutput->Add(tevent);
    foutput->Add(h1);
    foutput->Add(h2);
    foutput->Add(h3);
    foutput->Add(h4);
    foutput->Add(h5);
}

void TAnalysisTaskClustering::preAnalysis(){
    fNoiseLevel = 10;
    // Int_t fNoiseSize  = 1;

    fNoiseMap = new ChipMap(1024, 512, 0);

    h_pre0 = new TH2D("h_pre0", "pre_analysis hitmap", 1024, 0, 1024, 512, 0, 512);
    thit->Draw("y:x >> h_pre0");
    
    
    for(Short_t i=0; i<1024; i++){
        for(Short_t j=0; j<512; j++){
            fNoiseMap->Set(i,j,h_pre0->GetBinContent(i+1,j+1));
            // if(h_pre0->GetBinContent(i+1,j+1) != 0){
                // std::cout<<i<<"\t"<<j<<"\t"<< h_pre0->GetBinContent(i+1,j+1) <<" and "<< fNoiseMap->At(i,j)<<std::endl;
            // }
        }
    }

    fNoiseMap->PrintStatus();

    // for(Short_t i=0; i<1024; i++){
    //     for(Short_t j=0; j<512; j++){
    //         Int_t val = fNoiseMap->At(i,j);
    //         if(val>0){
    //             std::cout<<"NoiseMap ["<<i<<","<<j<<"]=\t"<<val<<std::endl;
    //             h5->SetBinContent(i+1,j+1,Double_t(val));
    //         }
    //     }
    // }

}

void TAnalysisTaskClustering::Exec(Int_t verbose, Int_t ratio){

    if(fNoiseMap == nullptr) preAnalysis();

    thit->GetEntry(0);
    UInt_t t_prev = read.t;
    TEvent * event = RecordingEvent;
    Int_t nentries = thit->GetEntries() / ratio;
    std::cout<<"NEntries for Analysis: " << nentries <<"\t/\t" << thit->GetEntries()<<std::endl;
    TTimeStamp starttime = TTimeStamp();
    Int_t passedpixels = 0;

    std::cout << "Started at " << starttime.AsString() << std::endl;

    for(Int_t i=0; i<nentries; i++){
        thit->GetEntry(i);
        if(t_prev != read.t || i==nentries-1){
            if(i==nentries-1){
                event->Append(TPixel(read.x, read.y, read.t));
            }
            if(event->GetNPixel()>400){
                std::cout<<"Event is too big (npixel>400) npix="<<event->GetNPixel()+passedpixels<<"("<<passedpixels<<")"<<", Drop Event."<<std::endl;
                event->Clear();
            }else{
                event->Clustering(fDistance);
                if(event->GetNCluster()>400){
                    std::cout<<"Event is too big (ncluster>400) ncluster="<<event->GetNCluster()<<", Drop Event."<<std::endl;
                    event->Clear();
                }else{
                    RecordEvent(event);
                }
            }
            passedpixels=0;
        }
        t_prev = read.t;
        if( event->GetNPixel()>500 ){
            passedpixels+=1;
            continue;
        }
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

    // Bool_t repeat = kTRUE;
    // while(repeat){
    //     repeat = kFALSE;
    //     for(Int_t i=0; i<RecordingEvent->GetNCluster(); i++){
    //         TCluster * cluster = RecordingEvent->GetCluster(i);
    //         if(cluster->GetNPixel() != 1) continue;
    //         TPixel pix = cluster->GetPixel(0);
    //         // std::cout<<pix.GetX() <<"\t"<< pix.GetY() << "\t"<<fNoiseMap->At( pix.GetX(), pix.GetY())<<std::endl;
    //         if(fNoiseMap->At( pix.GetX(), pix.GetY()) < fNoiseLevel ) continue;
    //         RecordingEvent->PopCluster(i);
    //         // std::cout<<"poped\t"<<pix.GetX()<<"\t"<<pix.GetY()<<std::endl;
    //         // delete cluster;
    //         repeat = kTRUE;
    //     }
    // }

    Bool_t startdebug = kFALSE;
    // if(event->GetCluster(0)->GetPixel(0).GetT() > 7820) startdebug = kTRUE;

    // if(startdebug) std::cout<<"DEBUG S" << event->GetCluster(0)->GetPixel(0).GetT() <<std::endl;
    // if(startdebug) std::cout<<"DEBUG 0"<<std::endl;
    for(Int_t i=0; i<RecordingEvent->GetNCluster(); i++){
        // std::cout<<"Cluster N\'\t"<<i<<std::endl;
        TCluster * cluster = RecordingEvent->GetCluster(i);
        // if(startdebug) std::cout<<"CLU "<< cluster->GetNPixel() <<std::endl;
        cluster->Dump(clusterdump, fNoiseMap);
        // if(startdebug) std::cout<<"CLU FILL"<<std::endl;
        tcluster->Fill();
        // if(startdebug) std::cout<<"CLU FILLED"<<std::endl;
    }
    // std::cout<<"DumpEvent"<<std::endl;
    // if(tevent->get)
    // if(startdebug) std::cout<<"DEBUG 1"<<std::endl;
    RecordingEvent->Dump(eventdump);
    // if(startdebug) std::cout<<"DEBUG 2"<<std::endl;
    tevent->Fill();
    // if(startdebug) std::cout<<"DEBUG 3"<<std::endl;
    event->Clear();
    // if(startdebug) std::cout<<"DEBUG 4-END"<<std::endl;
}

void TAnalysisTaskClustering::Save(const char *fname, Bool_t closing){
    
    foutput->Print();

    if(foutputfile==nullptr){
        if(fname == 0){
            foutputfile = new TFile("taskresult.root", "RECREATE");
        }else{
            foutputfile = new TFile(fname, "RECREATE");
        }
    }

    foutputfile->cd();
    foutputfile->Add(foutput->Clone(),kTRUE);
    foutputfile->Write();
};