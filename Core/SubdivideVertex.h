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
    SubdivideVertex(const Vector &pos, const Vector &nor = Vector());
    SubdivideVertex(float p1 = 0.0f, float p2 = 0.0f, float p3 = 0.0f, float n1 = 0.0f, float n2 = 0.0f, float n3 = 0.0f);
    int operator [](unsigned int i);

public:
    virtual ~SubdivideVertex(void)  override;
};
