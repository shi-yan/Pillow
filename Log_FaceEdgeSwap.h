#pragma once
#include "HistoryLog.h"

class Log_FaceEdgeSwap :
	public HistoryLog
{
public:
	const size_t index;
public:
	const size_t oIndex;
	const size_t tIndex;
public:
	Log_FaceEdgeSwap(size_t theTarget,size_t theIndex,size_t theOIndex,size_t theTIndex);
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
