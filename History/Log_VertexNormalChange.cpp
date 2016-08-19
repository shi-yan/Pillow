#include "Log_VertexNormalChange.h"

Log_VertexNormalChange::Log_VertexNormalChange(unsigned int target, unsigned int index, float x, float y, float z)
    :HistoryLog(LogType::Vertex_Normal_Change, target),
      m_index(index),
      m_x(x),
      m_y(y),
      m_z(z)
{
}

Log_VertexNormalChange::~Log_VertexNormalChange(void)
{
}
