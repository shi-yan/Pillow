#include "Log_ObjectEdgeRemove.h"

Log_ObjectEdgeRemove::Log_ObjectEdgeRemove(unsigned int theTarget,unsigned int theIndex,Edge *theO)
    :HistoryLog(LogType::Object_Edge_Remove,theTarget),
      m_index(theIndex),
      o(theO)
{
}

Log_ObjectEdgeRemove::~Log_ObjectEdgeRemove(void)
{
    //delete o;
}
