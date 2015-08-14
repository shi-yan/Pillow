#include "Log_ObjectEdgeAdd.h"

Log_ObjectEdgeAdd::Log_ObjectEdgeAdd(unsigned int theTarget,unsigned int theIndex)
    :HistoryLog(LogType::Object_Edge_Add,theTarget),
      m_index(theIndex)
{
}

Log_ObjectEdgeAdd::~Log_ObjectEdgeAdd(void)
{
}
