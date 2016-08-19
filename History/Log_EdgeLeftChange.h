#pragma once
#include "HistoryLog.h"

class Log_EdgeLeftChange :
    public HistoryLog
{
public:
    const unsigned int m_index;

public:
    const unsigned int left;

public:
    Log_EdgeLeftChange(unsigned int target,unsigned int index,unsigned int left);
    std::string toString()
    {
        char temp[256];
        //sprintf(temp,"\t\t\t<Log Type=\"EdgeLeftChange\" Target=\"%d\" Index=\"%d\" Left=\"%d\" />\n",target,index,left);
        std::string out(temp);
        return out;
    }

        void operator=(const Log_EdgeLeftChange& in)
    {
        in;
    }

public:
    ~Log_EdgeLeftChange(void);
};
