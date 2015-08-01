#include "Log_ObjectFaceAdd.h"

Log_ObjectFaceAdd::Log_ObjectFaceAdd(size_t theTarget,size_t theIndex):HistoryLog(LogType::Object_Face_Add,theTarget),index(theIndex)
{
}

Log_ObjectFaceAdd::~Log_ObjectFaceAdd(void)
{
}
