#include "ElementBase.h"

//初始化为不选择，不细分，不再临时数组中，下标为0
ElementBase::ElementBase(void):isSelected(false),isSub(false),isIn(false),index(0)
{
}

ElementBase::~ElementBase(void)
{
}
