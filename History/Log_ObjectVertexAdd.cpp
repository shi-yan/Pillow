#include "Log_ObjectVertexAdd.h"

Log_ObjectVertexAdd::Log_ObjectVertexAdd(unsigned int theTarget,unsigned int theIndex):HistoryLog(LogType::Object_Vertex_Add,theTarget),index(theIndex)
{
}

Log_ObjectVertexAdd::~Log_ObjectVertexAdd(void)
{
}
