#include "Log_VertexAdjacentSwap.h"

Log_VertexAdjacentSwap::Log_VertexAdjacentSwap(unsigned int theTarget,unsigned int theIndex,unsigned int theOIndex,unsigned int theTIndex)
    :HistoryLog(LogType::Vertex_Adjacent_Swap,theTarget),
      m_index(theIndex),
      oIndex(theOIndex),
      tIndex(theTIndex)
{
}

Log_VertexAdjacentSwap::~Log_VertexAdjacentSwap(void)
{
}
