#pragma once
#include<vector>
#include "ElementBase.h"
#include "SubdivideVertex.h"

class Vertex :public SubdivideVertex
{
public:
    typedef struct
    {
        size_t index;
        float x;
        float y;
        float z;
        float nx;
        float ny;
        float nz;
        size_t adjacentCount;
    } VertexInfo;

    int m_clone;

public:
	Vertex(void);
	Vertex(float p1,float p2,float p3);
	Vertex(float p1,float p2,float p3,float n1,float n2,float n3);
    Vertex(const Vector &pos);
    Vertex(const Vector &pos, const Vector &nor);

    VertexInfo getVertexInfo() const
	{
        VertexInfo result;
		result.index=index;
        result.x=m_position.x;
        result.y=m_position.y;
        result.z=m_position.z;
        result.nx=m_normal.x;
        result.ny=m_normal.y;
        result.nz=m_normal.z;
        result.adjacentCount=m_adjacentEdgeList.size();
		return result;
	}

public:
	~Vertex(void);
};
