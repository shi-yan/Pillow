#include "Log_EdgeEndChange.h"

Log_EdgeEndChange::Log_EdgeEndChange(unsigned int target,unsigned int theIndex,int theEnd)
    :HistoryLog(LogType::Edge_End_Change,target),
      m_index(theIndex),
      end(theEnd)
{
}

Log_EdgeEndChange::~Log_EdgeEndChange(void)
{
}
