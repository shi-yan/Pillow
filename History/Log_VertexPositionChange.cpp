#include "Log_VertexPositionChange.h"

Log_VertexPositionChange::Log_VertexPositionChange(unsigned int target, unsigned int index, float x, float y, float z)
    :HistoryLog(LogType::Vertex_Position_Change, target),
      m_index(index),
      m_x(x),
      m_y(y),
      m_z(z)
{
}

Log_VertexPositionChange::~Log_VertexPositionChange(void)
{
}
