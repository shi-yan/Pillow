#include "Log_EdgeLeftChange.h"

Log_EdgeLeftChange::Log_EdgeLeftChange(size_t theTarget,size_t theIndex,size_t theLeft):HistoryLog(LogType::Edge_Left_Change,theTarget),index(theIndex),left(theLeft)
{
}

Log_EdgeLeftChange::~Log_EdgeLeftChange(void)
{
}
