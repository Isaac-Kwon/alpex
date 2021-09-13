#ifndef __AXVECTOR__
#define __AXVECTOR__ 1

#include "string"

class AXVector{
    public:
    AXVector();
    AXVector(double x, double y);
    AXVector(double x, double y, double z);
    AXVector(const AXVector& other);

    //Get
    double    X();
    double    Y();
    double    Z();
    void      Get(double& x, double& y, double& z);
    AXVector  Unit();

    //Operator, returning result
    inline double    Dot(AXVector other) const;
    inline AXVector  Cross(AXVector other) const;
    AXVector  Scale(double& number);
    double    Norm();
    double    Norm2();

    //Operator, Self operation 
    void      AddThis(const AXVector& other);
    void      SubstractThis(const AXVector& other);
    void      CrossThis(const AXVector& other);
    void      ScaleThis(const double& number);

    //Operator, Coveniences
    // AXVector& operator+(const AXVector& other); //Add
    // AXVector& operator-(const AXVector& other); //Substract
    // double    operator*(const AXVector& other); //Dot product
    // AXVector& operator*(const double&   other); //Scaling
    // AXVector& operator^(const AXVector& other); //Cross product

    inline AXVector & operator=(const AXVector& other);

    friend AXVector operator+(AXVector& a, AXVector& b);
    friend AXVector operator+(AXVector  a, AXVector  b);
    // friend AXVector operator-(AXVector& a, AXVector& b);
    friend AXVector operator-(AXVector  a, AXVector  b);
    // friend AXVector operator^(AXVector& a, AXVector& b);
    friend AXVector operator^(AXVector  a, AXVector  b);
    // friend AXVector operator* (AXVector &vec,   double  other);
    // friend AXVector operator* (double    other, AXVector& vec);
    friend AXVector operator* (AXVector vec,   double  other);
    friend AXVector operator* (double    other, AXVector vec);
    // friend double   operator* (AXVector& other, AXVector& vec);
    friend double   operator* (AXVector other, AXVector vec);

    inline AXVector& operator+=(const AXVector& other); //Add
    inline AXVector& operator-=(const AXVector& other); //Substract
    inline AXVector& operator*=(const double&   other); //Scaling
    inline AXVector& operator^=(const AXVector& other); //Cross product

    double    GetAngle(AXVector other);
    double    GetAngleDegree(AXVector other);

    std::string Print(bool quite=false);

    protected:
    private:
    double fX=0., fY=0., fZ=0;

};

inline AXVector& AXVector::operator=(const AXVector& other){
    fX = other.fX;
    fY = other.fY;
    fZ = other.fZ;
    return *this;
}

inline AXVector& AXVector::operator+=(const AXVector& other){
    fX += other.fX;
    fY += other.fY;
    fZ += other.fX;
    return (*this);
}

inline AXVector& AXVector::operator-=(const AXVector& other){
    fX -= other.fX;
    fY -= other.fY;
    fZ -= other.fX;
    return (*this);
}

inline AXVector& AXVector::operator*=(const double&   other){
    fX *= other;
    fY *= other;
    fZ *= other;
    return (*this);
}

inline AXVector& AXVector::operator^=(const AXVector& other){
    fX = fY*other.fZ-fY*other.fZ;
    fY = fZ*other.fX-fX*other.fZ;
    fZ = fX*other.fY-fY*other.fX;
    return (*this);
}

inline double    AXVector::Dot(AXVector other) const{
    return fX*other.fX + fY*other.fY + fZ*other.fZ;
}

inline AXVector  AXVector::Cross(AXVector other) const{
    return AXVector(fY*other.fZ-other.fY*fZ, fZ*other.fX-other.fZ*fX, fX*other.fY-other.fX*fY);
}

// AXVector operator+ ( AXVector &, AXVector & );
AXVector operator+ ( AXVector  , AXVector   );
// AXVector operator- ( AXVector &, AXVector & );
AXVector operator- ( AXVector  , AXVector   );
// AXVector operator^ ( AXVector &, AXVector & );
AXVector operator^ ( AXVector  , AXVector   );

// AXVector operator* ( AXVector & ,  double  a );
// AXVector operator* ( double   a ,  AXVector & );
// double   operator* ( AXVector & ,  AXVector & );

AXVector operator* ( AXVector  ,  double  a );
AXVector operator* ( double   a ,  AXVector  );
double   operator* ( AXVector  ,  AXVector  );

#endif