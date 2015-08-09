#pragma once
#include "ElementBase.h"
#include "Vector.h"
#include <vector>

class SubdivideVertex : public ElementBase
{
friend class Object;
public:
	Vector position;
	Vector normal;
	std::vector<int> adjacentEdge;
	
private:
	int nextLevel;
	int subdivideId;
	int subdivideStep;
	bool edgeVertex;

public:
	SubdivideVertex(void);
	SubdivideVertex(Vector &pos,Vector &nor);
	SubdivideVertex(Vector &pos);
	SubdivideVertex(float p1,float p2,float p3);
	SubdivideVertex(float p1,float p2,float p3,float n1,float n2,float n3);
	const int& operator [](size_t i);

public:
	~SubdivideVertex(void);
};
