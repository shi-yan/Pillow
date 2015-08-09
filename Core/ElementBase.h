#pragma once

class ElementBase
{
public:
	bool isSelected;
	bool isIn;
	bool isSub;
    unsigned int index;

public:
	ElementBase(void);

public:
	virtual ~ElementBase(void);
};
