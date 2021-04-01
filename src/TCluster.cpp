#include "TCluster.hpp"

#include "iostream"
#include "TString.h"

#include "TCanvas.h"
#include "TPad.h"
#include "TClonesArray.h"

#include "TPixel.hpp"
#include "iostream"
#include "deque"

// #if !defined(__CLING__)
// ClassImp(TCluster)
// #endif

TCluster::TCluster(){
    // if(fPixes==nullptr) fPixes = new TClonesArray("TPixel", 1000);
}

TCluster::TCluster(const TPixel pix){
    // TCluster();
    // if(fPixes==nullptr) fPixes = new TClonesArray("TPixel", 1000);
    // std::cout<<"TCluster::TCluster - new cluster with pixel "<<std::endl;
    Append(pix);
}

TCluster::TCluster(const TCluster &cluster){
    // TObject::operator=(cluster);
    // fPixes    = new TClonesArray(*(cluster.fPixes));
    // if(fPixes==nullptr) fPixes = new TClonesArray("TPixel", 1000);
    CopyPixels(cluster);
    fNPixes   = cluster.fNPixes;
    ndistance = cluster.ndistance;
    manhattan = cluster.manhattan;
}

TCluster::TCluster(std::deque<TPixel> pixeldeque){
    // if(fPixes==nullptr) fPixes = new TClonesArray("TPixel", 1000);
    Int_t i=0;
    for(auto it = pixeldeque.begin(); it!=pixeldeque.end(); it++){
        Append(*it, 200);
    }

}

TCluster::~TCluster(){
    // if(fPixes!=nullptr){
        // fPixes->Clear("C");
    // }
}

void TCluster::Clear(){
    // fPixes->Clear("C");
}

void TCluster::CopyPixels(const TCluster &cluster){
    // fPixes->Clear();
    fNPixes=0;
    fPixes.clear();
    for(Int_t i=0; i<cluster.fNPixes; i++){
        fPixes.emplace_back(cluster.fPixes[i]);
    }
}

Bool_t TCluster::Append(TPixel pix, Double_t distance){
    Compress();
    if(GetNPixel()==0){
        ConfirmAppend(pix);
        return kTRUE;
    }
    
    if(IsOverlapped(pix)){
        return kTRUE;
    }

    if(IsNear(pix, distance)){
        ConfirmAppend(pix);
        return kTRUE;
    }
    return kFALSE;
}

Bool_t TCluster::Append(TCluster cluster, Double_t distance){
    Compress();
    if(!IsNear(cluster, distance) && !IsOverlapped(cluster) ) return kFALSE;
    for(Int_t i=0; i<cluster.GetNPixel(); i++){
        TPixel pix = cluster.GetPixel(i);
        if(!IsOverlapped(pix)) ConfirmAppend(pix);
    }
    return kTRUE;
}

void   TCluster::ConfirmAppend(TPixel pix){
    Compress();
    fPixes.emplace_back(pix);
    fNPixes++;
    // std::cout<<"TCluster::ConfirmAppend - After Append, cluster = "<<Print(true)<<std::endl;
}

TPixel TCluster::GetPixel(Int_t pixn){
    if(pixn>=fNPixes){
        std::cout<<"TCluster::GetPixel - ERROR - Call index over the number of pixels in cluster " << pixn << "/" << Int_t(fNPixes)-1 << " | " << "Entries " << fPixes.size() <<std::endl;
        return TPixel();
    }
    return fPixes[pixn];
}

UShort_t TCluster::GetCenterX(){
    UInt_t sumx = 0;
    for(Int_t i=0; i<GetNPixel(); i++){
        sumx += GetPixel(i).GetX();
    }
    return UShort_t(sumx/GetNPixel());    

}

UShort_t TCluster::GetCenterY(){
    UInt_t sumy = 0;
    for(Int_t i=0; i<GetNPixel(); i++){
        sumy += GetPixel(i).GetY();
    }
    return UShort_t(sumy/GetNPixel());
}

TPixel TCluster::PopFront(){
    TPixel ans = fPixes.front();
    // fPixes->RemoveFirst();
    // fPixes->Compress();
    fPixes.pop_front();
    fNPixes--;
    return ans;
}

TPixel TCluster::PopBack(){
    TPixel ans = fPixes.back();
    // fPixes->RemoveFirst();
    // fPixes->Compress();
    fPixes.pop_back();
    fNPixes--;
    return ans;
}

Bool_t TCluster::IsNear(TPixel pix, Double_t distance){
    for(Int_t i=0; i<fNPixes; i++){
        TPixel pix1 = GetPixel(i);
        if(pix.IsNear(pix1, distance)){
            // std::cout<<"TCluster::IsNear - Pixel " << pix.Print(kTRUE) << "is near Cluster "<<Print(kTRUE)<<std::endl;
            return kTRUE;
        }
    }
    return kFALSE;
}

Bool_t TCluster::IsNear(TCluster cluster, Double_t distance){
    for(Int_t i=0; i<cluster.GetNPixel(); i++){
        TPixel pix1 = cluster.GetPixel(i);
        if(IsNear(pix1, distance)){
            return kTRUE;
        }
    }
    return kFALSE;
}

Bool_t TCluster::IsOverlapped(TPixel pix){
    for(Int_t i=0; i<fNPixes; i++){
        TPixel pix1 = GetPixel(i);
        if(pix1==pix) return kTRUE;
    }
    return kFALSE;
}

Bool_t TCluster::IsOverlapped(TCluster cluster){
    for(Int_t i=0; i<cluster.GetNPixel(); i++){
        TPixel pix1 = GetPixel(i);
        if(IsOverlapped(pix1)) return kTRUE;
    }
    return kFALSE;
}

Bool_t TCluster::IsCoreless(){
    UShort_t minx = -1, miny=-1;
    UShort_t maxx = 0, maxy=0;
    UShort_t cx = GetCenterX();
    UShort_t cy = GetCenterY();
    Bool_t center=kFALSE;
    Bool_t xscan=kTRUE;
    Bool_t yscan=kTRUE;

    for(UInt_t i=0; i<GetNPixel(); i++){
        TPixel pix = GetPixel(i);
        if(pix.GetX()==cx && pix.GetY()==cy) center=kTRUE;

        minx = pix.GetX() > minx ? minx : pix.GetX();
        miny = pix.GetY() > miny ? miny : pix.GetY();

        maxx = pix.GetX() < maxx ? maxx : pix.GetX();
        maxy = pix.GetY() < maxy ? maxy : pix.GetY();
    }
    // if(center==kFALSE) return kTRUE;

    for(UShort_t y=miny; y<=maxy; y++){
        UShort_t firemin=-1, firemax=0;
        UInt_t   npixel=0;
        for(UInt_t i=0; i<GetNPixel(); i++){
            TPixel pix = GetPixel(i);
            if(pix.GetY()!=y) continue;
            firemin = pix.GetX() > firemin ? firemin : pix.GetX();
            firemax = pix.GetX() < firemax ? firemax : pix.GetX();
            npixel++;
        }
        if(npixel < (firemax-firemin)+1){
            xscan=kFALSE;
            break;
        }
    }

    for(UShort_t x=minx; x<=maxx; x++){
        UShort_t firemin=-1, firemax=0;
        UInt_t   npixel=0;
        for(UInt_t i=0; i<GetNPixel(); i++){
            TPixel pix = GetPixel(i);
            if(pix.GetX()!=x) continue;
            firemin = pix.GetY() > firemin ? firemin : pix.GetY();
            firemax = pix.GetY() < firemax ? firemax : pix.GetY();
            npixel++;
        }
        if(npixel < (firemax-firemin)+1){
            yscan=kFALSE;
            break;
        }
    }
  
    UShort_t ct = 0;
    if(!center){ct++;}
    if(!xscan){ct++;}
    if(!yscan){ct++;}
    
    if(ct>=2){
        return kTRUE;
    }else{
        return kFALSE;
    }

}

Bool_t TCluster::operator==(TCluster cluster){
    if(cluster.GetNPixel() != GetNPixel()) return kFALSE;
    for(Int_t i=0; i<cluster.GetNPixel(); i++){
        TPixel pix = GetPixel(i);
        if(! IsOverlapped(pix) ) return kFALSE;
    }
    return kTRUE;
}

TString TCluster::Print(Bool_t quite){
    TString ans = TString::Format("(%d) [", fNPixes);
    
    Bool_t first = kTRUE;
    for(Int_t i=0; i<GetNPixel(); i++){
        TPixel pix = GetPixel(i);
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

void TCluster::Dump(TClusterDump &dump){
    // std::cout<<"TCluster::Dump - Start"<<std::endl;
     
    dump.t = GetPixel(0).GetT();
    // std::cout<<"TCluster::Dump - t="<<dump.t<<std::endl;
    UShort_t minx = -1;
    UShort_t maxx = 0;
    UShort_t miny = -1;
    UShort_t maxy = 0;
    UInt_t sumx = 0;
    UInt_t sumy = 0;

    for(UInt_t i=0; i<GetNPixel(); i++){
        TPixel pix = GetPixel(i);
        dump.x[i] = pix.GetX();
        dump.y[i] = pix.GetY();

        // std::cout<<"TCluster::Dump - DUMP PIXEL No."<<i<<"\t"<<"["<< dump.x[i] << "," << dump.y[i] <<"]"<<std::endl;

        minx = pix.GetX() > minx ? minx : pix.GetX();
        miny = pix.GetY() > miny ? miny : pix.GetY();

        maxx = pix.GetX() < maxx ? maxx : pix.GetX();
        maxy = pix.GetY() < maxy ? maxy : pix.GetY();
    }
    dump.n = GetNPixel();
    dump.centerx = GetCenterX();
    dump.centery = GetCenterY();
    dump.sizex = maxx-minx+1;
    dump.sizey = maxy-miny+1;
    dump.coreless = IsCoreless();

    return;
}