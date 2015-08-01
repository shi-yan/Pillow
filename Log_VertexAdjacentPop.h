#pragma once
#include "Vertex.h"
#include "historylog.h"

class Log_VertexAdjacentPop :
	public HistoryLog
{
public:
	const size_t index;
	const int o;

public:
	Log_VertexAdjacentPop(size_t theTarget,size_t theIndex,int o);
	std::string toString()
	{
		char temp[256];
		sprintf_s(temp,"\t\t\t<Log Type=\"VertexAdjacentPop\" Target=\"%d\" Index=\"%d\" O=\"%d\" />\n",target,index,o);
		std::string out(temp);
		return out;
	};

		void operator=(const Log_VertexAdjacentPop& in) 
	{
		in;
	};
public:
	~Log_VertexAdjacentPop(void);
};
