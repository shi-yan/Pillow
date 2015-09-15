#pragma once

#include <memory.h>
#include <math.h>

//! random floating point number
#define psFRAND (((rand()%10000)-5000)/5000.0f)
//! absolute random floating point number
#define psFABSRAND ((rand()%10000)/10000.0f)

//! two vectors dot product
#define psVECDOT(v1,v2) ((v1).x*(v2).x+(v1).y*(v2).y+(v1).z*(v2).z)

//! lower bound for overflow detection
#define psSMALL 1.0e-4f
//! upper bound for overflow detection
#define psBIG   1.0e+10f
//! underflow detection
#define psISZERO(a) ((a)>-FLY_SMALL && (a)<FLY_SMALL)

//! represents the value of 2*pi
#define ps2PI                6.28318530718f
//! represents the value of pi
#define psPI                3.14159265359f
//! represents the value of pi/2
#define psPI2                1.57079632679f
//! represents the value of pi/180
#define psPIOVER180        1.74532925199433E-002f
//! represents the value of 180/pi
#define psPIUNDER180        5.72957795130823E+001f
//! represents cos(45)
#define psCOS45            0.7071067811865475244f
//! represents the square root of 3
#define psSQRT3            1.7320508075688772935f

//! returns the bitwise representation of a floating point number
#define psFPBITS(fp)        (*(int *)&(fp))
//! returns the absolute value of a floating point number in bitwise form
#define psFPABSBITS(fp)    (FLY_FPBITS(fp)&0x7FFFFFFF)
//! returns the signal bit of a floating point number
#define psFPSIGNBIT(fp)    (FLY_FPBITS(fp)&0x80000000)
//! returns the absolute value of a floating point number
#define psFPABS(fp)        (*((int *)&fp)=FLY_FPABSBITS(fp))
//! returns the value of 1.0f in bitwise form
#define psFPONEBITS        0x3F800000
