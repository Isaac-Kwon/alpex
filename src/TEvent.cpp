#include "TEvent.hpp"
#include "TClonesArray.h"
#include "TPixel.hpp"
#include "TCluster.hpp"

// #if !defined(__CLING__)
// ClassImp(TEvent)
// #endif

TEvent::TEvent(){
    std::cout<<"Construct Event"<<std::endl;
    // if(fPixes==nullptr)    fPixes     = new TClonesArray("TPixel", 1000); //unclustered pixels
    // if(fClusters==nullptr) fClusters  = new TClonesArray("TCluster", 1000); //clustered cluster
    // std::cout<<"Pixel\t"<<fPixes<<"\tCluster\t"<<fClusters<<std::endl;
    fNPixes         = 0;
    fNClusters      = 0;
}

TEvent::TEvent(const TEvent & event){
    std::cerr<<"TEvent::TEvent - DO NOT USE COPY CONSTRUCTOR (not developed"<<std::endl;
    // fPixes          = new TClonesArray(* event.fPixes   ); //unclustered pixels
    // fClusters       = new TClonesArray(* event.fClusters); //clustered cluster
    // fNPixes         = event.fNPixes;
    // fNClusters      = event.fNClusters;
    // fAutocluster     = event.fAutocluster;
    // fMaynotclustered = event.fMaynotclustered;
    // fMdistance       = event.fMdistance;
}

TEvent::~TEvent(){
    
}

void TEvent::Append(TPixel pix){
    fPixes.emplace_back(pix);
    fNPixes++;
    fMaynotclustered = kTRUE;
    if(fAutocluster) Clustering(kTRUE);
}

void TEvent::Append(TCluster* cluster){
    fClusters.emplace_back(cluster);
    fNClusters++;
    fMaynotclustered = kTRUE;
    if(fAutocluster) Clustering(kTRUE);
}

void TEvent::Clear(){
    // fPixes->Clear();
    // fClusters->Clear();
    fPixes.clear();
    fClusters.clear();

    fNPixes    = 0;
    fNClusters = 0;
}

void TEvent::Clustering(Double_t distance, Bool_t setautocluster){
    Compress();
    fAutocluster = setautocluster;

    if(fNClusters==0){
        Append(new TCluster(PopFrontPixel())); //Initilize the event
    }else if(fNClusters>0){ // dissociate all of clusters
        for(Int_t i=0; fNClusters>1; i++){
            TCluster * cluster = GetCluster(1);
            for(Int_t j=0; j<cluster->GetNPixel(); j++){
                Append(cluster->GetPixel(j));
            }
            // fClusters->Remove(cluster); //popfront_cluster
            fClusters.pop_front(); //popfront_cluster
            cluster->Clear();
            delete cluster;
            fNClusters--;
        }
    }
    // Now, the one cluster + n pixels.

    Bool_t clustered = kTRUE;
    // Int_t ncandpix = -1;
    while(clustered){
        clustered = kFALSE;
        for(Int_t i=0; i<fNPixes; i++){
            Bool_t clusteredp = kFALSE;
            TPixel pix = GetPixel(i);
            Int_t j=0;
            for(j=0; j<fNClusters; j++){
                if(GetCluster(j)->Append(pix,distance)){
                    clusteredp = kTRUE;
                    clustered  = kTRUE;
                    break;
                }
            }
            if(clusteredp){
                PopPixel(i);
                break;
            }
        }

        if(fNPixes>0 && !clustered){
            Append(new TCluster(PopFrontPixel()));
            clustered = kTRUE;
        }else if(fNPixes && !clustered){
            break;
        }
    }
    fMaynotclustered=kFALSE;
}

TPixel    TEvent::GetPixel(Int_t pixn){
    // fPixes->Compress();
    if(pixn>=fNPixes){
    std::cout<<"TEvent::GetPixel - ERROR - Call index over the number of pixels in event " << pixn << "/" << Int_t(fNPixes)-1 << " | " << "Entries " << fPixes.size() <<std::endl;
        return TPixel();
    }
    return fPixes[pixn];
}

TCluster *TEvent::GetCluster(Int_t clusn){
    // fClusters->Compress();

    if(clusn>=fNClusters){
        // std::cout<<"TEvent::GetPixel - ERROR - Call index over the number of pixels in event " << clusn << "/" << Int_t(fNClusters)-1 << " | " << "Entries " << fClusters->GetEntries() <<std::endl;
        return nullptr;
    }
    return fClusters[clusn];
}

UInt_t TEvent::GetNPixel(){
    Int_t n=0;
    for(Int_t i=0; i<fNClusters; i++){
        n += GetCluster(i)->GetNPixel();
    }
    return n+GetNPixelUnclustered();
}

TPixel TEvent::PopFrontPixel(){
    // std::cout<<"POPFRONTPIXEL"<<std::endl;
    return PopPixel(0);
}

TPixel TEvent::PopPixel(Int_t pixn){
    TPixel ans = GetPixel(pixn);
    // delete GetPixelPtr(pixn);
    // fPixes->RemoveAt(pixn);
    // fPixes->Compress();
    if(pixn>=0){
        if(fNPixes < pixn) return TPixel();
        fPixes.erase(fPixes.begin()+pixn);
    }else{
        if(fNPixes <= -1*pixn) return TPixel();
        fPixes.erase(fPixes.end()+pixn);
    }
    fNPixes--;
    return ans;
}

TPixel TEvent::PopBackPixel(){
    // std::cout<<"POPFRONTPIXEL"<<std::endl;
    return PopPixel(-1);
}

TCluster* TEvent::PopCluster(Int_t clusn){
    TCluster* ans = GetCluster(clusn);
    // delete GetPixelPtr(pixn);
    // fPixes->RemoveAt(pixn);
    // fPixes->Compress();
    if(clusn>=0){
        if(fNClusters < clusn) return new TCluster();
        fClusters.erase(fClusters.begin()+clusn);
    }else{
        if(fNClusters <= -1*clusn) return new TCluster();
        fClusters.erase(fClusters.end()+clusn);
    }
    fNClusters--;
    return ans;
}

Bool_t TEvent::IsInside(TPixel pix){
    for(Int_t i=0; i<fNPixes; i++){
        if(pix == GetPixel(i)) return kTRUE;
    }

    for(Int_t i=0; i<fNClusters; i++){
        TCluster * cluster_ = GetCluster(i);
        Int_t entries = cluster_->GetNPixel();
        for(Int_t i=0; i<entries; i++){
            if(cluster_->GetPixel(i) == pix) return kTRUE;
        }
    }
    return kFALSE;
}

Bool_t TEvent::IsInside(TCluster * cluster){
    for(Int_t i=0; i < cluster->GetNPixel(); i++){
        if(IsInside(cluster->GetPixel(i))) return kTRUE;
    }
    return kFALSE;
}

TString TEvent::Print(Bool_t quite){
    TString pixans = TString("[");
    Bool_t first1 = kTRUE;
    // for(TPixel pix : pixes){
    for(Int_t i=0; i<fNPixes; i++){
        TPixel pix = GetPixel(i);
        if(!first1) pixans += TString(", ");
        first1=kFALSE;
        pixans += pix.Print(kTRUE);
    }
    pixans += TString("]");


    TString clusans = TString("[");
    Bool_t first2 = kTRUE;
    // for(TCluster cluster : clusters){
    for(Int_t i=0; i<fNClusters; i++){
        TCluster *cluster = GetCluster(i);
        if(!first2) clusans += TString(", ");
        first2=kFALSE;
        clusans += cluster->Print(kTRUE);
    }
    clusans += TString("]");

    TString ans = TString::Format("Pix(%u) %s, Clusters(%u) %s",fNPixes ,pixans.Data(), fNClusters, clusans.Data());

    if(!quite){
        std::cout<<"TEvent "<< ans <<" "<<std::endl;
    }
    
    return ans;
}

void TEvent::Dump(TEventDump& dump){
    //     static struct TEventDump{
    //     Int_t    t = -1;
    //     UShort_t x[PIXELMAX];
    //     UShort_t y[PIXELMAX];
    //     UShort_t centerx[CLUSTERMAX];
    //     UShort_t centery[CLUSTERMAX];
    //     UShort_t npixel=0;
    //     UShort_t ncluster=0;
    // };
    // std::cout<<"TEvent::Dump - Start"<<std::endl;

    if(GetNPixelUnclustered()!=0){
        // std::cout<<"TEvent::Dump - with condition npixel!=0"<<std::endl;
        dump.t = GetPixel(0).GetT();
    }else if(GetNCluster()!=0){
        // std::cout<<"TEvent::Dump - with condition ncluster!=0"<<std::endl;
        dump.t = GetCluster(0)->GetPixel(0).GetT();
    }

    // std::cout<<"TEvent::Dump - t="<<dump.t<<std::endl;

    Int_t pixn=0;
    for(Int_t i=0; i<GetNCluster(); i++){
        TCluster * cluster = GetCluster(i);
        // std::cout<<"Will be dump \t" << cluster->GetNPixel() <<"\t pixels"<< std::endl;
        for(Int_t j=0; j<cluster->GetNPixel(); j++){
            TPixel pix = cluster->GetPixel(j);
            // std::cout<<"Dump Pixel in cluster " <<i<<"\t"<<j<<std::endl;
            dump.x[pixn] = pix.GetX();
            dump.y[pixn] = pix.GetY();
            // std::cout<<"TEvent::Dump - DUMP PIXEL No."<<pixn<<"\t"<<"["<< dump.x[pixn] << "," << dump.y[pixn] <<"]"<<std::endl;
            pixn++;
        }
        dump.centerx[i] = cluster->GetCenterX();
        dump.centery[i] = cluster->GetCenterY();
        // std::cout<<"TEvent::Dump - DUMP ClusterCentre No."<<i<<"\t"<<"["<< dump.centerx[i] << "," << dump.centery[i] <<"]"<<std::endl;
    }
    dump.npixel = pixn;
    dump.ncluster = GetNCluster();

    // std::cout<<"TCluster::Dump - npixel="<< (UInt_t) dump.npixel <<std::endl;
    // std::cout<<"TCluster::Dump - ncluster="<< (UInt_t) dump.ncluster <<std::endl;
}

