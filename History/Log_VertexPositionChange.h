#pragma once
#include "HistoryLog.h"

class Log_VertexPositionChange :
    public HistoryLog
{
public:
    const unsigned int m_index;

public:
    const float m_x;
    const float m_y;
    const float m_z;

public:
    Log_VertexPositionChange(unsigned int target, unsigned int index, float x, float y, float z);
    std::string toString()
    {
        char temp[256];
        //sprintf(temp,"\t\t\t<Log Type=\"VertexPositionChange\" Target=\"%d\" Index=\"%d\" X=\"%f\" Y=\"%f\" Z=\"%f\" />\n",target,index,x,y,z);
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
