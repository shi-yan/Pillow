#include "Quaternion.h"
#include "TheMath.h"

Quaternion::Quaternion(const Matrix &mat)
{
	float tr,s,q[4];
	int i,j,k;

	int nxt[3] = {1, 2, 0};

	tr = mat.m[0][0] + mat.m[1][1] + mat.m[2][2];

	// check the diagonal
	if (tr > 0.0) 
	{
		s = sqrtf(tr + 1.0f);
		w = s/2.0f;
		s = 0.5f/s;
		x = (mat.m[1][2] - mat.m[2][1]) * s;
		y = (mat.m[2][0] - mat.m[0][2]) * s;
		z = (mat.m[0][1] - mat.m[1][0]) * s;
	} 
	else 
	{		
		// diagonal is negative
    	i = 0;
		if (mat.m[1][1] > mat.m[0][0]) i = 1;
		if (mat.m[2][2] > mat.m[i][i]) i = 2;
		j = nxt[i];
		k = nxt[j];

		s=sqrtf((mat.m[i][i]-(mat.m[j][j] + mat.m[k][k])) + 1.0f);
      
		q[i]=s*0.5f;
            
		if(s!=0.0f) s = 0.5f/s;

		q[3] = (mat.m[j][k] - mat.m[k][j]) * s;
		q[j] = (mat.m[i][j] + mat.m[j][i]) * s;
		q[k] = (mat.m[i][k] + mat.m[k][i]) * s;

		x = q[0];
		y = q[1];
		z = q[2];
		w = q[3];
	}
}

void Quaternion::getMatrix(Matrix &mat) const 
{
	float wx, wy, wz, xx, yy, yz, xy, xz, zz, x2, y2, z2;

	// calculate coefficients
	x2 = x + x; 
	y2 = y + y; 
	z2 = z + z;
	xx = x * x2;
	xy = x * y2;   
	xz = x * z2;
	yy = y * y2;   
	yz = y * z2;   
	zz = z * z2;
	wx = w * x2;   
	wy = w * y2;   
	wz = w * z2;

	mat.m[0][0] = 1.0f-(yy+zz); 	
	mat.m[1][0] = xy - wz;
	mat.m[2][0] = xz + wy;		
	mat.m[3][0] = 0.0;

	mat.m[0][1] = xy + wz;		
	mat.m[1][1] = 1.0f-(xx + zz);
	mat.m[2][1] = yz - wx;		
	mat.m[3][1] = 0.0;

	mat.m[0][2] = xz - wy;		
	mat.m[1][2] = yz + wx;
	mat.m[2][2] = 1.0f - (xx + yy);		
	mat.m[3][2] = 0.0;

	mat.m[0][3] = 0;			
	mat.m[1][3] = 0;
	mat.m[2][3] = 0;			
	mat.m[3][3] = 1;
}

Quaternion Quaternion::operator +(const Quaternion& q)
{
	Quaternion res;
	res.x=x+q.x;
	res.y=y+q.y;
	res.z=z+q.z;
	res.w=w+q.w;
	return res;
}

Quaternion Quaternion::operator*(const Quaternion &q)
{
	Quaternion result;

	result.w = w * q.w - x * q.x - y * q.y - z * q.z;
	result.x = w * q.x + x * q.w + y * q.z - z * q.y;
	result.y = w * q.y + y * q.w + z * q.x - x * q.z;
	result.z = w * q.z + z * q.w + x * q.y - y * q.x;

	return result;
}

void Quaternion::normalize()
{
	float factor = 1.0f/sqrtf(x*x+y*y+z*z+w*w);

	x*=factor;
	y*=factor;
	z*=factor;
	w*=factor;
}

void Quaternion::getRotate(float &angle, Vector &axis) const
{
	angle=acosf(w)*2*psPIUNDER180;

	float f=sinf(angle*psPIOVER180*0.5f);

	axis.x=x/f;
	axis.y=y/f;
	axis.z=z/f;
}

void Quaternion::lerp(const Quaternion& q1, const Quaternion& q2,float t)
{
    float v;        // complement to t
    float o;        // complement to v (t)
    float theta;    // angle between q1 & q2
    float sin_t;    // sin(theta)
    float cos_t;    // cos(theta)
    float phi;      // spins added to theta
    int flip;       // flag for negating q2

    cos_t = q1.x * q2.x + q1.y * q2.y + q1.z * q2.z + q1.w * q2.w;

    if (cos_t < 0.0)
    {
		cos_t = -cos_t;
		flip = 1;
    }
    else 
		flip = 0;

    if (1.0 - cos_t < 1e-6)
    {
        v = 1.0f - t;
		o = t;
    }
    else
    {
        theta = acosf(cos_t);
        phi = theta;
        sin_t = sinf(theta);
        v = sinf(theta - t * phi) / sin_t;
        o = sinf(t * phi) / sin_t;
    }
    if (flip) o = -o;

    x = v * q1.x + o * q2.x;
    y = v * q1.y + o * q2.y;
    z = v * q1.z + o * q2.z;
    w = v * q1.w + o * q2.w;
}
