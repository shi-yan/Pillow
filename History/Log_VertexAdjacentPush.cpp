#include "Log_VertexAdjacentPush.h"

Log_VertexAdjacentPush::Log_VertexAdjacentPush(unsigned int theTarget,unsigned int theIndex)
    :HistoryLog(LogType::Vertex_Adjacent_Push,theTarget),
      m_index(theIndex)
{
}

Log_VertexAdjacentPush::~Log_VertexAdjacentPush(void)
{
}
