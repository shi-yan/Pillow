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
    Edge(int start = 0, int end = 0);
    EdgeInfo getEdgeInfo() const;

public:
    ~Edge(void) override;
};
