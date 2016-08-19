#include "Log_EdgeEndChange.h"

Log_EdgeEndChange::Log_EdgeEndChange(unsigned int target, unsigned int index, int end)
    :HistoryLog(LogType::Edge_End_Change, target),
      m_index(index),
      end(end)
{
}

Log_EdgeEndChange::~Log_EdgeEndChange(void)
{
}
