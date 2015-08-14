#pragma once
#include "HistoryLog.h"
#include "../Core/Vertex.h"

class Log_ObjectVertexRemove :
	public HistoryLog
{
public:
    const unsigned int m_index;
public:
	Vertex *o;
public:
    Log_ObjectVertexRemove(unsigned int theTarget,unsigned int theIndex,Vertex *theO);
	std::string toString()
	{
		char temp[256];
        sprintf(temp,"\t\t\t<Log Type=\"ObjectVertexRemove\" Target=\"%d\" Index=\"%d\" />\n",target,index);
		std::string out(temp);
		return out;
    }
		void clear()
	{
		delete o;
    }

		void operator=(const Log_ObjectVertexRemove& in) 
	{
		in;
    }
public:
	~Log_ObjectVertexRemove(void);
};
