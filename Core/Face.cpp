#include "Face.h"

//初始化无法线
Face::Face(void):ElementBase(),center(0),normal(0)
{
	//假定面的通常边数目小于5
	edge.reserve(5);
}

//返回指定的边列表元素
const int& Face::operator [](size_t i)
{
	return edge[i];
}

//得到面的信息,用于存储成文件
struct FaceInfo Face::getFaceInfo()
{
	struct FaceInfo result;
	result.index=index;
	result.edgeCount=edge.size();
	result.nx=normal.x;
	result.ny=normal.y;
	result.nz=normal.z;
	return result;
}

Face::~Face(void)
{
}