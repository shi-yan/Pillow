#include "SubdivideLevel.h"

SubdivideLevel::SubdivideLevel(unsigned int v,unsigned int e,unsigned int f)
    :m_vertex(),
      m_edge(),
      m_face()
{
    m_vertex.reserve(v);
    m_edge.reserve(e);
    m_face.reserve(f);
}
