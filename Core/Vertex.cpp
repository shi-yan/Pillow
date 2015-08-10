#include "Vertex.h"

Vertex::Vertex(void)
    :SubdivideVertex(),
      m_clone(0)
{
}

Vertex::Vertex(float p1,float p2,float p3)
    :SubdivideVertex(p1,p2,p3),
      m_clone(0)
{
}

Vertex::Vertex(float p1,float p2,float p3,float n1,float n2,float n3)
    :SubdivideVertex(p1,p2,p3,n1,n2,n3),
      m_clone(0)
{
}

Vertex::Vertex(const Vector &pos)
    :SubdivideVertex(pos),
      m_clone(0)
{
}

Vertex::Vertex(const Vector &pos, const Vector &nor)
    :SubdivideVertex(pos,nor),
      m_clone(0)
{
}

Vertex::~Vertex(void)
{
}
