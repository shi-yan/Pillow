#pragma once
#include "HistoryLog.h"

class Log_FaceEdgeSwap :
	public HistoryLog
{
public:
    const unsigned int index;
public:
    const unsigned int oIndex;
    const unsigned int tIndex;
public:
    Log_FaceEdgeSwap(unsigned int theTarget,unsigned int theIndex,unsigned int theOIndex,unsigned int theTIndex);
	std::string toString()
	{
		char temp[256];
        sprintf(temp,"\t\t\t<Log Type=\"FaceEdgeSwap\" Target=\"%d\" Index=\"%d\" OIndex=\"%d\" TIndex=\"%d\" />\n",target,index,oIndex,tIndex);
		std::string out(temp);
		return out;
	};

							void operator=(const Log_FaceEdgeSwap& in) 
	{
		in;
	};
public:
	~Log_FaceEdgeSwap(void);
};
