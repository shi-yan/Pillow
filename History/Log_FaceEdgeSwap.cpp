#include "Log_FaceEdgeSwap.h"

Log_FaceEdgeSwap::Log_FaceEdgeSwap(size_t theTarget,size_t theIndex,size_t theOIndex,size_t theTIndex):HistoryLog(LogType::Face_Edge_Swap,theTarget),index(theIndex),oIndex(theOIndex),tIndex(theTIndex)
{
}

Log_FaceEdgeSwap::~Log_FaceEdgeSwap(void)
{
}
