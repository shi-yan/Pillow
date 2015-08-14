#pragma once

#include <OpenGL/gl.h>
#include <OpenGL/glu.h>

#include "SubdivideLevel.h"
#include "IndexArray.h"
#include "Vertex.h"
#include "Edge.h"
#include "Face.h"

#include <iostream>
#include <string>

#include "History/Log_VertexPositionChange.h"
#include "History/Log_VertexNormalChange.h"
#include "History/Log_VertexAdjacentPush.h"
#include "History/Log_VertexAdjacentChange.h"
#include "History/Log_VertexAdjacentPop.h"
#include "History/Log_VertexAdjacentSwap.h"
#include "History/Log_VertexAdjacentRemove.h"
#include "History/Log_VertexAdjacentInsert.h"

#include "History/Log_EdgeStartChange.h"
#include "History/Log_EdgeEndChange.h"
#include "History/Log_EdgeLeftChange.h"
#include "History/Log_EdgeRightChange.h"

#include "History/Log_FaceEdgePush.h"
#include "History/Log_FaceEdgeChange.h"
#include "History/Log_FaceEdgePop.h"
#include "History/Log_FaceEdgeSwap.h"
#include "History/Log_FaceEdgeRemove.h"
#include "History/Log_FaceEdgeInsert.h"

#include "History/Log_ObjectPositionChange.h"
#include "History/Log_ObjectRotationChange.h"
#include "History/Log_ObjectScaleChange.h"
#include "History/Log_ObjectColorChange.h"
#include "History/Log_ObjectVertexAdd.h"
#include "History/Log_ObjectVertexRemove.h"
#include "History/Log_ObjectEdgeAdd.h"
#include "History/Log_ObjectEdgeRemove.h"
#include "History/Log_ObjectFaceAdd.h"
#include "History/Log_ObjectFaceRemove.h"
#include "History/HistoryManager.h"

extern HistoryManager *historyManager;

DECLARE_ENUM(RenderType)
	//´øÏß¿òÆ½»¬äÖÈ¾
	Wireframe=234,
	//´øÏß¿ò·ÇÆ½»¬äÖÈ¾
	WireframeFaced,
	//Ïß¿òäÖÈ¾
	Wire,
	//ÎÞÏß¿òÆ½»¬äÖÈ¾
	Smooth,
	//ÎÞÏß¿ò·ÇÆ½»¬äÖÈ¾
	Faced
END_ENUM()

//¾µÏñ¶Ô³ÆÃæ
DECLARE_ENUM(MirrorMode)
	MirrorXY=345,
	MirrorYZ,
	MirrorXZ,
	Nothing
END_ENUM()

//ÎïÌåÀàµÄÊôÐÔ,ÓÃÓÚ´æ´¢³ÉÎÄ¼þ
struct ObjectInfo
{
    unsigned int index;
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
    unsigned int vertexCount;
    unsigned int edgeCount;
    unsigned int faceCount;
};

//¶¨ÒåÎïÌåµÄÀà
class Object : public ElementBase
{
public:
	//ÎïÌåµÄÃû³Æ
	std::string name;
	//ÎïÌåµÄÖÐÐÄÎ»ÖÃ
	Vector center;
	//ÎïÌåÎ»ÖÃÆ«ÒÆÁ¿
	Vector position;
	//ÎïÌåµÄÐý×ªÆ«ÒÆÁ¿
	Vector rotation;
	//ÎïÌåµÄËõ·Å³ß¶È
	Vector scale;
	//ÊÇ·ñÐèÒªäÖÈ¾¾µÏñÎïÌå
	bool isMirror;
	//¾µÏñÎïÌåµÄ¶Ô³ÆÃæ
	MirrorMode theMirrorMode;
	//¾µÏñÎïÌåÏà¶ÔÔ­ÎïÌåÖÐÐÄµÄÎ»ÖÃ
	Vector mirrorPosition;
	//ÊÇ·ñ±»Òþ²ØÁË
	bool isHide;
	//²ÄÖÊ
	GLfloat mat_ambient[4];
	GLfloat mat_diffuse[4];
	GLfloat mat_specular[4];
	GLfloat mat_emission[4];
	GLfloat mat_shininess[4];
	//äÖÈ¾Ä£Ê½
	RenderType renderMode;

private:
	//¶¥µãÁÐ±í
	IndexArray<Vertex> vertexArray;
	//±ßÁÐ±í
	IndexArray<Edge> edgeArray;
	//ÃæÁÐ±í
	IndexArray<Face> faceArray;
	//¾Ö²¿Ï¸·ÖÃæ»º³åÇø
	std::vector<Face*> PSFaceCache;
	std::vector<SubdivideFace*> PSSubFaceCache[5];

private:
	//Ï¸·Ö²ã´Î,ÏÞÖÆÔÚ5²ã
	SubdivideLevel *subdivideLevel[5];
	//µ±Ç°ÓÐÏ¸·Ö²ã´ÎÊýÄ¿
	int subdivideLevelSize;
	//µ±Ç°µÄÏ¸·Ö±àºÅ
	int subdivideId;

public:
	Object(char *theName);
	//Ôö¼ÓÐÂµÄ¶¥µã,·µ»ØÌí¼ÓµÄµã±àºÅ
    unsigned int addVertex(float p1,float p2,float p3);
    unsigned int addVertex(float p1,float p2,float p3,float n1,float n2,float n3);
    unsigned int addVertex(Vector &pos);
    unsigned int addVertex(unsigned int ei,Vertex *theV);
    unsigned int addVertex(Vector &pos,Vector &nor);

	//Ôö¼ÓÐÂµÄ±ß,·µ»ØÌí¼ÓµÄµã±àºÅ
    unsigned int addEdge(int start,int end);
	//Ö±½ÓÔÚÖ¸¶¨µÄÎ»ÖÃÌí¼ÓÐÂµÄ±ß
    unsigned int addEdge(unsigned int ei,Edge *theE);

	//µÃµ½Ö¸¶¨Î»ÖÃµÄ¶¥µã
    Vertex * vertex(unsigned int index);
	//µÃµ½Ö¸¶¨Î»ÖÃµÄ±ß
    Edge * edge(unsigned int index);
	//µÃµ½Ö¸¶¨Î»ÖÃµÄÃæ
    Face * face(unsigned int index);

	//Ìí¼ÓÒ»¸öÃæ,·µ»ØÌí¼ÓµÄÃæµÄ±àºÅ
    unsigned int addFace(unsigned int theEdgeArray[],unsigned int size);
	//Ö±½ÓÌí¼ÓÒ»¸öÃæ
    unsigned int addFace(unsigned int ei,Face *theF);

	//½«ÎÄ¼þÊä³ö³Éobj¸ñÊ½,ÓÃÓÚ²âÊÔ
	void testOut(char *fileName);
	//½«Ö¸¶¨µÄÏ¸·Ö²ã´ÎÊä³ö³Éobj¸ñÊ½,ÓÃÓÚ²âÊÔ
	void SubTestOut(char *fileName,int level);
	//½«×îÍâ²ãµÄÏ¸·Ö²ã´ÎÊä³ö
	void SubTestOut(char *fileName);
	//½«µ±Ç°ÎïÌåÏ¸·Ö
	void subdivide();
	//Çå¿Õ¾Ö²¿Ï¸·ÖÃæ»º³åÇø
	void clearPSCache();
	//Í¨¹ý¾Ö²¿Ï¸·ÖÃæ¼¯À´½¨Á¢¾Ö²¿Ï¸·Ö»º³åÇø
	void buildPSCache(std::vector<Face*> &faceToBeSub);
	//¿ìËÙ½¨Á¢¾Ö²¿Ï¸·Ö»º³å,¾«È·ÐÔ»¹ÐèÒªÌ½ÌÖ
	void buildPSCacheFast(std::vector<Face*> &faceToBeSub);
	//¾Ö²¿Ï¸·Ö
	void partialSubdivision();
	//¼õÉÙÏ¸·ÖµÄ²ã´Î
	void unSubdivide();

	//Í¨¹ý¶¥µã±àºÅ¼¯Éú³É¾Ö²¿Ï¸·ÖÃæ»º³å
    void buildPSCacheFromVID(std::vector<unsigned int> &vertexToBeSub);
	//Í¨¹ý±ß±àºÅ¼¯Éú³É¾Ö²¿Ï¸·ÖÃæ»º³å
    void buildPSCacheFromEID(std::vector<unsigned int> &edgeToBeSub);
	//Í¨¹ýÃæ±àºÅ¼¯Éú³É¾Ö²¿Ï¸·ÖÃæ»º³å
    void buildPSCacheFromFID(std::vector<unsigned int> &faceToBeSub);

	//ÏÔÊ¾¾µÏñÎïÌå
	//type Îª¾µÏñÃæÀàÐÍ
	//theMirrorPosition ÎÒ¾µÏñÎïÌåÖÐÐÄÏà¶ÔÓÚÔ­ÎïÌåÖÐÐÄµÄÎ»ÖÃ
	void mirror(MirrorMode type,Vector &theMirrorPosition);
	//È¡Ïû¾µÏñÎïÌå
	void unMirror();
	//¸üÐÂÏ¸·Ö
	void updateSubdivision();

	//µÃµ½µ±Ç°ÎïÌåµÄÊôÐÔ,ÓÃÓÚ´æ´¢
	struct ObjectInfo getObjectInfo();
	//Ö±½ÓÌí¼Ó¶¥µã,ÓÃÓÚ´ÓÎÄ¼þÖÐ¶ÁÈ¡
	void directPushVertex(Vertex *theVertex);
	//Ö±½ÓÌí¼Ó±ß
	void directPushEdge(Edge *theEdge);
	//Ö±½ÓÌí¼ÓÃæ
	void directPushFace(Face *theFace);
	
	//»æÖÆÏß¿ò·ÇÆ½»¬ÎïÌå
	void drawWireframeFaced();
	//ÎªÑ¡È¡»æÖÆÎïÌå
	void selectionRenderObject();
	//äÖÈ¾Ïß¿òÆ½»¬ÎïÌå
	void drawWireframe();
	//äÖÈ¾Ïß¿ò
	void drawWire(GLuint r=0,GLuint g=0,GLuint b=0);
	//äÖÈ¾Æ½»¬ÎïÌå
	void drawSmooth();
	//»æÖÆÎïÌåµÄ·¨Ïß,ÓÃÓÚ²âÊÔ
	void drawNormal();
	//»æÖÆ·ÇÆ½»¬µÄÎïÌå
	void drawFaced();
	void drawWireEdgeSelected();
	void drawWireFaceSelected();
	void drawWireObjectSelected();
	void drawWireVertexSelected();
	//»æÖÆ·ÇÆ½»¬±ßÑ¡ÔñÄ£Ê½
	void drawFacedEdgeSelected();
	//»æÖÆ·ÇÆ½»¬ÃæÑ¡ÔñÄ£Ê½
	void drawFacedFaceSelected();
	//»æÖÆ·ÇÆ½»¬¶¥µãÑ¡ÔñÄ£Ê½
	void drawFacedVertexSelected();
	//»æÖÆ·ÇÆ½»¬ÎïÌåÑ¡ÔñÄ£Ê½
	void drawFacedObjectSelected();
	//»æÖÆÆ½»¬ÎïÌåÑ¡ÔñÄ£Ê½
	void drawSmoothObjectSelected();
	//»æÖÆÏß¿òÎïÌåÑ¡ÔñÄ£Ê½
//	void drawWireObjectSelected();
	//»æÖÆÎïÌåÑ¡ÔñÄ£Ê½
	void drawObjectSelected();
	//»æÖÆÎïÌåµÄ¾µÏñ
	void drawMirror();
	//»æÖÆÎïÌå
	void draw();
	//½«ËùÓÐ¶¥µãµÄ·¨Ïßµ¥Î»»¯
	void normalizeVertexNormal();
	//¸üÐÂ¶¥µãµÄ·¨Ïß
    void updateVNormal(SubdivideVertex *theVertex,unsigned int level);
	//¸üÐÂ¶¥µãµÄ·¨Ïß
	void updateVNormal(Vertex *theVertex);
	//¸üÐÂÃæµÄ·¨Ïß
	void updateFNormal(Face *theFace);
	//¸üÐÂÃæµÄ·¨Ïß
    void updateFNormal(SubdivideFace *theFace,unsigned int level);
	//¸üÐÂËùÓÐµÄ·¨Ïß
	void updateAllNormal();
	//¸üÐÂ×îÍâ²ãÏ¸·ÖÃæµÄ·¨Ïß
	void updateAllSubNormal();
	//µÃµ½±ßµÄÊýÄ¿
    unsigned int edgeCount();
	//µÃµ½¶¥µãµÄÊýÄ¿
    unsigned int vertexCount();
	//µÃµ½ÃæµÄÊýÄ¿
    unsigned int faceCount();
	//¸üÐÂ¶¥µãÎ»ÖÃµ½Ò»¸ö¾ø¶ÔÖµ
    void vertexPositionChangeA(unsigned int vertexID,float nx,float ny,float nz);
	//¸üÐÂ¶¥µãÎ»ÖÃµ½Ò»¸öÏà¶ÔÖµ
    void vertexPositionChangeR(unsigned int vertexID,float nx,float ny,float nz);
	//¸üÐÂ¶¥µã·¨Ïß
    void vertexNormalChange(unsigned int vertexID,float nx,float ny,float nz);
	//É¾³ýÃæ
    void objectFaceRemove(unsigned int faceID);
	//É¾³ý±ß
    void objectEdgeRemove(unsigned int edgeID);
	//É¾³ý¶¥µã
    void objectVertexRemove(unsigned int vertexID);
	//¸Ä±ä±ßµÄÓÒÃæ
    void edgeRightChange(unsigned int edgeID,unsigned int nr);
	//¸Ä±ä±ßµÄ×óÃæ
    void edgeLeftChange(unsigned int edgeID,unsigned int nl);
	//¸Ä±ä±ßµÄÆðÊ¼µã
    void edgeStartChange(unsigned int edgeID,unsigned int ns);
	//¸Ä±ä±ßµÄÖÕÖ¹µã
    void edgeEndChange(unsigned int edgeID,unsigned int ne);
	//É¾³ýµãµÄÒ»¸öÁÚ½Ó±ß
    void vertexAdjacentRemove(unsigned int vertexID,unsigned int adjID);
	//²åÈëµãµÄÒ»¸öÁÚ½Ó±ß
    void vertexAdjacentInsert(unsigned int vertexID,unsigned int adjID,unsigned int ne);
	//Ä©Î²²åÈëÒ»¸öÃæµÄ±ß
    void faceEdgePush(unsigned int faceID,int nEdge);
	//µ¯³öÃæµÄÒ»¸ö±ß
    void faceEdgePop(unsigned int faceID);
	//¸Ä±äÃæµÄÒ»¸ö±ß
    void faceEdgeChange(unsigned int faceID,unsigned int edgeID,int nEdge);
	//²åÈëÒ»¸öÃæµÄ±ß
    void faceEdgeInsert(unsigned int faceID,unsigned int edgeID,int nEdge);
	//²åÈëÒ»¸ö¶¥µãµÄÁÚ½Ó±ß
    void vertexAdjacentPush(unsigned int vertexID,unsigned int ne);
	//¸Ä±äÒ»¸ö¶¥µãµÄÁÚ½Ó±ß
    void vertexAdjacentChange(unsigned int vertexID,unsigned int edgeID,unsigned int ne);
	//µ¯³öÒ»¸ö¶¥µãµÄÁÚ½Ó±ß
    void vertexAdjacentPop(unsigned int vertexID);
	//½»»»Ò»¸ö¶¥µãµÄÁÚ½Ó±ß
    void vertexAdjacentSwap(unsigned int vertexID,unsigned int i1,unsigned int i2);
	//½»»»Ò»¸öÃæµÄÁÚ½Ó±ß
    void faceEdgeSwap(unsigned int faceID,unsigned int i1,unsigned int i2);
	//É¾³ýÒ»¸öÃæµÄÁÚ½Ó±ß
    void faceEdgeRemove(unsigned int faceID,unsigned int edgeID);
	//Êä³ö²âÊÔxml
	void testXMLOut(char *fileName);
	//´ÓÐÂ¶¨Òå¿ØÖÆµã
	void redefineControlPoint();

private:
	Vector EAdjacentVertex(Vertex *theVertex);
	Vector EAdjacentVertex(SubdivideVertex *theVertex,int level);
	Vector EAdjacentVertex(SubdivideVertex *theVertex);
	void subdivideFace(Face *theFace);
	void subdivideFace(SubdivideFace *theFace);
    void expandSubFace(std::vector<SubdivideFace*> &originalList,unsigned int level);
	void expandSubFace(std::vector<Face*> &originalList);
	void partialSubdivideFace(SubdivideFace *theFace,int level);
	void partialSubdivideFace(Face *theFace,int level);

public:
	~Object(void);
};
