#pragma once
#include <vector>
#include "Edge.h"
#include "SubdivideFace.h"

//ÃæÀàµÄÊôÐÔ£¬ÓÃÓÚ´æ´¢³ÉÎÄ¼þ
struct FaceInfo
{
    unsigned int index;
    unsigned int edgeCount;
	float nx;
	float ny;
	float nz;
};

//ÃæÀà
class Face : public ElementBase
{
	//ÔÊÐíObjectÀà¶ÔÆäÖ±½Ó²Ù×÷
	friend class Object;
public:
	//¶¨ÒåÁËÃæµÄ±ßÁÐ±í£¬×óÊÖÏµË³Ê±ÕëË³ÐòÅÅÁÐ¡£Èç¹û±ßµÄ³¯ÏòºÍÃæµÄË³Ê±ÕëË³ÐòÒ»ÖÂÔòÎªÕý£¬·ñÔòÎª¸º
	//Ò²¾ÍÊÇ±ßÔÚÆä×ó²àµÄÃæÖÐÊÇ¸º£¬ÔÚÓÒ±ßµÄÃæÖÐÊÇÕý
	std::vector<int> edge;
	//¶¨ÒåÁËÃæµÄ·¨Ïß·½Ïò
	Vector normal;

private:
	//ÔÚÏÂÒ»²ãÏ¸·ÖÖÐ´ËÃæµÄÖÐµã±àºÅ
    unsigned int center;
	//Õâ¸öÃæÔÚÏÂÒ»²ãÏ¸·ÖÖÐÉú³ÉµÄËùÓÐ×ÓÃæ
	std::vector<int> subdivideFace;

public:
	Face(void);
	//µÃµ½Ö¸¶¨µÄ±ßÁÐ±íÖÐµÄÔªËØ
    const int& operator [](unsigned int i);
	//·µ»ØÕâ¸öÃæµÄËùÓÐÊôÐÔ£¬ÓÃÓÚ´æ´¢³ÉÎÄ¼þ
	struct FaceInfo getFaceInfo();

public:
	~Face(void);
};
