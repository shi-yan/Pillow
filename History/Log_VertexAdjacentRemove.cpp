#include "Log_VertexAdjacentRemove.h"

Log_VertexAdjacentRemove::Log_VertexAdjacentRemove(unsigned int theTarget,unsigned int theIndex,unsigned int theAdjIndex,int theO)
    :HistoryLog(LogType::Vertex_Adjacent_Remove,theTarget),
      m_index(theIndex),
      adjIndex(theAdjIndex),
      o(theO)
{
}

Log_VertexAdjacentRemove::~Log_VertexAdjacentRemove(void)
{
}
