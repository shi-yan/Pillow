#pragma once
#include "historylog.h"

class Log_ObjectVertexAdd :
	public HistoryLog
{
public:
	const size_t index;
public:
	Log_ObjectVertexAdd(size_t theTarget,size_t theIndex);
	std::string toString()
	{
		char temp[256];
		sprintf_s(temp,"\t\t\t<Log Type=\"ObjectVertexAdd\" Target=\"%d\" Index=\"%d\" />\n",target,index);
		std::string out(temp);
		return out;
	};

	void operator=(const Log_ObjectVertexAdd& in) 
	{
		in;
	};
public:
	~Log_ObjectVertexAdd(void);
};
