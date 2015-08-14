#include "Log_ObjectPositionChange.h"

Log_ObjectPositionChange::Log_ObjectPositionChange(unsigned int theTarget,float theX,float theY,float theZ):HistoryLog(LogType::Object_Position_Change,theTarget),x(theX),y(theY),z(theZ)
{
}

Log_ObjectPositionChange::~Log_ObjectPositionChange(void)
{
}
