#include "Log_FaceEdgeInsert.h"

Log_FaceEdgeInsert::Log_FaceEdgeInsert(size_t theTarget,size_t theIndex,size_t theEdgeIndex):HistoryLog(LogType::Face_Edge_Insert,theTarget),index(theIndex),edgeIndex(theEdgeIndex)
{
}

Log_FaceEdgeInsert::~Log_FaceEdgeInsert(void)
{
}
