#include "Face.h"

//³õÊ¼»¯ÎÞ·¨Ïß
Face::Face(void):ElementBase(),center(0),normal(0)
{
	//¼Ù¶¨ÃæµÄÍ¨³£±ßÊýÄ¿Ð¡ÓÚ5
    edge.clear();
	edge.reserve(5);
}

//·µ»ØÖ¸¶¨µÄ±ßÁÐ±íÔªËØ
const int& Face::operator [](unsigned int i)
{
	return edge[i];
}

//µÃµ½ÃæµÄÐÅÏ¢,ÓÃÓÚ´æ´¢³ÉÎÄ¼þ
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
