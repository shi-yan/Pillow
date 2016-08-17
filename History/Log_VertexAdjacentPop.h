#pragma once
#include "Core/Vertex.h"
#include "HistoryLog.h"

class Log_VertexAdjacentPop :
    public HistoryLog
{
public:
    const unsigned int m_index;
    const int o;

public:
    Log_VertexAdjacentPop(unsigned int theTarget,unsigned int theIndex,int o);
    std::string toString()
    {
        char temp[256];
        //sprintf(temp,"\t\t\t<Log Type=\"VertexAdjacentPop\" Target=\"%d\" Index=\"%d\" O=\"%d\" />\n",target,index,o);
        std::string out(temp);
        return out;
    }

        void operator=(const Log_VertexAdjacentPop& in)
    {
        in;
    }
public:
    ~Log_VertexAdjacentPop(void);
};
