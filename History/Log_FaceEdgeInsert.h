#pragma once
#include "HistoryLog.h"

class Log_FaceEdgeInsert :
	public HistoryLog
{
public:
    const unsigned int index;
    const unsigned int edgeIndex;
public:
    Log_FaceEdgeInsert(unsigned int theTarget,unsigned int theIndex,unsigned int theEdgeIndex);
	std::string toString()
	{
		char temp[256];
        sprintf(temp,"\t\t\t<Log Type=\"FaceEdgeInsert\" Target=\"%d\" Index=\"%d\" EdgeIndex=\"%d\" />\n",target,index,edgeIndex);
		std::string out(temp);
		return out;
	};
				void operator=(const Log_FaceEdgeInsert& in) 
	{
		in;
	};
public:
	~Log_FaceEdgeInsert(void);
};
