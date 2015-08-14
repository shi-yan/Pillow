#include "Log_VertexAdjacentInsert.h"

Log_VertexAdjacentInsert::Log_VertexAdjacentInsert(unsigned int theTarget,unsigned int theIndex,unsigned int theAdjIndex):HistoryLog(LogType::Vertex_Adjacent_Insert,theTarget),index(theIndex),adjIndex(theAdjIndex)
{
}

Log_VertexAdjacentInsert::~Log_VertexAdjacentInsert(void)
{
}
