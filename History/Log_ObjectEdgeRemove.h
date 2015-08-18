#pragma once
#include "HistoryLog.h"
#include "../Core/Edge.h"

class Log_ObjectEdgeRemove :
    public HistoryLog
{
public:
    const unsigned int m_index;
public:
    Edge *o;

public:
    Log_ObjectEdgeRemove(unsigned int theTarget,unsigned int theIndex,Edge *theO);
    std::string toString()
    {
        char temp[256];
        sprintf(temp,"\t\t\t<Log Type=\"ObjectEdgeRemove\" Target=\"%d\" Index=\"%d\" />\n",target,index);
        std::string out(temp);
        return out;
    }

            void operator=(const Log_ObjectEdgeRemove& in)
    {
        in;
    }
public:
    ~Log_ObjectEdgeRemove(void);
};
