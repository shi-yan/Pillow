#include "Log_EdgeLeftChange.h"

Log_EdgeLeftChange::Log_EdgeLeftChange(unsigned int target, unsigned int index, unsigned int left)
    :HistoryLog(LogType::Edge_Left_Change, target),
      m_index(index),
      left(left)
{
}

Log_EdgeLeftChange::~Log_EdgeLeftChange(void)
{
}
