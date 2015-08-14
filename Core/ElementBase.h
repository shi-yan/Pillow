#pragma once

class ElementBase
{
public:
    bool m_isSelected;
    bool m_isIn;
    bool m_isSub;
    unsigned int m_index;

public:
	ElementBase(void);

public:
	virtual ~ElementBase(void);
};
