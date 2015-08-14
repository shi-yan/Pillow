#include "Log_ObjectFaceAdd.h"

Log_ObjectFaceAdd::Log_ObjectFaceAdd(unsigned int theTarget,unsigned int theIndex)
    :HistoryLog(LogType::Object_Face_Add,theTarget),
      m_index(theIndex)
{
}

Log_ObjectFaceAdd::~Log_ObjectFaceAdd(void)
{
}
