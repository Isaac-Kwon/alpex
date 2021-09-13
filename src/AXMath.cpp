#include "AXMath.hpp"

#include "TROOT.h"
#include "TMath.h"
#include "AXVector.hpp"

Double_t AXMath::Gaussian::Simple1D(Double_t x,
                                    Double_t integral,
                                    Double_t mean,
                                    Double_t sigma){
    if(sigma <= 0) return 0.;
    return TMath::Gaus(x, mean, sigma, true) * integral;
}

Double_t AXMath::Gaussian::Simple2D(Double_t x,
                                    Double_t y,
                                    Double_t integral,
                                    Double_t meanx,
                                    Double_t meany,
                                    Double_t sigma){
    if(sigma <= 0) return 0.;
    Double_t chi = TMath::Sqrt( TMath::Power(x-meanx,2) + TMath::Power(y-meany,2) ) / sigma;
    return AXMath::Gaussian::Simple1D(chi, integral*AXMath::InvSqrt2Pi/(sigma*sigma));
}

Double_t AXMath::Gaussian::UniAxis2D(Double_t x,
                                     Double_t y,
                                     Double_t integral,
                                     Double_t meanx,
                                     Double_t meany,
                                     Double_t sigmax,
                                     Double_t sigmay){
    if(sigmax<=0 || sigmay<=0) return 0.;
    Double_t chi = TMath::Sqrt(TMath::Power(x-meanx,2)/TMath::Power(sigmax,2) + TMath::Power(y-meany,2)/TMath::Power(sigmay,2));
    return AXMath::Gaussian::Simple1D(chi, integral*AXMath::InvSqrt2Pi/(sigmax*sigmay));
}

Double_t AXMath::Gaussian::RotAxis2D(Double_t x         ,
                                     Double_t y         ,
                                     Double_t integral  ,
                                     Double_t meanx     ,
                                     Double_t meany     ,
                                     Double_t sigmax    ,
                                     Double_t sigmay    ,
                                     Double_t rotation  ){
    if(sigmax<=0 || sigmay<=0) return 0.;
    Double_t xp = TMath::Cos(rotation)*(x-meanx) + TMath::Sin(rotation)*(y-meany);
    Double_t yp = TMath::Cos(rotation)*(y-meany) - TMath::Sin(rotation)*(x-meanx);
    Double_t chi = TMath::Sqrt(TMath::Power(xp,2)/TMath::Power(sigmax,2) + TMath::Power(yp,2)/TMath::Power(sigmay,2));
    return AXMath::Gaussian::Simple1D(chi, integral*AXMath::InvSqrt2Pi/(sigmax*sigmay));
}

// Double_t AXMath::Gaussian::UniCylinder3D(Double_t x,
//                                          Double_t y,
//                                          Double_t z,
//                                          Double_t dirx       ,
//                                          Double_t diry       ,
//                                          Double_t dirz       ,
//                                          Double_t rotcx      ,
//                                          Double_t rotcy      ,
//                                          Double_t rotcx      ,
//                                          Double_t stx        ,
//                                          Double_t sty        ,
//                                          Double_t stz        ,
//                                          Double_t integral   ,
//                                          Double_t sigmax     ,
//                                          Double_t sigmay     ,
//                                          Double_t rotation   ,
//                                          Double_t length     ,
//                                          Double_t dispersion ){
    
//     AXVector pos   = AXVector(x,y,z);
//     AXVector dir   = AXVector(dirx,diry,dirz).Unit();
//     AXVector stt   = AXVector(stx,sty,stz);

//     Double_t pdist = ((stt-pos).Cross(dir)).Norm();
//     Double_t ldist = TMath::Sqrt((stt-pos).Norm2() - TMath::Power(pdist,2));
    
    

//     return 0;
// }

Double_t AXMath::Gaussian::Assymetry2D(Double_t x,
                        Double_t y,
                        Double_t integral   ,
                        Double_t meanx      ,
                        Double_t meany      ,
                        Double_t sigmax     ,
                        Double_t sigmay     ,
                        Double_t rotation   ,
                        Double_t length     ,
                        Double_t tilt       ,
                        Double_t dispersion ){

    Double_t xp = x-meanx;
    Double_t yp = y-meany;

    // if( tilt > 0 && -1* length / TMath::Sin(tilt) > xp) return 0;

    Double_t z   = 0;
    Double_t xp3 = xp*TMath::Cos(tilt) + z*TMath::Sin(tilt);
    Double_t yp3 = yp;
    Double_t zp3 = -1*xp*TMath::Sin(tilt) + z*(tilt) - length;

    Double_t sigmapx = -1* dispersion * zp3 + sigmax;
    Double_t sigmapy = -1* dispersion * zp3 + sigmay;

    if(sigmapx<=0 || sigmapy<=0 ) return 0;

    return AXMath::Gaussian::RotAxis2D(xp3, yp3, integral, 0, 0, sigmapx, sigmapy, rotation);
}

Double_t AXMath::Gaussian::AF::Simple1D(Double_t* x, Double_t* par){
    return AXMath::Gaussian::Simple1D(x[0],par[0], par[1], par[2]);
}

Double_t AXMath::Gaussian::AF::Simple2D(Double_t* x, Double_t* par){
    return AXMath::Gaussian::Simple2D(x[0], x[1], par[0], par[1], par[2], par[3]);
}

Double_t AXMath::Gaussian::AF::UniAxis2D(Double_t* x, Double_t* par){
    return AXMath::Gaussian::UniAxis2D(x[0], x[1],par[0], par[1], par[2], par[3], par[4]);
}

Double_t AXMath::Gaussian::AF::RotAxis2D(Double_t* x, Double_t* par){
    return AXMath::Gaussian::RotAxis2D(x[0], x[1], par[0], par[1], par[2], par[3], par[4], par[5]);
}

Double_t AXMath::Gaussian::AF::Assymetry2D(Double_t* x, Double_t* par){
    return AXMath::Gaussian::Assymetry2D  (x[0], x[1], par[0], par[1], par[2], par[3], par[4], par[5], par[6], par[7], par[8]);
}

// Double_t AXMath::Gaussian::AF::UniCylinder3D(Double_t* x, Double_t* par){
//     return AXMath::Gaussian::UniCylinder3D(x[0], x[1], x[2], par[0], par[1], par[2], par[3], par[4], par[5], par[6], par[7], par[8], par[9], par[10], par[11]);
// }

Double_t AXMath::WoodSaxon::Simple1D(Double_t x, Double_t maximum, Double_t mean, Double_t a, Double_t radius){
    Double_t xp = x-mean;
    xp = x > 0 ? x : -1*x;
    return maximum / ( 1+TMath::Exp((xp-radius)/a));
}

Double_t AXMath::WoodSaxon::Simple2D(Double_t x, Double_t y,
                                     Double_t maximum, Double_t meanx, Double_t meany, Double_t a, Double_t radius){
    Double_t dist = TMath::Sqrt(TMath::Power(x-meanx,2)+TMath::Power(y-meany,2));
    return AXMath::WoodSaxon::Simple1D(dist, maximum, 0, a, radius);
}

Double_t AXMath::WoodSaxon::UniAxis2D(Double_t x, Double_t y, 
                   Double_t maximum, Double_t meanx, Double_t meany, Double_t ax, Double_t ay, Double_t radiusX, Double_t radiusY){
    Double_t dist = TMath::Sqrt(TMath::Power(x-meanx,2)+TMath::Power(y-meany,2));
    Double_t a      = AXMath::GeometryCalc::EllipseUniAxis(x-meanx, y-meany, ax, ay);
    Double_t radius = AXMath::GeometryCalc::EllipseUniAxis(x-meanx, y-meany, radiusX, radiusY);
    return AXMath::WoodSaxon::Simple1D(dist, maximum, 0, a, radius);
}

Double_t AXMath::WoodSaxon::RotAxis2D(Double_t x, Double_t y,
                   Double_t maximum, Double_t meanx, Double_t meany,
                   Double_t ax, Double_t ay, Double_t radiusX, Double_t radiusY, Double_t rotation){
    Double_t dist = TMath::Sqrt(TMath::Power(x-meanx,2)+TMath::Power(y-meany,2));
    Double_t a      = AXMath::GeometryCalc::EllipseRotAxis(x-meanx, y-meany, ax, ay, rotation);
    Double_t radius = AXMath::GeometryCalc::EllipseRotAxis(x-meanx, y-meany, radiusX, radiusY, rotation);
    return AXMath::WoodSaxon::Simple1D(dist, maximum, 0, a, radius);
}

Double_t AXMath::WoodSaxon::RotAxis2DUnsync(Double_t x, Double_t y,
                   Double_t maximum, Double_t meanx, Double_t meany,
                   Double_t ax, Double_t ay, Double_t radiusX, Double_t radiusY, Double_t rotationA, Double_t rotationR){
    Double_t dist = TMath::Sqrt(TMath::Power(x-meanx,2)+TMath::Power(y-meany,2));
    Double_t a      = AXMath::GeometryCalc::EllipseRotAxis(x-meanx, y-meany, ax, ay, rotationA);
    Double_t radius = AXMath::GeometryCalc::EllipseRotAxis(x-meanx, y-meany, radiusX, radiusY, rotationR);
    return AXMath::WoodSaxon::Simple1D(dist, maximum, 0, a, radius);
}

Double_t AXMath::WoodSaxon::AF::Simple1D(Double_t* x, Double_t* par){
    return AXMath::WoodSaxon::Simple1D(x[0], par[0], par[1], par[2], par[3]);
}

Double_t AXMath::WoodSaxon::AF::Simple2D(Double_t* x, Double_t* par){
    return AXMath::WoodSaxon::Simple2D(x[0], x[1], par[0], par[1], par[2], par[3], par[4]);
}

Double_t AXMath::WoodSaxon::AF::UniAxis2D(Double_t* x, Double_t* par){
    return AXMath::WoodSaxon::UniAxis2D(x[0], x[1], par[0], par[1], par[2], par[3], par[4], par[5], par[6]);
}

Double_t AXMath::WoodSaxon::AF::RotAxis2D(Double_t* x, Double_t* par){
    return AXMath::WoodSaxon::RotAxis2D(x[0], x[1], par[0], par[1], par[2], par[3], par[4], par[5], par[6], par[7])+par[8];
}

Double_t AXMath::WoodSaxon::AF::RotAxis2DUnsync(Double_t* x, Double_t* par){
    return AXMath::WoodSaxon::RotAxis2DUnsync(x[0], x[1], par[0], par[1], par[2], par[3], par[4], par[5], par[6], par[7], par[8]);
}

Double_t AXMath::GeometryCalc::EllipseUniAxis(Double_t x, Double_t y, Double_t lx, Double_t ly){
    return AXMath::GeometryCalc::EllipseRotAxis(x,y,lx,ly,0);
}

Double_t AXMath::GeometryCalc::EllipseRotAxis(Double_t x, Double_t y, Double_t lx, Double_t ly, Double_t theta){
    Double_t angle = AXMath::GeometryCalc::Angle(x,y) - theta;
    Double_t xp = lx * TMath::Cos(angle);
    Double_t yp = ly * TMath::Sin(angle);
    
    return TMath::Sqrt(TMath::Power(xp,2) + TMath::Power(yp,2));
}

Double_t AXMath::GeometryCalc::Angle(Double_t x, Double_t y, Double_t centx, Double_t centy){
    Double_t xp = x - centx;
    Double_t yp = y - centy;
    
    return TMath::ATan2(yp,xp);

}

Double_t AXMath::TF::Test1Func(Double_t* x, Double_t* par){
    return AXMath::Gaussian::RotAxis2D  (x[0],x[1], par[0], par[2], par[3], par[4], par[5], par[10]) *
           AXMath::WoodSaxon::RotAxis2D (x[0],x[1], par[1], par[2], par[3], par[6], par[7], par[8], par[9], par[11]);
}

