#ifndef __TPIXEL__
#define __TPIXEL__

#include "TROOT.h"
#include "TObject.h"

#include "deque"

class TCluster;
class TString;
class TPixel;

class TPixel{
    private:
    UInt_t x=-1;
    UInt_t y=-1;
    UInt_t t=-1;
    protected:
    public:
    TPixel();
    TPixel(UInt_t x_, UInt_t y_, UInt_t t_=-1);
    TPixel(const TPixel & pix);
    Bool_t IsNear(TPixel pix, Double_t distance=0, Bool_t manhattan=kFALSE);
    Bool_t IsNear(TCluster cluster, Double_t distance=0, Bool_t manhattan=kFALSE);
    Bool_t operator==(TPixel pix);
    UInt_t GetMDistance(const TPixel &pix);
    Double_t GetEDistance(const TPixel &pix);
    UInt_t GetX(){return x;}
    UInt_t GetY(){return y;}
    UInt_t GetT(){return t;}
    TString Print(Bool_t quite=kFALSE);
};

// TPixel::TPixel(){
//     x = -1;
//     y = -1;
//     t = -1;
// }

// TPixel::TPixel(UInt_t x_, UInt_t y_, UInt_t t_){
//     x = x_;
//     y = y_;
//     t = t_;
// }

// TPixel::TPixel(const TPixel & pix){
//     // std::cout<<"COPY " << pix.x <<"\t"<<pix.y <<"\t" <<pix.t<<std::endl;
//     x = pix.x;
//     y = pix.y;
//     t = pix.t;
//     // std::cout<<"TPixel::TPixel - Copy Contruction [" << pix.x <<","<< pix.y<<","<<pix.t<< "] to [" <<  x <<","<<  y <<","<< t << "]" <<std::endl;
// }

// Bool_t TPixel::IsNear(TPixel pix, Double_t distance, Bool_t manhattan){
//     //If the pixels are attached in a side (not edge)
//     if(GetMDistance(pix)==1){        return kTRUE;
//     }

//     if(manhattan){
//         if(GetMDistance(pix) < distance) return kTRUE;
//     }else{
//         if(GetEDistance(pix) < distance) return kTRUE;
//     }

//     return kFALSE;
// }

// Bool_t TPixel::operator==(TPixel pix){
//     if(pix.x == x && pix.y == y){
//         if(t==-1 || pix.t==-1){
//             return kTRUE;
//         }else{
//             if(t==pix.t){
//                 return kTRUE;
//             }
//         }
//     }
//     return kFALSE;
// }

// UInt_t TPixel::GetMDistance(const TPixel &pix){ //Manhattan Distance
//     return TMath::Abs(Int_t(pix.x-x)) + TMath::Abs(Int_t(pix.y-y));

// }

// Double_t TPixel::GetEDistance(const TPixel &pix){ //Eucludian Distance
//     return TMath::Sqrt(TMath::Power(Double_t(pix.x-x),2)
//                      + TMath::Power(Double_t(pix.y-y),2)
//                      );
// }

// TString TPixel::Print(Bool_t quite=kFALSE){
//     TString ans = TString::Format("[%u, %u, %u]", t,x,y);
//     if (!quite){
//         std::cout<<"TPixel "<<ans << " "<<std::endl;
//     }
//     return ans;
// }

#endif