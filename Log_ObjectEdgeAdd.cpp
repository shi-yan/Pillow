#include "Log_ObjectEdgeAdd.h"

Log_ObjectEdgeAdd::Log_ObjectEdgeAdd(size_t theTarget,size_t theIndex):HistoryLog(LogType::Object_Edge_Add,theTarget),index(theIndex)
{
}

Log_ObjectEdgeAdd::~Log_ObjectEdgeAdd(void)
{
}
