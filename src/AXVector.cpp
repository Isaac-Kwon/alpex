#include "AXVector.hpp"
#include "AXMath.hpp"

#include "math.h"

#include <iostream>
#include <string>
#include <sstream>

AXVector::AXVector(): fX(0), fY(0), fZ(0){}
AXVector::AXVector(double x, double y): fX(x), fY(y), fZ(0){}
AXVector::AXVector(double x, double y, double z): fX(x), fY(y), fZ(z){}
AXVector::AXVector(const AXVector& other): fX(other.fX), fY(other.fY), fZ(other.fZ){;}

//Get
double    AXVector::X(){return fX;}
double    AXVector::Y(){return fY;}
double    AXVector::Z(){return fZ;}
void      AXVector::Get(double& x, double& y, double& z){x = fX; y = fY; z=fZ;}

AXVector  AXVector::Unit(){
    double norm = Norm();
    return AXVector(fX/norm, fY/norm, fZ/norm);
}

//Operator, returning result

AXVector AXVector::Scale(double& number){
    // return AXVector(number*fX, number*fY, number*fZ);
    return AXVector(number*fX, number*fY, number*fZ);
}

double    AXVector::Norm(){
    return sqrt(fX*fX + fY*fY + fZ*fZ);
}

double    AXVector::Norm2(){
    return fX*fX + fY*fY + fZ*fZ;
}

//Operator, Self Closed operation 
void      AXVector::AddThis(const AXVector& other){
    // fX += other.fX;
    // fY += other.fY;
    // fZ += other.fZ;
    (*this)+=other;
}
void      AXVector::SubstractThis(const AXVector& other){
    // fX -= other.fX;
    // fY -= other.fY;
    // fZ -= other.fZ;
    (*this)-=other;
}
void      AXVector::CrossThis(const AXVector& other){
    (*this)^=other;
}

void      AXVector::ScaleThis(const double& number){
    (*this).operator*=(number);
}

//Operator, Coveniences

// double    AXVector::operator*(const AXVector& other){
//     return sqrt(fX*other.fX + fY*other.fY + fZ*other.fZ);
// }

// AXVector& AXVector::operator*(const double&   other){
//     return * new AXVector(fX*other, fY*other, fZ*other);
// }

// AXVector& AXVector::operator^(const AXVector& other){
//     double x = fY*other.fZ-fY*other.fZ;
//     double y = fZ*other.fX-fX*other.fZ;
//     double z = fX*other.fY-fY*other.fX;
//     return * new AXVector(x,y,z);
// }

double    AXVector::GetAngle(AXVector other){ //Get Angle [0, Pi]
    return acos(Unit().Dot(other.Unit()));
}

double    AXVector::GetAngleDegree(AXVector other){
    return GetAngle(other)*180/AXMath::Pi;
}

std::string AXVector::Print(bool quite){
    std::ostringstream anss;

    anss << "[" << fX << "," << fY << "," << fZ << "]";

    std::string ans = anss.str();
    if(!quite){
        // std::cout<<fX<<"\t"<<fY<<"\t"<<fZ<<std::endl;
        std::cout<<"[x,y,z] = " << ans << std::endl;
    }
    
    return ans;
}

// AXVector operator+ ( AXVector& a,  AXVector& b) {
//     return AXVector(a.fX+b.fX, a.fY+b.fY, a.fZ+b.fZ);
// }

AXVector operator+ ( AXVector  a,  AXVector  b) {
    return AXVector(a.fX+b.fX, a.fY+b.fY, a.fZ+b.fZ);
}

// AXVector operator- ( AXVector& a,  AXVector& b) {
//     return AXVector(a.fX-b.fX, a.fY-b.fY, a.fZ-b.fZ);
// }

AXVector operator- ( AXVector  a,  AXVector  b) {
    return AXVector(a.fX-b.fX, a.fY-b.fY, a.fZ-b.fZ);
}

// AXVector operator^ ( AXVector& a,  AXVector& b) {
//     double x = a.fY*b.fZ-a.fY*b.fZ;
//     double y = a.fZ*b.fX-a.fX*b.fZ;
//     double z = a.fX*b.fY-a.fY*b.fX;
//     return AXVector(x,y,z);
// }

AXVector operator^ ( AXVector  a,  AXVector  b) {
    double x = a.fY*b.fZ-a.fZ*b.fY;
    double y = a.fZ*b.fX-a.fX*b.fZ;
    double z = a.fX*b.fY-a.fY*b.fX;
    return AXVector(x,y,z);
}

// AXVector operator* ( AXVector &vec, double other) {
//     return AXVector(other*vec.fX, other*vec.fY, other*vec.fZ);
// }

AXVector operator* ( AXVector vec, double other) {
    return AXVector(other*vec.fX, other*vec.fY, other*vec.fZ);
}

// double   operator* ( AXVector& other,  AXVector& vec) {
//     return vec.Dot(other);
// }

// AXVector operator* (double other,  AXVector &vec) {
//     return AXVector(other*vec.fX, other*vec.fY, other*vec.fZ);
// }

AXVector operator* (double other,  AXVector vec) {
    return AXVector(other*vec.fX, other*vec.fY, other*vec.fZ);
}

double   operator* ( AXVector  other,  AXVector  vec) {
    return vec.Dot(other);
}