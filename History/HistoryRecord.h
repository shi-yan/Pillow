#pragma once
#include <vector>
#include <string>
#include "HistoryLog.h"

class HistoryRecord
{
private:
	std::vector<HistoryLog *> record;
public:
	std::string name;
public:
	HistoryRecord(std::string theName):name(theName)
	{
		record.reserve(1000);
	};
	void push(HistoryLog *theRecord)
	{
		record.push_back(theRecord);
	};
	HistoryLog & pop()
	{
		HistoryLog *result=record[record.size()-1];
		return *result;
	};
	HistoryLog * operator[](size_t i)
	{
		return record[i];
	};
	size_t size()
	{
		return record.size();
	};

	std::string toString()
	{
		std::string out;
		out+="\t\t<Record name=\""+name+"\">\n";
		size_t logSize=record.size();
		for(size_t i=0;i<logSize;++i)
		{
			out+=record[i]->toString();
			//printf("%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%");
		}
		out+="\t\t</Record>\n";
		return out;
	};
	void clear()
	{
		size_t logSize=record.size();
		for(size_t i=0;i<logSize;++i)
		{
			record[i]->clear();
		}
	};
public:
	~HistoryRecord(void)
	{
		size_t logSize=record.size();
		for(size_t i=0;i<logSize;++i)
		{
			delete record[i];
		}
	};
};
