#include "Log_FaceEdgeChange.h"

Log_FaceEdgeChange::Log_FaceEdgeChange(size_t theTarget,size_t theIndex,size_t theEdgeIndex,int theO):HistoryLog(LogType::Face_Edge_Change,theTarget),index(theIndex),edgeIndex(theEdgeIndex),o(theO)
{
}

Log_FaceEdgeChange::~Log_FaceEdgeChange(void)
{
}
