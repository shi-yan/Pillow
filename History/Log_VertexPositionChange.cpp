#include "Log_VertexPositionChange.h"

Log_VertexPositionChange::Log_VertexPositionChange(unsigned int theTarget,unsigned int theIndex,float theX,float theY,float theZ)
    :HistoryLog(LogType::Vertex_Position_Change,theTarget),
      m_index(theIndex),
      x(theX),
      y(theY),
      z(theZ)
{
}

Log_VertexPositionChange::~Log_VertexPositionChange(void)
{
}
