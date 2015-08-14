#include "Log_EdgeRightChange.h"

Log_EdgeRightChange::Log_EdgeRightChange(unsigned int theTarget,unsigned int theIndex,unsigned int theRight)
    :HistoryLog(LogType::Edge_Right_Change,theTarget),
      m_index(theIndex),
      right(theRight)
{
}

Log_EdgeRightChange::~Log_EdgeRightChange(void)
{
}
