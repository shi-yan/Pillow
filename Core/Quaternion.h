#pragma once
#include "TheMath.h"
#include "Vector.h"
#include "Matrix.h"

class Quaternion : public Vector
{
public:
	//! Copy-constructor
	Quaternion(const Quaternion& in) : Vector(in) 
	{ };

	//! Default constructor
	Quaternion() : Vector() 
	{ };
Quaternion(float initial) : Vector(initial) 
	{ };
	//! Construct the quaternion from a matrix
	Quaternion(const Matrix &mat);

	//! Construct the quaternion from the angle and axis
	Quaternion(float angle, const Vector &axis)
	{
		float f=sinf(angle*psPIOVER180*0.5f);
		x=axis.x*f;
		y=axis.y*f;
		z=axis.z*f;
		w=cosf(angle*psPIOVER180*0.5f);
	}

	//! Normalize the quaternion
	void normalize();
	//! Converts the quaternion into a matrix
	void getMatrix(Matrix &) const;
	//! Get the rotation angle and axis
	void getRotate(float &angle, Vector &axis) const;
	//! Interpolate two quaternions
	void lerp(const Quaternion& q1,const Quaternion& q2,float t);

	//! Multiplication between quaternions
	Quaternion operator *(const Quaternion& q);
	//! Addition between quaternions
	Quaternion operator +(const Quaternion& q);
};
