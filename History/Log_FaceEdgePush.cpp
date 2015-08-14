#include "Log_FaceEdgePush.h"

Log_FaceEdgePush::Log_FaceEdgePush(unsigned int theTarget,unsigned int theIndex)
    :HistoryLog(LogType::Face_Edge_Push,theTarget),
      m_index(theIndex)
{
}

Log_FaceEdgePush::~Log_FaceEdgePush(void)
{
}
