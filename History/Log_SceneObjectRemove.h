#pragma once
#include "HistoryLog.h"
#include "../Core/Object.h"

class Log_SceneObjectRemove :
	public HistoryLog
{
public:
	Object *o;
public:
    Log_SceneObjectRemove(unsigned int theTarget,Object *o);
	void clear()
	{
		delete o;
	};
public:
	~Log_SceneObjectRemove(void);
};
