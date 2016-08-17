#include "Log_ObjectColorChange.h"

Log_ObjectColorChange::Log_ObjectColorChange(unsigned int target,unsigned int theR,unsigned int theG,unsigned int theB,unsigned int theA):HistoryLog(LogType::Object_Color_Change,target),a(theA),r(theR),g(theG),b(theB)
{
}

Log_ObjectColorChange::~Log_ObjectColorChange(void)
{
}
