#include "Log_ObjectScaleChange.h"

Log_ObjectScaleChange::Log_ObjectScaleChange(unsigned int theTarget,float theX,float theY,float theZ):HistoryLog(LogType::Object_Scale_Change,theTarget),x(theX),y(theY),z(theZ)
{
}

Log_ObjectScaleChange::~Log_ObjectScaleChange(void)
{
}
