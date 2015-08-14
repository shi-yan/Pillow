#pragma once
#include "HistoryLog.h"

class Log_EdgeRightChange :
	public HistoryLog
{
public:
    const unsigned int index;
public:
    const unsigned int right;
public:
    Log_EdgeRightChange(unsigned int theTarget,unsigned int theIndex,unsigned int theRight);
	std::string toString()
	{
		char temp[256];
        sprintf(temp,"\t\t\t<Log Type=\"EdgeRightChange\" Target=\"%d\" Index=\"%d\" Right=\"%d\" />\n",target,index,right);
		std::string out(temp);
		return out;
	};

			void operator=(const Log_EdgeRightChange& in) 
	{
		in;
	};

public:
	~Log_EdgeRightChange(void);
};
