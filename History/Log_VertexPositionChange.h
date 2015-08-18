#pragma once
#include "HistoryLog.h"

class Log_VertexPositionChange :
    public HistoryLog
{
public:
    const unsigned int m_index;

public:
    const float x;
    const float y;
    const float z;

public:
    Log_VertexPositionChange(unsigned int theTarget,unsigned int theIndex,float theX,float theY,float theZ);
    std::string toString()
    {
        char temp[256];
        sprintf(temp,"\t\t\t<Log Type=\"VertexPositionChange\" Target=\"%d\" Index=\"%d\" X=\"%f\" Y=\"%f\" Z=\"%f\" />\n",target,index,x,y,z);
        std::string out(temp);
        return out;
    }

    void operator=(const Log_VertexPositionChange& in)
    {
        in;
    }
public:
    ~Log_VertexPositionChange(void);
};
