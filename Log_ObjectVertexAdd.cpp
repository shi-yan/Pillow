#include "Log_ObjectVertexAdd.h"

Log_ObjectVertexAdd::Log_ObjectVertexAdd(size_t theTarget,size_t theIndex):HistoryLog(LogType::Object_Vertex_Add,theTarget),index(theIndex)
{
}

Log_ObjectVertexAdd::~Log_ObjectVertexAdd(void)
{
}
