#pragma once
#include "historylog.h"

class Log_ObjectEdgeAdd :
	public HistoryLog
{
public:
	const size_t index;
public:
	Log_ObjectEdgeAdd(size_t theTarget,size_t theIndex);
	std::string toString()
	{
		char temp[256];
		sprintf_s(temp,"\t\t\t<Log Type=\"ObjectEdgeAdd\" Target=\"%d\" Index=\"%d\" />\n",target,index);
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
