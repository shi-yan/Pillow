#include "Log_VertexAdjacentInsert.h"

Log_VertexAdjacentInsert::Log_VertexAdjacentInsert(size_t theTarget,size_t theIndex,size_t theAdjIndex):HistoryLog(LogType::Vertex_Adjacent_Insert,theTarget),index(theIndex),adjIndex(theAdjIndex)
{
}

Log_VertexAdjacentInsert::~Log_VertexAdjacentInsert(void)
{
}
