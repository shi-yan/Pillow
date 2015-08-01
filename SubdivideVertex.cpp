#include "SubdivideVertex.h"

SubdivideVertex::SubdivideVertex(void):ElementBase(),position(0),normal(0),edgeVertex(false),subdivideId(0),subdivideStep(0),nextLevel(0)
{
	adjacentEdge.reserve(5);
}

SubdivideVertex::SubdivideVertex(Vector &pos,Vector &nor):position(pos),normal(nor),edgeVertex(false),subdivideId(0),subdivideStep(0),nextLevel(0)
{
	adjacentEdge.reserve(5);
}

SubdivideVertex::SubdivideVertex(Vector &pos):position(pos),normal(0),edgeVertex(false),subdivideId(0),subdivideStep(0),nextLevel(0)
{
	adjacentEdge.reserve(5);
}

SubdivideVertex::SubdivideVertex(float p1,float p2,float p3):position(p1,p2,p3),normal(0),edgeVertex(false),subdivideId(0),subdivideStep(0),nextLevel(0)
{
	adjacentEdge.reserve(5);
}

SubdivideVertex::SubdivideVertex(float p1,float p2,float p3,float n1,float n2,float n3):position(p1,p2,p3),normal(n1,n2,n3),edgeVertex(false),subdivideId(0),subdivideStep(0),nextLevel(0)
{
	adjacentEdge.reserve(5);
}

inline const int& SubdivideVertex::operator [](size_t i)
{
	return adjacentEdge[i];
}

SubdivideVertex::~SubdivideVertex(void)
{
}