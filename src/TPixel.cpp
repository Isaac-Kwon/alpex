#include "TMath.h"
#include "TObject.h"

#include "TPixel.hpp"
#include "iostream"

// #if !defined(__CLING__)
// ClassImp(TPixel)
// #endif

TPixel::TPixel(){
    x = -1;
    y = -1;
    t = -1;
}

TPixel::TPixel(UInt_t x_, UInt_t y_, UInt_t t_){
    x = x_;
    y = y_;
    t = t_;
}

TPixel::TPixel(const TPixel & pix){
    // std::cout<<"COPY " << pix.x <<"\t"<<pix.y <<"\t" <<pix.t<<std::endl;
    x = pix.x;
    y = pix.y;
    t = pix.t;
    // std::cout<<"TPixel::TPixel - Copy Contruction [" << pix.x <<","<< pix.y<<","<<pix.t<< "] to [" <<  x <<","<<  y <<","<< t << "]" <<std::endl;
}

Bool_t TPixel::IsNear(TPixel pix, Double_t distance, Bool_t manhattan){
    //If the pixels are attached in a side (not edge)
    if(GetMDistance(pix)==1){        return kTRUE;
    }

    if(manhattan){
        if(Double_t(GetMDistance(pix)) < distance) return kTRUE;
    }else{
        if(Double_t(GetEDistance(pix)) < distance) return kTRUE;
    }

    return kFALSE;
}

Bool_t TPixel::operator==(TPixel pix){
    if(pix.x == x && pix.y == y){
        if(t==-1 || pix.t==-1){
            return kTRUE;
        }else{
            if(t==pix.t){
                return kTRUE;
            }
        }
    }
    return kFALSE;
}

UInt_t TPixel::GetMDistance(const TPixel &pix){ //Manhattan Distance
    return TMath::Abs(Int_t(pix.x)-Int_t(x)) + TMath::Abs(Int_t(pix.y)-Int_t(y));

}

Double_t TPixel::GetEDistance(const TPixel &pix){ //Eucludian Distance
    return TMath::Sqrt(TMath::Power(Double_t(pix.x)-Double_t(x),2)
                     + TMath::Power(Double_t(pix.y)-Double_t(y),2)
                     );
}

TString TPixel::Print(Bool_t quite){
    TString ans = TString::Format("[%u, %u, %u]", t,x,y);
    if (!quite){
        std::cout<<"TPixel "<<ans << " "<<std::endl;
    }
    return ans;
}