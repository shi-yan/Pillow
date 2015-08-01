#pragma once
#include <deque>
#include <string>
#include <wx/dialog.h>
#include "ConsoleDialog.h"

class Console
{
private:
	std::deque<std::string> theContext;
	unsigned int capability;
	
public:
	ConsoleDialog *dialogInstance;
	Console(void);
	Console(unsigned int capability);
	void cprintf(const char *fmt, ...);
	void dprintf(const char *fmt, ...);
	void clear();
	void executeCommand(char *);
	void out()
	{
		for(size_t i=0;i<this->theContext.size();i++)
			printf("%s\n",this->theContext[i].c_str());
	}

	wxString outString()
	{
		wxString msg;
		for(size_t i=0;i<this->theContext.size();i++)
		{
			msg+=_T(this->theContext[i].c_str());
		}
		//msg<<std::endl;
		return msg;
	}

public:
	~Console(void);
};
