#include "Log_ObjectFaceRemove.h"

Log_ObjectFaceRemove::Log_ObjectFaceRemove(size_t theTarget,size_t theIndex,Face *theO):HistoryLog(LogType::Object_Face_Remove,theTarget),index(theIndex),o(theO)
{
}

Log_ObjectFaceRemove::~Log_ObjectFaceRemove(void)
{
}
