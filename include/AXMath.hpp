#ifndef __AXMATH__
#define __AXMATH__ 1

#include "TROOT.h"

namespace AXMath{

    // constexpr Double_t InvPi()     {return 0.3183098861837906715377675267450287240689192;}
    // constexpr Double_t InvSqrtPi() {return 0.5641895835477562869480794515607725858440506;}
    // constexpr Double_t InvSqrt2Pi(){return 0.3989422804014326779399460599343818684758586;}
    // constexpr Double_t Degree()    {return 0.01745329251994329576923690768488612713442871;}

    const Double_t InvPi        = 0.3183098861837906715377675267450287240689192;
    const Double_t InvSqrtPi    = 0.5641895835477562869480794515607725858440506;
    const Double_t InvSqrt2Pi   = 0.3989422804014326779399460599343818684758586;
    const Double_t Degree       = 0.01745329251994329576923690768488612713442871;    
    const Double_t Pi           = 3.1415926535897932384626433832795028841971693;

    namespace Gaussian{
        // Simplest Gaussian Distribution (General)
        Double_t Simple1D(Double_t x,
                          Double_t integral        =1., 
                          Double_t mean            =0., 
                          Double_t sigma           =1.);
        Double_t Simple2D(Double_t x,
                          Double_t y,
                          Double_t integral      =1.,
                          Double_t meanx         =0,
                          Double_t meany         =0,
                          Double_t sigma         =1.);
        Double_t UniAxis2D(Double_t x,
                           Double_t y,
                           Double_t integral     =1.,
                           Double_t meanx        =0.,
                           Double_t meany        =0.,
                           Double_t sigmax       =1.,
                           Double_t sigmay       =1.);
        Double_t RotAxis2D(Double_t x,
                           Double_t y,
                           Double_t integral     = 1.,
                           Double_t meanx        = 0.,
                           Double_t meany        = 0.,
                           Double_t sigmax       = 1.,
                           Double_t sigmay       = 1.,
                           Double_t rotation     = 0.);
        Double_t Assymetry2D(Double_t x,
                             Double_t y,
                             Double_t integral   = 1.,
                             Double_t meanx      = 0.,
                             Double_t meany      = 0.,
                             Double_t sigmax     = 1.,
                             Double_t sigmay     = 1.,
                             Double_t rotation   = 0.,
                             Double_t length     = 0.,
                             Double_t tilt       = 0.,
                             Double_t dispersion = 1.);
        // Double_t UniCylinder3D(Double_t x,
        //                        Double_t y,
        //                        Double_t z,
        //                        Double_t dirx       = 0.,
        //                        Double_t diry       = 0.,
        //                        Double_t dirz       = 1.,
        //                        Double_t stx        = 0.,
        //                        Double_t sty        = 0.,
        //                        Double_t stz        = 0.,
        //                        Double_t rotcx      = ,
        //                        Double_t rotcy      = ,
        //                        Double_t rotcx      = ,
        //                        Double_t integral   = 1.,
        //                        Double_t sigmax     = 1.,
        //                        Double_t sigmay     = 1.,
        //                        Double_t rotation   = 0.,
        //                        Double_t length     = 0.,
        //                        Double_t dispersion = 1.);
        namespace AF{
            Double_t Simple1D     (Double_t* x, Double_t* par);
            Double_t Simple2D     (Double_t* x, Double_t* par);
            Double_t UniAxis2D    (Double_t* x, Double_t* par);
            Double_t RotAxis2D    (Double_t* x, Double_t* par);
            Double_t Assymetry2D  (Double_t* x, Double_t* par);
            // Double_t UniCylinder3D(Double_t* x, Double_t* par);
        }
    }
    namespace WoodSaxon{
        Double_t Simple1D(Double_t x,
                          Double_t maximum = 1.0,
                          Double_t mean    = 0.0,
                          Double_t a       = 1.0,
                          Double_t radius  = 1.0);
        Double_t Simple2D(Double_t x,
                          Double_t y,
                          Double_t maximum = 1.0,
                          Double_t meanx   = 0.0,
                          Double_t meany   = 0.0,
                          Double_t a       = 1.0,
                          Double_t radius  = 1.0);
        Double_t UniAxis2D(Double_t x,
                           Double_t y,
                           Double_t maximum = 1.0,
                           Double_t meanx   = 0.0,
                           Double_t meany   = 0.0,
                           Double_t ax      = 1.0,
                           Double_t ay      = 1.0,
                           Double_t radiusX = 1.0,
                           Double_t radiusY = 1.0); //Non-Isometric but same with axis XY
        Double_t RotAxis2D(Double_t x,
                           Double_t y,
                           Double_t maximum  = 1.0,
                           Double_t meanx    = 0.0,
                           Double_t meany    = 0.0,
                           Double_t ax       = 1.0,
                           Double_t ay       = 1.0,
                           Double_t radiusX  = 1.0,
                           Double_t radiusY  = 1.0,
                           Double_t rotation = 0.0); //Non-Isometric but same with axis XY
        Double_t RotAxis2DUnsync(Double_t x,
                                 Double_t y,
                                 Double_t maximum  = 1.0,
                                 Double_t meanx    = 0.0,
                                 Double_t meany    = 0.0,
                                 Double_t ax       = 1.0,
                                 Double_t ay       = 1.0,
                                 Double_t radiusX  = 1.0,
                                 Double_t radiusY  = 1.0,
                                 Double_t rotationA = 0.0,
                                 Double_t rotationR = 0.0); //Non-Isometric but same with axis XY
        namespace AF{
            Double_t Simple1D(Double_t* x, Double_t* par);
            Double_t Simple2D(Double_t* x, Double_t* par);
            Double_t UniAxis2D(Double_t* x, Double_t* par);
            Double_t RotAxis2D(Double_t* x, Double_t* par);
            Double_t RotAxis2DUnsync(Double_t* x, Double_t* par);
        }
    }

    namespace GeometryCalc{
        Double_t EllipseUniAxis(Double_t x, Double_t y, Double_t lx, Double_t ly); 
        Double_t EllipseRotAxis(Double_t x, Double_t y, Double_t lx, Double_t ly, Double_t theta);
        Double_t Angle(Double_t x, Double_t y, Double_t centx=0., Double_t centy=0.);
    }

    namespace TF{
        Double_t Test1Func(Double_t* x, Double_t* par);
    }

}

// AXMath::InvPi      = 0.3183098861837906715377675267450287240689192914809128974953346881177935952684530701802276055325061719;
// AXMath::InvSqrtPi  = 0.5641895835477562869480794515607725858440506293289988568440857217106424684414934144867436602021073634;
// AXMath::InvSqrt2Pi = 0.3989422804014326779399460599343818684758586311649346576659258296706579258993018385012523339073069364;
// AXMath::Degree     = 0.01745329251994329576923690768488612713442871888541725456097191440171009114603449443682241569634509482;

#endif