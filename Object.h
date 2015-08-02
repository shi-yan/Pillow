#pragma once

#include <GL/gl.h>
#include <GL/glu.h>
#include "SubdivideLevel.h"
#include "IndexArray.h"
#include "Vertex.h"
#include "Edge.h"
#include "Face.h"

#include <iostream>
#include <string>

#include "Log_VertexPositionChange.h"
#include "Log_VertexNormalChange.h"
#include "Log_VertexAdjacentPush.h"
#include "Log_VertexAdjacentChange.h"
#include "Log_VertexAdjacentPop.h"
#include "Log_VertexAdjacentSwap.h"
#include "Log_VertexAdjacentRemove.h"
#include "Log_VertexAdjacentInsert.h"

#include "Log_EdgeStartChange.h"
#include "Log_EdgeEndChange.h"
#include "Log_EdgeLeftChange.h"
#include "Log_EdgeRightChange.h"

#include "Log_FaceEdgePush.h"
#include "Log_FaceEdgeChange.h"
#include "Log_FaceEdgePop.h"
#include "Log_FaceEdgeSwap.h"
#include "Log_FaceEdgeRemove.h"
#include "Log_FaceEdgeInsert.h"

#include "Log_ObjectPositionChange.h"
#include "Log_ObjectRotationChange.h"
#include "Log_ObjectScaleChange.h"
#include "Log_ObjectColorChange.h"
#include "Log_ObjectVertexAdd.h"
#include "Log_ObjectVertexRemove.h"
#include "Log_ObjectEdgeAdd.h"
#include "Log_ObjectEdgeRemove.h"
#include "Log_ObjectFaceAdd.h"
#include "Log_ObjectFaceRemove.h"
#include "HistoryManager.h"

//历史记录管理器
extern HistoryManager *historyManager;

//渲染类型
DECLARE_ENUM(RenderType)
	//带线框平滑渲染
	Wireframe=234,
	//带线框非平滑渲染
	WireframeFaced,
	//线框渲染
	Wire,
	//无线框平滑渲染
	Smooth,
	//无线框非平滑渲染
	Faced
END_ENUM()

//镜像对称面
DECLARE_ENUM(MirrorMode)
	MirrorXY=345,
	MirrorYZ,
	MirrorXZ,
	Nothing
END_ENUM()

//物体类的属性,用于存储成文件
struct ObjectInfo
{
	size_t index;
	float centerX;
	float centerY;
	float centerZ;
	float positionX;
	float positionY;
	float positionZ;
	float rotationX;
	float rotationY;
	float rotationZ;
	float scaleX;
	float scaleY;
	float scaleZ;
	GLfloat mat_ambient[4];
	GLfloat mat_diffuse[4];
	GLfloat mat_specular[4];
	GLfloat mat_emission[4];
	GLfloat mat_shininess[4];
	size_t vertexCount;
	size_t edgeCount;
	size_t faceCount;
};

//定义物体的类
class Object : public ElementBase
{
public:
	//物体的名称
	std::string name;
	//物体的中心位置
	Vector center;
	//物体位置偏移量
	Vector position;
	//物体的旋转偏移量
	Vector rotation;
	//物体的缩放尺度
	Vector scale;
	//是否需要渲染镜像物体
	bool isMirror;
	//镜像物体的对称面
	MirrorMode theMirrorMode;
	//镜像物体相对原物体中心的位置
	Vector mirrorPosition;
	//是否被隐藏了
	bool isHide;
	//材质
	GLfloat mat_ambient[4];
	GLfloat mat_diffuse[4];
	GLfloat mat_specular[4];
	GLfloat mat_emission[4];
	GLfloat mat_shininess[4];
	//渲染模式
	RenderType renderMode;

private:
	//顶点列表
	IndexArray<Vertex> vertexArray;
	//边列表
	IndexArray<Edge> edgeArray;
	//面列表
	IndexArray<Face> faceArray;
	//局部细分面缓冲区
	std::vector<Face*> PSFaceCache;
	std::vector<SubdivideFace*> PSSubFaceCache[5];

private:
	//细分层次,限制在5层
	SubdivideLevel *subdivideLevel[5];
	//当前有细分层次数目
	int subdivideLevelSize;
	//当前的细分编号
	int subdivideId;

public:
	Object(char *theName);
	//增加新的顶点,返回添加的点编号
	size_t addVertex(float p1,float p2,float p3);
	size_t addVertex(float p1,float p2,float p3,float n1,float n2,float n3);
	size_t addVertex(Vector &pos);
	size_t addVertex(size_t ei,Vertex *theV);
	size_t addVertex(Vector &pos,Vector &nor);

	//增加新的边,返回添加的点编号
	size_t addEdge(int start,int end);
	//直接在指定的位置添加新的边
	size_t addEdge(size_t ei,Edge *theE);

	//得到指定位置的顶点
	Vertex * vertex(size_t index);
	//得到指定位置的边
	Edge * edge(size_t index);
	//得到指定位置的面
	Face * face(size_t index);

	//添加一个面,返回添加的面的编号
	size_t addFace(size_t theEdgeArray[],size_t size);
	//直接添加一个面
	size_t addFace(size_t ei,Face *theF);

	//将文件输出成obj格式,用于测试
	void testOut(char *fileName);
	//将指定的细分层次输出成obj格式,用于测试
	void SubTestOut(char *fileName,int level);
	//将最外层的细分层次输出
	void SubTestOut(char *fileName);
	//将当前物体细分
	void subdivide();
	//清空局部细分面缓冲区
	void clearPSCache();
	//通过局部细分面集来建立局部细分缓冲区
	void buildPSCache(std::vector<Face*> &faceToBeSub);
	//快速建立局部细分缓冲,精确性还需要探讨
	void buildPSCacheFast(std::vector<Face*> &faceToBeSub);
	//局部细分
	void partialSubdivision();
	//减少细分的层次
	void unSubdivide();

	//通过顶点编号集生成局部细分面缓冲
	void buildPSCacheFromVID(std::vector<size_t> &vertexToBeSub);
	//通过边编号集生成局部细分面缓冲
	void buildPSCacheFromEID(std::vector<size_t> &edgeToBeSub);
	//通过面编号集生成局部细分面缓冲
	void buildPSCacheFromFID(std::vector<size_t> &faceToBeSub);

	//显示镜像物体
	//type 为镜像面类型
	//theMirrorPosition 我镜像物体中心相对于原物体中心的位置
	void mirror(MirrorMode type,Vector &theMirrorPosition);
	//取消镜像物体
	void unMirror();
	//更新细分
	void updateSubdivision();

	//得到当前物体的属性,用于存储
	struct ObjectInfo getObjectInfo();
	//直接添加顶点,用于从文件中读取
	void directPushVertex(Vertex *theVertex);
	//直接添加边
	void directPushEdge(Edge *theEdge);
	//直接添加面
	void directPushFace(Face *theFace);
	
	//绘制线框非平滑物体
	void drawWireframeFaced();
	//为选取绘制物体
	void selectionRenderObject();
	//渲染线框平滑物体
	void drawWireframe();
	//渲染线框
	void drawWire(GLuint r=0,GLuint g=0,GLuint b=0);
	//渲染平滑物体
	void drawSmooth();
	//绘制物体的法线,用于测试
	void drawNormal();
	//绘制非平滑的物体
	void drawFaced();
	void drawWireEdgeSelected();
	void drawWireFaceSelected();
	void drawWireObjectSelected();
	void drawWireVertexSelected();
	//绘制非平滑边选择模式
	void drawFacedEdgeSelected();
	//绘制非平滑面选择模式
	void drawFacedFaceSelected();
	//绘制非平滑顶点选择模式
	void drawFacedVertexSelected();
	//绘制非平滑物体选择模式
	void drawFacedObjectSelected();
	//绘制平滑物体选择模式
	void drawSmoothObjectSelected();
	//绘制线框物体选择模式
//	void drawWireObjectSelected();
	//绘制物体选择模式
	void drawObjectSelected();
	//绘制物体的镜像
	void drawMirror();
	//绘制物体
	void draw();
	//将所有顶点的法线单位化
	void normalizeVertexNormal();
	//更新顶点的法线
	void updateVNormal(SubdivideVertex *theVertex,size_t level);
	//更新顶点的法线
	void updateVNormal(Vertex *theVertex);
	//更新面的法线
	void updateFNormal(Face *theFace);
	//更新面的法线
	void updateFNormal(SubdivideFace *theFace,size_t level);
	//更新所有的法线
	void updateAllNormal();
	//更新最外层细分面的法线
	void updateAllSubNormal();
	//得到边的数目
	size_t edgeCount();
	//得到顶点的数目
	size_t vertexCount();
	//得到面的数目
	size_t faceCount();
	//更新顶点位置到一个绝对值
	void vertexPositionChangeA(size_t vertexID,float nx,float ny,float nz);
	//更新顶点位置到一个相对值
	void vertexPositionChangeR(size_t vertexID,float nx,float ny,float nz);
	//更新顶点法线
	void vertexNormalChange(size_t vertexID,float nx,float ny,float nz);
	//删除面
	void objectFaceRemove(size_t faceID);
	//删除边
	void objectEdgeRemove(size_t edgeID);
	//删除顶点
	void objectVertexRemove(size_t vertexID);
	//改变边的右面
	void edgeRightChange(size_t edgeID,size_t nr);
	//改变边的左面
	void edgeLeftChange(size_t edgeID,size_t nl);
	//改变边的起始点
	void edgeStartChange(size_t edgeID,size_t ns);
	//改变边的终止点
	void edgeEndChange(size_t edgeID,size_t ne);
	//删除点的一个邻接边
	void vertexAdjacentRemove(size_t vertexID,size_t adjID);
	//插入点的一个邻接边
	void vertexAdjacentInsert(size_t vertexID,size_t adjID,size_t ne);
	//末尾插入一个面的边
	void faceEdgePush(size_t faceID,int nEdge);
	//弹出面的一个边
	void faceEdgePop(size_t faceID);
	//改变面的一个边
	void faceEdgeChange(size_t faceID,size_t edgeID,int nEdge);
	//插入一个面的边
	void faceEdgeInsert(size_t faceID,size_t edgeID,int nEdge);
	//插入一个顶点的邻接边
	void vertexAdjacentPush(size_t vertexID,size_t ne);
	//改变一个顶点的邻接边
	void vertexAdjacentChange(size_t vertexID,size_t edgeID,size_t ne);
	//弹出一个顶点的邻接边
	void vertexAdjacentPop(size_t vertexID);
	//交换一个顶点的邻接边
	void vertexAdjacentSwap(size_t vertexID,size_t i1,size_t i2);
	//交换一个面的邻接边
	void faceEdgeSwap(size_t faceID,size_t i1,size_t i2);
	//删除一个面的邻接边
	void faceEdgeRemove(size_t faceID,size_t edgeID);
	//输出测试xml
	void testXMLOut(char *fileName);
	//从新定义控制点
	void redefineControlPoint();

private:
	Vector EAdjacentVertex(Vertex *theVertex);
	Vector EAdjacentVertex(SubdivideVertex *theVertex,int level);
	Vector EAdjacentVertex(SubdivideVertex *theVertex);
	void subdivideFace(Face *theFace);
	void subdivideFace(SubdivideFace *theFace);
	void expandSubFace(std::vector<SubdivideFace*> &originalList,size_t level);
	void expandSubFace(std::vector<Face*> &originalList);
	void partialSubdivideFace(SubdivideFace *theFace,int level);
	void partialSubdivideFace(Face *theFace,int level);

public:
	~Object(void);
};
