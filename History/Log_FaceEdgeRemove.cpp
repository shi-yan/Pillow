#include "Log_FaceEdgeRemove.h"

Log_FaceEdgeRemove::Log_FaceEdgeRemove(unsigned int target,unsigned int theIndex,unsigned int theEdgeIndex,int theO)
    :HistoryLog(LogType::Face_Edge_Remove,target),
      m_index(theIndex),
      edgeIndex(theEdgeIndex),
      o(theO)
{
}

Log_FaceEdgeRemove::~Log_FaceEdgeRemove(void)
{
}
