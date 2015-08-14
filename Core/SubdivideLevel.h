#pragma once
#include "IndexArray.h"
#include "SubdivideVertex.h"
#include "SubdivideEdge.h"
#include "SubdivideFace.h"

class SubdivideLevel
{
public:
	IndexArray<SubdivideVertex> vertex;
	IndexArray<SubdivideEdge> edge;
	IndexArray<SubdivideFace> face;
public:
	SubdivideLevel(void);
    SubdivideLevel(unsigned int v,unsigned int e,unsigned int f)
	{
		vertex.reserve(v);
		edge.reserve(e);
		face.reserve(f);
    }
public:
	~SubdivideLevel(void)
	{
    }
};
