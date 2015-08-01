#pragma once
#include "historylog.h"

class Log_EdgeRightChange :
	public HistoryLog
{
public:
	const size_t index;
public:
	const size_t right;
public:
	Log_EdgeRightChange(size_t theTarget,size_t theIndex,size_t theRight);
	std::string toString()
	{
		char temp[256];
		sprintf_s(temp,"\t\t\t<Log Type=\"EdgeRightChange\" Target=\"%d\" Index=\"%d\" Right=\"%d\" />\n",target,index,right);
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