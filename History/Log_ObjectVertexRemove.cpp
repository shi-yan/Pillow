#include "Log_ObjectVertexRemove.h"

Log_ObjectVertexRemove::Log_ObjectVertexRemove(unsigned int theTarget,unsigned int theIndex,Vertex *theO)
    :HistoryLog(LogType::Object_Vertex_Remove,theTarget),
      m_index(theIndex),
      o(theO)
{
}

Log_ObjectVertexRemove::~Log_ObjectVertexRemove(void)
{
    //delete o;
}
