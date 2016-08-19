#pragma once
#include <vector>
#include <string>
#include "HistoryLog.h"

class HistoryRecord
{
private:
    std::vector<HistoryLog *> m_record;
public:
    std::string m_name;
public:
    HistoryRecord(const std::string &name):m_name(name)
    {
        m_record.reserve(1000);
    }
    void push(HistoryLog *record)
    {
        m_record.push_back(record);
    }
    HistoryLog & pop()
    {
        HistoryLog *result=m_record[m_record.size()-1];
        return *result;
    }
    HistoryLog * operator[](unsigned int i)
    {
        return m_record[i];
    }
    unsigned int size()
    {
        return m_record.size();
    }

    std::string toString()
    {
        std::string out;
        out+="\t\t<Record name=\""+m_name+"\">\n";
        unsigned int logSize=m_record.size();
        for(unsigned int i=0;i<logSize;++i)
        {
            out+=m_record[i]->toString();
            //printf("%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%");
        }
        out+="\t\t</Record>\n";
        return out;
    }
    void clear()
    {
        unsigned int logSize=m_record.size();
        for(unsigned int i=0;i<logSize;++i)
        {
            m_record[i]->clear();
        }
    }
public:
    ~HistoryRecord(void)
    {
        unsigned int logSize=m_record.size();
        for(unsigned int i=0;i<logSize;++i)
        {
            delete m_record[i];
        }
    }
};
