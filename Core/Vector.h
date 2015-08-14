#pragma once
#include "TheMath.h"
#include <math.h>

class Vector
{
	public:
		float	x,	//!< the first component
				y,	//!< the second component
				z,	//!< the third component
				w;	//!< the fourth (of optional usage) component

	//! Default constructor
	Vector() : x(0), y(0), z(0), w(1)
	{ }

	//! Constructor from a floating point value, sets x, y, z and w to this value
	Vector(float f) 
	{ x=y=z=w=f; }

	//! Constructor from two floating point values, sets x, y, z first float and w to second
	Vector(float f1,float f2) 
	{ x=y=z=f1; w=f2; }

	//! Copy-constructor
	Vector(const Vector& v) : x(v.x), y(v.y), z(v.z), w(v.w) 
	{ }

	//! Constructor from 3 floating point numbers, sets x, y and z to their respective values, and w to 1.0f
	Vector(float x0,float y0,float z0)
    { x=x0; y=y0; z=z0; w=1; }

	//! Constructor from 4 floating point numbers
	Vector(float x0,float y0,float z0,float w0)
    { x=x0; y=y0; z=z0; w=w0; }

	//! Atribuition operator
	void operator=(const Vector& in) 
	{ x = in.x; y = in.y; z = in.z; w = in.w; }

	/* void operator+=(const Vector& in)
     {
          x += in.x;
          y += in.y;
		  z += in.z;
		  w+= in.w;
     }*/

	//! Nullifying function, sets x, y and z to zero, and w to 1.0f
	inline void null(void)
    { x=y=z=0; w=1; }

	//! Returns the length of the vector
	inline float length(void)
    { return sqrtf(x*x+y*y+z*z); }

	//! Returns the square of the length
	inline float length2(void)
    { return x*x+y*y+z*z; }

	//!	Returns the distance to another flyVector
	inline float distto(const Vector& v) const
    { return sqrtf((v.x-x)*(v.x-x)+(v.y-y)*(v.y-y)+(v.z-z)*(v.z-z)); }

	//!	Returns the square of the distance to another flyVector
	inline float distto2(const Vector& v) const
    { return (v.x-x)*(v.x-x)+(v.y-y)*(v.y-y)+(v.z-z)*(v.z-z); }

	//!	Set all the components to the given floating point value
	inline void vec(float f)
    { x=y=z=w=f; }

	//!	Set components to the given floating point values
	inline void vec(float x0,float y0,float z0)
    { x=x0; y=y0; z=z0; }

	//!	Set components to the given floating point values
	inline void vec(float x0,float y0,float z0,float w0)
    { x=x0; y=y0; z=z0; w=w0; }

	//!	Negate the first 3 components
	inline void negate(void)
    { x=-x; y=-y; z=-z; }

	//!	Compute the cross-product of two given flyVectors
	inline void cross(const Vector& v1, const Vector& v2)
	{
		x=v1.y*v2.z-v1.z*v2.y;
		y=v1.z*v2.x-v1.x*v2.z;
		z=v1.x*v2.y-v1.y*v2.x;
	}

	//!	Normalize the vector
	inline void normalize(void)
	{
		float len=sqrtf(x*x+y*y+z*z);
		if (psFPBITS(len)==0) return;
		len=1.0f/len;
		x*=len; y*=len; z*=len;
	}

	//!	Reference indexing operator
    inline float& operator[](int i) { return (&x)[i]; }
	//!	Indexing operator
	inline float operator[](int i) const { return (&x)[i]; }

	//!	Negates the vector (unary operator -)
	inline Vector operator-() const { return Vector(-x,-y,-z); }
};


//! Multiplies a vector by a floating point value
inline void operator*=(Vector& v,float f)
{
  v.x*=f; v.y*=f; v.z*=f;
}
//! Divides a vector by a floating point value
inline void operator/=(Vector& v,float f)
{
  v.x/=f; v.y/=f; v.z/=f;
}
//! Subtracts vector 'v2' from the original vector
inline void operator-=(Vector& v1,const Vector& v2)
{
  v1.x-=v2.x; v1.y-=v2.y; v1.z-=v2.z;
}
//! Adds the original vector with another
inline void operator+=(Vector& v1, const Vector& v2)
{
  v1.x+=v2.x; v1.y+=v2.y; v1.z+=v2.z;
}
//! Multiplies the original vector with another
inline void operator*=(Vector& v1, const Vector& v2)
{
  v1.x*=v2.x; v1.y*=v2.y; v1.z*=v2.z;
}
//! Divides the original vector by another
inline void operator/=(Vector& v1, const Vector& v2)
{
  v1.x/=v2.x; v1.y/=v2.y; v1.z/=v2.z;
}

//! Subtracts vector 'v2' from 'v1'
inline Vector operator-(Vector v1, const Vector& v2)
{
   v1.x-=v2.x; v1.y-=v2.y; v1.z-=v2.z;
   return v1;
}

inline Vector perpendicular(const Vector& v1, const Vector& v2)
{
	Vector result(0);
	result.x=v1.z*v2.y-v1.y*v2.z;
	result.y=v1.x*v2.z-v2.x*v1.z;
	result.z=v2.x*v1.y-v1.x*v2.y;
	return result;
}

//! Adds two vectors
inline Vector operator+(Vector v1, const Vector& v2)
{
   v1.x+=v2.x; v1.y+=v2.y; v1.z+=v2.z;
   return v1;
}
//! Multiplication between vectors
inline Vector operator*(Vector v1, const Vector& v2)
{
   v1.x*=v2.x; v1.y*=v2.y; v1.z*=v2.z;
   return v1;
}
//! Divides vector 'v1' by vector 'v2'
inline Vector operator/(Vector v1, const Vector& v2)
{
   v1.x/=v2.x; v1.y/=v2.y; v1.z/=v2.z;
   return v1;
}
//! Multiply a floating point value by a vector
inline Vector operator*(float f,Vector v)
{
   v.x*=f; v.y*=f; v.z*=f;
   return v;
}
//! Multiply a vector by a floating point value
inline Vector operator*(Vector v,float f)
{
   v.x*=f; v.y*=f; v.z*=f;
   return v;
}
//! Divide a vector by a floating point value
inline Vector operator/(Vector v,float f)
{
   v.x/=f; v.y/=f; v.z/=f;
   return v;
}
