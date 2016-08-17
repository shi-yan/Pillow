#include "Log_FaceEdgeSwap.h"

Log_FaceEdgeSwap::Log_FaceEdgeSwap(unsigned int target,unsigned int theIndex,unsigned int theOIndex,unsigned int theTIndex)
    :HistoryLog(LogType::Face_Edge_Swap,target),
      m_index(theIndex),
      oIndex(theOIndex),
      tIndex(theTIndex)
{
}

Log_FaceEdgeSwap::~Log_FaceEdgeSwap(void)
{
}
