#pragma once
#include<vector>
#include "ElementBase.h"
#include "SubdivideVertex.h"

struct VertexInfo
{
	size_t index;
	float x;
	float y;
	float z;
	float nx;
	float ny;
	float nz;
	size_t adjacentCount;
};

class Vertex :public SubdivideVertex
{
public:
	int clone;

public:
	Vertex(void);
	Vertex(float p1,float p2,float p3);
	Vertex(float p1,float p2,float p3,float n1,float n2,float n3);
	Vertex(Vector &pos);
	Vertex(Vector &pos,Vector &nor);

	struct VertexInfo getVertexInfo()
	{
		struct VertexInfo result;
		result.index=index;
		result.x=position.x;
		result.y=position.y;
		result.z=position.z;
		result.nx=normal.x;
		result.ny=normal.y;
		result.nz=normal.z;
		result.adjacentCount=adjacentEdge.size();
		return result;
	}

public:
	~Vertex(void);
};
