#include "SubdivideEdge.h"

SubdivideEdge::SubdivideEdge(void):ElementBase(),start(0),end(0),left(0),right(0),subdivideId(0),subdivideStep(0),middle(0),startEdge(0),endEdge(0),rightEdge(0),leftEdge(0)
{
}

SubdivideEdge::SubdivideEdge(unsigned int theStart,unsigned int theEnd):ElementBase(),start(theStart),end(theEnd),left(0),right(0),subdivideId(0),subdivideStep(0),middle(0),startEdge(0),endEdge(0),rightEdge(0),leftEdge(0)
{
}

SubdivideEdge::~SubdivideEdge(void)
{
}
