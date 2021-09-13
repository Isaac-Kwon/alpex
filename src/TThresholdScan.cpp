#include "TThresholdScan.hpp"
#include "TString.h"

// #include "TROOT.h"

#include "TMath.h"
#include "TGraph.h"
#include "TF1.h"
#include "TTree.h"
#include "TH1F.h"
#include "TH1D.h"
#include "TFile.h"

#include "iostream"
#include "time.h"
#include <unistd.h>

TThresholdScan::TThresholdScan(TString infile, TString outfile) : fInfileName(infile), fOutfileName(outfile){
}

Double_t TThresholdScan::erf( Double_t *xx, Double_t *par){
  return (nInj / 2) *TMath::Erf((xx[0] - par[0]) / (sqrt(2) *par[1])) +(nInj / 2);
}


int TThresholdScan::AddressToColumn      (int ARegion, int ADoubleCol, int AAddress)
{
    int Column    = ARegion * 32 + ADoubleCol * 2;    // Double columns before ADoubleCol
    int LeftRight = ((((AAddress % 4) == 1) || ((AAddress % 4) == 2))? 1:0);       // Left or right column within the double column
    
    Column += LeftRight;
    
    return Column;
}


int TThresholdScan::AddressToRow         (int ARegion, int ADoubleCol, int AAddress)
{
    int Row = AAddress / 2;   
    return Row;
}


void TThresholdScan::AnalyzeFile(){
    //std::cout<<"Start Reading\t"<<fInfileName<<std::endl;
  FILE *fp = fopen (fInfileName, "r");

    //std::cout<<"Make file Reading\t"<<fOutfileName<<std::endl;
  fOutfile = new TFile(fOutfileName, "RECREATE");
  
  tree = new TTree("thresholds", "Threshold Analysis Data");
  tree->Branch("X", &tvX, "x/s");
  tree->Branch("Y", &tvY, "y/s");
  tree->Branch("Threshold", &tvThreshold, "thres/F");
  tree->Branch("Noise", &tvNoise, "noise/F");
  tree->Branch("Chi2", &tvChi2, "chi2/F");

  fOutfile->Add(tree);
  
  int col, address, ampl, hits;
  int lastcol = -1, lastaddress = -1;
  NPoints  = 0;
  NPixels  = 0;
  NNostart = 0;
  NChisq   = 0;

  ResetData();

  //std::cout<<"File Reading\t"<<std::endl;
  while ((fscanf (fp, "%d %d %d %d", &col, &address, &ampl, &hits) == 4)) {
    //if ((col < 255) || ((col == 255) && (address < 280))) continue;
    if (((lastcol != col) || (address != lastaddress)) && (NPoints!= 0)) {
      ProcessPixel(lastcol, lastaddress);
      NPixels ++;
      ResetData();
      NPoints  = 0;
    }
    lastcol = col;
    lastaddress = address;
    data [NPoints] = (float)hits;
    x    [NPoints] = (float)ampl * ELECTRONS_PER_DAC;
    NPoints ++;
  }
  fclose(fp);

  h2thres = new TH2D("h2thres", "Threshold Map", 1024, 0, 1024, 512, 0, 512);
  h2noise = new TH2D("h2noise", "Noise Map", 1024, 0, 1024, 512, 0, 512);
  h2chi2  = new TH2D("h2chi2", "Chi2 Map", 1024, 0, 1024, 512, 0, 512);

  h1thres = new TH1D("h1thres", "Threshold Distribution", 500, 0, 500);
  h1noise = new TH1D("h1noise", "Noise Distribution", 50, 0, 50);
  h1chi2  = new TH1D("h1chi2", "Chi2 Distribution", 20, 0, 5);

  tree->Draw("y:x >> h2thres", "thres", "goff");
  tree->Draw("y:x >> h2noise", "noise", "goff");
  tree->Draw("y:x >> h2chi2", "chi2", "goff");

  tree->Draw("thres >> h1thres", "",  "goff");
  tree->Draw("noise >> h1noise",  "", "goff");
  tree->Draw("chi2 >> h1chi2",  "", "goff");

//   fOutfile->Append(tree);

//   fOutfile->Append(h2thres);
//   fOutfile->Append(h2noise);
//   fOutfile->Append(h2chi2);
//   fOutfile->Append(h1thres);
//   fOutfile->Append(h1noise);
//   fOutfile->Append(h1chi2);

  fOutfile->Write();
  fOutfile->Close();

}

void TThresholdScan::ResetData() {
  for (int i=0; i <= 256; i++) {
    data[i] = 0;
  }
}

void TThresholdScan::ProcessPixel (int col, int row) {
  double thresh,noise,chi2;

  if (!GetThreshold(&thresh,&noise,&chi2)) return;

    tvY         = AddressToRow   (col / 16, col % 16, row);
    tvX         = AddressToColumn(col / 16, col % 16, row);
    tvThreshold = -1;
    tvNoise = -1;

  if(chi2<maxchi2){
    tvThreshold = thresh;
    tvNoise     = noise ;
  }
  tvChi2      = chi2;
  tree->Fill();
}

Float_t TThresholdScan::FindStart() {
  float Upper = -1;
  float Lower = -1;

  for (int i = 0; i < NPoints; i ++) {
    if (data[i] == nInj) {
      Upper = (float) x[i];
      break;
    }
  }
  if (Upper == -1) return -1;
  for (int i = NPoints-1; i > 0; i--) {
    if (data[i] == 0) {
      Lower = (float)x[i];
      break;
    }
  }
  if ((Lower == -1) || (Upper < Lower)) return -1;
  return (Upper + Lower)/2;
}

Bool_t TThresholdScan::GetThreshold(Double_t *thresh,Double_t *noise,Double_t *chi2) {
    //std::cout<<"GetThreshold"<<std::endl;
  TGraph *g      = new TGraph(NPoints, x, data);
  TF1    *fitfcn = new TF1("fitfcn", TThresholdScan::erf,0,1500,2,1,TF1::EAddToList::kNo);
  double Start  = FindStart();
  //std::cout<<"FindStart End"<<std::endl;

  if (Start < 0) {
    NNostart ++;
    return false;
  }

  fitfcn->SetParameter(0,Start);

  fitfcn->SetParameter(1,8);

  fitfcn->SetParName(0, "Threshold");
  fitfcn->SetParName(1, "Noise");

  //g->SetMarkerStyle(20);
  //g->Draw("AP");
  g->Fit("fitfcn","Q");

  *noise =fitfcn->GetParameter(1);
  *thresh=fitfcn->GetParameter(0);
  *chi2  =fitfcn->GetChisquare()/fitfcn->GetNDF();
  g->Delete();
  fitfcn->Delete();
  return true;
}


void TThresholdScan::AnalyzeFileMT(){

    // ROOT::EnableThreadSafety();
  fOutfile = new TFile(fOutfileName, "RECREATE");

  tree = new TTree("thresholds", "Threshold Analysis Data");
  tree->Branch("X", &tvX, "x/s");
  tree->Branch("Y", &tvY, "y/s");
  tree->Branch("Threshold", &tvThreshold, "thres/F");
  tree->Branch("Noise", &tvNoise, "noise/F");
  tree->Branch("Chi2", &tvChi2, "chi2/F");

  fInjector = new std::thread(&TThresholdScan::ThreadInjector, this);
  sleep(1);
  fAnalyzers.emplace_back( new std::thread(&TThresholdScan::ThreadAnalyzer, this, 0) );
  
  std::cout<<"Waiting Terminate of thread"<<std::endl;

  fInjector->join();
  fAnalyzers.at(0)->join();

  std::cout<<"All Thread terminated"<<std::endl;

  h2thres = new TH2D("h2thres", "Threshold Map", 1024, 0, 1024, 512, 0, 512);
  h2noise = new TH2D("h2noise", "Noise Map", 1024, 0, 1024, 512, 0, 512);
  h2chi2  = new TH2D("h2chi2", "Chi2 Map", 1024, 0, 1024, 512, 0, 512);

  h1thres = new TH1D("h1thres", "Threshold Distribution", 500, 0, 500);
  h1noise = new TH1D("h1noise", "Noise Distribution", 50, 0, 50);
  h1chi2  = new TH1D("h1chi2", "Chi2 Distribution", 20, 0, 5);

  tree->Draw("y:x >> h2thres", "thres");
  tree->Draw("y:x >> h2noise", "noise");
  tree->Draw("y:x >> h2chi2", "chi2");

  tree->Draw("thres >> h1thres");
  tree->Draw("noise >> h1noise");
  tree->Draw("chi2 >> h1chi2");
  
  fOutfile->Write();
  fOutfile->Close();



}

void TThresholdScan::ThreadInjector(){

    std::cout<<"Injector Thread Start"<<std::endl;

  FILE *fp = fopen (fInfileName, "r");

  int col, address, ampl, hits;
  int lastcol = -1, lastaddress = -1;

  fInjectorRun = kTRUE;

  MeasureEntry* entry;
  entry = new MeasureEntry();
  while ((fscanf (fp, "%d %d %d %d", &col, &address, &ampl, &hits) == 4)) {
    //if ((col < 255) || ((col == 255) && (address < 280))) continue;
    if (((lastcol != col) || (address != lastaddress))) {
        entry->col = lastcol;
        entry->address = lastaddress;

        while( ! fMutexInjector.try_lock() ){;}
        fEntry.emplace_back(entry);
        fMutexInjector.unlock();

        entry = new MeasureEntry();
    }
    lastcol = col;
    lastaddress = address;
    
    entry->value.emplace_back((float)hits);
    entry->x.emplace_back((float)ampl * ELECTRONS_PER_DAC);
    // if(address==0) std::cout<<"col\t"<<col<<"\tInjecting"<<std::endl;
  }


  fInjectorRun = kFALSE;
  fclose(fp);
}

void TThresholdScan::ThreadAnalyzer(const int n){
    std::cout<<"Injector Analyzer Thread Start"<<std::endl;

    if(!fInjectorRun) return;

    while(fInjectorRun || fEntry.size()!=0){
        MeasureEntry * entry;

        while(!fMutexInjector.try_lock()){;} // Wait InjectorEntry available
        while(fEntry.size()==0){
            if(!fInjectorRun){
                break;
            }
            sleep(1);
            continue;
        }
        

        entry = fEntry.front();
        fEntry.pop_front();
        fMutexInjector.unlock();

        UShort_t Y         = AddressToRow   (entry->col / 16, entry->col % 16, entry->address);
        UShort_t X         = AddressToColumn(entry->col / 16, entry->col % 16, entry->address);
        Double_t Threshold;
        Double_t Noise;
        Double_t Chi2;

        GetThresholdMT(entry, &Threshold,&Noise,&Chi2, n);
        while(!fMutexTree.try_lock()){;} // Wait Tree available

        tvX = X;
        tvY = Y;
        tvThreshold = Threshold;
        tvNoise     = Noise;
        tvChi2      = Chi2;
        tree->Fill();

        fMutexTree.unlock();

        // if( entry->address==0) std::cout<<"col\t"<<entry->col<<"\tanalyzed"<<std::endl;

    }

}


Bool_t TThresholdScan::GetThresholdMT(MeasureEntry* entry, Double_t *thresh, Double_t *noise, Double_t *chi2, const int n) {
  TGraph *g      = new TGraph();
  TF1    *fitfcn = new TF1(TString::Format("fitfcn_%d", n), TThresholdScan::erf,0,1500,2);
  double Start  = FindStartMT(entry);

  if (Start < 0) {
    NNostart ++;
    return false;
  }

  fitfcn->SetParameter(0,Start);

  fitfcn->SetParameter(1,8);

  fitfcn->SetParName(0, "Threshold");
  fitfcn->SetParName(1, "Noise");

  int npoints = entry->value.size();
  for(int i=0; i<npoints; i++){
      g->SetPoint(i, entry->x[i], entry->value[i]);
  }

  while(! fMutexFitter.try_lock()){;}
  g->Fit(fitfcn,"Q");
  fMutexFitter.unlock();

  *noise =fitfcn->GetParameter(1);
  *thresh=fitfcn->GetParameter(0);
  *chi2  =fitfcn->GetChisquare()/fitfcn->GetNDF();

  g->Delete();
  fitfcn->Delete();

  return true;
}

Float_t TThresholdScan::FindStartMT(MeasureEntry* entry) {
  float Upper = -1;
  float Lower = -1;

  int npoints = entry->value.size();

  for (int i = 0; i < npoints; i ++) {
    if (entry->value[i] == nInj) {
      Upper = (float) entry->x[i];
      break;
    }
  }
  if (Upper == -1) return -1;
  for (int i = npoints-1; i > 0; i--) {
    if (entry->value[i] == 0) {
      Lower = (float)entry->x[i];
      break;
    }
  }
  if ((Lower == -1) || (Upper < Lower)) return -1;
  return (Upper + Lower)/2;
}