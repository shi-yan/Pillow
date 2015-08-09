#include "Log_VertexAdjacentPush.h"

Log_VertexAdjacentPush::Log_VertexAdjacentPush(size_t theTarget,size_t theIndex):HistoryLog(LogType::Vertex_Adjacent_Push,theTarget),index(theIndex)
{
}

Log_VertexAdjacentPush::~Log_VertexAdjacentPush(void)
{
}
