#include "Scene.h"
#include <math.h>

Scene *Scene::scene = nullptr;


void Scene::initialize()
{
    m_backend->initialize();
    m_isInCache.reserve(1000);
    m_axisCursor = new AxisCursor(m_backend);
    m_axisCursor->initialize();
}

void Scene::changeAxisCursorMode(AxisCursorMode::__Enum newMode)
{
    m_currentACMode=newMode;
    if(m_axisCursor->m_mode!=AxisCursorMode::NoAxis)
    {
        m_axisCursor->m_rotation.null();
        m_axisCursor->m_mode=m_currentACMode;
    }
}

void Scene::clearDualEdge(unsigned int tt,unsigned int vertexID)
    {
        if(tt && vertexID)
        {
            Vertex *vertex=m_objectList[tt]->vertex(vertexID);
            unsigned int adjEdgeCount=vertex->m_adjacentEdgeList.size();
            for(unsigned int e=0;e<adjEdgeCount;++e)
            {
                if(m_objectList[tt]->edge(vertex->m_adjacentEdgeList[e])->m_start==vertex->m_index)
                {
                    if(m_objectList[tt]->edge(vertex->m_adjacentEdgeList[e])->m_left>0 && m_objectList[tt]->face(m_objectList[tt]->edge(vertex->m_adjacentEdgeList[e])->m_left)->m_edge.size()<3)
                    {
                        Face *face=m_objectList[tt]->face(m_objectList[tt]->edge(vertex->m_adjacentEdgeList[e])->m_left);
                        if(face->m_edge[0]>0)
                        {
                            m_objectList[tt]->edgeRightChange(face->m_edge[0],0);
                        }
                        else
                        {
                            m_objectList[tt]->edgeLeftChange(face->m_edge[0],0);
                        }
                        if(face->m_edge[1]>0)
                        {
                            m_objectList[tt]->edgeRightChange(face->m_edge[1],0);
                        }
                        else
                        {
                            m_objectList[tt]->edgeLeftChange(face->m_edge[1],0);
                        }
                        m_objectList[tt]->objectFaceRemove(face->m_index);
                    }
                }
                else
                {
                    if(m_objectList[tt]->edge(vertex->m_adjacentEdgeList[e])->m_right>0 && m_objectList[tt]->face(m_objectList[tt]->edge(vertex->m_adjacentEdgeList[e])->m_right)->m_edge.size()<3)
                    {
                        Face *face=m_objectList[tt]->face(m_objectList[tt]->edge(vertex->m_adjacentEdgeList[e])->m_right);
                        if(face->m_edge[0]>0)
                        {
                            m_objectList[tt]->edgeRightChange(face->m_edge[0],0);
                        }
                        else
                        {
                            m_objectList[tt]->edgeLeftChange(face->m_edge[0],0);
                        }
                        if(face->m_edge[1]>0)
                        {
                            m_objectList[tt]->edgeRightChange(face->m_edge[1],0);
                        }
                        else
                        {
                            m_objectList[tt]->edgeLeftChange(face->m_edge[1],0);
                        }
                        m_objectList[tt]->objectFaceRemove(face->m_index);
                    }
                }
            }
            //µ½ÕâÒ»²½Ó¦¸Ã°ÑË«ÖØ±ß¼ÐµÄÃæ¶¼É¾³ýÁË
            //È»ºó±éÀúËùÓÐµÄ±ß ÕÒµ½Ò»×édual±ßµÄÊ±ºò£¬½øÐÐºÏ²¢
            for(unsigned int e=0;e<vertex->m_adjacentEdgeList.size()-1;++e)
            {
                Edge *currentEdge=m_objectList[tt]->edge(vertex->m_adjacentEdgeList[e]);
                if(currentEdge->m_start==vertex->m_index)
                {
                    for(unsigned int i=e+1;i<vertex->m_adjacentEdgeList.size();++i)
                    {
                        Edge *tempEdge=m_objectList[tt]->edge(vertex->m_adjacentEdgeList[i]);
                        if(tempEdge->m_end==vertex->m_index && currentEdge->m_end==tempEdge->m_start)
                        {
                            if(currentEdge->m_left>0 && currentEdge->m_right==0 && tempEdge->m_left>0 && tempEdge->m_right==0)
                            {
                                Face *left=m_objectList[tt]->face(tempEdge->m_left);
                                unsigned int fEdgeCount=left->m_edge.size();
                                for(unsigned int h=0;h<fEdgeCount;++h)
                                {
                                    if(left->m_edge[h]==-((int)(tempEdge->m_index)))
                                    {
                                        m_objectList[tt]->faceEdgeChange(left->m_index,h,currentEdge->m_index);
                                    }
                                }
                                deleteEdgeH(tt,tempEdge->m_index);
                            }
                            else
                            if(currentEdge->m_right>0 && currentEdge->m_left==0 && tempEdge->m_right>0 && tempEdge->m_left==0)
                            {
                                Face *right=m_objectList[tt]->face(tempEdge->m_right);
                                unsigned int fEdgeCount=right->m_edge.size();
                                for(unsigned int h=0;h<fEdgeCount;++h)
                                {
                                    if(right->m_edge[h]==((int)tempEdge->m_index))
                                    {
                                        m_objectList[tt]->faceEdgeChange(right->m_index,h,-((int)currentEdge->m_index));
                                    }
                                }
                                deleteEdgeH(tt,tempEdge->m_index);
                                tempEdge->m_index;
                            }
                            else
                            {
                                if(tempEdge->m_left==0 && tempEdge->m_right==0)
                                {
                                    deleteEdgeH(tt,tempEdge->m_index);
                                }
                                if(currentEdge->m_left==0 && currentEdge->m_left==0)
                                {
                                    deleteEdgeH(tt,currentEdge->m_index);
                                }
                            }
                        }
                        else
                        if(tempEdge->m_start==vertex->m_index && currentEdge->m_end==tempEdge->m_end)
                        {
                            if(currentEdge->m_left>0 && currentEdge->m_right==0 && tempEdge->m_right>0 && tempEdge->m_left==0)
                            {
                                Face *right=m_objectList[tt]->face(tempEdge->m_right);
                                unsigned int fEdgeCount=right->m_edge.size();
                                for(unsigned int h=0;h<fEdgeCount;++h)
                                {
                                    if(right->m_edge[h]==((int)tempEdge->m_index))
                                    {
                                        m_objectList[tt]->faceEdgeChange(right->m_index,h,-((int)currentEdge->m_index));
                                    }
                                }
                                deleteEdgeH(tt,tempEdge->m_index);
                            }
                            else
                                if(currentEdge->m_right>0 && currentEdge->m_left==0 && tempEdge->m_left>0 && tempEdge->m_right==0)
                            {
                                Face *left=m_objectList[tt]->face(tempEdge->m_left);
                                unsigned int fEdgeCount=left->m_edge.size();
                                for(unsigned int h=0;h<fEdgeCount;++h)
                                {
                                    if(left->m_edge[h]==-((int)tempEdge->m_index))
                                    {
                                        m_objectList[tt]->faceEdgeChange(left->m_index,h,-((int)currentEdge->m_index));
                                    }
                                }
                                deleteEdgeH(tt,tempEdge->m_index);
                            }
                            else
                            {
                                if(tempEdge->m_left==0 && tempEdge->m_right==0)
                                {
                                    deleteEdgeH(tt,tempEdge->m_index);
                                }
                                if(currentEdge->m_left==0 && currentEdge->m_left==0)
                                {
                                    deleteEdgeH(tt,currentEdge->m_index);
                                }
                            }
                        }
                    }
                }
                else
                {
                    for(unsigned int i=e+1;i<vertex->m_adjacentEdgeList.size();++i)
                    {
                        Edge *tempEdge=m_objectList[tt]->edge(vertex->m_adjacentEdgeList[i]);
                        if(tempEdge->m_end==vertex->m_index && currentEdge->m_start==tempEdge->m_start)
                        {
                            if(currentEdge->m_right>0 && currentEdge->m_left==0 && tempEdge->m_left>0 && tempEdge->m_right==0)
                            {
                                Face *left=m_objectList[tt]->face(tempEdge->m_left);
                                unsigned int fEdgeCount=left->m_edge.size();
                                for(unsigned int h=0;h<fEdgeCount;++h)
                                {
                                    if(left->m_edge[h]==-((int)tempEdge->m_index))
                                    {
                                        m_objectList[tt]->faceEdgeChange(left->m_index,h,currentEdge->m_index);
                                    }
                                }
                                deleteEdgeH(tt,tempEdge->m_index);
                            }
                            else
                                if(currentEdge->m_left>0 && currentEdge->m_right==0 && tempEdge->m_right>0 && tempEdge->m_left==0)
                            {
                                Face *right=m_objectList[tt]->face(tempEdge->m_right);
                                unsigned int fEdgeCount=right->m_edge.size();
                                for(unsigned int h=0;h<fEdgeCount;++h)
                                {
                                    if(right->m_edge[h]==(int)tempEdge->m_index)
                                    {
                                        m_objectList[tt]->faceEdgeChange(right->m_index,h,currentEdge->m_index);
                                    }
                                }
                                deleteEdgeH(tt,tempEdge->m_index);
                            }
                            else
                            {
                                if(tempEdge->m_left==0 && tempEdge->m_right==0)
                                {
                                    deleteEdgeH(tt,tempEdge->m_index);
                                }
                                if(currentEdge->m_left==0 && currentEdge->m_left==0)
                                {
                                    deleteEdgeH(tt,currentEdge->m_index);
                                }
                            }

                        }
                        else
                        if(tempEdge->m_start==vertex->m_index && currentEdge->m_start==tempEdge->m_end)
                        {
                            if(currentEdge->m_right>0 && currentEdge->m_left==0 && tempEdge->m_right>0 && tempEdge->m_left==0)
                            {
                                Face *right=m_objectList[tt]->face(tempEdge->m_right);
                                unsigned int fEdgeCount=right->m_edge.size();
                                for(unsigned int h=0;h<fEdgeCount;++h)
                                {
                                    if(right->m_edge[h]==(int)tempEdge->m_index)
                                    {
                                        m_objectList[tt]->faceEdgeChange(right->m_index,h,currentEdge->m_index);
                                    }
                                }
                                deleteEdgeH(tt,tempEdge->m_index);
                            }
                            else
                                if(currentEdge->m_left>0 && currentEdge->m_right==0 && tempEdge->m_left>0 && tempEdge->m_right==0)
                            {
                                Face *left=m_objectList[tt]->face(tempEdge->m_left);
                                unsigned int fEdgeCount=left->m_edge.size();
                                for(unsigned int h=0;h<fEdgeCount;++h)
                                {
                                    if(left->m_edge[h]==-((int)tempEdge->m_index))
                                    {
                                        m_objectList[tt]->faceEdgeChange(left->m_index,h,currentEdge->m_index);
                                    }
                                }
                                deleteEdgeH(tt,tempEdge->m_index);
                            }
                            else
                            {
                                if(tempEdge->m_left==0 && tempEdge->m_right==0)
                                {
                                    deleteEdgeH(tt,tempEdge->m_index);
                                }
                                if(currentEdge->m_left==0 && currentEdge->m_left==0)
                                {
                                    deleteEdgeH(tt,currentEdge->m_index);
                                }
                            }
                        }
                    }
                }
            }
        }
    }


Scene::Scene(OpenGLBackend *backend)
    :m_backend(backend),
      m_mode(SelectionMode::Object),
      m_target(0),
      m_currentACMode(AxisCursorMode::MoveAxis),
      m_isSplitMode(false),
      m_splitVertexID(0)
{

}



void Scene::moveVertex(float x,float y,float z)
{
    //ÕâÀïÓ¦¸Ã¼ÓÉÏÒ»Ð©ºÏ·¨ÐÔµÄÅÐ¶Ï£¬±ÈÈçµ±Ç°µÄÑ¡ÔñÄ£Ê½ÊÇ·ñÊÇ¶¥µãÄ£Ê½¡£
    //Ê×ÏÈ½øÐÐºÏ·¨ÐÔµÄÅÐ¶Ï
    if(m_mode==SelectionMode::Vertex && m_target)
    {
        //±éÀúËùÓÐÑ¡ÔñµÄµã
//        unsigned int selectionSize=m_selection.size();
        for(unsigned int e=0;e<m_selection.size();++e)
        {
            //µÃµ½µ±Ç°µÄÎ»ÖÃ
        //    Vertex *vertex=m_objectList[m_target]->vertex(m_selection[e]);
            //ÔÚµ±Ç°µÄÎ»ÖÃÉÏÔö¼Ó
            m_objectList[m_target]->vertexPositionChangeR(m_selection[e],x,y,z);
        }
    }
}

void Scene::moveEdge(float x,float y,float z)
{
    if(m_mode==SelectionMode::Edge && m_target)
    {

        unsigned int edgeCount=m_selection.size();
        for(unsigned int e=0;e<edgeCount;++e)
        {
            Vertex *vertex=m_objectList[m_target]->vertex(m_objectList[m_target]->edge(m_selection[e])->m_end);
            if(!vertex->m_isIn)
            {
                vertex->m_isIn=true;
                m_isInCache.push_back(vertex);
                m_objectList[m_target]->vertexPositionChangeR(vertex->m_index,x,y,z);
            }
            vertex=m_objectList[m_target]->vertex(m_objectList[m_target]->edge(m_selection[e])->m_start);
            if(!vertex->m_isIn)
            {
                vertex->m_isIn=true;
                m_isInCache.push_back(vertex);
                m_objectList[m_target]->vertexPositionChangeR(vertex->m_index,x,y,z);
            }
        }
        //ÕâÀïÒªÇå¿ÕisInCache
        clearIsInCache();
    }
}

void Scene::moveFace(float x,float y,float z)
{
    //Ê×ÏÈÒªµÃµ½±»ÒÆ¶¯µÄµã¼¯
    if(m_mode==SelectionMode::Face && m_target)
    {

        unsigned int selectionCount=m_selection.size();
        for(unsigned int i=0;i<selectionCount;++i)
        {
            Face *face=m_objectList[m_target]->face(m_selection[i]);
            unsigned int edgeCount=face->m_edge.size();
            for(unsigned int e=0;e<edgeCount;++e)
            {
                if(face->m_edge[e]>0)
                {
                    if(!m_objectList[m_target]->vertex(m_objectList[m_target]->edge(face->m_edge[e])->m_end)->m_isIn)
                    {
                        Vertex *vertex=m_objectList[m_target]->vertex(m_objectList[m_target]->edge(face->m_edge[e])->m_end);
                        vertex->m_isIn=true;
                        m_isInCache.push_back(vertex);
                        m_objectList[m_target]->vertexPositionChangeR(vertex->m_index,x,y,z);
                    }
                }
                else
                {
                    if(!m_objectList[m_target]->vertex(m_objectList[m_target]->edge(-face->m_edge[e])->m_start)->m_isIn)
                    {
                        Vertex *vertex=m_objectList[m_target]->vertex(m_objectList[m_target]->edge(-face->m_edge[e])->m_start);
                        vertex->m_isIn=true;
                        m_isInCache.push_back(vertex);
                        m_objectList[m_target]->vertexPositionChangeR(vertex->m_index,x,y,z);
                    }
                }
            }
        }
        clearIsInCache();

    }
}

//×¢Òâ£¬Çå³þÎÞÓÃµÄ¶Ëµãµ«Ã»ÓÐ±ßµÄ×óÓÒÃæ!!!!!!!!!!!
void Scene::deleteEdgeH(unsigned int t,unsigned int edgeID)
{
    Edge *edge=m_objectList[t]->edge(edgeID);
    Vertex *start=m_objectList[t]->vertex(edge->m_start);
    unsigned int adjEdgeCount=start->m_adjacentEdgeList.size();
    for(unsigned int e=0;e<adjEdgeCount;++e)
    {
        if(start->m_adjacentEdgeList[e]==(int)edge->m_index)
        {
            m_objectList[t]->vertexAdjacentRemove(start->m_index,e);
            break;
        }
    }
    if(start->m_adjacentEdgeList.empty())
    {
        m_objectList[t]->objectVertexRemove(start->m_index);
    }
    Vertex *end=m_objectList[t]->vertex(edge->m_end);
    adjEdgeCount=end->m_adjacentEdgeList.size();
    for(unsigned int e=0;e<adjEdgeCount;++e)
    {
        if(end->m_adjacentEdgeList[e]==(int)edge->m_index)
        {
            m_objectList[t]->vertexAdjacentRemove(end->m_index,e);
            break;
        }
    }
    if(end->m_adjacentEdgeList.empty())
    {
        m_objectList[t]->objectVertexRemove(end->m_index);
    }
    m_objectList[t]->objectEdgeRemove(edgeID);
}

void Scene::deleteVertex()
{
    if(m_mode==SelectionMode::Vertex && m_target)
    {
        unsigned int vertexCount=m_selection.size();
        for(unsigned int i=0;i<vertexCount;++i)
        {
            //Ê×ÏÈµÃµ½ÁÚ½ÓµÄÃæ
            Vertex *currentVertex=m_objectList[m_target]->vertex(m_selection[i]);
            unsigned int edgeCount=currentVertex->m_adjacentEdgeList.size();
            for(unsigned int e=0;e<edgeCount;++e)
            {
                if(m_objectList[m_target]->edge(currentVertex->m_adjacentEdgeList[e])->m_end==m_selection[i])
                {
                    Face *currentFace=m_objectList[m_target]->face(m_objectList[m_target]->edge(currentVertex->m_adjacentEdgeList[e])->m_right);
                    if(currentFace==NULL)
                        continue;
                    unsigned int edgeSize=currentFace->m_edge.size();
                    for(unsigned int h=0;h<edgeSize;++h)
                    {
                        if(currentFace->m_edge[h]>0)
                        {
                            m_objectList[m_target]->edgeRightChange(currentFace->m_edge[h],0);
                        }
                        else
                        {
                            m_objectList[m_target]->edgeLeftChange(-currentFace->m_edge[h],0);
                        }
                    }
                    m_objectList[m_target]->objectFaceRemove(currentFace->m_index);
                }
                else
                if(m_objectList[m_target]->edge(currentVertex->m_adjacentEdgeList[e])->m_start==m_selection[i])
                {
                    Face *currentFace=m_objectList[m_target]->face(m_objectList[m_target]->edge(currentVertex->m_adjacentEdgeList[e])->m_left);
                    if(currentFace==NULL)
                        continue;
                    unsigned int edgeSize=currentFace->m_edge.size();
                    for(unsigned int h=0;h<edgeSize;++h)
                    {
                        if(currentFace->m_edge[h]>0)
                        {
                            m_objectList[m_target]->edgeRightChange(currentFace->m_edge[h],0);
                        }
                        else
                        {
                            m_objectList[m_target]->edgeLeftChange(-currentFace->m_edge[h],0);
                        }
                    }
                    m_objectList[m_target]->objectFaceRemove(currentFace->m_index);
                }
            }
            for(int e=edgeCount-1;e>-1;--e)
            {
                Edge *edge=m_objectList[m_target]->edge(currentVertex->m_adjacentEdgeList[e]);
                deleteEdgeH(m_target,edge->m_index);
            }
        }
        clearSelection();
        m_objectList[m_target]->clearPSCache();
        updateAxisCursor();
    }
}

void Scene::deleteFace()
{
    if(m_mode==SelectionMode::Face && m_target)
    {
        //Ê×ÏÈµÃµ½ÒªÉ¾³ýµÄÃæÊý
        unsigned int faceCount=m_selection.size();
        for(unsigned int i=0;i<faceCount;++i)
        {
            Face *face=m_objectList[m_target]->face(m_selection[i]);
            unsigned int edgeCount=face->m_edge.size();
            for(unsigned int e=0;e<edgeCount;++e)
            {
                if(face->m_edge[e]>0)
                {
                    m_objectList[m_target]->edgeRightChange(face->m_edge[e],0);
                    if(m_objectList[m_target]->edge(face->m_edge[e])->m_left==0)
                    {
                        deleteEdgeH(m_target,face->m_edge[e]);
                    }
                }
                else
                {
                    m_objectList[m_target]->edgeLeftChange(-face->m_edge[e],0);
                    if(m_objectList[m_target]->edge(-face->m_edge[e])->m_right==0)
                    {
                        deleteEdgeH(m_target,-face->m_edge[e]);
                    }
                }
            }
            m_objectList[m_target]->objectFaceRemove(face->m_index);
        }
    }
}

void Scene::extrudeEdge(float x,float y,float z)
{
    if(m_mode==SelectionMode::Edge && m_target)
    {
        std::vector<unsigned int> newSelection;
        unsigned int edgeCount=m_selection.size();
        newSelection.reserve(edgeCount);
        for(unsigned int e=0;e<edgeCount;++e)
        {
            Edge *edge=m_objectList[m_target]->edge(m_selection[e]);
            if(edge->m_left==0 || edge->m_right==0)
            {
                Vertex *end=m_objectList[m_target]->vertex(edge->m_end);
                if(!end->m_isIn)
                {
                    end->m_clone=m_objectList[m_target]->addVertex(end->m_position.x+x,end->m_position.y+y,end->m_position.z+z);
                    m_objectList[m_target]->vertex(end->m_clone)->m_clone=m_objectList[m_target]->addEdge(end->m_index,end->m_clone);
                    end->m_isIn=true;
                    m_isInCache.push_back(end);
                }
                Vertex *start=m_objectList[m_target]->vertex(edge->m_start);
                if(!start->m_isIn)
                {
                    start->m_clone=m_objectList[m_target]->addVertex(start->m_position.x+x,start->m_position.y+y,start->m_position.z+z);
                    m_objectList[m_target]->vertex(start->m_clone)->m_clone=m_objectList[m_target]->addEdge(start->m_index,start->m_clone);
                    start->m_isIn=true;
                    m_isInCache.push_back(start);
                }
                //Éú³ÉÃæ
                unsigned int tempEdge[4]={0};
                if(edge->m_right==0)
                {
                    tempEdge[0]=edge->m_index;
                    tempEdge[1]=m_objectList[m_target]->vertex(end->m_clone)->m_clone;
                    tempEdge[2]=m_objectList[m_target]->addEdge(end->m_clone,start->m_clone);
                    newSelection.push_back(tempEdge[2]);
                    tempEdge[3]=m_objectList[m_target]->vertex(start->m_clone)->m_clone;
                }
                else
                {
                    tempEdge[0]=edge->m_index;
                    tempEdge[1]=m_objectList[m_target]->vertex(start->m_clone)->m_clone;
                    tempEdge[2]=m_objectList[m_target]->addEdge(end->m_clone,start->m_clone);
                    newSelection.push_back(tempEdge[2]);
                    tempEdge[3]=m_objectList[m_target]->vertex(end->m_clone)->m_clone;                }
                m_objectList[m_target]->addFace(tempEdge,4);
            }
        }
        clearIsInCache();

        clearSelection();
        for(unsigned int i=0;i<newSelection.size();++i)
        {
            selectionPush(m_objectList[m_target]->edge(newSelection[i]));
        }
    }
}

unsigned int Scene::insertVertex(int edgeID,float pos)
{
    Edge *edge=m_objectList[m_target]->edge(edgeID);
    Vertex *end=m_objectList[m_target]->vertex(edge->m_end);
    Vertex *start=m_objectList[m_target]->vertex(edge->m_start);
    unsigned int newVertex=m_objectList[m_target]->addVertex(start->m_position.x+(end->m_position.x-start->m_position.x)*pos,start->m_position.y+(end->m_position.y-start->m_position.y)*pos,start->m_position.z+(end->m_position.z-start->m_position.z)*pos);
    unsigned int right=edge->m_right;
    unsigned int left=edge->m_left;
    unsigned int edgeCount=end->m_adjacentEdgeList.size();
    unsigned int removedEdge=edge->m_index;
    deleteEdgeH(m_target,removedEdge);
    unsigned int edgeS=m_objectList[m_target]->addEdge(start->m_index,newVertex);
    unsigned int edgeE=m_objectList[m_target]->addEdge(newVertex,end->m_index);

    Face *face=m_objectList[m_target]->face(right);
    if(face)
    {
        edgeCount=face->m_edge.size();
        unsigned int e;
        for(e=0;e<edgeCount;++e)
        {
            if(face->m_edge[e]==(int)removedEdge)
            {
                m_objectList[m_target]->faceEdgeChange(face->m_index,e,edgeS);
                break;
            }
        }
        m_objectList[m_target]->faceEdgeInsert(face->m_index,e+1,edgeE);
        m_objectList[m_target]->edgeRightChange(edgeE,right);
        m_objectList[m_target]->edgeRightChange(edgeS,right);
    }

    face=m_objectList[m_target]->face(left);
    if(face)
    {
        edgeCount=face->m_edge.size();
        unsigned int e;
        for(e=0;e<edgeCount;++e)
        {
            if(face->m_edge[e]==-((int)removedEdge))
            {
                m_objectList[m_target]->faceEdgeChange(face->m_index,e,-((int)edgeE));
                break;
            }
        }
        m_objectList[m_target]->faceEdgeInsert(face->m_index,e+1,-((int)edgeS));
        m_objectList[m_target]->edgeLeftChange(edgeE,left);
        m_objectList[m_target]->edgeLeftChange(edgeS,left);
    }

    return newVertex;
}

void Scene::split(int startV,int endV)
{
    //printf("begin\n");
    Vertex *start=m_objectList[m_target]->vertex(startV);
    Vertex *end=m_objectList[m_target]->vertex(endV);
    unsigned int edgeCount=start->m_adjacentEdgeList.size();
    unsigned int e;
    Face *face=NULL;
    for(e=0;e<edgeCount;++e)
    {
        if(m_objectList[m_target]->edge(start->m_adjacentEdgeList[e])->m_start==end->m_index || m_objectList[m_target]->edge(start->m_adjacentEdgeList[e])->m_end==end->m_index)
        {
            //±éÀústartµã£¬¿´ÊÇ²»ÊÇºÍendµãÖ»ÓÐÒ»¸ö±ßÏàÁ¬
            return;
        }
        if(m_objectList[m_target]->edge(start->m_adjacentEdgeList[e])->m_left && !m_objectList[m_target]->face(m_objectList[m_target]->edge(start->m_adjacentEdgeList[e])->m_left)->m_isIn)
        {
            face=m_objectList[m_target]->face(m_objectList[m_target]->edge(start->m_adjacentEdgeList[e])->m_left);
            face->m_isIn=true;
            m_isInCache.push_back(face);
        }
    //    else
        if(m_objectList[m_target]->edge(start->m_adjacentEdgeList[e])->m_right && !m_objectList[m_target]->face(m_objectList[m_target]->edge(start->m_adjacentEdgeList[e])->m_right)->m_isIn)
        {
            face=m_objectList[m_target]->face(m_objectList[m_target]->edge(start->m_adjacentEdgeList[e])->m_right);
            face->m_isIn=true;
            m_isInCache.push_back(face);
        }
    }
    edgeCount=end->m_adjacentEdgeList.size();
    //Face *face=NULL;
    for(e=0;e<edgeCount;++e)
    {
        if(m_objectList[m_target]->edge(end->m_adjacentEdgeList[e])->m_left && m_objectList[m_target]->face(m_objectList[m_target]->edge(end->m_adjacentEdgeList[e])->m_left)->m_isIn)
        {
            face=m_objectList[m_target]->face(m_objectList[m_target]->edge(end->m_adjacentEdgeList[e])->m_left);
            break;
        }
    //    else
        if(m_objectList[m_target]->edge(end->m_adjacentEdgeList[e])->m_right && m_objectList[m_target]->face(m_objectList[m_target]->edge(end->m_adjacentEdgeList[e])->m_right)->m_isIn)
        {
            face=m_objectList[m_target]->face(m_objectList[m_target]->edge(end->m_adjacentEdgeList[e])->m_right);
            break;
        }
    }
    clearIsInCache();
    if(e==edgeCount)
    {
        //ËµÃ÷²»¹²Ãæ
        return;
    }
    //printf("begin2\n");
    //µ½ÕâÒ»²½¿ÉÒÔËµÃ÷ÕâÁ½¸öµã²»¹²±ß¶øÇÒ¹²ÏíÍ¬Ò»¸öÃæ
    //ÐÂ½¨Ò»¸ö±ß
    unsigned int newEdge=m_objectList[m_target]->addEdge(start->m_index,end->m_index);
    //printf("begin2.5\n");
    edgeCount=face->m_edge.size();
    unsigned int edgeS[2]={0};
    unsigned int edgeE[2]={0};
    for(unsigned int i=0;i<edgeCount;++i)
    {
        if(face->m_edge[i]>0)
        {
            if(m_objectList[m_target]->edge(face->m_edge[i])->m_end==start->m_index)
            {
                edgeS[0]=i;
                edgeS[1]=(i+1)%edgeCount;
            }
            if(m_objectList[m_target]->edge(face->m_edge[i])->m_end==end->m_index)
            {
                edgeE[0]=i;
                edgeE[1]=(i+1)%edgeCount;
            }
        }
        else
        {
            if(m_objectList[m_target]->edge(-face->m_edge[i])->m_start==start->m_index)
            {
                edgeS[0]=i;
                edgeS[1]=(i+1)%edgeCount;
            }
            if(m_objectList[m_target]->edge(-face->m_edge[i])->m_start==end->m_index)
            {
                edgeE[0]=i;
                edgeE[1]=(i+1)%edgeCount;
            }
        }
    }

    //printf("[%d]",face->m_index);
    //µÃµ½startËùÔÚµÄµÚÒ»¸ö±ß
    //Éú³ÉÁ½¸öÐÂµÄÃæ
    unsigned int *tempFace=new unsigned int[face->m_edge.size()*2];
    unsigned int b=edgeS[1];
    unsigned int count=0;
    while(b!=edgeE[1])
    {
        if(face->m_edge[b]>0)
        {
            m_objectList[m_target]->edgeRightChange(face->m_edge[b],0);
            tempFace[count]=face->m_edge[b];
            ++count;
        }
        else
        {
            m_objectList[m_target]->edgeLeftChange(-face->m_edge[b],0);
            tempFace[count]=-face->m_edge[b];
            ++count;
        }
        ++b;
        b=b%face->m_edge.size();
    }
    tempFace[count]=newEdge;
    //printf("count:%d,size:%d\n",count+1,face->m_edge.size()*2);
    m_objectList[m_target]->addFace(tempFace,count+1);
    delete tempFace;
    tempFace=new unsigned int[face->m_edge.size()*2];
    //printf("begin3\n");
    b=edgeE[1];
    count=0;
    while(b!=edgeS[1])
    {
        if(face->m_edge[b]>0)
        {
            m_objectList[m_target]->edgeRightChange(face->m_edge[b],0);
            tempFace[count]=face->m_edge[b];
            ++count;
        }
        else
        {
            m_objectList[m_target]->edgeLeftChange(-face->m_edge[b],0);
            tempFace[count]=-face->m_edge[b];
            ++count;
        }
        ++b;
        b=b%face->m_edge.size();
    }
    tempFace[count]=newEdge;
//    for(unsigned int i=0;i<count+1;i++)
//    {
        //printf("{%d}\n",tempFace[i]);
    //}
    m_objectList[m_target]->addFace(tempFace,count+1);
    //printf("begin4\n");
    //printf("[%d]",face->m_index);

    unsigned int fi=face->m_index;
    face=NULL;
    m_objectList[m_target]->objectFaceRemove(fi);
    delete tempFace;
}

void Scene::removeEdge()
{
    if(m_mode==SelectionMode::Edge && m_target){
    //±éÀúËùÓÐµÄ±ß
    unsigned int removedEdgeCount=m_selection.size();
    for(unsigned int i=0;i<removedEdgeCount;++i)
    {
        Edge *edge=m_objectList[m_target]->edge(m_selection[i]);
        if(edge==NULL)
        {
            continue;
        }
        //ÕâÀïÅÐ¶ÏÊÇ±ßÔµµÄ±ß»¹ÊÇÄÚ²¿µÄ±ß
        if(edge->m_left==0)
        {
            //±£Ö¤Ã¿¸ö±ß±ØÓÐÁÚ½ÓµÄÃæ
            Face *face=m_objectList[m_target]->face(edge->m_right);
            unsigned int adjEdgeCount=face->m_edge.size();
            for(unsigned int e=0;e<adjEdgeCount;++e)
            {
                if(face->m_edge[e]>0)
                {
                    m_objectList[m_target]->edgeRightChange(face->m_edge[e],0);
                    if(!m_objectList[m_target]->edge(face->m_edge[e])->m_left)
                    {
                        deleteEdgeH(m_target,face->m_edge[e]);
                    }
                }
                else
                {
                    m_objectList[m_target]->edgeLeftChange(-face->m_edge[e],0);
                    if(!m_objectList[m_target]->edge(-face->m_edge[e])->m_right)
                    {
                        deleteEdgeH(m_target,-face->m_edge[e]);
                    }
                }
            }
            m_objectList[m_target]->objectFaceRemove(face->m_index);
        }
        else
        if(edge->m_right==0)
        {
            Face *face=m_objectList[m_target]->face(edge->m_left);
            unsigned int adjEdgeCount=face->m_edge.size();
            for(unsigned int e=0;e<adjEdgeCount;++e)
            {
                if(face->m_edge[e]>0)
                {
                    m_objectList[m_target]->edgeRightChange(face->m_edge[e],0);
                    if(!m_objectList[m_target]->edge(face->m_edge[e])->m_left)
                    {
                        deleteEdgeH(m_target,face->m_edge[e]);
                    }
                }
                else
                {
                    m_objectList[m_target]->edgeLeftChange(-face->m_edge[e],0);
                    if(!m_objectList[m_target]->edge(-face->m_edge[e])->m_right)
                    {
                        deleteEdgeH(m_target,-face->m_edge[e]);
                    }
                }
            }
            m_objectList[m_target]->objectFaceRemove(face->m_index);
        }
        else
        {
            Face *left=m_objectList[m_target]->face(edge->m_left);
            Face *right=m_objectList[m_target]->face(edge->m_right);
            //Ê×ÏÈÔÚÓÒ²àµÄÃæÀïÃæÕÒµ½ÒªÉ¾³ýµÄÕâ¸ö±ß
            unsigned int adjEdgeCount=right->m_edge.size();
            unsigned int e=0;
            for(e=0;e<adjEdgeCount;++e)
            {
                if(right->m_edge[e]>0)
                {
                    if(edge->m_index==(unsigned int)right->m_edge[e])
                        break;
                }
                else
                {
                    if(edge->m_index==(unsigned int)(-right->m_edge[e]))
                        break;
                }
            }
            Vertex *start=m_objectList[m_target]->vertex(edge->m_start);
            Vertex *end=m_objectList[m_target]->vertex(edge->m_end);
            deleteEdgeH(m_target,edge->m_index);
            while(start->m_adjacentEdgeList.size()==1)
            {
                unsigned int next=0;
                if(start->m_index==m_objectList[m_target]->edge(start->m_adjacentEdgeList[0])->m_end)
                {
                    next=m_objectList[m_target]->edge(start->m_adjacentEdgeList[0])->m_start;
                }
                else
                {
                    next=m_objectList[m_target]->edge(start->m_adjacentEdgeList[0])->m_end;
                }
                deleteEdgeH(m_target,start->m_adjacentEdgeList[0]);
                start=m_objectList[m_target]->vertex(next);
            }

            while(end->m_adjacentEdgeList.size()==1)
            {
                unsigned int next=0;
                if(end->m_index==m_objectList[m_target]->edge(end->m_adjacentEdgeList[0])->m_end)
                {
                    next=m_objectList[m_target]->edge(end->m_adjacentEdgeList[0])->m_start;
                }
                else
                {
                    next=m_objectList[m_target]->edge(end->m_adjacentEdgeList[0])->m_end;
                }
                deleteEdgeH(m_target,end->m_adjacentEdgeList[0]);
                end=m_objectList[m_target]->vertex(next);
            }
            //¿ªÊ¼Éú³ÉÐÂµÄÃæÁË;
            unsigned int sEdgeR=0;
            unsigned int eEdgeR=0;
            unsigned int edgeCount=right->m_edge.size();
            for(unsigned int e=0;e<edgeCount;++e)
            {
                if(right->m_edge[e]>0)
                {
                    if(m_objectList[m_target]->edge(right->m_edge[e]))
                    {
                        if(m_objectList[m_target]->edge(right->m_edge[e])->m_start==end->m_index)
                        {
                            sEdgeR=e;
                        }
                        if(m_objectList[m_target]->edge(right->m_edge[e])->m_end==start->m_index)
                        {
                            eEdgeR=e;
                        }
                    }
                }
                else
                {
                    if(m_objectList[m_target]->edge(-right->m_edge[e]))
                    {
                        if(m_objectList[m_target]->edge(-right->m_edge[e])->m_end==end->m_index)
                        {
                            sEdgeR=e;
                        }
                        if(m_objectList[m_target]->edge(-right->m_edge[e])->m_start==start->m_index)
                        {
                            eEdgeR=e;
                        }
                    }
                }
            }

            unsigned int sEdgeL=0;
            unsigned int eEdgeL=0;
            edgeCount=left->m_edge.size();
            for(unsigned int e=0;e<edgeCount;++e)
            {
                if(left->m_edge[e]>0)
                {
                    if(m_objectList[m_target]->edge(left->m_edge[e]))
                    {
                        if(m_objectList[m_target]->edge(left->m_edge[e])->m_start==start->m_index)
                        {
                            sEdgeL=e;
                        }
                        if(m_objectList[m_target]->edge(left->m_edge[e])->m_end==end->m_index)
                        {
                            eEdgeL=e;
                        }
                    }
                }
                else
                {
                    if(m_objectList[m_target]->edge(-left->m_edge[e]))
                    {
                        if(m_objectList[m_target]->edge(-left->m_edge[e])->m_end==start->m_index)
                        {
                            sEdgeL=e;
                        }
                        if(m_objectList[m_target]->edge(-left->m_edge[e])->m_start==end->m_index)
                        {
                            eEdgeL=e;
                        }
                    }
                }
            }
            //ÕýÊ½Éú³ÉÁ½¸öÐÂµÄÃæ
            unsigned int b=sEdgeR;
            unsigned int count=0;
            unsigned int *tempEdge=new unsigned int[right->m_edge.size()+left->m_edge.size()];
            while(b!=eEdgeR)
            {
                tempEdge[count]=right->m_edge[b]>0?right->m_edge[b]:-right->m_edge[b];
                ++count;
                ++b;
                b=b%right->m_edge.size();
            }
            tempEdge[count]=right->m_edge[eEdgeR]>0?right->m_edge[eEdgeR]:-right->m_edge[eEdgeR];
            ++count;
            b=sEdgeL;
            while(b!=eEdgeL)
            {
                tempEdge[count]=left->m_edge[b]>0?left->m_edge[b]:-left->m_edge[b];
                ++count;
                ++b;
                b=b%left->m_edge.size();
            }
            //É¾³ýÔ­À´µÄÁ½¸öÃæ
            tempEdge[count]=left->m_edge[eEdgeL]>0?left->m_edge[eEdgeL]:-left->m_edge[eEdgeL];
            ++count;
            m_objectList[m_target]->addFace(tempEdge,count);
            m_objectList[m_target]->objectFaceRemove(left->m_index);
            m_objectList[m_target]->objectFaceRemove(right->m_index);
            delete tempEdge;
        }
    }}
}

void Scene::clearIsInCache()
{
    unsigned int cacheSize=m_isInCache.size();
    for(unsigned int i=0;i<cacheSize;++i)
    {
        if(m_isInCache[i])
        {
            m_isInCache[i]->m_isIn=false;
        }
    }
    m_isInCache.clear();
}

void Scene::targetWeldVertex(unsigned int vertexA,unsigned int vertexB)
{
    unsigned int mode=0;
    Vertex *original=m_objectList[m_target]->vertex(vertexA);
    Vertex *target=m_objectList[m_target]->vertex(vertexB);
    unsigned int edgeCount=original->m_adjacentEdgeList.size();
    std::vector<Edge *> OAdjEdge;
    OAdjEdge.reserve(10);
    std::vector<Vertex *> OEnd;
    OEnd.reserve(10);
    //Ê×ÏÈÒª±éÀúµÚÒ»¸öµãµÄÁÚ½Ó±ß£¬Òª¼ì²âÕâÁ½¸öµãµÄÁÚ½Ó±ß£¬ÅÐ¶ÏÊÇ·ñÊÇ±ßÔµ±ß£¬ÊÇ·ñÓÐ¹²µãµÈ
    unsigned int e=0;
    bool isEdge=false;
    for(e=0;e<edgeCount;++e)
    {
        Edge *edge=m_objectList[m_target]->edge(original->m_adjacentEdgeList[e]);
        OAdjEdge.push_back(edge);
        if(!edge->m_left || !edge->m_right)
        {
            isEdge=true;
        }
        if(edge->m_left)
        {
            m_objectList[m_target]->face(edge->m_left)->m_isIn=true;
            m_isInCache.push_back(m_objectList[m_target]->face(edge->m_left));
        }
        if(edge->m_right)
        {
            m_objectList[m_target]->face(edge->m_right)->m_isIn=true;
            m_isInCache.push_back(m_objectList[m_target]->face(edge->m_right));
        }
        if(edge->m_start==original->m_index)
        {
            if(edge->m_end==target->m_index)
            {
                //ÕâËµÃ÷º¸½ÓµÄÁ½¸öµãÔÚÍ¬Ò»¸ö±ß
                mode=1;
                break;
            }
            Vertex *vertex=m_objectList[m_target]->vertex(edge->m_end);
            OEnd.push_back(vertex);
            vertex->m_isIn=true;
            m_isInCache.push_back(vertex);
        }
        else
        {
            if(edge->m_start==target->m_index)
            {
                //ËµÃ÷º¸½ÓµÄÁ½¸öµãÔÚÍ¬Ò»¸ö±ßÉÏ
                mode=1;
                break;
            }
            Vertex *vertex=m_objectList[m_target]->vertex(edge->m_start);
            OEnd.push_back(vertex);
            vertex->m_isIn=true;
            m_isInCache.push_back(vertex);
        }
    }

    if(m_mode==1)
    {
        clearIsInCache();
        Edge *edge=m_objectList[m_target]->edge(original->m_adjacentEdgeList[e]);
        Vertex *start=NULL;
        Vertex *end=NULL;
        unsigned int eIndex=edge->m_index;
        unsigned int right=edge->m_right;
        unsigned int left=edge->m_left;
        if(edge->m_start==vertexB)
        {
            start=m_objectList[m_target]->vertex(edge->m_start);
            end=m_objectList[m_target]->vertex(edge->m_end);
        }
        else
        {
            start=m_objectList[m_target]->vertex(edge->m_end);
            end=m_objectList[m_target]->vertex(edge->m_start);
        }
        deleteEdgeH(m_target,edge->m_index);
        unsigned int edgeCount=end->m_adjacentEdgeList.size();
        for(unsigned int e=0;e<edgeCount;++e)
        {
            m_objectList[m_target]->vertexAdjacentPush(start->m_index,end->m_adjacentEdgeList[e]);
            //¸üÐÂÕâÐ©±ßµÄ¶Ëµã
            if(m_objectList[m_target]->edge(end->m_adjacentEdgeList[e])->m_end==end->m_index)
            {
                m_objectList[m_target]->edgeEndChange(end->m_adjacentEdgeList[e],start->m_index);
            }
            else
            {
                m_objectList[m_target]->edgeStartChange(end->m_adjacentEdgeList[e],start->m_index);
            }
        }
        m_objectList[m_target]->objectVertexRemove(end->m_index);
        if(left>0)
        {
            Face  *face=m_objectList[m_target]->face(left);
            unsigned int edgeCount=face->m_edge.size();
            for(unsigned int i=0;i<edgeCount;++i)
            {
                if(face->m_edge[i]==-((int)eIndex))
                {
                    m_objectList[m_target]->faceEdgeRemove(face->m_index,i);
                    break;
                }

            }
        }

        if(right>0)
        {

            Face  *face=m_objectList[m_target]->face(right);
            unsigned int edgeCount=face->m_edge.size();
            for(unsigned int i=0;i<edgeCount;++i)
            {
                if(face->m_edge[i]==(int)eIndex)
                {
                    m_objectList[m_target]->faceEdgeRemove(face->m_index,i);
                    break;
                }
            }
        }
        clearDualEdge(m_target,target->m_index);
        return;
    }

    //ÏÖÔÚÊ×ÏÈ¼ìÑéµÚÒ»¸öµãÊÇ·ñÊÇ±ßÔµÉÏµÄ
    if(isEdge)
    {
        isEdge=false;
        unsigned int edgeCount=target->m_adjacentEdgeList.size();
        Edge *edgeS[2]={NULL};
        Edge *edgeE[2]={NULL};
        unsigned int adjEdgeNum=0;
        for(unsigned int e=0;e<edgeCount;++e)
        {
            if(!m_objectList[m_target]->edge(target->m_adjacentEdgeList[e])->m_left || !m_objectList[m_target]->edge(target->m_adjacentEdgeList[e])->m_right)
            {
                if(m_objectList[m_target]->edge(target->m_adjacentEdgeList[e])->m_left)
                {
                    if(m_objectList[m_target]->face(m_objectList[m_target]->edge(target->m_adjacentEdgeList[e])->m_left)->m_isIn)
                    {
                        //ËµÃ÷¹²Ãæ
                        return;
                    }
                }
                if(m_objectList[m_target]->edge(target->m_adjacentEdgeList[e])->m_right)
                {
                    if(m_objectList[m_target]->face(m_objectList[m_target]->edge(target->m_adjacentEdgeList[e])->m_right)->m_isIn)
                    {
                        //ËµÃ÷¹²Ãæ
                        return;
                    }
                }
                //Èç¹ûÊÇ±ßÔµµÄ±ß
                isEdge=true;
                if(m_objectList[m_target]->edge(target->m_adjacentEdgeList[e])->m_end==target->m_index)
                {
                    //ÅÐ¶ÏÊÇ²»ÊÇÒ»¼¶ÁÚ½Ó
                    if(m_objectList[m_target]->vertex(m_objectList[m_target]->edge(target->m_adjacentEdgeList[e])->m_start)->m_isIn)
                    {
                        //¼ÇÂ¼ÁÚ½Ó±ß
                        edgeE[adjEdgeNum]=m_objectList[m_target]->edge(target->m_adjacentEdgeList[e]);
                        unsigned int OEndCount=OEnd.size();
                        for(unsigned int h=0;h<OEndCount;++h)
                        {
                            if(OEnd[h]->m_index==m_objectList[m_target]->edge(target->m_adjacentEdgeList[e])->m_start)
                            {
                                edgeS[adjEdgeNum]=OAdjEdge[h];
                                break;
                            }
                        }
                        ++adjEdgeNum;
                        if(adjEdgeNum==2)
                        {
                            break;
                        }
                    }
                }
                else
                {
                    if(m_objectList[m_target]->vertex(m_objectList[m_target]->edge(target->m_adjacentEdgeList[e])->m_end)->m_isIn)
                    {
                        edgeE[adjEdgeNum]=m_objectList[m_target]->edge(target->m_adjacentEdgeList[e]);
                        unsigned int OEndCount=OEnd.size();
                        for(unsigned int h=0;h<OEndCount;++h)
                        {
                            if(OEnd[h]->m_index==m_objectList[m_target]->edge(target->m_adjacentEdgeList[e])->m_end)
                            {
                                edgeS[adjEdgeNum]=OAdjEdge[h];
                                break;
                            }
                        }
                        ++adjEdgeNum;
                        if(adjEdgeNum==2)
                        {
                            break;
                        }
                    }
                }
            }
        }
        clearIsInCache();
        if(isEdge)
        {
            if(adjEdgeNum==0)
            {
                //Ö±½Óº¸½Ó
                unsigned int edgeCount=original->m_adjacentEdgeList.size();
                for(unsigned int h=0;h<edgeCount;++h)
                {
                    Edge *edge=m_objectList[m_target]->edge(original->m_adjacentEdgeList[h]);
                    m_objectList[m_target]->vertexAdjacentPush(target->m_index,edge->m_index);
                    if(edge->m_start==original->m_index)
                    {
                        m_objectList[m_target]->edgeStartChange(edge->m_index,target->m_index);
                    }
                    else
                    {
                        m_objectList[m_target]->edgeEndChange(edge->m_index,target->m_index);
                    }
                }
                m_objectList[m_target]->objectVertexRemove(original->m_index);
            }
            else
            if(adjEdgeNum>0)
            {
                unsigned int wMode[2]={0};
                for(unsigned int w=0;w<adjEdgeNum;++w)
                {
                    if(edgeS[w]->m_end==edgeE[w]->m_start)
                    {
                        if(edgeS[w]->m_right && edgeE[w]->m_right)
                        {
                            wMode[w]=1;
                        }
                        else
                        if(edgeS[w]->m_left && edgeE[w]->m_left)
                        {
                            wMode[w]=2;
                        }
                        else
                        {

                            return;
                        }
                    }
                    else
                    if(edgeS[w]->m_end==edgeE[w]->m_end)
                    {
                        if(edgeS[w]->m_right && edgeE[w]->m_left)
                        {
                            wMode[w]=3;
                        }
                        else
                        if(edgeS[w]->m_left && edgeE[w]->m_right)
                        {
                            wMode[w]=4;
                        }
                        else
                        {
                            return;
                        }
                    }
                    else
                    if(edgeS[w]->m_start==edgeE[w]->m_start)
                    {
                        if(edgeS[w]->m_left && edgeE[w]->m_right)
                        {
                            wMode[w]=5;
                        }
                        else
                        if(edgeS[w]->m_right && edgeE[w]->m_left)
                        {
                            wMode[w]=6;
                        }
                        else
                        {
                            return;
                        }
                    }
                    else
                    if(edgeS[w]->m_start==edgeE[w]->m_end)
                    {
                        if(edgeS[w]->m_left && edgeE[w]->m_left)
                        {
                            wMode[w]=7;
                        }
                        else
                        if(edgeS[w]->m_right && edgeE[w]->m_right)
                        {
                            wMode[w]=8;
                        }
                        else
                        {
                            return;
                        }
                    }
                }
                unsigned int originalIndex=original->m_index;
                for(unsigned int w=0;w<adjEdgeNum;w++)
                {
                    if(wMode[w]==1)
                    {
                        Face *face=m_objectList[m_target]->face(edgeS[w]->m_right);
                        unsigned int edgeCount=face->m_edge.size();
                        for(unsigned int h=0;h<edgeCount;++h)
                        {
                            if(face->m_edge[h]==(int)(edgeS[w]->m_index))
                            {
                                m_objectList[m_target]->faceEdgeChange(face->m_index,h,-(int)(edgeE[w]->m_index));
                                m_objectList[m_target]->edgeLeftChange(edgeE[w]->m_index,face->m_index);
                                deleteEdgeH(m_target,edgeS[w]->m_index);
                                break;
                            }
                        }
                    }
                    else
                    if(wMode[w]==2)
                    {
                        Face *face=m_objectList[m_target]->face(edgeS[w]->m_left);
                        unsigned int edgeCount=face->m_edge.size();
                        for(unsigned int h=0;h<edgeCount;++h)
                        {
                            if(face->m_edge[h]==-((int)edgeS[w]->m_index))
                            {
                                m_objectList[m_target]->faceEdgeChange(face->m_index,h,edgeE[w]->m_index);
                                m_objectList[m_target]->edgeRightChange(edgeE[w]->m_index,face->m_index);
                                deleteEdgeH(m_target,edgeS[w]->m_index);
                                break;
                            }
                        }
                    }
                    else
                    if(wMode[w]==3)
                    {
                        Face *face=m_objectList[m_target]->face(edgeS[w]->m_right);
                        unsigned int edgeCount=face->m_edge.size();
                        for(unsigned int h=0;h<edgeCount;++h)
                        {
                            if(face->m_edge[h]==(int)edgeS[w]->m_index)
                            {
                                m_objectList[m_target]->faceEdgeChange(face->m_index,h,edgeE[w]->m_index);
                                m_objectList[m_target]->edgeRightChange(edgeE[w]->m_index,face->m_index);
                                deleteEdgeH(m_target,edgeS[w]->m_index);
                                break;
                            }
                        }
                    }
                    else
                    if(wMode[w]==4)
                    {
                        Face *face=m_objectList[m_target]->face(edgeS[w]->m_left);
                        unsigned int edgeCount=face->m_edge.size();
                        for(unsigned int h=0;h<edgeCount;++h)
                        {
                            if(face->m_edge[h]==-((int)edgeS[w]->m_index))
                            {
                                m_objectList[m_target]->faceEdgeChange(face->m_index,h,-((int)edgeE[w]->m_index));
                                m_objectList[m_target]->edgeLeftChange(edgeE[w]->m_index,face->m_index);
                                deleteEdgeH(m_target,edgeS[w]->m_index);
                                break;
                            }
                        }
                    }
                    else
                    if(wMode[w]==5)
                    {
                        Face *face=m_objectList[m_target]->face(edgeS[w]->m_left);
                        unsigned int edgeCount=face->m_edge.size();
                        for(unsigned int h=0;h<edgeCount;++h)
                        {
                            if(face->m_edge[h]==-((int)edgeS[w]->m_index))
                            {
                                m_objectList[m_target]->faceEdgeChange(face->m_index,h,-(int)(edgeE[w]->m_index));
                                m_objectList[m_target]->edgeLeftChange(edgeE[w]->m_index,face->m_index);
                                deleteEdgeH(m_target,edgeS[w]->m_index);
                                break;
                            }
                        }
                    }
                    else
                    if(wMode[w]==6)
                    {
                        Face *face=m_objectList[m_target]->face(edgeS[w]->m_right);
                        unsigned int edgeCount=face->m_edge.size();
                        for(unsigned int h=0;h<edgeCount;++h)
                        {
                            if(face->m_edge[h]==(int)(edgeS[w]->m_index))
                            {
                                m_objectList[m_target]->faceEdgeChange(face->m_index,h,edgeE[w]->m_index);
                                m_objectList[m_target]->edgeRightChange(edgeE[w]->m_index,face->m_index);
                                deleteEdgeH(m_target,edgeS[w]->m_index);
                                break;
                            }
                        }
                    }
                    else
                    if(wMode[w]==7)
                    {
                        Face *face=m_objectList[m_target]->face(edgeS[w]->m_left);
                        unsigned int edgeCount=face->m_edge.size();
                        for(unsigned int h=0;h<edgeCount;++h)
                        {
                            if(face->m_edge[h]==-((int)edgeS[w]->m_index))
                            {
                                m_objectList[m_target]->faceEdgeChange(face->m_index,h,edgeE[w]->m_index);
                                m_objectList[m_target]->edgeRightChange(edgeE[w]->m_index,face->m_index);
                                deleteEdgeH(m_target,edgeS[w]->m_index);
                                break;
                            }
                        }
                    }
                    else
                    if(wMode[w]==8)
                    {
                        Face *face=m_objectList[m_target]->face(edgeS[w]->m_right);
                        unsigned int edgeCount=face->m_edge.size();
                        for(unsigned int h=0;h<edgeCount;++h)
                        {
                            if(face->m_edge[h]==(int)edgeS[w]->m_index)
                            {
                                m_objectList[m_target]->faceEdgeChange(face->m_index,h,-((int)edgeE[w]->m_index));
                                m_objectList[m_target]->edgeLeftChange(edgeE[w]->m_index,face->m_index);
                                deleteEdgeH(m_target,edgeS[w]->m_index);
                                break;
                            }
                        }
                    }
                }
                if(m_objectList[m_target]->vertex(originalIndex))
                {
                    original=m_objectList[m_target]->vertex(originalIndex);
                    unsigned int edgeCount=original->m_adjacentEdgeList.size();
                    for(unsigned int h=0;h<edgeCount;++h)
                    {
                        m_objectList[m_target]->vertexAdjacentPush(target->m_index,original->m_adjacentEdgeList[h]);
                        if(m_objectList[m_target]->edge(original->m_adjacentEdgeList[h])->m_end==original->m_index)
                        {
                            m_objectList[m_target]->edgeEndChange(original->m_adjacentEdgeList[h],target->m_index);
                        }
                        else
                        {
                            m_objectList[m_target]->edgeStartChange(original->m_adjacentEdgeList[h],target->m_index);
                        }
                    }
                    m_objectList[m_target]->objectVertexRemove(original->m_index);
                }
            }
        }
        else
        {
            return;
        }
    }
    else
    {
        return;
    }
}

void Scene::extrudeFaceGroup(float x,float y,float z)
{
    if(m_mode==SelectionMode::Face && m_target)
    {
        //Ê×ÏÈÒªÍ³¼Æ×é
        std::vector<unsigned int> newFaceSelected;
        unsigned int faceCount=m_selection.size();
        newFaceSelected.reserve(faceCount);
        for(unsigned int i=0;i<faceCount;++i)
        {

            //ÕâÀïÊ×ÏÈÒªµÃµ½Ò»¸öÃæµÄÁ¬Í¨ÇøÓò
            if(!m_objectList[m_target]->face(m_selection[i]) || m_objectList[m_target]->face(m_selection[i])->m_isIn)
            {
                continue;
            }
            else
            {
                std::vector<Face*> group;
                group.reserve(100);
                std::deque<Face*> toBeHandled;
                Face *face=m_objectList[m_target]->face(m_selection[i]);
                toBeHandled.push_back(face);
                while(!toBeHandled.empty())
                {
                    if(!toBeHandled[0]->m_isIn)
                    {
                        toBeHandled[0]->m_isIn=true;
                        m_isInCache.push_back(toBeHandled[0]);
                        group.push_back(toBeHandled[0]);
                        unsigned int edgeCount=toBeHandled[0]->m_edge.size();
                        for(unsigned int h=0;h<edgeCount;++h)
                        {
                            if(toBeHandled[0]->m_edge[h]>0)
                            {
                                Face *left=m_objectList[m_target]->face(m_objectList[m_target]->edge(toBeHandled[0]->m_edge[h])->m_left);
                                if(left && left->m_isSelected)
                                {
                                    toBeHandled.push_back(left);
                                }
                            }
                            else
                            {
                                Face *right=m_objectList[m_target]->face(m_objectList[m_target]->edge(-toBeHandled[0]->m_edge[h])->m_right);
                                if(right && right->m_isSelected)
                                {
                                    toBeHandled.push_back(right);
                                }
                            }
                        }
                    }
                    toBeHandled.pop_front();
                }
                //ÕâÀï¾ÍÒÑ¾­µÃµ½Ò»¸ögroupÁË
                //Ê×ÏÈ¾ÍÊÇ½«group¸´ÖÆ³ÉÒ»¸öÐÂµÄ
                unsigned int groupSize=group.size();
                std::vector<int> eEdgeList;
                eEdgeList.reserve(groupSize*5);
                for(unsigned int e=0;e<groupSize;++e)
                {
                    Face *face=group[e];
                    unsigned int edgeCount=face->m_edge.size();
                    unsigned int *tempEdge=new unsigned int[edgeCount];
                    for(unsigned int h=0;h<edgeCount;++h)
                    {
                        Edge *edge;
                        if(face->m_edge[h]>0)
                        {
                            edge=m_objectList[m_target]->edge(face->m_edge[h]);
                            if(!edge->m_isIn)
                            {
                                Vertex *start=m_objectList[m_target]->vertex(edge->m_start);
                                Vertex *end=m_objectList[m_target]->vertex(edge->m_end);
                                if(!start->m_isIn)
                                {
                                    start->m_clone=m_objectList[m_target]->addVertex(start->m_position.x+x,start->m_position.y+y,start->m_position.z+z);
                                    start->m_isIn=true;
                                    m_isInCache.push_back(start);
                                }
                                if(!end->m_isIn)
                                {
                                    end->m_clone=m_objectList[m_target]->addVertex(end->m_position.x+x,end->m_position.y+y,end->m_position.z+z);
                                    end->m_isIn=true;
                                    m_isInCache.push_back(end);
                                }
                                edge->m_clone=m_objectList[m_target]->addEdge(start->m_clone,end->m_clone);
                                edge->m_isIn=true;
                                m_isInCache.push_back(edge);
                                if(edge->m_left==0 ||(edge->m_left>0 && !m_objectList[m_target]->face(edge->m_left)->m_isIn))
                                {
                                    //ËµÃ÷ÊÇ±ßÉÏµÄ
                                    eEdgeList.push_back(edge->m_index);
                                }
                            }
                            tempEdge[h]=edge->m_clone;
                        }
                        else
                        {
                            edge=m_objectList[m_target]->edge(-face->m_edge[h]);
                            if(!edge->m_isIn)
                            {
                                Vertex *start=m_objectList[m_target]->vertex(edge->m_start);
                                Vertex *end=m_objectList[m_target]->vertex(edge->m_end);
                                if(!start->m_isIn)
                                {
                                    start->m_clone=m_objectList[m_target]->addVertex(start->m_position.x+x,start->m_position.y+y,start->m_position.z+z);
                                    start->m_isIn=true;
                                    m_isInCache.push_back(start);
                                }
                                if(!end->m_isIn)
                                {
                                    end->m_clone=m_objectList[m_target]->addVertex(end->m_position.x+x,end->m_position.y+y,end->m_position.z+z);
                                    end->m_isIn=true;
                                    m_isInCache.push_back(end);
                                }
                                edge->m_clone=m_objectList[m_target]->addEdge(start->m_clone,end->m_clone);
                                edge->m_isIn=true;
                                m_isInCache.push_back(edge);
                                if(edge->m_right==0 ||(edge->m_right>0 && !m_objectList[m_target]->face(edge->m_right)->m_isIn))
                                {
                                    //ËµÃ÷ÊÇ±ßÉÏµÄ
                                    eEdgeList.push_back(-((int)edge->m_index));
                                }
                            }
                            tempEdge[h]=edge->m_clone;
                        }
                    }
                    //m_objectList[m_target]->addFace(tempEdge,edgeCount);
                    newFaceSelected.push_back(m_objectList[m_target]->addFace(tempEdge,edgeCount));
                    delete tempEdge;
                }

                //Éú³É²àÃæµÄÃæ
                unsigned int eEdgeListSize=eEdgeList.size();
                for(unsigned int e=0;e<eEdgeListSize;++e)
                {
                    if(eEdgeList[e]>0)
                    {
                        Edge *edge=m_objectList[m_target]->edge(eEdgeList[e]);
                        Vertex *start=m_objectList[m_target]->vertex(edge->m_start);
                        Vertex *end=m_objectList[m_target]->vertex(edge->m_end);
                        unsigned int tempEdge[4]={0};
                        if(!m_objectList[m_target]->vertex(start->m_clone)->m_isIn)
                        {
                            m_objectList[m_target]->vertex(start->m_clone)->m_clone=m_objectList[m_target]->addEdge(start->m_index,start->m_clone);
                            m_objectList[m_target]->vertex(start->m_clone)->m_isIn=true;
                            m_isInCache.push_back(m_objectList[m_target]->vertex(start->m_clone));
                        }
                        tempEdge[3]=m_objectList[m_target]->vertex(start->m_clone)->m_clone;
                        if(!m_objectList[m_target]->vertex(end->m_clone)->m_isIn)
                        {
                            m_objectList[m_target]->vertex(end->m_clone)->m_clone=m_objectList[m_target]->addEdge(end->m_index,end->m_clone);
                            m_objectList[m_target]->vertex(end->m_clone)->m_isIn=true;
                            m_isInCache.push_back(m_objectList[m_target]->vertex(end->m_clone));
                        }
                        tempEdge[1]=m_objectList[m_target]->vertex(end->m_clone)->m_clone;
                        tempEdge[0]=edge->m_index;
                        tempEdge[2]=edge->m_clone;;
                        m_objectList[m_target]->addFace(tempEdge,4);
                    }
                    else
                    {
                        Edge *edge=m_objectList[m_target]->edge(-eEdgeList[e]);
                        Vertex *start=m_objectList[m_target]->vertex(edge->m_start);
                        Vertex *end=m_objectList[m_target]->vertex(edge->m_end);
                        unsigned int tempEdge[4]={0};
                        if(!m_objectList[m_target]->vertex(start->m_clone)->m_isIn)
                        {
                            m_objectList[m_target]->vertex(start->m_clone)->m_clone=m_objectList[m_target]->addEdge(start->m_index,start->m_clone);
                            m_objectList[m_target]->vertex(start->m_clone)->m_isIn=true;
                            m_isInCache.push_back(m_objectList[m_target]->vertex(start->m_clone));
                        }
                        tempEdge[1]=m_objectList[m_target]->vertex(start->m_clone)->m_clone;
                        if(!m_objectList[m_target]->vertex(end->m_clone)->m_isIn)
                        {
                            m_objectList[m_target]->vertex(end->m_clone)->m_clone=m_objectList[m_target]->addEdge(end->m_index,end->m_clone);
                            m_objectList[m_target]->vertex(end->m_clone)->m_isIn=true;
                            m_isInCache.push_back(m_objectList[m_target]->vertex(end->m_clone));
                        }
                        tempEdge[3]=m_objectList[m_target]->vertex(end->m_clone)->m_clone;
                        tempEdge[0]=edge->m_index;
                        tempEdge[2]=edge->m_clone;;
                        m_objectList[m_target]->addFace(tempEdge,4);
                    }
                }
                clearIsInCache();
                //É¾³ýÔ­À´µÄÃæ
                for(unsigned int e=0;e<groupSize;++e)
                {
                    Face *face=group[e];
                    unsigned int edgeCount=face->m_edge.size();
                    for(unsigned int h=0;h<edgeCount;++h)
                    {
                        if(face->m_edge[h]>0)
                        {
                            if(m_objectList[m_target]->edge(face->m_edge[h])->m_right==face->m_index)
                            {
                                m_objectList[m_target]->edgeRightChange(face->m_edge[h],0);
                                if(!m_objectList[m_target]->edge(face->m_edge[h])->m_left)
                                {
                                    deleteEdgeH(m_target,face->m_edge[h]);
                                }
                            }
                        }
                        else
                        {
                            if(m_objectList[m_target]->edge(-face->m_edge[h])->m_left==face->m_index)
                            {
                                m_objectList[m_target]->edgeLeftChange(-face->m_edge[h],0);
                                if(!m_objectList[m_target]->edge(-face->m_edge[h])->m_right)
                                {
                                    deleteEdgeH(m_target,-face->m_edge[h]);
                                }
                            }
                        }
                    }
                    m_objectList[m_target]->objectFaceRemove(face->m_index);
                }
                group.clear();

            }
        }

        clearSelection();

        for(unsigned int i=0;i<newFaceSelected.size();++i)
        {
            Face *face=m_objectList[m_target]->face(newFaceSelected[i]);
            if(face)
            {
                selectionPush(face);
            }
        }
    }
}

void Scene::sceneObjectAdd(char *name)
    {
    historyManager->record(new Log_SceneObjectAdd(m_objectList.add(new Object(name))));
    }
void Scene::sceneObjectAdd(Object *newObject)
    {

        historyManager->record(new Log_SceneObjectAdd(m_objectList.add(newObject)));
    }
void Scene::detach(Object *newObject)
    {
        if(m_mode==SelectionMode::Face && m_target)
        {
            sceneObjectAdd(newObject);

            //Ê×ÏÈÒªÍ³¼Æ×é
            unsigned int faceCount=m_selection.size();
            for(unsigned int e=0;e<faceCount;++e)
            {
                Face *face=m_objectList[m_target]->face(m_selection[e]);
                unsigned int edgeCount=face->m_edge.size();
                unsigned int *tempEdge=new unsigned int[edgeCount];
                for(unsigned int h=0;h<edgeCount;++h)
                {
                    Edge *edge;
                    if(face->m_edge[h]>0)
                    {
                        edge=m_objectList[m_target]->edge(face->m_edge[h]);
                    }
                    else
                    {
                        edge=m_objectList[m_target]->edge(-face->m_edge[h]);
                    }
                    if(!edge->m_isIn)
                    {
                        Vertex *start=m_objectList[m_target]->vertex(edge->m_start);
                        Vertex *end=m_objectList[m_target]->vertex(edge->m_end);
                        if(!start->m_isIn)
                        {
                            start->m_clone=newObject->addVertex(start->m_position);
                            start->m_isIn=true;
                            m_isInCache.push_back(start);
                        }
                        if(!end->m_isIn)
                        {
                            end->m_clone=newObject->addVertex(end->m_position.x,end->m_position.y,end->m_position.z);
                            end->m_isIn=true;
                            m_isInCache.push_back(end);
                        }
                        edge->m_clone=newObject->addEdge(start->m_clone,end->m_clone);
                        edge->m_isIn=true;
                        m_isInCache.push_back(edge);
                    }
                    tempEdge[h]=edge->m_clone;;
                }

                newObject->addFace(tempEdge,edgeCount);
                delete tempEdge;
            }
            clearIsInCache();
for(unsigned int e=0;e<faceCount;++e)
            {
                Face *face=m_objectList[m_target]->face(m_selection[e]);
unsigned int edgeCount=face->m_edge.size();
                for(unsigned int h=0;h<edgeCount;++h)
                {
                    if(face->m_edge[h]>0)
                    {
                        m_objectList[m_target]->edgeRightChange(face->m_edge[h],0);
                        if(!m_objectList[m_target]->edge(face->m_edge[h])->m_left)
                        {
                            deleteEdgeH(m_target,face->m_edge[h]);
                        }
                    }
                    else
                    {
                        m_objectList[m_target]->edgeLeftChange(-face->m_edge[h],0);
                        if(!m_objectList[m_target]->edge(-face->m_edge[h])->m_right)
                        {
                            deleteEdgeH(m_target,-face->m_edge[h]);
                        }
                    }
                }
                m_objectList[m_target]->objectFaceRemove(face->m_index);
                //printf("sf");
            }

newObject->m_center=m_objectList[m_target]->m_center;
newObject->m_rotation=m_objectList[m_target]->m_rotation;
newObject->m_position=m_objectList[m_target]->m_position;
newObject->m_scale=m_objectList[m_target]->m_scale;

        }


    }

void Scene::attach(unsigned int original,unsigned int toBeAttached)
    {
        if(original && toBeAttached)
        {
            Vector rotation=m_objectList[toBeAttached]->m_rotation;
            Vector position=m_objectList[toBeAttached]->m_position;
            Vector scale=m_objectList[toBeAttached]->m_scale;
            Vector center=m_objectList[toBeAttached]->m_center;

            Vector originalRotation=m_objectList[original]->m_rotation;
            Vector originalPosition=m_objectList[original]->m_position;
            Vector originalScale=m_objectList[original]->m_scale;
            Vector originalCenter=m_objectList[original]->m_center;

            Matrix transform;

            /*glMatrixMode(GL_MODELVIEW);
            glPushMatrix();
            glLoadIdentity();

            glTranslatef(-originalCenter.x,-originalCenter.y,-originalCenter.z);
            glScalef(originalScale.x,originalScale.y,originalScale.z);
            glRotatef(originalRotation.w,originalRotation.x,originalRotation.y,originalRotation.z);
            glTranslatef(originalCenter.x,originalCenter.y,originalCenter.z);
            glTranslatef(originalPosition.x,originalPosition.y,originalPosition.z);

            glTranslatef(position.x,position.y,position.z);
            glTranslatef(center.x,center.y,center.z);
            glRotatef(rotation.w,rotation.x,rotation.y,rotation.z);
            glScalef(scale.x,scale.y,scale.z);
            glTranslatef(-center.x,-center.y,-center.z);
            glGetFloatv(GL_MODELVIEW_MATRIX,(GLfloat*)(&transform.m));
            glPopMatrix();*/

            unsigned int vertexCount=m_objectList[toBeAttached]->vertexCount();
            for(unsigned int i=0;i<vertexCount;++i)
            {
                Vertex *vertex=m_objectList[toBeAttached]->vertex(i);
                if(vertex)
                {
                    Vector newVertex(transform*(vertex->m_position));
                    vertex->m_clone=m_objectList[original]->addVertex(newVertex,vertex->m_normal);
                }
            }

            unsigned int edgeCount=m_objectList[toBeAttached]->edgeCount();
            for(unsigned int e=0;e<edgeCount;++e)
            {
                Edge *edge=m_objectList[toBeAttached]->edge(e);
                if(e)
                {
                    edge->m_clone=m_objectList[original]->addEdge(m_objectList[toBeAttached]->vertex(edge->m_start)->m_clone,m_objectList[toBeAttached]->vertex(edge->m_end)->m_clone);
                }
            }

            unsigned int faceCount=m_objectList[toBeAttached]->faceCount();
            for(unsigned int f=1;f<faceCount;++f)
            {
                Face *face=m_objectList[toBeAttached]->face(f);
                if(face==NULL) continue;
                unsigned int edgeCount=face->m_edge.size();
                unsigned int *tempEdge=new unsigned int[edgeCount];
                for(unsigned int e=0;e<edgeCount;++e)
                {
                    if(face->m_edge[e]>0)
                    {
                        tempEdge[e]=m_objectList[toBeAttached]->edge(face->m_edge[e])->m_clone;
                    }
                    else
                    {
                        tempEdge[e]=m_objectList[toBeAttached]->edge(-face->m_edge[e])->m_clone;
                    }
                }
                m_objectList[original]->addFace(tempEdge,edgeCount);
                delete tempEdge;
            }
            //É¾³ýÄ£ÐÍ»¹Ã»ÓÐÄØ;
            sceneObjectRemove(toBeAttached);
        }
    }

void Scene::clearSelection()
    {
        unsigned int selectionSize=m_selection.size();
        if(m_mode==SelectionMode::Vertex)
        {
            for(unsigned int i=0;i<selectionSize;i++)
            {
                if(m_objectList[m_target]->vertex(m_selection[i]))
                {
                    m_objectList[m_target]->vertex(m_selection[i])->m_isSelected=false;
                }
            }
        }
        else
        if(m_mode==SelectionMode::Edge)
        {
            for(unsigned int i=0;i<selectionSize;i++)
            {
                if(m_objectList[m_target]->edge(m_selection[i]))
                {
                    m_objectList[m_target]->edge(m_selection[i])->m_isSelected=false;
                }
            }
        }
        else
        if(m_mode==SelectionMode::Face)
        {
            for(unsigned int i=0;i<selectionSize;i++)
            {
                if(m_objectList[m_target]->face(m_selection[i]))
                {
                    m_objectList[m_target]->face(m_selection[i])->m_isSelected=false;
                }
            }
        }
        else
        if(m_mode==SelectionMode::Object)
        {
            for(unsigned int i=0;i<selectionSize;i++)
            {
                if(m_objectList[m_selection[i]])
                {
                    m_objectList[m_selection[i]]->m_isSelected=false;
                }
            }
        }
        m_selection.clear();
    }

Scene::~Scene(void)
{
    delete m_axisCursor;
}
