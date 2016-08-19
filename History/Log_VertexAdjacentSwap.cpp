#include "Log_VertexAdjacentSwap.h"

Log_VertexAdjacentSwap::Log_VertexAdjacentSwap(unsigned int target, unsigned int index, unsigned int oIndex, unsigned int tIndex)
    :HistoryLog(LogType::Vertex_Adjacent_Swap, target),
      m_index(index),
      m_oIndex(oIndex),
      m_tIndex(tIndex)
{
}

Log_VertexAdjacentSwap::~Log_VertexAdjacentSwap(void)
{
}
