#pragma once
#include "Math.h"
#include "Vector.h"

class Matrix
{
    public:
        float m[4][4]; //!< matrix elements

    //! Default constructor, sets the identity matrix
    Matrix()
    {
        m[0][0]=m[1][1]=m[2][2]=m[3][3]=1.0f;
        m[0][1]=m[0][2]=m[0][3]=0.0f;
        m[1][0]=m[1][2]=m[1][3]=0.0f;
        m[2][0]=m[2][1]=m[2][3]=0.0f;
        m[3][0]=m[3][1]=m[3][2]=0.0f;
    }

    //! Copy-constructor
    Matrix(const Matrix& in)
    {
        m[0][0]=in.m[0][0]; m[0][1]=in.m[0][1]; m[0][2]=in.m[0][2]; m[0][3]=in.m[0][3];
        m[1][0]=in.m[1][0]; m[1][1]=in.m[1][1]; m[1][2]=in.m[1][2]; m[1][3]=in.m[1][3];
        m[2][0]=in.m[2][0]; m[2][1]=in.m[2][1]; m[2][2]=in.m[2][2]; m[2][3]=in.m[2][3];
        m[3][0]=in.m[3][0]; m[3][1]=in.m[3][1]; m[3][2]=in.m[3][2]; m[3][3]=in.m[3][3];
    }

    //! Atribuition operator
    void operator=(const Matrix& in)
    {
        m[0][0]=in.m[0][0]; m[0][1]=in.m[0][1]; m[0][2]=in.m[0][2]; m[0][3]=in.m[0][3];
        m[1][0]=in.m[1][0]; m[1][1]=in.m[1][1]; m[1][2]=in.m[1][2]; m[1][3]=in.m[1][3];
        m[2][0]=in.m[2][0]; m[2][1]=in.m[2][1]; m[2][2]=in.m[2][2]; m[2][3]=in.m[2][3];
        m[3][0]=in.m[3][0]; m[3][1]=in.m[3][1]; m[3][2]=in.m[3][2]; m[3][3]=in.m[3][3];
    }

    //! Nullify all elements
    inline void null(void)
    {
        m[0][0]=m[0][1]=m[0][2]=m[0][3]=
        m[1][0]=m[1][1]=m[1][2]=m[1][3]=
        m[2][0]=m[2][1]=m[2][2]=m[2][3]=
        m[3][0]=m[3][1]=m[3][2]=m[3][3]=0.0f;
    }

    //! Load the identity matrix
    inline void load_identity(void)
    {
        m[0][0]=m[1][1]=m[2][2]=m[3][3]=1.0f;
        m[0][1]=m[0][2]=m[0][3]=0.0f;
        m[1][0]=m[1][2]=m[1][3]=0.0f;
        m[2][0]=m[2][1]=m[2][3]=0.0f;
        m[3][0]=m[3][1]=m[3][2]=0.0f;
    }

    //! Set the matrix as the rotation matrix of angle given by 'ang' around direction 'dir'
    void setRotation( float ang, const Vector& dir );
    //! Multiply the matrix by another with rotation given by 'ang' around direction 'dir'
    void rotate( float ang, const Vector& dir );
    //! Interpolate between two rotation matrices using quaternions
    void lerp(const Matrix& m1, const Matrix& m2,float t);

    //! Multiplication operator
    inline Matrix operator*(const Matrix& m1) const
    {
      Matrix m2;
      int i,j;
      for(i=0; i<4; i++)
        for(j=0; j<4; j++)
        {
            m2.m[i][j] = m[i][0]*m1.m[0][j] + m[i][1]*m1.m[1][j] +
                m[i][2]*m1.m[2][j] + m[i][3]*m1.m[3][j];
        }
      return m2;
    }
};

inline Vector operator*(const Vector& v,const Matrix& m)
{
    Vector r;
    register float *f=(float *)&m;
    r.x = v.x*f[0] + v.y*f[4] + v.z*f[8] + v.w*f[12];
    r.y = v.x*f[1] + v.y*f[5] + v.z*f[9] + v.w*f[13];
    r.z = v.x*f[2] + v.y*f[6] + v.z*f[10] + v.w*f[14];
    r.w = v.x*f[3] + v.y*f[7] + v.z*f[11] + v.w*f[15];
    return r;
}
//! Multiplies a matrix by a vector 
inline Vector operator*(const Matrix& m, const Vector& v)
{
    Vector r;
    register float *f=(float *)&m;
    r.x = v.x*f[0] + v.y*f[4] + v.z*f[8] + v.w*f[12];
    r.y = v.x*f[1] + v.y*f[5] + v.z*f[9] + v.w*f[13];
    r.z = v.x*f[2] + v.y*f[6] + v.z*f[10] + v.w*f[14];
    r.w = v.x*f[3] + v.y*f[7] + v.z*f[11] + v.w*f[15];
    return r;
}
