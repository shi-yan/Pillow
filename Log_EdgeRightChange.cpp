#include "Log_EdgeRightChange.h"

Log_EdgeRightChange::Log_EdgeRightChange(size_t theTarget,size_t theIndex,size_t theRight):HistoryLog(LogType::Edge_Right_Change,theTarget),index(theIndex),right(theRight)
{
}

Log_EdgeRightChange::~Log_EdgeRightChange(void)
{
}
