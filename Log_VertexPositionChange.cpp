#include "Log_VertexPositionChange.h"

Log_VertexPositionChange::Log_VertexPositionChange(size_t theTarget,size_t theIndex,float theX,float theY,float theZ):HistoryLog(LogType::Vertex_Position_Change,theTarget),index(theIndex),x(theX),y(theY),z(theZ)
{
}

Log_VertexPositionChange::~Log_VertexPositionChange(void)
{
}
