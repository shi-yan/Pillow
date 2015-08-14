#include "Vertex.h"

Vertex::Vertex(float p1,float p2,float p3,float n1,float n2,float n3)
    :SubdivideVertex(p1,p2,p3,n1,n2,n3),
      m_clone(0)
{
}

Vertex::Vertex(const Vector &pos, const Vector &nor)
    :SubdivideVertex(pos,nor),
      m_clone(0)
{
}

Vertex::VertexInfo Vertex::getVertexInfo()
{
    Vertex::VertexInfo result;
    result.index=m_index;
    result.x=m_position.x;
    result.y=m_position.y;
    result.z=m_position.z;
    result.nx=m_normal.x;
    result.ny=m_normal.y;
    result.nz=m_normal.z;
    result.adjacentCount=m_adjacentEdgeList.size();
    return result;
}

Vertex::~Vertex(void)
{
}
