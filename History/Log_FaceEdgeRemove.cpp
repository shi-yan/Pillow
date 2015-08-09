#include "Log_FaceEdgeRemove.h"

Log_FaceEdgeRemove::Log_FaceEdgeRemove(size_t theTarget,size_t theIndex,size_t theEdgeIndex,int theO):HistoryLog(LogType::Face_Edge_Remove,theTarget),index(theIndex),edgeIndex(theEdgeIndex),o(theO)
{
}

Log_FaceEdgeRemove::~Log_FaceEdgeRemove(void)
{
}
