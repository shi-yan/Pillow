#include "Console.h"
#include <stdarg.h>

Console::Console(void)
{
	this->capability=40;
}

Console::Console(unsigned int capability)
{
	this->capability=capability;
}

void Console::cprintf(const char *fmt, ...)
{
	static char temp[16384];
	va_list va;
	va_start( va, fmt );
	vsprintf( temp, fmt, va );
	va_end( va );
	std::string result(temp);
	if(this->theContext.size()>=this->capability)
	{
		this->theContext.pop_front();
	}
	this->theContext.push_back(result);
}

void Console::dprintf(const char *fmt, ...)
{
	static char temp[16384];
	va_list va;
	va_start( va, fmt );
	vsprintf( temp, fmt, va );
	va_end( va );
	std::string result(temp);
	if(this->theContext.size()>=this->capability)
	{
		this->theContext.pop_front();
	}
	this->theContext.push_back(result);
	if(this->dialogInstance)
	{
		this->dialogInstance->refresh();
	}
}

void Console::clear()
{
	for(size_t i=0;i<this->theContext.size();i++)
	{
//		delete theContext[i];
	}
	theContext.clear();
}

void executeCommand(char *)
{
}

Console::~Console(void)
{
	for(size_t i=0;i<this->theContext.size();i++)
	{
	}
}