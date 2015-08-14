#include "Log_EdgeEndChange.h"

Log_EdgeEndChange::Log_EdgeEndChange(unsigned int theTarget,unsigned int theIndex,int theEnd):HistoryLog(LogType::Edge_End_Change,theTarget),index(theIndex),end(theEnd)
{
}

Log_EdgeEndChange::~Log_EdgeEndChange(void)
{
}
