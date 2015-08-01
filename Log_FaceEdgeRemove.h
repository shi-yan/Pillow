#pragma once
#include "historylog.h"

class Log_FaceEdgeRemove :
	public HistoryLog
{
public:
	const size_t index;
public:
	const size_t edgeIndex;
	const int o;
public:
	Log_FaceEdgeRemove(size_t theTarget,size_t theIndex,size_t theEdgeIndex,int theO);
	std::string toString()
	{
		char temp[256];
		sprintf_s(temp,"\t\t\t<Log Type=\"FaceEdgeRemove\" Target=\"%d\" Index=\"%d\" EdgeIndex=\"%d\" O=\"%d\" />\n",target,index,edgeIndex,o);
		std::string out(temp);
		return out;
	};

						void operator=(const Log_FaceEdgeRemove& in) 
	{
		in;
	};
public:
	~Log_FaceEdgeRemove(void);
};
