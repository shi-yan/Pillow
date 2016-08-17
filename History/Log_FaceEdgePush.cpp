#include "Log_FaceEdgePush.h"

Log_FaceEdgePush::Log_FaceEdgePush(unsigned int target,unsigned int theIndex)
    :HistoryLog(LogType::Face_Edge_Push,target),
      m_index(theIndex)
{
}

Log_FaceEdgePush::~Log_FaceEdgePush(void)
{
}
