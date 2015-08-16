#pragma once

#ifdef __APPLE__
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#endif
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
	Wireframe=234,
	WireframeFaced,
	Wire,
	Smooth,
	Faced
END_ENUM()

DECLARE_ENUM(MirrorMode)
	MirrorXY=345,
	MirrorYZ,
	MirrorXZ,
	Nothing
END_ENUM()

class Object : public ElementBase
{
public:
    typedef struct
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
        GLfloat matAmbient[4];
        GLfloat matDiffuse[4];
        GLfloat matSpecular[4];
        GLfloat matEmission[4];
        GLfloat matShininess[4];
        unsigned int vertexCount;
        unsigned int edgeCount;
        unsigned int faceCount;
    } ObjectInfo;
    std::string m_name;
    Vector m_center;
    Vector m_position;
    Vector m_rotation;
    Vector m_scale;
    bool m_isMirror;
    MirrorMode m_mirrorMode;
    Vector m_mirrorPosition;
    bool m_isHide;
    GLfloat m_matAmbient[4];
    GLfloat m_matDiffuse[4];
    GLfloat m_matSpecular[4];
    GLfloat m_matEmission[4];
    GLfloat m_matShininess[4];
    RenderType m_renderMode;

private:
    IndexArray<Vertex> m_vertexArray;
    IndexArray<Edge> m_edgeArray;
    IndexArray<Face> m_faceArray;
    std::vector<Face*> m_PSFaceCache;
    std::vector<SubdivideFace*> m_PSSubFaceCache[5];

private:
    SubdivideLevel *m_subdivideLevel[5];
    int m_subdivideLevelSize;
    int m_subdivideId;

public:
	Object(char *theName);
    unsigned int addVertex(float p1,float p2,float p3);
    unsigned int addVertex(float p1,float p2,float p3,float n1,float n2,float n3);
    unsigned int addVertex(Vector &pos);
    unsigned int addVertex(unsigned int ei,Vertex *theV);
    unsigned int addVertex(Vector &pos,Vector &nor);

    unsigned int addEdge(int start,int end);
    unsigned int addEdge(unsigned int ei,Edge *theE);

    Vertex * vertex(unsigned int index);
    Edge * edge(unsigned int index);
    Face * face(unsigned int index);

    unsigned int addFace(unsigned int theEdgeArray[],unsigned int size);
    unsigned int addFace(unsigned int ei,Face *theF);

	void testOut(char *fileName);
	void SubTestOut(char *fileName,int level);
	void SubTestOut(char *fileName);
	void subdivide();
	void clearPSCache();
	void buildPSCache(std::vector<Face*> &faceToBeSub);
	void buildPSCacheFast(std::vector<Face*> &faceToBeSub);
	void partialSubdivision();
	void unSubdivide();

    void buildPSCacheFromVID(std::vector<unsigned int> &vertexToBeSub);
    void buildPSCacheFromEID(std::vector<unsigned int> &edgeToBeSub);
    void buildPSCacheFromFID(std::vector<unsigned int> &faceToBeSub);

	void mirror(MirrorMode type,Vector &theMirrorPosition);
	void unMirror();
	void updateSubdivision();

    ObjectInfo getObjectInfo();
	void directPushVertex(Vertex *theVertex);
	void directPushEdge(Edge *theEdge);
	void directPushFace(Face *theFace);
	
	void drawWireframeFaced();
	void selectionRenderObject();
	void drawWireframe();
	void drawWire(GLuint r=0,GLuint g=0,GLuint b=0);
	void drawSmooth();
	void drawNormal();
	void drawFaced();
	void drawWireEdgeSelected();
	void drawWireFaceSelected();
	void drawWireObjectSelected();
	void drawWireVertexSelected();
	void drawFacedEdgeSelected();
	void drawFacedFaceSelected();
	void drawFacedVertexSelected();
	void drawFacedObjectSelected();
	void drawSmoothObjectSelected();
//	void drawWireObjectSelected();
	void drawObjectSelected();
	void drawMirror();
	void draw();
	void normalizeVertexNormal();
    void updateVNormal(SubdivideVertex *theVertex,unsigned int level);
	void updateVNormal(Vertex *theVertex);
	void updateFNormal(Face *theFace);
    void updateFNormal(SubdivideFace *theFace,unsigned int level);
	void updateAllNormal();
	void updateAllSubNormal();
    unsigned int edgeCount();
    unsigned int vertexCount();
    unsigned int faceCount();
    void vertexPositionChangeA(unsigned int vertexID,float nx,float ny,float nz);
    void vertexPositionChangeR(unsigned int vertexID,float nx,float ny,float nz);
    void vertexNormalChange(unsigned int vertexID,float nx,float ny,float nz);
    void objectFaceRemove(unsigned int faceID);
    void objectEdgeRemove(unsigned int edgeID);
    void objectVertexRemove(unsigned int vertexID);
    void edgeRightChange(unsigned int edgeID,unsigned int nr);
    void edgeLeftChange(unsigned int edgeID,unsigned int nl);
    void edgeStartChange(unsigned int edgeID,unsigned int ns);
    void edgeEndChange(unsigned int edgeID,unsigned int ne);
    void vertexAdjacentRemove(unsigned int vertexID,unsigned int adjID);
    void vertexAdjacentInsert(unsigned int vertexID,unsigned int adjID,unsigned int ne);
    void faceEdgePush(unsigned int faceID,int nEdge);
    void faceEdgePop(unsigned int faceID);
    void faceEdgeChange(unsigned int faceID,unsigned int edgeID,int nEdge);
    void faceEdgeInsert(unsigned int faceID,unsigned int edgeID,int nEdge);
    void vertexAdjacentPush(unsigned int vertexID,unsigned int ne);
    void vertexAdjacentChange(unsigned int vertexID,unsigned int edgeID,unsigned int ne);
    void vertexAdjacentPop(unsigned int vertexID);
    void vertexAdjacentSwap(unsigned int vertexID,unsigned int i1,unsigned int i2);
    void faceEdgeSwap(unsigned int faceID,unsigned int i1,unsigned int i2);
    void faceEdgeRemove(unsigned int faceID,unsigned int edgeID);
	void testXMLOut(char *fileName);
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
    ~Object(void) override;
};
