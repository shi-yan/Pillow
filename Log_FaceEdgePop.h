#pragma once
#include "historylog.h"

class Log_FaceEdgePop :
	public HistoryLog
{
public:
	const size_t index;
public:
	const int o;
public:
	Log_FaceEdgePop(size_t theTarget,size_t theIndex,int theO);
	std::string toString()
	{
		char temp[256];
		sprintf_s(temp,"\t\t\t<Log Type=\"FaceEdgePop\" Target=\"%d\" Index=\"%d\" O=\"%d\" />\n",target,index,o);
		std::string out(temp);
		return out;
	};
					void operator=(const Log_FaceEdgePop& in) 
	{
		in;
	};
public:
	~Log_FaceEdgePop(void);
};
