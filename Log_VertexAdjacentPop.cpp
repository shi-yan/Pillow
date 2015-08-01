#include "Log_VertexAdjacentPop.h"

Log_VertexAdjacentPop::Log_VertexAdjacentPop(size_t theTarget,size_t theIndex,int theO):HistoryLog(LogType::Vertex_Adjacent_Pop,theTarget),index(theIndex),o(theO)
{
}

Log_VertexAdjacentPop::~Log_VertexAdjacentPop(void)
{
}
