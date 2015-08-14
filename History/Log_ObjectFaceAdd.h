#pragma once
#include "HistoryLog.h"

class Log_ObjectFaceAdd :
	public HistoryLog
{
public:
    const unsigned int index;
public:
    Log_ObjectFaceAdd(unsigned int theTarget,unsigned int theIndex);
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
