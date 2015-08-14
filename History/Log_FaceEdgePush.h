#pragma once
#include "HistoryLog.h"

class Log_FaceEdgePush :
	public HistoryLog
{
public:
    const unsigned int index;
public:
    Log_FaceEdgePush(unsigned int theTarget,unsigned int theIndex);
	std::string toString()
	{
		char temp[256];
        sprintf(temp,"\t\t\t<Log Type=\"FaceEdgePush\" Target=\"%d\" Index=\"%d\" />\n",target,index);
		std::string out(temp);
		return out;
    }
					void operator=(const Log_FaceEdgePush& in) 
	{
		in;
    }

public:
	~Log_FaceEdgePush(void);
};
