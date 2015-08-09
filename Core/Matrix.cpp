#pragma once
#include "Matrix.h"
#include "Quaternion.h"

void Matrix::lerp(const Matrix& m1, const Matrix& m2,float t)
{
	Quaternion q1(m1),q2(m2),qt;
	qt.lerp(q1,q2,t);
	qt.getMatrix(*this);
	m[3][0]=m1.m[3][0]*(1-t)+m2.m[3][0]*t;
	m[3][1]=m1.m[3][1]*(1-t)+m2.m[3][1]*t;
	m[3][2]=m1.m[3][2]*(1-t)+m2.m[3][2]*t;
}

void Matrix::rotate( float ang, const Vector& dir )
{
	static Matrix m;
	m.setRotation(ang,dir);
	*this=*this*m;
}

void Matrix::setRotation( float ang, const Vector& dir2 )
{
	Vector dir(dir2);
	dir.normalize();

	ang*=-psPIOVER180;

	float fCos=cosf( ang );
	float fSin=sinf( ang );

    m[0][0] = ( dir.x * dir.x ) * ( 1.0f - fCos ) + fCos;
    m[0][1] = ( dir.x * dir.y ) * ( 1.0f - fCos ) - (dir.z * fSin);
    m[0][2] = ( dir.x * dir.z ) * ( 1.0f - fCos ) + (dir.y * fSin);

    m[1][0] = ( dir.y * dir.x ) * ( 1.0f - fCos ) + (dir.z * fSin);
    m[1][1] = ( dir.y * dir.y ) * ( 1.0f - fCos ) + fCos ;
    m[1][2] = ( dir.y * dir.z ) * ( 1.0f - fCos ) - (dir.x * fSin);

    m[2][0] = ( dir.z * dir.x ) * ( 1.0f - fCos ) - (dir.y * fSin);
    m[2][1] = ( dir.z * dir.y ) * ( 1.0f - fCos ) + (dir.x * fSin);
    m[2][2] = ( dir.z * dir.z ) * ( 1.0f - fCos ) + fCos;
    
    m[0][3] = m[1][3] = m[2][3] = 0;
    m[3][0] = m[3][1] = m[3][2] = 0;
    m[3][3] = 1;
}

