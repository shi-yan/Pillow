#pragma once
#include <iostream>
#include <deque>
#include "HistoryRecord.h"

class HistoryManager
{
private:
	std::deque<HistoryRecord *> undoList;
	std::vector<HistoryRecord *> redoList;
	bool recordLock;
    unsigned int capacity;
	HistoryRecord *currentRecord;
public:
    HistoryManager(unsigned int theCapacity=20):capacity(theCapacity),recordLock(false),currentRecord(NULL)
	{
		redoList.reserve(capacity);
    }

	void recordBegin(std::string theName)
	{
		if(!redoList.empty())
		{
			releaseRedo();
		}
		recordLock=true;
		currentRecord=new HistoryRecord(theName);
		//currentRecord->reserve(1000);
		if(undoList.size()<capacity)
		{
			undoList.push_back(currentRecord);
		}
		else
		{
			releaseUndo(0);
			undoList.pop_front();
			undoList.push_back(currentRecord);
		}
    }

	void recordBeginR(std::string &theName)
	{
		recordLock=true;
		currentRecord=new HistoryRecord(theName);
		undoList.push_back(currentRecord);
    }

	void recordEnd()
	{
		recordLock=false;
		currentRecord=NULL;
    }

	bool record(HistoryLog *theLog)
	{
		//delete theLog;
		if(recordLock)
		{
			currentRecord->push(theLog);
			return true;
		}
		else
		{
			delete theLog;
			return false;
		}
		//return false;
    }

	HistoryRecord & undoBegin()
	{
		return *undoList[undoList.size()-1];
    }

	HistoryRecord & redoBegin()
	{
		return *redoList[redoList.size()-1];
    }

	void redoRecordBegin(std::string &theName)
	{
		recordLock=true;
		currentRecord=new HistoryRecord(theName);
		redoList.push_back(currentRecord);
    }

	void redoRecordEnd()
	{
		recordLock=false;
		currentRecord=NULL;
    }

	void redoEnd()
	{
		delete redoList[redoList.size()-1];
		redoList.pop_back();
    }

	void undoEnd()
	{
		delete undoList[undoList.size()-1];
		undoList.pop_back();
    }

	void releaseRedo()
	{
        unsigned int redoSize=redoList.size();
        for(unsigned int i=0;i<redoSize;++i)
		{
			redoList[i]->clear();
			delete redoList[i];
		}
    }

	void releaseUndo()
	{
        unsigned int undoSize=undoList.size();
        for(unsigned int i=0;i<undoSize;++i)
		{
			undoList[i]->clear();
			delete undoList[i];
		}
    }

    void releaseUndo(unsigned int i)
	{
		undoList[i]->clear();
		delete undoList[i];
    }

    unsigned int undoSize()
	{
		return undoList.size();
    }

    unsigned int redoSize()
	{
		return  redoList.size();
    }

	void testOut()
	{
		std::cout<<"<History>"<<std::endl;
        unsigned int undoSize=undoList.size();
		if(undoSize)
		{
			std::cout<<"\t<Undo>"<<std::endl;
            for(unsigned int i=0;i<undoSize;i++)
			{
				std::cout<<undoList[i]->toString()<<std::endl;
			}
			std::cout<<"\t</UnDo>"<<std::endl;
		}
        unsigned int redoSize=redoList.size();
		if(redoSize)
		{
			std::cout<<"\t<Redo>"<<std::endl;
            for(unsigned int i=0;i<redoSize;i++)
			{
				std::cout<<redoList[i]->toString()<<std::endl;
			}
			std::cout<<"\t</Redo>"<<std::endl;
		}		
		std::cout<<"</History>"<<std::endl;
    }

	void testOut(char *fileName)
	{
		FILE *fp;
        fp = fopen(fileName,"w");
		fprintf(fp,"<History>\n");
        unsigned int undoSize=undoList.size();
		if(undoSize)
		{
			fprintf(fp,"\t<Undo>\n");
            for(unsigned int i=0;i<undoSize;i++)
			{
				fprintf(fp,"%s",undoList[i]->toString().c_str());
			}
			fprintf(fp,"\t</Undo>\n");
		}
        unsigned int redoSize=redoList.size();
		if(redoSize)
		{
			fprintf(fp,"\t<Redo>\n");
            for(unsigned int i=0;i<redoSize;i++)
			{
				fprintf(fp,redoList[i]->toString().c_str());
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
