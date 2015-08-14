#pragma once
#include "HistoryLog.h"

class Log_FaceEdgeRemove :
	public HistoryLog
{
public:
    const unsigned int m_index;
public:
    const unsigned int edgeIndex;
	const int o;
public:
    Log_FaceEdgeRemove(unsigned int theTarget,unsigned int theIndex,unsigned int theEdgeIndex,int theO);
	std::string toString()
	{
		char temp[256];
        sprintf(temp,"\t\t\t<Log Type=\"FaceEdgeRemove\" Target=\"%d\" Index=\"%d\" EdgeIndex=\"%d\" O=\"%d\" />\n",target,index,edgeIndex,o);
		std::string out(temp);
		return out;
    }

						void operator=(const Log_FaceEdgeRemove& in) 
	{
		in;
    }
public:
	~Log_FaceEdgeRemove(void);
};
