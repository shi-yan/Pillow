#pragma once
#include "HistoryLog.h"

class Log_FaceEdgePop :
    public HistoryLog
{
public:
    const unsigned int m_index;
public:
    const int o;
public:
    Log_FaceEdgePop(unsigned int theTarget,unsigned int theIndex,int theO);
    std::string toString()
    {
        char temp[256];
        sprintf(temp, "\t\t\t<Log Type=\"FaceEdgePop\" Target=\"%d\" Index=\"%d\" O=\"%d\" />\n", target, index, o);
        std::string out(temp);
        return out;
    }
                    void operator=(const Log_FaceEdgePop& in)
    {
        in;
    }
public:
    ~Log_FaceEdgePop(void);
};
