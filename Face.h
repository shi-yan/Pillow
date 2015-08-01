#pragma once
#include <vector>
#include "Edge.h"
#include "SubdivideFace.h"

//面类的属性，用于存储成文件
struct FaceInfo
{
	size_t index;
	size_t edgeCount;
	float nx;
	float ny;
	float nz;
};

//面类
class Face : public ElementBase
{
	//允许Object类对其直接操作
	friend class Object;
public:
	//定义了面的边列表，左手系顺时针顺序排列。如果边的朝向和面的顺时针顺序一致则为正，否则为负
	//也就是边在其左侧的面中是负，在右边的面中是正
	std::vector<int> edge;
	//定义了面的法线方向
	Vector normal;

private:
	//在下一层细分中此面的中点编号
	size_t center;
	//这个面在下一层细分中生成的所有子面
	std::vector<int> subdivideFace;

public:
	Face(void);
	//得到指定的边列表中的元素
	const int& operator [](size_t i);
	//返回这个面的所有属性，用于存储成文件
	struct FaceInfo getFaceInfo();

public:
	~Face(void);
};