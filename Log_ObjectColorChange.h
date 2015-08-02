#pragma once
#include "HistoryLog.h"

class Log_ObjectColorChange :
	public HistoryLog
{
public:
	const unsigned int r;
	const unsigned int g;
	const unsigned int b;
	const unsigned int a;
public:
	Log_ObjectColorChange(size_t theTarget,unsigned int theR,unsigned int theG,unsigned int theB,unsigned int theA);
	std::string toString()
	{
		char temp[256];
        sprintf(temp,"\t\t\t<Log Type=\"ObjectColorChange\" Target=\"%d\" R=\"%d\" G=\"%d\" B=\"%d\" A=\"%d\" />\n",target,r,g,b,a);
		std::string out(temp);
		return out;
	};

	void operator=(const Log_ObjectColorChange& in) 
	{
		in;
	};
public:
	~Log_ObjectColorChange(void);
};
