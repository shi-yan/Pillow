#include "Log_ObjectFaceRemove.h"

Log_ObjectFaceRemove::Log_ObjectFaceRemove(unsigned int theTarget,unsigned int theIndex,Face *theO):HistoryLog(LogType::Object_Face_Remove,theTarget),index(theIndex),o(theO)
{
}

Log_ObjectFaceRemove::~Log_ObjectFaceRemove(void)
{
}
