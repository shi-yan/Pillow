#include "Log_VertexAdjacentRemove.h"

Log_VertexAdjacentRemove::Log_VertexAdjacentRemove(size_t theTarget,size_t theIndex,size_t theAdjIndex,int theO):HistoryLog(LogType::Vertex_Adjacent_Remove,theTarget),index(theIndex),adjIndex(theAdjIndex),o(theO)
{
}

Log_VertexAdjacentRemove::~Log_VertexAdjacentRemove(void)
{
}
