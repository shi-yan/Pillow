#pragma once
#include "HistoryLog.h"
#include "Vertex.h"

class Log_ObjectVertexRemove :
	public HistoryLog
{
public:
	const size_t index;
public:
	Vertex *o;
public:
	Log_ObjectVertexRemove(size_t theTarget,size_t theIndex,Vertex *theO);
	std::string toString()
	{
		char temp[256];
        sprintf(temp,"\t\t\t<Log Type=\"ObjectVertexRemove\" Target=\"%d\" Index=\"%d\" />\n",target,index);
		std::string out(temp);
		return out;
	};
		void clear()
	{
		delete o;
	};

		void operator=(const Log_ObjectVertexRemove& in) 
	{
		in;
	};
public:
	~Log_ObjectVertexRemove(void);
};
