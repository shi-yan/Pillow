#include "Log_EdgeStartChange.h"

Log_EdgeStartChange::Log_EdgeStartChange(size_t theTarget,size_t theIndex,int theStart):HistoryLog(LogType::Edge_Start_Change,theTarget),index(theIndex),start(theStart)
{
}

Log_EdgeStartChange::~Log_EdgeStartChange(void)
{
}
