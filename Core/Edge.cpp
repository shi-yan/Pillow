#include "Edge.h"

Edge::Edge(void):SubdivideEdge(),clone(0)
{
}

Edge::Edge(int theStart,int theEnd):SubdivideEdge(theStart,theEnd),clone(0)
{
}

struct EdgeInfo Edge::getEdgeInfo()
{
	struct EdgeInfo result;
	result.index=index;
	result.start=start;
	result.end=end;
	result.left=left;
	result.right=right;
	return result;
}

Edge::~Edge(void)
{
}