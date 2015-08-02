#pragma once
#include "HistoryLog.h"

class Log_VertexAdjacentChange :
	public HistoryLog
{
public:
	const size_t index;
	const size_t adjIndex;

public:
	const int original;

public:
	Log_VertexAdjacentChange(size_t theTarget,size_t theIndex,size_t theAdjIndex,int theOriginal);
	std::string toString()
	{
		char temp[256];
        sprintf(temp,"\t\t\t<Log Type=\"VertexAdjacentChange\" Target=\"%d\" Index=\"%d\" AdjIndex=\"%d\" Original=\"%d\" />\n",target,index,adjIndex,original);
		std::string out(temp);
		return out;
	};
	void operator=(const Log_VertexAdjacentChange& in) 
	{
		in;
	};
public:
	~Log_VertexAdjacentChange(void);
};
