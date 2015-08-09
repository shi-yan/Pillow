#pragma once
#include "HistoryLog.h"

class Log_EdgeLeftChange :
	public HistoryLog
{
public:
	const size_t index;

public:
	const size_t left;

public:
	Log_EdgeLeftChange(size_t theTarget,size_t theIndex,size_t theLeft);
	std::string toString()
	{
		char temp[256];
        sprintf(temp,"\t\t\t<Log Type=\"EdgeLeftChange\" Target=\"%d\" Index=\"%d\" Left=\"%d\" />\n",target,index,left);
		std::string out(temp);
		return out;
	};

		void operator=(const Log_EdgeLeftChange& in) 
	{
		in;
	};

public:
	~Log_EdgeLeftChange(void);
};
