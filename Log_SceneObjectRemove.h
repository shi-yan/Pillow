#pragma once
#include "HistoryLog.h"
#include "Object.h"

class Log_SceneObjectRemove :
	public HistoryLog
{
public:
	Object *o;
public:
	Log_SceneObjectRemove(size_t theTarget,Object *o);
	void clear()
	{
		delete o;
	};
public:
	~Log_SceneObjectRemove(void);
};
