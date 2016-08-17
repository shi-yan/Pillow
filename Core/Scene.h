#pragma once
#pragma warning(disable : 4996)

#include "Enum.h"
#include "Core/IndexArray.h"
#include "Core/Object.h"
#include "History/Log_SceneObjectAdd.h"
#include "History/Log_SceneObjectRemove.h"
#include "UI/AxisCursor.h"
#include <fstream>
#include "Core/Matrix.h"
#include "Core/Quaternion.h"

#include <QImage>
#include <QDebug>

DECLARE_ENUM(SelectionMode)
    Object=234,
    Face,
    Edge,
    Vertex,
    Split,
    Weld
END_ENUM()

DECLARE_ENUM(AxisMode)
    axisX=678,
    axisY,
    axisZ
END_ENUM()

struct SelectionResult
{
    GLuint size;
    GLuint zMini;
    GLuint zMax;
    GLuint id;
};

struct ColorID
{
    unsigned char r;
    unsigned char g;
    unsigned char b;
    unsigned char a;
};

struct SceneInfo
{
    char ID[4];
    int version;
    unsigned int objectCount;
};

class Scene
{
private:
    OpenGLBackend *m_backend;
    std::vector<ElementBase *> m_isInCache;
    AxisCursor *m_axisCursor;
    AxisCursorMode m_currentACMode;
    IndexArray<Object> m_objectList;
    std::vector<unsigned int> m_selection;
    unsigned int m_target;
    SelectionMode m_mode;

public:
    std::string m_fileName;
    unsigned int m_splitVertexID;
    bool m_isSplitMode;
    //unsigned int splitVX;
    //unsigned int splitVY;

    Scene(OpenGLBackend *backend);
    unsigned int newCube(float x,float y,float z,float lx,float ly,float lz,unsigned int sx,unsigned int sy,unsigned int sz);
    void newCube(Object *cube,float x,float y,float z,float lx,float ly,float lz,unsigned int sx,unsigned int sy,unsigned int sz);
    unsigned int newCylinder(float x,float y,float z,float r,float h,AxisMode::__Enum axis,unsigned int sa,unsigned int sr,unsigned int sh);
    void newCylinder(Object *cylinder,float x,float y,float z,float r,float h,AxisMode::__Enum axis,unsigned int sa,unsigned int sr,unsigned int sh);
    unsigned int newSphere(float x,float y,float z,float r,AxisMode::__Enum axis,unsigned int sa,unsigned int sr);
    void newSphere(Object *sphere,float x,float y,float z,float r,AxisMode::__Enum axis,unsigned int sa,unsigned int sr);
    void moveVertex(float x,float y,float z);
    void moveEdge(float x,float y,float z);
    void moveFace(float x,float y,float z);
    unsigned int newPlane(float x,float y,float z,float length,float width,AxisMode::__Enum axis,unsigned int sl,unsigned int sw);
    void newPlane(Object *plane,float x,float y,float z,float length,float width,AxisMode::__Enum axis,unsigned int sl,unsigned int sw);
    void deleteEdgeH(unsigned int t,unsigned int edgeID);
    void deleteVertex();
    void deleteFace();
    void extrudeEdge(float x,float y,float z);
    unsigned int insertVertex(int edgeID,float pos);
    void split(int startV,int endV);
    void removeEdge();
    void clearIsInCache();
    void targetWeldVertex(unsigned int vertexA,unsigned int vertexB);
    void extrudeFaceGroup(float x,float y,float z);
    void sceneObjectAdd(char *name);
    void sceneObjectAdd(Object *newObject);
    void detach(Object *newObject);
    void attach(unsigned int original,unsigned int toBeAttached);
    void clearSelection();
    void initialize();
    void changeAxisCursorMode(AxisCursorMode::__Enum newMode);
    void clearDualEdge(unsigned int tt,unsigned int vertexID);

    void mirrorClone()
    {
        if(m_mode==SelectionMode::Object)
        {
            for(unsigned int i=0;i<m_selection.size();++i)
            {
                Object *original=m_objectList[m_selection[i]];
                if(original)
                {
                    if(original->m_isMirror && original->m_mirrorMode!=MirrorMode::Nothing)
                    {
                        Matrix transform;
                        /*glMatrixMode(GL_MODELVIEW);
                        glPushMatrix();
                        glLoadIdentity();
                        if(original->m_mirrorm_mode==MirrorMode::MirrorXY)
                        {
                            glScalef(1,1,-1);
                        }
                        else if(original->m_mirrorm_mode==MirrorMode::MirrorYZ)
                        {
                            glScalef(-1,1,1);
                        }
                        else if(original->m_mirrorm_mode==MirrorMode::MirrorXZ)
                        {
                            glScalef(1,-1,1);
                        }
                        glGetFloatv(GL_MODELVIEW_MATRIX,(GLfloat*)(&transform.m));
                        glPopMatrix();
                        Object *newObject=new Object("Cloned");
                        for(unsigned int e=1;e<original->vertexCount();++e)
                        {
                            Vertex *vertex=original->vertex(e);
                            if(vertex)
                            {
                                Vector newVertex=transform*(vertex->m_position);
                                vertex->m_clone=newObject->addVertex(newVertex);
                            }
                        }
                        for(unsigned int e=1;e<original->edgeCount();++e)
                        {
                            Edge *edge=original->edge(e);
                            if(edge)
                            {
                                edge->m_clone=newObject->addEdge(original->vertex(edge->m_start)->m_clone,original->vertex(edge->m_end)->m_clone);
                            }
                        }
                        for(unsigned int e=1;e<original->faceCount();++e)
                        {
                            Face *face=original->face(e);
                            if(face)
                            {
                                unsigned int *tempEdgeSet=new unsigned int[face->m_edge.size()];
                                for(unsigned int h=0;h<face->m_edge.size();++h)
                                {
                                    tempEdgeSet[h]=face->m_edge[h]>0?face->m_edge[h]:-face->m_edge[h];
                                }
                                newObject->addFace(tempEdgeSet,face->m_edge.size());
                                delete tempEdgeSet;
                            }
                        }
                        sceneObjectAdd(newObject);*/
                    }
                }
            }
        }
    }

    void clone()
    {
        if(m_mode==SelectionMode::Object)
        {
            for(unsigned int i=0;i<m_selection.size();++i)
            {
                Object *original=m_objectList[m_selection[i]];
                if(original)
                {
                    Object *newObject=new Object("Cloned");
                    for(unsigned int e=1;e<original->vertexCount();++e)
                    {
                        Vertex *vertex=original->vertex(e);
                        if(vertex)
                        {
                            vertex->m_clone=newObject->addVertex(vertex->m_position);
                        }
                    }

                    for(unsigned int e=1;e<original->edgeCount();++e)
                    {
                        Edge *edge=original->edge(e);
                        if(edge)
                        {
                            edge->m_clone=newObject->addEdge(original->vertex(edge->m_start)->m_clone,original->vertex(edge->m_end)->m_clone);
                        }
                    }

                    for(unsigned int e=1;e<original->faceCount();++e)
                    {
                        Face *face=original->face(e);
                        if(face)
                        {
                            unsigned int *tempEdgeSet=new unsigned int[face->m_edge.size()];
                            for(unsigned int h=0;h<face->m_edge.size();++h)
                            {
                                tempEdgeSet[h]=face->m_edge[h]>0?face->m_edge[h]:-face->m_edge[h];
                            }
                            newObject->addFace(tempEdgeSet,face->m_edge.size());
                            delete tempEdgeSet;
                        }
                    }
                    sceneObjectAdd(newObject);
                }
            }
        }
    }

    void getSceneInfo(unsigned int &objectCount,unsigned int &vertexCount,unsigned int &edgeCount,unsigned int &faceCount)
    {
        objectCount=m_objectList.size();
        for(unsigned int i=1;i<objectCount;++i)
        {
            if(m_objectList[i])
            {
                vertexCount+=m_objectList[i]->vertexCount()-1;
                edgeCount+=m_objectList[i]->edgeCount()-1;
                faceCount+=m_objectList[i]->faceCount()-1;
            }
        }
        --objectCount;
    }

    void onMerge()
    {
        if(m_mode==SelectionMode::Object)
        {
            for(unsigned int i=1;i<m_selection.size();++i)
            {
                attach(m_selection[0],m_selection[i]);
            }
            clearSelection();
        }
    }

    void onDetach()
    {
        Object *newObject=new Object("Detached");
        if(m_mode==SelectionMode::Face && m_target>0)
        {
            detach(newObject);
        }
        clearSelection();
    }

    void onMirror(MirrorMode type,float x,float y,float z)
    {
        Vector mirrorPosition(x,y,z);
        if(m_mode==SelectionMode::Object)
        {
            for(unsigned int i=0;i<m_selection.size();++i)
            {
                if(m_objectList[m_selection[i]])
                {
                    m_objectList[m_selection[i]]->mirror(type,mirrorPosition);
                }
            }
        }
        else if(m_target)
        {
            if(m_objectList[m_target])
            {
                m_objectList[m_target]->mirror(type,mirrorPosition);
            }
        }
    }

    void unMirror()
    {
        if(m_mode==SelectionMode::Object)
        {
            for(unsigned int i=0;i<m_selection.size();++i)
            {
                if(m_objectList[m_selection[i]])
                {
                    m_objectList[m_selection[i]]->unMirror();
                }
            }
        }
        else if(m_target)
        {
            if(m_objectList[m_target])
            {
                m_objectList[m_target]->unMirror();
            }
        }
    }

    void setSelectionMaterial(float ambient[4],float diffuse[4],float specular[4],float emission[4],float shininess)
    {
        if(m_mode==SelectionMode::Object)
        {
            if(m_selection.size()==1)
            {
                if(m_objectList[m_selection[0]])
                {
                    m_objectList[m_selection[0]]->m_matAmbient[0]=ambient[0];
                    m_objectList[m_selection[0]]->m_matAmbient[1]=ambient[1];
                    m_objectList[m_selection[0]]->m_matAmbient[2]=ambient[2];
                    m_objectList[m_selection[0]]->m_matAmbient[3]=ambient[3];
                    m_objectList[m_selection[0]]->m_matDiffuse[0]=diffuse[0];
                    m_objectList[m_selection[0]]->m_matDiffuse[1]=diffuse[1];
                    m_objectList[m_selection[0]]->m_matDiffuse[2]=diffuse[2];
                    m_objectList[m_selection[0]]->m_matDiffuse[3]=diffuse[3];
                    m_objectList[m_selection[0]]->m_matSpecular[0]=specular[0];
                    m_objectList[m_selection[0]]->m_matSpecular[1]=specular[1];
                    m_objectList[m_selection[0]]->m_matSpecular[2]=specular[2];
                    m_objectList[m_selection[0]]->m_matSpecular[3]=specular[3];
                    m_objectList[m_selection[0]]->m_matEmission[0]=emission[0];
                    m_objectList[m_selection[0]]->m_matEmission[1]=emission[1];
                    m_objectList[m_selection[0]]->m_matEmission[2]=emission[2];
                    m_objectList[m_selection[0]]->m_matEmission[3]=emission[3];
                    m_objectList[m_selection[0]]->m_matShininess[0]=shininess;
                    m_objectList[m_selection[0]]->m_matShininess[1]=shininess;
                    m_objectList[m_selection[0]]->m_matShininess[2]=shininess;
                    m_objectList[m_selection[0]]->m_matShininess[3]=shininess;
                }
            }
        }
        else if(m_target>0 && m_objectList[m_target])
        {
            m_objectList[m_target]->m_matAmbient[0]=ambient[0];
            m_objectList[m_target]->m_matAmbient[1]=ambient[1];
            m_objectList[m_target]->m_matAmbient[2]=ambient[2];
            m_objectList[m_target]->m_matAmbient[3]=ambient[3];
            m_objectList[m_target]->m_matDiffuse[0]=diffuse[0];
            m_objectList[m_target]->m_matDiffuse[1]=diffuse[1];
            m_objectList[m_target]->m_matDiffuse[2]=diffuse[2];
            m_objectList[m_target]->m_matDiffuse[3]=diffuse[3];
            m_objectList[m_target]->m_matSpecular[0]=specular[0];
            m_objectList[m_target]->m_matSpecular[1]=specular[1];
            m_objectList[m_target]->m_matSpecular[2]=specular[2];
            m_objectList[m_target]->m_matSpecular[3]=specular[3];
            m_objectList[m_target]->m_matEmission[0]=emission[0];
            m_objectList[m_target]->m_matEmission[1]=emission[1];
            m_objectList[m_target]->m_matEmission[2]=emission[2];
            m_objectList[m_target]->m_matEmission[3]=emission[3];
            m_objectList[m_target]->m_matShininess[0]=shininess;
            m_objectList[m_target]->m_matShininess[1]=shininess;
            m_objectList[m_target]->m_matShininess[2]=shininess;
            m_objectList[m_target]->m_matShininess[3]=shininess;
        }

    }

    void getSelectionMaterial(float *ambient,float *diffuse,float *specular,float *emission,float *shininess)
    {
        if(m_mode==SelectionMode::Object)
        {
            if(m_selection.size()==1)
            {
                if(m_objectList[m_selection[0]])
                {
                    ambient[0]=m_objectList[m_selection[0]]->m_matAmbient[0];
                    ambient[1]=m_objectList[m_selection[0]]->m_matAmbient[1];
                    ambient[2]=m_objectList[m_selection[0]]->m_matAmbient[2];
                    ambient[3]=m_objectList[m_selection[0]]->m_matAmbient[3];
                    diffuse[0]=m_objectList[m_selection[0]]->m_matDiffuse[0];
                    diffuse[1]=m_objectList[m_selection[0]]->m_matDiffuse[1];
                    diffuse[2]=m_objectList[m_selection[0]]->m_matDiffuse[2];
                    diffuse[3]=m_objectList[m_selection[0]]->m_matDiffuse[3];
                    specular[0]=m_objectList[m_selection[0]]->m_matSpecular[0];
                    specular[1]=m_objectList[m_selection[0]]->m_matSpecular[1];
                    specular[2]=m_objectList[m_selection[0]]->m_matSpecular[2];
                    specular[3]=m_objectList[m_selection[0]]->m_matSpecular[3];
                    emission[0]=m_objectList[m_selection[0]]->m_matEmission[0];
                    emission[1]=m_objectList[m_selection[0]]->m_matEmission[1];
                    emission[2]=m_objectList[m_selection[0]]->m_matEmission[2];
                    emission[3]=m_objectList[m_selection[0]]->m_matEmission[3];
                    (*shininess)=*(m_objectList[m_selection[0]]->m_matShininess);
                }
            }
        }
        else if(m_target>0 && m_objectList[m_target])
        {
            ambient[0]=m_objectList[m_target]->m_matAmbient[0];
            ambient[1]=m_objectList[m_target]->m_matAmbient[1];
            ambient[2]=m_objectList[m_target]->m_matAmbient[2];
            ambient[3]=m_objectList[m_target]->m_matAmbient[3];
            diffuse[0]=m_objectList[m_target]->m_matDiffuse[0];
            diffuse[1]=m_objectList[m_target]->m_matDiffuse[1];
            diffuse[2]=m_objectList[m_target]->m_matDiffuse[2];
            diffuse[3]=m_objectList[m_target]->m_matDiffuse[3];
            specular[0]=m_objectList[m_target]->m_matSpecular[0];
            specular[1]=m_objectList[m_target]->m_matSpecular[1];
            specular[2]=m_objectList[m_target]->m_matSpecular[2];
            specular[3]=m_objectList[m_target]->m_matSpecular[3];
            emission[0]=m_objectList[m_target]->m_matEmission[0];
            emission[1]=m_objectList[m_target]->m_matEmission[1];
            emission[2]=m_objectList[m_target]->m_matEmission[2];
            emission[3]=m_objectList[m_target]->m_matEmission[3];
            (*shininess)=*(m_objectList[m_target]->m_matShininess);
        }
    }

    void updateNormal()
    {
        if(m_mode==SelectionMode::Object)
        {
            for(unsigned int i=0;i<m_selection.size();++i)
            {
                if(m_objectList[m_selection[i]])
                {
                    m_objectList[m_selection[i]]->updateAllNormal();
                }
            }
        }
        else if(m_target>0 && m_objectList[m_target])
        {
            m_objectList[m_target]->updateAllNormal();
        }
    }

    void onDelete()
    {
        if(m_mode==SelectionMode::Object && m_target==0)
        {
            for(unsigned int i=0;i<m_selection.size();++i)
            {
                sceneObjectRemove(m_selection[i]);
            }
            clearSelection();
        }
        else if(m_target>0)
        {
            if(m_mode==SelectionMode::Vertex)
            {
                for(unsigned int i=0;i<m_selection.size();++i)
                {
                    removeVertex(m_target,m_selection[i]);
                }
                clearSelection();
            }
            else if(m_mode==SelectionMode::Edge)
            {
                removeEdge();
                clearSelection();
            }
            else if(m_mode==SelectionMode::Face)
            {
                deleteFace();
                clearSelection();
            }
        }

    }

    void removeVertex(unsigned int tt,unsigned int vertexID)
    {
        if(tt && vertexID)
        {
            if(m_objectList[tt]->vertex(vertexID)->m_adjacentEdgeList.size()==2)
            {
                Edge *edgeA=m_objectList[tt]->edge(m_objectList[tt]->vertex(vertexID)->m_adjacentEdgeList[0]);
                Edge *edgeB=m_objectList[tt]->edge(m_objectList[tt]->vertex(vertexID)->m_adjacentEdgeList[1]);
                if(edgeA->m_start==vertexID)
                {
                    if(edgeB->m_end==vertexID)
                    {//printf("@@@@@@@@1@@@@@@@@@@@@@");
                        unsigned int newEdge=m_objectList[tt]->addEdge(edgeB->m_start,edgeA->m_end);
                        if(edgeA->m_right>0 && edgeB->m_right>0 && edgeA->m_right==edgeB->m_right)
                        {
                            Face *face=m_objectList[tt]->face(edgeA->m_right);
                            unsigned int edgeCount=face->m_edge.size();
                            for(int e=edgeCount-1;e>-1;--e)
                            {
                                if(face->m_edge[e]==(int)(edgeB->m_index))
                                {
                                    m_objectList[tt]->faceEdgeChange(face->m_index,e,newEdge);
                                }
                                else
                                    if(face->m_edge[e]==(int)(edgeA->m_index))
                                {
                                    m_objectList[tt]->faceEdgeRemove(face->m_index,e);
                                }
                            }
                        }
                        if(edgeA->m_left>0 && edgeB->m_left>0 && edgeA->m_left==edgeB->m_left)
                        {
                            Face *face=m_objectList[tt]->face(edgeA->m_left);
                            unsigned int edgeCount=face->m_edge.size();
                            for(int e=edgeCount-1;e>-1;--e)
                            {
                                if(face->m_edge[e]==-(int)(edgeA->m_index))
                                {
                                    m_objectList[tt]->faceEdgeChange(face->m_index,e,-(int)(newEdge));
                                }
                                else
                                    if(face->m_edge[e]==-((int)edgeB->m_index))
                                {
                                    m_objectList[tt]->faceEdgeRemove(face->m_index,e);
                                }
                            }
                        }
                        deleteEdgeH(tt,edgeA->m_index);
                        deleteEdgeH(tt,edgeB->m_index);
                        return;
                    }
                    else
                    {//printf("@@@@@@@@@@2@@@@@@@@@@@");
                        unsigned int newEdge=m_objectList[tt]->addEdge(edgeB->m_end,edgeA->m_end);
                        if(edgeA->m_right>0 && edgeB->m_left>0 && edgeA->m_right==edgeB->m_left)
                        {
                            Face *face=m_objectList[tt]->face(edgeA->m_right);
                            unsigned int edgeCount=face->m_edge.size();
                            for(int e=edgeCount-1;e>-1;--e)
                            {
                                if(face->m_edge[e]==-((int)edgeB->m_index))
                                {
                                    m_objectList[tt]->faceEdgeChange(face->m_index,e,newEdge);
                                }
                                else
                                    if(face->m_edge[e]==((int)edgeA->m_index))
                                {
                                    m_objectList[tt]->faceEdgeRemove(face->m_index,e);
                                }
                            }
                        }
                        if(edgeA->m_left>0 && edgeB->m_right>0 && edgeA->m_left==edgeB->m_right)
                        {
                            Face *face=m_objectList[tt]->face(edgeA->m_left);
                            unsigned int edgeCount=face->m_edge.size();
                            for(int e=edgeCount-1;e>-1;--e)
                            {
                                if(face->m_edge[e]==-((int)edgeA->m_index))
                                {
                                    m_objectList[tt]->faceEdgeChange(face->m_index,e,-((int)newEdge));
                                }
                                else
                                    if(face->m_edge[e]==((int)edgeB->m_index))
                                {
                                    m_objectList[tt]->faceEdgeRemove(face->m_index,e);
                                }
                            }
                        }
                        deleteEdgeH(tt,edgeA->m_index);
                        deleteEdgeH(tt,edgeB->m_index);
                        return;
                    }
                }
                else
                {
                    if(edgeB->m_end==vertexID)
                    {//printf("@@@@@@@@@3@@@@@@@@@@@@");
                        unsigned int newEdge=m_objectList[tt]->addEdge(edgeB->m_start,edgeA->m_start);
                        if(edgeA->m_left>0 && edgeB->m_right>0 && edgeA->m_left==edgeB->m_right)
                        {
                            Face *face=m_objectList[tt]->face(edgeA->m_left);
                            unsigned int edgeCount=face->m_edge.size();
                            for(int e=edgeCount-1;e>-1;--e)
                            {
                                if(face->m_edge[e]==((int)edgeB->m_index))
                                {
                                    m_objectList[tt]->faceEdgeChange(face->m_index,e,newEdge);
                                }
                                else
                                    if(face->m_edge[e]==-((int)edgeA->m_index))
                                {
                                    m_objectList[tt]->faceEdgeRemove(face->m_index,e);
                                }
                            }
                        }
                        if(edgeA->m_right>0 && edgeB->m_left>0 && edgeA->m_right==edgeB->m_left)
                        {
                            Face *face=m_objectList[tt]->face(edgeA->m_right);
                            unsigned int edgeCount=face->m_edge.size();
                            for(int e=edgeCount-1;e>-1;--e)
                            {
                                if(face->m_edge[e]==((int)edgeA->m_index))
                                {
                                    m_objectList[tt]->faceEdgeChange(face->m_index,e,-((int)newEdge));
                                }
                                else
                                    if(face->m_edge[e]==-((int)edgeB->m_index))
                                {
                                    m_objectList[tt]->faceEdgeRemove(face->m_index,e);
                                }
                            }
                        }
                        deleteEdgeH(tt,edgeA->m_index);
                        deleteEdgeH(tt,edgeB->m_index);
                        return;
                    }
                    else
                    {
                        //printf("@@@@@@@@@@@4@@@@@@@@@@");
                        unsigned int newEdge=m_objectList[tt]->addEdge(edgeB->m_end,edgeA->m_start);
                        if(edgeA->m_left>0 && edgeB->m_left>0 && edgeA->m_left==edgeB->m_left)
                        {
                            Face *face=m_objectList[tt]->face(edgeA->m_left);
                            unsigned int edgeCount=face->m_edge.size();
                            for(int e=edgeCount-1;e>-1;--e)
                            {
                                if(face->m_edge[e]==-((int)edgeB->m_index))
                                {
                                    m_objectList[tt]->faceEdgeChange(face->m_index,e,-((int)newEdge));
                                }
                                else
                                    if(face->m_edge[e]==-((int)edgeA->m_index))
                                {
                                    m_objectList[tt]->faceEdgeRemove(face->m_index,e);
                                }
                            }
                        }
                        if(edgeA->m_right>0 && edgeB->m_right>0 && edgeA->m_right==edgeB->m_right)
                        {//printf("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
                            Face *face=m_objectList[tt]->face(edgeA->m_right);
                            int edgeCount=face->m_edge.size();
                            //printf("#%d#",edgeCount);
                            for(int e=edgeCount-1;e>-1;--e)
                            {
                                //printf("^^^%%%%%%%%%%%%%%%%%%%%%%%^^^^^^^^^^^^^^^^^^^");
                                if(face->m_edge[e]==(int)(edgeA->m_index))
                                {
                                    //printf("^^^^^^^^^^^^^^^^^^^^^^^^^");
                                    m_objectList[tt]->faceEdgeChange(face->m_index,e,-(int)newEdge);
                                }
                                else
                                    if(face->m_edge[e]==(int)edgeB->m_index)
                                {
                                    m_objectList[tt]->faceEdgeRemove(face->m_index,e);
                                }
                            }
                        }
                        deleteEdgeH(tt,edgeA->m_index);
                        deleteEdgeH(tt,edgeB->m_index);
                        return;
                    }
                }
            }
            else
            {
                return;
            }
        }
    }

    void redo()
    {
        HistoryRecord &record=historyManager->redoBegin();
        historyManager->recordBeginR(record.name);
        unsigned int recordSize=record.size();
        for(int i=recordSize-1;i>-1;--i)
        {
            parseLog(record[i]);
        }
        historyManager->recordEnd();
        historyManager->redoEnd();
    }

    void undo()
    {
        HistoryRecord &record=historyManager->undoBegin();
        historyManager->redoRecordBegin(record.name);
        unsigned int recordSize=record.size();
        for(int i=recordSize-1;i>-1;--i)
        {
            parseLog(record[i]);
        }
        historyManager->redoRecordEnd();
        historyManager->undoEnd();
    }

    void parseLog(HistoryLog *log)
    {

            if(log->m_type==LogType::Vertex_Position_Change)
            {
                Log_VertexPositionChange *original((Log_VertexPositionChange*)log);
                m_objectList[original->m_target]->vertexPositionChangeA(original->m_index,original->x,original->y,original->z);
            }
            else
            if(log->m_type==LogType::Vertex_Normal_Change)
            {
                Log_VertexNormalChange *original((Log_VertexNormalChange*)log);
                m_objectList[original->m_target]->vertexNormalChange(original->m_index,original->x,original->y,original->z);
            }
            else
            if(log->m_type==LogType::Vertex_Adjacent_Push)
            {
                Log_VertexAdjacentPush *original((Log_VertexAdjacentPush*)log);
                m_objectList[original->m_target]->vertexAdjacentPop(original->m_index);
            }
            else
            if(log->m_type==LogType::Vertex_Adjacent_Change)
            {
                Log_VertexAdjacentChange *original((Log_VertexAdjacentChange*)log);
                m_objectList[original->m_target]->vertexAdjacentChange(original->m_index,original->adjIndex,original->original);
            }
            else
            if(log->m_type==LogType::Vertex_Adjacent_Pop)
            {
                Log_VertexAdjacentPop *original((Log_VertexAdjacentPop*)log);
                m_objectList[original->m_target]->vertexAdjacentPush(original->m_index,original->o);
            }
            else
            if(log->m_type==LogType::Vertex_Adjacent_Swap)
            {
                Log_VertexAdjacentSwap *original((Log_VertexAdjacentSwap*)log);
                m_objectList[original->m_target]->vertexAdjacentSwap(original->m_index,original->tIndex,original->oIndex);
            }
            else
            if(log->m_type==LogType::Vertex_Adjacent_Remove)
            {
                Log_VertexAdjacentRemove *original((Log_VertexAdjacentRemove*)log);
                m_objectList[original->m_target]->vertexAdjacentInsert(original->m_index,original->adjIndex,original->o);
            }
            else
            if(log->m_type==LogType::Vertex_Adjacent_Insert)
            {
                Log_VertexAdjacentInsert *original((Log_VertexAdjacentInsert*)log);
                m_objectList[original->m_target]->vertexAdjacentRemove(original->m_index,original->adjIndex);
            }
            else
            if(log->m_type==LogType::Edge_Start_Change)
            {
                Log_EdgeStartChange *original((Log_EdgeStartChange*)log);
                m_objectList[original->m_target]->edgeStartChange(original->m_index,original->start);
            }
            else
            if(log->m_type==LogType::Edge_End_Change)
            {
                Log_EdgeEndChange *original((Log_EdgeEndChange*)log);
                m_objectList[original->m_target]->edgeEndChange(original->m_index, original->end);
            }
            else
            if(log->m_type==LogType::Edge_Left_Change)
            {
                Log_EdgeLeftChange *original((Log_EdgeLeftChange*)log);
                m_objectList[original->m_target]->edgeLeftChange(original->m_index,original->left);
            }
            else
            if(log->m_type==LogType::Edge_Right_Change)
            {
                Log_EdgeRightChange *original((Log_EdgeRightChange*)log);
                m_objectList[original->m_target]->edgeRightChange(original->m_index, original->right);
            }
            else
            if(log->m_type==LogType::Face_Edge_Push)
            {
                Log_FaceEdgePush *original((Log_FaceEdgePush*)log);
                m_objectList[original->m_target]->faceEdgePop(original->m_index);
            }
            else
            if(log->m_type==LogType::Face_Edge_Change)
            {
                Log_FaceEdgeChange *original((Log_FaceEdgeChange*)log);
                m_objectList[original->m_target]->faceEdgeChange(original->m_index,original->edgeIndex,original->o);
            }
            else
            if(log->m_type==LogType::Face_Edge_Pop)
            {
                Log_FaceEdgePop *original((Log_FaceEdgePop*)log);
                m_objectList[original->m_target]->faceEdgePush(original->m_index,original->o);
            }
            else
            if(log->m_type==LogType::Face_Edge_Swap)
            {
                Log_FaceEdgeSwap *original((Log_FaceEdgeSwap*)log);
                m_objectList[original->m_target]->faceEdgeSwap(original->m_index,original->tIndex,original->oIndex);
            }
            else
            if(log->m_type==LogType::Face_Edge_Remove)
            {
                Log_FaceEdgeRemove *original((Log_FaceEdgeRemove*)log);
                m_objectList[original->m_target]->faceEdgeInsert(original->m_index,original->edgeIndex,original->o);
            }
            else
            if(log->m_type==LogType::Face_Edge_Insert)
            {
                Log_FaceEdgeInsert *original((Log_FaceEdgeInsert*)log);
                m_objectList[original->m_target]->faceEdgeRemove(original->m_index,original->edgeIndex);
            }
            else
            if(log->m_type==LogType::Object_Position_Change)
            {
                Log_ObjectPositionChange *original((Log_ObjectPositionChange*)log);
                original;
            }
            else
            if(log->m_type==LogType::Object_Rotation_Change)
            {
                Log_ObjectRotationChange *original((Log_ObjectRotationChange*)log);
                original;
            }
            else
            if(log->m_type==LogType::Object_Scale_Change)
            {
                Log_ObjectScaleChange *original((Log_ObjectScaleChange*)log);
                original;
            }
            else
            if(log->m_type==LogType::Object_Color_Change)
            {
                Log_ObjectColorChange *original((Log_ObjectColorChange*)log);
                original;
            }
            else
            if(log->m_type==LogType::Object_Vertex_Add)
            {
                Log_ObjectVertexAdd *original((Log_ObjectVertexAdd*)log);
                m_objectList[original->m_target]->objectVertexRemove(original->m_index);
            }
            else
            if(log->m_type==LogType::Object_Vertex_Remove)
            {
                Log_ObjectVertexRemove *original((Log_ObjectVertexRemove*)log);
                m_objectList[original->m_target]->addVertex(original->m_index,original->o);
            }
            else
            if(log->m_type==LogType::Object_Edge_Add)
            {
                Log_ObjectEdgeAdd *original((Log_ObjectEdgeAdd*)log);
                m_objectList[original->m_target]->objectEdgeRemove(original->m_index);
            }
            else
            if(log->m_type==LogType::Object_Edge_Remove)
            {
                Log_ObjectEdgeRemove *original((Log_ObjectEdgeRemove*)log);
                m_objectList[original->m_target]->addEdge(original->m_index,original->o);
            }
            else
            if(log->m_type==LogType::Object_Face_Add)
            {
                Log_ObjectFaceAdd *original((Log_ObjectFaceAdd*)log);
                m_objectList[original->m_target]->objectFaceRemove(original->m_index);
            }
            else
            if(log->m_type==LogType::Object_Face_Remove)
            {
                Log_ObjectFaceRemove *original((Log_ObjectFaceRemove*)log);
                m_objectList[original->m_target]->addFace(original->m_index,original->o);
            }
            else
            if(log->m_type==LogType::Scene_Object_Add)
            {
                Log_SceneObjectAdd *original((Log_SceneObjectAdd*)log);
                sceneObjectRemove(original->m_target);
            }
            else
            if(log->m_type==LogType::Scene_Object_Remove)
            {
                Log_SceneObjectRemove *original((Log_SceneObjectRemove*)log);
                sceneObjectAdd(original->m_target,original->o);
            }
            else
            if(log->m_type==LogType::Scene_Selection_Add)
            {

            }
            else
            if(log->m_type==LogType::Scene_Selection_Remove)
            {
            }
    }

    void sceneObjectRemove(unsigned int objectID)
    {
        if(!historyManager->record(new Log_SceneObjectRemove(objectID,m_objectList[objectID])))
        {
            delete m_objectList[objectID];
        }
        m_objectList.remove(objectID);
    }

    void clearScene()
    {
        m_objectList.clear();
    }

    unsigned int sceneObjectAdd(unsigned int objectID,Object *object)
    {
        m_objectList.addI(objectID,object);
        historyManager->record(new Log_SceneObjectAdd(objectID));
        return objectID;
    }


    void selectDualSideObject(bool isAppend=false)
    {
        if(!isAppend)
        {
            clearSelection();
        }
        for(unsigned int i=1;i<m_objectList.size();++i)
        {
            struct SelectionResult selectBuffer[512];
          /*  glSelectBuffer(2048, (GLuint*)selectBuffer);
            glRenderMode(GL_SELECT);
            glInitNames();
            glPushName(0);
            unsigned int objectCount=0;
            while(i<m_objectList.size() && objectCount<512)
            {
                if(m_objectList[i])
                {
                    Object *object=m_objectList[i];
                    glLoadName(i);
                    object->selectionRenderObject();
                }
                ++i;
                ++objectCount;
            }
            --i;
            glFlush();
            GLuint hits = glRenderMode(GL_RENDER);
*/
            /*for (unsigned int e=0; e<hits; e++)
            {
                struct SelectionResult result=selectBuffer[e];
                if(result.size==1)
                {
                    selectionPush(m_objectList[result.id]);
                }
            }*/
        }
        updateAxisCursor();
    }

    void selectDualSideVertex(bool isAppend=false)
    {
        if(!isAppend)
        {
            clearSelection();
        }
        Object *object=m_objectList[m_target];
      /*          glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glTranslatef(object->m_position.x,object->m_position.y,object->m_position.z);
        glTranslatef(object->m_center.x,object->m_center.y,object->m_center.z);
        glRotatef(object->m_rotation.w,object->m_rotation.x,object->m_rotation.y,object->m_rotation.z);
        glScalef(object->m_scale.x,object->m_scale.y,object->m_scale.z);
        glTranslatef(-object->m_center.x,-object->m_center.y,-object->m_center.z);
*/
        for(unsigned int i=1;i<object->vertexCount();++i)
        {
        /*    struct SelectionResult selectBuffer[512];
            glSelectBuffer(2048, (GLuint*)selectBuffer);
            glRenderMode(GL_SELECT);
            glInitNames();
            glPushName(0);
            unsigned int objectCount=0;
            while(i<object->vertexCount() && objectCount<512)
            {
                Vertex *vertex=object->vertex(i);
                if(vertex)
                {
                    glLoadName(i);
                    glBegin(GL_POINTS);
                    glVertex3f(vertex->m_position.x,vertex->m_position.y,vertex->m_position.z);
                    glEnd();
                }
                ++i;
                ++objectCount;
            }
            --i;
            glFlush();

            GLuint hits = glRenderMode(GL_RENDER);

            for (unsigned int e=0; e<hits; ++e)
            {
                struct SelectionResult result=selectBuffer[e];
                if(result.size==1)
                {
                    selectionPush(object->vertex(result.id));
                }
            }*/
        }
             //       glPopMatrix();
        updateAxisCursor();
            if(!m_selection.empty())
            {
                object->buildPSCacheFromVID(m_selection);
            }
    }

    void selectDualSideEdge(bool isAppend=false)
    {
        if(!isAppend)
        {
            clearSelection();
        }
        Object *object=m_objectList[m_target];
    /*glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glTranslatef(object->m_position.x,object->m_position.y,object->m_position.z);
                glTranslatef(object->m_center.x,object->m_center.y,object->m_center.z);
        glRotatef(object->m_rotation.w,object->m_rotation.x,object->m_rotation.y,object->m_rotation.z);
glScalef(object->m_scale.x,object->m_scale.y,object->m_scale.z);
        glTranslatef(-object->m_center.x,-object->m_center.y,-object->m_center.z);

        for(unsigned int i=1;i<object->edgeCount();++i)
        {
            struct SelectionResult selectBuffer[512];
            glSelectBuffer(2048, (GLuint*)selectBuffer);
            glRenderMode(GL_SELECT);
            glInitNames();
            glPushName(0);
            unsigned int objectCount=0;
            while(i<object->edgeCount() && objectCount<512)
            {
                Edge *edge=object->edge(i);
                if(edge)
                {
                    glLoadName(i);
                    glBegin(GL_LINES);
                    Vertex *start=object->vertex(edge->m_start);
                    Vertex *end=object->vertex(edge->m_end);
                    glVertex3f(start->m_position.x,start->m_position.y,start->m_position.z);
                    glVertex3f(end->m_position.x,end->m_position.y,end->m_position.z);
                    glEnd();
                }
                ++i;
                ++objectCount;
            }
            --i;
            glFlush();

            GLuint hits = glRenderMode(GL_RENDER);

            for (unsigned int e=0; e<hits; ++e)
            {
                struct SelectionResult result=selectBuffer[e];
                if(result.size==1)
                {
                    selectionPush(object->edge(result.id));
                }
                else
                {
                    return;
                }
            }
        }
                glPopMatrix();
        updateAxisCursor();
                    if(!m_selection.empty())
            {
                object->buildPSCacheFromEID(selection);
            }*/
    }

    void redefineControlPoint()
    {
        if(m_mode==SelectionMode::Object)
        {
            for(unsigned int i=0;i<m_selection.size();++i)
            {
                if(m_objectList[m_selection[i]])
                {
                    m_objectList[m_selection[i]]->redefineControlPoint();
                }
            }
        }
        else if(m_target>0)
        {
            clearSelection();
            m_objectList[m_target]->clearPSCache();
            m_objectList[m_target]->redefineControlPoint();
        }
    }

    void selectDualSideFace(bool isAppend=false)
    {
        if(!isAppend)
        {
            clearSelection();
        }
        Object *object=m_objectList[m_target];
        /*glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glTranslatef(object->m_position.x,object->m_position.y,object->m_position.z);
                glTranslatef(object->m_center.x,object->m_center.y,object->m_center.z);
        glRotatef(object->m_rotation.w,object->m_rotation.x,object->m_rotation.y,object->m_rotation.z);
glScalef(object->m_scale.x,object->m_scale.y,object->m_scale.z);
        glTranslatef(-object->m_center.x,-object->m_center.y,-object->m_center.z);

        for(unsigned int i=1;i<object->faceCount();++i)
        {
            struct SelectionResult selectBuffer[512];
            glSelectBuffer(2048, (GLuint*)selectBuffer);
            glRenderMode(GL_SELECT);
            glInitNames();
            glPushName(0);
            unsigned int objectCount=0;
            while(i<object->faceCount() && objectCount<512)
            {
                Face *face=object->face(i);
                if(face)
                {
                    glLoadName(i);
                    glBegin(GL_POLYGON);
                    for(unsigned int e=0;e<face->m_edge.size();++e)
                    {
                        if(face->m_edge[e]>0)
                        {
                            Vertex *v=object->vertex(object->edge(face->m_edge[e])->m_start);
                            glVertex3f(v->m_position.x,v->m_position.y,v->m_position.z);
                        }
                        else
                        {
                            Vertex *v=object->vertex(object->edge(-face->m_edge[e])->m_end);
                            glVertex3f(v->m_position.x,v->m_position.y,v->m_position.z);
                        }
                    }
                    glEnd();
                }
                ++i;
                ++objectCount;
            }
            --i;
            glFlush();
            GLuint hits = glRenderMode(GL_RENDER);

            for (unsigned int e=0; e<hits; ++e)
            {
                struct SelectionResult result=selectBuffer[e];
                if(result.size==1)
                {
                    selectionPush(object->face(result.id));
                }
            }
        }
        glPopMatrix();*/
        updateAxisCursor();
                                if(!m_selection.empty())
            {
                object->buildPSCacheFromFID(m_selection);
            }
    }

    void selectionPush(ElementBase *element)
    {
        if(!element->m_isSelected)
        {
            element->m_isSelected=true;
            m_selection.push_back(element->m_index);
        }
    }

    void selectDualSide(bool isAppend=false)
    {
        if(m_mode==SelectionMode::Object && m_target==0)
        {
            selectDualSideObject(isAppend);
        }
        else if(m_mode==SelectionMode::Face && m_target)
        {
            selectDualSideFace(isAppend);
        }
        else if(m_mode==SelectionMode::Edge && m_target)
        {
            selectDualSideEdge(isAppend);
        }
        else if(m_mode==SelectionMode::Vertex && m_target)
        {
            selectDualSideVertex(isAppend);
        }
    }

    void changeShadeMode(RenderType::__Enum type)
    {
        if(m_mode==SelectionMode::Object && m_target==0)
        {
            unsigned int selectionSize=m_selection.size();
            for(unsigned int i=0;i<selectionSize;++i)
            {
                if(m_objectList[m_selection[i]])
                {
                    m_objectList[m_selection[i]]->m_renderMode=type;
                }
            }
        }
        else
        if(m_target)
        {
            if(m_objectList[m_target])
            {
                m_objectList[m_target]->m_renderMode=type;
            }
        }
    }

    void splitPress(unsigned int x1,unsigned int y1,unsigned int height)
    {
        Object *object=m_objectList[m_target];
        if(m_mode==SelectionMode::Split && object->vertexCount()<16777215 && object->edgeCount()<16777215)
        {
            unsigned int newSplitVX;
            unsigned int newSplitVY;
            unsigned int cutVertex=0;
            glClearColor(1.0f,1.0f,1.0f,1.0f);
            glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);
            glDisable(GL_DITHER);
            //glDisable(GL_LIGHTING);
            glDisable(GL_TEXTURE_2D);
           // glDisable(GL_ALPHA_TEST);
            glDisable(GL_BLEND);
           /* glMatrixMode(GL_MODELVIEW);
            glPushMatrix();
            glTranslatef(object->m_position.x,object->m_position.y,object->m_position.z);
            glTranslatef(object->m_center.x,object->m_center.y,object->m_center.z);
            glRotatef(object->m_rotation.w,object->m_rotation.x,object->m_rotation.y,object->m_rotation.z);
            glScalef(object->m_scale.x,object->m_scale.y,object->m_scale.z);
            glTranslatef(-object->m_center.x,-object->m_center.y,-object->m_center.z);
            glColorMask(GL_FALSE,GL_FALSE,GL_FALSE,GL_FALSE);
            glEnable(GL_POLYGON_OFFSET_FILL);
            glPolygonOffset(1.0f,1.0f);
            for(unsigned int i=1;i<object->faceCount();++i)
            {
                Face *face=object->face(i);
                if(face)
                {
                    struct ColorID colorID=(*(struct ColorID*)&(i));
                    glColor4ub(colorID.r, colorID.g,colorID.b,colorID.a);
                    glBegin(GL_POLYGON);
                    for(unsigned int e=0;e<face->m_edge.size();++e)
                    {
                        if(face->m_edge[e]>0)
                        {
                            Vertex *v=object->vertex(object->edge(face->m_edge[e])->m_start);
                            glVertex3f(v->m_position.x,v->m_position.y,v->m_position.z);
                        }
                        else
                        {
                            Vertex *v=object->vertex(object->edge(-face->m_edge[e])->m_end);
                            glVertex3f(v->m_position.x,v->m_position.y,v->m_position.z);
                        }
                    }
                    glEnd();
                }
            }
            glDisable(GL_POLYGON_OFFSET_FILL);
            glColorMask(GL_TRUE,GL_TRUE,GL_TRUE,GL_TRUE);
            glPointSize(5.0f);
            glBegin(GL_POINTS);
            for(unsigned int i=0;i<object->vertexCount();++i)
            {
                Vertex *vertex=object->vertex(i);
                if(vertex)
                {
                    struct ColorID colorID=(*(struct ColorID*)&(i));
                    glColor4ub(colorID.r, colorID.g,colorID.b,colorID.a);
                    glVertex3f(vertex->m_position.x,vertex->m_position.y,vertex->m_position.z);
                }
            }
            glEnd();
            glFlush();
            glPopMatrix();
            unsigned int sw=5;
            unsigned int sh=5;
            unsigned int length=sw*sh;
            struct ColorID *pixel=new struct ColorID[length];
            glReadPixels(x1,height-y1,sw,sh, GL_RGBA, GL_UNSIGNED_BYTE, pixel);
            for(unsigned int e=0;e<length;++e)
            {
                pixel[e].a=0;
                unsigned int result=(*(unsigned int*)&(pixel[e]));
                if(result<16777215)
                {
                    if(result<object->vertexCount()&&object->vertex(result))
                    {
                        //selectionPush(object->vertex(result));
                        cutVertex=result;
                        newSplitVX=x1;
                        newSplitVY=y1;
                        break;
                    }
                }
            }
            delete pixel;
            glEnable(GL_BLEND);
            glEnable(GL_ALPHA_TEST);
            glEnable(GL_TEXTURE_2D);
            glEnable(GL_LIGHTING);
            glEnable(GL_DITHER);
            glClearColor(128.0f/255.0f,128.0f/255.0f,128.0f/255.0f,1.0f);

            if(splitVertexID==0 && cutVertex>0)
            {
                splitVertexID=cutVertex;
//                splitVX=newSplitVX;
    //            splitVY=newSplitVY;
            }
            else if(splitVertexID>0 && cutVertex>0)
            {
                split(splitVertexID,cutVertex);
                splitVertexID=cutVertex;
//                splitVX=newSplitVX;
            //    splitVY=newSplitVY;
            }
            else if(cutVertex==0)
            {
                unsigned int edgeBeCuted=0;
                glClearColor(1.0f,1.0f,1.0f,1.0f);
                glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);
                glDisable(GL_DITHER);
                glDisable(GL_LIGHTING);
                glDisable(GL_TEXTURE_2D);
                glDisable(GL_ALPHA_TEST);
                glDisable(GL_BLEND);
                glMatrixMode(GL_MODELVIEW);
                glPushMatrix();
                glTranslatef(object->m_position.x,object->m_position.y,object->m_position.z);
                glTranslatef(object->m_center.x,object->m_center.y,object->m_center.z);
                glRotatef(object->m_rotation.w,object->m_rotation.x,object->m_rotation.y,object->m_rotation.z);
                glScalef(object->m_scale.x,object->m_scale.y,object->m_scale.z);
                glTranslatef(-object->m_center.x,-object->m_center.y,-object->m_center.z);
                glColorMask(GL_FALSE,GL_FALSE,GL_FALSE,GL_FALSE);
                glEnable(GL_POLYGON_OFFSET_FILL);
                glPolygonOffset(1.0f,1.0f);
                for(unsigned int i=1;i<object->faceCount();++i)
                {
                    Face *face=object->face(i);
                    if(face)
                    {
                        struct ColorID colorID=(*(struct ColorID*)&(i));
                        glColor4ub(colorID.r, colorID.g,colorID.b,colorID.a);
                        glBegin(GL_POLYGON);
                        for(unsigned int e=0;e<face->m_edge.size();++e)
                        {
                            if(face->m_edge[e]>0)
                            {
                                Vertex *v=object->vertex(object->edge(face->m_edge[e])->m_start);
                                glVertex3f(v->m_position.x,v->m_position.y,v->m_position.z);
                            }
                            else
                            {
                                Vertex *v=object->vertex(object->edge(-face->m_edge[e])->m_end);
                                glVertex3f(v->m_position.x,v->m_position.y,v->m_position.z);
                            }
                        }
                        glEnd();
                    }
                }
                glDisable(GL_POLYGON_OFFSET_FILL);
                glColorMask(GL_TRUE,GL_TRUE,GL_TRUE,GL_TRUE);
                glBegin(GL_LINES);
                for(unsigned int i=1;i<object->edgeCount();++i)
                {
                    Edge *edge=object->edge(i);
                    if(edge)
                    {
                        struct ColorID colorID=(*(struct ColorID*)&(i));
                        glColor4ub(colorID.r, colorID.g,colorID.b,colorID.a);
                        Vertex *start=object->vertex(edge->m_start);
                        Vertex *end=object->vertex(edge->m_end);
                        glVertex3f(start->m_position.x,start->m_position.y,start->m_position.z);
                        glVertex3f(end->m_position.x,end->m_position.y,end->m_position.z);
                    }
                }
                glEnd();
                glFlush();
                glPopMatrix();
                unsigned int sw=5;
                unsigned int sh=5;
                unsigned int length=sw*sh;
                struct ColorID *pixel=new struct ColorID[length];
                glReadPixels(x1,height-y1,sw,sh, GL_RGBA, GL_UNSIGNED_BYTE, pixel);
                for(unsigned int e=0;e<length;++e)
                {
                    pixel[e].a=0;
                    unsigned int result=(*(unsigned int*)&(pixel[e]));
                    if(result<16777215)
                    {
                        if(result<object->edgeCount()&&object->edge(result))
                        {
                            edgeBeCuted=result;
                            break;
                        }
                    }
                }
                delete pixel;
                glEnable(GL_BLEND);
                glEnable(GL_ALPHA_TEST);
                glEnable(GL_TEXTURE_2D);
                glEnable(GL_LIGHTING);
                glEnable(GL_DITHER);
                glClearColor(128.0f/255.0f,128.0f/255.0f,128.0f/255.0f,1.0f);
                if(edgeBeCuted)
                {
                    cutVertex=insertVertex(edgeBeCuted,0.5f);
                    if(splitVertexID==0)
                    {
                                        splitVertexID=cutVertex;

                    }
                    else
                    {
                        split(splitVertexID,cutVertex);
                        splitVertexID=cutVertex;
                    }
                }
            }*/
        }
    }

    void weldVertex()
    {
        if(m_mode==SelectionMode::Vertex && m_target && m_selection.size()>1)
        {
            unsigned int m_targetV=m_selection[0];
            for(unsigned int i=m_selection.size()-1;i>0;--i)
            {
                targetWeldVertex(m_selection[i],m_targetV);
            }
            clearSelection();
            m_objectList[m_target]->clearPSCache();
        }
    }

    void beginSplit()
    {
        m_isSplitMode=true;
        changeSelectionMode(SelectionMode::Split);
    }

    void nextSplit()
    {
        m_splitVertexID=0;
    }

    void endSplit()
    {
        m_isSplitMode=false;
        m_splitVertexID=0;
    }

    void selectSingleSide(unsigned int x1,unsigned int y1,unsigned int x2,unsigned int y2,unsigned int height,bool isAppend=false)
    {
        if(m_mode==SelectionMode::Object && m_target==0)
        {
            selectSingleSideObject(x1,y1,x2,y2,height,isAppend);
        }
        else if(m_mode==SelectionMode::Face && m_target)
        {
            selectSingleSideFace(x1,y1,x2,y2,height,isAppend);
        }
        else if(m_mode==SelectionMode::Edge && m_target)
        {
            selectSingleSideEdge(x1,y1,x2,y2,height,isAppend);
        }
        else if(m_mode==SelectionMode::Vertex && m_target)
        {
            selectSingleSideVertex(x1,y1,x2,y2,height,isAppend);
        }
        else if(m_mode==SelectionMode::Split && m_target)
        {
            splitPress(x1,y1,height);
        }
    }

    void selectSingleSideVertex(unsigned int x1,unsigned int y1,unsigned int x2,unsigned int y2,unsigned int height,bool isAppend=false)
    {
        if(!isAppend)
        {
            clearSelection();
        }
        Object *object=m_objectList[m_target];

        if(object->vertexCount()<16777215)
        {
            glClearColor(1.0f,1.0f,1.0f,1.0f);
            glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);
          /*  glDisable(GL_DITHER);
            glDisable(GL_LIGHTING);
            glDisable(GL_TEXTURE_2D);
            glDisable(GL_ALPHA_TEST);
            glDisable(GL_BLEND);
                        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glTranslatef(object->m_position.x,object->m_position.y,object->m_position.z);
                glTranslatef(object->m_center.x,object->m_center.y,object->m_center.z);
        glRotatef(object->m_rotation.w,object->m_rotation.x,object->m_rotation.y,object->m_rotation.z);
glScalef(object->m_scale.x,object->m_scale.y,object->m_scale.z);
        glTranslatef(-object->m_center.x,-object->m_center.y,-object->m_center.z);

            glColorMask(GL_FALSE,GL_FALSE,GL_FALSE,GL_FALSE);
            glEnable(GL_POLYGON_OFFSET_FILL);
            glPolygonOffset(1.0f,1.0f);
            for(unsigned int i=1;i<object->faceCount();++i)
            {
                Face *face=object->face(i);
                if(face)
                {
                    struct ColorID colorID=(*(struct ColorID*)&(i));
                    glColor4ub(colorID.r, colorID.g,colorID.b,colorID.a);
                    glBegin(GL_POLYGON);
                    for(unsigned int e=0;e<face->m_edge.size();++e)
                    {
                        if(face->m_edge[e]>0)
                        {
                            Vertex *v=object->vertex(object->edge(face->m_edge[e])->m_start);
                            glVertex3f(v->m_position.x,v->m_position.y,v->m_position.z);
                        }
                        else
                        {
                            Vertex *v=object->vertex(object->edge(-face->m_edge[e])->m_end);
                            glVertex3f(v->m_position.x,v->m_position.y,v->m_position.z);
                        }
                    }
                    glEnd();
                }
            }
            glDisable(GL_POLYGON_OFFSET_FILL);
            glColorMask(GL_TRUE,GL_TRUE,GL_TRUE,GL_TRUE);
            glPointSize(5.0f);
            glBegin(GL_POINTS);
            for(unsigned int i=0;i<object->vertexCount();++i)
            {
                Vertex *vertex=object->vertex(i);
                if(vertex)
                {
                    struct ColorID colorID=(*(struct ColorID*)&(i));
                    glColor4ub(colorID.r, colorID.g,colorID.b,colorID.a);
                    glVertex3f(vertex->m_position.x,vertex->m_position.y,vertex->m_position.z);
                }
            }
            glEnd();
            glFlush();
            glPopMatrix();
            unsigned int sw=(x2-x1)>5?(x2-x1):5;
            unsigned int sh=(y2-y1)>5?(y2-y1):5;
            unsigned int length=sw*sh;
            struct ColorID *pixel=new struct ColorID[length];
            glReadPixels(x1,height-y2,sw,sh, GL_RGBA, GL_UNSIGNED_BYTE, pixel);
            for(unsigned int e=0;e<length;++e)
            {
                pixel[e].a=0;
                unsigned int result=(*(unsigned int*)&(pixel[e]));
                if(result<16777215)
                {
                    if(result<object->vertexCount()&&object->vertex(result))
                    {
                        selectionPush(object->vertex(result));
                    }
                }
            }
            delete pixel;
            glEnable(GL_BLEND);
            glEnable(GL_ALPHA_TEST);
            glEnable(GL_TEXTURE_2D);
            glEnable(GL_LIGHTING);
            glEnable(GL_DITHER);
            glClearColor(128.0f/255.0f,128.0f/255.0f,128.0f/255.0f,1.0f);
            updateAxisCursor();
            m_objectList[m_target]->clearPSCache();
            if(!m_selection.empty())
            {
                object->buildPSCacheFromVID(selection);
            }*/
        }
    }

    void subdivide()
    {
        if(m_mode==SelectionMode::Object && m_target==0)
        {
            if(!m_selection.empty())
            {
                for(unsigned int i=0;i<m_selection.size();++i)
                {
                    m_objectList[m_selection[i]]->subdivide();
                }
            }
        }
        else
        {
            if(m_mode==SelectionMode::Vertex && m_target)
            {
                m_objectList[m_target]->subdivide();
                if(!m_selection.empty())
                {
                    m_objectList[m_target]->clearPSCache();
                    m_objectList[m_target]->buildPSCacheFromVID(m_selection);
                }
            }
        }
    }

    bool isAxisSelected(CameraMode::__Enum cameraMode,const Vector &eye,const unsigned int height,const unsigned int x,const unsigned int y,float &rx,float &ry,int &cursorMode)
    {
        if(m_selection.empty())
        {
            return false;
        }
        else
        {
            bool result(false);
            cursorMode=0;
            glClearColor(1.0f,1.0f,1.0f,1.0f);
            glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);
            glDisable(GL_DITHER);
            /*glDisable(GL_LIGHTING);
            glDisable(GL_TEXTURE_2D);
            glDisable(GL_ALPHA_TEST);
            glDisable(GL_BLEND);
            m_axisCursor->onPaint(eye,cameraMode);
            glFlush();
            struct ColorID pixel;
            glReadPixels(x,height-y,1,1, GL_RGBA, GL_UNSIGNED_BYTE,&pixel);
            if(pixel.r==255 && pixel.g==0 && pixel.b==0)
            {
                result=true;
                cursorMode=1;//xÖá
                GLint viewport[4];
                glGetIntegerv(GL_VIEWPORT, viewport);
                GLdouble modelMatrix[16];
                glGetDoublev(GL_MODELVIEW_MATRIX,modelMatrix);
                GLdouble projMatrix[16];
                glGetDoublev(GL_PROJECTION_MATRIX,projMatrix);
                GLdouble ox;
                GLdouble oy;
                GLdouble oz;
                GLdouble winx;
                GLdouble winy;
                GLdouble winz;
                if(m_axisCursor->m_m_mode==AxisCursorMode::MoveAxis)
                {
                    gluProject(0,0,0,modelMatrix,projMatrix,viewport,&ox,&oy,&oz);
                    oy=viewport[3]-(GLint)oy;
                    gluProject(10,0,0,modelMatrix,projMatrix,viewport,&winx,&winy,&winz);
                    winy=viewport[3]-(GLint)winy;
                    rx=(float)(winx-ox);
                    ry=(float)(winy-oy);
                }
                else if(m_axisCursor->m_m_mode==AxisCursorMode::RotateAxis)
                {
                    gluProject(0,0,0,modelMatrix,projMatrix,viewport,&ox,&oy,&oz);
                    oy=viewport[3]-(GLint)oy;
                    gluProject(0,10,0,modelMatrix,projMatrix,viewport,&winx,&winy,&winz);
                    winy=viewport[3]-(GLint)winy;
                    rx=(float)(winx-ox);
                    ry=(float)(winy-oy);
                }
                else if(m_axisCursor->m_m_mode==AxisCursorMode::ScaleAxis)
                {
                    gluProject(0,0,0,modelMatrix,projMatrix,viewport,&ox,&oy,&oz);
                    oy=viewport[3]-(GLint)oy;
                    gluProject(10,0,0,modelMatrix,projMatrix,viewport,&winx,&winy,&winz);
                    winy=viewport[3]-(GLint)winy;
                    rx=(float)(winx-ox);
                    ry=(float)(winy-oy);
                }
            }
            else if(pixel.r==0 && pixel.g==0 && pixel.b==255)
            {
                result=true;
                cursorMode=2;
                GLint viewport[4];
                glGetIntegerv(GL_VIEWPORT, viewport);
                GLdouble modelMatrix[16];
                glGetDoublev(GL_MODELVIEW_MATRIX,modelMatrix);
                GLdouble projMatrix[16];
                glGetDoublev(GL_PROJECTION_MATRIX,projMatrix);
                GLdouble ox;
                GLdouble oy;
                GLdouble oz;
                GLdouble winx;
                GLdouble winy;
                GLdouble winz;
                if(m_axisCursor->m_m_mode==AxisCursorMode::MoveAxis)
                {
                    gluProject(0,0,0,modelMatrix,projMatrix,viewport,&ox,&oy,&oz);
                    oy=viewport[3]-(GLint)oy;
                    gluProject(0,10,0,modelMatrix,projMatrix,viewport,&winx,&winy,&winz);
                    winy=viewport[3]-(GLint)winy;
                    rx=(float)(winx-ox);
                    ry=(float)(winy-oy);
                }
                else if(m_axisCursor->m_m_mode==AxisCursorMode::RotateAxis)
                {
                    gluProject(0,0,0,modelMatrix,projMatrix,viewport,&ox,&oy,&oz);
                    oy=viewport[3]-(GLint)oy;
                    gluProject(0,0,10,modelMatrix,projMatrix,viewport,&winx,&winy,&winz);
                    winy=viewport[3]-(GLint)winy;
                    rx=(float)(winx-ox);
                    ry=(float)(winy-oy);
                }
                else if(m_axisCursor->m_m_mode==AxisCursorMode::ScaleAxis)
                {
                    gluProject(0,0,0,modelMatrix,projMatrix,viewport,&ox,&oy,&oz);
                    oy=viewport[3]-(GLint)oy;
                    gluProject(0,10,0,modelMatrix,projMatrix,viewport,&winx,&winy,&winz);
                    winy=viewport[3]-(GLint)winy;
                    rx=(float)(winx-ox);
                    ry=(float)(winy-oy);
                }
            }
            else if(pixel.r==255 && pixel.g==252 && pixel.b==0)
            {
                result=true;
                cursorMode=3;
                GLint viewport[4];
                glGetIntegerv(GL_VIEWPORT, viewport);
                GLdouble modelMatrix[16];
                glGetDoublev(GL_MODELVIEW_MATRIX,modelMatrix);
                GLdouble projMatrix[16];
                glGetDoublev(GL_PROJECTION_MATRIX,projMatrix);
                GLdouble ox;
                GLdouble oy;
                GLdouble oz;
                GLdouble winx;
                GLdouble winy;
                GLdouble winz;
                if(m_axisCursor->m_m_mode==AxisCursorMode::MoveAxis)
                {
                    gluProject(0,0,0,modelMatrix,projMatrix,viewport,&ox,&oy,&oz);
                    oy=viewport[3]-(GLint)oy;
                    gluProject(0,0,10,modelMatrix,projMatrix,viewport,&winx,&winy,&winz);
                    winy=viewport[3]-(GLint)winy;
                    rx=(float)(winx-ox);
                    ry=(float)(winy-oy);
                }
                else if(m_axisCursor->m_m_mode==AxisCursorMode::RotateAxis)
                {
                    gluProject(0,0,0,modelMatrix,projMatrix,viewport,&ox,&oy,&oz);
                    oy=viewport[3]-(GLint)oy;
                    gluProject(10,0,0,modelMatrix,projMatrix,viewport,&winx,&winy,&winz);
                    winy=viewport[3]-(GLint)winy;
                    rx=(float)(winx-ox);
                    ry=(float)(winy-oy);
                }
                else if(m_axisCursor->m_m_mode==AxisCursorMode::ScaleAxis)
                {
                    gluProject(0,0,0,modelMatrix,projMatrix,viewport,&ox,&oy,&oz);
                    oy=viewport[3]-(GLint)oy;
                    gluProject(0,0,10,modelMatrix,projMatrix,viewport,&winx,&winy,&winz);
                    winy=viewport[3]-(GLint)winy;
                    rx=(float)(winx-ox);
                    ry=(float)(winy-oy);
                }
            }
            else if(pixel.r==0 && pixel.g==255 && pixel.b==0)
            {
                result=true;
                cursorMode=4;
            }
            glEnable(GL_BLEND);
            glEnable(GL_ALPHA_TEST);
            glEnable(GL_TEXTURE_2D);
            glEnable(GL_LIGHTING);
            glEnable(GL_DITHER);
            glClearColor(128.0f/255.0f,128.0f/255.0f,128.0f/255.0f,1.0f);*/
            return result;
        }
    }

    void axisXMove(float step)
    {
        m_axisCursor->m_position.x+=step;
        if(m_mode==SelectionMode::Object && m_target==0)
        {
            for(unsigned int i=0;i<m_selection.size();++i)
            {
                Object *object=m_objectList[m_selection[i]];
                if(object)
                {
                    object->m_position.x+=step;
                }
            }
        }
        else if(m_mode==SelectionMode::Vertex && m_target)
        {
            Object *object=m_objectList[m_target];
            Quaternion angle(-object->m_rotation.w,Vector(object->m_rotation));
            Matrix rotateMatrix;
            angle.getMatrix(rotateMatrix);
            Vector xAxis(1,0,0);
            xAxis=(xAxis)*(rotateMatrix)*step;
            moveVertex(xAxis.x,xAxis.y,xAxis.z);
            object->partialSubdivision();
        }
        else if(m_mode==SelectionMode::Edge && m_target)
        {
            Object *object=m_objectList[m_target];
            Quaternion angle(-object->m_rotation.w,Vector(object->m_rotation));
            Matrix rotateMatrix;
            angle.getMatrix(rotateMatrix);
            Vector xAxis(1,0,0);
            xAxis=(xAxis)*(rotateMatrix)*step;
            moveEdge(xAxis.x,xAxis.y,xAxis.z);
            object->partialSubdivision();
        }
        else if(m_mode==SelectionMode::Face && m_target)
        {
            Object *object=m_objectList[m_target];
            Quaternion angle(-object->m_rotation.w,Vector(object->m_rotation));
            Matrix rotateMatrix;
            angle.getMatrix(rotateMatrix);
            Vector xAxis(1,0,0);
            xAxis=(xAxis)*(rotateMatrix)*step;
            moveFace(xAxis.x,xAxis.y,xAxis.z);
            object->partialSubdivision();
        }
    }

    void axisXRotate(float step)
    {
        m_axisCursor->m_rotation*=m_axisCursor->m_rotation.w;
        m_axisCursor->m_rotation.z+=step;
        float angle=m_axisCursor->m_rotation.length();
        m_axisCursor->m_rotation.normalize();
        m_axisCursor->m_rotation.w=angle;

        Vector rotateAxis=Vector(0,0,1);
        Quaternion rotateQuaternion(step,rotateAxis);
        Matrix rotateMatrix;
        rotateQuaternion.getMatrix(rotateMatrix);

        if(m_mode==SelectionMode::Object && m_target==0)
        {
            for(unsigned int i=0;i<m_selection.size();++i)
            {
                Object *object=m_objectList[m_selection[i]];
                if(object)
                {
                    //Ðý×ª·Ö³ÉÁ½²¿£¬µÚÒ»²½ÊÇ¸üÐÂÎ»ÖÃ£¬µÚ¶þ²½ÊÇ¸üÐÂ½Ç¶È
                    //Ê×ÏÈÒª¸üÐÂÎ»ÖÃ
                    //¶ÔÓÚÃ¿¸öÎïÌåÊ×ÏÈÇó³öÖÐÐÄµãµ½Ðý×ªÖáµÄÒ»¸öÏòÁ¿
                    //new position;
                    Vector CToR(object->m_center+object->m_position-m_axisCursor->m_position);
                    //½«Õâ¸öÏòÁ¿ÑØ×ÅÐý×ªÖá½øÐÐÐý×ª
                    CToR=(rotateMatrix)*(CToR);
                    //Ðý×ªÖ®ºó»¹Ô­ÐÂµÄÖÐÐÄµã
                    object->m_position=m_axisCursor->m_position+CToR-object->m_center;
                    //Ö®ºóÒªÐý×ªÎïÌåµÄ½Ç¶È
                    Quaternion original(object->m_rotation.w,Vector(object->m_rotation));
                    Quaternion add(step,Vector(0,0,1));
                    Quaternion result=add*original;
                    result.getRotate(object->m_rotation.w,object->m_rotation);
                    /*object->m_rotation*=object->m_rotation.w;
                    object->m_rotation.z+=step;
                    float angle=object->m_rotation.length();
                    object->m_rotation.normalize();
                    object->m_rotation.w=angle;*/
                }
            }
        }
        else if(m_mode==SelectionMode::Vertex)
        {
            Object *object=m_objectList[m_target];
            Quaternion original(-object->m_rotation.w,Vector(object->m_rotation));
            Matrix originalMatrix;
            Matrix rotateMatrix;
            rotateQuaternion.getMatrix(rotateMatrix);
            original.getMatrix(originalMatrix);
            rotateVertex(originalMatrix*(m_axisCursor->m_position-object->m_center)+object->m_center,rotateMatrix);
            object->partialSubdivision();
        }
        else if(m_mode==SelectionMode::Edge)
        {
            Object *object=m_objectList[m_target];
            Quaternion original(-object->m_rotation.w,Vector(object->m_rotation));
            Matrix originalMatrix;
            Matrix rotateMatrix;
            rotateQuaternion.getMatrix(rotateMatrix);
            original.getMatrix(originalMatrix);
            rotateEdge(originalMatrix*(m_axisCursor->m_position-object->m_center)+object->m_center,rotateMatrix);
            object->partialSubdivision();
        }
        else if(m_mode==SelectionMode::Face)
        {
            Object *object=m_objectList[m_target];
            Quaternion original(-object->m_rotation.w,Vector(object->m_rotation));
            Matrix originalMatrix;
            Matrix rotateMatrix;
            rotateQuaternion.getMatrix(rotateMatrix);
            original.getMatrix(originalMatrix);
            rotateFace(originalMatrix*(m_axisCursor->m_position-object->m_center)+object->m_center,rotateMatrix);
            object->partialSubdivision();
        }
    }

    void axisYRotate(float step)
    {
        m_axisCursor->m_rotation*=m_axisCursor->m_rotation.w;
        m_axisCursor->m_rotation.x+=step;
        float angle=m_axisCursor->m_rotation.length();
        m_axisCursor->m_rotation.normalize();
        m_axisCursor->m_rotation.w=angle;

        Vector rotateAxis=Vector(1,0,0);
        Quaternion rotateQuaternion(step,rotateAxis);
        Matrix rotateMatrix;
        rotateQuaternion.getMatrix(rotateMatrix);

        if(m_mode==SelectionMode::Object && m_target==0)
        {
            for(unsigned int i=0;i<m_selection.size();++i)
            {
                Object *object=m_objectList[m_selection[i]];
                if(object)
                {
                    //Ðý×ª·Ö³ÉÁ½²¿£¬µÚÒ»²½ÊÇ¸üÐÂÎ»ÖÃ£¬µÚ¶þ²½ÊÇ¸üÐÂ½Ç¶È
                    //Ê×ÏÈÒª¸üÐÂÎ»ÖÃ
                    //¶ÔÓÚÃ¿¸öÎïÌåÊ×ÏÈÇó³öÖÐÐÄµãµ½Ðý×ªÖáµÄÒ»¸öÏòÁ¿
                    //new position;
                    Vector CToR(object->m_center+object->m_position-m_axisCursor->m_position);
                    //½«Õâ¸öÏòÁ¿ÑØ×ÅÐý×ªÖá½øÐÐÐý×ª
                    CToR=(rotateMatrix)*(CToR);
                    //Ðý×ªÖ®ºó»¹Ô­ÐÂµÄÖÐÐÄµã
                    object->m_position=m_axisCursor->m_position+CToR-object->m_center;
                    //Ö®ºóÒªÐý×ªÎïÌåµÄ½Ç¶È
/*                    object->m_rotation*=object->m_rotation.w;
                    object->m_rotation.x+=step;
                    float angle=object->m_rotation.length();
                    object->m_rotation.normalize();
                    object->m_rotation.w=angle;*/

                    Quaternion original(object->m_rotation.w,Vector(object->m_rotation));
                    Quaternion add(step,Vector(1,0,0));
                    Quaternion result=add*original;
                    result.getRotate(object->m_rotation.w,object->m_rotation);

                }
            }
        }
        else if(m_mode==SelectionMode::Vertex)
        {
            Object *object=m_objectList[m_target];
            Quaternion original(-object->m_rotation.w,Vector(object->m_rotation));
            Matrix originalMatrix;
            Matrix rotateMatrix;
            rotateQuaternion.getMatrix(rotateMatrix);
            original.getMatrix(originalMatrix);
            rotateVertex(originalMatrix*(m_axisCursor->m_position-object->m_center)+object->m_center,rotateMatrix);
            object->partialSubdivision();
        }
        else if(m_mode==SelectionMode::Edge)
        {
            Object *object=m_objectList[m_target];
            Quaternion original(-object->m_rotation.w,Vector(object->m_rotation));
            Matrix originalMatrix;
            Matrix rotateMatrix;
            rotateQuaternion.getMatrix(rotateMatrix);
            original.getMatrix(originalMatrix);
            rotateEdge(originalMatrix*(m_axisCursor->m_position-object->m_center)+object->m_center,rotateMatrix);
            object->partialSubdivision();
        }
        else if(m_mode==SelectionMode::Face)
        {
            Object *object=m_objectList[m_target];
            Quaternion original(-object->m_rotation.w,Vector(object->m_rotation));
            Matrix originalMatrix;
            Matrix rotateMatrix;
            rotateQuaternion.getMatrix(rotateMatrix);
            original.getMatrix(originalMatrix);
            rotateFace(originalMatrix*(m_axisCursor->m_position-object->m_center)+object->m_center,rotateMatrix);
            object->partialSubdivision();
        }
    }

    void rotateVertex(const Vector &center, Matrix &angle)
    {
        unsigned int selectionCount=m_selection.size();
        for(unsigned int i=0;i<selectionCount;++i)
        {
            Vertex *vertex=m_objectList[m_target]->vertex(m_selection[i]);
            if(vertex)
            {
                Vector PToC=(vertex->m_position-center);
                PToC=angle*PToC+center;
                m_objectList[m_target]->vertexPositionChangeA(vertex->m_index,PToC.x,PToC.y,PToC.z);
            }
        }
    }

    void rotateEdge(const Vector &center,Matrix &angle)
    {
        unsigned int selectionCount=m_selection.size();
        std::vector<Vertex *> vertexToBeRotate;
        vertexToBeRotate.reserve(selectionCount*2);
        for(unsigned int i=0;i<selectionCount;++i)
        {
            Edge *edge=m_objectList[m_target]->edge(m_selection[i]);
            if(edge)
            {
                Vertex *start=m_objectList[m_target]->vertex(edge->m_start);
                Vertex *end=m_objectList[m_target]->vertex(edge->m_end);
                if(!start->m_isIn)
                {
                    vertexToBeRotate.push_back(start);
                    m_isInCache.push_back(start);
                    start->m_isIn=true;
                }
                if(!end->m_isIn)
                {
                    vertexToBeRotate.push_back(end);
                    m_isInCache.push_back(end);
                    end->m_isIn=true;
                }
            }
        }

        clearIsInCache();

        for(unsigned int i=0;i<vertexToBeRotate.size();++i)
        {
            Vertex *vertex=vertexToBeRotate[i];
            if(vertex)
            {
                Vector PToC=(vertex->m_position-center);
                PToC=angle*PToC+center;
                m_objectList[m_target]->vertexPositionChangeA(vertex->m_index,PToC.x,PToC.y,PToC.z);
            }
        }
    }

    void rotateFace(const Vector &center,Matrix &angle)
    {
        unsigned int selectionCount=m_selection.size();
        std::vector<Vertex *> vertexToBeRotate;
        vertexToBeRotate.reserve(selectionCount*2);
        for(unsigned int i=0;i<selectionCount;++i)
        {
            Face *face=m_objectList[m_target]->face(m_selection[i]);
            unsigned int edgeCount=face->m_edge.size();
            for(unsigned int e=0;e<edgeCount;++e)
            {
                if(face->m_edge[e]>0)
                {
                    Edge *edge=m_objectList[m_target]->edge(face->m_edge[e]);
                    Vertex *vertex=m_objectList[m_target]->vertex(edge->m_end);
                    if(!vertex->m_isIn)
                    {
                        vertexToBeRotate.push_back(vertex);
                        vertex->m_isIn=true;
                        m_isInCache.push_back(vertex);
                    }
                }
                else
                {
                    Edge *edge=m_objectList[m_target]->edge(-face->m_edge[e]);
                    Vertex *vertex=m_objectList[m_target]->vertex(edge->m_start);
                    if(!vertex->m_isIn)
                    {
                        vertexToBeRotate.push_back(vertex);
                        vertex->m_isIn=true;
                        m_isInCache.push_back(vertex);
                    }
                }
            }
        }

        clearIsInCache();

        for(unsigned int i=0;i<vertexToBeRotate.size();++i)
        {
            Vertex *vertex=vertexToBeRotate[i];
            if(vertex)
            {
                Vector PToC=(vertex->m_position-center);
                PToC=angle*PToC+center;
                m_objectList[m_target]->vertexPositionChangeA(vertex->m_index,PToC.x,PToC.y,PToC.z);
            }
        }
    }

    void scaleVertex(Vector &center,Matrix &forward,Matrix &backward,float scale,int smode)
    {
        unsigned int selectionCount=m_selection.size();
        for(unsigned int i=0;i<selectionCount;++i)
        {
            Vertex *vertex=m_objectList[m_target]->vertex(m_selection[i]);
            if(vertex)
            {
                Vector newPosition(vertex->m_position-m_objectList[m_target]->m_center);
                newPosition=forward*newPosition;
                newPosition=newPosition-(center-m_objectList[m_target]->m_center);
                if(smode==1)
                {
                    newPosition.x*=scale;
                }
                else if(smode==2)
                {
                    newPosition.y*=scale;
                }
                else if(smode==3)
                {
                    newPosition.z*=scale;
                }
                else if(smode==4)
                {
                    newPosition*=scale;
                }
                newPosition+=(center-m_objectList[m_target]->m_center);
                newPosition=backward*newPosition;
                newPosition+=m_objectList[m_target]->m_center;
                m_objectList[m_target]->vertexPositionChangeA(vertex->m_index,newPosition.x,newPosition.y,newPosition.z);
            }
        }
    }

    void scaleEdge(Vector &center,Matrix &forward,Matrix &backward,float scale,int smode)
    {
        unsigned int selectionCount=m_selection.size();
        std::vector<Vertex *> vertexToBeScale;
        vertexToBeScale.reserve(selectionCount*2);
        for(unsigned int i=0;i<selectionCount;++i)
        {
            Edge *edge=m_objectList[m_target]->edge(m_selection[i]);
            if(edge)
            {
                Vertex *start=m_objectList[m_target]->vertex(edge->m_start);
                Vertex *end=m_objectList[m_target]->vertex(edge->m_end);
                if(!start->m_isIn)
                {
                    vertexToBeScale.push_back(start);
                    m_isInCache.push_back(start);
                    start->m_isIn=true;
                }
                if(!end->m_isIn)
                {
                    vertexToBeScale.push_back(end);
                    m_isInCache.push_back(end);
                    end->m_isIn=true;
                }
            }
        }

        clearIsInCache();

        unsigned int vertexCount=vertexToBeScale.size();
        for(unsigned int i=0;i<vertexCount;++i)
        {
            Vertex *vertex=vertexToBeScale[i];
            if(vertex)
            {
                Vector newPosition(vertex->m_position-m_objectList[m_target]->m_center);
                newPosition=forward*newPosition;
                newPosition=newPosition-(center-m_objectList[m_target]->m_center);
                if(smode==1)
                {
                    newPosition.x*=scale;
                }
                else if(smode==2)
                {
                    newPosition.y*=scale;
                }
                else if(smode==3)
                {
                    newPosition.z*=scale;
                }
                else if(smode==4)
                {
                    newPosition*=scale;
                }
                newPosition+=(center-m_objectList[m_target]->m_center);
                newPosition=backward*newPosition;
                newPosition+=m_objectList[m_target]->m_center;
                m_objectList[m_target]->vertexPositionChangeA(vertex->m_index,newPosition.x,newPosition.y,newPosition.z);
            }
        }
    }

    void scaleFace(Vector &center,Matrix &forward,Matrix &backward,float scale,int smode)
    {
        unsigned int selectionCount=m_selection.size();
        std::vector<Vertex *> vertexToBeScale;
        vertexToBeScale.reserve(selectionCount*2);
        for(unsigned int i=0;i<selectionCount;++i)
        {
            Face *face=m_objectList[m_target]->face(m_selection[i]);
            unsigned int edgeCount=face->m_edge.size();
            for(unsigned int e=0;e<edgeCount;++e)
            {
                if(face->m_edge[e]>0)
                {
                    Edge *edge=m_objectList[m_target]->edge(face->m_edge[e]);
                    Vertex *vertex=m_objectList[m_target]->vertex(edge->m_end);
                    if(!vertex->m_isIn)
                    {
                        vertexToBeScale.push_back(vertex);
                        vertex->m_isIn=true;
                        m_isInCache.push_back(vertex);
                    }
                }
                else
                {
                    Edge *edge=m_objectList[m_target]->edge(-face->m_edge[e]);
                    Vertex *vertex=m_objectList[m_target]->vertex(edge->m_start);
                    if(!vertex->m_isIn)
                    {
                        vertexToBeScale.push_back(vertex);
                        vertex->m_isIn=true;
                        m_isInCache.push_back(vertex);
                    }
                }
            }
        }

        clearIsInCache();

        unsigned int vertexCount=vertexToBeScale.size();
        for(unsigned int i=0;i<vertexCount;++i)
        {
            Vertex *vertex=vertexToBeScale[i];
            if(vertex)
            {
                Vector newPosition(vertex->m_position-m_objectList[m_target]->m_center);
                newPosition=forward*newPosition;
                newPosition=newPosition-(center-m_objectList[m_target]->m_center);
                if(smode==1)
                {
                    newPosition.x*=scale;
                }
                else if(smode==2)
                {
                    newPosition.y*=scale;
                }
                else if(smode==3)
                {
                    newPosition.z*=scale;
                }
                else if(smode==4)
                {
                    newPosition*=scale;
                }
                newPosition+=(center-m_objectList[m_target]->m_center);
                newPosition=backward*newPosition;
                newPosition+=m_objectList[m_target]->m_center;
                m_objectList[m_target]->vertexPositionChangeA(vertex->m_index,newPosition.x,newPosition.y,newPosition.z);
            }
        }
    }

    void axisZRotate(float step)
    {
        m_axisCursor->m_rotation*=m_axisCursor->m_rotation.w;
        m_axisCursor->m_rotation.y+=step;
        float angle=m_axisCursor->m_rotation.length();
        m_axisCursor->m_rotation.normalize();
        m_axisCursor->m_rotation.w=angle;

        Vector rotateAxis=Vector(0,1,0);
        Quaternion rotateQuaternion(step,rotateAxis);
        Matrix rotateMatrix;
        rotateQuaternion.getMatrix(rotateMatrix);

        if(m_mode==SelectionMode::Object && m_target==0)
        {
            for(unsigned int i=0;i<m_selection.size();++i)
            {
                Object *object=m_objectList[m_selection[i]];
                if(object)
                {
                    //Ðý×ª·Ö³ÉÁ½²¿£¬µÚÒ»²½ÊÇ¸üÐÂÎ»ÖÃ£¬µÚ¶þ²½ÊÇ¸üÐÂ½Ç¶È
                    //Ê×ÏÈÒª¸üÐÂÎ»ÖÃ
                    //¶ÔÓÚÃ¿¸öÎïÌåÊ×ÏÈÇó³öÖÐÐÄµãµ½Ðý×ªÖáµÄÒ»¸öÏòÁ¿
                    //new position;
                    Vector CToR(object->m_center+object->m_position-m_axisCursor->m_position);
                    //½«Õâ¸öÏòÁ¿ÑØ×ÅÐý×ªÖá½øÐÐÐý×ª
                    CToR=(rotateMatrix)*(CToR);
                    //Ðý×ªÖ®ºó»¹Ô­ÐÂµÄÖÐÐÄµã
                    object->m_position=m_axisCursor->m_position+CToR-object->m_center;
                    //Ö®ºóÒªÐý×ªÎïÌåµÄ½Ç¶È
                /*    object->m_rotation*=object->m_rotation.w;
                    object->m_rotation.y+=step;
                    float angle=object->m_rotation.length();
                    object->m_rotation.normalize();
                    object->m_rotation.w=angle;*/

                    Quaternion original(object->m_rotation.w,Vector(object->m_rotation));
                    Quaternion add(step,Vector(0,1,0));
                    Quaternion result=add*original;
                    result.getRotate(object->m_rotation.w,object->m_rotation);
                }
            }
        }
        else if(m_mode==SelectionMode::Vertex)
        {
            Object *object=m_objectList[m_target];
            Quaternion original(-object->m_rotation.w,Vector(object->m_rotation));
            Matrix originalMatrix;
            Matrix rotateMatrix;
            rotateQuaternion.getMatrix(rotateMatrix);
            original.getMatrix(originalMatrix);
            rotateVertex(originalMatrix*(m_axisCursor->m_position-object->m_center)+object->m_center,rotateMatrix);
            object->partialSubdivision();
        }
        else if(m_mode==SelectionMode::Edge)
        {
            Object *object=m_objectList[m_target];
            Quaternion original(-object->m_rotation.w,Vector(object->m_rotation));
            Matrix originalMatrix;
            Matrix rotateMatrix;
            rotateQuaternion.getMatrix(rotateMatrix);
            original.getMatrix(originalMatrix);
            rotateEdge(originalMatrix*(m_axisCursor->m_position-object->m_center)+object->m_center,rotateMatrix);
            object->partialSubdivision();
        }
        else if(m_mode==SelectionMode::Face)
        {
            Object *object=m_objectList[m_target];
            Quaternion original(-object->m_rotation.w,Vector(object->m_rotation));
            Matrix originalMatrix;
            Matrix rotateMatrix;
            rotateQuaternion.getMatrix(rotateMatrix);
            original.getMatrix(originalMatrix);
            rotateFace(originalMatrix*(m_axisCursor->m_position-object->m_center)+object->m_center,rotateMatrix);
            object->partialSubdivision();
        }
    }

    void axisYMove(float step)
    {
        m_axisCursor->m_position.y+=step;
        if(m_mode==SelectionMode::Object && m_target==0)
        {
            for(unsigned int i=0;i<m_selection.size();++i)
            {
                Object *object=m_objectList[m_selection[i]];
                if(object)
                {
                    object->m_position.y+=step;
                }
            }
        }
        else if(m_mode==SelectionMode::Vertex && m_target)
        {
            Object *object=m_objectList[m_target];
            Quaternion angle(-object->m_rotation.w,Vector(object->m_rotation));
            Matrix rotateMatrix;
            angle.getMatrix(rotateMatrix);
            Vector xAxis(0,1,0);
            xAxis=(xAxis)*(rotateMatrix)*step;
            moveVertex(xAxis.x,xAxis.y,xAxis.z);
            object->partialSubdivision();
        }
        else if(m_mode==SelectionMode::Edge && m_target)
        {
                        Object *object=m_objectList[m_target];
            Quaternion angle(-object->m_rotation.w,Vector(object->m_rotation));
            Matrix rotateMatrix;
            angle.getMatrix(rotateMatrix);
            Vector xAxis(0,1,0);
            xAxis=(xAxis)*(rotateMatrix)*step;
            moveEdge(xAxis.x,xAxis.y,xAxis.z);
            object->partialSubdivision();
        }
        else if(m_mode==SelectionMode::Face && m_target)
        {
                        Object *object=m_objectList[m_target];
            Quaternion angle(-object->m_rotation.w,Vector(object->m_rotation));
            Matrix rotateMatrix;
            angle.getMatrix(rotateMatrix);
            Vector xAxis(0,1,0);
            xAxis=(xAxis)*(rotateMatrix)*step;
            moveFace(xAxis.x,xAxis.y,xAxis.z);
            object->partialSubdivision();
        }
    }

    void axisZMove(float step)
    {
        m_axisCursor->m_position.z+=step;
        if(m_mode==SelectionMode::Object && m_target==0)
        {
            for(unsigned int i=0;i<m_selection.size();++i)
            {
                Object *object=m_objectList[m_selection[i]];
                if(object)
                {
                    object->m_position.z+=step;
                }
            }
        }
        else if(m_mode==SelectionMode::Vertex && m_target)
        {
            Object *object=m_objectList[m_target];
            Quaternion angle(-object->m_rotation.w,Vector(object->m_rotation));
            Matrix rotateMatrix;
            angle.getMatrix(rotateMatrix);
            Vector xAxis(0,0,1);
            xAxis=(xAxis)*(rotateMatrix)*step;
            moveVertex(xAxis.x,xAxis.y,xAxis.z);
            object->partialSubdivision();
        }
        else if(m_mode==SelectionMode::Edge && m_target)
        {
            Object *object=m_objectList[m_target];
            Quaternion angle(-object->m_rotation.w,Vector(object->m_rotation));
            Matrix rotateMatrix;
            angle.getMatrix(rotateMatrix);
            Vector xAxis(0,0,1);
            xAxis=(xAxis)*(rotateMatrix)*step;
            moveEdge(xAxis.x,xAxis.y,xAxis.z);
            object->partialSubdivision();
        }
                else if(m_mode==SelectionMode::Face && m_target)
        {
            Object *object=m_objectList[m_target];
            Quaternion angle(-object->m_rotation.w,Vector(object->m_rotation));
            Matrix rotateMatrix;
            angle.getMatrix(rotateMatrix);
            Vector xAxis(0,0,1);
            xAxis=(xAxis)*(rotateMatrix)*step;
            moveFace(xAxis.x,xAxis.y,xAxis.z);
            object->partialSubdivision();
        }

    }

    void axisXScale(float step)
    {
        if(m_mode==SelectionMode::Object && m_target==0)
        {
            for(unsigned int i=0;i<m_selection.size();++i)
            {
                Object *object=m_objectList[m_selection[i]];
                if(object)
                {
                    Quaternion angle(-object->m_rotation.w,Vector(object->m_rotation));
                    Matrix rotateMatrix;
                    angle.getMatrix(rotateMatrix);
                    Vector xAxis(1,0,0);
                    xAxis=(xAxis)*(rotateMatrix);
                    object->m_scale+=step*xAxis*0.01f;
                }
            }
        }
        else if(m_mode==SelectionMode::Vertex && m_target)
        {
            Object *object=m_objectList[m_target];
            Quaternion original(object->m_rotation.w,Vector(object->m_rotation));
            Quaternion back(-object->m_rotation.w,Vector(object->m_rotation));
            Matrix originalMatrix;
            Matrix backMatrix;
            original.getMatrix(originalMatrix);
            back.getMatrix(backMatrix);
            scaleVertex(m_axisCursor->m_position,originalMatrix,backMatrix,(1+step*0.1f),1);
            object->partialSubdivision();
        }
        else if(m_mode==SelectionMode::Edge && m_target)
        {
            Object *object=m_objectList[m_target];
            Quaternion original(object->m_rotation.w,Vector(object->m_rotation));
            Quaternion back(-object->m_rotation.w,Vector(object->m_rotation));
            Matrix originalMatrix;
            Matrix backMatrix;
            original.getMatrix(originalMatrix);
            back.getMatrix(backMatrix);
            scaleEdge(m_axisCursor->m_position,originalMatrix,backMatrix,(1+step*0.1f),1);
            object->partialSubdivision();
        }
        else if(m_mode==SelectionMode::Face && m_target)
        {
            Object *object=m_objectList[m_target];
            Quaternion original(object->m_rotation.w,Vector(object->m_rotation));
            Quaternion back(-object->m_rotation.w,Vector(object->m_rotation));
            Matrix originalMatrix;
            Matrix backMatrix;
            original.getMatrix(originalMatrix);
            back.getMatrix(backMatrix);
            scaleFace(m_axisCursor->m_position,originalMatrix,backMatrix,(1+step*0.1f),1);
            object->partialSubdivision();
        }
    }

    void axisYScale(float step)
    {
        if(m_mode==SelectionMode::Object && m_target==0)
        {
            for(unsigned int i=0;i<m_selection.size();++i)
            {
                Object *object=m_objectList[m_selection[i]];
                if(object)
                {
                    Quaternion angle(-object->m_rotation.w,Vector(object->m_rotation));
                    Matrix rotateMatrix;
                    angle.getMatrix(rotateMatrix);
                    Vector yAxis(0,1,0);
                    yAxis=(yAxis)*(rotateMatrix);
                    object->m_scale+=step*yAxis*0.01f;
                }
            }
        }
        else if(m_mode==SelectionMode::Vertex && m_target)
        {
            Object *object=m_objectList[m_target];
            Quaternion original(object->m_rotation.w,Vector(object->m_rotation));
            Quaternion back(-object->m_rotation.w,Vector(object->m_rotation));
            Matrix originalMatrix;
            Matrix backMatrix;
            original.getMatrix(originalMatrix);
            back.getMatrix(backMatrix);
            //scaleVertex(originalMatrix*(m_axisCursor->m_position-object->m_center)+object->m_center,xAxis,(step*0.1));
            scaleVertex(m_axisCursor->m_position,originalMatrix,backMatrix,(1+step*0.1f),2);
            object->partialSubdivision();
        }
        else if(m_mode==SelectionMode::Edge && m_target)
        {
            Object *object=m_objectList[m_target];
            Quaternion original(object->m_rotation.w,Vector(object->m_rotation));
            Quaternion back(-object->m_rotation.w,Vector(object->m_rotation));
            Matrix originalMatrix;
            Matrix backMatrix;
            original.getMatrix(originalMatrix);
            back.getMatrix(backMatrix);
            //scaleVertex(originalMatrix*(m_axisCursor->m_position-object->m_center)+object->m_center,xAxis,(step*0.1));
            scaleEdge(m_axisCursor->m_position,originalMatrix,backMatrix,(1+step*0.1f),2);
            object->partialSubdivision();
        }
        else if(m_mode==SelectionMode::Face && m_target)
        {
            Object *object=m_objectList[m_target];
            Quaternion original(object->m_rotation.w,Vector(object->m_rotation));
            Quaternion back(-object->m_rotation.w,Vector(object->m_rotation));
            Matrix originalMatrix;
            Matrix backMatrix;
            original.getMatrix(originalMatrix);
            back.getMatrix(backMatrix);
            //scaleVertex(originalMatrix*(m_axisCursor->m_position-object->m_center)+object->m_center,xAxis,(step*0.1));
            scaleFace(m_axisCursor->m_position,originalMatrix,backMatrix,(1+step*0.1f),2);
            object->partialSubdivision();
        }
    }

    void axisZScale(float step)
    {
        if(m_mode==SelectionMode::Object && m_target==0)
        {
            for(unsigned int i=0;i<m_selection.size();++i)
            {
                Object *object=m_objectList[m_selection[i]];
                if(object)
                {
                    Quaternion angle(-object->m_rotation.w,Vector(object->m_rotation));
                    Matrix rotateMatrix;
                    angle.getMatrix(rotateMatrix);
                    Vector zAxis(0,0,1);
                    zAxis=(zAxis)*(rotateMatrix);
                    object->m_scale+=step*zAxis*0.01f;
                }
            }
        }
        else if(m_mode==SelectionMode::Vertex && m_target)
        {
            Object *object=m_objectList[m_target];
            Quaternion original(object->m_rotation.w,Vector(object->m_rotation));
                    Quaternion back(-object->m_rotation.w,Vector(object->m_rotation));
            Matrix originalMatrix;
            Matrix backMatrix;
            original.getMatrix(originalMatrix);
            back.getMatrix(backMatrix);

            //scaleVertex(originalMatrix*(m_axisCursor->m_position-object->m_center)+object->m_center,xAxis,(step*0.1));
            scaleVertex(m_axisCursor->m_position,originalMatrix,backMatrix,(1+step*0.1f),3);
            object->partialSubdivision();
        }
        else if(m_mode==SelectionMode::Edge && m_target)
        {
            Object *object=m_objectList[m_target];
            Quaternion original(object->m_rotation.w,Vector(object->m_rotation));
                    Quaternion back(-object->m_rotation.w,Vector(object->m_rotation));
            Matrix originalMatrix;
            Matrix backMatrix;
            original.getMatrix(originalMatrix);
            back.getMatrix(backMatrix);

            //scaleVertex(originalMatrix*(m_axisCursor->m_position-object->m_center)+object->m_center,xAxis,(step*0.1));
            scaleEdge(m_axisCursor->m_position,originalMatrix,backMatrix,(1+step*0.1f),3);
            object->partialSubdivision();
        }
        else if(m_mode==SelectionMode::Face && m_target)
        {
            Object *object=m_objectList[m_target];
            Quaternion original(object->m_rotation.w,Vector(object->m_rotation));
                    Quaternion back(-object->m_rotation.w,Vector(object->m_rotation));
            Matrix originalMatrix;
            Matrix backMatrix;
            original.getMatrix(originalMatrix);
            back.getMatrix(backMatrix);

            //scaleVertex(originalMatrix*(m_axisCursor->m_position-object->m_center)+object->m_center,xAxis,(step*0.1));
            scaleFace(m_axisCursor->m_position,originalMatrix,backMatrix,(1+step*0.1f),3);
            object->partialSubdivision();
        }
    }

    void axisRelease()
    {
        m_axisCursor->m_rotation.null();
    }

    void ctrlDrag(Vector &horizontal,Vector &vertical,int x,int y,bool isExtrude)
    {
        if(!m_selection.empty())
        {
            GLdouble ox;
            GLdouble oy;
            GLdouble oz;
            GLdouble winx;
            GLdouble winy;
            GLdouble winz;
            GLint viewport[4];
            glGetIntegerv(GL_VIEWPORT, viewport);
            GLdouble modelMatrix[16];
            /*glGetDoublev(GL_MODELVIEW_MATRIX,modelMatrix);
            GLdouble projMatrix[16];
            glGetDoublev(GL_PROJECTION_MATRIX,projMatrix);
            gluProject(0,0,0,modelMatrix,projMatrix,viewport,&ox,&oy,&oz);
            oy=(GLdouble)viewport[3]-(GLint)oy;
            gluProject(horizontal.x,horizontal.y,horizontal.z,modelMatrix,projMatrix,viewport,&winx,&winy,&winz);
            winy=(GLdouble)viewport[3]-winy;
            Vector projUnitX((float)(winx-ox),(float)(winy-oy),0.0f);
            gluProject(vertical.x,vertical.y,vertical.z,modelMatrix,projMatrix,viewport,&winx,&winy,&winz);
            winy=(GLdouble)viewport[3]-winy;
            Vector projUnitY((float)(winx-ox),(float)(winy-oy),0.0f);
            Vector offset(horizontal*(float)x/projUnitX.length()+vertical*(float)y/projUnitY.length());
            if(m_mode==SelectionMode::Object)
            {
                for(unsigned int i=0;i<m_selection.size();++i)
                {
                    m_objectList[m_selection[i]]->m_position-=offset;
                }
            }
            else if(m_mode==SelectionMode::Vertex && m_target)
            {
                moveVertex(-offset.x,-offset.y,-offset.z);
                m_objectList[m_target]->partialSubdivision();
            }
            else if(m_mode==SelectionMode::Edge && m_target)
            {
                if(isExtrude)
                {
                    m_objectList[m_target]->clearPSCache();
                    extrudeEdge(-offset.x,-offset.y,-offset.z);
                    m_objectList[m_target]->updateSubdivision();
                    m_objectList[m_target]->buildPSCacheFromEID(selection);
                }
                else
                {
                    moveEdge(-offset.x,-offset.y,-offset.z);
                    m_objectList[m_target]->partialSubdivision();
                }
            }
            else if(m_mode==SelectionMode::Face && m_target)
            {
                if(isExtrude)
                {
                    m_objectList[m_target]->clearPSCache();
                    extrudeFaceGroup(-offset.x,-offset.y,-offset.z);
                    m_objectList[m_target]->updateSubdivision();
                    m_objectList[m_target]->buildPSCacheFromFID(selection);
                }
                else
                {
                    moveFace(-offset.x,-offset.y,-offset.z);
                    m_objectList[m_target]->partialSubdivision();
                }
            }*/
        }
    }

    void axisDrag(const float &step,const int &cursorMode,float &rx,float &ry)
    {
        GLint viewport[4];
        glGetIntegerv(GL_VIEWPORT, viewport);
        GLdouble modelMatrix[16];
        /*glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glTranslatef(m_axisCursor->m_position.x,m_axisCursor->m_position.y,m_axisCursor->m_position.z);
        glRotatef(m_axisCursor->m_rotation.w,m_axisCursor->m_rotation.x,m_axisCursor->m_rotation.y,m_axisCursor->m_rotation.z);
        glGetDoublev(GL_MODELVIEW_MATRIX,modelMatrix);
        glPopMatrix();
        GLdouble projMatrix[16];
        glGetDoublev(GL_PROJECTION_MATRIX,projMatrix);
        GLdouble ox;
        GLdouble oy;
        GLdouble oz;
        GLdouble winx;
        GLdouble winy;
        GLdouble winz;
        if(cursorm_mode==1)
        {
            //xÖá
            if(m_axisCursor->m_m_mode==AxisCursorMode::MoveAxis)
            {
                gluProject(0,0,0,modelMatrix,projMatrix,viewport,&ox,&oy,&oz);
                oy=(GLdouble)viewport[3]-(GLint)oy;
                gluProject(1,0,0,modelMatrix,projMatrix,viewport,&winx,&winy,&winz);
                winy=(GLdouble)viewport[3]-winy;
                Vector projUnit((float)(winx-ox),(float)(winy-oy),0.0f);
                axisXMove(step/projUnit.length());
            }
            else if(m_axisCursor->m_m_mode==AxisCursorMode::RotateAxis)
            {
                gluProject(0,0,0,modelMatrix,projMatrix,viewport,&ox,&oy,&oz);
                oy=(GLdouble)viewport[3]-(GLint)oy;
                gluProject(0,1,0,modelMatrix,projMatrix,viewport,&winx,&winy,&winz);
                winy=(GLdouble)viewport[3]-winy;
                Vector projUnit((float)(winx-ox),(float)(winy-oy),0.0f);
                axisXRotate(step/projUnit.length());
                //ÕâÀïÐÞ¸ÄÐÂÏòÁ¿
                rx=(float)(winx-ox);
                ry=(float)(winy-oy);
            }
            else if(m_axisCursor->m_m_mode==AxisCursorMode::ScaleAxis)
            {
                gluProject(0,0,0,modelMatrix,projMatrix,viewport,&ox,&oy,&oz);
                oy=(GLdouble)viewport[3]-(GLint)oy;
                gluProject(1,0,0,modelMatrix,projMatrix,viewport,&winx,&winy,&winz);
                winy=(GLdouble)viewport[3]-winy;
                Vector projUnit((float)(winx-ox),(float)(winy-oy),0.0f);
                //m_axisCursor->m_position.x+=step/projUnit.length();
                axisXScale(step/projUnit.length());
            }
        }
        else if(cursorm_mode==2)
        {
            if(m_axisCursor->m_m_mode==AxisCursorMode::MoveAxis)
            {
                gluProject(0,0,0,modelMatrix,projMatrix,viewport,&ox,&oy,&oz);
                oy=(GLdouble)viewport[3]-oy;
                gluProject(0,1,0,modelMatrix,projMatrix,viewport,&winx,&winy,&winz);
                winy=(GLdouble)viewport[3]-winy;
                Vector projUnit((float)(winx-ox),(float)(winy-oy),0.0f);
                axisYMove(step/projUnit.length());
            }
            else if(m_axisCursor->m_m_mode==AxisCursorMode::RotateAxis)
            {
                gluProject(0,0,0,modelMatrix,projMatrix,viewport,&ox,&oy,&oz);
                oy=(GLdouble)viewport[3]-(GLint)oy;
                gluProject(0,0,1,modelMatrix,projMatrix,viewport,&winx,&winy,&winz);
                winy=(GLdouble)viewport[3]-winy;
                Vector projUnit((float)(winx-ox),(float)(winy-oy),0.0f);
                axisYRotate(step/projUnit.length());
                //ÕâÀïÐÞ¸ÄÐÂÏòÁ¿
                rx=(float)(winx-ox);
                ry=(float)(winy-oy);
            }
            else if(m_axisCursor->m_m_mode==AxisCursorMode::ScaleAxis)
            {
                gluProject(0,0,0,modelMatrix,projMatrix,viewport,&ox,&oy,&oz);
                oy=(GLdouble)viewport[3]-(GLint)oy;
                gluProject(0,1,0,modelMatrix,projMatrix,viewport,&winx,&winy,&winz);
                winy=(GLdouble)viewport[3]-winy;
                Vector projUnit((float)(winx-ox),(float)(winy-oy),0.0f);
                //m_axisCursor->m_position.y+=step/projUnit.length();
                axisYScale(step/projUnit.length());
            }
        }
        else if(cursorm_mode==3)
        {
            if(m_axisCursor->m_m_mode==AxisCursorMode::MoveAxis)
            {
                gluProject(0,0,0,modelMatrix,projMatrix,viewport,&ox,&oy,&oz);
                oy=(GLdouble)viewport[3]-oy;
                gluProject(0,0,1,modelMatrix,projMatrix,viewport,&winx,&winy,&winz);
                winy=(GLdouble)viewport[3]-winy;
                Vector projUnit((float)(winx-ox),(float)(winy-oy),0.0f);
                axisZMove(step/projUnit.length());
            }
            else if(m_axisCursor->m_m_mode==AxisCursorMode::RotateAxis)
            {
                gluProject(0,0,0,modelMatrix,projMatrix,viewport,&ox,&oy,&oz);
                oy=(GLdouble)viewport[3]-(GLint)oy;
                gluProject(1,0,0,modelMatrix,projMatrix,viewport,&winx,&winy,&winz);
                winy=(GLdouble)viewport[3]-winy;
                Vector projUnit((float)(winx-ox),(float)(winy-oy),0.0f);
        axisZRotate(step/projUnit.length());
                //ÕâÀïÐÞ¸ÄÐÂÏòÁ¿
                rx=(float)(winx-ox);
                ry=(float)(winy-oy);
            }
            else if(m_axisCursor->m_m_mode==AxisCursorMode::ScaleAxis)
            {
                gluProject(0,0,0,modelMatrix,projMatrix,viewport,&ox,&oy,&oz);
                oy=(GLdouble)viewport[3]-(GLint)oy;
                gluProject(0,0,1,modelMatrix,projMatrix,viewport,&winx,&winy,&winz);
                winy=(GLdouble)viewport[3]-winy;
                Vector projUnit((float)(winx-ox),(float)(winy-oy),0.0f);
                //m_axisCursor->m_position.z+=step/projUnit.length();
                axisZScale(step/projUnit.length());
            }
        }
        else if(cursorm_mode==4)
        {

        }*/
    }

    void unSubdivide()
    {
        if(m_mode==SelectionMode::Object && m_target==0)
        {
            if(!m_selection.empty())
            {
                for(unsigned int i=0;i<m_selection.size();++i)
                {
                    m_objectList[m_selection[i]]->unSubdivide();
                }
            }
        }
        else
        {
            if(m_mode==SelectionMode::Vertex && m_target)
            {
                m_objectList[m_target]->clearPSCache();
                m_objectList[m_target]->unSubdivide();
                if(!m_selection.empty())
                {
                    m_objectList[m_target]->clearPSCache();
                    m_objectList[m_target]->buildPSCacheFromVID(m_selection);
                }
            }
        }
    }

    void selectSingleSideEdge(unsigned int x1,unsigned int y1,unsigned int x2,unsigned int y2,unsigned int height,bool isAppend=false)
    {
        if(!isAppend)
        {
            clearSelection();
        }
        Object *object=m_objectList[m_target];
        if(object->edgeCount()<16777215)
        {
            glClearColor(1.0f,1.0f,1.0f,1.0f);
            glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);
            glDisable(GL_DITHER);
            /*glDisable(GL_LIGHTING);
            glDisable(GL_TEXTURE_2D);
            glDisable(GL_ALPHA_TEST);
            glDisable(GL_BLEND);
                glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glTranslatef(object->m_position.x,object->m_position.y,object->m_position.z);
        glTranslatef(object->m_center.x,object->m_center.y,object->m_center.z);
        glRotatef(object->m_rotation.w,object->m_rotation.x,object->m_rotation.y,object->m_rotation.z);
        glScalef(object->m_scale.x,object->m_scale.y,object->m_scale.z);
        glTranslatef(-object->m_center.x,-object->m_center.y,-object->m_center.z);


            glColorMask(GL_FALSE,GL_FALSE,GL_FALSE,GL_FALSE);
            glEnable(GL_POLYGON_OFFSET_FILL);
            glPolygonOffset(1.0f,1.0f);
            for(unsigned int i=1;i<object->faceCount();++i)
            {
                Face *face=object->face(i);
                if(face)
                {
                    struct ColorID colorID=(*(struct ColorID*)&(i));
                    glColor4ub(colorID.r, colorID.g,colorID.b,colorID.a);
                    glBegin(GL_POLYGON);
                    for(unsigned int e=0;e<face->m_edge.size();++e)
                    {
                        if(face->m_edge[e]>0)
                        {
                            Vertex *v=object->vertex(object->edge(face->m_edge[e])->m_start);
                            glVertex3f(v->m_position.x,v->m_position.y,v->m_position.z);
                        }
                        else
                        {
                            Vertex *v=object->vertex(object->edge(-face->m_edge[e])->m_end);
                            glVertex3f(v->m_position.x,v->m_position.y,v->m_position.z);
                        }
                    }
                    glEnd();
                }
            }
            glDisable(GL_POLYGON_OFFSET_FILL);
            glColorMask(GL_TRUE,GL_TRUE,GL_TRUE,GL_TRUE);
            glBegin(GL_LINES);
            for(unsigned int i=1;i<object->edgeCount();++i)
            {
                Edge *edge=object->edge(i);
                if(edge)
                {
                    struct ColorID colorID=(*(struct ColorID*)&(i));
                    glColor4ub(colorID.r, colorID.g,colorID.b,colorID.a);
                    Vertex *start=object->vertex(edge->m_start);
                    Vertex *end=object->vertex(edge->m_end);
                    glVertex3f(start->m_position.x,start->m_position.y,start->m_position.z);
                    glVertex3f(end->m_position.x,end->m_position.y,end->m_position.z);
                }
            }
            glEnd();
            glFlush();
            glPopMatrix();
            unsigned int sw=(x2-x1)>5?(x2-x1):5;
            unsigned int sh=(y2-y1)>5?(y2-y1):5;
            unsigned int length=sw*sh;
            struct ColorID *pixel=new struct ColorID[length];
            glReadPixels(x1,height-y2,sw,sh, GL_RGBA, GL_UNSIGNED_BYTE, pixel);
            for(unsigned int e=0;e<length;++e)
            {
                pixel[e].a=0;
                unsigned int result=(*(unsigned int*)&(pixel[e]));
                if(result<16777215)
                {
                    if(result<object->edgeCount()&&object->edge(result))
                    {
                        selectionPush(object->edge(result));
                    }
                }
            }
            delete pixel;
            glEnable(GL_BLEND);
            glEnable(GL_ALPHA_TEST);
            glEnable(GL_TEXTURE_2D);
            glEnable(GL_LIGHTING);
            glEnable(GL_DITHER);
            glClearColor(128.0f/255.0f,128.0f/255.0f,128.0f/255.0f,1.0f);
            updateAxisCursor();
            if(!m_selection.empty())
            {
                m_objectList[m_target]->clearPSCache();
                object->buildPSCacheFromEID(selection);
            }*/
        }
    }

    void selectSingleSideFace(unsigned int x1,unsigned int y1,unsigned int x2,unsigned int y2,unsigned int height,bool isAppend=false)
    {
        if(!isAppend)
        {
            clearSelection();
        }
        Object *object=m_objectList[m_target];
        if(object->faceCount()<16777215)
        {
            glClearColor(1.0f,1.0f,1.0f,1.0f);
            glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);
            glDisable(GL_DITHER);
            /*glDisable(GL_LIGHTING);
            glDisable(GL_TEXTURE_2D);
            glDisable(GL_ALPHA_TEST);
            glDisable(GL_BLEND);
            glMatrixMode(GL_MODELVIEW);
            glPushMatrix();
            glTranslatef(object->m_position.x,object->m_position.y,object->m_position.z);
            glTranslatef(object->m_center.x,object->m_center.y,object->m_center.z);
            glRotatef(object->m_rotation.w,object->m_rotation.x,object->m_rotation.y,object->m_rotation.z);
            glScalef(object->m_scale.x,object->m_scale.y,object->m_scale.z);
            glTranslatef(-object->m_center.x,-object->m_center.y,-object->m_center.z);

            for(unsigned int i=1;i<object->faceCount();++i)
            {
                Face *face=object->face(i);
                if(face)
                {
                    struct ColorID colorID=(*(struct ColorID*)&(i));
                    glColor4ub(colorID.r, colorID.g,colorID.b,colorID.a);
                    glBegin(GL_POLYGON);
                    for(unsigned int e=0;e<face->m_edge.size();++e)
                    {
                        if(face->m_edge[e]>0)
                        {
                            Vertex *v=object->vertex(object->edge(face->m_edge[e])->m_start);
                            glVertex3f(v->m_position.x,v->m_position.y,v->m_position.z);
                        }
                        else
                        {
                            Vertex *v=object->vertex(object->edge(-face->m_edge[e])->m_end);
                            glVertex3f(v->m_position.x,v->m_position.y,v->m_position.z);
                        }
                    }
                    glEnd();
                }
            }
            glFlush();
            glPopMatrix();
            unsigned int sw=(x2-x1)>5?(x2-x1):5;
            unsigned int sh=(y2-y1)>5?(y2-y1):5;
            unsigned int length=sw*sh;
            struct ColorID *pixel=new struct ColorID[length];
            glReadPixels(x1,height-y2,sw,sh, GL_RGBA, GL_UNSIGNED_BYTE, pixel);
            qDebug() << glGetError();
            for(unsigned int e=0;e<length;e+=2)
            {
                pixel[e].a=0;
                unsigned int result=(*(unsigned int*)&(pixel[e]));
                if(result<16777215)
                {
                    if(result<object->faceCount()&&object->face(result))
                    {
                        selectionPush(object->face(result));
                    }


                }
                qDebug() << "result: " << result;
            }
            delete pixel;
            glEnable(GL_BLEND);
            glEnable(GL_ALPHA_TEST);
            glEnable(GL_TEXTURE_2D);
            glEnable(GL_LIGHTING);
            glEnable(GL_DITHER);
            glClearColor(128.0f/255.0f,128.0f/255.0f,128.0f/255.0f,1.0f);
            updateAxisCursor();
            if(!m_selection.empty())
            {
                m_objectList[m_target]->clearPSCache();
                object->buildPSCacheFromFID(selection);
            }*/
        }
    }

    void selectSingleSideObject(unsigned int x1,unsigned int y1,unsigned int x2,unsigned int y2,unsigned int height,bool isAppend=false)
    {
        qDebug() << "single side object select";
        if(!isAppend)
        {
            clearSelection();
        }
        if(m_objectList.size()<16777215)
        {
            //glClearColor(1.0f,1.0f,1.0f,1.0f);
            //glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);
            glDisable(GL_DITHER);
            /*glDisable(GL_LIGHTING);
            glDisable(GL_TEXTURE_2D);
            glDisable(GL_ALPHA_TEST);
            glDisable(GL_BLEND);
            for(unsigned int i=1;i<m_objectList.size();++i)
            {
                if(m_objectList[i])
                {
                    Object *object=m_objectList[i];
                    struct ColorID colorID=(*(struct ColorID*)&(i));
                    glColor4ub(colorID.r, colorID.g,colorID.b,colorID.a);
                    object->selectionRenderObject();
                }
            }
            glFlush();
            unsigned int sw=(x2-x1)>5?(x2-x1):5;
            unsigned int sh=(y2-y1)>5?(y2-y1):5;
            unsigned int length=sw*sh;
            struct ColorID *pixel=new struct ColorID[length];
            glReadPixels(x1,height-y2,sw,sh, GL_RGBA, GL_UNSIGNED_BYTE, pixel);

         //   QImage image((uchar*) pixel, sw, sh,QImage::Format_RGBA8888);
         //   image.save("haha.png");

            for(unsigned int e=0;e<length;e+=2)
            {
                pixel[e].a=0;
                unsigned int result=(*(unsigned int*)&(pixel[e]));
                if(result<16777215)
                {
                    if(result<m_objectList.size()&&m_objectList[result])
                    {
                        selectionPush(m_objectList[result]);
                    }
                }
            }
            delete pixel;
            glEnable(GL_BLEND);
            glEnable(GL_ALPHA_TEST);
            glEnable(GL_TEXTURE_2D);
            glEnable(GL_LIGHTING);
            glEnable(GL_DITHER);
            glClearColor(128.0f/255.0f,128.0f/255.0f,128.0f/255.0f,1.0f);*/
            updateAxisCursor();
        }
    }

    void changeSelectionMode(SelectionMode::__Enum mode)
    {
        if(m_mode==SelectionMode::Object && m_target==0)
        {
            if(m_selection.empty())
            {

            }
            else
            {
                m_target=m_selection[0];
                clearSelection();//ÒòÎªÊÇºÍÑ¡ÔñÀ?ÐÍÏ?¹ØµÄ£¬¼Ç×¡Ò»¶¨ÒªÏÈÇå¿Õ£¬·ñÔòÈç¹ûÀ?ÐÍ¸Ä±ä£¬Çå¿Õ¾Í²»Õý³£ÁË
                m_mode=mode;
            }
        }
        else
        {
            if(m_target)
            {
                if(m_mode==SelectionMode::Split)
                {
                    endSplit();
                }
                clearSelection();
                m_objectList[m_target]->clearPSCache();
                if((m_mode=mode)==SelectionMode::Object)
                {
                    m_target=0;
                }
            }
            else
            {

            }
        }
        updateAxisCursor();
    }

    void unhideAll()
    {
        for(unsigned int i=1;i<m_objectList.size();++i)
        {
            if(m_objectList[i])
            {
                m_objectList[i]->m_isHide=false;
            }
        }
    }

    void selectAll()
    {
        if(m_mode==SelectionMode::Object)
        {
            clearSelection();
            for(unsigned int i=1;i<m_objectList.size();++i)
            {
                if(m_objectList[i])
                {
                    selectionPush(m_objectList[i]);
                }
            }
        }
    }

    void hideUnselection()
    {
        if(m_mode==SelectionMode::Object)
        {
            for(unsigned int i=1;i<m_objectList.size();++i)
            {
                if(m_objectList[i] && !(m_objectList[i]->m_isSelected))
                {
                    m_objectList[i]->m_isHide=true;
                }
            }
        }
        else if(m_target>0)
        {
            for(unsigned int i=1;i<m_objectList.size();++i)
            {
                if(m_objectList[i])
                {
                    m_objectList[i]->m_isHide=true;
                }
            }

            if(m_objectList[m_target])
            {
                m_objectList[m_target]->m_isHide=false;
            }
        }
    }

    void hideSelection()
    {
        if(m_mode==SelectionMode::Object)
        {
            for(unsigned int i=0;i<m_selection.size();++i)
            {
                if(m_objectList[m_selection[i]])
                {
                    m_objectList[m_selection[i]]->m_isHide=true;
                }
            }
            clearSelection();
        }
        else if(m_target>0 && m_objectList[m_target])
        {
            m_objectList[m_target]->m_isHide=true;
            clearSelection();
            m_objectList[m_target]->clearPSCache();
            changeSelectionMode(SelectionMode::Object);
        }
    }

    void onPaint();
    void drawCutVertex();
    void updateAxisCursor();
    void drawCursor(CameraMode::__Enum cameraMode,const Vector &eye);
    void drawSelectedVertex();
    void drawSelectedEdge();
    void drawSelectedFace();

    void openFromPWB(const char *fileName);
    void loadFromPWB(const char *fileName);
    void saveToFileOBJ(const char *fileName);
    void saveToFilePWB(const char *fileName);

    void invertSelection()
    {
        if(m_mode==SelectionMode::Object)
        {
            std::vector<unsigned int> objectToBeSelected;
            objectToBeSelected.reserve(100);
            for(unsigned int i=1;i<m_objectList.size();++i)
            {
                if(m_objectList[i] && !(m_objectList[i]->m_isSelected))
                {
                    objectToBeSelected.push_back(i);
                }
            }

            clearSelection();

            for(unsigned int i=0;i<objectToBeSelected.size();++i)
            {
                selectionPush(m_objectList[objectToBeSelected[i]]);
            }

            updateAxisCursor();
        }
    }
public:
    ~Scene(void);
    static Scene *scene;
};
