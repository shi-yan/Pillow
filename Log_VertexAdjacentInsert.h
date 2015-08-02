#pragma once
#include "HistoryLog.h"

class Log_VertexAdjacentInsert :
	public HistoryLog
{
public:
	const size_t index;

public:
	const size_t adjIndex;

public:
	Log_VertexAdjacentInsert(size_t theTarget,size_t theIndex,size_t adjIndex);
	std::string toString()
	{
		char temp[256];
        sprintf(temp,"\t\t\t<Log Type=\"VertexAdjacentInsert\" Target=\"%d\" Index=\"%d\" AdjIndex=\"%d\" />\n",target,index,adjIndex);
		std::string out(temp);
		return out;
	};

	void operator=(const Log_VertexAdjacentInsert& in) 
	{
		in;
	};
public:
	~Log_VertexAdjacentInsert(void);
};
