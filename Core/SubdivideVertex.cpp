#include "SubdivideVertex.h"

SubdivideVertex::SubdivideVertex(const Vector &pos, const Vector &nor)
    :m_position(pos),
      m_normal(nor),
      m_edgeVertex(false),
      m_subdivideId(0),
      m_subdivideStep(0),
      m_nextLevel(0)
{
    m_adjacentEdgeList.reserve(5);
}

SubdivideVertex::SubdivideVertex(float p1,float p2,float p3,float n1,float n2,float n3)
    :m_position(p1,p2,p3),
      m_normal(n1,n2,n3),
      m_edgeVertex(false),
      m_subdivideId(0),
      m_subdivideStep(0),
      m_nextLevel(0)
{
    m_adjacentEdgeList.reserve(5);
}

inline int SubdivideVertex::operator [](unsigned int i)
{
    return m_adjacentEdgeList[i];
}

SubdivideVertex::~SubdivideVertex(void)
{
}
