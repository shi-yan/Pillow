#include "Log_EdgeLeftChange.h"

Log_EdgeLeftChange::Log_EdgeLeftChange(unsigned int theTarget,unsigned int theIndex,unsigned int theLeft):HistoryLog(LogType::Edge_Left_Change,theTarget),index(theIndex),left(theLeft)
{
}

Log_EdgeLeftChange::~Log_EdgeLeftChange(void)
{
}
