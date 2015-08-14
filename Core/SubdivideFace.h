#pragma once
#include "ElementBase.h"
#include "SubdivideVertex.h"

class SubdivideFace : public ElementBase
{
	friend class Object;
private:
    int m_edge[4];
    int m_center;
    int m_subFace[4];
    Vector m_normal;

public:
	SubdivideFace(int v1,int v2,int v3,int v4);

public:
    ~SubdivideFace(void) override;
};
