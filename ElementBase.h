#pragma once

//所有可以被选择以及细分和添加到IndexArray的元素基类
class ElementBase
{
public:
	//标记当前元素是否被选择
	bool isSelected;
	//标记当前元素是否在一个临时的数组内
	bool isIn;
	//标记当前的元素是否在局部细分中被处理
	bool isSub;
	//标记当前元素在IndexArray中的下标
	size_t index;

public:
	ElementBase(void);

public:
	virtual ~ElementBase(void);
};