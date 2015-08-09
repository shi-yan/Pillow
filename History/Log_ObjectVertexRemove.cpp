#include "Log_ObjectVertexRemove.h"

Log_ObjectVertexRemove::Log_ObjectVertexRemove(size_t theTarget,size_t theIndex,Vertex *theO):HistoryLog(LogType::Object_Vertex_Remove,theTarget),index(theIndex),o(theO)
{
}

Log_ObjectVertexRemove::~Log_ObjectVertexRemove(void)
{
	//delete o;
}
