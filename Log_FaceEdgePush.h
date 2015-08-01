#pragma once
#include "historylog.h"

class Log_FaceEdgePush :
	public HistoryLog
{
public:
	const size_t index;
public:
	Log_FaceEdgePush(size_t theTarget,size_t theIndex);
	std::string toString()
	{
		char temp[256];
		sprintf_s(temp,"\t\t\t<Log Type=\"FaceEdgePush\" Target=\"%d\" Index=\"%d\" />\n",target,index);
		std::string out(temp);
		return out;
	};
					void operator=(const Log_FaceEdgePush& in) 
	{
		in;
	};

public:
	~Log_FaceEdgePush(void);
};
