#pragma once
#include "Enum.h"
#include <string>

DECLARE_ENUM(LogType)

	Nothing=601,

	Vertex_Position_Change,//
	Vertex_Normal_Change,//
	Vertex_Adjacent_Push,//
	Vertex_Adjacent_Change,//
	Vertex_Adjacent_Pop,//
	Vertex_Adjacent_Swap,//
	Vertex_Adjacent_Remove,//
	Vertex_Adjacent_Insert,//

	Edge_Start_Change,//
	Edge_End_Change,//
	Edge_Left_Change,//
	Edge_Right_Change,//

	Face_Edge_Push,//
	Face_Edge_Change,//
	Face_Edge_Pop,//
	Face_Edge_Swap,//
	Face_Edge_Remove,//
	Face_Edge_Insert,//

	Object_Position_Change,//
	Object_Rotation_Change,//
	Object_Scale_Change,//
	Object_Color_Change,//
	Object_Vertex_Add,//
	Object_Vertex_Remove,//
	Object_Edge_Add,//
	Object_Edge_Remove,//
	Object_Face_Add,//
	Object_Face_Remove,//

	Scene_Object_Add,
	Scene_Object_Remove,
	Scene_Selection_Add,
	Scene_Selection_Remove,

END_ENUM()

class HistoryLog
{
public:
	const LogType type;
    const unsigned int target;
public:
    HistoryLog(LogType::__Enum theType,unsigned int theTarget);
	virtual std::string toString()
	{
		return "\t\t\t<Log />\n";
    }
	
	virtual void operator=(const HistoryLog& in) 
	{
		in;
    }
	virtual void clear()
    {}
public:
	virtual ~HistoryLog(void);
};
