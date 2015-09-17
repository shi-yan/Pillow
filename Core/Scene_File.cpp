#include "Scene.h"

void Scene::saveToFilePWB(const char *fileName)
{
    FILE *fp=fopen(fileName,"wb");
    struct SceneInfo sceneInfo;
    sceneInfo.ID[0]='P';
    sceneInfo.ID[1]='W';
    sceneInfo.ID[2]='B';
    sceneInfo.ID[3]=0;
    sceneInfo.version=1;
    sceneInfo.objectCount=theObjectList.size();
    fwrite(&sceneInfo,sizeof(sceneInfo),1,fp);
    for(unsigned int i=1;i<sceneInfo.objectCount;++i)
    {
        Object *theObject=theObjectList[i];
        if(theObject)
        {
            Object::ObjectInfo objectInfo=theObjectList[i]->getObjectInfo();
            fwrite(&objectInfo,sizeof(objectInfo),1,fp);
            for(unsigned int e=1;e<objectInfo.vertexCount;++e)
            {
                Vertex *theVertex=theObject->vertex(e);
                if(theVertex)
                {
                    Vertex::VertexInfo vertexInfo=theVertex->getVertexInfo();
                    fwrite(&vertexInfo,sizeof(vertexInfo),1,fp);
                    fwrite(&(theVertex->m_adjacentEdgeList[0]),sizeof(unsigned int),vertexInfo.adjacentCount,fp);
                }
                else
                {
                    Vertex::VertexInfo vertexInfo;
                    vertexInfo.index=0;
                    vertexInfo.adjacentCount=0;
                    fwrite(&vertexInfo,sizeof(vertexInfo),1,fp);
                }
            }

            for(unsigned int e=1;e<objectInfo.edgeCount;++e)
            {
                Edge *theEdge=theObject->edge(e);
                if(theEdge)
                {
                    Edge::EdgeInfo edgeInfo=theEdge->getEdgeInfo();
                    fwrite(&edgeInfo,sizeof(edgeInfo),1,fp);
                }
                else
                {
                    Edge::EdgeInfo edgeInfo;
                    edgeInfo.index=0;
                    fwrite(&edgeInfo,sizeof(edgeInfo),1,fp);
                }
            }

            for(unsigned int e=1;e<objectInfo.faceCount;++e)
            {
                Face *theFace=theObject->face(e);
                if(theFace)
                {
                    Face::FaceInfo faceInfo=theFace->getFaceInfo();
                    fwrite(&faceInfo,sizeof(faceInfo),1,fp);
                    fwrite(&(theFace->m_edge[0]),sizeof(unsigned int),faceInfo.edgeCount,fp);
                }
                else
                {
                    Face::FaceInfo faceInfo;
                    faceInfo.index=0;
                    fwrite(&faceInfo,sizeof(faceInfo),1,fp);
                }
            }
        }
        else
        {
            Object::ObjectInfo objectInfo;
            objectInfo.index=0;
            objectInfo.vertexCount=0;
            objectInfo.edgeCount=0;
            objectInfo.faceCount=0;
            fwrite(&objectInfo,sizeof(objectInfo),1,fp);
        }
    }
    fclose(fp);
}

void Scene::saveToFileOBJ(const char *fileName)
{
    FILE *fp=fopen(fileName,"w");
    unsigned int vertexBase=0;
    for(unsigned int h=1;h<theObjectList.size();++h)
    {
        Object *theObject=theObjectList[h];
        if(theObject)
        {
            unsigned int i=1;
            for(i=1;i<theObject->vertexCount();i++)
            {
                if(theObject->vertex(i)==NULL)
                {
                    fprintf(fp,"v %f %f %f\n",0,0,0);
                }
                else
                {
                    Vertex *theVertex=theObject->vertex(i);
                    fprintf(fp,"v %f %f %f\n",theVertex->m_position.x,theVertex->m_position.y,theVertex->m_position.z);
                }
            }
            unsigned int tempBase=i-1;
            fprintf(fp,"g box01\n");
            for(unsigned int i=1;i<theObject->faceCount();i++)
            {
                Face *theFace=theObject->face(i);
                if(theFace)
                {
                    fprintf(fp,"f ");
                    for(unsigned int e=0;e<theFace->m_edge.size();e++)
                    {
                        if(theFace->m_edge[e]<0)
                        {
                            fprintf(fp,"%d ",theObject->edge(-theFace->m_edge[e])->m_end+vertexBase);
                        }
                        else
                        {
                            fprintf(fp,"%d ",theObject->edge(theFace->m_edge[e])->m_start+vertexBase);
                        }
                    }
                    fprintf(fp,"\n");
                }
            }
            fprintf(fp,"g\n");
            vertexBase=tempBase;
        }
    }
    fclose(fp);
}

void Scene::loadFromPWB(const char *fileName)
{
    FILE *fp=fopen(fileName,"rb");
    struct SceneInfo sceneInfo;
    fread(&sceneInfo,sizeof(sceneInfo),1,fp);
    for(unsigned int i=1;i<sceneInfo.objectCount;++i)
    {
        Object::ObjectInfo objectInfo;
        fread(&objectInfo,sizeof(objectInfo),1,fp);
        if(objectInfo.index>0)
        {
            Object *theObject=new Object("new object");
            theObject->m_index=objectInfo.index;
            theObject->m_center.vec(objectInfo.centerX,objectInfo.centerY,objectInfo.centerZ);
            theObject->m_position.vec(objectInfo.positionX,objectInfo.positionY,objectInfo.positionZ);
            theObject->m_rotation.vec(objectInfo.rotationX,objectInfo.rotationY,objectInfo.rotationZ);
            theObject->m_scale.vec(objectInfo.scaleX,objectInfo.scaleY,objectInfo.scaleZ);
            theObject->m_matAmbient[0]=objectInfo.matAmbient[0];
            theObject->m_matAmbient[1]=objectInfo.matAmbient[1];
            theObject->m_matAmbient[2]=objectInfo.matAmbient[2];
            theObject->m_matAmbient[3]=objectInfo.matAmbient[3];

            theObject->m_matDiffuse[0]=objectInfo.matDiffuse[0];
            theObject->m_matDiffuse[1]=objectInfo.matDiffuse[1];
            theObject->m_matDiffuse[2]=objectInfo.matDiffuse[2];
            theObject->m_matDiffuse[3]=objectInfo.matDiffuse[3];

            theObject->m_matSpecular[0]=objectInfo.matSpecular[0];
            theObject->m_matSpecular[1]=objectInfo.matSpecular[1];
            theObject->m_matSpecular[2]=objectInfo.matSpecular[2];
            theObject->m_matSpecular[3]=objectInfo.matSpecular[3];

            theObject->m_matEmission[0]=objectInfo.matEmission[0];
            theObject->m_matEmission[1]=objectInfo.matEmission[1];
            theObject->m_matEmission[2]=objectInfo.matEmission[2];
            theObject->m_matEmission[3]=objectInfo.matEmission[3];

            theObject->m_matShininess[0]=objectInfo.matShininess[0];
            theObject->m_matShininess[1]=objectInfo.matShininess[1];
            theObject->m_matShininess[2]=objectInfo.matShininess[2];
            theObject->m_matShininess[3]=objectInfo.matShininess[3];

            for(unsigned int e=1;e<objectInfo.vertexCount;++e)
            {
                Vertex::VertexInfo vertexInfo;
                fread(&vertexInfo,sizeof(vertexInfo),1,fp);
                if(vertexInfo.index>0)
                {
                    Vertex *theVertex=new Vertex();
                    theVertex->m_index=vertexInfo.index;
                    theVertex->m_position.vec(vertexInfo.x,vertexInfo.y,vertexInfo.z);
                    theVertex->m_normal.vec(vertexInfo.nx,vertexInfo.ny,vertexInfo.nz);
                    for(unsigned int h=0;h<vertexInfo.adjacentCount;++h)
                    {
                        unsigned int theAdj;
                        fread(&theAdj,sizeof(unsigned int),1,fp);
                        theVertex->m_adjacentEdgeList.push_back(theAdj);
                    }
                    theObject->directPushVertex(theVertex);
                }
                else
                {
                    theObject->directPushVertex(NULL);
                }
            }

            for(unsigned int e=1;e<objectInfo.edgeCount;++e)
            {
                Edge::EdgeInfo edgeInfo;
                fread(&edgeInfo,sizeof(edgeInfo),1,fp);
                if(edgeInfo.index>0)
                {
                    Edge *theEdge=new Edge(edgeInfo.start,edgeInfo.end);
                    theEdge->m_index=edgeInfo.index;
                    theEdge->m_left=edgeInfo.left;
                    theEdge->m_right=edgeInfo.right;
                    theObject->directPushEdge(theEdge);
                }
                else
                {
                    theObject->directPushEdge(NULL);
                }
            }

            for(unsigned int e=1;e<objectInfo.faceCount;++e)
            {
                Face::FaceInfo faceInfo;
                fread(&faceInfo,sizeof(faceInfo),1,fp);
                if(faceInfo.index>0)
                {
                    Face *theFace=new Face();
                    theFace->m_index=faceInfo.index;
                    theFace->m_normal.vec(faceInfo.nx,faceInfo.ny,faceInfo.nz);
                    for(unsigned int h=0;h<faceInfo.edgeCount;++h)
                    {
                        int theE;
                        fread(&theE,sizeof(int),1,fp);
                        theFace->m_edge.push_back(theE);
                    }
                    theObject->directPushFace(theFace);
                }
                else
                {
                    theObject->directPushFace(NULL);
                }
            }
            sceneObjectAdd(theObject);
        }
        else
        {
            theObjectList.pushNullS();
        }
    }
    fclose(fp);
}

void Scene::openFromPWB(const char *fileName)
{
    clearScene();
    loadFromPWB(fileName);
}
