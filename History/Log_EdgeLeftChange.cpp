#include "Log_EdgeLeftChange.h"

Log_EdgeLeftChange::Log_EdgeLeftChange(unsigned int target,unsigned int theIndex,unsigned int theLeft)
    :HistoryLog(LogType::Edge_Left_Change,target),
      m_index(theIndex),
      left(theLeft)
{
}

Log_EdgeLeftChange::~Log_EdgeLeftChange(void)
{
}
