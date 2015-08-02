#pragma once
#include "HistoryLog.h"

class Log_VertexAdjacentRemove :
	public HistoryLog
{
public:
	const size_t index;
	const size_t adjIndex;

public:
	const int o;

public:
	Log_VertexAdjacentRemove(size_t theTarget,size_t theIndex,size_t theAdjIndex,int theO);
	std::string toString()
	{
		char temp[256];
        sprintf(temp,"\t\t\t<Log Type=\"VertexAdjacentRemove\" Target=\"%d\" Index=\"%d\" AdjIndex=\"%d\" O=\"%d\" />\n",target,index,adjIndex,o);
		std::string out(temp);
		return out;
	};

	void operator=(const Log_VertexAdjacentRemove& in) 
	{
		in;
	};
public:
	~Log_VertexAdjacentRemove(void);
};
