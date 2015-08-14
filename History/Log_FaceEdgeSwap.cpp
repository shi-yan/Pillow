#include "Log_FaceEdgeSwap.h"

Log_FaceEdgeSwap::Log_FaceEdgeSwap(unsigned int theTarget,unsigned int theIndex,unsigned int theOIndex,unsigned int theTIndex):HistoryLog(LogType::Face_Edge_Swap,theTarget),index(theIndex),oIndex(theOIndex),tIndex(theTIndex)
{
}

Log_FaceEdgeSwap::~Log_FaceEdgeSwap(void)
{
}
