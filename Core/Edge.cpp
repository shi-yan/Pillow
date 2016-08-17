#include "Edge.h"

Edge::Edge(int start, int end)
    :SubdivideEdge(start, end),
      m_clone(0)
{
}

Edge::EdgeInfo Edge::getEdgeInfo() const
{
    EdgeInfo result;
    result.index = m_index;
    result.start = m_start;
    result.end = m_end;
    result.left = m_left;
    result.right = m_right;
    return result;
}

Edge::~Edge(void)
{
}
