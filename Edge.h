#pragma once
#include "SubdivideEdge.h"

struct EdgeInfo
{
    unsigned int index;
    unsigned int start;
    unsigned int end;
    unsigned int left;
    unsigned int right;
};

class Edge : public SubdivideEdge
{
public:
	int clone;

public:
	Edge(void);
	Edge(int theStart,int theEnd);
	struct EdgeInfo getEdgeInfo();

public:
	~Edge(void);
};
