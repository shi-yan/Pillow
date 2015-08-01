#pragma once
#include "SubdivideEdge.h"

//定义边的属性，用于存储成文件
struct EdgeInfo
{
	size_t index;
	size_t start;
	size_t end;
	size_t left;
	size_t right;
};

//边类
class Edge : public SubdivideEdge
{
public:
	//边的克隆边编号，用于生成这个边的克隆
	int clone;

public:
	Edge(void);
	//theStart边的起始点 theEnd边的终点
	//边的方向是从边的起始点指向终点
	Edge(int theStart,int theEnd);
	//得到边的所有属性，用于存储成文件
	struct EdgeInfo getEdgeInfo();

public:
	~Edge(void);
};