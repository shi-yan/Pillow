#pragma once
#include "HistoryLog.h"
#include "Edge.h"

class Log_ObjectEdgeRemove :
	public HistoryLog
{
public:
	const size_t index;
public:
	Edge *o;

public:
	Log_ObjectEdgeRemove(size_t theTarget,size_t theIndex,Edge *theO);
	std::string toString()
	{
		char temp[256];
        sprintf(temp,"\t\t\t<Log Type=\"ObjectEdgeRemove\" Target=\"%d\" Index=\"%d\" />\n",target,index);
		std::string out(temp);
		return out;
	};

			void operator=(const Log_ObjectEdgeRemove& in) 
	{
		in;
	};
public:
	~Log_ObjectEdgeRemove(void);
};
