#include "Log_VertexAdjacentChange.h"

Log_VertexAdjacentChange::Log_VertexAdjacentChange(size_t theTarget,size_t theIndex,size_t theAdjIndex,int theOriginal):HistoryLog(LogType::Vertex_Adjacent_Change,theTarget),index(theIndex),adjIndex(theAdjIndex),original(theOriginal)
{
}

Log_VertexAdjacentChange::~Log_VertexAdjacentChange(void)
{
}
