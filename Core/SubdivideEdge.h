#pragma once
#include "ElementBase.h"

class SubdivideEdge : public ElementBase
{
    friend class Object;
public:
    unsigned int m_start;
    unsigned int m_end;
    unsigned int m_left;
    unsigned int m_right;

private:
    int m_subdivideId;
    int m_subdivideStep;
    int m_middle;
    int m_startEdge;
    int m_endEdge;
    int m_rightEdge;
    int m_leftEdge;

public:
    SubdivideEdge(unsigned int start = 0, unsigned int end = 0);

public:
    virtual ~SubdivideEdge(void) override;
};
