#include "Log_FaceEdgePush.h"

Log_FaceEdgePush::Log_FaceEdgePush(size_t theTarget,size_t theIndex):HistoryLog(LogType::Face_Edge_Push,theTarget),index(theIndex)
{
}

Log_FaceEdgePush::~Log_FaceEdgePush(void)
{
}
