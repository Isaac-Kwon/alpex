
#ifndef TEVENT_H
#define TEVENT_H 1

#include "TROOT.h"
#include "TPixel.hpp"
#include "TCluster.hpp"
#include "deque"
#include "iostream"

class TEvent{
    private:
    std::deque<TPixel> pixes; //unclustered pixels
    std::deque<TCluster> clusters; //clustered cluster
    Bool_t autocluster = kFALSE;
    Bool_t maynotclustered = kTRUE;
    Double_t mdistance = 1.0;
    protected:
    public:
    TEvent();
    TEvent(const TEvent & event);
    void AddPixel(TPixel pix);
    void AddCluster(TCluster cluster);
    void Clear();
    void Clustering(Double_t distance = 0, Bool_t setautocluster = kFALSE);
    Int_t  GetNPixel();
    Int_t  GetNCluster();
    Int_t  GetNPixelUnclustered();
    TPixel  GetPixel(Int_t i){return pixes.at(i);}
    TCluster  GetCluster(Int_t i){return clusters.at(i);}
    Bool_t IsInside(TPixel pix);
    Bool_t IsInside(TCluster cluster);
    TString Print(Bool_t quite=kFALSE);
};

TEvent::TEvent(){

}

TEvent::TEvent(const TEvent & event){
    for(TPixel pix : event.pixes){
        pixes.emplace_back(pix);
    }
    for(TCluster cluster : event.clusters){
        clusters.emplace_back(cluster);
    }
    autocluster = event.autocluster;
    maynotclustered = event.maynotclustered;
    mdistance = event.mdistance;
}

void TEvent::AddPixel(TPixel pix){
    pixes.emplace_back(pix);
    maynotclustered = kTRUE;
    if(autocluster) Clustering(kTRUE);
}

void TEvent::AddCluster(TCluster cluster){
    clusters.emplace_back(cluster);
    maynotclustered = kTRUE;
    if(autocluster) Clustering(kTRUE);
}

void TEvent::Clear(){
    pixes.clear();
    clusters.clear(); 
}

void TEvent::Clustering(Double_t distance, Bool_t setautocluster = kFALSE){
    autocluster = setautocluster;

    //if no cluster, first seed pixel -> cluster
    if(clusters.size()==0){
        clusters.emplace_back(pixes.at(0));
        pixes.pop_front();
    }else if(clusters.size()>0){
        for(std::deque<TCluster>::iterator it = clusters.begin()+1; it != clusters.end(); it++){
            for(Int_t i=0; i<(*it).GetNPixel(); i++){
                pixes.emplace_back((*it).GetPixel(i));
            }
            clusters.erase(it);
        }
    }

    Bool_t clustered = kTRUE;
    Int_t ncandpix = -1;
 
    while(clustered){
    clustered = kFALSE;
        for(std::deque<TPixel>::iterator it1  = pixes.begin(); it1 != pixes.end(); it1 ++){
            Bool_t clusteredp = kFALSE;
            for(std::deque<TCluster>::iterator it2 = clusters.begin(); it2!= clusters.end(); it2++){
                if((*it2).Append(*it1, mdistance)){
                    clusteredp = kTRUE;
                    clustered  = kTRUE;
                    break;
                }
            }
            if(clusteredp){
                pixes.erase(it1);
                break;
            }
        }

        if(pixes.size()>0 && !clustered){
            clusters.emplace_back(pixes.front());
            pixes.pop_front();
            clustered = kTRUE;
        }else if(pixes.size()==0 && !clustered){
            break;
        }
    }
    
    // std::cout<<"3"<<std::endl;
    //After pixel -> cluster clustering, there's no element in <pixes>
    // if(pixes.size()!=0){
    //     std::cout<<"DEBUG :: (TEvent::Clustering) :: pixes is not empty now"<<std::endl;
    // }

    // std::cout<<"4"<<std::endl;
    //Clustering cluster -> cluster
    // for(std::deque<TCluster>::iterator it1  = clusters.begin();
    //                                    it1 != clusters.end();
    //                                    it1 ++){
    //     Bool_t clustered = kFALSE;
    //     for(std::deque<TCluster>::iterator it2  = clusters.begin();
    //                                        it2 != clusters.end();
    //                                        it2 ++){
    //         if((*it2) == (*it1)) continue;
    //         if((*it2).Append(*it1, mdistance)){
    //             clustered = kTRUE;
    //             break;
    //         }
    //     }
    //     clusters.erase(it1);
    // }
    maynotclustered=kFALSE;
}

Int_t  TEvent::GetNPixel(){
    Int_t n=0;
    for(TCluster cluster : clusters){
        n+=cluster.GetNPixel();
    }
    return n+pixes.size();
}

Int_t  TEvent::GetNCluster(){
    return clusters.size();
}

Int_t  TEvent::GetNPixelUnclustered(){
    return pixes.size();
}


Bool_t TEvent::IsInside(TPixel pix){
    for(TPixel pix_ : pixes){
        if(pix == pix_) return kTRUE;
    }

    for(TCluster cluster_ : clusters){
        for(Int_t i=0; i<cluster_.GetNPixel(); i++){
            if(cluster_.GetPixel(i) == pix) return kTRUE;
        }
    }
    return kFALSE;
}

Bool_t TEvent::IsInside(TCluster cluster){
    for(Int_t i=0; i < cluster.GetNPixel(); i++){
        if(IsInside(cluster.GetPixel(i))) return kTRUE;
    }
    return kFALSE;
}

TString TEvent::Print(Bool_t quite){
    TString pixans = TString("[");
    Bool_t first1 = kTRUE;
    for(TPixel pix : pixes){
        if(!first1) pixans += TString(", ");
        first1=kFALSE;
        pixans += pix.Print(kTRUE);
    }
    pixans += TString("]");


    TString clusans = TString("[");
    Bool_t first2 = kTRUE;
    for(TCluster cluster : clusters){
        if(!first2) clusans += TString(", ");
        first2=kFALSE;
        clusans += cluster.Print(kTRUE);
    }
    clusans += TString("]");

    TString ans = TString::Format("Pix(%lu) %s, Clusters(%lu) %s",pixes.size() ,pixans.Data(), clusters.size(), clusans.Data());

    if(!quite){
        std::cout<<"TEvent "<< ans <<" "<<std::endl;
    }
    
    return ans;
}




#endif