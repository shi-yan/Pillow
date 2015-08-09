#include "Vertex.h"

Vertex::Vertex(void):SubdivideVertex(),clone(0)
{
}

Vertex::Vertex(float p1,float p2,float p3):SubdivideVertex(p1,p2,p3),clone(0)
{
}

Vertex::Vertex(float p1,float p2,float p3,float n1,float n2,float n3):SubdivideVertex(p1,p2,p3,n1,n2,n3),clone(0)
{
}

Vertex::Vertex(Vector &pos):SubdivideVertex(pos),clone(0)
{
}

Vertex::Vertex(Vector &pos,Vector &nor):SubdivideVertex(pos,nor),clone(0)
{
}

Vertex::~Vertex(void)
{
}
