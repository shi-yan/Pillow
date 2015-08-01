#include "Log_VertexAdjacentSwap.h"

Log_VertexAdjacentSwap::Log_VertexAdjacentSwap(size_t theTarget,size_t theIndex,size_t theOIndex,size_t theTIndex):HistoryLog(LogType::Vertex_Adjacent_Swap,theTarget),index(theIndex),oIndex(theOIndex),tIndex(theTIndex)
{
}

Log_VertexAdjacentSwap::~Log_VertexAdjacentSwap(void)
{
}
