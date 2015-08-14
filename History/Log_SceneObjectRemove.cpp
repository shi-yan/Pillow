#include "Log_SceneObjectRemove.h"

Log_SceneObjectRemove::Log_SceneObjectRemove(unsigned int theTarget,Object *theO):HistoryLog(LogType::Scene_Object_Remove,theTarget),o(theO)
{
}

Log_SceneObjectRemove::~Log_SceneObjectRemove(void)
{
}
