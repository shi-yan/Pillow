#include "Log_VertexNormalChange.h"

Log_VertexNormalChange::Log_VertexNormalChange(unsigned int theTarget,unsigned int theIndex,float theX,float theY,float theZ)
    :HistoryLog(LogType::Vertex_Normal_Change,theTarget),
      m_index(theIndex),
      x(theX),
      y(theY),
      z(theZ)
{
}

Log_VertexNormalChange::~Log_VertexNormalChange(void)
{
}
