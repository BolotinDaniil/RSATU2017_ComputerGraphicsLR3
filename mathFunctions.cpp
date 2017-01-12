#include "mathFunctions.h"

/** vec3 **/

vec3::vec3(double _x, double _y, double _z):x(_x), y(_y), z(_z){
}

vec3::vec3(const vec3& vec):x(vec.x), y(vec.y), z(vec.z){
}

vec3& vec3::operator *= (double scolar){
    x *= scolar;
    y *= scolar;
    z *= scolar;
    return *this;
}

vec3 vec3::operator *(double scolar)const{
    vec3 newVec(x * scolar, y * scolar, z * scolar);
    return newVec;
}

vec3& vec3::operator += (const vec3& vec){
    x += vec.x;
    y += vec.y;
    z += vec.z;

    return (*this);
}

vec3 vec3::operator + (const vec3& vec)const{
    vec3 result(*this);

    return result += vec;
}

vec3& vec3::operator -= (const vec3& vec){
    x -= vec.x;
    y -= vec.y;
    z -= vec.z;

    return (*this);
}

vec3 vec3::operator - (const vec3& vec)const{
    vec3 result(*this);

    return result -= vec;
}

double vec3::operator & (const vec3& v)const{
    return (x * v.x + y * v.y + z * v.z);
}

vec3 vec3::operator ^ (const vec3& v)const{
    return vec3(
        y * v.z - v.y * z,
        z * v.x - v.z * x,
        x * v.y - v.x * y
    );
}

bool vec3::operator == (const vec3& vec)const{
    /*return (abs(vec.x - x) <= EPS)
        && (abs(vec.y - y) <= EPS)
        && (abs(vec.z - z) <= EPS);*/

    return (vec.x == x)
		&& (vec.y == y)
		&& (vec.z == z);
}

bool vec3::operator != (const vec3& vec)const{
    /*return (abs(vec.x - x) > EPS)
        && (abs(vec.y - y) > EPS)
        && (abs(vec.z - z) > EPS);*/

    return !((vec.x == x)
		&& (vec.y == y)
		&& (vec.z == z));
}

ostream& operator <<(ostream& out, const vec3& vect){
    out << "(" << vect.x << " : " << vect.y << " : " << vect.z << ")";
    return out;
}

double vec3::length() const{
	return sqrtf(x*x + y*y + z*z);
}


/** vec2 **/

vec2::vec2(double _x, double _y):x(_x), y(_y){
}

vec2::vec2(const vec2& vec):x(vec.x), y(vec.y){
}

vec2& vec2::operator *= (double scolar){
    x *= scolar;
    y *= scolar;
    return *this;
}

vec2 vec2::operator *(double scolar)const{
    vec2 newVec(x * scolar, y * scolar);
    return newVec;
}

vec2& vec2::operator += (const vec2& vec){
    x += vec.x;
    y += vec.y;

    return (*this);
}

vec2 vec2::operator + (const vec2& vec)const{
    vec2 result(*this);

    return result += vec;
}

vec2& vec2::operator -= (const vec2& vec){
    x -= vec.x;
    y -= vec.y;

    return (*this);
}

vec2 vec2::operator - (const vec2& vec)const{
    vec2 result(*this);

    return result -= vec;
}

bool vec2::operator == (const vec2& vec)const{
    return (vec.x == x)
		&& (vec.y == y);
}

bool vec2::operator != (const vec2& vec)const{
    return !(*this == vec);
}

ostream& operator <<(ostream& out, const vec2& vect){
    out << "(" << vect.x << " : " << vect.y << ")";
    return out;
}

double vec2::length() const{
	return sqrtf(x*x + y*y);
}

/** vec3i **/

vec3i::vec3i(int _x, int _y, int _z) : x(_x), y(_y), z(_z){
}

/** Math **/

double GetDistance(const vec3& p1, const vec3& p2){
    return sqrtf(pow(p1.x -p2.x, 2.0) +pow(p1.y -p2.y, 2.0) +pow(p1.z -p2.z, 2.0));
}

double GetDistance2(const vec3& p1, const vec3& p2){
    return (pow(p1.x -p2.x, 2.0) +pow(p1.y -p2.y, 2.0) +pow(p1.z -p2.z, 2.0));
}

vec3 GetMiddle(const vec3& p1, const vec3& p2){
    return vec3(
        p1.x +(p2.x -p1.x)/2.0,
        p1.y +(p2.y -p1.y)/2.0,
        p1.z +(p2.z -p1.z)/2.0
    );
}

vec3 GetDecart(const vec3& spherical){
    return vec3(
        spherical.x *sin(spherical.z) *cos(spherical.y),
        spherical.x *sin(spherical.z) *sin(spherical.y),
        spherical.x *cos(spherical.z)
    );
}

vec3 GetSpherical(const vec3& decart){
    double r = GetDistance(decart, vec3());
    /*
    if(fabs(decart.x) < 0.001 && fabs(decart.y) < 0.001){
        return vec3(r, 0.0, (sign(decart.z)>0)? 0.0 : M_PI);
    }
    //*/
    return vec3(
        r,
        //atan2(decart.y, decart.x),
        //acos(decart.z/r)
        atan2(decart.y, decart.x),
        atan2(hypot(decart.y, decart.x), decart.z)
    );
}

int TestNeighbours(const vec3 p1[3], const vec3 p2[3]){
    int samePointsCounter = 0;
    for(int i=0; i<3 && samePointsCounter < 2; i++){
        for(int j=0; j<3 && samePointsCounter < 2; j++){
            samePointsCounter += (p1[i] == p2[j]);
        }
    }
    return (samePointsCounter == 2)? 1 : 0;
}

vec3 Normalize(vec3& v){
    v *= 1.0/GetDistance(vec3(), v);
    return v;
}

vec3 Normalized(vec3 v){
    v *= 1.0/GetDistance(vec3(), v);
    return v;
}

vec3 GetNormal(const vec3 p[3]){
    vec3 b(p[2].x -p[0].x, p[2].y -p[0].y, p[2].z -p[0].z);
    vec3 a(p[1].x -p[0].x, p[1].y -p[0].y, p[1].z -p[0].z);
    vec3 normal(a.y*b.z -a.z*b.y, a.z*b.x -a.x*b.z, a.x*b.y -a.y*b.x);
    Normalize(normal); //normal *= 1.0/GetDistance(vec3(0, 0, 0), normal);
    return normal;
}

/*
int srand_started=0;
int randomI(int min,int max){
    if(!srand_started){
        srand(time(NULL));
        srand_started=1;
    }
    return rand()%(min-max)+min;
}
*/

void GetRotMat(double angle, const vec3& axis, double mat[9]){
    double _cos = cosf(angle);
    double _sin = sinf(angle);
    mat[0] = _cos +(1 -_cos)*axis.x*axis.x;
    mat[1] = (1 -_cos)*axis.x*axis.y -_sin*axis.z;
    mat[2] = (1 -_cos)*axis.x*axis.z +_sin*axis.y;
    mat[3] = (1 -_cos)*axis.y*axis.x +_sin*axis.z;
    mat[4] = _cos +(1 -_cos)*axis.y*axis.y;
    mat[5] = (1 -_cos)*axis.y*axis.z -_sin*axis.x;
    mat[6] = (1 -_cos)*axis.z*axis.x -_sin*axis.y;
    mat[7] = (1 -_cos)*axis.z*axis.y +_sin*axis.x;
    mat[8] = _cos +(1 -_cos)*axis.x*axis.x;
}

vec3 ApplyRotMat(const vec3& v, double mat[9]){
    return vec3(
        v.x * mat[0] + v.y * mat[3] + v.z * mat[6],// + mat[9],
        v.x * mat[1] + v.y * mat[4] + v.z * mat[7],// + mat[10],
        v.x * mat[2] + v.y * mat[5] + v.z * mat[8] // + mat[11]
    );
}

/*
 0  1  2  3)(0)
 4  5  6  7)(1)
 8  9 10 11)(2)
12 13 14 15)(3)
*/

void MulVec4ByMat(double v[4], double mat[16], double res[4]){
    double tmpRes[4] = {
        v[0] * mat[0] + v[1] * mat[1] + v[2] * mat[2] + v[3] * mat[3],
        v[0] * mat[4] + v[1] * mat[5] + v[2] * mat[6] + v[3] * mat[7],
        v[0] * mat[8] + v[1] * mat[9] + v[2] * mat[10] + v[3] * mat[11],
        v[0] * mat[12] + v[1] * mat[13] + v[2] * mat[14] + v[3] * mat[15]
    };
    memcpy(res, tmpRes, 4*sizeof(double));
}

/*
0 1 2
3 4 5
6 7 8
*/

void CombRotMat(const double mat1[9], const double mat2[9], double res[9]){
    res[0] = mat1[0]*mat2[0] +mat1[1]*mat2[3] +mat1[2]*mat2[6];
    res[1] = mat1[0]*mat2[1] +mat1[1]*mat2[4] +mat1[2]*mat2[7];
    res[2] = mat1[0]*mat2[2] +mat1[1]*mat2[5] +mat1[2]*mat2[8];

    res[3] = mat1[3]*mat2[0] +mat1[4]*mat2[3] +mat1[5]*mat2[6];
    res[4] = mat1[3]*mat2[1] +mat1[4]*mat2[4] +mat1[5]*mat2[7];
    res[5] = mat1[3]*mat2[2] +mat1[4]*mat2[5] +mat1[5]*mat2[8];

    res[6] = mat1[6]*mat2[0] +mat1[7]*mat2[3] +mat1[8]*mat2[6];
    res[7] = mat1[6]*mat2[1] +mat1[7]*mat2[4] +mat1[8]*mat2[7];
    res[8] = mat1[6]*mat2[2] +mat1[7]*mat2[5] +mat1[8]*mat2[8];
}

/*
 0  1  2  3
 4  5  6  7
 8  9 10 11
12 13 14 15
*/

double* GetMatrixIJ(double* mat, int matOrder, int i, int j){
    return (mat +(i*matOrder +j)*sizeof(double));
}

void CombMat(double* mat1, double* mat2, double* res, int matOrder){
    for(int i=0; i<matOrder*matOrder; i++){
        res[i] = 0;
    }
    for(int row = 0; row < matOrder; row++) {
        for(int col = 0; col < matOrder; col++) {
            // Multiply the row of A by the column of B to get the row, column of product.
            for(int inner = 0; inner < matOrder; inner++){
                *GetMatrixIJ(res, matOrder, row, col) += *GetMatrixIJ(mat1, matOrder, row, inner) **GetMatrixIJ(mat2, matOrder, inner, col);
            }
        }
    }
}

double GetAngle(const vec3& v1, const vec3& v2){
    double scalar = v1.x *v2.x +v1.y *v2.y +v1.z *v2.z;
    //double _cos = scalar/(GetDistance(vec3(0,0,0), v1) *GetDistance(vec3(0,0,0), v2));
    //return fabsf(atan2(sqrt(1 -_cos*_cos), _cos));
    return acosf(scalar/(GetDistance(vec3(), v1) *GetDistance(vec3(), v2)));
}

double GetSign(const vec3& v1, const vec3& v2, const vec3& v3){
    double det1 = v1.x*v2.y*v3.z - v1.x*v2.z*v3.y;
    double det2 = -v1.y*v2.x*v3.z + v1.y*v2.z*v3.x;
    double det3 = v1.z*v2.x*v3.y - v1.z*v2.y*v3.x;
    
    return (det1 + det2 + det3 > 0.0)? 1.0 : -1.0;
}

double GetSignedAngle(const vec3& v1, const vec3& v2, const vec3& up){
    return GetAngle(v1, v2) * GetSign(v1, v2, up);
}

vec3 GetMiddle(int n, const vec3* points){
    vec3 res;
    for(int i = 0; i < n; i++){
        res += points[i];
    }
    res *= 1.0/(double)n;
    return res;
}

void Inversion(double* Mat, int N){
    double temp;
    double **E = new double*[N];

    for(int i = 0; i < N; i++){
        E[i] = new double[N];
    }
    for(int i = 0; i < N; i++){
        for(int j = 0; j < N; j++){
            E[i][j] = 0.0;
            if(i == j){
                E[i][j] = 1.0;
            }
        }
    }

    for(int k = 0; k < N; k++){
        temp = Mat[k*N +k];
        for(int j = 0; j < N; j++){
            Mat[k*N +j] /= temp;
            E[k][j] /= temp;
        }

        for(int i = k + 1; i < N; i++){
            temp = Mat[i*N +k];
            for(int j = 0; j < N; j++){
                Mat[i*N +j] -= Mat[k*N +j] * temp;
                E[i][j] -= E[k][j] * temp;
            }
        }
    }

    for(int k = N - 1; k > 0; k--){
        for(int i = k - 1; i >= 0; i--){
            temp = Mat[i*N +k];
            for (int j = 0; j < N; j++){
                Mat[i*N +j] -= Mat[k*N +j] * temp;
                E[i][j] -= E[k][j] * temp;
            }
        }
    }

    for(int i = 0; i < N; i++){
        for(int j = 0; j < N; j++){
            Mat[i*N +j] = E[i][j];
        }
    }

    for(int i = 0; i < N; i++){
        delete [] E[i];
    }
    delete [] E;
}

bool FindTriangleAndLineIntersection(
    vec3  v1, // ������� ������������.
    vec3  v2,
    vec3  v3,
    vec3  n,  // ������� ������������.
    vec3  p1, // ������ ����� �������.
    vec3  p2, // ������ ����� �������.
    vec3& pc  // ������������ ����� �����������.
){
    // ��������� ���������� ����� ������� ������� � ���������� ������������.
    double r1 = n & vec3(p1.x - v1.x, p1.y - v1.y, p1.z - v1.z);
    double r2 = n & vec3(p2.x - v1.x, p2.y - v1.y, p2.z - v1.z);
    // ���� ��� ����� ������� ����� �� ���� ������� �� ���������, �� �������
    // �� ���������� �����������.
    if( sign(r1) == sign(r2) ){
        return false;
    }
    // ��������� ����� ����������� ������� � ���������� ������������.
    vec3 ip = vec3(
        p1.x + ((p2.x - p1.x) * (-r1 / (r2 - r1))),
        p1.y + ((p2.y - p1.y) * (-r1 / (r2 - r1))),
        p1.z + ((p2.z - p1.z) * (-r1 / (r2 - r1)))
    );

    // ���������, ��������� �� ����� ����������� ������ ������������.
    vec3 t1(v2.x - v1.x, v2.y - v1.y, v2.z - v1.z);
    vec3 t2(ip.x - v1.x, ip.y - v1.y, ip.z - v1.z);
    if( ((t1 ^ t2) & n) <= 0){
        return false;
    }
    t1 = vec3(v3.x - v2.x, v3.y - v2.y, v3.z - v2.z);
    t2 = vec3(ip.x - v2.x, ip.y - v2.y, ip.z - v2.z);
    if( ((t1 ^ t2) & n) <= 0){
        return false;
    }
    t1 = vec3(v1.x - v3.x, v1.y - v3.y, v1.z - v3.z);
    t2 = vec3(ip.x - v3.x, ip.y - v3.y, ip.z - v3.z);
    if( ((t1 ^ t2) & n) <= 0){
        return false;
    }
    pc = ip;
    return true;
}