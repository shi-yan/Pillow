#pragma once
#include "ElementBase.h"
#include "Vector.h"
#include <vector>

class SubdivideVertex : public ElementBase
{
friend class Object;
public:
    Vector m_position;
    Vector m_normal;
    std::vector<int> m_adjacentEdgeList;
	
private:
    int m_nextLevel;
    int m_subdivideId;
    int m_subdivideStep;
    bool m_edgeVertex;

public:
	SubdivideVertex(void);
    SubdivideVertex(const Vector &pos, const Vector &nor);
    SubdivideVertex(const Vector &pos);
	SubdivideVertex(float p1,float p2,float p3);
	SubdivideVertex(float p1,float p2,float p3,float n1,float n2,float n3);
	const int& operator [](size_t i);

public:
	~SubdivideVertex(void);
};
