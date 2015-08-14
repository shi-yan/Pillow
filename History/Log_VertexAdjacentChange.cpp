#include "Log_VertexAdjacentChange.h"

Log_VertexAdjacentChange::Log_VertexAdjacentChange(unsigned int theTarget,unsigned int theIndex,unsigned int theAdjIndex,int theOriginal):HistoryLog(LogType::Vertex_Adjacent_Change,theTarget),index(theIndex),adjIndex(theAdjIndex),original(theOriginal)
{
}

Log_VertexAdjacentChange::~Log_VertexAdjacentChange(void)
{
}
