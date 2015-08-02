#pragma once
#include "HistoryLog.h"

class Log_VertexAdjacentSwap :
	public HistoryLog
{
public:
	const size_t index;

public:
	const size_t oIndex;
	const size_t tIndex;

public:
	Log_VertexAdjacentSwap(size_t theTarget,size_t theIndex,size_t theOIndex,size_t theTIndex);
	std::string toString()
	{
		char temp[256];
        sprintf(temp,"\t\t\t<Log Type=\"VertexAdjacentSwap\" Target=\"%d\" Index=\"%d\" OIndex=\"%d\" TIndex=\"%d\" />\n",target,index,oIndex,tIndex);
		std::string out(temp);
		return out;
	};

	void operator=(const Log_VertexAdjacentSwap& in) 
	{
		in;
	};
public:
	~Log_VertexAdjacentSwap(void);
};
