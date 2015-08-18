#include "Face.h"

Face::Face(void)
    :ElementBase(),
      m_normal(0),
      m_center(0)
{
    m_edge.reserve(5);
}

int Face::operator [](unsigned int i) const
{
    return m_edge[i];
}

Face::FaceInfo Face::getFaceInfo() const
{
    Face::FaceInfo result;
    result.index = m_index;
    result.edgeCount = m_edge.size();
    result.nx = m_normal.x;
    result.ny = m_normal.y;
    result.nz = m_normal.z;
    return result;
}

Face::~Face(void)
{
}
