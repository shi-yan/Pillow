#pragma once
#include "ElementBase.h"

class SubdivideEdge:public ElementBase
{
	friend class Object;
public:
    unsigned int start;
    unsigned int end;
    unsigned int left;
    unsigned int right;

private:
	int subdivideId;
	int subdivideStep;
	int middle;
	int startEdge;
	int endEdge;
	int rightEdge;
	int leftEdge;

public:
	SubdivideEdge(void);
    SubdivideEdge(unsigned int theStart,unsigned int theEnd);

public:
	~SubdivideEdge(void);
};
