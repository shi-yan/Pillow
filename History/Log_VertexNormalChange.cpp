#include "Log_VertexNormalChange.h"

Log_VertexNormalChange::Log_VertexNormalChange(size_t theTarget,size_t theIndex,float theX,float theY,float theZ):HistoryLog(LogType::Vertex_Normal_Change,theTarget),index(theIndex),x(theX),y(theY),z(theZ)
{
}

Log_VertexNormalChange::~Log_VertexNormalChange(void)
{
}
