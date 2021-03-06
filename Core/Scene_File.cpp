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
    sceneInfo.objectCount=m_objectList.size();
    fwrite(&sceneInfo,sizeof(sceneInfo),1,fp);
    for(unsigned int i=1;i<sceneInfo.objectCount;++i)
    {
        Object *object=m_objectList[i];
        if(object)
        {
            Object::ObjectInfo objectInfo=m_objectList[i]->getObjectInfo();
            fwrite(&objectInfo,sizeof(objectInfo),1,fp);
            for(unsigned int e=1;e<objectInfo.vertexCount;++e)
            {
                Vertex *vertex=object->vertex(e);
                if(vertex)
                {
                    Vertex::VertexInfo vertexInfo=vertex->getVertexInfo();
                    fwrite(&vertexInfo,sizeof(vertexInfo),1,fp);
                    fwrite(&(vertex->m_adjacentEdgeList[0]),sizeof(unsigned int),vertexInfo.adjacentCount,fp);
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
                Edge *edge=object->edge(e);
                if(edge)
                {
                    Edge::EdgeInfo edgeInfo=edge->getEdgeInfo();
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
                Face *face=object->face(e);
                if(face)
                {
                    Face::FaceInfo faceInfo=face->getFaceInfo();
                    fwrite(&faceInfo,sizeof(faceInfo),1,fp);
                    fwrite(&(face->m_edge[0]),sizeof(unsigned int),faceInfo.edgeCount,fp);
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
    for(unsigned int h=1;h<m_objectList.size();++h)
    {
        Object *object=m_objectList[h];
        if(object)
        {
            unsigned int i=1;
            for(i=1;i<object->vertexCount();i++)
            {
                if(object->vertex(i)==NULL)
                {
                    fprintf(fp,"v %f %f %f\n",0,0,0);
                }
                else
                {
                    Vertex *vertex=object->vertex(i);
                    fprintf(fp,"v %f %f %f\n",vertex->m_position.x,vertex->m_position.y,vertex->m_position.z);
                }
            }
            unsigned int tempBase=i-1;
            fprintf(fp,"g box01\n");
            for(unsigned int i=1;i<object->faceCount();i++)
            {
                Face *face=object->face(i);
                if(face)
                {
                    fprintf(fp,"f ");
                    for(unsigned int e=0;e<face->m_edge.size();e++)
                    {
                        if(face->m_edge[e]<0)
                        {
                            fprintf(fp,"%d ",object->edge(-face->m_edge[e])->m_end+vertexBase);
                        }
                        else
                        {
                            fprintf(fp,"%d ",object->edge(face->m_edge[e])->m_start+vertexBase);
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
            Object *object=new Object("new object");
            object->m_index=objectInfo.index;
            object->m_center.vec(objectInfo.centerX,objectInfo.centerY,objectInfo.centerZ);
            object->m_position.vec(objectInfo.positionX,objectInfo.positionY,objectInfo.positionZ);
            object->m_rotation.vec(objectInfo.rotationX,objectInfo.rotationY,objectInfo.rotationZ);
            object->m_scale.vec(objectInfo.scaleX,objectInfo.scaleY,objectInfo.scaleZ);
            object->m_matAmbient[0]=objectInfo.matAmbient[0];
            object->m_matAmbient[1]=objectInfo.matAmbient[1];
            object->m_matAmbient[2]=objectInfo.matAmbient[2];
            object->m_matAmbient[3]=objectInfo.matAmbient[3];

            object->m_matDiffuse[0]=objectInfo.matDiffuse[0];
            object->m_matDiffuse[1]=objectInfo.matDiffuse[1];
            object->m_matDiffuse[2]=objectInfo.matDiffuse[2];
            object->m_matDiffuse[3]=objectInfo.matDiffuse[3];

            object->m_matSpecular[0]=objectInfo.matSpecular[0];
            object->m_matSpecular[1]=objectInfo.matSpecular[1];
            object->m_matSpecular[2]=objectInfo.matSpecular[2];
            object->m_matSpecular[3]=objectInfo.matSpecular[3];

            object->m_matEmission[0]=objectInfo.matEmission[0];
            object->m_matEmission[1]=objectInfo.matEmission[1];
            object->m_matEmission[2]=objectInfo.matEmission[2];
            object->m_matEmission[3]=objectInfo.matEmission[3];

            object->m_matShininess[0]=objectInfo.matShininess[0];
            object->m_matShininess[1]=objectInfo.matShininess[1];
            object->m_matShininess[2]=objectInfo.matShininess[2];
            object->m_matShininess[3]=objectInfo.matShininess[3];

            for(unsigned int e=1;e<objectInfo.vertexCount;++e)
            {
                Vertex::VertexInfo vertexInfo;
                fread(&vertexInfo,sizeof(vertexInfo),1,fp);
                if(vertexInfo.index>0)
                {
                    Vertex *vertex=new Vertex();
                    vertex->m_index=vertexInfo.index;
                    vertex->m_position.vec(vertexInfo.x,vertexInfo.y,vertexInfo.z);
                    vertex->m_normal.vec(vertexInfo.nx,vertexInfo.ny,vertexInfo.nz);
                    for(unsigned int h=0;h<vertexInfo.adjacentCount;++h)
                    {
                        unsigned int adjacent;
                        fread(&adjacent,sizeof(unsigned int),1,fp);
                        vertex->m_adjacentEdgeList.push_back(adjacent);
                    }
                    object->directPushVertex(vertex);
                }
                else
                {
                    object->directPushVertex(NULL);
                }
            }

            for(unsigned int e=1;e<objectInfo.edgeCount;++e)
            {
                Edge::EdgeInfo edgeInfo;
                fread(&edgeInfo,sizeof(edgeInfo),1,fp);
                if(edgeInfo.index>0)
                {
                    Edge *edge=new Edge(edgeInfo.start,edgeInfo.end);
                    edge->m_index=edgeInfo.index;
                    edge->m_left=edgeInfo.left;
                    edge->m_right=edgeInfo.right;
                    object->directPushEdge(edge);
                }
                else
                {
                    object->directPushEdge(NULL);
                }
            }

            for(unsigned int e=1;e<objectInfo.faceCount;++e)
            {
                Face::FaceInfo faceInfo;
                fread(&faceInfo,sizeof(faceInfo),1,fp);
                if(faceInfo.index>0)
                {
                    Face *face=new Face();
                    face->m_index=faceInfo.index;
                    face->m_normal.vec(faceInfo.nx,faceInfo.ny,faceInfo.nz);
                    for(unsigned int h=0;h<faceInfo.edgeCount;++h)
                    {
                        int edge;
                        fread(&edge,sizeof(int),1,fp);
                        face->m_edge.push_back(edge);
                    }
                    object->directPushFace(face);
                }
                else
                {
                    object->directPushFace(NULL);
                }
            }
            sceneObjectAdd(object);
        }
        else
        {
            m_objectList.pushNullS();
        }
    }
    fclose(fp);
}

void Scene::openFromPWB(const char *fileName)
{
    clearScene();
    loadFromPWB(fileName);
}
