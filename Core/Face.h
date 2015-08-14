#pragma once
#include <vector>
#include "Edge.h"
#include "SubdivideFace.h"

class Face : public ElementBase
{
	friend class Object;
public:
    typedef struct
    {
        unsigned int index;
        unsigned int edgeCount;
        float nx;
        float ny;
        float nz;
    } FaceInfo;

    std::vector<int> m_edge;
    Vector m_normal;

private:
    unsigned int m_center;
    std::vector<int> m_subdivideFace;

public:
	Face(void);
    int operator [](unsigned int i) const;
    FaceInfo getFaceInfo() const;

public:
    ~Face(void) override;
};
