#pragma once
#include "SubdivideEdge.h"

class Edge : public SubdivideEdge
{
public:
    typedef struct
    {
        unsigned int index;
        unsigned int start;
        unsigned int end;
        unsigned int left;
        unsigned int right;
    } EdgeInfo;

    int m_clone;

public:
    Edge(int theStart = 0, int theEnd = 0);
    EdgeInfo getEdgeInfo() const;

public:
    ~Edge(void) override;
};
