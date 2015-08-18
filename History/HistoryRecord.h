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
    }
    void push(HistoryLog *theRecord)
    {
        record.push_back(theRecord);
    }
    HistoryLog & pop()
    {
        HistoryLog *result=record[record.size()-1];
        return *result;
    }
    HistoryLog * operator[](unsigned int i)
    {
        return record[i];
    }
    unsigned int size()
    {
        return record.size();
    }

    std::string toString()
    {
        std::string out;
        out+="\t\t<Record name=\""+name+"\">\n";
        unsigned int logSize=record.size();
        for(unsigned int i=0;i<logSize;++i)
        {
            out+=record[i]->toString();
            //printf("%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%");
        }
        out+="\t\t</Record>\n";
        return out;
    }
    void clear()
    {
        unsigned int logSize=record.size();
        for(unsigned int i=0;i<logSize;++i)
        {
            record[i]->clear();
        }
    }
public:
    ~HistoryRecord(void)
    {
        unsigned int logSize=record.size();
        for(unsigned int i=0;i<logSize;++i)
        {
            delete record[i];
        }
    }
};
