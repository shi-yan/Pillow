#pragma once
#include "HistoryLog.h"

class Log_ObjectEdgeAdd :
	public HistoryLog
{
public:
    const unsigned int index;
public:
    Log_ObjectEdgeAdd(unsigned int theTarget,unsigned int theIndex);
	std::string toString()
	{
		char temp[256];
        sprintf(temp,"\t\t\t<Log Type=\"ObjectEdgeAdd\" Target=\"%d\" Index=\"%d\" />\n",target,index);
		std::string out(temp);
		return out;
	};

		void operator=(const Log_ObjectEdgeAdd& in) 
	{
		in;
	};
public:
	~Log_ObjectEdgeAdd(void);
};
