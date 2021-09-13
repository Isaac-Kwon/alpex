#ifndef __TMAP2Base__
#define __TMAP2Base__ 1

#include "TROOT.h"

// TMap2Base2<IndexType, ReturnType>

template <typename L, typename T>
class TMap2Base{

    public:
    TMap2Base();
    TMap2Base(L max1, L max2, T val=0);
    T Get(L index1, L index2);
    T & operator[](L i);
    // T & operator[](L i, L j);
    T & operator()(L i);
    T & operator()(L i, L j);
    T   Set(L i, L j, T val);
    T   At(L i);
    T   At(L i, L j);
    Bool_t IsInside(L index);
    Bool_t IsInside(L index1, L index2);

    // T FindHighest(){FindHighest(nullptr, nullptr, 0);}
    // T FindHighest(L& index1, L& index2, Int_t rank=0);

    Int_t CountHigher(T val, Bool_t same=kTRUE){return CountValue(val, kTRUE, same);}
    Int_t CountLower(T val, Bool_t same=kTRUE){return CountValue(val, kFALSE, same);}

    void PrintStatus(){
        std::cout<<"TMap2\t"<<fM<<"x"<<fN<<std::endl;
    }

    protected:
    void Freeing();
    Int_t CountValue(T val, Bool_t high, Bool_t same=kTRUE);

    private:
    void Init();
    T * fElement;
    L   fM=1;
    L   fN=1;

    T   fVal=-1;
};

template <typename L, typename T>
TMap2Base<L,T>::TMap2Base(){
    Init();
}

template <typename L, typename T>
TMap2Base<L,T>::TMap2Base(L max1, L max2, T val): fM(max1), fN(max2), fVal(val){
    Init();
}

template <typename L, typename T>
void TMap2Base<L,T>::Init(){
    fElement = (T*) std::malloc(fM*fN*sizeof(T));
    for(int i=0; i<fN*fM; i++){
        fElement[i] = 0.;
    }
}

template <typename L, typename T>
void TMap2Base<L,T>::Freeing(){
    // if(fElement == nullptr){
    //     return;
    // }
    std::free(fElement);
}

template <typename L, typename T>
T TMap2Base<L,T>::At(L i){
    if( !IsInside(i) ){
        std::cout<<"TMap2Base::At(1) - Out if index"<<"["<<i<<"]"<<std::endl;
        return -1;
    }
    return fElement[i];
}

template <typename L, typename T>
T TMap2Base<L,T>::At(L i, L j){
    if( !IsInside(i,j) ){
        std::cout<<"TMap2Base::At(2) - Out if index "<<"["<<i<<","<<j<<"]"<<std::endl;
        return -1;
    }
    return fElement[fM*j + i];
}

template <typename L, typename T>
T TMap2Base<L,T>::Set(L i, L j, T val){
    if( !IsInside(i,j) ){
        std::cout<<"TMap2Base::Set - Out if index "<<"["<<i<<","<<j<<"]"<<std::endl;
        return -1;
    }
    fElement[fM*j + i] = val;
    return val;
}

template <typename L, typename T>
T & TMap2Base<L,T>::operator[](L i){
    if( !IsInside(i) ){
        std::cout<<"TMap2Base::operator[](1) - Out if index"<<"["<<i<<"]"<<std::endl;
        return fVal;
    }
    return fElement[i];
}

// template <typename L, typename T>
// T & TMap2Base<L,T>::operator[](L i, L j){
//     if( !IsInside(i, j) ){
//         std::cout<<"TMap2Base::operatorp[](2) - Out if index"<<"["<<i<<","<<j<<"]"<<std::endl;
//         return fVal;
//     }
//     return fElement[i*fM+j];
// }

template <typename L, typename T>
T & TMap2Base<L,T>::operator()(L i){
    if( !IsInside(i) ){
        std::cout<<"TMap2Base::operator()(1) - Out if index"<<"["<<i<<"]"<<std::endl;
        return fVal;
    }
    return fElement[i];
}

template <typename L, typename T>
T & TMap2Base<L,T>::operator()(L i, L j){
    if( !IsInside(i, j) ){
        std::cout<<"TMap2Base::operator()(2) - Out if index"<<"["<<i<<","<<j<<"]"<<std::endl;
        return fVal;
    }
    return fElement[fM*j + i];
}

template <typename L, typename T>
Bool_t TMap2Base<L,T>::IsInside(L i){
    if(i >= 0 && i< fN*fM) return kTRUE;
    return kFALSE;
}

template <typename L, typename T>
Bool_t TMap2Base<L,T>::IsInside(L i, L j){
    if( i>=0 && i<fM && j>=0 && j<fN ){
        return kTRUE;
    }
    return kFALSE;
}

template <typename L, typename T>
Int_t TMap2Base<L,T>::CountValue(T val, Bool_t high, Bool_t same){
    Int_t ans=0;

    Int_t maxind = fN;
    maxind *= fM;

    for(Int_t i=0; i<maxind; i++){
        T target = At(i);
        if(same){
            if(val == target){
                ans++;
                continue;
            }
        }
        if(high ? target>val : target<val ){
            ans++;
            continue;
        }
    }
    return ans;
}



#endif
