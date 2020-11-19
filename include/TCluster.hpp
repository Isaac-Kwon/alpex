#ifndef TCLUSTER_H
#define TCLUSTER_H 1

#include "TROOT.h"
#include "TClonesArray.h"
#include "deque"
#include "TPixel.hpp"

#define CLUSTERMAX 100

struct TClusterDump{
    
    UShort_t centerx = 0;
    UShort_t centery = 0;

    UShort_t sizex    = 0;
    UShort_t sizey    = 0;

    Bool_t  coreless=kFALSE;

    Int_t t = -1;
    UShort_t x[CLUSTERMAX];
    UShort_t y[CLUSTERMAX];
    UShort_t n       = 0;
};
class TCluster: public TObject{
    private:
    TClonesArray *fPixes = 0;
    UInt_t fNPixes      = 0;
    Double_t ndistance = 0;
    Bool_t   manhattan = kFALSE; // kFALSE : Euclidean Distance
                                 // kTRUE  : Manhattan Distance 
    protected:
    void   CopyPixels(const TCluster &cluster);
    void   Compress(){fPixes->Compress();}
    void   ConfirmAppend(TPixel pix);
    public:
    TCluster();
    TCluster(const TPixel pix);
    TCluster(const TCluster &cluster);
    ~TCluster();
    void   SetMode(Bool_t IsManhattan){manhattan = IsManhattan;}
    UInt_t GetNPixel(){return fNPixes;}
    TPixel GetPixel(Int_t pixn);
    TPixel PopFront();
    UShort_t GetCenterX();
    UShort_t GetCenterY();
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
    void   Dump(TClusterDump &dump);

    ClassDef(TCluster,10);
};

TCluster::TCluster(){
    if(fPixes==nullptr) fPixes = new TClonesArray("TPixel", 1000);
}

TCluster::TCluster(const TPixel pix){
    // TCluster();
    if(fPixes==nullptr) fPixes = new TClonesArray("TPixel", 1000);
    // std::cout<<"TCluster::TCluster - new cluster with pixel "<<std::endl;
    Append(pix);
}

TCluster::TCluster(const TCluster &cluster){
    TObject::operator=(cluster);
    // fPixes    = new TClonesArray(*(cluster.fPixes));
    if(fPixes==nullptr) fPixes = new TClonesArray("TPixel", 1000);
    CopyPixels(cluster);
    fNPixes   = cluster.fNPixes;
    ndistance = cluster.ndistance;
    manhattan = cluster.manhattan;
}

TCluster::~TCluster(){
    fPixes->Clear();
    fPixes->Delete();
}

void TCluster::CopyPixels(const TCluster &cluster){
    fPixes->Clear();
    fNPixes=0;
    for(Int_t i=0; i<cluster.fNPixes; i++){
        fPixes->operator[](i) = new TPixel(*((TPixel*)cluster.fPixes->At(i)));
    }

}



Bool_t TCluster::Append(TPixel pix, UInt_t distance){
    // std::cout<<"TCluster::Append - Append pixel in cluster ("<<pix.Print(true)<<") to ("<<Print(true)<<")"<<std::endl;
    Compress();
    if(GetNPixel()==0){
        ConfirmAppend(pix);
        return kTRUE;
    }
    if(!IsNear(pix, distance)) return kFALSE;
    if(IsOverlapped(pix)) return kTRUE;
    ConfirmAppend(pix);
    return kTRUE;
}

Bool_t TCluster::Append(TCluster cluster, UInt_t distance){
    Compress();
    if(!IsNear(cluster, distance) && !IsOverlapped(cluster) ) return kFALSE;
    for(Int_t i=0; i<cluster.GetNPixel(); i++){
        TPixel pix = cluster.GetPixel(i);
        if(!IsOverlapped(pix)) ConfirmAppend(pix);
    }
    return kTRUE;
}

void   TCluster::ConfirmAppend(TPixel pix){
    // std::cout<<"TCluster::ConfirmAppend - "<< "Add pixel "<<pix.Print(true)<< " into cluster " << Print(true) <<std::endl;
    Compress();
    fPixes->operator[](fNPixes)= new TPixel(pix);
    fNPixes++;
    // std::cout<<"TCluster::ConfirmAppend - After Append, cluster = "<<Print(true)<<std::endl;
}

TPixel TCluster::GetPixel(Int_t pixn){
    if(pixn>=fNPixes){
        std::cout<<"TCluster::GetPixel - ERROR - Call index over the number of pixels in cluster " << pixn << "/" << Int_t(fNPixes)-1 << " | " << "Entries " << fPixes->GetEntries() <<std::endl;
        return TPixel();
    }
    return *((TPixel*) (fPixes->At(pixn)));
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
    TPixel ans = GetPixel(0);
    fPixes->RemoveFirst();
    fPixes->Compress();
    fNPixes--;
    return ans;
}

Bool_t TCluster::IsNear(TPixel pix, UInt_t distance){
    for(Int_t i=0; i<fNPixes; i++){
        TPixel pix1 = GetPixel(i);
        if(pix.IsNear(pix1, distance)){
            return kTRUE;
        }
    }
    return kFALSE;
}

Bool_t TCluster::IsNear(TCluster cluster, UInt_t distance){
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
  // will be updated
  return kFALSE;
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

    // std::cout<<"TCluster::Dump - n="<< dump.n <<std::endl;
    // std::cout<<"TCluster::Dump - centerx="<< dump.centerx <<std::endl;
    // std::cout<<"TCluster::Dump - centery="<< dump.centery <<std::endl;
    // std::cout<<"TCluster::Dump - sizex="<< (UInt_t) dump.sizex <<std::endl;
    // std::cout<<"TCluster::Dump - sizey="<< (UInt_t) dump.sizey <<std::endl;
    // std::cout<<"TCluster::Dump - coreless="<< dump.coreless <<std::endl;

    // std::cout<<"TCluster::Dump - SizeM x=["<<minx<<","<<maxx<<"] y=["<<miny<<","<<maxy<<"]"<<std::endl;

    // Int_t t = -1;
    // UShort_t x[CLUSTERMAX];
    // UShort_t y[CLUSTERMAX];
    // UShort_t n;
    // UChar_t sizex = 0;
    // UChar_t sizey = 0;
    // Bool_t  coreless;

    return;
}


#endif