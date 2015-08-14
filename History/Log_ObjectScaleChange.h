#pragma once
#include "HistoryLog.h"

class Log_ObjectScaleChange :
	public HistoryLog
{
public:
	const float x;
	const float y;
	const float z;
public:
    Log_ObjectScaleChange(unsigned int theTarget,float theX,float theY,float theZ);
	std::string toString()
	{
		char temp[256];
        sprintf(temp,"\t\t\t<Log Type=\"ObjectScaleChange\" Target=\"%d\" X=\"%f\" Y=\"%f\" Z=\"%f\" />\n",target,x,y,z);
		std::string out(temp);
		return out;
    }

	void operator=(const Log_ObjectScaleChange& in) 
	{
		in;
    }
public:
	~Log_ObjectScaleChange(void);
};
