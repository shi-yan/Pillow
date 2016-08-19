#pragma once
#include <iostream>
#include <deque>
#include "HistoryRecord.h"

class HistoryManager
{
private:
    std::deque<HistoryRecord *> m_undoList;
    std::vector<HistoryRecord *> m_redoList;
    bool m_recordLock;
    unsigned int m_capacity;
    HistoryRecord *m_currentRecord;
public:
    HistoryManager(unsigned int capacity=20)
        :m_capacity(capacity),
          m_recordLock(false),
          m_currentRecord(NULL)
    {
        m_redoList.reserve(capacity);
    }

    void recordBegin(const std::string &name)
    {
        if(!m_redoList.empty())
        {
            releaseRedo();
        }
        m_recordLock=true;
        m_currentRecord=new HistoryRecord(name);
        //currentRecord->reserve(1000);
        if(m_undoList.size()<m_capacity)
        {
            m_undoList.push_back(m_currentRecord);
        }
        else
        {
            releaseUndo(0);
            m_undoList.pop_front();
            m_undoList.push_back(m_currentRecord);
        }
    }

    void recordBeginR(const std::string &name)
    {
        m_recordLock=true;
        m_currentRecord=new HistoryRecord(name);
        m_undoList.push_back(m_currentRecord);
    }

    void recordEnd()
    {
        m_recordLock=false;
        m_currentRecord=NULL;
    }

    bool record(HistoryLog *log)
    {
        if(m_recordLock)
        {
            m_currentRecord->push(log);
            return true;
        }
        else
        {
            delete log;
            return false;
        }
    }

    HistoryRecord & undoBegin()
    {
        return *m_undoList[m_undoList.size()-1];
    }

    HistoryRecord & redoBegin()
    {
        return *m_redoList[m_redoList.size()-1];
    }

    void redoRecordBegin(const std::string &name)
    {
        m_recordLock=true;
        m_currentRecord=new HistoryRecord(name);
        m_redoList.push_back(m_currentRecord);
    }

    void redoRecordEnd()
    {
        m_recordLock=false;
        m_currentRecord=NULL;
    }

    void redoEnd()
    {
        delete m_redoList[m_redoList.size()-1];
        m_redoList.pop_back();
    }

    void undoEnd()
    {
        delete m_undoList[m_undoList.size()-1];
        m_undoList.pop_back();
    }

    void releaseRedo()
    {
        unsigned int redoSize=m_redoList.size();
        for(unsigned int i=0;i<redoSize;++i)
        {
            m_redoList[i]->clear();
            delete m_redoList[i];
        }
    }

    void releaseUndo()
    {
        unsigned int undoSize=m_undoList.size();
        for(unsigned int i=0;i<undoSize;++i)
        {
            m_undoList[i]->clear();
            delete m_undoList[i];
        }
    }

    void releaseUndo(unsigned int i)
    {
        m_undoList[i]->clear();
        delete m_undoList[i];
    }

    unsigned int undoSize()
    {
        return m_undoList.size();
    }

    unsigned int redoSize()
    {
        return  m_redoList.size();
    }

    void testOut()
    {
        std::cout<<"<History>"<<std::endl;
        unsigned int undoSize=m_undoList.size();
        if(undoSize)
        {
            std::cout<<"\t<Undo>"<<std::endl;
            for(unsigned int i=0;i<undoSize;i++)
            {
                std::cout<<m_undoList[i]->toString()<<std::endl;
            }
            std::cout<<"\t</UnDo>"<<std::endl;
        }
        unsigned int redoSize=m_redoList.size();
        if(redoSize)
        {
            std::cout<<"\t<Redo>"<<std::endl;
            for(unsigned int i=0;i<redoSize;i++)
            {
                std::cout<<m_redoList[i]->toString()<<std::endl;
            }
            std::cout<<"\t</Redo>"<<std::endl;
        }
        std::cout<<"</History>"<<std::endl;
    }

    void testOut(const char *fileName)
    {
        FILE *fp;
        fp = fopen(fileName,"w");
        fprintf(fp,"<History>\n");
        unsigned int undoSize=m_undoList.size();
        if(undoSize)
        {
            fprintf(fp,"\t<Undo>\n");
            for(unsigned int i=0;i<undoSize;i++)
            {
                fprintf(fp,"%s",m_undoList[i]->toString().c_str());
            }
            fprintf(fp,"\t</Undo>\n");
        }
        unsigned int redoSize=m_redoList.size();
        if(redoSize)
        {
            fprintf(fp,"\t<Redo>\n");
            for(unsigned int i=0;i<redoSize;i++)
            {
                fprintf(fp,m_redoList[i]->toString().c_str());
            }
            fprintf(fp,"\t</Redo>\n");
        }
        fprintf(fp,"</History>\n");
        fclose(fp);
    }

public:
    ~HistoryManager(void)
    {
        releaseRedo();
        releaseUndo();
    }
};

static HistoryManager *historyManager=new HistoryManager();
