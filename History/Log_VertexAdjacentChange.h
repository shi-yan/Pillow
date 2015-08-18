#pragma once
#include "HistoryLog.h"

class Log_VertexAdjacentChange :
    public HistoryLog
{
public:
    const unsigned int m_index;
    const unsigned int adjIndex;

public:
    const int original;

public:
    Log_VertexAdjacentChange(unsigned int theTarget,unsigned int theIndex,unsigned int theAdjIndex,int theOriginal);
    std::string toString()
    {
        char temp[256];
        sprintf(temp,"\t\t\t<Log Type=\"VertexAdjacentChange\" Target=\"%d\" Index=\"%d\" AdjIndex=\"%d\" Original=\"%d\" />\n",target,index,adjIndex,original);
        std::string out(temp);
        return out;
    }
    void operator=(const Log_VertexAdjacentChange& in)
    {
        in;
    }
public:
    ~Log_VertexAdjacentChange(void);
};
