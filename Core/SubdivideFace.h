#pragma once
#include "ElementBase.h"
#include "SubdivideVertex.h"

class SubdivideFace:public ElementBase
{
	friend class Object;
private:
	int edge[4];
	int center;
	int subFace[4];
	Vector normal;

public:
	SubdivideFace(int v1,int v2,int v3,int v4);

public:
	~SubdivideFace(void);
};
