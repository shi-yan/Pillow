#pragma once
#include "HistoryLog.h"

class Log_ObjectFaceAdd :
	public HistoryLog
{
public:
	const size_t index;
public:
	Log_ObjectFaceAdd(size_t theTarget,size_t theIndex);
	std::string toString()
	{
		char temp[256];
        sprintf(temp,"\t\t\t<Log Type=\"ObjectFaceAdd\" Target=\"%d\" Index=\"%d\" />\n",target,index);
		std::string out(temp);
		return out;
	};

				void operator=(const Log_ObjectFaceAdd& in) 
	{
		in;
	};

public:
	~Log_ObjectFaceAdd(void);
};
