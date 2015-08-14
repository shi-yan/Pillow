#include "SubdivideFace.h"

SubdivideFace::SubdivideFace(int v1, int v2, int v3, int v4)
    :ElementBase(),
      m_center(0),
      m_normal()
{
    m_edge[0]=v1;
    m_edge[1]=v2;
    m_edge[2]=v3;
    m_edge[3]=v4;
    m_subFace[0]=0;
    m_subFace[1]=0;
    m_subFace[2]=0;
    m_subFace[3]=0;
}

SubdivideFace::~SubdivideFace(void)
{
}
