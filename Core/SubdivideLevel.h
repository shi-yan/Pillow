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
	SubdivideLevel(size_t v,size_t e,size_t f)
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
