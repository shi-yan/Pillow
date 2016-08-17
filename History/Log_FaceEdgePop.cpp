#include "Log_FaceEdgePop.h"

Log_FaceEdgePop::Log_FaceEdgePop(unsigned int target,unsigned int theIndex,int theO)
    :HistoryLog(LogType::Face_Edge_Pop,target),
      m_index(theIndex),
      o(theO)
{
}

Log_FaceEdgePop::~Log_FaceEdgePop(void)
{
}
