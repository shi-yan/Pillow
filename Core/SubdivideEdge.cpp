#include "SubdivideEdge.h"

SubdivideEdge::SubdivideEdge(unsigned int start, unsigned int end)
    :ElementBase(),
      m_start(start),
      m_end(end),
      m_left(0),
      m_right(0),
      m_subdivideId(0),
      m_subdivideStep(0),
      m_middle(0),
      m_startEdge(0),
      m_endEdge(0),
      m_rightEdge(0),
      m_leftEdge(0)
{
}

SubdivideEdge::~SubdivideEdge(void)
{
}
