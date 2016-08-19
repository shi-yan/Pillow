#include "Log_EdgeRightChange.h"

Log_EdgeRightChange::Log_EdgeRightChange(unsigned int target, unsigned int index, unsigned int right)
    :HistoryLog(LogType::Edge_Right_Change, target),
      m_index(index),
      right(right)
{
}

Log_EdgeRightChange::~Log_EdgeRightChange(void)
{
}
