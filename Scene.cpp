#include "Scene.h"
#include<math.h>

Vector positionAAxis(AxisMode::__Enum axis,float x,float y,float z,float ax,float ay,float az)
{
	Vector result(x,y,z);
	if(axis==AxisMode::axisZ)
	{
		Vector add(ax,ay,az);
		result+=add;
	}
	else if(axis==AxisMode::axisX)
	{
		Vector add(az,ax,ay);
		result+=add;
	}
	else if(axis==AxisMode::axisY)
	{
		Vector add(ax,az,ay);
		result+=add;
	}
	return result;
}

void Scene::initialize()
{
	isInCache.reserve(1000);
	theAxisCursor=new AxisCursor();
	theAxisCursor->initialize();
}

void Scene::changeAxisCursorMode(AxisCursorMode::__Enum newMode)
{
	currentACMode=newMode;
	if(theAxisCursor->mode!=AxisCursorMode::NoAxis)
	{
		theAxisCursor->rotation.null();
		theAxisCursor->mode=currentACMode;
	}
}

void Scene::clearDualEdge(unsigned int tt,unsigned int vertexID)
	{
		if(tt && vertexID)
		{
			//Ê×ÏÈ±éÀúÕâ¸öµãµÄÁÚ½ÓÃæ£¬½«edgeµÄ´óÐ¡Îª2µÄÉ¾³ý
			Vertex *theVertex=theObjectList[tt]->vertex(vertexID);
            unsigned int adjEdgeCount=theVertex->m_adjacentEdgeList.size();
            for(unsigned int e=0;e<adjEdgeCount;++e)
			{
                if(theObjectList[tt]->edge(theVertex->m_adjacentEdgeList[e])->start==theVertex->index)
				{
                    if(theObjectList[tt]->edge(theVertex->m_adjacentEdgeList[e])->left>0 && theObjectList[tt]->face(theObjectList[tt]->edge(theVertex->m_adjacentEdgeList[e])->left)->edge.size()<3)
					{
                        Face *theFace=theObjectList[tt]->face(theObjectList[tt]->edge(theVertex->m_adjacentEdgeList[e])->left);
						if(theFace->edge[0]>0)
						{
							theObjectList[tt]->edgeRightChange(theFace->edge[0],0);
						}
						else
						{
							theObjectList[tt]->edgeLeftChange(theFace->edge[0],0);
						}
						if(theFace->edge[1]>0)
						{
							theObjectList[tt]->edgeRightChange(theFace->edge[1],0);
						}
						else
						{
							theObjectList[tt]->edgeLeftChange(theFace->edge[1],0);
						}
						theObjectList[tt]->objectFaceRemove(theFace->index);
					}
				}
				else
				{
                    if(theObjectList[tt]->edge(theVertex->m_adjacentEdgeList[e])->right>0 && theObjectList[tt]->face(theObjectList[tt]->edge(theVertex->m_adjacentEdgeList[e])->right)->edge.size()<3)
					{
                        Face *theFace=theObjectList[tt]->face(theObjectList[tt]->edge(theVertex->m_adjacentEdgeList[e])->right);
						if(theFace->edge[0]>0)
						{
							theObjectList[tt]->edgeRightChange(theFace->edge[0],0);
						}
						else
						{
							theObjectList[tt]->edgeLeftChange(theFace->edge[0],0);
						}
						if(theFace->edge[1]>0)
						{
							theObjectList[tt]->edgeRightChange(theFace->edge[1],0);
						}
						else
						{
							theObjectList[tt]->edgeLeftChange(theFace->edge[1],0);
						}
						theObjectList[tt]->objectFaceRemove(theFace->index);
					}
				}
			}
			//µ½ÕâÒ»²½Ó¦¸Ã°ÑË«ÖØ±ß¼ÐµÄÃæ¶¼É¾³ýÁË
			//È»ºó±éÀúËùÓÐµÄ±ß ÕÒµ½Ò»×édual±ßµÄÊ±ºò£¬½øÐÐºÏ²¢
            for(unsigned int e=0;e<theVertex->m_adjacentEdgeList.size()-1;++e)
			{
                Edge *currentEdge=theObjectList[tt]->edge(theVertex->m_adjacentEdgeList[e]);
				if(currentEdge->start==theVertex->index)
				{
                    for(unsigned int i=e+1;i<theVertex->m_adjacentEdgeList.size();++i)
					{
                        Edge *tempEdge=theObjectList[tt]->edge(theVertex->m_adjacentEdgeList[i]);
						if(tempEdge->end==theVertex->index && currentEdge->end==tempEdge->start)
						{
							if(currentEdge->left>0 && currentEdge->right==0 && tempEdge->left>0 && tempEdge->right==0)
							{
								Face *theLeft=theObjectList[tt]->face(tempEdge->left);
                                unsigned int fEdgeCount=theLeft->edge.size();
                                for(unsigned int h=0;h<fEdgeCount;++h)
								{
									if(theLeft->edge[h]==-((int)(tempEdge->index)))
									{
										theObjectList[tt]->faceEdgeChange(theLeft->index,h,currentEdge->index);
									}
								}
								deleteEdgeH(tt,tempEdge->index);
							}
							else
							if(currentEdge->right>0 && currentEdge->left==0 && tempEdge->right>0 && tempEdge->left==0)
							{
								Face *theRight=theObjectList[tt]->face(tempEdge->right);
                                unsigned int fEdgeCount=theRight->edge.size();
                                for(unsigned int h=0;h<fEdgeCount;++h)
								{
									if(theRight->edge[h]==((int)tempEdge->index))
									{
										theObjectList[tt]->faceEdgeChange(theRight->index,h,-((int)currentEdge->index));
									}
								}
								deleteEdgeH(tt,tempEdge->index);
								tempEdge->index;
							}
							else
							{
								if(tempEdge->left==0 && tempEdge->right==0)
								{
									deleteEdgeH(tt,tempEdge->index);	
								}
								if(currentEdge->left==0 && currentEdge->left==0)
								{
									deleteEdgeH(tt,currentEdge->index);
								}
							}
						}
						else
						if(tempEdge->start==theVertex->index && currentEdge->end==tempEdge->end)
						{
							if(currentEdge->left>0 && currentEdge->right==0 && tempEdge->right>0 && tempEdge->left==0)
							{
								Face *theRight=theObjectList[tt]->face(tempEdge->right);
                                unsigned int fEdgeCount=theRight->edge.size();
                                for(unsigned int h=0;h<fEdgeCount;++h)
								{
									if(theRight->edge[h]==((int)tempEdge->index))
									{
										theObjectList[tt]->faceEdgeChange(theRight->index,h,-((int)currentEdge->index));
									}
								}
								deleteEdgeH(tt,tempEdge->index);
							}
							else
								if(currentEdge->right>0 && currentEdge->left==0 && tempEdge->left>0 && tempEdge->right==0)
							{
								Face *theLeft=theObjectList[tt]->face(tempEdge->left);
                                unsigned int fEdgeCount=theLeft->edge.size();
                                for(unsigned int h=0;h<fEdgeCount;++h)
								{
									if(theLeft->edge[h]==-((int)tempEdge->index))
									{
										theObjectList[tt]->faceEdgeChange(theLeft->index,h,-((int)currentEdge->index));
									}
								}
								deleteEdgeH(tt,tempEdge->index);
							}
							else
							{
								if(tempEdge->left==0 && tempEdge->right==0)
								{
									deleteEdgeH(tt,tempEdge->index);	
								}
								if(currentEdge->left==0 && currentEdge->left==0)
								{
									deleteEdgeH(tt,currentEdge->index);
								}
							}
						}
					}
				}
				else
				{
                    for(unsigned int i=e+1;i<theVertex->m_adjacentEdgeList.size();++i)
					{
                        Edge *tempEdge=theObjectList[tt]->edge(theVertex->m_adjacentEdgeList[i]);
						if(tempEdge->end==theVertex->index && currentEdge->start==tempEdge->start)
						{
							if(currentEdge->right>0 && currentEdge->left==0 && tempEdge->left>0 && tempEdge->right==0)
							{
								Face *theLeft=theObjectList[tt]->face(tempEdge->left);
                                unsigned int fEdgeCount=theLeft->edge.size();
                                for(unsigned int h=0;h<fEdgeCount;++h)
								{
									if(theLeft->edge[h]==-((int)tempEdge->index))
									{
										theObjectList[tt]->faceEdgeChange(theLeft->index,h,currentEdge->index);
									}
								}
								deleteEdgeH(tt,tempEdge->index);
							}
							else
								if(currentEdge->left>0 && currentEdge->right==0 && tempEdge->right>0 && tempEdge->left==0)
							{
								Face *theRight=theObjectList[tt]->face(tempEdge->right);
                                unsigned int fEdgeCount=theRight->edge.size();
                                for(unsigned int h=0;h<fEdgeCount;++h)
								{
									if(theRight->edge[h]==(int)tempEdge->index)
									{
										theObjectList[tt]->faceEdgeChange(theRight->index,h,currentEdge->index);
									}
								}
								deleteEdgeH(tt,tempEdge->index);
							}
							else
							{
								if(tempEdge->left==0 && tempEdge->right==0)
								{
									deleteEdgeH(tt,tempEdge->index);	
								}
								if(currentEdge->left==0 && currentEdge->left==0)
								{
									deleteEdgeH(tt,currentEdge->index);
								}
							}

						}
						else
						if(tempEdge->start==theVertex->index && currentEdge->start==tempEdge->end)
						{
							if(currentEdge->right>0 && currentEdge->left==0 && tempEdge->right>0 && tempEdge->left==0)
							{
								Face *theRight=theObjectList[tt]->face(tempEdge->right);
                                unsigned int fEdgeCount=theRight->edge.size();
                                for(unsigned int h=0;h<fEdgeCount;++h)
								{
									if(theRight->edge[h]==(int)tempEdge->index)
									{
										theObjectList[tt]->faceEdgeChange(theRight->index,h,currentEdge->index);
									}
								}
								deleteEdgeH(tt,tempEdge->index);
							}
							else
								if(currentEdge->left>0 && currentEdge->right==0 && tempEdge->left>0 && tempEdge->right==0)
							{
								Face *theLeft=theObjectList[tt]->face(tempEdge->left);
                                unsigned int fEdgeCount=theLeft->edge.size();
                                for(unsigned int h=0;h<fEdgeCount;++h)
								{
									if(theLeft->edge[h]==-((int)tempEdge->index))
									{
										theObjectList[tt]->faceEdgeChange(theLeft->index,h,currentEdge->index);
									}
								}
								deleteEdgeH(tt,tempEdge->index);
							}
							else
							{
								if(tempEdge->left==0 && tempEdge->right==0)
								{
									deleteEdgeH(tt,tempEdge->index);	
								}
								if(currentEdge->left==0 && currentEdge->left==0)
								{
									deleteEdgeH(tt,currentEdge->index);
								}
							}
						}
					}
				}
			}
		}
	}


Scene::Scene(void):mode(SelectionMode::Object),target(0),currentACMode(AxisCursorMode::MoveAxis),isSplitMode(false),splitVertexID(0)
{

}

unsigned int Scene::newCube(float x,float y,float z,float lx,float ly,float lz,unsigned int sx,unsigned int sy,unsigned int sz)
{
	Object *theCube=new Object("Cube");
	newCube(theCube,x,y,z,lx,ly,lz,sx,sy,sz);
	theObjectList.add(theCube);
	return theCube->index;
}

unsigned int Scene::newPlane(float x,float y,float z,float length,float width,AxisMode::__Enum axis,unsigned int sl,unsigned int sw)
{
	Object *thePlane=new Object("Plane");
	newPlane(thePlane,x,y,z,length,width,axis,sl,sw);
	theObjectList.add(thePlane);
	return thePlane->index;
}

void Scene::newPlane(Object *thePlane,float x,float y,float z,float length,float width,AxisMode::__Enum axis,unsigned int sl,unsigned int sw)
{
	thePlane->center.x=x;
	thePlane->center.y=y;
	thePlane->center.z=z;
	
	{
		//Ê×ÏÈ¼ÆËãÆðÊ¼µãµÄÎ»ÖÃ
		float halfLength=length*0.5f;
		float halfWidth=width*0.5f;
		float segmentL=length/sl;
		float segmentW=length/sw;
		
        unsigned int *tempVertexListRight=new unsigned int[sl+1];
        unsigned int *tempVertexListLeft=new unsigned int[sl+1];
        unsigned int *rightEdge=new unsigned int[sl];
        unsigned int *leftEdge=new unsigned int[sl];

		//Ê×ÏÈ¼ÆËãµÚÒ»ÐÐ
		Vector positionAAxisV(positionAAxis(axis,x,y,z,-halfLength,-halfWidth,0));
		tempVertexListRight[0]=thePlane->addVertex(positionAAxisV);
        for(unsigned int e=1;e<=sl;++e)
		{
			positionAAxisV=positionAAxis(axis,x,y,z,-halfLength+e*segmentL,-halfWidth,0);
			tempVertexListRight[e]=thePlane->addVertex(positionAAxisV);
			rightEdge[e-1]=thePlane->addEdge(tempVertexListRight[e-1],tempVertexListRight[e]);
		}

        for(unsigned int i=1;i<=sw;++i)
		{
			//µÃµ½ÕâÒ»ÐÐµÄµÚÒ»¸ö
			positionAAxisV=positionAAxis(axis,x,y,z,-halfLength,-halfWidth+i*segmentW,0);
			tempVertexListLeft[0]=thePlane->addVertex(positionAAxisV);
            for(unsigned int e=1;e<=sl;++e)
			{
				positionAAxisV=positionAAxis(axis,x,y,z,-halfLength+e*segmentL,-halfWidth+i*segmentW,0);
				tempVertexListLeft[e]=thePlane->addVertex(positionAAxisV);
				leftEdge[e-1]=thePlane->addEdge(tempVertexListLeft[e-1],tempVertexListLeft[e]);
			}

			//Éú³ÉµÚÒ»¸öµ×±ß
            unsigned int bottomEdge=thePlane->addEdge(tempVertexListRight[0],tempVertexListLeft[0]);
			//Éú³ÉµÚÒ»¸ö¶¥±ß
            for(unsigned int e=0;e<sl;++e)
			{
                unsigned int topEdge=thePlane->addEdge(tempVertexListRight[e+1],tempVertexListLeft[e+1]);
                unsigned int tempEdges[4];
				tempEdges[0]=bottomEdge;
				tempEdges[1]=leftEdge[e];
				tempEdges[2]=topEdge;
				tempEdges[3]=rightEdge[e];

				thePlane->addFace(tempEdges,4);
				bottomEdge=topEdge;
			}

            for(unsigned int e=0;e<sl;++e)
			{
				rightEdge[e]=leftEdge[e];
				tempVertexListRight[e]=tempVertexListLeft[e];
			}
			tempVertexListRight[sl]=tempVertexListLeft[sl];
		}
		
		delete leftEdge;
		delete rightEdge;
	  delete tempVertexListLeft;
		delete tempVertexListRight;
	}
}

void Scene::newCube(Object *theCube,float x,float y,float z,float lx,float ly,float lz,unsigned int sx,unsigned int sy,unsigned int sz)
{
	theCube->center.x=x;
	theCube->center.y=y;
	theCube->center.z=z;

	//¼ÆËã°ë±ß³¤
	float hlx=lx/2;
	float hly=ly/2;
	float hlz=lz/2;
	//¼ÆËãÃ¿Ò»¶ÎµÄ±ß³¤
	float slx=lx/sx;
	float sly=ly/sy;
	float slz=lz/sz;
	//Ê×ÏÈÉùÃ÷±ßºÍ±ßÉÏµÄµãÁÐ±í
    unsigned int *edge1=new unsigned int[sz];
    unsigned int *vertex1=new unsigned int[sz+1];
    unsigned int *edge2=new unsigned int[sz];
    unsigned int *vertex2=new unsigned int[sz+1];
    unsigned int *edge3=new unsigned int[sz];
    unsigned int *vertex3=new unsigned int[sz+1];
    unsigned int *edge4=new unsigned int[sz];
    unsigned int *vertex4=new unsigned int[sz+1];
    unsigned int *edge5=new unsigned int[sy];
    unsigned int *vertex5=new unsigned int[sy+1];
    unsigned int *edge6=new unsigned int[sy];
    unsigned int *vertex6=new unsigned int[sy+1];
    unsigned int *edge7=new unsigned int[sx];
    unsigned int *vertex7=new unsigned int[sx+1];
    unsigned int *edge8=new unsigned int[sx];
    unsigned int *vertex8=new unsigned int[sx+1];
    unsigned int *edge9=new unsigned int[sy];
    unsigned int *vertex9=new unsigned int[sy+1];
    unsigned int *edge10=new unsigned int[sy];
    unsigned int *vertex10=new unsigned int[sy+1];

    unsigned int *edge11=new unsigned int[sx];
    unsigned int *vertex11=new unsigned int[sx+1];
    unsigned int *edge12=new unsigned int[sx];
    unsigned int *vertex12=new unsigned int[sx+1];

	//È»ºó¼ÆËã¸÷¸ö¶¥µã
	vertex12[sx]=vertex1[0]=vertex6[0]=theCube->addVertex(x+hlx,y+hly,z-hlz);
	vertex6[sy]=vertex8[0]=vertex2[0]=theCube->addVertex(x+hlx,y-hly,z-hlz);
	vertex8[sx]=vertex10[0]=vertex3[0]=theCube->addVertex(x-hlx,y-hly,z-hlz);
	vertex10[sy]=vertex12[0]=vertex4[0]=theCube->addVertex(x-hlx,y+hly,z-hlz);

	vertex11[sx]=vertex5[0]=vertex1[sz]=theCube->addVertex(x+hlx,y+hly,z+hlz);
	vertex5[sy]=vertex7[0]=vertex2[sz]=theCube->addVertex(x+hlx,y-hly,z+hlz);
	vertex7[sx]=vertex9[0]=vertex3[sz]=theCube->addVertex(x-hlx,y-hly,z+hlz);
	vertex9[sy]=vertex11[0]=vertex4[sz]=theCube->addVertex(x-hlx,y+hly,z+hlz);
	
	//²¹È«¶¥µãÁÐ±íÉÏÃæµÄÊý¾Ý
    for(unsigned int i=1;i<sz;++i)
	{
		vertex1[i]=theCube->addVertex(x+hlx,y+hly,z-hlz+i*slz);
		vertex2[i]=theCube->addVertex(x+hlx,y-hly,z-hlz+i*slz);
		vertex3[i]=theCube->addVertex(x-hlx,y-hly,z-hlz+i*slz);
		vertex4[i]=theCube->addVertex(x-hlx,y+hly,z-hlz+i*slz);
	}

    for(unsigned int i=1;i<sy;++i)
	{
		vertex5[i]=theCube->addVertex(x+hlx,y+hly-i*sly,z+hlz);
		vertex6[i]=theCube->addVertex(x+hlx,y+hly-i*sly,z-hlz);
		vertex9[i]=theCube->addVertex(x-hlx,y-hly+i*sly,z+hlz);
		vertex10[i]=theCube->addVertex(x-hlx,y-hly+i*sly,z-hlz);
	}

    for(unsigned int i=1;i<sx;++i)
	{
		vertex7[i]=theCube->addVertex(x+hlx-i*slx,y-hly,z+hlz);
		vertex8[i]=theCube->addVertex(x+hlx-i*slx,y-hly,z-hlz);
		vertex11[i]=theCube->addVertex(x-hlx+i*slx,y+hly,z+hlz);
		vertex12[i]=theCube->addVertex(x-hlx+i*slx,y+hly,z-hlz);
	}

	//È»ºó¿ªÊ¼Éú³ÉËùÓÐµÄ±ßÁÐ±í
    for(unsigned int i=0;i<sz;++i)
	{
		edge1[i]=theCube->addEdge(vertex1[i],vertex1[i+1]);
		edge2[i]=theCube->addEdge(vertex2[i],vertex2[i+1]);
		edge3[i]=theCube->addEdge(vertex3[i],vertex3[i+1]);
		edge4[i]=theCube->addEdge(vertex4[i],vertex4[i+1]);
	}

    for(unsigned int i=0;i<sx;++i)
	{
		edge7[i]=theCube->addEdge(vertex7[i],vertex7[i+1]);
		edge8[i]=theCube->addEdge(vertex8[i],vertex8[i+1]);
		edge11[i]=theCube->addEdge(vertex11[i],vertex11[i+1]);
		edge12[i]=theCube->addEdge(vertex12[i],vertex12[i+1]);
	}

    for(unsigned int i=0;i<sy;++i)
	{
		edge5[i]=theCube->addEdge(vertex5[i],vertex5[i+1]);
		edge6[i]=theCube->addEdge(vertex6[i],vertex6[i+1]);
		edge9[i]=theCube->addEdge(vertex9[i],vertex9[i+1]);
		edge10[i]=theCube->addEdge(vertex10[i],vertex10[i+1]);
	}

	//È»ºó¿ªÊ¼ÕýÊ½Éú³ÉÃæ
	{
		//Éú³ÉÃæ1ºÍÃæ3
        unsigned int *rightEdge1=new unsigned int[sz];
        unsigned int *rightEdge3=new unsigned int[sz];
        unsigned int *rightVertex1=new unsigned int[sz];
        unsigned int *rightVertex3=new unsigned int[sz];
        for(unsigned int i=0;i<sz;++i)
		{
			rightEdge1[i]=edge1[i];
			rightEdge3[i]=edge3[i];
			rightVertex1[i]=vertex1[i+1];
			rightVertex3[i]=vertex3[i+1];
		}
        for(unsigned int i=0;i<sy-1;++i)
		{
            unsigned int lowEdge1=edge6[i];
            unsigned int lowEdge3=edge10[i];
            unsigned int lowLeftVertex1=vertex6[i+1];
            unsigned int lowLeftVertex3=vertex10[i+1];
            unsigned int *leftEdge1=new unsigned int[sz];
            unsigned int *leftEdge3=new unsigned int[sz];
            for(unsigned int e=0;e<sz-1;++e)
			{
                unsigned int newVertex1=theCube->addVertex(x+hlx,y+hly-(i+1)*sly,z-hlz+(e+1)*slz);
                unsigned int newVertex3=theCube->addVertex(x-hlx,y-hly+(i+1)*sly,z-hlz+(e+1)*slz);
				leftEdge1[e]=theCube->addEdge(lowLeftVertex1,newVertex1);
				leftEdge3[e]=theCube->addEdge(lowLeftVertex3,newVertex3);
                unsigned int topEdge1=theCube->addEdge(newVertex1,rightVertex1[e]);
                unsigned int topEdge3=theCube->addEdge(newVertex3,rightVertex3[e]);
                unsigned int edgeList1[4]={0};
                unsigned int edgeList3[4]={0};
				edgeList1[0]=leftEdge1[e];
				edgeList3[0]=leftEdge3[e];
				edgeList1[1]=topEdge1;
				edgeList3[1]=topEdge3;
				edgeList1[2]=rightEdge1[e];
				edgeList3[2]=rightEdge3[e];
				edgeList1[3]=lowEdge1;
				edgeList3[3]=lowEdge3;
				theCube->addFace(edgeList1,4);
				theCube->addFace(edgeList3,4);
				lowEdge1=topEdge1;
				lowEdge3=topEdge3;
				lowLeftVertex1=newVertex1;
				lowLeftVertex3=newVertex3;
				rightVertex1[e]=newVertex1;
				rightVertex3[e]=newVertex3;
			}
			{
				//¼ÆËã×îÉÏÃæÒ»¸ñ
				leftEdge1[sz-1]=theCube->addEdge(lowLeftVertex1,vertex5[i+1]);
				leftEdge3[sz-1]=theCube->addEdge(lowLeftVertex3,vertex9[i+1]);
                unsigned int topEdge1=edge5[i];
                unsigned int topEdge3=edge9[i];
                unsigned int edgeList1[4]={0};
                unsigned int edgeList3[4]={0};
				edgeList1[0]=leftEdge1[sz-1];
				edgeList3[0]=leftEdge3[sz-1];
				edgeList1[1]=topEdge1;
				edgeList3[1]=topEdge3;
				edgeList1[2]=rightEdge1[sz-1];
				edgeList3[2]=rightEdge3[sz-1];
				edgeList1[3]=lowEdge1;
				edgeList3[3]=lowEdge3;
				theCube->addFace(edgeList1,4);
				theCube->addFace(edgeList3,4);
				rightVertex1[sz-1]=vertex5[i+1];
				rightVertex3[sz-1]=vertex9[i+1];
			}
            for(unsigned int e=0;e<sz;++e)
			{
				rightEdge1[e]=leftEdge1[e];
				rightEdge3[e]=leftEdge3[e];
			}
			delete leftEdge1;
			delete leftEdge3;
		}
        unsigned int lowEdge1=edge6[sy-1];
        unsigned int lowEdge3=edge10[sy-1];
        unsigned int lowLeftVertex1=vertex2[0];
        unsigned int lowLeftVertex3=vertex4[0];
        for(unsigned int e=0;e<sz-1;++e)
		{
            unsigned int newVertex1=vertex2[e+1];
            unsigned int newVertex3=vertex4[e+1];
            unsigned int topEdge1=theCube->addEdge(newVertex1,rightVertex1[e]);
            unsigned int topEdge3=theCube->addEdge(newVertex3,rightVertex3[e]);
            unsigned int edgeList1[4]={0};
            unsigned int edgeList3[4]={0};
			edgeList1[0]=edge2[e];
			edgeList3[0]=edge4[e];
			edgeList1[1]=topEdge1;
			edgeList3[1]=topEdge3;
			edgeList1[2]=rightEdge1[e];
			edgeList3[2]=rightEdge3[e];
			edgeList1[3]=lowEdge1;
			edgeList3[3]=lowEdge3;
			theCube->addFace(edgeList1,4);
			theCube->addFace(edgeList3,4);
			lowEdge1=topEdge1;
			lowEdge3=topEdge3;
			lowLeftVertex1=newVertex1;
			lowLeftVertex3=newVertex3;
		}
		//¼ÆËã×îÉÏÃæµÄ·½¸ñ
		{
            unsigned int edgeList1[4]={0};
            unsigned int edgeList3[4]={0};
			edgeList1[0]=edge2[sz-1];
			edgeList3[0]=edge4[sz-1];
			edgeList1[1]=edge5[sy-1];
			edgeList3[1]=edge9[sy-1];
			edgeList1[2]=rightEdge1[sz-1];
			edgeList3[2]=rightEdge3[sz-1];
			edgeList1[3]=lowEdge1;
			edgeList3[3]=lowEdge3;
			theCube->addFace(edgeList1,4);
			theCube->addFace(edgeList3,4);
		}
		delete rightEdge1;
		delete rightEdge3;
		delete rightVertex1;
		delete rightVertex3;
	}
	{
		//Éú³ÉÃæ2ºÍÃæ4
        unsigned int *rightEdge2=new unsigned int[sz];
        unsigned int *rightEdge4=new unsigned int[sz];
        unsigned int *rightVertex2=new unsigned int[sz];
        unsigned int *rightVertex4=new unsigned int[sz];
        for(unsigned int i=0;i<sz;++i)
		{
			rightEdge2[i]=edge2[i];
			rightEdge4[i]=edge4[i];
			rightVertex2[i]=vertex2[i+1];
			rightVertex4[i]=vertex4[i+1];
		}
        for(unsigned int i=0;i<sx-1;++i)
		{
            unsigned int lowEdge2=edge8[i];
            unsigned int lowEdge4=edge12[i];
            unsigned int lowLeftVertex2=vertex8[i+1];
            unsigned int lowLeftVertex4=vertex12[i+1];
            unsigned int *leftEdge2=new unsigned int[sz];
            unsigned int *leftEdge4=new unsigned int[sz];
            for(unsigned int e=0;e<sz-1;++e)
			{
                unsigned int newVertex2=theCube->addVertex(x+hlx-(i+1)*slx,y-hly,z-hlz+(e+1)*slz);
                unsigned int newVertex4=theCube->addVertex(x-hlx+(i+1)*slx,y+hly,z-hlz+(e+1)*slz);
				leftEdge2[e]=theCube->addEdge(lowLeftVertex2,newVertex2);
				leftEdge4[e]=theCube->addEdge(lowLeftVertex4,newVertex4);
                unsigned int topEdge2=theCube->addEdge(newVertex2,rightVertex2[e]);
                unsigned int topEdge4=theCube->addEdge(newVertex4,rightVertex4[e]);
                unsigned int edgeList2[4]={0};
                unsigned int edgeList4[4]={0};
				edgeList2[0]=leftEdge2[e];
				edgeList4[0]=leftEdge4[e];
				edgeList2[1]=topEdge2;
				edgeList4[1]=topEdge4;
				edgeList2[2]=rightEdge2[e];
				edgeList4[2]=rightEdge4[e];
				edgeList2[3]=lowEdge2;
				edgeList4[3]=lowEdge4;
				theCube->addFace(edgeList2,4);
				theCube->addFace(edgeList4,4);
				lowEdge2=topEdge2;
				lowEdge4=topEdge4;
				lowLeftVertex2=newVertex2;
				lowLeftVertex4=newVertex4;
				rightVertex2[e]=newVertex2;
				rightVertex4[e]=newVertex4;
			}
			{
				//¼ÆËã×îÉÏÃæÒ»¸ñ
				leftEdge2[sz-1]=theCube->addEdge(lowLeftVertex2,vertex7[i+1]);
				leftEdge4[sz-1]=theCube->addEdge(lowLeftVertex4,vertex11[i+1]);
                unsigned int topEdge2=edge7[i];
                unsigned int topEdge4=edge11[i];
                unsigned int edgeList2[4]={0};
                unsigned int edgeList4[4]={0};
				edgeList2[0]=leftEdge2[sz-1];
				edgeList4[0]=leftEdge4[sz-1];
				edgeList2[1]=topEdge2;
				edgeList4[1]=topEdge4;
				edgeList2[2]=rightEdge2[sz-1];
				edgeList4[2]=rightEdge4[sz-1];
				edgeList2[3]=lowEdge2;
				edgeList4[3]=lowEdge4;
				theCube->addFace(edgeList2,4);
				theCube->addFace(edgeList4,4);
				rightVertex2[sz-1]=vertex7[i+1];
				rightVertex4[sz-1]=vertex11[i+1];
			}
            for(unsigned int e=0;e<sz;++e)
			{
				rightEdge2[e]=leftEdge2[e];
				rightEdge4[e]=leftEdge4[e];
			}
			delete leftEdge2;
			delete leftEdge4;
		}
        unsigned int lowEdge2=edge8[sx-1];
        unsigned int lowEdge4=edge12[sx-1];
        unsigned int lowLeftVertex2=vertex3[0];
        unsigned int lowLeftVertex4=vertex1[0];
        for(unsigned int e=0;e<sz-1;e++)
		{
            unsigned int newVertex2=vertex3[e+1];
            unsigned int newVertex4=vertex1[e+1];
            unsigned int topEdge2=theCube->addEdge(newVertex2,rightVertex2[e]);
            unsigned int topEdge4=theCube->addEdge(newVertex4,rightVertex4[e]);
            unsigned int edgeList2[4]={0};
            unsigned int edgeList4[4]={0};
			edgeList2[0]=edge3[e];
			edgeList4[0]=edge1[e];
			edgeList2[1]=topEdge2;
			edgeList4[1]=topEdge4;
			edgeList2[2]=rightEdge2[e];
			edgeList4[2]=rightEdge4[e];
			edgeList2[3]=lowEdge2;
			edgeList4[3]=lowEdge4;
			theCube->addFace(edgeList2,4);
			theCube->addFace(edgeList4,4);
			lowEdge2=topEdge2;
			lowEdge4=topEdge4;
			lowLeftVertex2=newVertex2;
			lowLeftVertex4=newVertex4;
		}
		//¼ÆËã×îÉÏÃæµÄ·½¸ñ
		{
            unsigned int edgeList2[4]={0};
            unsigned int edgeList4[4]={0};
			edgeList2[0]=edge3[sz-1];
			edgeList4[0]=edge1[sz-1];
			edgeList2[1]=edge7[sx-1];
			edgeList4[1]=edge11[sx-1];
			edgeList2[2]=rightEdge2[sz-1];
			edgeList4[2]=rightEdge4[sz-1];
			edgeList2[3]=lowEdge2;
			edgeList4[3]=lowEdge4;
			theCube->addFace(edgeList2,4);
			theCube->addFace(edgeList4,4);
		}
		delete rightEdge2;
		delete rightEdge4;
		delete rightVertex2;
		delete rightVertex4;
	}
	{
		//Éú³ÉÃæ5,ÉÏÃæµÄÃæÊÇ5
        unsigned int *rightEdge5=new unsigned int[sx];
        unsigned int *rightVertex5=new unsigned int[sx];
        for(unsigned int i=0;i<sx;++i)
		{
			rightEdge5[i]=edge7[i];
			rightVertex5[i]=vertex7[i+1];
		}
        for(unsigned int i=0;i<sy-1;++i)
		{
            unsigned int lowEdge5=edge5[sy-1-i];
            unsigned int lowLeftVertex5=vertex5[sy-1-i];
            unsigned int *leftEdge5=new unsigned int[sx];
            for(unsigned int e=0;e<sx-1;++e)
			{
                unsigned int newVertex5=theCube->addVertex(x+hlx-(e+1)*slx,y-hly+(i+1)*sly,z+hlz);
				leftEdge5[e]=theCube->addEdge(lowLeftVertex5,newVertex5);
                unsigned int topEdge5=theCube->addEdge(newVertex5,rightVertex5[e]);
                unsigned int edgeList5[4]={0};
				edgeList5[3]=leftEdge5[e];
				edgeList5[2]=topEdge5;
				edgeList5[1]=rightEdge5[e];
				edgeList5[0]=lowEdge5;
				theCube->addFace(edgeList5,4);
				lowEdge5=topEdge5;
				lowLeftVertex5=newVertex5;
				rightVertex5[e]=newVertex5;
			}
			{
				//¼ÆËã×îÉÏÃæÒ»¸ñ
				leftEdge5[sx-1]=theCube->addEdge(lowLeftVertex5,vertex9[i+1]);
                unsigned int topEdge5=edge9[i];
                unsigned int edgeList5[4]={0};
				edgeList5[3]=leftEdge5[sx-1];
				edgeList5[2]=topEdge5;
				edgeList5[1]=rightEdge5[sx-1];
				edgeList5[0]=lowEdge5;
				theCube->addFace(edgeList5,4);
				rightVertex5[sx-1]=vertex9[i+1];
			}
            for(unsigned int e=0;e<sx;++e)
			{
				rightEdge5[e]=leftEdge5[e];
			}
			delete leftEdge5;
		}
        unsigned int lowEdge5=edge5[0];
        unsigned int lowLeftVertex5=vertex11[sx];
        for(unsigned int e=0;e<sx-1;++e)
		{
            unsigned int newVertex5=vertex11[sx-1-e];
            unsigned int topEdge5=theCube->addEdge(newVertex5,rightVertex5[e]);
            unsigned int edgeList5[4]={0};
			edgeList5[3]=edge11[sx-1-e];
			edgeList5[2]=topEdge5;
			edgeList5[1]=rightEdge5[e];
			edgeList5[0]=lowEdge5;
			theCube->addFace(edgeList5,4);
			lowEdge5=topEdge5;
			lowLeftVertex5=newVertex5;
		}
		//¼ÆËã×îÉÏÃæµÄ·½¸ñ
		{
            unsigned int edgeList5[4]={0};
			edgeList5[3]=edge11[0];
			edgeList5[2]=edge9[sy-1];
			edgeList5[1]=rightEdge5[sx-1];
			edgeList5[0]=lowEdge5;
			theCube->addFace(edgeList5,4);
		}
		delete rightEdge5;
		delete rightVertex5;
	}
	{
		//Éú³ÉÃæ6,ÉÏÃæµÄÃæÊÇ5
        unsigned int *rightEdge6=new unsigned int[sy];
        unsigned int *rightVertex6=new unsigned int[sy];
        for(unsigned int i=0;i<sy;++i)
		{
			rightEdge6[i]=edge6[i];
			rightVertex6[i]=vertex6[i+1];
		}
        for(unsigned int i=0;i<sx-1;++i)
		{
            unsigned int lowEdge6=edge12[sx-1-i];
            unsigned int lowLeftVertex6=vertex12[sx-1-i];
            unsigned int *leftEdge6=new unsigned int[sy];
            for(unsigned int e=0;e<sy-1;++e)
			{
                unsigned int newVertex6=theCube->addVertex(x+hlx-(i+1)*slx,y+hly-(e+1)*sly,z-hlz);
				leftEdge6[e]=theCube->addEdge(lowLeftVertex6,newVertex6);
                unsigned int topEdge6=theCube->addEdge(newVertex6,rightVertex6[e]);
                unsigned int edgeList6[4]={0};
				edgeList6[0]=leftEdge6[e];
				edgeList6[1]=topEdge6;
				edgeList6[2]=rightEdge6[e];
				edgeList6[3]=lowEdge6;
				theCube->addFace(edgeList6,4);
				lowEdge6=topEdge6;
				lowLeftVertex6=newVertex6;
				rightVertex6[e]=newVertex6;
			}
			{
				//¼ÆËã×îÉÏÃæÒ»¸ñ
				leftEdge6[sy-1]=theCube->addEdge(lowLeftVertex6,vertex8[i+1]);
                unsigned int topEdge6=edge8[i];
                unsigned int edgeList6[4]={0};
				edgeList6[0]=leftEdge6[sy-1];
				edgeList6[1]=topEdge6;
				edgeList6[2]=rightEdge6[sy-1];
				edgeList6[3]=lowEdge6;
				theCube->addFace(edgeList6,4);
				rightVertex6[sy-1]=vertex8[i+1];
			}
            for(unsigned int e=0;e<sy;++e)
			{
				rightEdge6[e]=leftEdge6[e];
			}
			delete leftEdge6;
		}
		
        unsigned int lowEdge6=edge12[0];
        unsigned int lowLeftVertex6=vertex10[sy];
        for(unsigned int e=0;e<sy-1;++e)
		{
            unsigned int newVertex6=vertex10[sy-1-e];
            unsigned int topEdge6=theCube->addEdge(newVertex6,rightVertex6[e]);
            unsigned int edgeList6[4]={0};
			edgeList6[0]=edge10[sy-1-e];
			edgeList6[1]=topEdge6;
			edgeList6[2]=rightEdge6[e];
			edgeList6[3]=lowEdge6;
			theCube->addFace(edgeList6,4);
			lowEdge6=topEdge6;
			lowLeftVertex6=newVertex6;
		}
		//¼ÆËã×îÉÏÃæµÄ·½¸ñ
		{
            unsigned int edgeList6[4]={0};
			edgeList6[0]=edge10[0];
			edgeList6[1]=edge8[sx-1];
			edgeList6[2]=rightEdge6[sy-1];
			edgeList6[3]=lowEdge6;
			theCube->addFace(edgeList6,4);
		}
		delete rightEdge6;
		delete rightVertex6;
	}
	//ÊÍ·Å¿Õ¼ä
	delete edge1;
	delete vertex1;
	delete edge2;
	delete vertex2;
	delete edge3;
	delete vertex3;
	delete edge4;
	delete vertex4;

	delete edge5;
	delete vertex5;
	delete edge6;
	delete vertex6;

	delete edge7;
	delete vertex7;
	delete edge8;
	delete vertex8;

	delete edge9;
	delete vertex9;
	delete edge10;
	delete vertex10;

	delete edge11;
	delete vertex11;
	delete edge12;
	delete vertex12;
}

unsigned int Scene::newCylinder(float x,float y,float z,float r,float h,AxisMode::__Enum axis,unsigned int sa,unsigned int sr,unsigned int sh)
{
	Object *theCylinder=new Object("Clinder");
	newCylinder(theCylinder,x,y,z,r,h,axis,sa,sr,sh);
	theObjectList.add(theCylinder);
	return theCylinder->index;
}
void Scene::newCylinder(Object *theCylinder,float x,float y,float z,float r,float h,AxisMode::__Enum axis,unsigned int sa,unsigned int sr,unsigned int sh)
{
	//Ê×ÏÈÉú³ÉÒ»¸öÎïÌå
	theCylinder->center.x=x;
	theCylinder->center.y=y;
	theCylinder->center.z=z;

	{
		//Ê×ÏÈÉú³ÉÖáÉÏµÄÁ½¸öµã
		Vector positionAAxisV(positionAAxis(axis,x,y,z,0,0,0));
        unsigned int center=theCylinder->addVertex(positionAAxisV);
		positionAAxisV=positionAAxis(axis,x,y,z,0,0,h);
        unsigned int centerH=theCylinder->addVertex(positionAAxisV);

		//¼ÆËã»¡¶È¼ä¸ô
		float da=ps2PI/sa;
		//¼ÆËã°ë¾¶µÝÔö¼ä¸ô
		float dr=r/sr;
		//¼ÆËã¸ß¶ÈµÝÔö¼ä¸ô
		float dh=h/sh;

        unsigned int *startHEdge=new unsigned int[sh];
        unsigned int *startHVertex=new unsigned int[sh+1];

        unsigned int *startUEdge=new unsigned int[sr];
        unsigned int *startUVertex=new unsigned int[sr+1];

        unsigned int *startLEdge=new unsigned int[sr];
        unsigned int *startLVertex=new unsigned int[sr+1];

		//³õÊ¼»¯ÕâÐ©ÐÅÏ¢

		startUVertex[0]=centerH;
		startLVertex[0]=center;
		
		positionAAxisV=positionAAxis(axis,x,y,z,r,0,h);
		startUVertex[sr]=startHVertex[sh]=theCylinder->addVertex(positionAAxisV);
		positionAAxisV=positionAAxis(axis,x,y,z,r,0,0);
		startLVertex[sr]=startHVertex[0]=theCylinder->addVertex(positionAAxisV);

		//³õÊ¼»¯ÆðÊ¼Ïß
        for(unsigned int i=1;i<sr;++i)
		{
			positionAAxisV=positionAAxis(axis,x,y,z,dr*i,0,h);
			startUVertex[i]=theCylinder->addVertex(positionAAxisV);
			startUEdge[i-1]=theCylinder->addEdge(startUVertex[i-1],startUVertex[i]);
			positionAAxisV=positionAAxis(axis,x,y,z,dr*i,0,0);
			startLVertex[i]=theCylinder->addVertex(positionAAxisV);
			startLEdge[i-1]=theCylinder->addEdge(startLVertex[i-1],startLVertex[i]);
		}
		startUEdge[sr-1]=theCylinder->addEdge(startUVertex[sr-1],startUVertex[sr]);
		startLEdge[sr-1]=theCylinder->addEdge(startLVertex[sr-1],startLVertex[sr]);

        for(unsigned int i=1;i<sh;++i)
		{
			positionAAxisV=positionAAxis(axis,x,y,z,r,0,dh*i);
			startHVertex[i]=theCylinder->addVertex(positionAAxisV);
			startHEdge[i-1]=theCylinder->addEdge(startHVertex[i-1],startHVertex[i]);
		}
		startHEdge[sh-1]=theCylinder->addEdge(startHVertex[sh-1],startHVertex[sh]);

        unsigned int *endHEdgeC=new unsigned int[sh];
        unsigned int *endHVertexC=new unsigned int[sh+1];

        for(unsigned int i=0;i<sh;++i)
		{
			endHEdgeC[i]=startHEdge[i];
			endHVertexC[i]=startHVertex[i];
		}
		endHVertexC[sh]=startHVertex[sh];

        unsigned int *endUEdgeC=new unsigned int[sr];
        unsigned int *endUVertexC=new unsigned int[sr+1];
        unsigned int *endLEdgeC=new unsigned int[sr];
        unsigned int *endLVertexC=new unsigned int[sr+1];

        for(unsigned int i=0;i<sr;++i)
		{
			endUEdgeC[i]=startUEdge[i];
			endUVertexC[i]=startUVertex[i];
			endLEdgeC[i]=startLEdge[i];
			endLVertexC[i]=startLVertex[i];
		}
		endUVertexC[sr]=startUVertex[sr];
		endLVertexC[sr]=startLVertex[sr];

        for(unsigned int e=1;e<sa;++e)
		{
            unsigned int *endHEdge=new unsigned int[sh];
            unsigned int *endHVertex=new unsigned int[sh+1];
            unsigned int *endUEdge=new unsigned int[sr];
            unsigned int *endUVertex=new unsigned int[sr+1];
            unsigned int *endLEdge=new unsigned int[sr];
            unsigned int *endLVertex=new unsigned int[sr+1];

			//³õÊ¼»¯ÕâÐ©ÐÅÏ¢
		
			endUVertex[0]=centerH;
			endLVertex[0]=center;
			
			positionAAxisV=positionAAxis(axis,x,y,z,r*cos(da*e),r*sin(da*e),h);
			endUVertex[sr]=endHVertex[sh]=theCylinder->addVertex(positionAAxisV);
			positionAAxisV=positionAAxis(axis,x,y,z,r*cos(da*e),r*sin(da*e),0);
			endLVertex[sr]=endHVertex[0]=theCylinder->addVertex(positionAAxisV);

		//³õÊ¼»¯ÆðÊ¼Ïß
            for(unsigned int i=1;i<sr;++i)
			{
				positionAAxisV=positionAAxis(axis,x,y,z,dr*i*cos(da*e),dr*i*sin(da*e),h);
				endUVertex[i]=theCylinder->addVertex(positionAAxisV);
				endUEdge[i-1]=theCylinder->addEdge(endUVertex[i-1],endUVertex[i]);
				positionAAxisV=positionAAxis(axis,x,y,z,dr*i*cos(da*e),dr*i*sin(da*e),0);
				endLVertex[i]=theCylinder->addVertex(positionAAxisV);
				endLEdge[i-1]=theCylinder->addEdge(endLVertex[i-1],endLVertex[i]);
			}
			endUEdge[sr-1]=theCylinder->addEdge(endUVertex[sr-1],endUVertex[sr]);
			endLEdge[sr-1]=theCylinder->addEdge(endLVertex[sr-1],endLVertex[sr]);

            for(unsigned int i=1;i<sh;++i)
			{
				positionAAxisV=positionAAxis(axis,x,y,z,r*cos(da*e),r*sin(da*e),dh*i);
				endHVertex[i]=theCylinder->addVertex(positionAAxisV);
				endHEdge[i-1]=theCylinder->addEdge(endHVertex[i-1],endHVertex[i]);
			}
			endHEdge[sh-1]=theCylinder->addEdge(endHVertex[sh-1],endHVertex[sh]);
			
			//ÕýÊ½Éú³ÉÃæ£¬Ê×ÏÈÊÇÉÏÏÂÃæ
            unsigned int LEdge=theCylinder->addEdge(endHVertex[0],startHVertex[0]);
            unsigned int UEdge=theCylinder->addEdge(endHVertex[sh],startHVertex[sh]);
            unsigned int HLEdge=LEdge;
            unsigned int HUEdge=UEdge;
            for(unsigned int i=sr-1;i>0;--i)
			{
                unsigned int tempLEdge=theCylinder->addEdge(endLVertex[i],startLVertex[i]);
                unsigned int tempUEdge=theCylinder->addEdge(endUVertex[i],startUVertex[i]);
				
                unsigned int edgeListL[4]={0};
				edgeListL[3]=tempLEdge;
				edgeListL[2]=endLEdge[i];
				edgeListL[1]=LEdge;
				edgeListL[0]=startLEdge[i];
				theCylinder->addFace(edgeListL,4);

                unsigned int edgeListU[4]={0};
				edgeListU[3]=tempUEdge;
				edgeListU[2]=startUEdge[i];
				edgeListU[1]=UEdge;
				edgeListU[0]=endUEdge[i];
				theCylinder->addFace(edgeListU,4);

				LEdge=tempLEdge;
				UEdge=tempUEdge;
			}
			//²¹È«Èý½ÇÃæ
            unsigned int edgeListL[3]={0};
			edgeListL[2]=endLEdge[0];
			edgeListL[1]=LEdge;
			edgeListL[0]=startLEdge[0];
			theCylinder->addFace(edgeListL,3);

            unsigned int edgeListU[3]={0};
			edgeListU[2]=startUEdge[0];
			edgeListU[1]=UEdge;
			edgeListU[0]=endUEdge[0];
			theCylinder->addFace(edgeListU,3);

			//Éú³É×ÝÏòµÄÃæ
            for(unsigned int i=1;i<sh;i++)
			{
                unsigned int tempHLEdge=theCylinder->addEdge(endHVertex[i],startHVertex[i]);
                unsigned int edgeListH[4]={0};
				edgeListH[3]=tempHLEdge;
				edgeListH[2]=startHEdge[i-1];
				edgeListH[1]=HLEdge;
				edgeListH[0]=endHEdge[i-1];
				theCylinder->addFace(edgeListH,4);
				HLEdge=tempHLEdge;
			}
                unsigned int edgeListH[4]={0};
				edgeListH[3]=HUEdge;
				edgeListH[2]=startHEdge[sh-1];
				edgeListH[1]=HLEdge;
				edgeListH[0]=endHEdge[sh-1];
				theCylinder->addFace(edgeListH,4);
		
			delete startHEdge;
			delete startHVertex;
			delete startUEdge;
			delete startUVertex;
			delete startLEdge;
			delete startLVertex;
			
			startHEdge=endHEdge;
			startHVertex=endHVertex;
			startUEdge=endUEdge;
			startUVertex=endUVertex;
			startLEdge=endLEdge;
			startLVertex=endLVertex;
		}
		
            unsigned int LEdge=theCylinder->addEdge(endHVertexC[0],startHVertex[0]);
            unsigned int UEdge=theCylinder->addEdge(endHVertexC[sh],startHVertex[sh]);
            unsigned int HLEdge=LEdge;
            unsigned int HUEdge=UEdge;
            for(unsigned int i=sr-1;i>0;--i)
			{
                unsigned int tempLEdge=theCylinder->addEdge(endLVertexC[i],startLVertex[i]);
                unsigned int tempUEdge=theCylinder->addEdge(endUVertexC[i],startUVertex[i]);
				
                unsigned int edgeListL[4]={0};
				edgeListL[3]=tempLEdge;
				edgeListL[2]=endLEdgeC[i];
				edgeListL[1]=LEdge;
				edgeListL[0]=startLEdge[i];
				theCylinder->addFace(edgeListL,4);

                unsigned int edgeListU[4]={0};
				edgeListU[3]=tempUEdge;
				edgeListU[2]=startUEdge[i];
				edgeListU[1]=UEdge;
				edgeListU[0]=endUEdgeC[i];
				theCylinder->addFace(edgeListU,4);

				LEdge=tempLEdge;
				UEdge=tempUEdge;
			}
			//²¹È«Èý½ÇÃæ
            unsigned int edgeListL[3]={0};
			edgeListL[2]=endLEdgeC[0];
			edgeListL[1]=LEdge;
			edgeListL[0]=startLEdge[0];
			theCylinder->addFace(edgeListL,3);

            unsigned int edgeListU[3]={0};
			edgeListU[2]=startUEdge[0];
			edgeListU[1]=UEdge;
			edgeListU[0]=endUEdgeC[0];
			theCylinder->addFace(edgeListU,3);

			//Éú³É×ÝÏòµÄÃæ
            for(unsigned int i=1;i<sh;++i)
			{
                unsigned int tempHLEdge=theCylinder->addEdge(endHVertexC[i],startHVertex[i]);
                unsigned int edgeListH[4]={0};
				edgeListH[3]=tempHLEdge;
				edgeListH[2]=startHEdge[i-1];
				edgeListH[1]=HLEdge;
				edgeListH[0]=endHEdgeC[i-1];
				theCylinder->addFace(edgeListH,4);
				HLEdge=tempHLEdge;
			}
                unsigned int edgeListH[4]={0};
				edgeListH[3]=HUEdge;
				edgeListH[2]=startHEdge[sh-1];
				edgeListH[1]=HLEdge;
				edgeListH[0]=endHEdgeC[sh-1];
				theCylinder->addFace(edgeListH,4);
			
			delete startHEdge;
			delete startHVertex;
			delete startUEdge;
			delete startUVertex;
			delete startLEdge;
			delete startLVertex;

			delete endHEdgeC;
			delete endHVertexC;
			delete endUEdgeC;
			delete endUVertexC;
			delete endLEdgeC;
			delete endLVertexC;
	}

}

unsigned int Scene::newSphere(float x,float y,float z,float r,AxisMode::__Enum axis,unsigned int sa,unsigned int sr)
{
	Object *theSphere=new Object("Sphere");
	newSphere(theSphere,x,y,z,r,axis,sa,sr);
	theObjectList.add(theSphere);
	return theSphere->index;
}

void Scene::newSphere(Object *theSphere,float x,float y,float z,float r,AxisMode::__Enum axis,unsigned int sa,unsigned int sr)
{
	theSphere->center.x=x;
	theSphere->center.y=y;
	theSphere->center.z=z;
	//Ê×ÏÈ¼ÆËãÉÏÏÂÁ½¸ö¶¥µã
	Vector positionAAxisV(positionAAxis(axis,x,y,z,0,0,r));
    unsigned int centerU=theSphere->addVertex(positionAAxisV);
	positionAAxisV=positionAAxis(axis,x,y,z,0,0,-r);
    unsigned int centerL=theSphere->addVertex(positionAAxisV);
	//¼ÆËã¼ä¸ô
	float dr=psPI2/sr;
	float da=ps2PI/sa;

    unsigned int *startUEdge=new unsigned int[sr];
    unsigned int *startUVertex=new unsigned int[sr+1];

    unsigned int *startLEdge=new unsigned int[sr];
    unsigned int *startLVertex=new unsigned int[sr+1];

    unsigned int *endUEdgeC=new unsigned int[sr];
    unsigned int *endUVertexC=new unsigned int[sr+1];

    unsigned int *endLEdgeC=new unsigned int[sr];
    unsigned int *endLVertexC=new unsigned int[sr+1];

	startUVertex[0]=endUVertexC[0]=centerU;
	startLVertex[0]=endLVertexC[0]=centerL;

	positionAAxisV=positionAAxis(axis,x,y,z,r,0,0);
	startUVertex[sr]=endUVertexC[sr]=startLVertex[sr]=endLVertexC[sr]=theSphere->addVertex(positionAAxisV);

    for(unsigned int i=1;i<sr;++i)
	{
		positionAAxisV=positionAAxis(axis,x,y,z,r*cos((sr-i)*dr),0,r*sin((sr-i)*dr));
		startUVertex[i]=endUVertexC[i]=theSphere->addVertex(positionAAxisV);
		positionAAxisV=positionAAxis(axis,x,y,z,r*cos((sr-i)*dr),0,-r*sin((sr-i)*dr));
		startLVertex[i]=endLVertexC[i]=theSphere->addVertex(positionAAxisV);
	}
	//½¨Á¢µÚÒ»Ìõ±ß
    for(unsigned int i=0;i<sr;++i)
	{
		startUEdge[i]=endUEdgeC[i]=theSphere->addEdge(startUVertex[i],startUVertex[i+1]);
		startLEdge[i]=endLEdgeC[i]=theSphere->addEdge(startLVertex[i],startLVertex[i+1]);
	}
	//¿ªÊ¼Ñ­»·»æÖÆ
    for(unsigned int e=0;e<sa-1;++e)
	{
        unsigned int *endUEdge=new unsigned int[sr];
        unsigned int *endUVertex=new unsigned int[sr+1];
        unsigned int *endLEdge=new unsigned int[sr];
        unsigned int *endLVertex=new unsigned int[sr+1];
		endUVertex[0]=centerU;
		endLVertex[0]=centerL;
		positionAAxisV=positionAAxis(axis,x,y,z,r*cos((e+1)*da),r*sin((e+1)*da),0);
		endUVertex[sr]=endLVertex[sr]=theSphere->addVertex(positionAAxisV);
	
        for(unsigned int i=1;i<sr;++i)
		{
			positionAAxisV=positionAAxis(axis,x,y,z,r*cos((sr-i)*dr)*cos((e+1)*da),r*cos((sr-i)*dr)*sin((e+1)*da),r*sin((sr-i)*dr));
			endUVertex[i]=theSphere->addVertex(positionAAxisV);
			positionAAxisV=positionAAxis(axis,x,y,z,r*cos((sr-i)*dr)*cos((e+1)*da),r*cos((sr-i)*dr)*sin((e+1)*da),-r*sin((sr-i)*dr));
			endLVertex[i]=theSphere->addVertex(positionAAxisV);
		}

        for(unsigned int i=0;i<sr;++i)
		{
			endUEdge[i]=theSphere->addEdge(endUVertex[i],endUVertex[i+1]);
			endLEdge[i]=theSphere->addEdge(endLVertex[i],endLVertex[i+1]);
		}
		
		//ÕýÊ½Éú³ÉÃæ
        unsigned int middleEdge=theSphere->addEdge(endUVertex[sr],startUVertex[sr]);
        unsigned int upEdge=theSphere->addEdge(endUVertex[1],startUVertex[1]);
        unsigned int lowEdge=theSphere->addEdge(endLVertex[1],startLVertex[1]);
        unsigned int tempUpEdge=upEdge;
        unsigned int tempLowEdge=lowEdge;
        for(unsigned int i=1;i<sr-1;++i)
		{
            unsigned int tempEdgeU=theSphere->addEdge(endUVertex[i+1],startUVertex[i+1]);
            unsigned int edgeListU[4]={0};
			edgeListU[3]=endUEdge[i];
			edgeListU[2]=tempUpEdge;
			edgeListU[1]=startUEdge[i];
			edgeListU[0]=tempEdgeU;
			theSphere->addFace(edgeListU,4);

            unsigned int tempEdgeL=theSphere->addEdge(endLVertex[i+1],startLVertex[i+1]);
            unsigned int edgeListL[4]={0};
			edgeListL[3]=endLEdge[i];
			edgeListL[2]=tempEdgeL;
			edgeListL[1]=startLEdge[i];
			edgeListL[0]=tempLowEdge;
			theSphere->addFace(edgeListL,4);

			tempUpEdge=tempEdgeU;
			tempLowEdge=tempEdgeL;
		}
		
        unsigned int edgeListU[4]={0};
		edgeListU[3]=endUEdge[sr-1];
		edgeListU[2]=tempUpEdge;
		edgeListU[1]=startUEdge[sr-1];
		edgeListU[0]=middleEdge;
		theSphere->addFace(edgeListU,4);

        unsigned int edgeListL[4]={0};
		edgeListL[3]=endLEdge[sr-1];
		edgeListL[2]=middleEdge;
		edgeListL[1]=startLEdge[sr-1];
		edgeListL[0]=tempLowEdge;
		theSphere->addFace(edgeListL,4);
		
		edgeListU[2]=endUEdge[0];
		edgeListU[1]=startUEdge[0];
		edgeListU[0]=upEdge;
		theSphere->addFace(edgeListU,3);

		edgeListL[2]=endLEdge[0];
		edgeListL[1]=lowEdge;
		edgeListL[0]=startLEdge[0];
		theSphere->addFace(edgeListL,3);

		delete startUEdge;
		delete startUVertex;
		delete startLEdge;
		delete startLVertex;
		
		startUEdge=endUEdge;
		startUVertex=endUVertex;
		startLEdge=endLEdge;
		startLVertex=endLVertex;
	}

	//Éú³É×îºóÒ»Ìõ
		
        unsigned int middleEdge=theSphere->addEdge(endUVertexC[sr],startUVertex[sr]);
        unsigned int upEdge=theSphere->addEdge(endUVertexC[1],startUVertex[1]);
        unsigned int lowEdge=theSphere->addEdge(endLVertexC[1],startLVertex[1]);
        unsigned int tempUpEdge=upEdge;
        unsigned int tempLowEdge=lowEdge;
        for(unsigned int i=1;i<sr-1;++i)
		{
            unsigned int tempEdgeU=theSphere->addEdge(endUVertexC[i+1],startUVertex[i+1]);
            unsigned int edgeListU[4]={0};
			edgeListU[3]=endUEdgeC[i];
			edgeListU[2]=tempUpEdge;
			edgeListU[1]=startUEdge[i];
			edgeListU[0]=tempEdgeU;
			theSphere->addFace(edgeListU,4);

            unsigned int tempEdgeL=theSphere->addEdge(endLVertexC[i+1],startLVertex[i+1]);
            unsigned int edgeListL[4]={0};
			edgeListL[3]=endLEdgeC[i];
			edgeListL[2]=tempEdgeL;
			edgeListL[1]=startLEdge[i];
			edgeListL[0]=tempLowEdge;
			theSphere->addFace(edgeListL,4);

			tempUpEdge=tempEdgeU;
			tempLowEdge=tempEdgeL;
		}
		
        unsigned int edgeListU[4]={0};
		edgeListU[3]=endUEdgeC[sr-1];
		edgeListU[2]=tempUpEdge;
		edgeListU[1]=startUEdge[sr-1];
		edgeListU[0]=middleEdge;
		theSphere->addFace(edgeListU,4);

        unsigned int edgeListL[4]={0};
		edgeListL[3]=endLEdgeC[sr-1];
		edgeListL[2]=middleEdge;
		edgeListL[1]=startLEdge[sr-1];
		edgeListL[0]=tempLowEdge;
		theSphere->addFace(edgeListL,4);
		
		edgeListU[2]=endUEdgeC[0];
		edgeListU[1]=startUEdge[0];
		edgeListU[0]=upEdge;
		theSphere->addFace(edgeListU,3);

		edgeListL[2]=endLEdgeC[0];
		edgeListL[1]=lowEdge;
		edgeListL[0]=startLEdge[0];
		theSphere->addFace(edgeListL,3);

		delete startUEdge;
		delete startUVertex;
		delete startLEdge;
		delete startLVertex;
		
		delete endUEdgeC;
		delete endUVertexC;
		delete endLEdgeC;
		delete endLVertexC;
}

void Scene::moveVertex(float x,float y,float z)
{
	//ÕâÀïÓ¦¸Ã¼ÓÉÏÒ»Ð©ºÏ·¨ÐÔµÄÅÐ¶Ï£¬±ÈÈçµ±Ç°µÄÑ¡ÔñÄ£Ê½ÊÇ·ñÊÇ¶¥µãÄ£Ê½¡£
	//Ê×ÏÈ½øÐÐºÏ·¨ÐÔµÄÅÐ¶Ï
	if(mode==SelectionMode::Vertex && target)
	{
		//±éÀúËùÓÐÑ¡ÔñµÄµã
//		unsigned int selectionSize=selection.size();
        for(unsigned int e=0;e<selection.size();++e)
		{
			//µÃµ½µ±Ç°µÄÎ»ÖÃ
		//	Vertex *theVertex=theObjectList[target]->vertex(selection[e]);
			//ÔÚµ±Ç°µÄÎ»ÖÃÉÏÔö¼Ó
			theObjectList[target]->vertexPositionChangeR(selection[e],x,y,z);
		}
	}
}

void Scene::moveEdge(float x,float y,float z)
{
	if(mode==SelectionMode::Edge && target)
	{
		
        unsigned int edgeCount=selection.size();
        for(unsigned int e=0;e<edgeCount;++e)
		{
			Vertex *theVertex=theObjectList[target]->vertex(theObjectList[target]->edge(selection[e])->end);
			if(!theVertex->isIn)
			{
				theVertex->isIn=true;
				isInCache.push_back(theVertex);
				theObjectList[target]->vertexPositionChangeR(theVertex->index,x,y,z);
			}
			theVertex=theObjectList[target]->vertex(theObjectList[target]->edge(selection[e])->start);
			if(!theVertex->isIn)
			{
				theVertex->isIn=true;
				isInCache.push_back(theVertex);
				theObjectList[target]->vertexPositionChangeR(theVertex->index,x,y,z);
			}
		}
		//ÕâÀïÒªÇå¿ÕisInCache
		clearIsInCache();
	}
}

void Scene::moveFace(float x,float y,float z)
{
	//Ê×ÏÈÒªµÃµ½±»ÒÆ¶¯µÄµã¼¯
	if(mode==SelectionMode::Face && target)
	{
		
        unsigned int selectionCount=selection.size();
        for(unsigned int i=0;i<selectionCount;++i)
		{
			Face *theFace=theObjectList[target]->face(selection[i]);
            unsigned int edgeCount=theFace->edge.size();
            for(unsigned int e=0;e<edgeCount;++e)
			{
				if(theFace->edge[e]>0)
				{
					if(!theObjectList[target]->vertex(theObjectList[target]->edge(theFace->edge[e])->end)->isIn)
					{
						Vertex *theVertex=theObjectList[target]->vertex(theObjectList[target]->edge(theFace->edge[e])->end);
						theVertex->isIn=true;
						isInCache.push_back(theVertex);
						theObjectList[target]->vertexPositionChangeR(theVertex->index,x,y,z);
					}
				}
				else
				{
					if(!theObjectList[target]->vertex(theObjectList[target]->edge(-theFace->edge[e])->start)->isIn)
					{
						Vertex *theVertex=theObjectList[target]->vertex(theObjectList[target]->edge(-theFace->edge[e])->start);
						theVertex->isIn=true;
						isInCache.push_back(theVertex);
						theObjectList[target]->vertexPositionChangeR(theVertex->index,x,y,z);
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
	Edge *theEdge=theObjectList[t]->edge(edgeID);
	Vertex *theStart=theObjectList[t]->vertex(theEdge->start);
    unsigned int adjEdgeCount=theStart->m_adjacentEdgeList.size();
    for(unsigned int e=0;e<adjEdgeCount;++e)
	{
        if(theStart->m_adjacentEdgeList[e]==(int)theEdge->index)
		{
			theObjectList[t]->vertexAdjacentRemove(theStart->index,e);
			break;
		}
	}
    if(theStart->m_adjacentEdgeList.empty())
	{
		theObjectList[t]->objectVertexRemove(theStart->index);
	}
	Vertex *theEnd=theObjectList[t]->vertex(theEdge->end);
    adjEdgeCount=theEnd->m_adjacentEdgeList.size();
    for(unsigned int e=0;e<adjEdgeCount;++e)
	{
        if(theEnd->m_adjacentEdgeList[e]==(int)theEdge->index)
		{
			theObjectList[t]->vertexAdjacentRemove(theEnd->index,e);
			break;
		}
	}
    if(theEnd->m_adjacentEdgeList.empty())
	{
		theObjectList[t]->objectVertexRemove(theEnd->index);
	}
	theObjectList[t]->objectEdgeRemove(edgeID);
}

void Scene::deleteVertex()
{
	if(mode==SelectionMode::Vertex && target)
	{
        unsigned int vertexCount=selection.size();
        for(unsigned int i=0;i<vertexCount;++i)
		{
			//Ê×ÏÈµÃµ½ÁÚ½ÓµÄÃæ
			Vertex *currentVertex=theObjectList[target]->vertex(selection[i]);
            unsigned int edgeCount=currentVertex->m_adjacentEdgeList.size();
            for(unsigned int e=0;e<edgeCount;++e)
			{
                if(theObjectList[target]->edge(currentVertex->m_adjacentEdgeList[e])->end==selection[i])
				{
                    Face *currentFace=theObjectList[target]->face(theObjectList[target]->edge(currentVertex->m_adjacentEdgeList[e])->right);
					if(currentFace==NULL) 
						continue;
                    unsigned int edgeSize=currentFace->edge.size();
                    for(unsigned int h=0;h<edgeSize;++h)
					{
						if(currentFace->edge[h]>0)
						{
							theObjectList[target]->edgeRightChange(currentFace->edge[h],0);
						}
						else
						{
							theObjectList[target]->edgeLeftChange(-currentFace->edge[h],0);
						}
					}
					theObjectList[target]->objectFaceRemove(currentFace->index);
				}
				else
                if(theObjectList[target]->edge(currentVertex->m_adjacentEdgeList[e])->start==selection[i])
				{
                    Face *currentFace=theObjectList[target]->face(theObjectList[target]->edge(currentVertex->m_adjacentEdgeList[e])->left);
					if(currentFace==NULL) 
						continue;
                    unsigned int edgeSize=currentFace->edge.size();
                    for(unsigned int h=0;h<edgeSize;++h)
					{
						if(currentFace->edge[h]>0)
						{
							theObjectList[target]->edgeRightChange(currentFace->edge[h],0);
						}
						else
						{
							theObjectList[target]->edgeLeftChange(-currentFace->edge[h],0);
						}
					}
					theObjectList[target]->objectFaceRemove(currentFace->index);
				}
			}
			for(int e=edgeCount-1;e>-1;--e)
			{
                Edge *theEdge=theObjectList[target]->edge(currentVertex->m_adjacentEdgeList[e]);
				deleteEdgeH(target,theEdge->index);
			}
		}
		clearSelection();
		theObjectList[target]->clearPSCache();
		updateAxisCursor();
	}
}

void Scene::deleteFace()
{
	if(mode==SelectionMode::Face && target)
	{
		//Ê×ÏÈµÃµ½ÒªÉ¾³ýµÄÃæÊý
        unsigned int faceCount=selection.size();
        for(unsigned int i=0;i<faceCount;++i)
		{
			Face *theFace=theObjectList[target]->face(selection[i]);
            unsigned int edgeCount=theFace->edge.size();
            for(unsigned int e=0;e<edgeCount;++e)
			{
				if(theFace->edge[e]>0)
				{
					theObjectList[target]->edgeRightChange(theFace->edge[e],0);
					if(theObjectList[target]->edge(theFace->edge[e])->left==0)
					{
						deleteEdgeH(target,theFace->edge[e]);
					}
				}
				else
				{
					theObjectList[target]->edgeLeftChange(-theFace->edge[e],0);
					if(theObjectList[target]->edge(-theFace->edge[e])->right==0)
					{
						deleteEdgeH(target,-theFace->edge[e]);
					}
				}
			}
			theObjectList[target]->objectFaceRemove(theFace->index);
		}
	}
}

void Scene::extrudeEdge(float x,float y,float z)
{
	if(mode==SelectionMode::Edge && target)
	{
        std::vector<unsigned int> newSelection;
        unsigned int edgeCount=selection.size();
		newSelection.reserve(edgeCount);
        for(unsigned int e=0;e<edgeCount;++e)
		{
			Edge *theEdge=theObjectList[target]->edge(selection[e]);
			if(theEdge->left==0 || theEdge->right==0)
			{
				Vertex *theEnd=theObjectList[target]->vertex(theEdge->end);
				if(!theEnd->isIn)
				{
                    theEnd->m_clone=theObjectList[target]->addVertex(theEnd->m_position.x+x,theEnd->m_position.y+y,theEnd->m_position.z+z);
                    theObjectList[target]->vertex(theEnd->m_clone)->m_clone=theObjectList[target]->addEdge(theEnd->index,theEnd->m_clone);
					theEnd->isIn=true;
					isInCache.push_back(theEnd);
				}
				Vertex *theStart=theObjectList[target]->vertex(theEdge->start);
				if(!theStart->isIn)
				{
                    theStart->m_clone=theObjectList[target]->addVertex(theStart->m_position.x+x,theStart->m_position.y+y,theStart->m_position.z+z);
                    theObjectList[target]->vertex(theStart->m_clone)->m_clone=theObjectList[target]->addEdge(theStart->index,theStart->m_clone);
					theStart->isIn=true;
                    isInCache.push_back(theStart);
				}
				//Éú³ÉÃæ
                unsigned int tempEdge[4]={0};
				if(theEdge->right==0)
				{
					tempEdge[0]=theEdge->index;
                    tempEdge[1]=theObjectList[target]->vertex(theEnd->m_clone)->m_clone;
                    tempEdge[2]=theObjectList[target]->addEdge(theEnd->m_clone,theStart->m_clone);
					newSelection.push_back(tempEdge[2]);
                    tempEdge[3]=theObjectList[target]->vertex(theStart->m_clone)->m_clone;
				}
				else
				{
					tempEdge[0]=theEdge->index;
                    tempEdge[1]=theObjectList[target]->vertex(theStart->m_clone)->m_clone;
                    tempEdge[2]=theObjectList[target]->addEdge(theEnd->m_clone,theStart->m_clone);
					newSelection.push_back(tempEdge[2]);
                    tempEdge[3]=theObjectList[target]->vertex(theEnd->m_clone)->m_clone;				}
				theObjectList[target]->addFace(tempEdge,4);
			}
		}
		clearIsInCache();

		clearSelection();
        for(unsigned int i=0;i<newSelection.size();++i)
		{
			selectionPush(theObjectList[target]->edge(newSelection[i]));
		}
	}
}

unsigned int Scene::insertVertex(int edgeID,float pos)
{
	Edge *theEdge=theObjectList[target]->edge(edgeID);
	Vertex *theEnd=theObjectList[target]->vertex(theEdge->end);
	Vertex *theStart=theObjectList[target]->vertex(theEdge->start);
    unsigned int newVertex=theObjectList[target]->addVertex(theStart->m_position.x+(theEnd->m_position.x-theStart->m_position.x)*pos,theStart->m_position.y+(theEnd->m_position.y-theStart->m_position.y)*pos,theStart->m_position.z+(theEnd->m_position.z-theStart->m_position.z)*pos);
    unsigned int right=theEdge->right;
    unsigned int left=theEdge->left;
    unsigned int edgeCount=theEnd->m_adjacentEdgeList.size();
    unsigned int removedEdge=theEdge->index;
	deleteEdgeH(target,removedEdge);
    unsigned int edgeS=theObjectList[target]->addEdge(theStart->index,newVertex);
    unsigned int edgeE=theObjectList[target]->addEdge(newVertex,theEnd->index);

	Face *theFace=theObjectList[target]->face(right);
	if(theFace)
	{
		edgeCount=theFace->edge.size();
        unsigned int e;
		for(e=0;e<edgeCount;++e)
		{
			if(theFace->edge[e]==(int)removedEdge)
			{
				theObjectList[target]->faceEdgeChange(theFace->index,e,edgeS);
				break;
			}
		}
		theObjectList[target]->faceEdgeInsert(theFace->index,e+1,edgeE);
		theObjectList[target]->edgeRightChange(edgeE,right);
		theObjectList[target]->edgeRightChange(edgeS,right);
	}

	theFace=theObjectList[target]->face(left);
	if(theFace)
	{
		edgeCount=theFace->edge.size();
        unsigned int e;
		for(e=0;e<edgeCount;++e)
		{
			if(theFace->edge[e]==-((int)removedEdge))
			{
				theObjectList[target]->faceEdgeChange(theFace->index,e,-((int)edgeE));
				break;
			}
		}
		theObjectList[target]->faceEdgeInsert(theFace->index,e+1,-((int)edgeS));
		theObjectList[target]->edgeLeftChange(edgeE,left);
		theObjectList[target]->edgeLeftChange(edgeS,left);
	}

	return newVertex;
}

void Scene::split(int startV,int endV)
{
	//printf("begin\n");
	Vertex *theStart=theObjectList[target]->vertex(startV);
	Vertex *theEnd=theObjectList[target]->vertex(endV);
    unsigned int edgeCount=theStart->m_adjacentEdgeList.size();
    unsigned int e;
	Face *theFace=NULL;
	for(e=0;e<edgeCount;++e)
	{
        if(theObjectList[target]->edge(theStart->m_adjacentEdgeList[e])->start==theEnd->index || theObjectList[target]->edge(theStart->m_adjacentEdgeList[e])->end==theEnd->index)
		{
			//±éÀústartµã£¬¿´ÊÇ²»ÊÇºÍendµãÖ»ÓÐÒ»¸ö±ßÏàÁ¬
			return;
		}
        if(theObjectList[target]->edge(theStart->m_adjacentEdgeList[e])->left && !theObjectList[target]->face(theObjectList[target]->edge(theStart->m_adjacentEdgeList[e])->left)->isIn)
		{
            theFace=theObjectList[target]->face(theObjectList[target]->edge(theStart->m_adjacentEdgeList[e])->left);
			theFace->isIn=true;
			isInCache.push_back(theFace);
		}
	//	else
        if(theObjectList[target]->edge(theStart->m_adjacentEdgeList[e])->right && !theObjectList[target]->face(theObjectList[target]->edge(theStart->m_adjacentEdgeList[e])->right)->isIn)
		{
            theFace=theObjectList[target]->face(theObjectList[target]->edge(theStart->m_adjacentEdgeList[e])->right);
			theFace->isIn=true;			
			isInCache.push_back(theFace);
		}
	}
    edgeCount=theEnd->m_adjacentEdgeList.size();
	//Face *theFace=NULL;
	for(e=0;e<edgeCount;++e)
	{
        if(theObjectList[target]->edge(theEnd->m_adjacentEdgeList[e])->left && theObjectList[target]->face(theObjectList[target]->edge(theEnd->m_adjacentEdgeList[e])->left)->isIn)
		{
            theFace=theObjectList[target]->face(theObjectList[target]->edge(theEnd->m_adjacentEdgeList[e])->left);
			break;
		}
	//	else
        if(theObjectList[target]->edge(theEnd->m_adjacentEdgeList[e])->right && theObjectList[target]->face(theObjectList[target]->edge(theEnd->m_adjacentEdgeList[e])->right)->isIn)
		{
            theFace=theObjectList[target]->face(theObjectList[target]->edge(theEnd->m_adjacentEdgeList[e])->right);
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
    unsigned int newEdge=theObjectList[target]->addEdge(theStart->index,theEnd->index);
	//printf("begin2.5\n");
	edgeCount=theFace->edge.size();
    unsigned int edgeS[2]={0};
    unsigned int edgeE[2]={0};
    for(unsigned int i=0;i<edgeCount;++i)
	{
		if(theFace->edge[i]>0)
		{
			if(theObjectList[target]->edge(theFace->edge[i])->end==theStart->index)
			{
				edgeS[0]=i;
				edgeS[1]=(i+1)%edgeCount;
			}
			if(theObjectList[target]->edge(theFace->edge[i])->end==theEnd->index)
			{
				edgeE[0]=i;
				edgeE[1]=(i+1)%edgeCount;				
			}
		}
		else
		{
			if(theObjectList[target]->edge(-theFace->edge[i])->start==theStart->index)
			{
				edgeS[0]=i;
				edgeS[1]=(i+1)%edgeCount;
			}
			if(theObjectList[target]->edge(-theFace->edge[i])->start==theEnd->index)
			{
				edgeE[0]=i;
				edgeE[1]=(i+1)%edgeCount;
			}
		}
	}
	
	//printf("[%d]",theFace->index);
	//µÃµ½theStartËùÔÚµÄµÚÒ»¸ö±ß
	//Éú³ÉÁ½¸öÐÂµÄÃæ
    unsigned int *tempFace=new unsigned int[theFace->edge.size()*2];
    unsigned int b=edgeS[1];
    unsigned int count=0;
	while(b!=edgeE[1])
	{
		if(theFace->edge[b]>0)
		{
			theObjectList[target]->edgeRightChange(theFace->edge[b],0);
			tempFace[count]=theFace->edge[b];
			++count;
		}
		else
		{
			theObjectList[target]->edgeLeftChange(-theFace->edge[b],0);
			tempFace[count]=-theFace->edge[b];
			++count;
		}
		++b;
		b=b%theFace->edge.size();
	}
	tempFace[count]=newEdge;
	//printf("count:%d,size:%d\n",count+1,theFace->edge.size()*2);
	theObjectList[target]->addFace(tempFace,count+1);
	delete tempFace;
    tempFace=new unsigned int[theFace->edge.size()*2];
	//printf("begin3\n");
	b=edgeE[1];
	count=0;
	while(b!=edgeS[1])
	{
		if(theFace->edge[b]>0)
		{
			theObjectList[target]->edgeRightChange(theFace->edge[b],0);
			tempFace[count]=theFace->edge[b];
			++count;
		}
		else
		{
			theObjectList[target]->edgeLeftChange(-theFace->edge[b],0);
			tempFace[count]=-theFace->edge[b];
			++count;
		}
		++b;
		b=b%theFace->edge.size();
	}
	tempFace[count]=newEdge;
//	for(unsigned int i=0;i<count+1;i++)
//	{
		//printf("{%d}\n",tempFace[i]);
	//}
	theObjectList[target]->addFace(tempFace,count+1);
	//printf("begin4\n");
	//printf("[%d]",theFace->index);

    unsigned int fi=theFace->index;
	theFace=NULL;
	theObjectList[target]->objectFaceRemove(fi);
	delete tempFace;
}

void Scene::removeEdge()
{
	if(mode==SelectionMode::Edge && target){
	//±éÀúËùÓÐµÄ±ß
    unsigned int removedEdgeCount=selection.size();
    for(unsigned int i=0;i<removedEdgeCount;++i)
	{
		Edge *theEdge=theObjectList[target]->edge(selection[i]);
		if(theEdge==NULL)
		{
			continue;
		}
		//ÕâÀïÅÐ¶ÏÊÇ±ßÔµµÄ±ß»¹ÊÇÄÚ²¿µÄ±ß
		if(theEdge->left==0)
		{
			//±£Ö¤Ã¿¸ö±ß±ØÓÐÁÚ½ÓµÄÃæ
			Face *theFace=theObjectList[target]->face(theEdge->right);
            unsigned int adjEdgeCount=theFace->edge.size();
            for(unsigned int e=0;e<adjEdgeCount;++e)
			{
				if(theFace->edge[e]>0)
				{
					theObjectList[target]->edgeRightChange(theFace->edge[e],0);
					if(!theObjectList[target]->edge(theFace->edge[e])->left)
					{
						deleteEdgeH(target,theFace->edge[e]);
					}
				}
				else
				{
					theObjectList[target]->edgeLeftChange(-theFace->edge[e],0);
					if(!theObjectList[target]->edge(-theFace->edge[e])->right)
					{
						deleteEdgeH(target,-theFace->edge[e]);
					}
				}
			}
			theObjectList[target]->objectFaceRemove(theFace->index);
		}
		else
		if(theEdge->right==0)
		{
			Face *theFace=theObjectList[target]->face(theEdge->left);
            unsigned int adjEdgeCount=theFace->edge.size();
            for(unsigned int e=0;e<adjEdgeCount;++e)
			{
				if(theFace->edge[e]>0)
				{
					theObjectList[target]->edgeRightChange(theFace->edge[e],0);
					if(!theObjectList[target]->edge(theFace->edge[e])->left)
					{
						deleteEdgeH(target,theFace->edge[e]);
					}
				}
				else
				{
					theObjectList[target]->edgeLeftChange(-theFace->edge[e],0);
					if(!theObjectList[target]->edge(-theFace->edge[e])->right)
					{
						deleteEdgeH(target,-theFace->edge[e]);
					}
				}
			}
			theObjectList[target]->objectFaceRemove(theFace->index);
		}
		else
		{
			Face *theLeft=theObjectList[target]->face(theEdge->left);
			Face *theRight=theObjectList[target]->face(theEdge->right);
			//Ê×ÏÈÔÚÓÒ²àµÄÃæÀïÃæÕÒµ½ÒªÉ¾³ýµÄÕâ¸ö±ß
            unsigned int adjEdgeCount=theRight->edge.size();
            unsigned int e=0;
			for(e=0;e<adjEdgeCount;++e)
			{
				if(theRight->edge[e]>0)
				{
                    if(theEdge->index==(unsigned int)theRight->edge[e])
						break;
				}
				else
				{
                    if(theEdge->index==(unsigned int)(-theRight->edge[e]))
						break;
				}
			}
			Vertex *theStart=theObjectList[target]->vertex(theEdge->start);
			Vertex *theEnd=theObjectList[target]->vertex(theEdge->end);
			deleteEdgeH(target,theEdge->index);
            while(theStart->m_adjacentEdgeList.size()==1)
			{
                unsigned int next=0;
                if(theStart->index==theObjectList[target]->edge(theStart->m_adjacentEdgeList[0])->end)
				{
                    next=theObjectList[target]->edge(theStart->m_adjacentEdgeList[0])->start;
				}
				else
				{
                    next=theObjectList[target]->edge(theStart->m_adjacentEdgeList[0])->end;
				}
                deleteEdgeH(target,theStart->m_adjacentEdgeList[0]);
				theStart=theObjectList[target]->vertex(next);
			}

            while(theEnd->m_adjacentEdgeList.size()==1)
			{
                unsigned int next=0;
                if(theEnd->index==theObjectList[target]->edge(theEnd->m_adjacentEdgeList[0])->end)
				{
                    next=theObjectList[target]->edge(theEnd->m_adjacentEdgeList[0])->start;
				}
				else
				{
                    next=theObjectList[target]->edge(theEnd->m_adjacentEdgeList[0])->end;
				}
                deleteEdgeH(target,theEnd->m_adjacentEdgeList[0]);
				theEnd=theObjectList[target]->vertex(next);
			}
			//¿ªÊ¼Éú³ÉÐÂµÄÃæÁË;
            unsigned int sEdgeR=0;
            unsigned int eEdgeR=0;
            unsigned int edgeCount=theRight->edge.size();
            for(unsigned int e=0;e<edgeCount;++e)
			{
				if(theRight->edge[e]>0)
				{
					if(theObjectList[target]->edge(theRight->edge[e]))
					{
						if(theObjectList[target]->edge(theRight->edge[e])->start==theEnd->index)
						{
							sEdgeR=e;
						}
						if(theObjectList[target]->edge(theRight->edge[e])->end==theStart->index)
						{
							eEdgeR=e;
						}
					}
				}
				else
				{
					if(theObjectList[target]->edge(-theRight->edge[e]))
					{
						if(theObjectList[target]->edge(-theRight->edge[e])->end==theEnd->index)
						{
							sEdgeR=e;
						}
						if(theObjectList[target]->edge(-theRight->edge[e])->start==theStart->index)
						{
							eEdgeR=e;
						}
					}
				}
			}

            unsigned int sEdgeL=0;
            unsigned int eEdgeL=0;
			edgeCount=theLeft->edge.size();
            for(unsigned int e=0;e<edgeCount;++e)
			{
				if(theLeft->edge[e]>0)
				{
					if(theObjectList[target]->edge(theLeft->edge[e]))
					{
						if(theObjectList[target]->edge(theLeft->edge[e])->start==theStart->index)
						{
							sEdgeL=e;
						}
						if(theObjectList[target]->edge(theLeft->edge[e])->end==theEnd->index)
						{
							eEdgeL=e;
						}
					}
				}
				else
				{
					if(theObjectList[target]->edge(-theLeft->edge[e]))
					{
						if(theObjectList[target]->edge(-theLeft->edge[e])->end==theStart->index)
						{
							sEdgeL=e;
						}
						if(theObjectList[target]->edge(-theLeft->edge[e])->start==theEnd->index)
						{
							eEdgeL=e;
						}
					}
				}
			}
			//ÕýÊ½Éú³ÉÁ½¸öÐÂµÄÃæ
            unsigned int b=sEdgeR;
            unsigned int count=0;
            unsigned int *tempEdge=new unsigned int[theRight->edge.size()+theLeft->edge.size()];
			while(b!=eEdgeR)
			{
				tempEdge[count]=theRight->edge[b]>0?theRight->edge[b]:-theRight->edge[b];
				++count;
				++b;
				b=b%theRight->edge.size();
			}
			tempEdge[count]=theRight->edge[eEdgeR]>0?theRight->edge[eEdgeR]:-theRight->edge[eEdgeR];
			++count;
			b=sEdgeL;
			while(b!=eEdgeL)
			{
				tempEdge[count]=theLeft->edge[b]>0?theLeft->edge[b]:-theLeft->edge[b];
				++count;
				++b;
				b=b%theLeft->edge.size();
			}
			//É¾³ýÔ­À´µÄÁ½¸öÃæ
			tempEdge[count]=theLeft->edge[eEdgeL]>0?theLeft->edge[eEdgeL]:-theLeft->edge[eEdgeL];
			++count;
			theObjectList[target]->addFace(tempEdge,count);
			theObjectList[target]->objectFaceRemove(theLeft->index);
			theObjectList[target]->objectFaceRemove(theRight->index);
			delete tempEdge;
		}
	}}
}

void Scene::clearIsInCache()
{
    unsigned int cacheSize=isInCache.size();
    for(unsigned int i=0;i<cacheSize;++i)
	{
		if(isInCache[i])
		{
			isInCache[i]->isIn=false;
		}
	}
	isInCache.clear();
}

void Scene::targetWeldVertex(unsigned int vertexA,unsigned int vertexB)
{
    unsigned int mode=0;
	Vertex *theOriginal=theObjectList[target]->vertex(vertexA);
	Vertex *theTarget=theObjectList[target]->vertex(vertexB);
    unsigned int edgeCount=theOriginal->m_adjacentEdgeList.size();
	std::vector<Edge *> OAdjEdge;
	OAdjEdge.reserve(10);
	std::vector<Vertex *> OEnd;
	OEnd.reserve(10);
	//Ê×ÏÈÒª±éÀúµÚÒ»¸öµãµÄÁÚ½Ó±ß£¬Òª¼ì²âÕâÁ½¸öµãµÄÁÚ½Ó±ß£¬ÅÐ¶ÏÊÇ·ñÊÇ±ßÔµ±ß£¬ÊÇ·ñÓÐ¹²µãµÈ
    unsigned int e=0;
	bool isEdge=false;
	for(e=0;e<edgeCount;++e)
	{
        Edge *theEdge=theObjectList[target]->edge(theOriginal->m_adjacentEdgeList[e]);
		OAdjEdge.push_back(theEdge);
		if(!theEdge->left || !theEdge->right)
		{
			isEdge=true;
		}
		if(theEdge->left)
		{
			theObjectList[target]->face(theEdge->left)->isIn=true;
			isInCache.push_back(theObjectList[target]->face(theEdge->left));
		}
		if(theEdge->right)
		{
			theObjectList[target]->face(theEdge->right)->isIn=true;
			isInCache.push_back(theObjectList[target]->face(theEdge->right));
		}
		if(theEdge->start==theOriginal->index)
		{
			if(theEdge->end==theTarget->index)
			{
				//ÕâËµÃ÷º¸½ÓµÄÁ½¸öµãÔÚÍ¬Ò»¸ö±ß
				mode=1;
				break;
			}
			Vertex *theVertex=theObjectList[target]->vertex(theEdge->end);
			OEnd.push_back(theVertex);
			theVertex->isIn=true;
			isInCache.push_back(theVertex);
		}
		else
		{
			if(theEdge->start==theTarget->index)
			{
				//ËµÃ÷º¸½ÓµÄÁ½¸öµãÔÚÍ¬Ò»¸ö±ßÉÏ
				mode=1;
				break;
			}
			Vertex *theVertex=theObjectList[target]->vertex(theEdge->start);
			OEnd.push_back(theVertex);
			theVertex->isIn=true;
			isInCache.push_back(theVertex);
		}
	}

	if(mode==1)
	{
		clearIsInCache();
        Edge *theEdge=theObjectList[target]->edge(theOriginal->m_adjacentEdgeList[e]);
		Vertex *theStart=NULL;
		Vertex *theEnd=NULL;
        unsigned int eIndex=theEdge->index;
        unsigned int right=theEdge->right;
        unsigned int left=theEdge->left;
		if(theEdge->start==vertexB)
		{
			theStart=theObjectList[target]->vertex(theEdge->start);
			theEnd=theObjectList[target]->vertex(theEdge->end);
		}
		else
		{
			theStart=theObjectList[target]->vertex(theEdge->end);
			theEnd=theObjectList[target]->vertex(theEdge->start);
		}
		deleteEdgeH(target,theEdge->index);
        unsigned int edgeCount=theEnd->m_adjacentEdgeList.size();
        for(unsigned int e=0;e<edgeCount;++e)
		{
            theObjectList[target]->vertexAdjacentPush(theStart->index,theEnd->m_adjacentEdgeList[e]);
			//¸üÐÂÕâÐ©±ßµÄ¶Ëµã
            if(theObjectList[target]->edge(theEnd->m_adjacentEdgeList[e])->end==theEnd->index)
			{
                theObjectList[target]->edgeEndChange(theEnd->m_adjacentEdgeList[e],theStart->index);
			}
			else
			{
                theObjectList[target]->edgeStartChange(theEnd->m_adjacentEdgeList[e],theStart->index);
			}
		}
		theObjectList[target]->objectVertexRemove(theEnd->index);
		if(left>0)
		{
			Face  *theFace=theObjectList[target]->face(left);
            unsigned int edgeCount=theFace->edge.size();
            for(unsigned int i=0;i<edgeCount;++i)
			{
				if(theFace->edge[i]==-((int)eIndex))
				{
					theObjectList[target]->faceEdgeRemove(theFace->index,i);
					break;
				}
				
			}
		}

		if(right>0)
		{
			
			Face  *theFace=theObjectList[target]->face(right);
            unsigned int edgeCount=theFace->edge.size();
            for(unsigned int i=0;i<edgeCount;++i)
			{
				if(theFace->edge[i]==(int)eIndex)
				{
					theObjectList[target]->faceEdgeRemove(theFace->index,i);
					break;
				}
			}
		}
		clearDualEdge(target,theTarget->index);
		return;
	}

	//ÏÖÔÚÊ×ÏÈ¼ìÑéµÚÒ»¸öµãÊÇ·ñÊÇ±ßÔµÉÏµÄ
	if(isEdge)
	{
		isEdge=false;
        unsigned int edgeCount=theTarget->m_adjacentEdgeList.size();
		Edge *edgeS[2]={NULL};
		Edge *edgeE[2]={NULL};
        unsigned int adjEdgeNum=0;
        for(unsigned int e=0;e<edgeCount;++e)
		{
            if(!theObjectList[target]->edge(theTarget->m_adjacentEdgeList[e])->left || !theObjectList[target]->edge(theTarget->m_adjacentEdgeList[e])->right)
			{
                if(theObjectList[target]->edge(theTarget->m_adjacentEdgeList[e])->left)
				{
                    if(theObjectList[target]->face(theObjectList[target]->edge(theTarget->m_adjacentEdgeList[e])->left)->isIn)
					{
						//ËµÃ÷¹²Ãæ
						return;
					}
				}
                if(theObjectList[target]->edge(theTarget->m_adjacentEdgeList[e])->right)
				{
                    if(theObjectList[target]->face(theObjectList[target]->edge(theTarget->m_adjacentEdgeList[e])->right)->isIn)
					{
						//ËµÃ÷¹²Ãæ
						return;
					}
				}
				//Èç¹ûÊÇ±ßÔµµÄ±ß
				isEdge=true;
                if(theObjectList[target]->edge(theTarget->m_adjacentEdgeList[e])->end==theTarget->index)
				{
					//ÅÐ¶ÏÊÇ²»ÊÇÒ»¼¶ÁÚ½Ó
                    if(theObjectList[target]->vertex(theObjectList[target]->edge(theTarget->m_adjacentEdgeList[e])->start)->isIn)
					{
						//¼ÇÂ¼ÁÚ½Ó±ß
                        edgeE[adjEdgeNum]=theObjectList[target]->edge(theTarget->m_adjacentEdgeList[e]);
                        unsigned int OEndCount=OEnd.size();
                        for(unsigned int h=0;h<OEndCount;++h)
						{
                            if(OEnd[h]->index==theObjectList[target]->edge(theTarget->m_adjacentEdgeList[e])->start)
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
                    if(theObjectList[target]->vertex(theObjectList[target]->edge(theTarget->m_adjacentEdgeList[e])->end)->isIn)
					{
                        edgeE[adjEdgeNum]=theObjectList[target]->edge(theTarget->m_adjacentEdgeList[e]);
                        unsigned int OEndCount=OEnd.size();
                        for(unsigned int h=0;h<OEndCount;++h)
						{
                            if(OEnd[h]->index==theObjectList[target]->edge(theTarget->m_adjacentEdgeList[e])->end)
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
                unsigned int edgeCount=theOriginal->m_adjacentEdgeList.size();
                for(unsigned int h=0;h<edgeCount;++h)
				{
                    Edge *theEdge=theObjectList[target]->edge(theOriginal->m_adjacentEdgeList[h]);
					theObjectList[target]->vertexAdjacentPush(theTarget->index,theEdge->index);
					if(theEdge->start==theOriginal->index)
					{
						theObjectList[target]->edgeStartChange(theEdge->index,theTarget->index);
					}
					else
					{
						theObjectList[target]->edgeEndChange(theEdge->index,theTarget->index);
					}
				}
				theObjectList[target]->objectVertexRemove(theOriginal->index);
			}
			else
			if(adjEdgeNum>0)
			{
                unsigned int wMode[2]={0};
                for(unsigned int w=0;w<adjEdgeNum;++w)
				{
					if(edgeS[w]->end==edgeE[w]->start)
					{
						if(edgeS[w]->right && edgeE[w]->right)
						{
							wMode[w]=1;
						}
						else
						if(edgeS[w]->left && edgeE[w]->left)
						{
							wMode[w]=2;
						}
						else
						{
							
							return;
						}
					}
					else
					if(edgeS[w]->end==edgeE[w]->end)
					{
						if(edgeS[w]->right && edgeE[w]->left)
						{
							wMode[w]=3;
						}
						else
						if(edgeS[w]->left && edgeE[w]->right)
						{
							wMode[w]=4;
						}
						else
						{
							return;
						}
					}
					else
					if(edgeS[w]->start==edgeE[w]->start)
					{
						if(edgeS[w]->left && edgeE[w]->right)
						{
							wMode[w]=5;
						}
						else
						if(edgeS[w]->right && edgeE[w]->left)
						{
							wMode[w]=6;
						}
						else
						{
							return;
						}
					}
					else
					if(edgeS[w]->start==edgeE[w]->end)
					{
						if(edgeS[w]->left && edgeE[w]->left)
						{
							wMode[w]=7;
						}
						else
						if(edgeS[w]->right && edgeE[w]->right)
						{
							wMode[w]=8;
						}
						else
						{
							return;
						}
					}
				}
                unsigned int theOriginalIndex=theOriginal->index;
                for(unsigned int w=0;w<adjEdgeNum;w++)
				{
					if(wMode[w]==1)
					{
						Face *theF=theObjectList[target]->face(edgeS[w]->right);
                        unsigned int edgeCount=theF->edge.size();
                        for(unsigned int h=0;h<edgeCount;++h)
						{
							if(theF->edge[h]==(int)(edgeS[w]->index))
							{
								theObjectList[target]->faceEdgeChange(theF->index,h,-(int)(edgeE[w]->index));
								theObjectList[target]->edgeLeftChange(edgeE[w]->index,theF->index);
								deleteEdgeH(target,edgeS[w]->index);
								break;
							}
						}
					}
					else
					if(wMode[w]==2)
					{
						Face *theF=theObjectList[target]->face(edgeS[w]->left);
                        unsigned int edgeCount=theF->edge.size();
                        for(unsigned int h=0;h<edgeCount;++h)
						{
							if(theF->edge[h]==-((int)edgeS[w]->index))
							{
								theObjectList[target]->faceEdgeChange(theF->index,h,edgeE[w]->index);
								theObjectList[target]->edgeRightChange(edgeE[w]->index,theF->index);
								deleteEdgeH(target,edgeS[w]->index);
								break;
							}
						}
					}
					else
					if(wMode[w]==3)
					{
						Face *theF=theObjectList[target]->face(edgeS[w]->right);
                        unsigned int edgeCount=theF->edge.size();
                        for(unsigned int h=0;h<edgeCount;++h)
						{
							if(theF->edge[h]==(int)edgeS[w]->index)
							{
								theObjectList[target]->faceEdgeChange(theF->index,h,edgeE[w]->index);
								theObjectList[target]->edgeRightChange(edgeE[w]->index,theF->index);
								deleteEdgeH(target,edgeS[w]->index);
								break;
							}
						}
					}
					else
					if(wMode[w]==4)
					{
						Face *theF=theObjectList[target]->face(edgeS[w]->left);
                        unsigned int edgeCount=theF->edge.size();
                        for(unsigned int h=0;h<edgeCount;++h)
						{
							if(theF->edge[h]==-((int)edgeS[w]->index))
							{
								theObjectList[target]->faceEdgeChange(theF->index,h,-((int)edgeE[w]->index));
								theObjectList[target]->edgeLeftChange(edgeE[w]->index,theF->index);
								deleteEdgeH(target,edgeS[w]->index);
								break;
							}
						}
					}
					else
					if(wMode[w]==5)
					{
						Face *theF=theObjectList[target]->face(edgeS[w]->left);
                        unsigned int edgeCount=theF->edge.size();
                        for(unsigned int h=0;h<edgeCount;++h)
						{
							if(theF->edge[h]==-((int)edgeS[w]->index))
							{
								theObjectList[target]->faceEdgeChange(theF->index,h,-(int)(edgeE[w]->index));
								theObjectList[target]->edgeLeftChange(edgeE[w]->index,theF->index);
								deleteEdgeH(target,edgeS[w]->index);
								break;
							}
						}
					}
					else
					if(wMode[w]==6)
					{
						Face *theF=theObjectList[target]->face(edgeS[w]->right);
                        unsigned int edgeCount=theF->edge.size();
                        for(unsigned int h=0;h<edgeCount;++h)
						{
							if(theF->edge[h]==(int)(edgeS[w]->index))
							{
								theObjectList[target]->faceEdgeChange(theF->index,h,edgeE[w]->index);
								theObjectList[target]->edgeRightChange(edgeE[w]->index,theF->index);
								deleteEdgeH(target,edgeS[w]->index);
								break;
							}
						}
					}
					else
					if(wMode[w]==7)
					{
						Face *theF=theObjectList[target]->face(edgeS[w]->left);
                        unsigned int edgeCount=theF->edge.size();
                        for(unsigned int h=0;h<edgeCount;++h)
						{
							if(theF->edge[h]==-((int)edgeS[w]->index))
							{
								theObjectList[target]->faceEdgeChange(theF->index,h,edgeE[w]->index);
								theObjectList[target]->edgeRightChange(edgeE[w]->index,theF->index);
								deleteEdgeH(target,edgeS[w]->index);
								break;
							}
						}
					}
					else
					if(wMode[w]==8)
					{
						Face *theF=theObjectList[target]->face(edgeS[w]->right);
                        unsigned int edgeCount=theF->edge.size();
                        for(unsigned int h=0;h<edgeCount;++h)
						{
							if(theF->edge[h]==(int)edgeS[w]->index)
							{
								theObjectList[target]->faceEdgeChange(theF->index,h,-((int)edgeE[w]->index));
								theObjectList[target]->edgeLeftChange(edgeE[w]->index,theF->index);
								deleteEdgeH(target,edgeS[w]->index);
								break;
							}
						}
					}
				}
				if(theObjectList[target]->vertex(theOriginalIndex))
				{
					theOriginal=theObjectList[target]->vertex(theOriginalIndex);
                    unsigned int edgeCount=theOriginal->m_adjacentEdgeList.size();
                    for(unsigned int h=0;h<edgeCount;++h)
					{
                        theObjectList[target]->vertexAdjacentPush(theTarget->index,theOriginal->m_adjacentEdgeList[h]);
                        if(theObjectList[target]->edge(theOriginal->m_adjacentEdgeList[h])->end==theOriginal->index)
						{
                            theObjectList[target]->edgeEndChange(theOriginal->m_adjacentEdgeList[h],theTarget->index);
						}
						else
						{
                            theObjectList[target]->edgeStartChange(theOriginal->m_adjacentEdgeList[h],theTarget->index);
						}
					}
					theObjectList[target]->objectVertexRemove(theOriginal->index);
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
	if(mode==SelectionMode::Face && target)
	{
		//Ê×ÏÈÒªÍ³¼Æ×é
        std::vector<unsigned int> newFaceSelected;
        unsigned int faceCount=selection.size();
		newFaceSelected.reserve(faceCount);
        for(unsigned int i=0;i<faceCount;++i)
		{
			
			//ÕâÀïÊ×ÏÈÒªµÃµ½Ò»¸öÃæµÄÁ¬Í¨ÇøÓò
			if(!theObjectList[target]->face(selection[i]) || theObjectList[target]->face(selection[i])->isIn)
			{
				continue;
			}
			else
			{
				std::vector<Face*> theGroup;
				theGroup.reserve(100);
				std::deque<Face*> toBeHandled;
				Face *theFace=theObjectList[target]->face(selection[i]);
				toBeHandled.push_back(theFace);
				while(!toBeHandled.empty())
				{
					if(!toBeHandled[0]->isIn)
					{
						toBeHandled[0]->isIn=true;
						isInCache.push_back(toBeHandled[0]);
						theGroup.push_back(toBeHandled[0]);
                        unsigned int edgeCount=toBeHandled[0]->edge.size();
                        for(unsigned int h=0;h<edgeCount;++h)
						{
							if(toBeHandled[0]->edge[h]>0)
							{
								Face *theLeft=theObjectList[target]->face(theObjectList[target]->edge(toBeHandled[0]->edge[h])->left);
								if(theLeft && theLeft->isSelected)
								{
									toBeHandled.push_back(theLeft);
								}
							}
							else
							{
								Face *theRight=theObjectList[target]->face(theObjectList[target]->edge(-toBeHandled[0]->edge[h])->right);
								if(theRight && theRight->isSelected)
								{
									toBeHandled.push_back(theRight);
								}
							}
						}
					}
					toBeHandled.pop_front();
				}
				//ÕâÀï¾ÍÒÑ¾­µÃµ½Ò»¸ögroupÁË
				//Ê×ÏÈ¾ÍÊÇ½«group¸´ÖÆ³ÉÒ»¸öÐÂµÄ
                unsigned int groupSize=theGroup.size();
				std::vector<int> eEdgeList;
				eEdgeList.reserve(groupSize*5);
                for(unsigned int e=0;e<groupSize;++e)
				{
					Face *theFace=theGroup[e];
                    unsigned int edgeCount=theFace->edge.size();
                    unsigned int *tempEdge=new unsigned int[edgeCount];
                    for(unsigned int h=0;h<edgeCount;++h)
					{
						Edge *theEdge;
						if(theFace->edge[h]>0)
						{
							theEdge=theObjectList[target]->edge(theFace->edge[h]);
							if(!theEdge->isIn)
							{
								Vertex *theStart=theObjectList[target]->vertex(theEdge->start);
								Vertex *theEnd=theObjectList[target]->vertex(theEdge->end);
								if(!theStart->isIn)
								{
                                    theStart->m_clone=theObjectList[target]->addVertex(theStart->m_position.x+x,theStart->m_position.y+y,theStart->m_position.z+z);
									theStart->isIn=true;
									isInCache.push_back(theStart);
								}
								if(!theEnd->isIn)
								{
                                    theEnd->m_clone=theObjectList[target]->addVertex(theEnd->m_position.x+x,theEnd->m_position.y+y,theEnd->m_position.z+z);
									theEnd->isIn=true;
									isInCache.push_back(theEnd);
								}
                                theEdge->clone=theObjectList[target]->addEdge(theStart->m_clone,theEnd->m_clone);
								theEdge->isIn=true;
								isInCache.push_back(theEdge);
								if(theEdge->left==0 ||(theEdge->left>0 && !theObjectList[target]->face(theEdge->left)->isIn))
								{
									//ËµÃ÷ÊÇ±ßÉÏµÄ
									eEdgeList.push_back(theEdge->index);
								}
							}
                            tempEdge[h]=theEdge->clone;
						}
						else
						{
							theEdge=theObjectList[target]->edge(-theFace->edge[h]);
							if(!theEdge->isIn)
							{
								Vertex *theStart=theObjectList[target]->vertex(theEdge->start);
								Vertex *theEnd=theObjectList[target]->vertex(theEdge->end);
								if(!theStart->isIn)
								{
                                    theStart->m_clone=theObjectList[target]->addVertex(theStart->m_position.x+x,theStart->m_position.y+y,theStart->m_position.z+z);
									theStart->isIn=true;
									isInCache.push_back(theStart);
								}
								if(!theEnd->isIn)
								{
                                    theEnd->m_clone=theObjectList[target]->addVertex(theEnd->m_position.x+x,theEnd->m_position.y+y,theEnd->m_position.z+z);
									theEnd->isIn=true;
									isInCache.push_back(theEnd);
								}
                                theEdge->clone=theObjectList[target]->addEdge(theStart->m_clone,theEnd->m_clone);
								theEdge->isIn=true;
								isInCache.push_back(theEdge);
								if(theEdge->right==0 ||(theEdge->right>0 && !theObjectList[target]->face(theEdge->right)->isIn))
								{
									//ËµÃ÷ÊÇ±ßÉÏµÄ
									eEdgeList.push_back(-((int)theEdge->index));
								}
							}
                            tempEdge[h]=theEdge->clone;
						}
					}
					//theObjectList[target]->addFace(tempEdge,edgeCount);
					newFaceSelected.push_back(theObjectList[target]->addFace(tempEdge,edgeCount));
					delete tempEdge;
				}

				//Éú³É²àÃæµÄÃæ
                unsigned int eEdgeListSize=eEdgeList.size();
                for(unsigned int e=0;e<eEdgeListSize;++e)
				{
					if(eEdgeList[e]>0)
					{
						Edge *theEdge=theObjectList[target]->edge(eEdgeList[e]);
						Vertex *theStart=theObjectList[target]->vertex(theEdge->start);
						Vertex *theEnd=theObjectList[target]->vertex(theEdge->end);
                        unsigned int tempEdge[4]={0};
                        if(!theObjectList[target]->vertex(theStart->m_clone)->isIn)
						{
                            theObjectList[target]->vertex(theStart->m_clone)->m_clone=theObjectList[target]->addEdge(theStart->index,theStart->m_clone);
                            theObjectList[target]->vertex(theStart->m_clone)->isIn=true;
                            isInCache.push_back(theObjectList[target]->vertex(theStart->m_clone));
						}
                        tempEdge[3]=theObjectList[target]->vertex(theStart->m_clone)->m_clone;
                        if(!theObjectList[target]->vertex(theEnd->m_clone)->isIn)
						{
                            theObjectList[target]->vertex(theEnd->m_clone)->m_clone=theObjectList[target]->addEdge(theEnd->index,theEnd->m_clone);
                            theObjectList[target]->vertex(theEnd->m_clone)->isIn=true;
                            isInCache.push_back(theObjectList[target]->vertex(theEnd->m_clone));
						}
                        tempEdge[1]=theObjectList[target]->vertex(theEnd->m_clone)->m_clone;
						tempEdge[0]=theEdge->index;
                        tempEdge[2]=theEdge->clone;;
						theObjectList[target]->addFace(tempEdge,4);
					}
					else
					{
						Edge *theEdge=theObjectList[target]->edge(-eEdgeList[e]);
						Vertex *theStart=theObjectList[target]->vertex(theEdge->start);
						Vertex *theEnd=theObjectList[target]->vertex(theEdge->end);
                        unsigned int tempEdge[4]={0};
                        if(!theObjectList[target]->vertex(theStart->m_clone)->isIn)
						{
                            theObjectList[target]->vertex(theStart->m_clone)->m_clone=theObjectList[target]->addEdge(theStart->index,theStart->m_clone);
                            theObjectList[target]->vertex(theStart->m_clone)->isIn=true;
                            isInCache.push_back(theObjectList[target]->vertex(theStart->m_clone));
						}
                        tempEdge[1]=theObjectList[target]->vertex(theStart->m_clone)->m_clone;
                        if(!theObjectList[target]->vertex(theEnd->m_clone)->isIn)
						{
                            theObjectList[target]->vertex(theEnd->m_clone)->m_clone=theObjectList[target]->addEdge(theEnd->index,theEnd->m_clone);
                            theObjectList[target]->vertex(theEnd->m_clone)->isIn=true;
                            isInCache.push_back(theObjectList[target]->vertex(theEnd->m_clone));
						}
                        tempEdge[3]=theObjectList[target]->vertex(theEnd->m_clone)->m_clone;
						tempEdge[0]=theEdge->index;
                        tempEdge[2]=theEdge->clone;;
						theObjectList[target]->addFace(tempEdge,4);
					}
				}
				clearIsInCache();
				//É¾³ýÔ­À´µÄÃæ
                for(unsigned int e=0;e<groupSize;++e)
				{
					Face *theFace=theGroup[e];
                    unsigned int edgeCount=theFace->edge.size();
                    for(unsigned int h=0;h<edgeCount;++h)
					{
						if(theFace->edge[h]>0)
						{
							if(theObjectList[target]->edge(theFace->edge[h])->right==theFace->index)
							{
								theObjectList[target]->edgeRightChange(theFace->edge[h],0);
								if(!theObjectList[target]->edge(theFace->edge[h])->left)
								{
									deleteEdgeH(target,theFace->edge[h]);
								}
							}
						}
						else
						{
							if(theObjectList[target]->edge(-theFace->edge[h])->left==theFace->index)
							{
								theObjectList[target]->edgeLeftChange(-theFace->edge[h],0);
								if(!theObjectList[target]->edge(-theFace->edge[h])->right)
								{
									deleteEdgeH(target,-theFace->edge[h]);
								}
							}
						}
					}
					theObjectList[target]->objectFaceRemove(theFace->index);
				}
				theGroup.clear();
				
			}
		}

		clearSelection();

        for(unsigned int i=0;i<newFaceSelected.size();++i)
		{
			Face *theFace=theObjectList[target]->face(newFaceSelected[i]);
			if(theFace)
			{
				selectionPush(theFace);
			}
		}
	}
}

void Scene::sceneObjectAdd(char *name)
	{
	historyManager->record(new Log_SceneObjectAdd(theObjectList.add(new Object(name))));
	}
void Scene::sceneObjectAdd(Object *newObject)
	{
		
		historyManager->record(new Log_SceneObjectAdd(theObjectList.add(newObject)));
	}
void Scene::detach(Object *newObject)
	{
		if(mode==SelectionMode::Face && target)
		{
			sceneObjectAdd(newObject);
			
			//Ê×ÏÈÒªÍ³¼Æ×é
            unsigned int faceCount=selection.size();
            for(unsigned int e=0;e<faceCount;++e)
			{
				Face *theFace=theObjectList[target]->face(selection[e]);
                unsigned int edgeCount=theFace->edge.size();
                unsigned int *tempEdge=new unsigned int[edgeCount];
                for(unsigned int h=0;h<edgeCount;++h)
				{
					Edge *theEdge;
					if(theFace->edge[h]>0)
					{
						theEdge=theObjectList[target]->edge(theFace->edge[h]);
					}
					else
					{
						theEdge=theObjectList[target]->edge(-theFace->edge[h]);
					}
					if(!theEdge->isIn)
					{
						Vertex *theStart=theObjectList[target]->vertex(theEdge->start);
						Vertex *theEnd=theObjectList[target]->vertex(theEdge->end);
						if(!theStart->isIn)
						{
                            theStart->m_clone=newObject->addVertex(theStart->m_position);
							theStart->isIn=true;
							isInCache.push_back(theStart);
						}
						if(!theEnd->isIn)
						{
                            theEnd->m_clone=newObject->addVertex(theEnd->m_position.x,theEnd->m_position.y,theEnd->m_position.z);
							theEnd->isIn=true;
							isInCache.push_back(theEnd);
						}
                        theEdge->clone=newObject->addEdge(theStart->m_clone,theEnd->m_clone);
						theEdge->isIn=true;
						isInCache.push_back(theEdge);
					}
                    tempEdge[h]=theEdge->clone;;
				}

				newObject->addFace(tempEdge,edgeCount);
				delete tempEdge;
			}
			clearIsInCache();
for(unsigned int e=0;e<faceCount;++e)
			{
				Face *theFace=theObjectList[target]->face(selection[e]);
unsigned int edgeCount=theFace->edge.size();
                for(unsigned int h=0;h<edgeCount;++h)
				{
					if(theFace->edge[h]>0)
					{
						theObjectList[target]->edgeRightChange(theFace->edge[h],0);
						if(!theObjectList[target]->edge(theFace->edge[h])->left)
						{
							deleteEdgeH(target,theFace->edge[h]);
						}
					}
					else
					{
						theObjectList[target]->edgeLeftChange(-theFace->edge[h],0);
						if(!theObjectList[target]->edge(-theFace->edge[h])->right)
						{
							deleteEdgeH(target,-theFace->edge[h]);
						}
					}
				}
				theObjectList[target]->objectFaceRemove(theFace->index);
				//printf("sf");
			}
			
newObject->center=theObjectList[target]->center;
newObject->rotation=theObjectList[target]->rotation;
newObject->position=theObjectList[target]->position;
newObject->scale=theObjectList[target]->scale;

		}

		
	}

void Scene::attach(unsigned int original,unsigned int toBeAttached)
	{
		if(original && toBeAttached)
		{
			Vector rotation=theObjectList[toBeAttached]->rotation;
            Vector position=theObjectList[toBeAttached]->position;
			Vector scale=theObjectList[toBeAttached]->scale;
			Vector center=theObjectList[toBeAttached]->center;

			Vector originalRotation=theObjectList[original]->rotation;
            Vector originalPosition=theObjectList[original]->position;
			Vector originalScale=theObjectList[original]->scale;
			Vector originalCenter=theObjectList[original]->center;

			Matrix transform;

			glMatrixMode(GL_MODELVIEW);
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
			glPopMatrix();

            unsigned int vertexCount=theObjectList[toBeAttached]->vertexCount();
            for(unsigned int i=0;i<vertexCount;++i)
			{
				Vertex *theVertex=theObjectList[toBeAttached]->vertex(i);
				if(theVertex)
				{
                    Vector theNV(transform*(theVertex->m_position));
                    theVertex->m_clone=theObjectList[original]->addVertex(theNV,theVertex->m_normal);
				}
			}

            unsigned int edgeCount=theObjectList[toBeAttached]->edgeCount();
            for(unsigned int e=0;e<edgeCount;++e)
			{
				Edge *theEdge=theObjectList[toBeAttached]->edge(e);
				if(e)
				{
                    theEdge->clone=theObjectList[original]->addEdge(theObjectList[toBeAttached]->vertex(theEdge->start)->m_clone,theObjectList[toBeAttached]->vertex(theEdge->end)->m_clone);
				}
			}

            unsigned int faceCount=theObjectList[toBeAttached]->faceCount();
            for(unsigned int f=1;f<faceCount;++f)
			{
				Face *theFace=theObjectList[toBeAttached]->face(f);
				if(theFace==NULL) continue;
                unsigned int edgeCount=theFace->edge.size();
                unsigned int *tempEdge=new unsigned int[edgeCount];
                for(unsigned int e=0;e<edgeCount;++e)
				{
					if(theFace->edge[e]>0)
					{
                        tempEdge[e]=theObjectList[toBeAttached]->edge(theFace->edge[e])->clone;
					}
					else
					{
                        tempEdge[e]=theObjectList[toBeAttached]->edge(-theFace->edge[e])->clone;
					}
				}
				theObjectList[original]->addFace(tempEdge,edgeCount);
				delete tempEdge;
			}
			//É¾³ýÄ£ÐÍ»¹Ã»ÓÐÄØ;
			sceneObjectRemove(toBeAttached);
		}
	}

void Scene::clearSelection()
	{
        unsigned int selectionSize=selection.size();
		if(mode==SelectionMode::Vertex)
		{
            for(unsigned int i=0;i<selectionSize;i++)
			{
				if(theObjectList[target]->vertex(selection[i]))
				{
					theObjectList[target]->vertex(selection[i])->isSelected=false;
				}
			}
		}
		else
		if(mode==SelectionMode::Edge)
		{
            for(unsigned int i=0;i<selectionSize;i++)
			{
				if(theObjectList[target]->edge(selection[i]))
				{
					theObjectList[target]->edge(selection[i])->isSelected=false;
				}
			}			
		}
		else
		if(mode==SelectionMode::Face)
		{
            for(unsigned int i=0;i<selectionSize;i++)
			{
				if(theObjectList[target]->face(selection[i]))
				{
					theObjectList[target]->face(selection[i])->isSelected=false;
				}
			}			
		}
		else
		if(mode==SelectionMode::Object)
		{
            for(unsigned int i=0;i<selectionSize;i++)
			{
				if(theObjectList[selection[i]])
				{
					theObjectList[selection[i]]->isSelected=false;
				}
			}						
		}
		selection.clear();
    }

Scene::~Scene(void)
{
	delete theAxisCursor;
}
