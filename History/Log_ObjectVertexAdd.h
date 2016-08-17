#pragma once
#include "HistoryLog.h"

class Log_ObjectVertexAdd :
    public HistoryLog
{
public:
    const unsigned int m_index;
public:
    Log_ObjectVertexAdd(unsigned int theTarget, unsigned int theIndex);
    std::string toString()
    {
        char temp[256];
        //sprintf(temp,"\t\t\t<Log Type=\"ObjectVertexAdd\" Target=\"%d\" Index=\"%d\" />\n",target,index);
        std::string out(temp);
        return out;
    }

    void operator=(const Log_ObjectVertexAdd& in)
    {
        in;
    }
public:
    ~Log_ObjectVertexAdd(void);
};
