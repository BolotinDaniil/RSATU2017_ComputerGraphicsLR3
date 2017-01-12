#pragma once

#include <cmath>
#include <stdio.h>
#include <time.h>
#include <cfloat>
#include <string.h>
#include <iostream>

using namespace std;

#ifndef EPS
    #define EPS 0.0000001
#endif // EPS

#ifndef M_PI
	#define M_PI 3.1415927
#endif

struct vec3{
    vec3(double _x = 0.0, double _y = 0.0, double _z = 0.0);
    vec3(const vec3& vec);

    vec3& operator *= (double scolar);
    vec3 operator * (double scolar)const;

    vec3& operator +=(const vec3& vec);
    vec3 operator +(const vec3& vec)const;

    vec3& operator -=(const vec3& vec);
    vec3 operator -(const vec3& vec)const;
    /// Dot
    double operator & (const vec3& v)const;
    /// Cross
    vec3 operator ^ (const vec3& v)const;

    bool operator == (const vec3& vec)const;
    bool operator != (const vec3& vec)const;

    friend ostream& operator <<(ostream& out, const vec3& vect);
    
    double length() const;

    double x;
    double y;
    double z;
};

struct vec2{
    vec2(double _x = 0.0, double _y = 0.0);
    vec2(const vec2& vec);

    vec2& operator *= (double scolar);
    vec2 operator * (double scolar)const;

    vec2& operator +=(const vec2& vec);
    vec2 operator +(const vec2& vec)const;

    vec2& operator -=(const vec2& vec);
    vec2 operator -(const vec2& vec)const;

    bool operator == (const vec2& vec)const;
    bool operator != (const vec2& vec)const;

    friend ostream& operator <<(ostream& out, const vec2& vect);
    
    double length() const;

    double x;
    double y;
};

struct vec3i{
    vec3i(int _x = 0, int _y = 0, int _z = 0);

    int x, y, z;
};

template <typename T> int sign(T val) {
    return (T(0) < val) - (val < T(0));
}

double GetDistance(const vec3& p1, const vec3& p2);
double GetDistance2(const vec3& p1, const vec3& p2);
vec3 GetMiddle(const vec3& p1, const vec3& p2);
vec3 GetDecart(const vec3& spherical);
vec3 GetSpherical(const vec3& decart);

int TestNeighbours(const vec3 p1[3], const vec3 p2[3]);
vec3 Normalize(vec3& v);
vec3 Normalized(vec3 v);
vec3 GetNormal(const vec3 _Points[3]);
//int randomI(int min,int max);
void GetRotMat(double angle, const vec3& axis, double mat[9]);
vec3 ApplyRotMat(const vec3& v, double mat[9]);
void MulVec4ByMat(double v[4], double mat[16], double res[4]);
void CombRotMat(const double mat1[9], const double mat2[9], double res[9]);
double* GetMatrixIJ(double* mat, int matOrder, int i, int j);
void CombMat(double* mat1, double* mat2, double* res, int matOrder);
double GetAngle(const vec3& v1, const vec3& v2);
double GetSign(const vec3& v1, const vec3& v2, const vec3& v3);
double GetSignedAngle(const vec3& v1, const vec3& v2, const vec3& up);
vec3 GetMiddle(int n, const vec3* points);
void Inversion(double* Mat, int N);
bool FindTriangleAndLineIntersection(
    vec3  v1, // вершины треугольника.
    vec3  v2,
    vec3  v3,
    vec3  n,  // нормаль треугольника.
    vec3  p1, // первый конец отрезка.
    vec3  p2, // второй конец отрезка.
    vec3& pc  // возвращаемая точка пересечения.
);