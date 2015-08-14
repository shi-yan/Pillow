#include "Log_FaceEdgeRemove.h"

Log_FaceEdgeRemove::Log_FaceEdgeRemove(unsigned int theTarget,unsigned int theIndex,unsigned int theEdgeIndex,int theO):HistoryLog(LogType::Face_Edge_Remove,theTarget),index(theIndex),edgeIndex(theEdgeIndex),o(theO)
{
}

Log_FaceEdgeRemove::~Log_FaceEdgeRemove(void)
{
}
