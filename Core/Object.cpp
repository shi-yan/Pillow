#include "Object.h"
#include <iostream>

Object::Object(char *theName)
    :ElementBase(),
      m_center(),
      m_position(),
      m_rotation(),
      m_scale(1,1,1,1),
      m_name(theName),
      m_subdivideId(0),
      m_subdivideLevelSize(0),
      m_isHide(false),
      m_isMirror(false),
      m_mirrorMode(MirrorMode::Nothing),
      m_mirrorPosition(0)
{
    m_matAmbient[0] =  0.7f;
    m_matAmbient[1] =  0.7f;
    m_matAmbient[2] =  0.7f;
    m_matAmbient[3] =  1.0f;

    m_matDiffuse[0] =  0.9f;
    m_matDiffuse[1] =  0.9f;
    m_matDiffuse[2] =  0.9f;
    m_matDiffuse[3] =  1.0f;

    m_matSpecular[0] =  0.11f;
    m_matSpecular[1] =  0.11f;
    m_matSpecular[2] =  0.11f;
    m_matSpecular[3] =  1.0f;

    m_matEmission[0] = 0.0f;
    m_matEmission[1] = 0.0f;
    m_matEmission[2] = 0.0f;
    m_matEmission[3] = 1.0f;

    m_matShininess[0] = 10.0f;
    m_matShininess[1] = 10.0f;
    m_matShininess[2] = 10.0f;
    m_matShininess[3] = 10.0f;

    //m_vertexArray.clear();
    m_vertexArray.reserve(5000);
    //m_edgeArray.clear();
    m_edgeArray.reserve(5000);
    //m_faceArray.clear();
    m_faceArray.reserve(5000);
    m_subdivideLevel[0]=NULL;
    m_subdivideLevel[1]=NULL;
    m_subdivideLevel[2]=NULL;
    m_subdivideLevel[3]=NULL;
    m_subdivideLevel[4]=NULL;
    m_renderMode=RenderType::WireframeFaced;
}


unsigned int Object::addVertex(float p1,float p2,float p3)
{
    unsigned int vi=m_vertexArray.add(new Vertex(p1,p2,p3));
    historyManager->record(new Log_ObjectVertexAdd(m_index,vi));
    return vi;
}

unsigned int Object::addVertex(float p1,float p2,float p3,float n1,float n2,float n3)
{
    unsigned int vi=m_vertexArray.add(new Vertex(p1,p2,p3,n1,n2,n3));
    historyManager->record(new Log_ObjectVertexAdd(m_index,vi));
    return vi;
}

unsigned int Object::addEdge(unsigned int ei,Edge *theE)
    {
        m_edgeArray.addI(ei,theE);
        historyManager->record(new Log_ObjectEdgeAdd(m_index,ei));
        return ei;
    }

unsigned int Object::addFace(unsigned int ei,Face *theF)
    {
        m_faceArray.addI(ei,theF);
        historyManager->record(new Log_ObjectFaceAdd(m_index,ei));
        return ei;
    }

void Object::drawMirror()
    {
        if(!m_isHide && m_isMirror)
        {
            /*glMatrixMode(GL_MODELVIEW);
            glPushMatrix();
            //glTranslatef(100,100,100);
            glTranslatef(m_mirrorPosition.x,m_mirrorPosition.y,m_mirrorPosition.z);
            glTranslatef(m_position.x,m_position.y,m_position.z);
            glTranslatef(m_center.x,m_center.y,m_center.z);
            glRotatef(m_rotation.w,m_rotation.x,m_rotation.y,m_rotation.z);
            glScalef(m_scale.x,m_scale.y,m_scale.z);
            glTranslatef(-m_center.x,-m_center.y,-m_center.z);
            if(m_mirrorMode==MirrorMode::MirrorXY)
            {


                glScalef(1,1,-1);
            }
            else if(m_mirrorMode==MirrorMode::MirrorYZ)
            {


                glScalef(-1,1,1);
            }
            else if(m_mirrorMode==MirrorMode::MirrorXZ)
            {

                glScalef(1,-1,1);

            }

            if(m_renderMode==RenderType::Faced)
            {
                drawFaced();
            }
            else if(m_renderMode==RenderType::Smooth)
            {
                drawSmooth();
            }
            else if(m_renderMode==RenderType::Wire)
            {
                drawWire();
            }
            else if(m_renderMode==RenderType::Wireframe)
            {
                drawWireframe();
            }
            else if(m_renderMode==RenderType::WireframeFaced)
            {
                drawWireframeFaced();
            }

            glPopMatrix();*/
        }
    }

unsigned int Object::addVertex(Vector &pos)
{
    unsigned int vi=m_vertexArray.add(new Vertex(pos));
    historyManager->record(new Log_ObjectVertexAdd(m_index,vi));
    return vi;
}

unsigned int Object::addVertex(unsigned int ei,Vertex *theV)
{
    m_vertexArray.addI(ei,theV);
    historyManager->record(new Log_ObjectVertexAdd(m_index,ei));
    return ei;
}

unsigned int Object::addVertex(Vector &pos,Vector &nor)
{
    unsigned int vi=m_vertexArray.add(new Vertex(pos,nor));
    historyManager->record(new Log_ObjectVertexAdd(m_index,vi));
    return vi;
}

void Object::testOut(char *fileName)
{
    FILE *fp=fopen(fileName,"w");
    for(unsigned int i=1;i<m_vertexArray.size();i++)
    {
        if(m_vertexArray[i]==NULL)
        {
            fprintf(fp,"v %f %f %f\n",0,0,0);
        }
        else
        {
            fprintf(fp,"v %f %f %f\n",m_vertexArray[i]->m_position.x,m_vertexArray[i]->m_position.y,m_vertexArray[i]->m_position.z);
        }
    }
    fprintf(fp,"g box01\n");
    for(unsigned int i=1;i<m_faceArray.size();i++)
    {
        if(m_faceArray[i]==NULL) continue;
        fprintf(fp,"f ");
        for(unsigned int e=0;e<m_faceArray[i]->m_edge.size();e++)
        {
            if(m_faceArray[i]->m_edge[e]<0)
            {
                fprintf(fp,"%d ",m_edgeArray[-m_faceArray[i]->m_edge[e]]->m_end);
            }
            else
            {
                fprintf(fp,"%d ",m_edgeArray[m_faceArray[i]->m_edge[e]]->m_start);
            }
        }
        fprintf(fp,"\n");
    }
    fprintf(fp,"g\n");
    fclose(fp);
}

unsigned int Object::addEdge(int start,int end)
{
    //printf("cao0");
    unsigned int ei=m_edgeArray.add(new Edge(start,end));
    //printf("cao0.5");
//    printf("!%d,%d!",index,ei);
    Log_ObjectEdgeAdd *theLog=new Log_ObjectEdgeAdd(m_index,ei);
    //printf("cao0.6");
    historyManager->record(theLog);
    //printf("cao1");
    m_vertexArray[start]->m_adjacentEdgeList.push_back((int)ei);
    historyManager->record(new Log_VertexAdjacentPush(m_index,start));
    //printf("cao2");
    m_vertexArray[end]->m_adjacentEdgeList.push_back((int)ei);
    historyManager->record(new Log_VertexAdjacentPush(m_index,end));
    //printf("cao3");
    return ei;
}

Vertex * Object::vertex(unsigned int index)
{
    return m_vertexArray[index];
}

Edge * Object::edge(unsigned int index)
{
    return m_edgeArray[index];
}

Face * Object::face(unsigned int index)
{
    return m_faceArray[index];
}

unsigned int Object::addFace(unsigned int them_edgeArray[],unsigned int size)
{
    //printf("start\n");
    Face *theFace=new Face();
    unsigned int resultIndex=m_faceArray.add(theFace);
    historyManager->record(new Log_ObjectFaceAdd(m_index,resultIndex));
    for(unsigned int i=0;i<size;++i)
    {
        unsigned int i1=them_edgeArray[i];
        unsigned int i2=them_edgeArray[(i+1)%size];
        if(m_edgeArray[i1]->m_end==m_edgeArray[i2]->m_start || m_edgeArray[i1]->m_end==m_edgeArray[i2]->m_end)
        {
            m_edgeArray[i1]->m_right=theFace->m_index;
            historyManager->record(new Log_EdgeRightChange(m_index,i1,theFace->m_index));
            theFace->m_edge.push_back(i1);
            historyManager->record(new Log_FaceEdgePush(m_index,theFace->m_index));
        }
        else
        if(m_edgeArray[i1]->m_start==m_edgeArray[i2]->m_start || m_edgeArray[i1]->m_start==m_edgeArray[i2]->m_end)
        {
            m_edgeArray[i1]->m_left=theFace->m_index;
            historyManager->record(new Log_EdgeLeftChange(m_index,i1,theFace->m_index));
            theFace->m_edge.push_back(-((int)i1));
            historyManager->record(new Log_FaceEdgePush(m_index,theFace->m_index));
        }
    }
    //printf("end\n");
    return resultIndex;
}

void Object::SubTestOut(char *fileName,int level)
{
    FILE *fp=NULL;
    fp=fopen(fileName,"w");
    SubdivideLevel *theSub=m_subdivideLevel[level];
    unsigned int vertexCount=theSub->m_vertex.size();
    for(unsigned int i=1;i<vertexCount;++i)
    {
        fprintf(fp,"v %f %f %f\n",theSub->m_vertex[i]->m_position.x,theSub->m_vertex[i]->m_position.y,theSub->m_vertex[i]->m_position.z);
    }
    fprintf(fp,"g box01\n");
    for(unsigned int i=1;i<theSub->m_face.size();++i)
    {
        fprintf(fp,"f ");
        for(unsigned int e=0;e<4;e++)
        {
            if(theSub->m_face[i]->m_edge[e]>0)
            {
                fprintf(fp,"%d ",theSub->m_edge[theSub->m_face[i]->m_edge[e]]->m_start);
            }
            else
            {
                fprintf(fp,"%d ",theSub->m_edge[-theSub->m_face[i]->m_edge[e]]->m_end);
            }
        }
        fprintf(fp,"\n");
    }
    fprintf(fp,"g\n");
    fclose(fp);
}

void Object::SubTestOut(char *fileName)
{
    SubTestOut(fileName,0);
}

void Object::subdivide()
{
    //µÝÔöÏ¸·ÖµÄ±àºÅ
    ++m_subdivideId;
    if(!m_subdivideLevelSize)
    {
        m_subdivideLevel[0]=new SubdivideLevel(m_vertexArray.size()+m_edgeArray.size()+m_faceArray.size(),m_edgeArray.size()*4,m_edgeArray.size()*2);
        ++m_subdivideLevelSize;
        unsigned int faceCount=  m_faceArray.size();
        for(unsigned int i=1;i<faceCount;++i)
        {
            if(m_faceArray[i]==NULL)
                continue;
            subdivideFace(m_faceArray[i]);
        }
    }
    else
    if(m_subdivideLevelSize>0 && m_subdivideLevelSize<5)
    {
        for(int e=m_subdivideLevelSize;e>0;--e)
        {
            m_subdivideLevel[e]=m_subdivideLevel[e-1];
        }
        m_subdivideLevel[0]=new SubdivideLevel(m_subdivideLevel[0]->m_vertex.size()+m_subdivideLevel[0]->m_edge.size()+m_subdivideLevel[0]->m_face.size(),m_subdivideLevel[0]->m_edge.size()*4,m_subdivideLevel[0]->m_edge.size()*2);
        ++m_subdivideLevelSize;
        unsigned int faceCount=m_subdivideLevel[1]->m_face.size();
        for(unsigned int i=1;i<faceCount;i++)
        {
            if(m_subdivideLevel[1]->m_face[i]==NULL)
                continue;
            subdivideFace(m_subdivideLevel[1]->m_face[i]);
        }
    }
    updateAllSubNormal();
}

Vector Object::EAdjacentVertex(Vertex *theVertex)
{
    theVertex->m_edgeVertex=false;
    Vector result(0);
    unsigned int vertexCount=theVertex->m_adjacentEdgeList.size();
    for(unsigned int i=0;i<vertexCount;++i)
    {
        if(m_edgeArray[theVertex->m_adjacentEdgeList[i]]->m_left && m_edgeArray[theVertex->m_adjacentEdgeList[i]]->m_right && !theVertex->m_edgeVertex)
        {
            if(theVertex->m_index==m_edgeArray[theVertex->m_adjacentEdgeList[i]]->m_end)
            {
                result+=m_vertexArray[m_edgeArray[theVertex->m_adjacentEdgeList[i]]->m_start]->m_position;
            }
            else
            {
                result+=m_vertexArray[m_edgeArray[theVertex->m_adjacentEdgeList[i]]->m_end]->m_position;
            }
        }
        else
        {
            if(!theVertex->m_edgeVertex)
            {
                theVertex->m_edgeVertex=true;
                result.null();
            }
            if(!m_edgeArray[theVertex->m_adjacentEdgeList[i]]->m_left || !m_edgeArray[theVertex->m_adjacentEdgeList[i]]->m_right)
            {
                if(theVertex->m_index==m_edgeArray[theVertex->m_adjacentEdgeList[i]]->m_end)
                {
                    result+=m_vertexArray[m_edgeArray[theVertex->m_adjacentEdgeList[i]]->m_start]->m_position;
                }
                else
                {
                    result+=m_vertexArray[m_edgeArray[theVertex->m_adjacentEdgeList[i]]->m_end]->m_position;
                }
            }
        }
    }
    return result;
}

Vector Object::EAdjacentVertex(SubdivideVertex *theVertex,int level)
{
    theVertex->m_edgeVertex=false;
    Vector result(0);
    unsigned int vertexCount=theVertex->m_adjacentEdgeList.size();
    for(unsigned int i=0;i<vertexCount;++i)
    {
        if(m_subdivideLevel[level]->m_edge[theVertex->m_adjacentEdgeList[i]]->m_left!=0 && m_subdivideLevel[level]->m_edge[theVertex->m_adjacentEdgeList[i]]->m_right!=0 && !theVertex->m_edgeVertex)
        {
            if(theVertex->m_index==m_subdivideLevel[level]->m_edge[theVertex->m_adjacentEdgeList[i]]->m_end)
            {
                result+=m_subdivideLevel[level]->m_vertex[m_subdivideLevel[level]->m_edge[theVertex->m_adjacentEdgeList[i]]->m_start]->m_position;
            }
            else
            {
                result+=m_subdivideLevel[level]->m_vertex[m_subdivideLevel[level]->m_edge[theVertex->m_adjacentEdgeList[i]]->m_end]->m_position;
            }
        }
        else
        {
            if(!theVertex->m_edgeVertex)
            {
                theVertex->m_edgeVertex=true;
                result.null();
            }
            if(m_subdivideLevel[level]->m_edge[theVertex->m_adjacentEdgeList[i]]->m_left==0 || m_subdivideLevel[level]->m_edge[theVertex->m_adjacentEdgeList[i]]->m_right==0)
            {
                if(theVertex->m_index==m_subdivideLevel[level]->m_edge[theVertex->m_adjacentEdgeList[i]]->m_end)
                {
                    result+=m_subdivideLevel[level]->m_vertex[m_subdivideLevel[level]->m_edge[theVertex->m_adjacentEdgeList[i]]->m_start]->m_position;
                }
                else
                {
                    result+=m_subdivideLevel[level]->m_vertex[m_subdivideLevel[level]->m_edge[theVertex->m_adjacentEdgeList[i]]->m_end]->m_position;
                }
            }
        }
    }
    return result;
}

Vector Object::EAdjacentVertex(SubdivideVertex *theVertex)
{
    theVertex->m_edgeVertex=false;
    Vector result(0);
    unsigned int vertexCount=theVertex->m_adjacentEdgeList.size();
    for(unsigned int i=0;i<vertexCount;++i)
    {
        if(m_subdivideLevel[1]->m_edge[theVertex->m_adjacentEdgeList[i]]->m_left!=0 && m_subdivideLevel[1]->m_edge[theVertex->m_adjacentEdgeList[i]]->m_right!=0 && !theVertex->m_edgeVertex)
        {
            if(theVertex->m_index==m_subdivideLevel[1]->m_edge[theVertex->m_adjacentEdgeList[i]]->m_end)
            {
                result+=m_subdivideLevel[1]->m_vertex[m_subdivideLevel[1]->m_edge[theVertex->m_adjacentEdgeList[i]]->m_start]->m_position;
            }
            else
            {
                result+=m_subdivideLevel[1]->m_vertex[m_subdivideLevel[1]->m_edge[theVertex->m_adjacentEdgeList[i]]->m_end]->m_position;
            }
        }
        else
        {
            if(!theVertex->m_edgeVertex)
            {
                theVertex->m_edgeVertex=true;
                result.null();
            }
            if(m_subdivideLevel[1]->m_edge[theVertex->m_adjacentEdgeList[i]]->m_left==0 || m_subdivideLevel[1]->m_edge[theVertex->m_adjacentEdgeList[i]]->m_right==0)
            {
                if(theVertex->m_index==m_subdivideLevel[1]->m_edge[theVertex->m_adjacentEdgeList[i]]->m_end)
                {
                    result+=m_subdivideLevel[1]->m_vertex[m_subdivideLevel[1]->m_edge[theVertex->m_adjacentEdgeList[i]]->m_start]->m_position;
                }
                else
                {
                    result+=m_subdivideLevel[1]->m_vertex[m_subdivideLevel[1]->m_edge[theVertex->m_adjacentEdgeList[i]]->m_end]->m_position;
                }
            }
        }
    }
    return result;
}

void Object::subdivideFace(Face *theFace)
{
    //Ê×ÏÈÒªµÃµ½Õâ¸öÃæµÄ¶Ëµã
    //µÃµ½¶ËµãºÍ±ßµÄÊýÄ¿
    unsigned int edgeCount=theFace->m_edge.size();
    //ÐÂ½¨´æ·Å¶¥µãµÄÊý×é
    Vertex **theVertexList=new Vertex*[edgeCount];
    theFace->m_center=m_subdivideLevel[0]->m_vertex.add(new SubdivideVertex());
    //m_subdivideLevel[0]->m_vertex[theFace->m_center]->m_adjacentEdgeList.clear();
    m_subdivideLevel[0]->m_vertex[theFace->m_center]->m_adjacentEdgeList.reserve(4);
    for(unsigned int e=0;e<edgeCount;++e)
    {
        if(theFace->m_edge[e]<0)
        {
            theVertexList[e]=m_vertexArray[m_edgeArray[-theFace->m_edge[e]]->m_start];
        }
        else
        {
            theVertexList[e]=m_vertexArray[m_edgeArray[theFace->m_edge[e]]->m_end];
        }
        m_subdivideLevel[0]->m_vertex[theFace->m_center]->m_position+=theVertexList[e]->m_position;
        //m_subdivideLevel[0]->m_vertex[theFace->m_center]->m_normal+=theVertexList[e]->m_normal;
    }
    m_subdivideLevel[0]->m_vertex[theFace->m_center]->m_position/=(float)edgeCount;
    //m_subdivideLevel[0]->m_vertex[theFace->m_center]->m_normal/=edgeCount;
    //¼ÆËãÐÂµÄ¶¥µã
    for(unsigned int i=0;i<edgeCount;++i)
    {
        Vertex *theV=theVertexList[i];
        if(theV->m_subdivideId!=m_subdivideId)
        {
            //Èç¹ûÊÇµÚÒ»´Î¼ÆËãµ½Õâ¸öµã
            //³õÊ¼»¯Ï¸·Ö²½Êý
            theV->m_subdivideId=m_subdivideId;
            theV->m_subdivideStep=0;
            //Éú³ÉÕâ¸öµãµÄÏ¸·Öµã
            theV->m_nextLevel=m_subdivideLevel[0]->m_vertex.add(new SubdivideVertex());
            //m_subdivideLevel[0]->m_vertex[theV->m_nextLevel]->m_adjacentEdgeList.clear();
            m_subdivideLevel[0]->m_vertex[theV->m_nextLevel]->m_adjacentEdgeList.reserve(theV->m_adjacentEdgeList.size());
            //Ê×ÏÈ¼ÆËãÏàÁÚµãµÄ×ø±êºÍEv
            m_subdivideLevel[0]->m_vertex[theV->m_nextLevel]->m_position+=EAdjacentVertex(theV);
            if(theV->m_edgeVertex)
            {
                m_subdivideLevel[0]->m_vertex[theV->m_nextLevel]->m_position+=theV->m_position*6;
                m_subdivideLevel[0]->m_vertex[theV->m_nextLevel]->m_position/=8;
            }
            else
            {
                //¼ÓÉÏÕâ¸öÃæµÄÖÐµã
                m_subdivideLevel[0]->m_vertex[theV->m_nextLevel]->m_position+=m_subdivideLevel[0]->m_vertex[theFace->m_center]->m_position;
            }
            ++(theV->m_subdivideStep);
        }
        else
        {
            if(!theV->m_edgeVertex)
            {
                m_subdivideLevel[0]->m_vertex[theV->m_nextLevel]->m_position+=m_subdivideLevel[0]->m_vertex[theFace->m_center]->m_position;
                ++(theV->m_subdivideStep);
                unsigned int n=theV->m_adjacentEdgeList.size();
                if(n==(unsigned int)theV->m_subdivideStep)
                {
                    m_subdivideLevel[0]->m_vertex[theV->m_nextLevel]->m_position+=theV->m_position*(float)(n*n-2*n);
                    m_subdivideLevel[0]->m_vertex[theV->m_nextLevel]->m_position/=(float)(n*n);
                }
            }
        }
    }
    delete theVertexList;
    //Ö®ºó´¦ÀíÃ¿Ò»¸ö±ß
    for(unsigned int i=0;i<edgeCount;++i)
    {
        Edge *theEdge;
        if(theFace->m_edge[i]>0)
        {
            theEdge=m_edgeArray[theFace->m_edge[i]];
            if(theEdge->m_subdivideId!=m_subdivideId)
            {
                //Èç¹ûÊÇµÚÒ»´Î´¦ÀíÕâ¸ö±ß
                theEdge->m_subdivideId=m_subdivideId;
                Vector theN=m_vertexArray[theEdge->m_start]->m_position+m_vertexArray[theEdge->m_end]->m_position;
                theEdge->m_middle=m_subdivideLevel[0]->m_vertex.add(new SubdivideVertex(theN));
                //m_subdivideLevel[0]->m_vertex[theEdge->m_middle]->m_adjacentEdgeList.clear();
                m_subdivideLevel[0]->m_vertex[theEdge->m_middle]->m_adjacentEdgeList.reserve(4);
                if(theEdge->m_left && theEdge->m_right)
                {
                    m_subdivideLevel[0]->m_vertex[theEdge->m_middle]->m_position+=m_subdivideLevel[0]->m_vertex[theFace->m_center]->m_position;
                }
                else
                {
                    m_subdivideLevel[0]->m_vertex[theEdge->m_middle]->m_position/=2;
                }
                theEdge->m_startEdge=m_subdivideLevel[0]->m_edge.add(new SubdivideEdge(m_vertexArray[theEdge->m_start]->m_nextLevel,theEdge->m_middle));
                m_subdivideLevel[0]->m_vertex[m_vertexArray[theEdge->m_start]->m_nextLevel]->m_adjacentEdgeList.push_back(theEdge->m_startEdge);
                m_subdivideLevel[0]->m_vertex[theEdge->m_middle]->m_adjacentEdgeList.push_back(theEdge->m_startEdge);
                theEdge->m_endEdge=m_subdivideLevel[0]->m_edge.add(new SubdivideEdge(theEdge->m_middle,m_vertexArray[theEdge->m_end]->m_nextLevel));
                m_subdivideLevel[0]->m_vertex[m_vertexArray[theEdge->m_end]->m_nextLevel]->m_adjacentEdgeList.push_back(theEdge->m_endEdge);
                m_subdivideLevel[0]->m_vertex[theEdge->m_middle]->m_adjacentEdgeList.push_back(theEdge->m_endEdge);
                theEdge->m_rightEdge=m_subdivideLevel[0]->m_edge.add(new SubdivideEdge(theEdge->m_middle,theFace->m_center));
                m_subdivideLevel[0]->m_vertex[theEdge->m_middle]->m_adjacentEdgeList.push_back(theEdge->m_rightEdge);
                m_subdivideLevel[0]->m_vertex[theFace->m_center]->m_adjacentEdgeList.push_back(theEdge->m_rightEdge);
            }
            else
            {
                //Èç¹û²»ÊÇµÚÒ»´Î¼ÆËã
                m_subdivideLevel[0]->m_vertex[theEdge->m_middle]->m_position+=m_subdivideLevel[0]->m_vertex[theFace->m_center]->m_position;
                m_subdivideLevel[0]->m_vertex[theEdge->m_middle]->m_position/=4;
                theEdge->m_rightEdge=m_subdivideLevel[0]->m_edge.add(new SubdivideEdge(theEdge->m_middle,theFace->m_center));
                m_subdivideLevel[0]->m_vertex[theEdge->m_middle]->m_adjacentEdgeList.push_back(theEdge->m_rightEdge);
                m_subdivideLevel[0]->m_vertex[theFace->m_center]->m_adjacentEdgeList.push_back(theEdge->m_rightEdge);
            }
        }
        else
        {
            theEdge=m_edgeArray[-theFace->m_edge[i]];
            if(theEdge->m_subdivideId!=m_subdivideId)
            {
                //Èç¹ûÊÇµÚÒ»´Î´¦ÀíÕâ¸ö±ß
                theEdge->m_subdivideId=m_subdivideId;
                Vector theN=m_vertexArray[theEdge->m_start]->m_position+m_vertexArray[theEdge->m_end]->m_position;
                theEdge->m_middle=m_subdivideLevel[0]->m_vertex.add(new SubdivideVertex(theN));
                //m_subdivideLevel[0]->m_vertex[theEdge->m_middle]->m_adjacentEdgeList.clear();
                m_subdivideLevel[0]->m_vertex[theEdge->m_middle]->m_adjacentEdgeList.reserve(4);
                if(theEdge->m_left!=0 && theEdge->m_right!=0)
                {
                    m_subdivideLevel[0]->m_vertex[theEdge->m_middle]->m_position+=m_subdivideLevel[0]->m_vertex[theFace->m_center]->m_position;
                }
                else
                {
                    m_subdivideLevel[0]->m_vertex[theEdge->m_middle]->m_position/=2;
                }
                theEdge->m_startEdge=m_subdivideLevel[0]->m_edge.add(new SubdivideEdge(m_vertexArray[theEdge->m_start]->m_nextLevel,theEdge->m_middle));
                m_subdivideLevel[0]->m_vertex[m_vertexArray[theEdge->m_start]->m_nextLevel]->m_adjacentEdgeList.push_back(theEdge->m_startEdge);
                m_subdivideLevel[0]->m_vertex[theEdge->m_middle]->m_adjacentEdgeList.push_back(theEdge->m_startEdge);
                theEdge->m_endEdge=m_subdivideLevel[0]->m_edge.add(new SubdivideEdge(theEdge->m_middle,m_vertexArray[theEdge->m_end]->m_nextLevel));
                m_subdivideLevel[0]->m_vertex[m_vertexArray[theEdge->m_end]->m_nextLevel]->m_adjacentEdgeList.push_back(theEdge->m_endEdge);
                m_subdivideLevel[0]->m_vertex[theEdge->m_middle]->m_adjacentEdgeList.push_back(theEdge->m_endEdge);
                theEdge->m_leftEdge=m_subdivideLevel[0]->m_edge.add(new SubdivideEdge(theEdge->m_middle,theFace->m_center));
                m_subdivideLevel[0]->m_vertex[theEdge->m_middle]->m_adjacentEdgeList.push_back(theEdge->m_leftEdge);
                m_subdivideLevel[0]->m_vertex[theFace->m_center]->m_adjacentEdgeList.push_back(theEdge->m_leftEdge);
            }
            else
            {
                //Èç¹û²»ÊÇµÚÒ»´Î¼ÆËã
                m_subdivideLevel[0]->m_vertex[theEdge->m_middle]->m_position+=m_subdivideLevel[0]->m_vertex[theFace->m_center]->m_position;
                m_subdivideLevel[0]->m_vertex[theEdge->m_middle]->m_position/=4;
                theEdge->m_leftEdge=m_subdivideLevel[0]->m_edge.add(new SubdivideEdge(theEdge->m_middle,theFace->m_center));
                m_subdivideLevel[0]->m_vertex[theEdge->m_middle]->m_adjacentEdgeList.push_back(theEdge->m_leftEdge);
                m_subdivideLevel[0]->m_vertex[theFace->m_center]->m_adjacentEdgeList.push_back(theEdge->m_leftEdge);
            }
        }
    }
    //½¨Á¢Ï¸·ÖÖ®ºóµÄÃæ
    //theFace->m_subdivideFace.clear();
    theFace->m_subdivideFace.reserve(edgeCount);
    for(unsigned int i=0;i<edgeCount;++i)
    {
        unsigned int i2=(i+1)%edgeCount;
        if(theFace->m_edge[i]>0)
        {
            Edge *theEdge1=m_edgeArray[theFace->m_edge[i]];
            if(theFace->m_edge[i2]>0)
            {
                Edge *theEdge2=m_edgeArray[theFace->m_edge[i2]];
                unsigned int faceId=
                m_subdivideLevel[0]->m_edge[theEdge1->m_rightEdge]->m_left=
                m_subdivideLevel[0]->m_edge[theEdge1->m_endEdge]->m_right=
                m_subdivideLevel[0]->m_edge[theEdge2->m_startEdge]->m_right=
                m_subdivideLevel[0]->m_edge[theEdge2->m_rightEdge]->m_right=m_subdivideLevel[0]->m_face.add(new SubdivideFace(-theEdge1->m_rightEdge,theEdge1->m_endEdge,theEdge2->m_startEdge,theEdge2->m_rightEdge));
                theFace->m_subdivideFace.push_back(faceId);
            }
            else
            {
                Edge *theEdge2=m_edgeArray[-theFace->m_edge[i2]];
                unsigned int faceId=
                m_subdivideLevel[0]->m_edge[theEdge1->m_rightEdge]->m_left=
                m_subdivideLevel[0]->m_edge[theEdge1->m_endEdge]->m_right=
                m_subdivideLevel[0]->m_edge[theEdge2->m_endEdge]->m_left=
                m_subdivideLevel[0]->m_edge[theEdge2->m_leftEdge]->m_right=m_subdivideLevel[0]->m_face.add(new SubdivideFace(-theEdge1->m_rightEdge,theEdge1->m_endEdge,-theEdge2->m_endEdge,theEdge2->m_leftEdge));
                theFace->m_subdivideFace.push_back(faceId);
            }
        }
        else
        {
            Edge *theEdge1=m_edgeArray[-theFace->m_edge[i]];
            if(theFace->m_edge[i2]>0)
            {
                Edge *theEdge2=m_edgeArray[theFace->m_edge[i2]];
                unsigned int faceId=
                m_subdivideLevel[0]->m_edge[theEdge1->m_leftEdge]->m_left=
                m_subdivideLevel[0]->m_edge[theEdge1->m_startEdge]->m_left=
                m_subdivideLevel[0]->m_edge[theEdge2->m_startEdge]->m_right=
                m_subdivideLevel[0]->m_edge[theEdge2->m_rightEdge]->m_right=m_subdivideLevel[0]->m_face.add(new SubdivideFace(-theEdge1->m_leftEdge,-theEdge1->m_startEdge,theEdge2->m_startEdge,theEdge2->m_rightEdge));
                theFace->m_subdivideFace.push_back(faceId);
            }
            else
            {
                Edge *theEdge2=m_edgeArray[-theFace->m_edge[i2]];
                unsigned int faceId=
                m_subdivideLevel[0]->m_edge[theEdge1->m_leftEdge]->m_left=
                m_subdivideLevel[0]->m_edge[theEdge1->m_startEdge]->m_left=
                m_subdivideLevel[0]->m_edge[theEdge2->m_endEdge]->m_left=
                m_subdivideLevel[0]->m_edge[theEdge2->m_leftEdge]->m_right=m_subdivideLevel[0]->m_face.add(new SubdivideFace(-theEdge1->m_leftEdge,-theEdge1->m_startEdge,-theEdge2->m_endEdge,theEdge2->m_leftEdge));
                theFace->m_subdivideFace.push_back(faceId);
            }
        }
    }
}

void Object::subdivideFace(SubdivideFace *theFace)
{
    //Ê×ÏÈÒªµÃµ½Õâ¸öÃæµÄ¶Ëµã
    //µÃµ½¶ËµãºÍ±ßµÄÊýÄ¿
    unsigned int edgeCount=4;
    SubdivideLevel *target=m_subdivideLevel[0];
    SubdivideLevel *original=m_subdivideLevel[1];
    //ÐÂ½¨´æ·Å¶¥µãµÄÊý×é
    SubdivideVertex **theVertexList=new SubdivideVertex*[edgeCount];
    theFace->m_center=target->m_vertex.add(new SubdivideVertex());
    //target->m_vertex[theFace->m_center]->m_adjacentEdgeList.clear();
    target->m_vertex[theFace->m_center]->m_adjacentEdgeList.reserve(4);
    for(unsigned int e=0;e<edgeCount;++e)
    {
        if(theFace->m_edge[e]<0)
        {
            theVertexList[e]=original->m_vertex[original->m_edge[-theFace->m_edge[e]]->m_start];
        }
        else
        {
            theVertexList[e]=original->m_vertex[original->m_edge[theFace->m_edge[e]]->m_end];
        }
        target->m_vertex[theFace->m_center]->m_position+=theVertexList[e]->m_position;
        //target->m_vertex[theFace->m_center]->m_normal+=theVertexList[e]->m_normal;
    }
    //¼ÆËãÕâ¸öÃæµÄÖÐµã
    target->m_vertex[theFace->m_center]->m_position/=(float)edgeCount;
    //target->m_vertex[theFace->m_center]->m_normal/=edgeCount;
    //¼ÆËãÐÂµÄ¶¥µã
    for(unsigned int i=0;i<edgeCount;++i)
    {
        SubdivideVertex *theV=theVertexList[i];
        if(theV->m_subdivideId!=m_subdivideId)
        {
            //Èç¹ûÊÇµÚÒ»´Î¼ÆËãµ½Õâ¸öµã
            //³õÊ¼»¯Ï¸·Ö²½Êý
            theV->m_subdivideId=m_subdivideId;
            theV->m_subdivideStep=0;
            //Éú³ÉÕâ¸öµãµÄÏ¸·Öµã
            theV->m_nextLevel=target->m_vertex.add(new SubdivideVertex());
            //target->m_vertex[theV->m_nextLevel]->m_adjacentEdgeList.clear();
            target->m_vertex[theV->m_nextLevel]->m_adjacentEdgeList.reserve(theV->m_adjacentEdgeList.size());
            //Ê×ÏÈ¼ÆËãÏàÁÚµãµÄ×ø±êºÍEv
            target->m_vertex[theV->m_nextLevel]->m_position+=EAdjacentVertex(theV);
            if(theV->m_edgeVertex)
            {
                target->m_vertex[theV->m_nextLevel]->m_position+=theV->m_position*6;
                target->m_vertex[theV->m_nextLevel]->m_position/=8;
            }
            else
            {
                //¼ÓÉÏÕâ¸öÃæµÄÖÐµã
                target->m_vertex[theV->m_nextLevel]->m_position+=target->m_vertex[theFace->m_center]->m_position;
            }
            ++(theV->m_subdivideStep);
        }
        else
        {
            if(!theV->m_edgeVertex)
            {
                target->m_vertex[theV->m_nextLevel]->m_position+=target->m_vertex[theFace->m_center]->m_position;
                theV->m_subdivideStep++;
                int n=theV->m_adjacentEdgeList.size();
                if(n==theV->m_subdivideStep)
                {
                    target->m_vertex[theV->m_nextLevel]->m_position+=theV->m_position*(float)(n*n-2*n);
                    target->m_vertex[theV->m_nextLevel]->m_position/=(float)(n*n);
                }
            }
        }
    }
    delete theVertexList;
    //Ö®ºó´¦ÀíÃ¿Ò»¸ö±ß
    for(unsigned int i=0;i<edgeCount;++i)
    {
        SubdivideEdge *theEdge;
        if(theFace->m_edge[i]>0)
        {
            theEdge=original->m_edge[theFace->m_edge[i]];
            if(theEdge->m_subdivideId!=m_subdivideId)
            {
                //Èç¹ûÊÇµÚÒ»´Î´¦ÀíÕâ¸ö±ß
                theEdge->m_subdivideId=m_subdivideId;
                Vector theN=original->m_vertex[theEdge->m_start]->m_position+original->m_vertex[theEdge->m_end]->m_position;
                theEdge->m_middle=target->m_vertex.add(new SubdivideVertex(theN));
                //target->m_vertex[theEdge->m_middle]->m_adjacentEdgeList.clear();
                target->m_vertex[theEdge->m_middle]->m_adjacentEdgeList.reserve(4);
                if(theEdge->m_left && theEdge->m_right)
                {
                    target->m_vertex[theEdge->m_middle]->m_position+=target->m_vertex[theFace->m_center]->m_position;
                }
                else
                {
                    target->m_vertex[theEdge->m_middle]->m_position/=2;
                }
                theEdge->m_startEdge=target->m_edge.add(new SubdivideEdge(original->m_vertex[theEdge->m_start]->m_nextLevel,theEdge->m_middle));
                target->m_vertex[original->m_vertex[theEdge->m_start]->m_nextLevel]->m_adjacentEdgeList.push_back(theEdge->m_startEdge);
                target->m_vertex[theEdge->m_middle]->m_adjacentEdgeList.push_back(theEdge->m_startEdge);
                theEdge->m_endEdge=target->m_edge.add(new SubdivideEdge(theEdge->m_middle,original->m_vertex[theEdge->m_end]->m_nextLevel));
                target->m_vertex[original->m_vertex[theEdge->m_end]->m_nextLevel]->m_adjacentEdgeList.push_back(theEdge->m_endEdge);
                target->m_vertex[theEdge->m_middle]->m_adjacentEdgeList.push_back(theEdge->m_endEdge);
                theEdge->m_rightEdge=target->m_edge.add(new SubdivideEdge(theEdge->m_middle,theFace->m_center));
                target->m_vertex[theEdge->m_middle]->m_adjacentEdgeList.push_back(theEdge->m_rightEdge);
                target->m_vertex[theFace->m_center]->m_adjacentEdgeList.push_back(theEdge->m_rightEdge);
            }
            else
            {
                //Èç¹û²»ÊÇµÚÒ»´Î¼ÆËã
                target->m_vertex[theEdge->m_middle]->m_position+=target->m_vertex[theFace->m_center]->m_position;
                target->m_vertex[theEdge->m_middle]->m_position/=4;
                theEdge->m_rightEdge=target->m_edge.add(new SubdivideEdge(theEdge->m_middle,theFace->m_center));
                target->m_vertex[theEdge->m_middle]->m_adjacentEdgeList.push_back(theEdge->m_rightEdge);
                target->m_vertex[theFace->m_center]->m_adjacentEdgeList.push_back(theEdge->m_rightEdge);
            }
        }
        else
        {
            theEdge=original->m_edge[-theFace->m_edge[i]];
            if(theEdge->m_subdivideId!=m_subdivideId)
            {
                //Èç¹ûÊÇµÚÒ»´Î´¦ÀíÕâ¸ö±ß
                theEdge->m_subdivideId=m_subdivideId;
                Vector theN=original->m_vertex[theEdge->m_start]->m_position+original->m_vertex[theEdge->m_end]->m_position;
                theEdge->m_middle=target->m_vertex.add(new SubdivideVertex(theN));
                //target->m_vertex[theEdge->m_middle]->m_adjacentEdgeList.clear();
                target->m_vertex[theEdge->m_middle]->m_adjacentEdgeList.reserve(4);
                if(theEdge->m_left && theEdge->m_right)
                {
                    target->m_vertex[theEdge->m_middle]->m_position+=target->m_vertex[theFace->m_center]->m_position;
                }
                else
                {
                    target->m_vertex[theEdge->m_middle]->m_position/=2;
                }
                theEdge->m_startEdge=target->m_edge.add(new SubdivideEdge(original->m_vertex[theEdge->m_start]->m_nextLevel,theEdge->m_middle));
                target->m_vertex[original->m_vertex[theEdge->m_start]->m_nextLevel]->m_adjacentEdgeList.push_back(theEdge->m_startEdge);
                target->m_vertex[theEdge->m_middle]->m_adjacentEdgeList.push_back(theEdge->m_startEdge);
                theEdge->m_endEdge=target->m_edge.add(new SubdivideEdge(theEdge->m_middle,original->m_vertex[theEdge->m_end]->m_nextLevel));
                target->m_vertex[original->m_vertex[theEdge->m_end]->m_nextLevel]->m_adjacentEdgeList.push_back(theEdge->m_endEdge);
                target->m_vertex[theEdge->m_middle]->m_adjacentEdgeList.push_back(theEdge->m_endEdge);
                theEdge->m_leftEdge=target->m_edge.add(new SubdivideEdge(theEdge->m_middle,theFace->m_center));
                target->m_vertex[theEdge->m_middle]->m_adjacentEdgeList.push_back(theEdge->m_leftEdge);
                target->m_vertex[theFace->m_center]->m_adjacentEdgeList.push_back(theEdge->m_leftEdge);
            }
            else
            {
                //Èç¹û²»ÊÇµÚÒ»´Î¼ÆËã
                target->m_vertex[theEdge->m_middle]->m_position+=target->m_vertex[theFace->m_center]->m_position;
                target->m_vertex[theEdge->m_middle]->m_position/=4;
                theEdge->m_leftEdge=target->m_edge.add(new SubdivideEdge(theEdge->m_middle,theFace->m_center));
                target->m_vertex[theEdge->m_middle]->m_adjacentEdgeList.push_back(theEdge->m_leftEdge);
                target->m_vertex[theFace->m_center]->m_adjacentEdgeList.push_back(theEdge->m_leftEdge);
            }
        }
    }
    //½¨Á¢Ï¸·ÖÖ®ºóµÄÃæ
    for(unsigned int i=0;i<edgeCount;++i)
    {
        unsigned int i2=(i+1)%edgeCount;
        if(theFace->m_edge[i]>0)
        {
            SubdivideEdge *theEdge1=original->m_edge[theFace->m_edge[i]];
            if(theFace->m_edge[i2]>0)
            {
                SubdivideEdge *theEdge2=original->m_edge[theFace->m_edge[i2]];
                m_subdivideLevel[0]->m_edge[theEdge1->m_rightEdge]->m_left=
                m_subdivideLevel[0]->m_edge[theEdge1->m_endEdge]->m_right=
                m_subdivideLevel[0]->m_edge[theEdge2->m_startEdge]->m_right=
                m_subdivideLevel[0]->m_edge[theEdge2->m_rightEdge]->m_right=theFace->m_subFace[i]=
                target->m_face.add(new SubdivideFace(-theEdge1->m_rightEdge,theEdge1->m_endEdge,theEdge2->m_startEdge,theEdge2->m_rightEdge));
            }
            else
            {
                SubdivideEdge *theEdge2=original->m_edge[-theFace->m_edge[i2]];
                m_subdivideLevel[0]->m_edge[theEdge1->m_rightEdge]->m_left=
                m_subdivideLevel[0]->m_edge[theEdge1->m_endEdge]->m_right=
                m_subdivideLevel[0]->m_edge[theEdge2->m_endEdge]->m_left=
                m_subdivideLevel[0]->m_edge[theEdge2->m_leftEdge]->m_right=
                theFace->m_subFace[i]=
                target->m_face.add(new SubdivideFace(-theEdge1->m_rightEdge,theEdge1->m_endEdge,-theEdge2->m_endEdge,theEdge2->m_leftEdge));
            }
        }
        else
        {
            SubdivideEdge *theEdge1=original->m_edge[-theFace->m_edge[i]];
            if(theFace->m_edge[i2]>0)
            {
                SubdivideEdge *theEdge2=original->m_edge[theFace->m_edge[i2]];
                m_subdivideLevel[0]->m_edge[theEdge1->m_leftEdge]->m_left=
                m_subdivideLevel[0]->m_edge[theEdge1->m_startEdge]->m_left=
                m_subdivideLevel[0]->m_edge[theEdge2->m_startEdge]->m_right=
                m_subdivideLevel[0]->m_edge[theEdge2->m_rightEdge]->m_right=
                theFace->m_subFace[i]=
                target->m_face.add(new SubdivideFace(-theEdge1->m_leftEdge,-theEdge1->m_startEdge,theEdge2->m_startEdge,theEdge2->m_rightEdge));
            }
            else
            {
                SubdivideEdge *theEdge2=original->m_edge[-theFace->m_edge[i2]];
                m_subdivideLevel[0]->m_edge[theEdge1->m_leftEdge]->m_left=
                m_subdivideLevel[0]->m_edge[theEdge1->m_startEdge]->m_left=
                m_subdivideLevel[0]->m_edge[theEdge2->m_endEdge]->m_left=
                m_subdivideLevel[0]->m_edge[theEdge2->m_leftEdge]->m_right=
                theFace->m_subFace[i]=
                target->m_face.add(new SubdivideFace(-theEdge1->m_leftEdge,-theEdge1->m_startEdge,-theEdge2->m_endEdge,theEdge2->m_leftEdge));
            }
        }
    }
}

void Object::clearPSCache()
{
    unsigned int cacheSize=m_PSFaceCache.size();
    for(int i=cacheSize-1;i>-1;--i)
    {
        m_PSFaceCache[i]->m_isSub=false;
        m_PSFaceCache.pop_back();
    }
    m_PSFaceCache.clear();
    for(unsigned int e=0;e<5;e++)
    {
        cacheSize=m_PSSubFaceCache[e].size();
        for(int i=cacheSize-1;i>-1;--i)
        {
            (m_PSSubFaceCache[e])[i]->m_isSub=false;
            m_PSSubFaceCache[e].pop_back();
        }
        m_PSSubFaceCache[e].clear();
    }
}

void Object::mirror(MirrorMode type,Vector &them_mirrorPosition)
{
    m_mirrorMode=type;
    m_mirrorPosition=them_mirrorPosition;
    m_isMirror=true;
}

void Object::unMirror()
    {
        m_mirrorMode=MirrorMode::Nothing;
        m_mirrorPosition.null();
        m_isMirror=false;
    }

void Object::directPushVertex(Vertex *theVertex)
    {
        if(theVertex)
        {
            m_vertexArray.directPush(theVertex);
        }
        else
        {
            m_vertexArray.pushNullS();
        }
    }

void Object::directPushEdge(Edge *theEdge)
    {
        if(theEdge)
        {
            m_edgeArray.directPush(theEdge);
        }
        else
        {
            m_edgeArray.pushNullS();
        }
    }

void Object::directPushFace(Face *theFace)
        {
            if(theFace)
            {
                m_faceArray.directPush(theFace);
            }
            else
            {
                m_faceArray.pushNullS();
            }
        }

void Object::normalizeVertexNormal()
    {
        for(unsigned int i=1;i<m_subdivideLevel[0]->m_vertex.size();++i)
        {
            m_subdivideLevel[0]->m_vertex[i]->m_normal.normalize();
        }
    }

void Object::updateFNormal(Face *theFace)
    {
        unsigned int edgeCount=theFace->m_edge.size();
        Vector *theVector=new Vector[edgeCount];
        for(unsigned int i=0;i<edgeCount;++i)
        {
            theVector[i].null();
            if(theFace->m_edge[i]>0)
            {
                theVector[i]=m_vertexArray[m_edgeArray[theFace->m_edge[i]]->m_end]->m_position-m_vertexArray[m_edgeArray[theFace->m_edge[i]]->m_start]->m_position;
            }
            else
            {
                theVector[i]=m_vertexArray[m_edgeArray[-theFace->m_edge[i]]->m_start]->m_position-m_vertexArray[m_edgeArray[-theFace->m_edge[i]]->m_end]->m_position;
            }
        }
        --edgeCount;
        for(unsigned int i=0;i<edgeCount;++i)
        {
            theFace->m_normal+=perpendicular(theVector[i],theVector[i+1]);
        }
        theFace->m_normal+=perpendicular(theVector[edgeCount],theVector[0]);
        theFace->m_normal.normalize();
        delete theVector;
    }

void Object::updateFNormal(SubdivideFace *theFace,unsigned int level)
    {
        Vector theVector[4];
        for(unsigned int i=0;i<4;++i)
        {
            theVector[i].null();
            if(theFace->m_edge[i]>0)
            {
                theVector[i]=m_subdivideLevel[level]->m_vertex[m_subdivideLevel[level]->m_edge[theFace->m_edge[i]]->m_end]->m_position-m_subdivideLevel[level]->m_vertex[m_subdivideLevel[level]->m_edge[theFace->m_edge[i]]->m_start]->m_position;
            }
            else
            {
                theVector[i]=m_subdivideLevel[level]->m_vertex[m_subdivideLevel[level]->m_edge[-theFace->m_edge[i]]->m_start]->m_position-m_subdivideLevel[level]->m_vertex[m_subdivideLevel[level]->m_edge[-theFace->m_edge[i]]->m_end]->m_position;
            }
        }
        for(unsigned int i=0;i<3;++i)
        {
            theFace->m_normal+=perpendicular(theVector[i],theVector[i+1]);
        }
        theFace->m_normal+=perpendicular(theVector[3],theVector[0]);
        theFace->m_normal.normalize();
    }

void Object::updateAllSubNormal()
    {
        unsigned int faceCount=m_subdivideLevel[0]->m_face.size();
        for(unsigned int i=1;i<faceCount;++i)
        {
            updateFNormal(m_subdivideLevel[0]->m_face[i],0);
        }
        unsigned int vertexCount=m_subdivideLevel[0]->m_vertex.size();
        for(unsigned int i=1;i<vertexCount;++i)
        {
            updateVNormal(m_subdivideLevel[0]->m_vertex[i],0);
        }
    }

//µÃµ½±ßµÄÊýÄ¿
unsigned int Object::edgeCount()
    {
        return m_edgeArray.size();
    }

unsigned int Object::vertexCount()
    {
        return m_vertexArray.size();
    }

unsigned int Object::faceCount()
    {
        return m_faceArray.size();
    }

void Object::vertexPositionChangeR(unsigned int vertexID,float nx,float ny,float nz)
    {
        Vertex *theVertex=m_vertexArray[vertexID];
        historyManager->record(new Log_VertexPositionChange(m_index,vertexID,theVertex->m_position.x,theVertex->m_position.y,theVertex->m_position.z));
        theVertex->m_position.x+=nx;
        theVertex->m_position.y+=ny;
        theVertex->m_position.z+=nz;
    }

void Object::vertexNormalChange(unsigned int vertexID,float nx,float ny,float nz)
    {
        Vertex *theVertex=m_vertexArray[vertexID];
        historyManager->record(new Log_VertexNormalChange(m_index,vertexID,theVertex->m_normal.x,theVertex->m_normal.y,theVertex->m_normal.z));
        theVertex->m_normal.x=nx;
        theVertex->m_normal.y=ny;
        theVertex->m_normal.z=nz;
    }

void Object::objectEdgeRemove(unsigned int edgeID)
    {
        if(!historyManager->record(new Log_ObjectEdgeRemove(m_index,edgeID,m_edgeArray[edgeID])))
        {
            delete m_edgeArray[edgeID];
        }
        m_edgeArray.remove(edgeID);
    }

void Object::objectVertexRemove(unsigned int vertexID)
    {
        if(!historyManager->record(new Log_ObjectVertexRemove(m_index,vertexID,m_vertexArray[vertexID])))
        {
            delete m_vertexArray[vertexID];
        }
        m_vertexArray.remove(vertexID);
    }

void Object::edgeRightChange(unsigned int edgeID,unsigned int nr)
    {
        historyManager->record(new Log_EdgeRightChange(m_index,edgeID,m_edgeArray[edgeID]->m_right));
        m_edgeArray[edgeID]->m_right=nr;
    }

void Object::edgeStartChange(unsigned int edgeID,unsigned int ns)
    {
        historyManager->record(new Log_EdgeStartChange(m_index,edgeID,m_edgeArray[edgeID]->m_start));
        m_edgeArray[edgeID]->m_start=ns;
    }

void Object::vertexAdjacentRemove(unsigned int vertexID,unsigned int adjID)
    {
        unsigned int adjEdgeCount=m_vertexArray[vertexID]->m_adjacentEdgeList.size()-1;
        historyManager->record(new Log_VertexAdjacentRemove(m_index,vertexID,adjID,m_vertexArray[vertexID]->m_adjacentEdgeList[adjID]));
        unsigned int temp=m_vertexArray[vertexID]->m_adjacentEdgeList[adjEdgeCount];
        m_vertexArray[vertexID]->m_adjacentEdgeList[adjEdgeCount]=m_vertexArray[vertexID]->m_adjacentEdgeList[adjID];
        m_vertexArray[vertexID]->m_adjacentEdgeList[adjID]=temp;
        m_vertexArray[vertexID]->m_adjacentEdgeList.pop_back();
    }

void Object::vertexAdjacentInsert(unsigned int vertexID,unsigned int adjID,unsigned int ne)
    {
        historyManager->record(new Log_VertexAdjacentInsert(m_index,vertexID,adjID));
        m_vertexArray[vertexID]->m_adjacentEdgeList.push_back(ne);
        unsigned int temp=m_vertexArray[vertexID]->m_adjacentEdgeList[adjID];
        unsigned int adjEdgeCount=m_vertexArray[vertexID]->m_adjacentEdgeList.size()-1;
        m_vertexArray[vertexID]->m_adjacentEdgeList[adjID]=m_vertexArray[vertexID]->m_adjacentEdgeList[adjEdgeCount];
        m_vertexArray[vertexID]->m_adjacentEdgeList[adjEdgeCount]=temp;
    }

void Object::faceEdgePush(unsigned int faceID,int nEdge)
    {
        historyManager->record(new Log_FaceEdgePush(m_index,faceID));
        m_faceArray[faceID]->m_edge.push_back(nEdge);
    }

void Object::faceEdgeChange(unsigned int faceID,unsigned int edgeID,int nEdge)
    {
        historyManager->record(new Log_FaceEdgeChange(m_index,faceID,edgeID,m_faceArray[faceID]->m_edge[edgeID]));
        m_faceArray[faceID]->m_edge[edgeID]=nEdge;
    }

void Object::faceEdgeInsert(unsigned int faceID,unsigned int edgeID,int nEdge)
    {
        historyManager->record(new Log_FaceEdgeInsert(m_index,faceID,edgeID));
        Face *theFace=m_faceArray[faceID];
        theFace->m_edge.push_back(0);
        for(unsigned int h=theFace->m_edge.size()-1;h>edgeID;--h)
        {
            theFace->m_edge[h]=theFace->m_edge[h-1];
        }
        theFace->m_edge[edgeID]=nEdge;
    }

void Object::vertexAdjacentPush(unsigned int vertexID,unsigned int ne)
    {
        historyManager->record(new Log_VertexAdjacentPush(m_index,vertexID));
        m_vertexArray[vertexID]->m_adjacentEdgeList.push_back(ne);
    }

void Object::vertexAdjacentChange(unsigned int vertexID,unsigned int edgeID,unsigned int ne)
    {
        historyManager->record(new Log_VertexAdjacentChange(m_index,vertexID,edgeID,m_vertexArray[vertexID]->m_adjacentEdgeList[edgeID]));
        m_vertexArray[vertexID]->m_adjacentEdgeList[edgeID]=ne;
    }

void Object::vertexAdjacentPop(unsigned int vertexID)
    {
        historyManager->record(new Log_VertexAdjacentPop(m_index,vertexID,m_vertexArray[vertexID]->m_adjacentEdgeList[m_vertexArray[vertexID]->m_adjacentEdgeList.size()-1]));
        m_vertexArray[vertexID]->m_adjacentEdgeList.pop_back();
    }
void Object::faceEdgeSwap(unsigned int faceID,unsigned int i1,unsigned int i2)
    {
        historyManager->record(new Log_FaceEdgeSwap(m_index,faceID,i1,i2));
        int temp=m_faceArray[faceID]->m_edge[i1];
        m_faceArray[faceID]->m_edge[i1]=m_faceArray[faceID]->m_edge[i2];
        m_faceArray[faceID]->m_edge[i2]=temp;
    }
void Object::faceEdgeRemove(unsigned int faceID,unsigned int edgeID)
    {
        Face *theFace=m_faceArray[faceID];
        historyManager->record(new Log_FaceEdgeRemove(m_index,faceID,edgeID,theFace->m_edge[edgeID]));
        unsigned int edgeCount=theFace->m_edge.size();
        for(unsigned int e=edgeID+1;e<edgeCount;++e)
        {
            theFace->m_edge[e-1]=theFace->m_edge[e];
        }
        theFace->m_edge.pop_back();
    }

void Object::redefineControlPoint()
    {
        m_vertexArray.~IndexArray();
        m_vertexArray.pushNull();
        unsigned int vertexCount=m_subdivideLevel[0]->m_vertex.size();
        //m_vertexArray.clear();
        m_vertexArray.reserve(vertexCount);
        for(unsigned int i=1;i<vertexCount;i++)
        {
            addVertex(m_subdivideLevel[0]->m_vertex[i]->m_position,m_subdivideLevel[0]->m_vertex[i]->m_normal);
        }
        m_edgeArray.~IndexArray();
        m_edgeArray.pushNull();
        unsigned int edgeCount=m_subdivideLevel[0]->m_edge.size();
        //m_edgeArray.clear();
        m_edgeArray.reserve(edgeCount);
        for(unsigned int i=1;i<edgeCount;i++)
        {
        //    printf("-%d-",m_subdivideLevel[0]->m_edge[i]->m_start);
        //    printf("-%d-",m_subdivideLevel[0]->m_edge[i]->m_end);
            addEdge(m_subdivideLevel[0]->m_edge[i]->m_start,m_subdivideLevel[0]->m_edge[i]->m_end);
        }
        m_faceArray.~IndexArray();
        m_faceArray.pushNull();
        unsigned int faceCount=m_subdivideLevel[0]->m_face.size();
        //m_faceArray.clear();
        m_faceArray.reserve(faceCount);
        for(unsigned int i=1;i<faceCount;i++)
        {
            //printf("-%d--",i);
            SubdivideFace *theSF=m_subdivideLevel[0]->m_face[i];
            unsigned int tempEdge[4]={0};
            tempEdge[0]=theSF->m_edge[0]>0?theSF->m_edge[0]:-(theSF->m_edge[0]);
            tempEdge[1]=theSF->m_edge[1]>0?theSF->m_edge[1]:-(theSF->m_edge[1]);
            tempEdge[2]=theSF->m_edge[2]>0?theSF->m_edge[2]:-(theSF->m_edge[2]);
            tempEdge[3]=theSF->m_edge[3]>0?theSF->m_edge[3]:-(theSF->m_edge[3]);
            addFace(tempEdge,4);
        }

        for(unsigned int i=0;i<(unsigned int)m_subdivideLevelSize;i++)
        {
            delete m_subdivideLevel[i];
            m_subdivideLevel[i]=NULL;
        }
        m_subdivideLevelSize=0;
    }


void Object::testXMLOut(char *fileName)
    {
        FILE *fp=fopen(fileName,"w");
        fprintf(fp,"<O>\n");
        for(unsigned int i=0;i<m_vertexArray.size();++i)
        {
            if(!m_vertexArray[i]) continue;
            fprintf(fp,"<V i=\"%d\">\n",m_vertexArray[i]->m_index);
            for(unsigned int e=0;e<m_vertexArray[i]->m_adjacentEdgeList.size();e++)
            {
                fprintf(fp,"<AE a=\"%d\" />\n",m_vertexArray[i]->m_adjacentEdgeList[e]);
            }
            fprintf(fp,"</V>\n");
        }
        for(unsigned int i=0;i<m_edgeArray.size();i++)
        {
            if(!m_edgeArray[i]) continue;
            fprintf(fp,"<E i=\"%d\" s=\"%d\" e=\"%d\" l=\"%d\" r=\"%d\" />\n",m_edgeArray[i]->m_index,m_edgeArray[i]->m_start,m_edgeArray[i]->m_end,m_edgeArray[i]->m_left,m_edgeArray[i]->m_right);
        }
        for(unsigned int i=0;i<m_faceArray.size();i++)
        {
            if(!m_faceArray[i]) continue;
            fprintf(fp,"<F i=\"%d\">\n",m_faceArray[i]->m_index);
            for(unsigned int e=0;e<m_faceArray[i]->m_edge.size();e++)
                fprintf(fp,"<FE e=\"%d\" />\n",m_faceArray[i]->m_edge[e]);
            fprintf(fp,"</F>\n");
        }
        fprintf(fp,"</O>");
        fclose(fp);
    }


void Object::vertexAdjacentSwap(unsigned int vertexID,unsigned int i1,unsigned int i2)
    {
        historyManager->record(new Log_VertexAdjacentSwap(m_index,vertexID,i1,i2));
        unsigned int temp=m_vertexArray[vertexID]->m_adjacentEdgeList[i1];
        m_vertexArray[vertexID]->m_adjacentEdgeList[i1]=m_vertexArray[vertexID]->m_adjacentEdgeList[i2];
        m_vertexArray[vertexID]->m_adjacentEdgeList[i2]=temp;
    }

void Object::faceEdgePop(unsigned int faceID)
    {
        historyManager->record(new Log_FaceEdgePop(m_index,faceID,m_faceArray[faceID]->m_edge[m_faceArray[faceID]->m_edge.size()-1]));
        m_faceArray[faceID]->m_edge.pop_back();
    }

void Object::edgeEndChange(unsigned int edgeID,unsigned int ne)
    {
        historyManager->record(new Log_EdgeEndChange(m_index,edgeID,m_edgeArray[edgeID]->m_end));
        m_edgeArray[edgeID]->m_end=ne;
    }

void Object::edgeLeftChange(unsigned int edgeID,unsigned int nl)
    {
        historyManager->record(new Log_EdgeLeftChange(m_index,edgeID,m_edgeArray[edgeID]->m_left));
        m_edgeArray[edgeID]->m_left=nl;
    }

void Object::objectFaceRemove(unsigned int faceID)
    {
        if(!historyManager->record(new Log_ObjectFaceRemove(m_index,faceID,m_faceArray[faceID])))
        {
            delete m_faceArray[faceID];
        }
        m_faceArray.remove(faceID);
    }

    void Object::vertexPositionChangeA(unsigned int vertexID,float nx,float ny,float nz)
    {
        Vertex *theVertex=m_vertexArray[vertexID];
        historyManager->record(new Log_VertexPositionChange(m_index,vertexID,theVertex->m_position.x,theVertex->m_position.y,theVertex->m_position.z));
        theVertex->m_position.x=nx;
        theVertex->m_position.y=ny;
        theVertex->m_position.z=nz;
    }


void Object::updateVNormal(Vertex *theVertex)
    {
        unsigned int adjCount=theVertex->m_adjacentEdgeList.size();
        theVertex->m_normal.null();
        for(unsigned int i=0;i<adjCount;++i)
        {
            if(m_edgeArray[theVertex->m_adjacentEdgeList[i]]->m_start==theVertex->m_index)
            {
                Face *theFace=m_faceArray[m_edgeArray[theVertex->m_adjacentEdgeList[i]]->m_left];
                if(theFace)
                theVertex->m_normal+=theFace->m_normal;
            }
            else
            {
                Face *theFace=m_faceArray[m_edgeArray[theVertex->m_adjacentEdgeList[i]]->m_right];
                if(theFace)
                theVertex->m_normal+=theFace->m_normal;
            }
        }
        theVertex->m_normal.normalize();
    }


void Object::updateVNormal(SubdivideVertex *theVertex,unsigned int level)
    {
        unsigned int adjCount=theVertex->m_adjacentEdgeList.size();
        theVertex->m_normal.null();
        for(unsigned int i=0;i<adjCount;++i)
        {
            if(m_subdivideLevel[level]->m_edge[theVertex->m_adjacentEdgeList[i]]->m_start==theVertex->m_index)
            {
                SubdivideFace *theFace=m_subdivideLevel[level]->m_face[m_subdivideLevel[level]->m_edge[theVertex->m_adjacentEdgeList[i]]->m_left];
                if(theFace)
                theVertex->m_normal+=theFace->m_normal;
            }
            else
            {
                SubdivideFace *theFace=m_subdivideLevel[level]->m_face[m_subdivideLevel[level]->m_edge[theVertex->m_adjacentEdgeList[i]]->m_right];
                if(theFace)
                theVertex->m_normal+=theFace->m_normal;
            }
        }
        theVertex->m_normal.normalize();
    }


void Object::draw()
    {
        if(!m_isHide)
        {
            /*glMatrixMode(GL_MODELVIEW);
            glPushMatrix();
            glTranslatef(m_position.x,m_position.y,m_position.z);
            glTranslatef(m_center.x,m_center.y,m_center.z);
            glRotatef(m_rotation.w,m_rotation.x,m_rotation.y,m_rotation.z);
            glScalef(m_scale.x,m_scale.y,m_scale.z);
            glTranslatef(-m_center.x,-m_center.y,-m_center.z);

            if(m_renderMode==RenderType::Faced)
            {
                drawFaced();
            }
            else if(m_renderMode==RenderType::Smooth)
            {
                drawSmooth();
            }
            else if(m_renderMode==RenderType::Wire)
            {
                drawWire();
            }
            else if(m_renderMode==RenderType::Wireframe)
            {
                drawWireframe();
            }
            else if(m_renderMode==RenderType::WireframeFaced)
            {
                drawWireframeFaced();
            }
            glPopMatrix();*/
        }
    }


void Object::drawObjectSelected()
    {
       /* glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glTranslatef(m_position.x,m_position.y,m_position.z);
        glTranslatef(m_center.x,m_center.y,m_center.z);
        glRotatef(m_rotation.w,m_rotation.x,m_rotation.y,m_rotation.z);
glScalef(m_scale.x,m_scale.y,m_scale.z);
        glTranslatef(-m_center.x,-m_center.y,-m_center.z);

        if(m_renderMode==RenderType::Smooth || m_renderMode==RenderType::Wireframe)
        {
        drawSmoothObjectSelected();
        }
        else if(m_renderMode==RenderType::WireframeFaced || m_renderMode==RenderType::Faced)
        {
        drawFacedObjectSelected();
        }
        else if(m_renderMode==RenderType::Wire)
        {
        drawWireObjectSelected();
        }
            glPopMatrix();*/
    }

void Object::drawWireVertexSelected()
{
  /*  glLineWidth(1);
    glDisable(GL_LIGHTING);
    glColor3ub(114,150,250);
    glLineWidth(1);
    if(m_subdivideLevelSize==0)
    {
        glBegin(GL_LINES);
        for(unsigned int i=1;i<m_edgeArray.size();++i)
        {
            Edge *e=m_edgeArray[i];
            if(e)
            {
                Vertex *start=m_vertexArray[e->m_start];
                Vertex *end=m_vertexArray[e->m_end];
                glVertex3f(start->m_position.x,start->m_position.y,start->m_position.z);
                glVertex3f(end->m_position.x,end->m_position.y,end->m_position.z);
            }
        }
        glEnd();
    }
    else
    {
        glBegin(GL_LINES);
        for(unsigned int i=1;i<m_edgeArray.size();++i)
        {
            Edge *e=m_edgeArray[i];
            if(e)
            {
                Vertex *start=m_vertexArray[e->m_start];
                Vertex *end=m_vertexArray[e->m_end];
                glVertex3f(start->m_position.x,start->m_position.y,start->m_position.z);
                glVertex3f(end->m_position.x,end->m_position.y,end->m_position.z);
            }
        }
        glColor3ub(0,0,0);
        for(unsigned int i=1;i<m_subdivideLevel[0]->m_edge.size();++i)
        {
            SubdivideEdge *e=m_subdivideLevel[0]->m_edge[i];
            SubdivideVertex *start=m_subdivideLevel[0]->m_vertex[e->m_start];
            SubdivideVertex *end=m_subdivideLevel[0]->m_vertex[e->m_end];
            glVertex3f(start->m_position.x,start->m_position.y,start->m_position.z);
            glVertex3f(end->m_position.x,end->m_position.y,end->m_position.z);
        }
        glEnd();
    }
    glEnable(GL_LIGHTING);*/
}

void Object::drawWireEdgeSelected()
{
      /*      glLineWidth(1);
        glDisable(GL_LIGHTING);
        glColor3ub(114,150,250);
        glLineWidth(1);
        if(m_subdivideLevelSize==0)
        {
            glBegin(GL_LINES);
            for(unsigned int i=1;i<m_edgeArray.size();++i)
            {
                Edge *e=m_edgeArray[i];
                if(e && !(e->m_isSelected))
                {
                    Vertex *start=m_vertexArray[e->m_start];
                    Vertex *end=m_vertexArray[e->m_end];
                    glVertex3f(start->m_position.x,start->m_position.y,start->m_position.z);
                    glVertex3f(end->m_position.x,end->m_position.y,end->m_position.z);
                }
            }
            glEnd();
        }
        else
        {
            glBegin(GL_LINES);
            for(unsigned int i=1;i<m_edgeArray.size();++i)
            {
                Edge *e=m_edgeArray[i];
                if(e)
                {
                    Vertex *start=m_vertexArray[e->m_start];
                    Vertex *end=m_vertexArray[e->m_end];
                    glVertex3f(start->m_position.x,start->m_position.y,start->m_position.z);
                    glVertex3f(end->m_position.x,end->m_position.y,end->m_position.z);
                }
            }
            glColor3ub(0,0,0);
            for(unsigned int i=1;i<m_subdivideLevel[0]->m_edge.size();++i)
            {
                SubdivideEdge *e=m_subdivideLevel[0]->m_edge[i];
                SubdivideVertex *start=m_subdivideLevel[0]->m_vertex[e->m_start];
                SubdivideVertex *end=m_subdivideLevel[0]->m_vertex[e->m_end];
                glVertex3f(start->m_position.x,start->m_position.y,start->m_position.z);
                glVertex3f(end->m_position.x,end->m_position.y,end->m_position.z);
            }
            glEnd();
        }
        glEnable(GL_LIGHTING);*/
    }

void Object::drawWireFaceSelected()
{
    /*    glLineWidth(1);
        glDisable(GL_LIGHTING);
        glColor3ub(114,150,250);
        glLineWidth(1);
        if(m_subdivideLevelSize==0)
        {
            glBegin(GL_LINES);
            for(unsigned int i=1;i<m_edgeArray.size();++i)
            {
                Edge *e=m_edgeArray[i];
                if(e)
                {
                    Vertex *start=m_vertexArray[e->m_start];
                    Vertex *end=m_vertexArray[e->m_end];
                    glVertex3f(start->m_position.x,start->m_position.y,start->m_position.z);
                    glVertex3f(end->m_position.x,end->m_position.y,end->m_position.z);
                }
            }
            glEnd();
        }
        else
        {
            glBegin(GL_LINES);
            for(unsigned int i=1;i<m_edgeArray.size();++i)
            {
                Edge *e=m_edgeArray[i];
                if(e)
                {
                    Vertex *start=m_vertexArray[e->m_start];
                    Vertex *end=m_vertexArray[e->m_end];
                    glVertex3f(start->m_position.x,start->m_position.y,start->m_position.z);
                    glVertex3f(end->m_position.x,end->m_position.y,end->m_position.z);
                }
            }
            glColor3ub(0,0,0);
            for(unsigned int i=1;i<m_subdivideLevel[0]->m_edge.size();++i)
            {
                SubdivideEdge *e=m_subdivideLevel[0]->m_edge[i];
                SubdivideVertex *start=m_subdivideLevel[0]->m_vertex[e->m_start];
                SubdivideVertex *end=m_subdivideLevel[0]->m_vertex[e->m_end];
                glVertex3f(start->m_position.x,start->m_position.y,start->m_position.z);
                glVertex3f(end->m_position.x,end->m_position.y,end->m_position.z);
            }
            glEnd();
        }
        glEnable(GL_LIGHTING);*/
    }


void Object::drawWireObjectSelected()
    {
    /*    glLineWidth(1);
        glDisable(GL_LIGHTING);
        glColor3ub(114,150,250);
        glLineWidth(1);
        if(m_subdivideLevelSize==0)
        {
            glBegin(GL_LINES);
            for(unsigned int i=1;i<m_edgeArray.size();++i)
            {
                Edge *e=m_edgeArray[i];
                if(e)
                {
                    Vertex *start=m_vertexArray[e->m_start];
                    Vertex *end=m_vertexArray[e->m_end];
                    glVertex3f(start->m_position.x,start->m_position.y,start->m_position.z);
                    glVertex3f(end->m_position.x,end->m_position.y,end->m_position.z);
                }
            }
            glEnd();
        }
        else
        {
            glBegin(GL_LINES);
            for(unsigned int i=1;i<m_edgeArray.size();++i)
            {
                Edge *e=m_edgeArray[i];
                if(e)
                {
                    Vertex *start=m_vertexArray[e->m_start];
                    Vertex *end=m_vertexArray[e->m_end];
                    glVertex3f(start->m_position.x,start->m_position.y,start->m_position.z);
                    glVertex3f(end->m_position.x,end->m_position.y,end->m_position.z);
                }
            }
            glColor3ub(0,0,0);
            for(unsigned int i=1;i<m_subdivideLevel[0]->m_edge.size();++i)
            {
                SubdivideEdge *e=m_subdivideLevel[0]->m_edge[i];
                SubdivideVertex *start=m_subdivideLevel[0]->m_vertex[e->m_start];
                SubdivideVertex *end=m_subdivideLevel[0]->m_vertex[e->m_end];
                glVertex3f(start->m_position.x,start->m_position.y,start->m_position.z);
                glVertex3f(end->m_position.x,end->m_position.y,end->m_position.z);
            }
            glEnd();
        }
        glEnable(GL_LIGHTING);*/
    }


void Object::drawSmoothObjectSelected()
    {
    /*    glMaterialfv(GL_FRONT, GL_AMBIENT, m_matAmbient);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, m_matDiffuse);
        glMaterialfv(GL_FRONT, GL_SPECULAR, m_matSpecular);
        glMaterialfv(GL_FRONT, GL_SHININESS, m_matShininess);
        glMaterialfv(GL_FRONT, GL_EMISSION, m_matEmission);
        glEnable(GL_LIGHTING);
        if(m_subdivideLevelSize==0)
        {
            glEnable(GL_POLYGON_OFFSET_FILL);
            glPolygonOffset(1.0f,1.0f);
            for(unsigned int i=1;i<m_faceArray.size();++i)
            {
                if(m_faceArray[i])
                {
                    Face *theFace=m_faceArray[i];
                    glBegin(GL_POLYGON);
                    for(unsigned int e=0;e<theFace->m_edge.size();++e)
                    {
                        if(theFace->m_edge[e]>0)
                        {
                            Vertex *v=m_vertexArray[m_edgeArray[theFace->m_edge[e]]->m_start];
                            glNormal3f(v->m_normal.x,v->m_normal.y,v->m_normal.z);
                            glVertex3f(v->m_position.x,v->m_position.y,v->m_position.z);
                        }
                        else
                        {
                            Vertex *v=m_vertexArray[m_edgeArray[-theFace->m_edge[e]]->m_end];
                            glNormal3f(v->m_normal.x,v->m_normal.y,v->m_normal.z);
                            glVertex3f(v->m_position.x,v->m_position.y,v->m_position.z);
                        }
                    }
                    glEnd();
                }
            }
            glDisable(GL_POLYGON_OFFSET_FILL);

            glLineWidth(1);
            glDisable(GL_LIGHTING);
            glColor3ub(114,186,221);
            glLineWidth(1);
            glBegin(GL_LINES);
            for(unsigned int i=1;i<m_edgeArray.size();++i)
            {
                Edge *e=m_edgeArray[i];
                if(e)
                {
                    Vertex *start=m_vertexArray[e->m_start];
                    Vertex *end=m_vertexArray[e->m_end];
                    glVertex3f(start->m_position.x,start->m_position.y,start->m_position.z);
                    glVertex3f(end->m_position.x,end->m_position.y,end->m_position.z);
                }
            }
            glEnd();
            glEnable(GL_LIGHTING);
        }
        else
        {
            glEnable(GL_POLYGON_OFFSET_FILL);
            glPolygonOffset(1.0f,1.0f);
            glBegin(GL_QUADS);
            for(unsigned int i=1;i<m_subdivideLevel[0]->m_face.size();++i)
            {
                //glBegin(GL_POLYGON);
                SubdivideFace *theFace=m_subdivideLevel[0]->m_face[i];
            //    glNormal3f(theFace->m_normal.x,theFace->m_normal.y,theFace->m_normal.z);
                if(theFace->m_edge[0]>0)
                {
                    SubdivideVertex *v=m_subdivideLevel[0]->m_vertex[m_subdivideLevel[0]->m_edge[theFace->m_edge[0]]->m_start];
                    glNormal3f(v->m_normal.x,v->m_normal.y,v->m_normal.z);
                    glVertex3f(v->m_position.x,v->m_position.y,v->m_position.z);
                }
                else
                {
                    SubdivideVertex *v=m_subdivideLevel[0]->m_vertex[m_subdivideLevel[0]->m_edge[-theFace->m_edge[0]]->m_end];
                    glNormal3f(v->m_normal.x,v->m_normal.y,v->m_normal.z);
                    glVertex3f(v->m_position.x,v->m_position.y,v->m_position.z);
                }
                if(theFace->m_edge[1]>0)
                {
                    SubdivideVertex *v=m_subdivideLevel[0]->m_vertex[m_subdivideLevel[0]->m_edge[theFace->m_edge[1]]->m_start];
                    glNormal3f(v->m_normal.x,v->m_normal.y,v->m_normal.z);
                    glVertex3f(v->m_position.x,v->m_position.y,v->m_position.z);
                }
                else
                {
                    SubdivideVertex *v=m_subdivideLevel[0]->m_vertex[m_subdivideLevel[0]->m_edge[-theFace->m_edge[1]]->m_end];
                    glNormal3f(v->m_normal.x,v->m_normal.y,v->m_normal.z);
                    glVertex3f(v->m_position.x,v->m_position.y,v->m_position.z);
                }
                if(theFace->m_edge[2]>0)
                {
                    SubdivideVertex *v=m_subdivideLevel[0]->m_vertex[m_subdivideLevel[0]->m_edge[theFace->m_edge[2]]->m_start];
                    glNormal3f(v->m_normal.x,v->m_normal.y,v->m_normal.z);
                    glVertex3f(v->m_position.x,v->m_position.y,v->m_position.z);
                }
                else
                {
                    SubdivideVertex *v=m_subdivideLevel[0]->m_vertex[m_subdivideLevel[0]->m_edge[-theFace->m_edge[2]]->m_end];
                    glNormal3f(v->m_normal.x,v->m_normal.y,v->m_normal.z);
                    glVertex3f(v->m_position.x,v->m_position.y,v->m_position.z);
                }
                if(theFace->m_edge[3]>0)
                {
                    SubdivideVertex *v=m_subdivideLevel[0]->m_vertex[m_subdivideLevel[0]->m_edge[theFace->m_edge[3]]->m_start];
                    glNormal3f(v->m_normal.x,v->m_normal.y,v->m_normal.z);
                    glVertex3f(v->m_position.x,v->m_position.y,v->m_position.z);
                }
                else
                {
                    SubdivideVertex *v=m_subdivideLevel[0]->m_vertex[m_subdivideLevel[0]->m_edge[-theFace->m_edge[3]]->m_end];
                    glNormal3f(v->m_normal.x,v->m_normal.y,v->m_normal.z);
                    glVertex3f(v->m_position.x,v->m_position.y,v->m_position.z);
                }
            //    glEnd();
            }
            glEnd();
            glDisable(GL_POLYGON_OFFSET_FILL);
            glLineWidth(1);
            glDisable(GL_LIGHTING);
            glColor3ub(0,0,0);
            glLineWidth(1);
            glBegin(GL_LINES);
            for(unsigned int i=1;i<m_subdivideLevel[0]->m_edge.size();++i)
            {
                SubdivideEdge *e=m_subdivideLevel[0]->m_edge[i];
                SubdivideVertex *start=m_subdivideLevel[0]->m_vertex[e->m_start];
                SubdivideVertex *end=m_subdivideLevel[0]->m_vertex[e->m_end];
                glVertex3f(start->m_position.x,start->m_position.y,start->m_position.z);
                glVertex3f(end->m_position.x,end->m_position.y,end->m_position.z);
            }
            glColor3ub(114,186,221);
            for(unsigned int i=1;i<m_edgeArray.size();++i)
            {
                Edge *e=m_edgeArray[i];
                if(e)
                {
                    Vertex *start=m_vertexArray[e->m_start];
                    Vertex *end=m_vertexArray[e->m_end];
                    glVertex3f(start->m_position.x,start->m_position.y,start->m_position.z);
                    glVertex3f(end->m_position.x,end->m_position.y,end->m_position.z);
                }
            }
            glEnd();
            glEnable(GL_LIGHTING);
        }*/

    }

void Object::drawFacedObjectSelected()
    {
     /*   glMaterialfv(GL_FRONT, GL_AMBIENT, m_matAmbient);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, m_matDiffuse);
        glMaterialfv(GL_FRONT, GL_SPECULAR, m_matSpecular);
        glMaterialfv(GL_FRONT, GL_SHININESS, m_matShininess);
        glMaterialfv(GL_FRONT, GL_EMISSION, m_matEmission);
        glEnable(GL_LIGHTING);
        if(m_subdivideLevelSize==0)
        {
            glEnable(GL_POLYGON_OFFSET_FILL);
            glPolygonOffset(1.0f,1.0f);
            for(unsigned int i=1;i<m_faceArray.size();++i)
            {
                if(m_faceArray[i])
                {
                    Face *theFace=m_faceArray[i];
                    glBegin(GL_POLYGON);
                    glNormal3f(theFace->m_normal.x,theFace->m_normal.y,theFace->m_normal.z);
                    for(unsigned int e=0;e<theFace->m_edge.size();++e)
                    {
                        if(theFace->m_edge[e]>0)
                        {
                            Vertex *v=m_vertexArray[m_edgeArray[theFace->m_edge[e]]->m_start];
                            glVertex3f(v->m_position.x,v->m_position.y,v->m_position.z);
                        }
                        else
                        {
                            Vertex *v=m_vertexArray[m_edgeArray[-theFace->m_edge[e]]->m_end];
                            glVertex3f(v->m_position.x,v->m_position.y,v->m_position.z);
                        }
                    }
                    glEnd();
                }
            }
            glDisable(GL_POLYGON_OFFSET_FILL);
            glLineWidth(1);
            glDisable(GL_LIGHTING);
            glColor3ub(114,186,221);
            glLineWidth(1);
            glBegin(GL_LINES);
            for(unsigned int i=1;i<m_edgeArray.size();++i)
            {
                Edge *e=m_edgeArray[i];
                if(e)
                {
                    Vertex *start=m_vertexArray[e->m_start];
                    Vertex *end=m_vertexArray[e->m_end];
                    glVertex3f(start->m_position.x,start->m_position.y,start->m_position.z);
                    glVertex3f(end->m_position.x,end->m_position.y,end->m_position.z);
                }
            }
            glEnd();
            glEnable(GL_LIGHTING);
        }
        else
        {
            glEnable(GL_POLYGON_OFFSET_FILL);
            glPolygonOffset(1.0f,1.0f);
            glBegin(GL_QUADS);
            for(unsigned int i=1;i<m_subdivideLevel[0]->m_face.size();++i)
            {
                SubdivideFace *theFace=m_subdivideLevel[0]->m_face[i];
                glNormal3f(theFace->m_normal.x,theFace->m_normal.y,theFace->m_normal.z);
                if(theFace->m_edge[0]>0)
                {
                    SubdivideVertex *v=m_subdivideLevel[0]->m_vertex[m_subdivideLevel[0]->m_edge[theFace->m_edge[0]]->m_start];
                    glVertex3f(v->m_position.x,v->m_position.y,v->m_position.z);
                }
                else
                {
                    SubdivideVertex *v=m_subdivideLevel[0]->m_vertex[m_subdivideLevel[0]->m_edge[-theFace->m_edge[0]]->m_end];
                    glVertex3f(v->m_position.x,v->m_position.y,v->m_position.z);
                }
                if(theFace->m_edge[1]>0)
                {
                    SubdivideVertex *v=m_subdivideLevel[0]->m_vertex[m_subdivideLevel[0]->m_edge[theFace->m_edge[1]]->m_start];
                    glVertex3f(v->m_position.x,v->m_position.y,v->m_position.z);
                }
                else
                {
                    SubdivideVertex *v=m_subdivideLevel[0]->m_vertex[m_subdivideLevel[0]->m_edge[-theFace->m_edge[1]]->m_end];
                    glVertex3f(v->m_position.x,v->m_position.y,v->m_position.z);
                }
                if(theFace->m_edge[2]>0)
                {
                    SubdivideVertex *v=m_subdivideLevel[0]->m_vertex[m_subdivideLevel[0]->m_edge[theFace->m_edge[2]]->m_start];
                    glVertex3f(v->m_position.x,v->m_position.y,v->m_position.z);
                }
                else
                {
                    SubdivideVertex *v=m_subdivideLevel[0]->m_vertex[m_subdivideLevel[0]->m_edge[-theFace->m_edge[2]]->m_end];
                    glVertex3f(v->m_position.x,v->m_position.y,v->m_position.z);
                }
                if(theFace->m_edge[3]>0)
                {
                    SubdivideVertex *v=m_subdivideLevel[0]->m_vertex[m_subdivideLevel[0]->m_edge[theFace->m_edge[3]]->m_start];
                    glVertex3f(v->m_position.x,v->m_position.y,v->m_position.z);
                }
                else
                {
                    SubdivideVertex *v=m_subdivideLevel[0]->m_vertex[m_subdivideLevel[0]->m_edge[-theFace->m_edge[3]]->m_end];
                    glVertex3f(v->m_position.x,v->m_position.y,v->m_position.z);
                }
            }
            glEnd();
            glDisable(GL_POLYGON_OFFSET_FILL);
            glLineWidth(1);
            glDisable(GL_LIGHTING);
            glColor3ub(0,0,0);
            glLineWidth(1);
            glBegin(GL_LINES);
            for(unsigned int i=1;i<m_subdivideLevel[0]->m_edge.size();++i)
            {
                SubdivideEdge *e=m_subdivideLevel[0]->m_edge[i];
                SubdivideVertex *start=m_subdivideLevel[0]->m_vertex[e->m_start];
                SubdivideVertex *end=m_subdivideLevel[0]->m_vertex[e->m_end];
                glVertex3f(start->m_position.x,start->m_position.y,start->m_position.z);
                glVertex3f(end->m_position.x,end->m_position.y,end->m_position.z);
            }

                        glColor3ub(114,186,221);
            for(unsigned int i=1;i<m_edgeArray.size();++i)
            {
                Edge *e=m_edgeArray[i];
                if(e)
                {
                    Vertex *start=m_vertexArray[e->m_start];
                    Vertex *end=m_vertexArray[e->m_end];
                    glVertex3f(start->m_position.x,start->m_position.y,start->m_position.z);
                    glVertex3f(end->m_position.x,end->m_position.y,end->m_position.z);
                }
            }

            glEnd();
            glEnable(GL_LIGHTING);
        }*/

    }

void Object::drawFacedVertexSelected()
    {
   /*     glMaterialfv(GL_FRONT, GL_AMBIENT, m_matAmbient);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, m_matDiffuse);
        glMaterialfv(GL_FRONT, GL_SPECULAR, m_matSpecular);
        glMaterialfv(GL_FRONT, GL_SHININESS, m_matShininess);
        glMaterialfv(GL_FRONT, GL_EMISSION, m_matEmission);
        glEnable(GL_LIGHTING);
                glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
                glTranslatef(m_position.x,m_position.y,m_position.z);
        glTranslatef(m_center.x,m_center.y,m_center.z);
        glRotatef(m_rotation.w,m_rotation.x,m_rotation.y,m_rotation.z);
glScalef(m_scale.x,m_scale.y,m_scale.z);
        glTranslatef(-m_center.x,-m_center.y,-m_center.z);

        if(m_subdivideLevelSize==0)
        {
            glEnable(GL_POLYGON_OFFSET_FILL);
            glPolygonOffset(1.0f,1.0f);
            for(unsigned int i=1;i<m_faceArray.size();++i)
            {
                if(m_faceArray[i])
                {
                    Face *theFace=m_faceArray[i];
                    glBegin(GL_POLYGON);
                    glNormal3f(theFace->m_normal.x,theFace->m_normal.y,theFace->m_normal.z);
                    for(unsigned int e=0;e<theFace->m_edge.size();++e)
                    {
                        if(theFace->m_edge[e]>0)
                        {
                            Vertex *v=m_vertexArray[m_edgeArray[theFace->m_edge[e]]->m_start];
                            glVertex3f(v->m_position.x,v->m_position.y,v->m_position.z);
                        }
                        else
                        {
                            Vertex *v=m_vertexArray[m_edgeArray[-theFace->m_edge[e]]->m_end];
                            glVertex3f(v->m_position.x,v->m_position.y,v->m_position.z);
                        }
                    }
                    glEnd();
                }
            }
            glDisable(GL_POLYGON_OFFSET_FILL);
            glLineWidth(1);
            glDisable(GL_LIGHTING);
            glColor3ub(114,186,221);
            glLineWidth(1);
            glBegin(GL_LINES);
            for(unsigned int i=1;i<m_edgeArray.size();++i)
            {
                Edge *e=m_edgeArray[i];
                if(e)
                {
                    Vertex *start=m_vertexArray[e->m_start];
                    Vertex *end=m_vertexArray[e->m_end];
                    glVertex3f(start->m_position.x,start->m_position.y,start->m_position.z);
                    glVertex3f(end->m_position.x,end->m_position.y,end->m_position.z);
                }
            }
            glEnd();
            glColor3ub(0,200,0);
            glPointSize(5.0f);
            glBegin(GL_POINTS);

            for(unsigned int i=1;i<m_vertexArray.size();++i)
            {
                Vertex *v=m_vertexArray[i];
                if(v && !v->m_isSelected)
                {
                    glVertex3f(v->m_position.x,v->m_position.y,v->m_position.z);
                }
            }
            glEnd();
            glEnable(GL_LIGHTING);
        }
        else
        {
            glEnable(GL_POLYGON_OFFSET_FILL);
            glPolygonOffset(1.0f,1.0f);
            glBegin(GL_QUADS);
            for(unsigned int i=1;i<m_subdivideLevel[0]->m_face.size();++i)
            {
                SubdivideFace *theFace=m_subdivideLevel[0]->m_face[i];
                glNormal3f(theFace->m_normal.x,theFace->m_normal.y,theFace->m_normal.z);
                if(theFace->m_edge[0]>0)
                {
                    SubdivideVertex *v=m_subdivideLevel[0]->m_vertex[m_subdivideLevel[0]->m_edge[theFace->m_edge[0]]->m_start];
                    glVertex3f(v->m_position.x,v->m_position.y,v->m_position.z);
                }
                else
                {
                    SubdivideVertex *v=m_subdivideLevel[0]->m_vertex[m_subdivideLevel[0]->m_edge[-theFace->m_edge[0]]->m_end];
                    glVertex3f(v->m_position.x,v->m_position.y,v->m_position.z);
                }
                if(theFace->m_edge[1]>0)
                {
                    SubdivideVertex *v=m_subdivideLevel[0]->m_vertex[m_subdivideLevel[0]->m_edge[theFace->m_edge[1]]->m_start];
                    glVertex3f(v->m_position.x,v->m_position.y,v->m_position.z);
                }
                else
                {
                    SubdivideVertex *v=m_subdivideLevel[0]->m_vertex[m_subdivideLevel[0]->m_edge[-theFace->m_edge[1]]->m_end];
                    glVertex3f(v->m_position.x,v->m_position.y,v->m_position.z);
                }
                if(theFace->m_edge[2]>0)
                {
                    SubdivideVertex *v=m_subdivideLevel[0]->m_vertex[m_subdivideLevel[0]->m_edge[theFace->m_edge[2]]->m_start];
                    glVertex3f(v->m_position.x,v->m_position.y,v->m_position.z);
                }
                else
                {
                    SubdivideVertex *v=m_subdivideLevel[0]->m_vertex[m_subdivideLevel[0]->m_edge[-theFace->m_edge[2]]->m_end];
                    glVertex3f(v->m_position.x,v->m_position.y,v->m_position.z);
                }
                if(theFace->m_edge[3]>0)
                {
                    SubdivideVertex *v=m_subdivideLevel[0]->m_vertex[m_subdivideLevel[0]->m_edge[theFace->m_edge[3]]->m_start];
                    glVertex3f(v->m_position.x,v->m_position.y,v->m_position.z);
                }
                else
                {
                    SubdivideVertex *v=m_subdivideLevel[0]->m_vertex[m_subdivideLevel[0]->m_edge[-theFace->m_edge[3]]->m_end];
                    glVertex3f(v->m_position.x,v->m_position.y,v->m_position.z);
                }
            }
            glEnd();
            glDisable(GL_POLYGON_OFFSET_FILL);
            glLineWidth(1);
            glDisable(GL_LIGHTING);
            glColor3ub(0,0,0);
            glLineWidth(1);
            glBegin(GL_LINES);
            for(unsigned int i=1;i<m_subdivideLevel[0]->m_edge.size();++i)
            {
                SubdivideEdge *e=m_subdivideLevel[0]->m_edge[i];
                SubdivideVertex *start=m_subdivideLevel[0]->m_vertex[e->m_start];
                SubdivideVertex *end=m_subdivideLevel[0]->m_vertex[e->m_end];
                glVertex3f(start->m_position.x,start->m_position.y,start->m_position.z);
                glVertex3f(end->m_position.x,end->m_position.y,end->m_position.z);
            }

                        glColor3ub(114,186,221);
            for(unsigned int i=1;i<m_edgeArray.size();++i)
            {
                Edge *e=m_edgeArray[i];
                if(e)
                {
                    Vertex *start=m_vertexArray[e->m_start];
                    Vertex *end=m_vertexArray[e->m_end];
                    glVertex3f(start->m_position.x,start->m_position.y,start->m_position.z);
                    glVertex3f(end->m_position.x,end->m_position.y,end->m_position.z);
                }
            }

            glEnd();
            glColor3ub(0,200,0);
            glPointSize(5.0f);
            glBegin(GL_POINTS);
            for(unsigned int i=1;i<m_vertexArray.size();++i)
            {
                Vertex *v=m_vertexArray[i];
                if(v && !v->m_isSelected)
                {
                    glVertex3f(v->m_position.x,v->m_position.y,v->m_position.z);
                }
            }
            glEnd();
            glEnable(GL_LIGHTING);
        }
        glPopMatrix();*/
    }


void Object::drawFacedFaceSelected()
    {
     /*   glMaterialfv(GL_FRONT, GL_AMBIENT, m_matAmbient);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, m_matDiffuse);
        glMaterialfv(GL_FRONT, GL_SPECULAR, m_matSpecular);
        glMaterialfv(GL_FRONT, GL_SHININESS, m_matShininess);
        glMaterialfv(GL_FRONT, GL_EMISSION, m_matEmission);
        glEnable(GL_LIGHTING);
        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
                glTranslatef(m_position.x,m_position.y,m_position.z);
        glTranslatef(m_center.x,m_center.y,m_center.z);
        glRotatef(m_rotation.w,m_rotation.x,m_rotation.y,m_rotation.z);
        glScalef(m_scale.x,m_scale.y,m_scale.z);
        glTranslatef(-m_center.x,-m_center.y,-m_center.z);

        if(m_subdivideLevelSize==0)
        {
            glEnable(GL_POLYGON_OFFSET_FILL);
            glPolygonOffset(1.0f,1.0f);
            for(unsigned int i=1;i<m_faceArray.size();++i)
            {
                if(m_faceArray[i] && !m_faceArray[i]->m_isSelected)
                {
                    Face *theFace=m_faceArray[i];
                    glBegin(GL_POLYGON);
                    glNormal3f(theFace->m_normal.x,theFace->m_normal.y,theFace->m_normal.z);
                    for(unsigned int e=0;e<theFace->m_edge.size();++e)
                    {
                        if(theFace->m_edge[e]>0)
                        {
                            Vertex *v=m_vertexArray[m_edgeArray[theFace->m_edge[e]]->m_start];
                            glVertex3f(v->m_position.x,v->m_position.y,v->m_position.z);
                        }
                        else
                        {
                            Vertex *v=m_vertexArray[m_edgeArray[-theFace->m_edge[e]]->m_end];
                            glVertex3f(v->m_position.x,v->m_position.y,v->m_position.z);
                        }
                    }
                    glEnd();
                }
            }
            glDisable(GL_POLYGON_OFFSET_FILL);
            glLineWidth(1);
            glDisable(GL_LIGHTING);
            glColor3ub(100,0,0);
            glLineWidth(1);
            glBegin(GL_LINES);
            for(unsigned int i=1;i<m_edgeArray.size();++i)
            {
                Edge *e=m_edgeArray[i];
                if(e)
                {
                    Vertex *start=m_vertexArray[e->m_start];
                    Vertex *end=m_vertexArray[e->m_end];
                    glVertex3f(start->m_position.x,start->m_position.y,start->m_position.z);
                    glVertex3f(end->m_position.x,end->m_position.y,end->m_position.z);
                }
            }
            glEnd();
            glEnable(GL_LIGHTING);
        }
        else
        {
            glEnable(GL_POLYGON_OFFSET_FILL);
            glPolygonOffset(1.0f,1.0f);
            glBegin(GL_QUADS);
            for(unsigned int i=1;i<m_subdivideLevel[0]->m_face.size();++i)
            {
                SubdivideFace *theFace=m_subdivideLevel[0]->m_face[i];
                glNormal3f(theFace->m_normal.x,theFace->m_normal.y,theFace->m_normal.z);
                if(theFace->m_edge[0]>0)
                {
                    SubdivideVertex *v=m_subdivideLevel[0]->m_vertex[m_subdivideLevel[0]->m_edge[theFace->m_edge[0]]->m_start];
                    glVertex3f(v->m_position.x,v->m_position.y,v->m_position.z);
                }
                else
                {
                    SubdivideVertex *v=m_subdivideLevel[0]->m_vertex[m_subdivideLevel[0]->m_edge[-theFace->m_edge[0]]->m_end];
                    glVertex3f(v->m_position.x,v->m_position.y,v->m_position.z);
                }
                if(theFace->m_edge[1]>0)
                {
                    SubdivideVertex *v=m_subdivideLevel[0]->m_vertex[m_subdivideLevel[0]->m_edge[theFace->m_edge[1]]->m_start];
                    glVertex3f(v->m_position.x,v->m_position.y,v->m_position.z);
                }
                else
                {
                    SubdivideVertex *v=m_subdivideLevel[0]->m_vertex[m_subdivideLevel[0]->m_edge[-theFace->m_edge[1]]->m_end];
                    glVertex3f(v->m_position.x,v->m_position.y,v->m_position.z);
                }
                if(theFace->m_edge[2]>0)
                {
                    SubdivideVertex *v=m_subdivideLevel[0]->m_vertex[m_subdivideLevel[0]->m_edge[theFace->m_edge[2]]->m_start];
                    glVertex3f(v->m_position.x,v->m_position.y,v->m_position.z);
                }
                else
                {
                    SubdivideVertex *v=m_subdivideLevel[0]->m_vertex[m_subdivideLevel[0]->m_edge[-theFace->m_edge[2]]->m_end];
                    glVertex3f(v->m_position.x,v->m_position.y,v->m_position.z);
                }
                if(theFace->m_edge[3]>0)
                {
                    SubdivideVertex *v=m_subdivideLevel[0]->m_vertex[m_subdivideLevel[0]->m_edge[theFace->m_edge[3]]->m_start];
                    glVertex3f(v->m_position.x,v->m_position.y,v->m_position.z);
                }
                else
                {
                    SubdivideVertex *v=m_subdivideLevel[0]->m_vertex[m_subdivideLevel[0]->m_edge[-theFace->m_edge[3]]->m_end];
                    glVertex3f(v->m_position.x,v->m_position.y,v->m_position.z);
                }
            }
            glEnd();
            glDisable(GL_POLYGON_OFFSET_FILL);
            glLineWidth(1);
            glDisable(GL_LIGHTING);
            glColor3ub(0,0,0);
            glLineWidth(1);
            glBegin(GL_LINES);
            for(unsigned int i=1;i<m_subdivideLevel[0]->m_edge.size();++i)
            {
                SubdivideEdge *e=m_subdivideLevel[0]->m_edge[i];
                SubdivideVertex *start=m_subdivideLevel[0]->m_vertex[e->m_start];
                SubdivideVertex *end=m_subdivideLevel[0]->m_vertex[e->m_end];
                glVertex3f(start->m_position.x,start->m_position.y,start->m_position.z);
                glVertex3f(end->m_position.x,end->m_position.y,end->m_position.z);
            }

                        glColor3ub(100,0,0);
            for(unsigned int i=1;i<m_edgeArray.size();++i)
            {
                Edge *e=m_edgeArray[i];
                if(e)
                {
                    Vertex *start=m_vertexArray[e->m_start];
                    Vertex *end=m_vertexArray[e->m_end];
                    glVertex3f(start->m_position.x,start->m_position.y,start->m_position.z);
                    glVertex3f(end->m_position.x,end->m_position.y,end->m_position.z);
                }
            }

            glEnd();
            glEnable(GL_LIGHTING);
        }
        glPopMatrix();*/
    }

void Object::drawFacedEdgeSelected()
    {
    /*    glMaterialfv(GL_FRONT, GL_AMBIENT, m_matAmbient);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, m_matDiffuse);
        glMaterialfv(GL_FRONT, GL_SPECULAR, m_matSpecular);
        glMaterialfv(GL_FRONT, GL_SHININESS, m_matShininess);
        glMaterialfv(GL_FRONT, GL_EMISSION, m_matEmission);
        glEnable(GL_LIGHTING);
        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
                glTranslatef(m_position.x,m_position.y,m_position.z);
        glTranslatef(m_center.x,m_center.y,m_center.z);
        glRotatef(m_rotation.w,m_rotation.x,m_rotation.y,m_rotation.z);
        glScalef(m_scale.x,m_scale.y,m_scale.z);
        glTranslatef(-m_center.x,-m_center.y,-m_center.z);

        if(m_subdivideLevelSize==0)
        {
            glEnable(GL_POLYGON_OFFSET_FILL);
            glPolygonOffset(1.0f,1.0f);
            for(unsigned int i=1;i<m_faceArray.size();++i)
            {
                if(m_faceArray[i])
                {
                    Face *theFace=m_faceArray[i];
                    glBegin(GL_POLYGON);
                    glNormal3f(theFace->m_normal.x,theFace->m_normal.y,theFace->m_normal.z);
                    for(unsigned int e=0;e<theFace->m_edge.size();++e)
                    {
                        if(theFace->m_edge[e]>0)
                        {
                            Vertex *v=m_vertexArray[m_edgeArray[theFace->m_edge[e]]->m_start];
                            glVertex3f(v->m_position.x,v->m_position.y,v->m_position.z);
                        }
                        else
                        {
                            Vertex *v=m_vertexArray[m_edgeArray[-theFace->m_edge[e]]->m_end];
                            glVertex3f(v->m_position.x,v->m_position.y,v->m_position.z);
                        }
                    }
                    glEnd();
                }
            }
            glDisable(GL_POLYGON_OFFSET_FILL);
            glLineWidth(1);
            glDisable(GL_LIGHTING);
            glColor3ub(0,0,100);
            glLineWidth(1);
            glBegin(GL_LINES);
            for(unsigned int i=1;i<m_edgeArray.size();++i)
            {
                Edge *e=m_edgeArray[i];
                if(e && !e->m_isSelected)
                {
                    Vertex *start=m_vertexArray[e->m_start];
                    Vertex *end=m_vertexArray[e->m_end];
                    glVertex3f(start->m_position.x,start->m_position.y,start->m_position.z);
                    glVertex3f(end->m_position.x,end->m_position.y,end->m_position.z);
                }
            }
            glEnd();
            glEnable(GL_LIGHTING);
        }
        else
        {
            glEnable(GL_POLYGON_OFFSET_FILL);
            glPolygonOffset(1.0f,1.0f);
            glBegin(GL_QUADS);
            for(unsigned int i=1;i<m_subdivideLevel[0]->m_face.size();++i)
            {
                SubdivideFace *theFace=m_subdivideLevel[0]->m_face[i];
                glNormal3f(theFace->m_normal.x,theFace->m_normal.y,theFace->m_normal.z);
                if(theFace->m_edge[0]>0)
                {
                    SubdivideVertex *v=m_subdivideLevel[0]->m_vertex[m_subdivideLevel[0]->m_edge[theFace->m_edge[0]]->m_start];
                    glVertex3f(v->m_position.x,v->m_position.y,v->m_position.z);
                }
                else
                {
                    SubdivideVertex *v=m_subdivideLevel[0]->m_vertex[m_subdivideLevel[0]->m_edge[-theFace->m_edge[0]]->m_end];
                    glVertex3f(v->m_position.x,v->m_position.y,v->m_position.z);
                }
                if(theFace->m_edge[1]>0)
                {
                    SubdivideVertex *v=m_subdivideLevel[0]->m_vertex[m_subdivideLevel[0]->m_edge[theFace->m_edge[1]]->m_start];
                    glVertex3f(v->m_position.x,v->m_position.y,v->m_position.z);
                }
                else
                {
                    SubdivideVertex *v=m_subdivideLevel[0]->m_vertex[m_subdivideLevel[0]->m_edge[-theFace->m_edge[1]]->m_end];
                    glVertex3f(v->m_position.x,v->m_position.y,v->m_position.z);
                }
                if(theFace->m_edge[2]>0)
                {
                    SubdivideVertex *v=m_subdivideLevel[0]->m_vertex[m_subdivideLevel[0]->m_edge[theFace->m_edge[2]]->m_start];
                    glVertex3f(v->m_position.x,v->m_position.y,v->m_position.z);
                }
                else
                {
                    SubdivideVertex *v=m_subdivideLevel[0]->m_vertex[m_subdivideLevel[0]->m_edge[-theFace->m_edge[2]]->m_end];
                    glVertex3f(v->m_position.x,v->m_position.y,v->m_position.z);
                }
                if(theFace->m_edge[3]>0)
                {
                    SubdivideVertex *v=m_subdivideLevel[0]->m_vertex[m_subdivideLevel[0]->m_edge[theFace->m_edge[3]]->m_start];
                    glVertex3f(v->m_position.x,v->m_position.y,v->m_position.z);
                }
                else
                {
                    SubdivideVertex *v=m_subdivideLevel[0]->m_vertex[m_subdivideLevel[0]->m_edge[-theFace->m_edge[3]]->m_end];
                    glVertex3f(v->m_position.x,v->m_position.y,v->m_position.z);
                }
            }
            glEnd();
            glDisable(GL_POLYGON_OFFSET_FILL);
            glLineWidth(1);
            glDisable(GL_LIGHTING);
            glColor3ub(0,0,0);
            glLineWidth(1);
            glBegin(GL_LINES);
            for(unsigned int i=1;i<m_subdivideLevel[0]->m_edge.size();++i)
            {
                SubdivideEdge *e=m_subdivideLevel[0]->m_edge[i];
                SubdivideVertex *start=m_subdivideLevel[0]->m_vertex[e->m_start];
                SubdivideVertex *end=m_subdivideLevel[0]->m_vertex[e->m_end];
                glVertex3f(start->m_position.x,start->m_position.y,start->m_position.z);
                glVertex3f(end->m_position.x,end->m_position.y,end->m_position.z);
            }

            glColor3ub(100,0,0);
            for(unsigned int i=1;i<m_edgeArray.size();++i)
            {
                Edge *e=m_edgeArray[i];
                if(e && !e->m_isSelected)
                {
                    Vertex *start=m_vertexArray[e->m_start];
                    Vertex *end=m_vertexArray[e->m_end];
                    glVertex3f(start->m_position.x,start->m_position.y,start->m_position.z);
                    glVertex3f(end->m_position.x,end->m_position.y,end->m_position.z);
                }
            }
            glEnd();
            glEnable(GL_LIGHTING);
        }
        glPopMatrix();*/
    }

void Object::drawFaced()
    {
    /*    glMaterialfv(GL_FRONT, GL_AMBIENT, m_matAmbient);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, m_matDiffuse);
        glMaterialfv(GL_FRONT, GL_SPECULAR, m_matSpecular);
        glMaterialfv(GL_FRONT, GL_SHININESS, m_matShininess);
        glMaterialfv(GL_FRONT, GL_EMISSION, m_matEmission);
        glEnable(GL_LIGHTING);
        if(m_subdivideLevelSize==0)
        {
            for(unsigned int i=1;i<m_faceArray.size();++i)
            {
                if(m_faceArray[i])
                {
                    Face *theFace=m_faceArray[i];
                    glBegin(GL_POLYGON);
                    glNormal3f(theFace->m_normal.x,theFace->m_normal.y,theFace->m_normal.z);
                    for(unsigned int e=0;e<theFace->m_edge.size();++e)
                    {
                        if(theFace->m_edge[e]>0)
                        {
                            Vertex *v=m_vertexArray[m_edgeArray[theFace->m_edge[e]]->m_start];
                            glVertex3f(v->m_position.x,v->m_position.y,v->m_position.z);
                        }
                        else
                        {
                            Vertex *v=m_vertexArray[m_edgeArray[-theFace->m_edge[e]]->m_end];
                            glVertex3f(v->m_position.x,v->m_position.y,v->m_position.z);
                        }
                    }
                    glEnd();
                }
            }
        }
        else
        {
            glBegin(GL_QUADS);
            for(unsigned int i=1;i<m_subdivideLevel[0]->m_face.size();++i)
            {
                SubdivideFace *theFace=m_subdivideLevel[0]->m_face[i];
                glNormal3f(theFace->m_normal.x,theFace->m_normal.y,theFace->m_normal.z);
                if(theFace->m_edge[0]>0)
                {
                    SubdivideVertex *v=m_subdivideLevel[0]->m_vertex[m_subdivideLevel[0]->m_edge[theFace->m_edge[0]]->m_start];
                    glVertex3f(v->m_position.x,v->m_position.y,v->m_position.z);
                }
                else
                {
                    SubdivideVertex *v=m_subdivideLevel[0]->m_vertex[m_subdivideLevel[0]->m_edge[-theFace->m_edge[0]]->m_end];
                    glVertex3f(v->m_position.x,v->m_position.y,v->m_position.z);
                }
                if(theFace->m_edge[1]>0)
                {
                    SubdivideVertex *v=m_subdivideLevel[0]->m_vertex[m_subdivideLevel[0]->m_edge[theFace->m_edge[1]]->m_start];
                    glVertex3f(v->m_position.x,v->m_position.y,v->m_position.z);
                }
                else
                {
                    SubdivideVertex *v=m_subdivideLevel[0]->m_vertex[m_subdivideLevel[0]->m_edge[-theFace->m_edge[1]]->m_end];
                    glVertex3f(v->m_position.x,v->m_position.y,v->m_position.z);
                }
                if(theFace->m_edge[2]>0)
                {
                    SubdivideVertex *v=m_subdivideLevel[0]->m_vertex[m_subdivideLevel[0]->m_edge[theFace->m_edge[2]]->m_start];
                    glVertex3f(v->m_position.x,v->m_position.y,v->m_position.z);
                }
                else
                {
                    SubdivideVertex *v=m_subdivideLevel[0]->m_vertex[m_subdivideLevel[0]->m_edge[-theFace->m_edge[2]]->m_end];
                    glVertex3f(v->m_position.x,v->m_position.y,v->m_position.z);
                }
                if(theFace->m_edge[3]>0)
                {
                    SubdivideVertex *v=m_subdivideLevel[0]->m_vertex[m_subdivideLevel[0]->m_edge[theFace->m_edge[3]]->m_start];
                    glVertex3f(v->m_position.x,v->m_position.y,v->m_position.z);
                }
                else
                {
                    SubdivideVertex *v=m_subdivideLevel[0]->m_vertex[m_subdivideLevel[0]->m_edge[-theFace->m_edge[3]]->m_end];
                    glVertex3f(v->m_position.x,v->m_position.y,v->m_position.z);
                }
            }
            glEnd();
        }*/

    }


void Object::drawNormal()
    {
    /*    glDisable(GL_LIGHTING);
        glBegin(GL_LINES);
        for(unsigned int i=1;i<m_vertexArray.size();++i)
        {
            Vertex *v=m_vertexArray[i];
            Vector normal=v->m_normal*10+v->m_position;
            glVertex3f(v->m_position.x,v->m_position.y,v->m_position.z);
            glVertex3f(normal.x,normal.y,normal.z);
        }
        glEnd();
        glEnable(GL_LIGHTING);*/
    }

void Object::drawSmooth()
    {
    /*    glMaterialfv(GL_FRONT, GL_AMBIENT, m_matAmbient);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, m_matDiffuse);
        glMaterialfv(GL_FRONT, GL_SPECULAR, m_matSpecular);
        glMaterialfv(GL_FRONT, GL_SHININESS, m_matShininess);
        glMaterialfv(GL_FRONT, GL_EMISSION, m_matEmission);
        glEnable(GL_LIGHTING);
        if(m_subdivideLevelSize==0)
        {
            for(unsigned int i=1;i<m_faceArray.size();++i)
            {
                if(m_faceArray[i])
                {
                    Face *theFace=m_faceArray[i];
                    if(theFace->m_isSelected)
                    {
                        GLfloat diffuse[4]={1.0f,0.0f,0.0f,1.0f};
                        glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
                    }
                    else
                    {
                        glMaterialfv(GL_FRONT, GL_DIFFUSE, m_matDiffuse);
                    }
                    glBegin(GL_POLYGON);
                    for(unsigned int e=0;e<theFace->m_edge.size();++e)
                    {
                        if(theFace->m_edge[e]>0)
                        {
                            Vertex *v=m_vertexArray[m_edgeArray[theFace->m_edge[e]]->m_start];
                            glNormal3f(v->m_normal.x,v->m_normal.y,v->m_normal.z);
                            glVertex3f(v->m_position.x,v->m_position.y,v->m_position.z);
                        }
                        else
                        {
                            Vertex *v=m_vertexArray[m_edgeArray[-theFace->m_edge[e]]->m_end];
                            glNormal3f(v->m_normal.x,v->m_normal.y,v->m_normal.z);
                            glVertex3f(v->m_position.x,v->m_position.y,v->m_position.z);
                        }
                    }
                    glEnd();
                }
            }
        }
        else
        {
            glBegin(GL_QUADS);
            for(unsigned int i=1;i<m_subdivideLevel[0]->m_face.size();++i)
            {
                SubdivideFace *theFace=m_subdivideLevel[0]->m_face[i];
                if(theFace->m_edge[0]>0)
                {
                    SubdivideVertex *v=m_subdivideLevel[0]->m_vertex[m_subdivideLevel[0]->m_edge[theFace->m_edge[0]]->m_start];
                    glNormal3f(v->m_normal.x,v->m_normal.y,v->m_normal.z);
                    glVertex3f(v->m_position.x,v->m_position.y,v->m_position.z);
                }
                else
                {
                    SubdivideVertex *v=m_subdivideLevel[0]->m_vertex[m_subdivideLevel[0]->m_edge[-theFace->m_edge[0]]->m_end];
                    glNormal3f(v->m_normal.x,v->m_normal.y,v->m_normal.z);
                    glVertex3f(v->m_position.x,v->m_position.y,v->m_position.z);
                }
                if(theFace->m_edge[1]>0)
                {
                    SubdivideVertex *v=m_subdivideLevel[0]->m_vertex[m_subdivideLevel[0]->m_edge[theFace->m_edge[1]]->m_start];
                    glNormal3f(v->m_normal.x,v->m_normal.y,v->m_normal.z);
                    glVertex3f(v->m_position.x,v->m_position.y,v->m_position.z);
                }
                else
                {
                    SubdivideVertex *v=m_subdivideLevel[0]->m_vertex[m_subdivideLevel[0]->m_edge[-theFace->m_edge[1]]->m_end];
                    glNormal3f(v->m_normal.x,v->m_normal.y,v->m_normal.z);
                    glVertex3f(v->m_position.x,v->m_position.y,v->m_position.z);
                }
                if(theFace->m_edge[2]>0)
                {
                    SubdivideVertex *v=m_subdivideLevel[0]->m_vertex[m_subdivideLevel[0]->m_edge[theFace->m_edge[2]]->m_start];
                    glNormal3f(v->m_normal.x,v->m_normal.y,v->m_normal.z);
                    glVertex3f(v->m_position.x,v->m_position.y,v->m_position.z);
                }
                else
                {
                    SubdivideVertex *v=m_subdivideLevel[0]->m_vertex[m_subdivideLevel[0]->m_edge[-theFace->m_edge[2]]->m_end];
                    glNormal3f(v->m_normal.x,v->m_normal.y,v->m_normal.z);
                    glVertex3f(v->m_position.x,v->m_position.y,v->m_position.z);
                }
                if(theFace->m_edge[3]>0)
                {
                    SubdivideVertex *v=m_subdivideLevel[0]->m_vertex[m_subdivideLevel[0]->m_edge[theFace->m_edge[3]]->m_start];
                    glNormal3f(v->m_normal.x,v->m_normal.y,v->m_normal.z);
                    glVertex3f(v->m_position.x,v->m_position.y,v->m_position.z);
                }
                else
                {
                    SubdivideVertex *v=m_subdivideLevel[0]->m_vertex[m_subdivideLevel[0]->m_edge[-theFace->m_edge[3]]->m_end];
                    glNormal3f(v->m_normal.x,v->m_normal.y,v->m_normal.z);
                    glVertex3f(v->m_position.x,v->m_position.y,v->m_position.z);
                }
            }
            glEnd();
        }*/

    }


void Object::drawWire(GLuint r,GLuint g,GLuint b)
    {
     /*   glLineWidth(1);
        glDisable(GL_LIGHTING);
        glColor3ub((GLubyte)r,(GLubyte)g,(GLubyte)b);
        glLineWidth(1);
        if(m_subdivideLevelSize==0)
        {
            glBegin(GL_LINES);
            for(unsigned int i=1;i<m_edgeArray.size();++i)
            {
                Edge *e=m_edgeArray[i];
                if(e)
                {
                    Vertex *start=m_vertexArray[e->m_start];
                    Vertex *end=m_vertexArray[e->m_end];
                    glVertex3f(start->m_position.x,start->m_position.y,start->m_position.z);
                    glVertex3f(end->m_position.x,end->m_position.y,end->m_position.z);
                }
            }
            glEnd();
        }
        else
        {
            glBegin(GL_LINES);
            for(unsigned int i=1;i<m_subdivideLevel[0]->m_edge.size();++i)
            {
                SubdivideEdge *e=m_subdivideLevel[0]->m_edge[i];
                SubdivideVertex *start=m_subdivideLevel[0]->m_vertex[e->m_start];
                SubdivideVertex *end=m_subdivideLevel[0]->m_vertex[e->m_end];
                glVertex3f(start->m_position.x,start->m_position.y,start->m_position.z);
                glVertex3f(end->m_position.x,end->m_position.y,end->m_position.z);
            }
            glEnd();
        }
        glEnable(GL_LIGHTING);*/
    }


void Object::drawWireframe()
    {
    /*    glMaterialfv(GL_FRONT, GL_AMBIENT, m_matAmbient);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, m_matDiffuse);
        glMaterialfv(GL_FRONT, GL_SPECULAR, m_matSpecular);
        glMaterialfv(GL_FRONT, GL_SHININESS, m_matShininess);
        glMaterialfv(GL_FRONT, GL_EMISSION, m_matEmission);
        glEnable(GL_LIGHTING);
        if(m_subdivideLevelSize==0)
        {
            glEnable(GL_POLYGON_OFFSET_FILL);
            glPolygonOffset(1.0f,1.0f);
            for(unsigned int i=1;i<m_faceArray.size();++i)
            {
                if(m_faceArray[i])
                {
                    Face *theFace=m_faceArray[i];
                    glBegin(GL_POLYGON);
                    for(unsigned int e=0;e<theFace->m_edge.size();++e)
                    {
                        if(theFace->m_edge[e]>0)
                        {
                            Vertex *v=m_vertexArray[m_edgeArray[theFace->m_edge[e]]->m_start];
                            glNormal3f(v->m_normal.x,v->m_normal.y,v->m_normal.z);
                            glVertex3f(v->m_position.x,v->m_position.y,v->m_position.z);
                        }
                        else
                        {
                            Vertex *v=m_vertexArray[m_edgeArray[-theFace->m_edge[e]]->m_end];
                            glNormal3f(v->m_normal.x,v->m_normal.y,v->m_normal.z);
                            glVertex3f(v->m_position.x,v->m_position.y,v->m_position.z);
                        }
                    }
                    glEnd();
                }
            }
            glDisable(GL_POLYGON_OFFSET_FILL);

            glLineWidth(1);
            glDisable(GL_LIGHTING);
            glColor3ub(0,0,0);
            glLineWidth(1);
            glBegin(GL_LINES);
            for(unsigned int i=1;i<m_edgeArray.size();++i)
            {
                Edge *e=m_edgeArray[i];
                if(e)
                {
                    Vertex *start=m_vertexArray[e->m_start];
                    Vertex *end=m_vertexArray[e->m_end];
                    glVertex3f(start->m_position.x,start->m_position.y,start->m_position.z);
                    glVertex3f(end->m_position.x,end->m_position.y,end->m_position.z);
                }
            }
            glEnd();
            glEnable(GL_LIGHTING);
        }
        else
        {
            glEnable(GL_POLYGON_OFFSET_FILL);
            glPolygonOffset(1.0f,1.0f);
            glBegin(GL_QUADS);
            for(unsigned int i=1;i<m_subdivideLevel[0]->m_face.size();++i)
            {
                //glBegin(GL_POLYGON);
                SubdivideFace *theFace=m_subdivideLevel[0]->m_face[i];
            //    glNormal3f(theFace->m_normal.x,theFace->m_normal.y,theFace->m_normal.z);
                if(theFace->m_edge[0]>0)
                {
                    SubdivideVertex *v=m_subdivideLevel[0]->m_vertex[m_subdivideLevel[0]->m_edge[theFace->m_edge[0]]->m_start];
                    glNormal3f(v->m_normal.x,v->m_normal.y,v->m_normal.z);
                    glVertex3f(v->m_position.x,v->m_position.y,v->m_position.z);
                }
                else
                {
                    SubdivideVertex *v=m_subdivideLevel[0]->m_vertex[m_subdivideLevel[0]->m_edge[-theFace->m_edge[0]]->m_end];
                    glNormal3f(v->m_normal.x,v->m_normal.y,v->m_normal.z);
                    glVertex3f(v->m_position.x,v->m_position.y,v->m_position.z);
                }
                if(theFace->m_edge[1]>0)
                {
                    SubdivideVertex *v=m_subdivideLevel[0]->m_vertex[m_subdivideLevel[0]->m_edge[theFace->m_edge[1]]->m_start];
                    glNormal3f(v->m_normal.x,v->m_normal.y,v->m_normal.z);
                    glVertex3f(v->m_position.x,v->m_position.y,v->m_position.z);
                }
                else
                {
                    SubdivideVertex *v=m_subdivideLevel[0]->m_vertex[m_subdivideLevel[0]->m_edge[-theFace->m_edge[1]]->m_end];
                    glNormal3f(v->m_normal.x,v->m_normal.y,v->m_normal.z);
                    glVertex3f(v->m_position.x,v->m_position.y,v->m_position.z);
                }
                if(theFace->m_edge[2]>0)
                {
                    SubdivideVertex *v=m_subdivideLevel[0]->m_vertex[m_subdivideLevel[0]->m_edge[theFace->m_edge[2]]->m_start];
                    glNormal3f(v->m_normal.x,v->m_normal.y,v->m_normal.z);
                    glVertex3f(v->m_position.x,v->m_position.y,v->m_position.z);
                }
                else
                {
                    SubdivideVertex *v=m_subdivideLevel[0]->m_vertex[m_subdivideLevel[0]->m_edge[-theFace->m_edge[2]]->m_end];
                    glNormal3f(v->m_normal.x,v->m_normal.y,v->m_normal.z);
                    glVertex3f(v->m_position.x,v->m_position.y,v->m_position.z);
                }
                if(theFace->m_edge[3]>0)
                {
                    SubdivideVertex *v=m_subdivideLevel[0]->m_vertex[m_subdivideLevel[0]->m_edge[theFace->m_edge[3]]->m_start];
                    glNormal3f(v->m_normal.x,v->m_normal.y,v->m_normal.z);
                    glVertex3f(v->m_position.x,v->m_position.y,v->m_position.z);
                }
                else
                {
                    SubdivideVertex *v=m_subdivideLevel[0]->m_vertex[m_subdivideLevel[0]->m_edge[-theFace->m_edge[3]]->m_end];
                    glNormal3f(v->m_normal.x,v->m_normal.y,v->m_normal.z);
                    glVertex3f(v->m_position.x,v->m_position.y,v->m_position.z);
                }
            //    glEnd();
            }
            glEnd();
            glDisable(GL_POLYGON_OFFSET_FILL);
            glLineWidth(1);
            glDisable(GL_LIGHTING);
            glColor3ub(0,0,0);
            glLineWidth(1);
            glBegin(GL_LINES);
            for(unsigned int i=1;i<m_subdivideLevel[0]->m_edge.size();++i)
            {
                SubdivideEdge *e=m_subdivideLevel[0]->m_edge[i];
                SubdivideVertex *start=m_subdivideLevel[0]->m_vertex[e->m_start];
                SubdivideVertex *end=m_subdivideLevel[0]->m_vertex[e->m_end];
                glVertex3f(start->m_position.x,start->m_position.y,start->m_position.z);
                glVertex3f(end->m_position.x,end->m_position.y,end->m_position.z);
            }
            glEnd();
            glEnable(GL_LIGHTING);
        }
*/
    }


void Object::selectionRenderObject()
    {
   /*     glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glTranslatef(m_position.x,m_position.y,m_position.z);
        glTranslatef(m_center.x,m_center.y,m_center.z);
        glRotatef(m_rotation.w,m_rotation.x,m_rotation.y,m_rotation.z);
        glScalef(m_scale.x,m_scale.y,m_scale.z);
        glTranslatef(-m_center.x,-m_center.y,-m_center.z);

        if(m_subdivideLevelSize==0)
        {
            for(unsigned int i=1;i<m_faceArray.size();++i)
            {
                if(m_faceArray[i])
                {
                    Face *theFace=m_faceArray[i];
                    glBegin(GL_POLYGON);
                    for(unsigned int e=0;e<theFace->m_edge.size();++e)
                    {
                        if(theFace->m_edge[e]>0)
                        {
                            Vertex *v=m_vertexArray[m_edgeArray[theFace->m_edge[e]]->m_start];
                            glVertex3f(v->m_position.x,v->m_position.y,v->m_position.z);
                        }
                        else
                        {
                            Vertex *v=m_vertexArray[m_edgeArray[-theFace->m_edge[e]]->m_end];
                            glVertex3f(v->m_position.x,v->m_position.y,v->m_position.z);
                        }
                    }
                    glEnd();
                }
            }
        }
        else
        {
            glBegin(GL_QUADS);
            for(unsigned int i=1;i<m_subdivideLevel[0]->m_face.size();++i)
            {
                SubdivideFace *theFace=m_subdivideLevel[0]->m_face[i];
                if(theFace->m_edge[0]>0)
                {
                    SubdivideVertex *v=m_subdivideLevel[0]->m_vertex[m_subdivideLevel[0]->m_edge[theFace->m_edge[0]]->m_start];
                    glVertex3f(v->m_position.x,v->m_position.y,v->m_position.z);
                }
                else
                {
                    SubdivideVertex *v=m_subdivideLevel[0]->m_vertex[m_subdivideLevel[0]->m_edge[-theFace->m_edge[0]]->m_end];
                    glVertex3f(v->m_position.x,v->m_position.y,v->m_position.z);
                }
                if(theFace->m_edge[1]>0)
                {
                    SubdivideVertex *v=m_subdivideLevel[0]->m_vertex[m_subdivideLevel[0]->m_edge[theFace->m_edge[1]]->m_start];
                    glVertex3f(v->m_position.x,v->m_position.y,v->m_position.z);
                }
                else
                {
                    SubdivideVertex *v=m_subdivideLevel[0]->m_vertex[m_subdivideLevel[0]->m_edge[-theFace->m_edge[1]]->m_end];
                    glVertex3f(v->m_position.x,v->m_position.y,v->m_position.z);
                }
                if(theFace->m_edge[2]>0)
                {
                    SubdivideVertex *v=m_subdivideLevel[0]->m_vertex[m_subdivideLevel[0]->m_edge[theFace->m_edge[2]]->m_start];
                    glVertex3f(v->m_position.x,v->m_position.y,v->m_position.z);
                }
                else
                {
                    SubdivideVertex *v=m_subdivideLevel[0]->m_vertex[m_subdivideLevel[0]->m_edge[-theFace->m_edge[2]]->m_end];
                    glVertex3f(v->m_position.x,v->m_position.y,v->m_position.z);
                }
                if(theFace->m_edge[3]>0)
                {
                    SubdivideVertex *v=m_subdivideLevel[0]->m_vertex[m_subdivideLevel[0]->m_edge[theFace->m_edge[3]]->m_start];
                    glVertex3f(v->m_position.x,v->m_position.y,v->m_position.z);
                }
                else
                {
                    SubdivideVertex *v=m_subdivideLevel[0]->m_vertex[m_subdivideLevel[0]->m_edge[-theFace->m_edge[3]]->m_end];
                    glVertex3f(v->m_position.x,v->m_position.y,v->m_position.z);
                }
            }
            glEnd();
        }
        glPopMatrix();*/
    }

void Object::drawWireframeFaced()
    {
   /*     glMaterialfv(GL_FRONT, GL_AMBIENT, m_matAmbient);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, m_matDiffuse);
        glMaterialfv(GL_FRONT, GL_SPECULAR, m_matSpecular);
        glMaterialfv(GL_FRONT, GL_SHININESS, m_matShininess);
        glMaterialfv(GL_FRONT, GL_EMISSION, m_matEmission);
        glEnable(GL_LIGHTING);
        if(m_subdivideLevelSize==0)
        {
            glEnable(GL_POLYGON_OFFSET_FILL);
            glPolygonOffset(1.0f,1.0f);
            for(unsigned int i=1;i<m_faceArray.size();++i)
            {
                if(m_faceArray[i])
                {
                    Face *theFace=m_faceArray[i];
                    glBegin(GL_POLYGON);
                    glNormal3f(theFace->m_normal.x,theFace->m_normal.y,theFace->m_normal.z);
                    for(unsigned int e=0;e<theFace->m_edge.size();++e)
                    {
                        if(theFace->m_edge[e]>0)
                        {
                            Vertex *v=m_vertexArray[m_edgeArray[theFace->m_edge[e]]->m_start];
                            glVertex3f(v->m_position.x,v->m_position.y,v->m_position.z);
                        }
                        else
                        {
                            Vertex *v=m_vertexArray[m_edgeArray[-theFace->m_edge[e]]->m_end];
                            glVertex3f(v->m_position.x,v->m_position.y,v->m_position.z);
                        }
                    }
                    glEnd();
                }
            }
            glDisable(GL_POLYGON_OFFSET_FILL);
            glLineWidth(1);
            glDisable(GL_LIGHTING);
            glColor3ub(0,0,0);
            glLineWidth(1);
            glBegin(GL_LINES);
            for(unsigned int i=1;i<m_edgeArray.size();++i)
            {
                Edge *e=m_edgeArray[i];
                if(e)
                {
                    Vertex *start=m_vertexArray[e->m_start];
                    Vertex *end=m_vertexArray[e->m_end];
                    glVertex3f(start->m_position.x,start->m_position.y,start->m_position.z);
                    glVertex3f(end->m_position.x,end->m_position.y,end->m_position.z);
                }
            }
            glEnd();
            glEnable(GL_LIGHTING);
        }
        else
        {
            glEnable(GL_POLYGON_OFFSET_FILL);
            glPolygonOffset(1.0f,1.0f);
            glBegin(GL_QUADS);
            for(unsigned int i=1;i<m_subdivideLevel[0]->m_face.size();++i)
            {
                SubdivideFace *theFace=m_subdivideLevel[0]->m_face[i];
                glNormal3f(theFace->m_normal.x,theFace->m_normal.y,theFace->m_normal.z);
                if(theFace->m_edge[0]>0)
                {
                    SubdivideVertex *v=m_subdivideLevel[0]->m_vertex[m_subdivideLevel[0]->m_edge[theFace->m_edge[0]]->m_start];
                    glVertex3f(v->m_position.x,v->m_position.y,v->m_position.z);
                }
                else
                {
                    SubdivideVertex *v=m_subdivideLevel[0]->m_vertex[m_subdivideLevel[0]->m_edge[-theFace->m_edge[0]]->m_end];
                    glVertex3f(v->m_position.x,v->m_position.y,v->m_position.z);
                }
                if(theFace->m_edge[1]>0)
                {
                    SubdivideVertex *v=m_subdivideLevel[0]->m_vertex[m_subdivideLevel[0]->m_edge[theFace->m_edge[1]]->m_start];
                    glVertex3f(v->m_position.x,v->m_position.y,v->m_position.z);
                }
                else
                {
                    SubdivideVertex *v=m_subdivideLevel[0]->m_vertex[m_subdivideLevel[0]->m_edge[-theFace->m_edge[1]]->m_end];
                    glVertex3f(v->m_position.x,v->m_position.y,v->m_position.z);
                }
                if(theFace->m_edge[2]>0)
                {
                    SubdivideVertex *v=m_subdivideLevel[0]->m_vertex[m_subdivideLevel[0]->m_edge[theFace->m_edge[2]]->m_start];
                    glVertex3f(v->m_position.x,v->m_position.y,v->m_position.z);
                }
                else
                {
                    SubdivideVertex *v=m_subdivideLevel[0]->m_vertex[m_subdivideLevel[0]->m_edge[-theFace->m_edge[2]]->m_end];
                    glVertex3f(v->m_position.x,v->m_position.y,v->m_position.z);
                }
                if(theFace->m_edge[3]>0)
                {
                    SubdivideVertex *v=m_subdivideLevel[0]->m_vertex[m_subdivideLevel[0]->m_edge[theFace->m_edge[3]]->m_start];
                    glVertex3f(v->m_position.x,v->m_position.y,v->m_position.z);
                }
                else
                {
                    SubdivideVertex *v=m_subdivideLevel[0]->m_vertex[m_subdivideLevel[0]->m_edge[-theFace->m_edge[3]]->m_end];
                    glVertex3f(v->m_position.x,v->m_position.y,v->m_position.z);
                }
            }
            glEnd();
            glDisable(GL_POLYGON_OFFSET_FILL);
            glLineWidth(1);
            glDisable(GL_LIGHTING);
            glColor3ub(0,0,0);
            glLineWidth(1);
            glBegin(GL_LINES);
            for(unsigned int i=1;i<m_subdivideLevel[0]->m_edge.size();++i)
            {
                SubdivideEdge *e=m_subdivideLevel[0]->m_edge[i];
                SubdivideVertex *start=m_subdivideLevel[0]->m_vertex[e->m_start];
                SubdivideVertex *end=m_subdivideLevel[0]->m_vertex[e->m_end];
                glVertex3f(start->m_position.x,start->m_position.y,start->m_position.z);
                glVertex3f(end->m_position.x,end->m_position.y,end->m_position.z);
            }
            glEnd();
            glEnable(GL_LIGHTING);
        }*/
}

Object::ObjectInfo Object::getObjectInfo()
    {
        Object::ObjectInfo result;
        result.index=m_index;
        result.centerX=m_center.x;
        result.centerY=m_center.y;
        result.centerZ=m_center.z;

        result.positionX=m_position.x;
        result.positionY=m_position.y;
        result.positionZ=m_position.z;

        result.rotationX=m_rotation.x;
        result.rotationY=m_rotation.y;
        result.rotationZ=m_rotation.z;

        result.scaleX=m_scale.x;
        result.scaleY=m_scale.y;
        result.scaleZ=m_scale.z;

        result.matAmbient[0]=m_matAmbient[0];
        result.matAmbient[1]=m_matAmbient[1];
        result.matAmbient[2]=m_matAmbient[2];
        result.matAmbient[3]=m_matAmbient[3];

        result.matDiffuse[0]=m_matDiffuse[0];
        result.matDiffuse[1]=m_matDiffuse[1];
        result.matDiffuse[2]=m_matDiffuse[2];
        result.matDiffuse[3]=m_matDiffuse[3];

        result.matSpecular[0]=m_matSpecular[0];
        result.matSpecular[1]=m_matSpecular[1];
        result.matSpecular[2]=m_matSpecular[2];
        result.matSpecular[3]=m_matSpecular[3];

        result.matEmission[0]=m_matEmission[0];
        result.matEmission[1]=m_matEmission[1];
        result.matEmission[2]=m_matEmission[2];
        result.matEmission[3]=m_matEmission[3];

        result.matShininess[0]=m_matShininess[0];
        result.matShininess[1]=m_matShininess[1];
        result.matShininess[2]=m_matShininess[2];
        result.matShininess[3]=m_matShininess[3];

        result.vertexCount=m_vertexArray.size();
        result.edgeCount=m_edgeArray.size();
        result.faceCount=m_faceArray.size();
        return result;
}

void Object::buildPSCacheFromEID(std::vector<unsigned int> &edgeToBeSub)
{
    if(m_subdivideLevelSize>0)
    {
        std::vector<unsigned int> vertexToBeSub;
        //vertexToBeSub.clear();
        vertexToBeSub.reserve(1000);
        for(unsigned int i=0;i<edgeToBeSub.size();++i)
        {
            Edge *theEdge=m_edgeArray[edgeToBeSub[i]];
            if(theEdge)
            {
                if(!m_vertexArray[theEdge->m_start]->m_isSub)
                {
                    vertexToBeSub.push_back(theEdge->m_start);
                    m_vertexArray[theEdge->m_start]->m_isSub=true;
                }

                if(!m_vertexArray[theEdge->m_end]->m_isSub)
                {
                    vertexToBeSub.push_back(theEdge->m_end);
                    m_vertexArray[theEdge->m_start]->m_isSub=true;
                }
            }
        }

        for(unsigned int i=0;i<vertexToBeSub.size();++i)
        {
            m_vertexArray[vertexToBeSub[i]]->m_isSub=false;
        }

        buildPSCacheFromVID(vertexToBeSub);
    }
}

void Object::buildPSCacheFromFID(std::vector<unsigned int> &faceToBeSub)
{
    if(m_subdivideLevelSize>0)
    {
        std::vector<unsigned int> vertexToBeSub;
        //vertexToBeSub.clear();
        vertexToBeSub.reserve(1000);
        for(unsigned int i=0;i<faceToBeSub.size();++i)
        {
            Face *theFace=m_faceArray[faceToBeSub[i]];
            for(unsigned int e=0;e<theFace->m_edge.size();++e)
            {
                if(theFace->m_edge[e]>0)
                {
                    Edge *theEdge=m_edgeArray[theFace->m_edge[e]];
                    if(!m_vertexArray[theEdge->m_start]->m_isSub)
                    {
                        vertexToBeSub.push_back(theEdge->m_start);
                        m_vertexArray[theEdge->m_start]->m_isSub=true;
                    }

                    if(!m_vertexArray[theEdge->m_end]->m_isSub)
                    {
                        vertexToBeSub.push_back(theEdge->m_end);
                        m_vertexArray[theEdge->m_end]->m_isSub=true;
                    }
                }
                else
                {
                    Edge *theEdge=m_edgeArray[-theFace->m_edge[e]];
                    if(!m_vertexArray[theEdge->m_start]->m_isSub)
                    {
                        vertexToBeSub.push_back(theEdge->m_start);
                        m_vertexArray[theEdge->m_start]->m_isSub=true;
                    }

                    if(!m_vertexArray[theEdge->m_end]->m_isSub)
                    {
                        vertexToBeSub.push_back(theEdge->m_end);
                        m_vertexArray[theEdge->m_end]->m_isSub=true;
                    }
                }
            }
        }

        for(unsigned int i=0;i<vertexToBeSub.size();++i)
        {
            m_vertexArray[vertexToBeSub[i]]->m_isSub=false;
        }

        buildPSCacheFromVID(vertexToBeSub);
    }
}

void Object::buildPSCacheFromVID(std::vector<unsigned int> &vertexToBeSub)
{
    if(m_subdivideLevelSize>0)
    {
        std::vector<Face *> faceToBeSub;
        //faceToBeSub.clear();
        faceToBeSub.reserve(1000);
        for(unsigned int i=0;i<vertexToBeSub.size();++i)
        {
            Vertex *theVertex=m_vertexArray[vertexToBeSub[i]];
            if(theVertex)
            {
                for(unsigned int e=0;e<theVertex->m_adjacentEdgeList.size();++e)
                {
                    Edge *theEdge=m_edgeArray[theVertex->m_adjacentEdgeList[e]];
                    if(theEdge)
                    {
                        if(theEdge->m_start==theVertex->m_index)
                        {
                            if(theEdge->m_right && !m_faceArray[theEdge->m_right]->m_isSub)
                            {
                                faceToBeSub.push_back(m_faceArray[theEdge->m_right]);
                                m_faceArray[theEdge->m_right]->m_isSub=true;
                            }
                        }
                        else
                        {
                            if(theEdge->m_left && !m_faceArray[theEdge->m_left]->m_isSub)
                            {
                                faceToBeSub.push_back(m_faceArray[theEdge->m_left]);
                                m_faceArray[theEdge->m_left]->m_isSub=true;
                            }
                        }
                    }
                }
            }
        }
        buildPSCache(faceToBeSub);
    }
}

void Object::expandSubFace(std::vector<SubdivideFace*> &originalList,unsigned int level)
{
    //ÕâÀïoriginalListÓ¦¸Ã·ÖÅäºÃ¿Õ¼ä
    //originalListÖÐµÄidÓ¦¸ÃÊÇ´ýÏ¸·ÖµÄ×´Ì¬
    SubdivideLevel *theLevel=m_subdivideLevel[level];
    unsigned int originalCount=originalList.size();
    std::vector<SubdivideVertex *> tempVertexList;
    //tempVertexList.clear();
    tempVertexList.reserve(originalCount*4);
    for(unsigned int i=0;i<originalCount;++i)
    {
        SubdivideFace *theFace=originalList[i];
        for(unsigned int e=0;e<4;++e)
        {
            if(theFace->m_edge[e]>0)
            {
                SubdivideVertex *theVertex=theLevel->m_vertex[theLevel->m_edge[theFace->m_edge[e]]->m_end];
                if(!theVertex->m_isSub)
                {
                    tempVertexList.push_back(theVertex);
                    theVertex->m_isSub=true;
                }
            }
            else
            {
                SubdivideVertex *theVertex=theLevel->m_vertex[theLevel->m_edge[-theFace->m_edge[e]]->m_start];
                if(!theVertex->m_isSub)
                {
                    tempVertexList.push_back(theVertex);
                    theVertex->m_isSub=true;
                }
            }
        }
    }

    for(unsigned int i=0;i<tempVertexList.size();++i)
    {
        SubdivideVertex *theVertex=tempVertexList[i];
        theVertex->m_isSub=false;
        for(unsigned int e=0;e<theVertex->m_adjacentEdgeList.size();++e)
        {
            SubdivideEdge *theEdge=theLevel->m_edge[theVertex->m_adjacentEdgeList[e]];
            if(theEdge->m_start==theVertex->m_index)
            {
                SubdivideFace *theFace=theLevel->m_face[theEdge->m_right];
                if(theFace && !theFace->m_isSub)
                {
                    originalList.push_back(theFace);
                    theFace->m_isSub=true;
                }
            }
            else
            {
                SubdivideFace *theFace=theLevel->m_face[theEdge->m_left];
                if(theFace && !theFace->m_isSub)
                {
                    originalList.push_back(theFace);
                    theFace->m_isSub=true;
                }
            }
        }
    }
}

/*void Object::expandSubFace(std::vector<SubdivideFace*> &originalList,unsigned int level)
{
    //ÕâÀïoriginalListÓ¦¸Ã·ÖÅäºÃ¿Õ¼ä
    //originalListÖÐµÄidÓ¦¸ÃÊÇ´ýÏ¸·ÖµÄ×´Ì¬
    unsigned int originalCount=originalList.size();
    for(unsigned int i=0;i<originalCount;++i)
    {
        SubdivideFace *theFace=originalList[i];
        for(unsigned int e=0;e<4;++e)
        {
            SubdivideVertex *theVertex;
            SubdivideLevel *theLevel=m_subdivideLevel[level];
            if(theFace->m_edge[e]>0)
            {
                if(theLevel->m_edge[theFace->m_edge[e]]->m_left && theLevel->m_face[theLevel->m_edge[theFace->m_edge[e]]->m_left]->m_isSub)
                    continue;
                theVertex=theLevel->m_vertex[theLevel->m_edge[theFace->m_edge[e]]->m_end];
            }
            else
            {
                if(theLevel->m_edge[-theFace->m_edge[e]]->m_right>0 && theLevel->m_face[theLevel->m_edge[-theFace->m_edge[e]]->m_right]->m_isSub)
                    continue;
                theVertex=theLevel->m_vertex[theLevel->m_edge[-theFace->m_edge[e]]->m_start];
            }
            unsigned int adjacentEdgeCount=theVertex->adjacentEdge.size();
            for(unsigned int h=0;h<adjacentEdgeCount;++h)
            {
                if(theLevel->m_edge[theVertex->adjacentEdge[h]]->m_right && !theLevel->m_face[theLevel->m_edge[theVertex->adjacentEdge[h]]->m_right]->m_isSub)
                {
                    SubdivideFace *theSubFace=theLevel->m_face[theLevel->m_edge[theVertex->adjacentEdge[h]]->m_right];
                    originalList.push_back(theSubFace);
                    theSubFace->m_isSub=true;
                }
                if(theLevel->m_edge[theVertex->adjacentEdge[h]]->m_left && !theLevel->m_face[theLevel->m_edge[theVertex->adjacentEdge[h]]->m_left]->m_isSub)
                {
                    SubdivideFace *theSubFace=theLevel->m_face[theLevel->m_edge[theVertex->adjacentEdge[h]]->m_left];
                    originalList.push_back(theSubFace);
                    theSubFace->m_isSub=true;
                }
            }
        }
    }
}*/

void Object::expandSubFace(std::vector<Face*> &originalList)
{
    //ÕâÀïoriginalListÓ¦¸Ã·ÖÅäºÃ¿Õ¼ä
    //originalListÖÐµÄidÓ¦¸ÃÊÇ´ýÏ¸·ÖµÄ×´Ì¬
    unsigned int originalCount=originalList.size();
    std::vector<Vertex *> tempVertexList;
    //tempVertexList.clear();
    tempVertexList.reserve(originalCount*5);
    for(unsigned int i=0;i<originalCount;++i)
    {
        Face *theFace=originalList[i];
        unsigned int edgeCount=theFace->m_edge.size();
        for(unsigned int e=0;e<edgeCount;++e)
        {
            if(theFace->m_edge[e]>0)
            {
                Vertex *theVertex=m_vertexArray[m_edgeArray[theFace->m_edge[e]]->m_end];
                if(!theVertex->m_isSub)
                {
                    tempVertexList.push_back(theVertex);
                }
            }
            else
            {
                Vertex *theVertex=m_vertexArray[m_edgeArray[-theFace->m_edge[e]]->m_start];
                if(!theVertex->m_isSub)
                {
                    tempVertexList.push_back(theVertex);
                }
            }
        }
    }
    for(unsigned int i=0;i<tempVertexList.size();++i)
    {
        Vertex *theVertex=tempVertexList[i];
        theVertex->m_isSub=false;
        for(unsigned int e=0;e<theVertex->m_adjacentEdgeList.size();++e)
        {
            Edge *theEdge=m_edgeArray[theVertex->m_adjacentEdgeList[e]];
            if(theEdge->m_start==theVertex->m_index)
            {
                Face *theFace=m_faceArray[theEdge->m_right];
                if(theFace && !theFace->m_isSub)
                originalList.push_back(theFace);
            }
            else
            {
                Face *theFace=m_faceArray[theEdge->m_left];
                if(theFace && !theFace->m_isSub)
                originalList.push_back(theFace);
            }
        }
    }
}

/*void Object::expandSubFace(std::vector<Face*> &originalList)
{
    //ÕâÀïoriginalListÓ¦¸Ã·ÖÅäºÃ¿Õ¼ä
    //originalListÖÐµÄidÓ¦¸ÃÊÇ´ýÏ¸·ÖµÄ×´Ì¬
    unsigned int originalCount=originalList.size();
    for(unsigned int i=0;i<originalCount;++i)
    {
        Face *theFace=originalList[i];
        unsigned int edgeCount=theFace->m_edge.size();
        for(unsigned int e=0;e<edgeCount;++e)
        {
            Vertex *theVertex;
            if(theFace->m_edge[e]>0)
            {
                if(m_edgeArray[theFace->m_edge[e]]->m_left && m_faceArray[m_edgeArray[theFace->m_edge[e]]->m_left]->m_isSub)
                    continue;
                theVertex=m_vertexArray[m_edgeArray[theFace->m_edge[e]]->m_end];
            }
            else
            {
                if(m_edgeArray[-theFace->m_edge[e]]->m_right>0 && m_faceArray[m_edgeArray[-theFace->m_edge[e]]->m_right]->m_isSub)
                    continue;
                theVertex=m_vertexArray[m_edgeArray[-theFace->m_edge[e]]->m_start];
            }
            unsigned int adjacentEdgeCount=theVertex->adjacentEdge.size();
            for(unsigned int h=0;h<adjacentEdgeCount;++h)
            {
                if(m_edgeArray[theVertex->adjacentEdge[h]]->m_right && !m_faceArray[m_edgeArray[theVertex->adjacentEdge[h]]->m_right]->m_isSub)
                {
                    Face *theSubFace=m_faceArray[m_edgeArray[theVertex->adjacentEdge[h]]->m_right];
                    originalList.push_back(theSubFace);
                    theSubFace->m_isSub=true;
                }
                if(m_edgeArray[theVertex->adjacentEdge[h]]->m_left && !m_faceArray[m_edgeArray[theVertex->adjacentEdge[h]]->m_left]->m_isSub)
                {
                    Face *theSubFace=m_faceArray[m_edgeArray[theVertex->adjacentEdge[h]]->m_left];
                    originalList.push_back(theSubFace);
                    theSubFace->m_isSub=true;
                }
            }
        }
    }
}*/

void Object::buildPSCache(std::vector<Face*> &faceToBeSub)
{
    if(m_subdivideLevelSize>0)
    {
        unsigned int level=m_subdivideLevelSize-1;
        unsigned int faceCount=faceToBeSub.size();
        //m_PSFaceCache.clear();
        m_PSFaceCache.reserve(faceCount);

        //m_PSSubFaceCache[level].clear();
        m_PSSubFaceCache[level].reserve(faceCount*5);
        for(unsigned int i=0;i<faceCount;++i)
        {
            m_PSFaceCache.push_back(faceToBeSub[i]);
            faceToBeSub[i]->m_isSub=true;
            unsigned int subFaceCount=faceToBeSub[i]->m_subdivideFace.size();
            for(unsigned int h=0;h<subFaceCount;++h)
            {
                SubdivideFace *theSubF=m_subdivideLevel[level]->m_face[faceToBeSub[i]->m_subdivideFace[h]];
                m_PSSubFaceCache[level].push_back(theSubF);
                theSubF->m_isSub=true;
            }
        }
        expandSubFace(m_PSSubFaceCache[level],level);
        for(int h=level-1;h>0;--h)
        {
            unsigned int e2=h;
            unsigned int e1=h+1;
            faceCount=m_PSSubFaceCache[e1].size();
            //m_PSSubFaceCache[e2].clear();
            m_PSSubFaceCache[e2].reserve(faceCount*5);
            for(unsigned int i=0;i<faceCount;++i)
            {
                SubdivideFace *theSubFace=m_subdivideLevel[e2]->m_face[(m_PSSubFaceCache[e1])[i]->m_subFace[0]];
                m_PSSubFaceCache[e2].push_back(theSubFace);
                theSubFace->m_isSub=true;
                theSubFace=m_subdivideLevel[e2]->m_face[(m_PSSubFaceCache[e1])[i]->m_subFace[1]];
                m_PSSubFaceCache[e2].push_back(theSubFace);
                theSubFace->m_isSub=true;
                theSubFace=m_subdivideLevel[e2]->m_face[(m_PSSubFaceCache[e1])[i]->m_subFace[2]];
                m_PSSubFaceCache[e2].push_back(theSubFace);
                theSubFace->m_isSub=true;
                theSubFace=m_subdivideLevel[e2]->m_face[(m_PSSubFaceCache[e1])[i]->m_subFace[3]];
                m_PSSubFaceCache[e2].push_back(theSubFace);
                theSubFace->m_isSub=true;
            }
            expandSubFace(m_PSSubFaceCache[e2],e2);
        }
    }
}

void Object::updateAllNormal()
    {
        unsigned int faceCount=m_faceArray.size();
        for(unsigned int i=1;i<faceCount;++i)
        {
            if(m_faceArray[i])
            {
                updateFNormal(m_faceArray[i]);
            }
        }
        unsigned int vertexCount=m_vertexArray.size();
        for(unsigned int i=1;i<vertexCount;++i)
        {
            if(m_vertexArray[i])
            {
                updateVNormal(m_vertexArray[i]);
            }
        }
    }

void Object::partialSubdivision()
{
    if(m_subdivideLevelSize)
    {
        unsigned int level=m_subdivideLevelSize-1;
        ++m_subdivideId;
        unsigned int faceCount=m_PSFaceCache.size();
        for(unsigned int i=0;i<faceCount;++i)
        {
            partialSubdivideFace(m_PSFaceCache[i],level);
        }
        for(unsigned int e=level;e>0;--e)
        {
            faceCount=m_PSSubFaceCache[e].size();
            for(unsigned int i=0;i<faceCount;++i)
            {
                partialSubdivideFace((m_PSSubFaceCache[e])[i],e);
            }
        }
    }
}

void Object::updateSubdivision()
    {
        unsigned int subdivideCount=m_subdivideLevelSize;
        for(unsigned int i=0;i<5;++i)
        {
            unSubdivide();
        }
        for(unsigned int i=0;i<subdivideCount;++i)
        {
            subdivide();
        }
    }


void Object::unSubdivide()
{
    if(m_subdivideLevelSize)
    {
        delete m_subdivideLevel[0];
        for(int i=1;i<m_subdivideLevelSize;i++)
        {
            m_subdivideLevel[i-1]=m_subdivideLevel[i];
        }
        --m_subdivideLevelSize;
        m_subdivideLevel[m_subdivideLevelSize]=NULL;
    }
}

void Object::partialSubdivideFace(SubdivideFace *theFace,int level)
{
    SubdivideLevel *theSubLevel=m_subdivideLevel[level-1];
    SubdivideLevel *theLevel=m_subdivideLevel[level];
    //Ê×ÏÈÒªµÃµ½Õâ¸öÃæµÄ¶Ëµã
    //µÃµ½¶ËµãºÍ±ßµÄÊýÄ¿
    unsigned int edgeCount=4;
    //ÐÂ½¨´æ·Å¶¥µãµÄÊý×é
    SubdivideVertex **theVertexList=new SubdivideVertex*[edgeCount];
    //¼ÆËãÕâ¸öÃæµÄÖÐµã
    //½«Ô­Ê¼µÄÖÐµãÇåÁã
    theSubLevel->m_vertex[theFace->m_center]->m_position.null();
    theSubLevel->m_vertex[theFace->m_center]->m_normal.null();
    for(unsigned int e=0;e<edgeCount;++e)
    {
        if(theFace->m_edge[e]<0)
        {
            theVertexList[e]=theLevel->m_vertex[theLevel->m_edge[-theFace->m_edge[e]]->m_start];
        }
        else
        {
            theVertexList[e]=theLevel->m_vertex[theLevel->m_edge[theFace->m_edge[e]]->m_end];
        }
        theSubLevel->m_vertex[theFace->m_center]->m_position+=theVertexList[e]->m_position;
        theSubLevel->m_vertex[theFace->m_center]->m_normal+=theVertexList[e]->m_normal;
    }
    theSubLevel->m_vertex[theFace->m_center]->m_position/=(float)edgeCount;
    theSubLevel->m_vertex[theFace->m_center]->m_normal/=(float)edgeCount;
    //¼ÆËãÐÂµÄ¶¥µã
    for(unsigned int i=0;i<edgeCount;++i)
    {
        SubdivideVertex *theV=theVertexList[i];
        if(theV->m_subdivideId!=m_subdivideId)
        {
            //Èç¹ûÊÇµÚÒ»´Î¼ÆËãµ½Õâ¸öµã
            //³õÊ¼»¯Ï¸·Ö²½Êý
            theV->m_subdivideId=m_subdivideId;
            theV->m_subdivideStep=0;
            //Éú³ÉÕâ¸öµãµÄÏ¸·Öµã
            //Ê×ÏÈ¼ÆËãÏàÁÚµãµÄ×ø±êºÍEv
            theSubLevel->m_vertex[theV->m_nextLevel]->m_position.null();
            theSubLevel->m_vertex[theV->m_nextLevel]->m_position+=EAdjacentVertex(theV,level);
            if(theV->m_edgeVertex)
            {
                theSubLevel->m_vertex[theV->m_nextLevel]->m_position+=theV->m_position*6;
                theSubLevel->m_vertex[theV->m_nextLevel]->m_position/=8;
            }
            else
            {
                //¼ÓÉÏÕâ¸öÃæµÄÖÐµã
                theSubLevel->m_vertex[theV->m_nextLevel]->m_position+=theSubLevel->m_vertex[theFace->m_center]->m_position;
                theV->m_subdivideStep++;
                unsigned int adjEdgeCount=theV->m_adjacentEdgeList.size();
                for(unsigned int h=0;h<adjEdgeCount;++h)
                {
                    if(theLevel->m_edge[theV->m_adjacentEdgeList[h]]->m_end==theV->m_index)
                    {
                        if(!theLevel->m_face[theLevel->m_edge[theV->m_adjacentEdgeList[h]]->m_left]->m_isSub)
                        {
                            theSubLevel->m_vertex[theV->m_nextLevel]->m_position+=theSubLevel->m_vertex[theLevel->m_face[theLevel->m_edge[theV->m_adjacentEdgeList[h]]->m_left]->m_center]->m_position;
                            theV->m_subdivideStep++;
                        }
                    }
                    else
                    {
                        if(!theLevel->m_face[theLevel->m_edge[theV->m_adjacentEdgeList[h]]->m_right]->m_isSub)
                        {
                            theSubLevel->m_vertex[theV->m_nextLevel]->m_position+=theSubLevel->m_vertex[theLevel->m_face[theLevel->m_edge[theV->m_adjacentEdgeList[h]]->m_right]->m_center]->m_position;
                            theV->m_subdivideStep++;
                        }
                    }
                }
                unsigned int n=theV->m_adjacentEdgeList.size();
                if(n==(unsigned int)(theV->m_subdivideStep))
                {
                    theSubLevel->m_vertex[theV->m_nextLevel]->m_position+=theV->m_position*(float)(n*n-2*n);
                    theSubLevel->m_vertex[theV->m_nextLevel]->m_position/=(float)(n*n);
                }
            }
        }
        else
        {
            if(!theV->m_edgeVertex)
            {
                theSubLevel->m_vertex[theV->m_nextLevel]->m_position+=theSubLevel->m_vertex[theFace->m_center]->m_position;
                theV->m_subdivideStep++;
                unsigned int n=theV->m_adjacentEdgeList.size();
                if(n==(unsigned int)(theV->m_subdivideStep))
                {
                    theSubLevel->m_vertex[theV->m_nextLevel]->m_position+=theV->m_position*(float)(n*n-2*n);
                    theSubLevel->m_vertex[theV->m_nextLevel]->m_position/=(float)(n*n);
                }
            }
        }
    }
    delete theVertexList;
    //Ö®ºó´¦ÀíÃ¿Ò»¸ö±ß
    for(unsigned int i=0;i<edgeCount;++i)
    {
        SubdivideEdge *theEdge;
        if(theFace->m_edge[i]>0)
        {
            theEdge=theLevel->m_edge[theFace->m_edge[i]];
            if(theEdge->m_subdivideId!=m_subdivideId)
            {
                //Èç¹ûÊÇµÚÒ»´Î´¦ÀíÕâ¸ö±ß
                theEdge->m_subdivideId=m_subdivideId;
                theSubLevel->m_vertex[theEdge->m_middle]->m_position.null();
                theSubLevel->m_vertex[theEdge->m_middle]->m_position+=theLevel->m_vertex[theEdge->m_start]->m_position+theLevel->m_vertex[theEdge->m_end]->m_position;
                if(theEdge->m_left!=0 )
                {
                    if(theLevel->m_face[theEdge->m_left]->m_isSub)
                    {
                        //Èç¹ûÕâ¸ö±ßµÄÁ½±ß¶¼ÔÚ¾Ö²¿Ï¸·ÖµÄ·¶Î§Ö®ÄÚ
                        theSubLevel->m_vertex[theEdge->m_middle]->m_position+=theSubLevel->m_vertex[theFace->m_center]->m_position;
                    }
                    else
                    {
                        theSubLevel->m_vertex[theEdge->m_middle]->m_position+=theSubLevel->m_vertex[theLevel->m_face[theEdge->m_left]->m_center]->m_position+theSubLevel->m_vertex[theFace->m_center]->m_position;
                        theSubLevel->m_vertex[theEdge->m_middle]->m_position/=4;
                    }
                }
                else
                {
                    theSubLevel->m_vertex[theEdge->m_middle]->m_position/=2;
                }
            }
            else
            {
                //Èç¹û²»ÊÇµÚÒ»´Î¼ÆËã
                theSubLevel->m_vertex[theEdge->m_middle]->m_position+=theSubLevel->m_vertex[theFace->m_center]->m_position;
                theSubLevel->m_vertex[theEdge->m_middle]->m_position/=4;
            }
        }
        else
        {
            theEdge=theLevel->m_edge[-theFace->m_edge[i]];
            if(theEdge->m_subdivideId!=m_subdivideId)
            {
                //Èç¹ûÊÇµÚÒ»´Î´¦ÀíÕâ¸ö±ß
                theEdge->m_subdivideId=m_subdivideId;
                theSubLevel->m_vertex[theEdge->m_middle]->m_position.null();
                theSubLevel->m_vertex[theEdge->m_middle]->m_position+=theLevel->m_vertex[theEdge->m_start]->m_position+theLevel->m_vertex[theEdge->m_end]->m_position;
                if( theEdge->m_right!=0)
                {
                    if(theLevel->m_face[theEdge->m_right]->m_isSub)
                    {
                        theSubLevel->m_vertex[theEdge->m_middle]->m_position+=theSubLevel->m_vertex[theFace->m_center]->m_position;
                    }
                    else
                    {
                        theSubLevel->m_vertex[theEdge->m_middle]->m_position+=theSubLevel->m_vertex[theLevel->m_face[theEdge->m_right]->m_center]->m_position+theSubLevel->m_vertex[theFace->m_center]->m_position;
                        theSubLevel->m_vertex[theEdge->m_middle]->m_position/=4;
                    }
                }
                else
                {
                    theSubLevel->m_vertex[theEdge->m_middle]->m_position/=2;
                }
            }
            else
            {
                //Èç¹û²»ÊÇµÚÒ»´Î¼ÆËã
                theSubLevel->m_vertex[theEdge->m_middle]->m_position+=theSubLevel->m_vertex[theFace->m_center]->m_position;
                theSubLevel->m_vertex[theEdge->m_middle]->m_position/=4;
            }
        }
    }
}

void Object::buildPSCacheFast(std::vector<Face*> &faceToBeSub)
{
    if(m_subdivideLevelSize>0)
    {
        unsigned int subCount=faceToBeSub.size();
        //m_PSFaceCache.clear();
        m_PSFaceCache.reserve(subCount*2);
        for(unsigned int i=0;i<subCount;++i)
        {
            m_PSFaceCache.push_back(faceToBeSub[i]);
            faceToBeSub[i]->m_isSub=true;
        }
        expandSubFace(faceToBeSub);
        subCount=m_PSFaceCache.size();
        unsigned int level=m_subdivideLevelSize-1;
        //m_PSSubFaceCache[level].clear();
        m_PSSubFaceCache[level].reserve(5*subCount);
        for(unsigned int i=0;i<subCount;++i)
        {
            unsigned int subFaceCount=m_PSFaceCache[i]->m_subdivideFace.size();
            for(unsigned int h=0;h<subFaceCount;++h)
            {
                m_PSSubFaceCache[level].push_back(m_subdivideLevel[level]->m_face[m_PSFaceCache[i]->m_subdivideFace[h]]);
                m_subdivideLevel[level]->m_face[m_PSFaceCache[i]->m_subdivideFace[h]]->m_isSub=true;
            }
        }
        for(unsigned int e=level;e>0;--e)
        {
            unsigned int subCount=m_PSSubFaceCache[e].size();
            unsigned int e2=e-1;
            //m_PSSubFaceCache[e2].clear();
            m_PSSubFaceCache[e2].reserve(subCount*2);
            for(unsigned int i=0;i<subCount;++i)
            {
                m_PSSubFaceCache[e2].push_back(m_subdivideLevel[e2]->m_face[(m_PSSubFaceCache[level])[i]->m_subFace[0]]);
                m_subdivideLevel[e2]->m_face[(m_PSSubFaceCache[level])[i]->m_subFace[0]]->m_isSub=true;
                m_PSSubFaceCache[e2].push_back(m_subdivideLevel[e2]->m_face[(m_PSSubFaceCache[level])[i]->m_subFace[1]]);
                m_subdivideLevel[e2]->m_face[(m_PSSubFaceCache[level])[i]->m_subFace[1]]->m_isSub=true;
                m_PSSubFaceCache[e2].push_back(m_subdivideLevel[e2]->m_face[(m_PSSubFaceCache[level])[i]->m_subFace[2]]);
                m_subdivideLevel[e2]->m_face[(m_PSSubFaceCache[level])[i]->m_subFace[2]]->m_isSub=true;
                m_PSSubFaceCache[e2].push_back(m_subdivideLevel[e2]->m_face[(m_PSSubFaceCache[level])[i]->m_subFace[3]]);
                m_subdivideLevel[e2]->m_face[(m_PSSubFaceCache[level])[i]->m_subFace[3]]->m_isSub=true;
            }
        }
    }
}

void Object::partialSubdivideFace(Face *theFace,int level)
{
    //Ê×ÏÈÒªµÃµ½Õâ¸öÃæµÄ¶Ëµã
    //µÃµ½¶ËµãºÍ±ßµÄÊýÄ¿
    unsigned int edgeCount=theFace->m_edge.size();
    SubdivideLevel *theLevel=m_subdivideLevel[level];
    //ÐÂ½¨´æ·Å¶¥µãµÄÊý×é
    Vertex **theVertexList=new Vertex*[edgeCount];
    theLevel->m_vertex[theFace->m_center]->m_position.null();
    theLevel->m_vertex[theFace->m_center]->m_normal.null();
    for(unsigned int e=0;e<edgeCount;++e)
    {
        if(theFace->m_edge[e]<0)
        {
            theVertexList[e]=m_vertexArray[m_edgeArray[-theFace->m_edge[e]]->m_start];
        }
        else
        {
            theVertexList[e]=m_vertexArray[m_edgeArray[theFace->m_edge[e]]->m_end];
        }
        theLevel->m_vertex[theFace->m_center]->m_position+=theVertexList[e]->m_position;
        theLevel->m_vertex[theFace->m_center]->m_normal+=theVertexList[e]->m_normal;
    }
    //¼ÆËãÕâ¸öÃæµÄÖÐµã
    theLevel->m_vertex[theFace->m_center]->m_position/=(float)edgeCount;
    theLevel->m_vertex[theFace->m_center]->m_normal/=(float)edgeCount;
    //¼ÆËãÐÂµÄ¶¥µã
    for(unsigned int i=0;i<edgeCount;++i)
    {
        Vertex *theV=theVertexList[i];
        if(theV->m_subdivideId!=m_subdivideId)
        {
            //Èç¹ûÊÇµÚÒ»´Î¼ÆËãµ½Õâ¸öµã
            //³õÊ¼»¯Ï¸·Ö²½Êý
            theV->m_subdivideId=m_subdivideId;
            theV->m_subdivideStep=0;
            //Éú³ÉÕâ¸öµãµÄÏ¸·Öµã
            //Ê×ÏÈ¼ÆËãÏàÁÚµãµÄ×ø±êºÍEv
            theLevel->m_vertex[theV->m_nextLevel]->m_position.null();
            theLevel->m_vertex[theV->m_nextLevel]->m_position+=EAdjacentVertex(theV);
            if(theV->m_edgeVertex)
            {
                theLevel->m_vertex[theV->m_nextLevel]->m_position+=theV->m_position*6;
                theLevel->m_vertex[theV->m_nextLevel]->m_position/=8;
            }
            else
            {
                //¼ÓÉÏÕâ¸öÃæµÄÖÐµã
                theLevel->m_vertex[theV->m_nextLevel]->m_position+=theLevel->m_vertex[theFace->m_center]->m_position;
                theV->m_subdivideStep++;
                unsigned int adjEdgeCount=theV->m_adjacentEdgeList.size();
                for(unsigned int h=0;h<adjEdgeCount;++h)
                {
                    if(m_edgeArray[theV->m_adjacentEdgeList[h]]->m_end==theV->m_index)
                    {
                        if(!m_faceArray[m_edgeArray[theV->m_adjacentEdgeList[h]]->m_left]->m_isSub)
                        {
                            theLevel->m_vertex[theV->m_nextLevel]->m_position+=theLevel->m_vertex[m_faceArray[m_edgeArray[theV->m_adjacentEdgeList[h]]->m_left]->m_center]->m_position;
                            ++(theV->m_subdivideStep);
                        }
                    }
                    else
                    {
                        if(!m_faceArray[m_edgeArray[theV->m_adjacentEdgeList[h]]->m_right]->m_isSub)
                        {
                            theLevel->m_vertex[theV->m_nextLevel]->m_position+=theLevel->m_vertex[m_faceArray[m_edgeArray[theV->m_adjacentEdgeList[h]]->m_right]->m_center]->m_position;
                            ++(theV->m_subdivideStep);
                        }
                    }
                }
                unsigned int n=theV->m_adjacentEdgeList.size();
                if(n==(unsigned int)(theV->m_subdivideStep))
                {
                    theLevel->m_vertex[theV->m_nextLevel]->m_position+=theV->m_position*(float)(n*n-2*n);
                    theLevel->m_vertex[theV->m_nextLevel]->m_position/=(float)(n*n);
                }
            }
        }
        else
        {
            if(!theV->m_edgeVertex)
            {
                theLevel->m_vertex[theV->m_nextLevel]->m_position+=theLevel->m_vertex[theFace->m_center]->m_position;
                ++(theV->m_subdivideStep);
                unsigned int n=theV->m_adjacentEdgeList.size();
                if(n==(unsigned int)(theV->m_subdivideStep))
                {
                    theLevel->m_vertex[theV->m_nextLevel]->m_position+=theVertexList[i]->m_position*(float)(n*n-2*n);
                    theLevel->m_vertex[theV->m_nextLevel]->m_position/=(float)(n*n);
                }
            }
        }
    }
    delete theVertexList;
    //Ö®ºó´¦ÀíÃ¿Ò»¸ö±ß
    for(unsigned int i=0;i<edgeCount;++i)
    {
        Edge *theEdge;
        if(theFace->m_edge[i]>0)
        {
            theEdge=m_edgeArray[theFace->m_edge[i]];
            if(theEdge->m_subdivideId!=m_subdivideId)
            {
                //Èç¹ûÊÇµÚÒ»´Î´¦ÀíÕâ¸ö±ß
                theEdge->m_subdivideId=m_subdivideId;
                theLevel->m_vertex[theEdge->m_middle]->m_position.null();
                theLevel->m_vertex[theEdge->m_middle]->m_position+=m_vertexArray[theEdge->m_start]->m_position+m_vertexArray[theEdge->m_end]->m_position;
                if(theEdge->m_left)
                {
                    if(m_faceArray[theEdge->m_left]->m_isSub)
                    {
                        //Èç¹ûÕâ¸ö±ßµÄÁ½±ß¶¼ÔÚ¾Ö²¿Ï¸·ÖµÄ·¶Î§Ö®ÄÚ
                        theLevel->m_vertex[theEdge->m_middle]->m_position+=theLevel->m_vertex[theFace->m_center]->m_position;
                    }
                    else
                    {
                        theLevel->m_vertex[theEdge->m_middle]->m_position+=theLevel->m_vertex[m_faceArray[theEdge->m_left]->m_center]->m_position+theLevel->m_vertex[theFace->m_center]->m_position;
                        theLevel->m_vertex[theEdge->m_middle]->m_position/=4;
                    }
                }
                else
                {
                    theLevel->m_vertex[theEdge->m_middle]->m_position/=2;
                }
            }
            else
            {
                //Èç¹û²»ÊÇµÚÒ»´Î¼ÆËã
                theLevel->m_vertex[theEdge->m_middle]->m_position+=theLevel->m_vertex[theFace->m_center]->m_position;
                theLevel->m_vertex[theEdge->m_middle]->m_position/=4;
            }
        }
        else
        {
            theEdge=m_edgeArray[-theFace->m_edge[i]];
            if(theEdge->m_subdivideId!=m_subdivideId)
            {
                //Èç¹ûÊÇµÚÒ»´Î´¦ÀíÕâ¸ö±ß
                theEdge->m_subdivideId=m_subdivideId;
                theLevel->m_vertex[theEdge->m_middle]->m_position.null();
                theLevel->m_vertex[theEdge->m_middle]->m_position+=m_vertexArray[theEdge->m_start]->m_position+m_vertexArray[theEdge->m_end]->m_position;
                if(theEdge->m_right)
                {
                    if(m_faceArray[theEdge->m_right]->m_isSub)
                    {
                        theLevel->m_vertex[theEdge->m_middle]->m_position+=theLevel->m_vertex[theFace->m_center]->m_position;
                    }
                    else
                    {
                        theLevel->m_vertex[theEdge->m_middle]->m_position+=theLevel->m_vertex[m_faceArray[theEdge->m_right]->m_center]->m_position+theLevel->m_vertex[theFace->m_center]->m_position;
                        theLevel->m_vertex[theEdge->m_middle]->m_position/=4;
                    }
                }
                else
                {
                    theLevel->m_vertex[theEdge->m_middle]->m_position/=2;
                }
            }
            else
            {
                //Èç¹û²»ÊÇµÚÒ»´Î¼ÆËã
                theLevel->m_vertex[theEdge->m_middle]->m_position+=theLevel->m_vertex[theFace->m_center]->m_position;
                theLevel->m_vertex[theEdge->m_middle]->m_position/=4;
            }
        }
    }
}


Object::~Object(void)
{
}
