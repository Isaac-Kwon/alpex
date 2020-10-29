#ifndef TCLUSTER_H
#define TCLUSTER_H 1

#include "TROOT.h"
#include "deque"
#include "TPixel.hpp"

class TCluster{
    private:
    std::deque<TPixel> pixes;
    Double_t ndistance = 0;
    Bool_t   manhattan = kFALSE; // kFALSE : Euclidean Distance
                                 // kTRUE  : Manhattan Distance 
    protected:
    public:
    TCluster();
    TCluster(const TPixel pix);
    TCluster(const TCluster &cluster);
    void   SetMode(Bool_t IsManhattan){manhattan = IsManhattan;}
    Int_t  GetNPixel();
    TPixel GetPixel(Int_t pixn);
    Bool_t Append(TPixel   pix, UInt_t distance = 0);
    Bool_t Append(TCluster cluster, UInt_t distance = 0);
    Bool_t IsNear(TPixel   pix, UInt_t distance = 0);
    Bool_t IsNear(TCluster cluster, UInt_t distance = 0);
    Bool_t IsOverlapped(TPixel   pix);
    Bool_t IsOverlapped(TCluster cluster);
    Bool_t IsManhattan(){return manhattan;}
    Bool_t IsEuclidean(){return !manhattan;}
    Bool_t IsCoreless();
    Bool_t operator==(TCluster cluster);
    TString Print(Bool_t quite=kFALSE);
};

TCluster::TCluster(){
}

TCluster::TCluster(const TPixel pix){
    pixes.emplace_back(pix);
}

TCluster::TCluster(const TCluster &cluster){
    for(TPixel pix : cluster.pixes){
        pixes.emplace_back(pix);
    }
}

Int_t  TCluster::GetNPixel(){
    return pixes.size();
}

TPixel TCluster::GetPixel(Int_t i){
    return pixes.at(i);
}

Bool_t TCluster::Append(TPixel pix, UInt_t distance){
    if(!IsNear(pix, distance) && !IsOverlapped(pix)) return kFALSE;
    // std::cout<<"TCluster::Append - Appending pixel in pixes"<<std::endl;
    pixes.emplace_back(pix);
    return kTRUE;
}

Bool_t TCluster::Append(TCluster cluster, UInt_t distance){
    if(!IsNear(cluster, distance) && !IsOverlapped(cluster) ) return kFALSE;
    for(TPixel pix: cluster.pixes){
        if(!IsOverlapped(pix)) pixes.emplace_back(pix);
    }
    return kTRUE;
}

Bool_t TCluster::IsNear(TPixel pix, UInt_t distance){
    for(TPixel pix1: pixes){
        if(pix.IsNear(pix1, distance)){
            // std::cout<<"TCluster::IsNear - return true"<<std::endl;
            return kTRUE;
        }
    }
    return kFALSE;
}

Bool_t TCluster::IsNear(TCluster cluster, UInt_t distance){
    for(TPixel pix1: cluster.pixes){
        if(IsNear(pix1, distance)){
            return kTRUE;
        }
    }
    return kFALSE;
}

Bool_t TCluster::IsOverlapped(TPixel   pix){
    for(TPixel pix1: pixes){
        if(pix1==pix) return kTRUE;
    }
    return kFALSE;
}

Bool_t TCluster::IsOverlapped(TCluster pix){
    for(TPixel pix1: pix.pixes){
        if(IsOverlapped(pix1)) return kTRUE;
    }
    return kFALSE;
}

Bool_t TCluster::IsCoreless(){
  // will be updated
  return kFALSE;
}

Bool_t TCluster::operator==(TCluster cluster){
    if(cluster.GetNPixel() != GetNPixel()) return kFALSE;
    for(TPixel pix : cluster.pixes){
        if(! IsOverlapped(pix) ) return kFALSE;
    }
    return kTRUE;
}

TString TCluster::Print(Bool_t quite){
    TString ans = TString("[");
    
    Bool_t first = kTRUE;
    for(TPixel pix : pixes){
        if(!first) ans += TString(", ");
        first=kFALSE;
        ans += pix.Print(kTRUE);
    }
    ans += TString("]");

    if (!quite){
        std::cout<<"TCluster "<<ans<<" "<<std::endl;
    }
    return ans;

}

#endif