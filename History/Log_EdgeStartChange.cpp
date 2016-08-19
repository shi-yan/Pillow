#include "Log_EdgeStartChange.h"

Log_EdgeStartChange::Log_EdgeStartChange(unsigned int target, unsigned int index, int start)
    :HistoryLog(LogType::Edge_Start_Change, target),
      m_index(index),
      start(start)
{
}

Log_EdgeStartChange::~Log_EdgeStartChange(void)
{
}
