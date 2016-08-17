#pragma once
#include "HistoryLog.h"

class Log_VertexAdjacentPush :
    public HistoryLog
{
public:
    const unsigned int m_index;

public:
    Log_VertexAdjacentPush(unsigned int theTarget,unsigned int index);
    std::string toString()
    {
        char temp[256];
        //sprintf(temp,"\t\t\t<Log Type=\"VertexAdjacentPush\" Target=\"%d\" Index=\"%d\" />\n",target,index);
        std::string out(temp);
        return out;
    }

            void operator=(const Log_VertexAdjacentPush& in)
    {
        in;
    }
public:
    ~Log_VertexAdjacentPush(void);
};
