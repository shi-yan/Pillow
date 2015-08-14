#pragma once
#include "IndexArray.h"
#include "SubdivideVertex.h"
#include "SubdivideEdge.h"
#include "SubdivideFace.h"

class SubdivideLevel
{
public:
    IndexArray<SubdivideVertex> m_vertex;
    IndexArray<SubdivideEdge> m_edge;
    IndexArray<SubdivideFace> m_face;

    SubdivideLevel(unsigned int v = 0,unsigned int e = 0,unsigned int f = 0);

public:
	~SubdivideLevel(void)
	{
    }
};
