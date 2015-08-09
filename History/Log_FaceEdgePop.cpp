#include "Log_FaceEdgePop.h"

Log_FaceEdgePop::Log_FaceEdgePop(size_t theTarget,size_t theIndex,int theO):HistoryLog(LogType::Face_Edge_Pop,theTarget),index(theIndex),o(theO)
{
}

Log_FaceEdgePop::~Log_FaceEdgePop(void)
{
}
