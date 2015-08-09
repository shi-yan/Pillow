#pragma once
#include "HistoryLog.h"

class Log_ObjectRotationChange :
	public HistoryLog
{
public:
	const float x;
	const float y;
	const float z;
public:
	Log_ObjectRotationChange(size_t theTarget,float theX,float theY,float theZ);
	std::string toString()
	{
		char temp[256];
        sprintf(temp,"\t\t\t<Log Type=\"ObjectRotationChange\" Target=\"%d\" X=\"%f\" Y=\"%f\" Z=\"%f\" />\n",target,x,y,z);
		std::string out(temp);
		return out;
	};

								void operator=(const Log_ObjectRotationChange& in) 
	{
		in;
	};
public:
	~Log_ObjectRotationChange(void);
};
