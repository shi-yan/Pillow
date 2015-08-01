#pragma once
#include "ElementBase.h"

class SubdivideEdge:public ElementBase
{
	friend class Object;
public:
	size_t start;
	size_t end;
	size_t left;
	size_t right;

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
	SubdivideEdge(size_t theStart,size_t theEnd);

public:
	~SubdivideEdge(void);
};
