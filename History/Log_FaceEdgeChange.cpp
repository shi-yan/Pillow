#include "Log_FaceEdgeChange.h"

Log_FaceEdgeChange::Log_FaceEdgeChange(unsigned int target,unsigned int theIndex,unsigned int theEdgeIndex,int theO)
    :HistoryLog(LogType::Face_Edge_Change,target),
      m_index(theIndex),
      edgeIndex(theEdgeIndex),
      o(theO)
{
}

Log_FaceEdgeChange::~Log_FaceEdgeChange(void)
{
}
