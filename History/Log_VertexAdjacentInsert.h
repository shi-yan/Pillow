#pragma once
#include "HistoryLog.h"

class Log_VertexAdjacentInsert :
	public HistoryLog
{
public:
    const unsigned int index;

public:
    const unsigned int adjIndex;

public:
    Log_VertexAdjacentInsert(unsigned int theTarget,unsigned int theIndex,unsigned int adjIndex);
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
