#include "Object.h"
#include <iostream>


Object::Object(char *theName):center(),position(),rotation(),scale(1,1,1,1),name(theName),subdivideId(0),subdivideLevelSize(0),isHide(false),isMirror(false),theMirrorMode(MirrorMode::Nothing),mirrorPosition(0)
{
	mat_ambient[0] =  0.7f;
	mat_ambient[1] =  0.7f;
	mat_ambient[2] =  0.7f;
	mat_ambient[3] =  1.0f;

	mat_diffuse[0] =  0.9f;
	mat_diffuse[1] =  0.9f;
	mat_diffuse[2] =  0.9f;
	mat_diffuse[3] =  1.0f;

	mat_specular[0] =  0.11f;
	mat_specular[1] =  0.11f;
	mat_specular[2] =  0.11f;
	mat_specular[3] =  1.0f;

	mat_emission[0] = 0.0f;
	mat_emission[1] = 0.0f;
	mat_emission[2] = 0.0f;
	mat_emission[3] = 1.0f;

	mat_shininess[0] = 10.0f;
	mat_shininess[1] = 10.0f;
	mat_shininess[2] = 10.0f;
	mat_shininess[3] = 10.0f;

	vertexArray.reserve(5000);
	edgeArray.reserve(5000);
	faceArray.reserve(5000);
	subdivideLevel[0]=NULL;
	subdivideLevel[1]=NULL;
	subdivideLevel[2]=NULL;
	subdivideLevel[3]=NULL;
	subdivideLevel[4]=NULL;
	renderMode=RenderType::WireframeFaced;
};


size_t Object::addVertex(float p1,float p2,float p3)
{
	size_t vi=vertexArray.add(new Vertex(p1,p2,p3));
	historyManager->record(new Log_ObjectVertexAdd(index,vi));
	return vi;
}

size_t Object::addVertex(float p1,float p2,float p3,float n1,float n2,float n3)
{
	size_t vi=vertexArray.add(new Vertex(p1,p2,p3,n1,n2,n3));
	historyManager->record(new Log_ObjectVertexAdd(index,vi));
	return vi;
}

size_t Object::addEdge(size_t ei,Edge *theE)
	{
		edgeArray.addI(ei,theE);
		historyManager->record(new Log_ObjectEdgeAdd(index,ei));
		return ei;
	};

size_t Object::addFace(size_t ei,Face *theF)
	{
		faceArray.addI(ei,theF);
		historyManager->record(new Log_ObjectFaceAdd(index,ei));
		return ei;
	}

void Object::drawMirror()
	{
		if(!isHide && isMirror)
		{			
			glMatrixMode(GL_MODELVIEW);
			glPushMatrix();
			//glTranslatef(100,100,100);
			glTranslatef(mirrorPosition.x,mirrorPosition.y,mirrorPosition.z);
			glTranslatef(position.x,position.y,position.z);
			glTranslatef(center.x,center.y,center.z);
			glRotatef(rotation.w,rotation.x,rotation.y,rotation.z);
			glScalef(scale.x,scale.y,scale.z);
			glTranslatef(-center.x,-center.y,-center.z);
			if(theMirrorMode==MirrorMode::MirrorXY)
			{
				
				
				glScalef(1,1,-1);
			}
			else if(theMirrorMode==MirrorMode::MirrorYZ)
			{
				
			
				glScalef(-1,1,1);
			}
			else if(theMirrorMode==MirrorMode::MirrorXZ)
			{
				
				glScalef(1,-1,1);
				
			}

			if(renderMode==RenderType::Faced)
			{
				drawFaced();
			}
			else if(renderMode==RenderType::Smooth)
			{
				drawSmooth();
			}
			else if(renderMode==RenderType::Wire)
			{
				drawWire();
			}
			else if(renderMode==RenderType::Wireframe)
			{
				drawWireframe();
			}
			else if(renderMode==RenderType::WireframeFaced)
			{
				drawWireframeFaced();
			}

			glPopMatrix();
		}
	}

size_t Object::addVertex(Vector &pos)
{
	size_t vi=vertexArray.add(new Vertex(pos));
	historyManager->record(new Log_ObjectVertexAdd(index,vi));
	return vi;
}

size_t Object::addVertex(size_t ei,Vertex *theV)
{
	vertexArray.addI(ei,theV);
	historyManager->record(new Log_ObjectVertexAdd(index,ei));
	return ei;
}

size_t Object::addVertex(Vector &pos,Vector &nor)
{
	size_t vi=vertexArray.add(new Vertex(pos,nor));
	historyManager->record(new Log_ObjectVertexAdd(index,vi));
	return vi;
}

void Object::testOut(char *fileName)
{
	FILE *fp=fopen(fileName,"w");
	for(size_t i=1;i<vertexArray.size();i++)
	{
		if(vertexArray[i]==NULL)
		{
			fprintf(fp,"v %f %f %f\n",0,0,0);
		}
		else
		{
			fprintf(fp,"v %f %f %f\n",vertexArray[i]->position.x,vertexArray[i]->position.y,vertexArray[i]->position.z);
		}
	}
	fprintf(fp,"g box01\n");
	for(size_t i=1;i<faceArray.size();i++)
	{
		if(faceArray[i]==NULL) continue;
		fprintf(fp,"f ");
		for(size_t e=0;e<faceArray[i]->edge.size();e++)
		{
			if(faceArray[i]->edge[e]<0)
			{
				fprintf(fp,"%d ",edgeArray[-faceArray[i]->edge[e]]->end);
			}
			else
			{
				fprintf(fp,"%d ",edgeArray[faceArray[i]->edge[e]]->start);
			}
		}
		fprintf(fp,"\n");
	}
	fprintf(fp,"g\n");
	fclose(fp);
};

size_t Object::addEdge(int start,int end)
{
	//printf("cao0");
	size_t ei=edgeArray.add(new Edge(start,end));
	//printf("cao0.5");
//	printf("!%d,%d!",index,ei);
	Log_ObjectEdgeAdd *theLog=new Log_ObjectEdgeAdd(index,ei);
	//printf("cao0.6");
	historyManager->record(theLog);
	//printf("cao1");
	vertexArray[start]->adjacentEdge.push_back((int)ei);
	historyManager->record(new Log_VertexAdjacentPush(index,start));
	//printf("cao2");
	vertexArray[end]->adjacentEdge.push_back((int)ei);
	historyManager->record(new Log_VertexAdjacentPush(index,end));
	//printf("cao3");
	return ei;
};

Vertex * Object::vertex(size_t index)
{
	return vertexArray[index];
};

Edge * Object::edge(size_t index)
{
	return edgeArray[index];
};

Face * Object::face(size_t index)
{
	return faceArray[index];
};

size_t Object::addFace(size_t theEdgeArray[],size_t size)
{
	//printf("start\n");
	Face *theFace=new Face();
	size_t resultIndex=faceArray.add(theFace);
	historyManager->record(new Log_ObjectFaceAdd(index,resultIndex));
	for(size_t i=0;i<size;++i)
	{
		size_t i1=theEdgeArray[i];
		size_t i2=theEdgeArray[(i+1)%size];
		if(edgeArray[i1]->end==edgeArray[i2]->start || edgeArray[i1]->end==edgeArray[i2]->end)
		{
			edgeArray[i1]->right=theFace->index;
			historyManager->record(new Log_EdgeRightChange(index,i1,theFace->index));
			theFace->edge.push_back(i1);
			historyManager->record(new Log_FaceEdgePush(index,theFace->index));
		}
		else
		if(edgeArray[i1]->start==edgeArray[i2]->start || edgeArray[i1]->start==edgeArray[i2]->end)
		{
			edgeArray[i1]->left=theFace->index;
			historyManager->record(new Log_EdgeLeftChange(index,i1,theFace->index));
			theFace->edge.push_back(-((int)i1));
			historyManager->record(new Log_FaceEdgePush(index,theFace->index));
		}
	}
	//printf("end\n");
	return resultIndex;
};

void Object::SubTestOut(char *fileName,int level)
{
	FILE *fp=NULL;
	fp=fopen(fileName,"w");
	SubdivideLevel *theSub=subdivideLevel[level];
	size_t vertexCount=theSub->vertex.size();
	for(size_t i=1;i<vertexCount;++i)
	{
		fprintf(fp,"v %f %f %f\n",theSub->vertex[i]->position.x,theSub->vertex[i]->position.y,theSub->vertex[i]->position.z);
	}
	fprintf(fp,"g box01\n");
	for(size_t i=1;i<theSub->face.size();++i)
	{
		fprintf(fp,"f ");
		for(size_t e=0;e<4;e++)
		{
			if(theSub->face[i]->edge[e]>0)
			{
				fprintf(fp,"%d ",theSub->edge[theSub->face[i]->edge[e]]->start);
			}
			else
			{
				fprintf(fp,"%d ",theSub->edge[-theSub->face[i]->edge[e]]->end);
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
	//递增细分的编号
	++subdivideId;
	if(!subdivideLevelSize)
	{
		subdivideLevel[0]=new SubdivideLevel(vertexArray.size()+edgeArray.size()+faceArray.size(),edgeArray.size()*4,edgeArray.size()*2);
		++subdivideLevelSize;
        size_t faceCount=  faceArray.size();
		for(size_t i=1;i<faceCount;++i)
		{
			if(faceArray[i]==NULL)
				continue;
			subdivideFace(faceArray[i]);
		}
	}
	else
	if(subdivideLevelSize>0 && subdivideLevelSize<5)
	{
		for(int e=subdivideLevelSize;e>0;--e)
		{
			subdivideLevel[e]=subdivideLevel[e-1];
		}
		subdivideLevel[0]=new SubdivideLevel(subdivideLevel[0]->vertex.size()+subdivideLevel[0]->edge.size()+subdivideLevel[0]->face.size(),subdivideLevel[0]->edge.size()*4,subdivideLevel[0]->edge.size()*2);
		++subdivideLevelSize;
		size_t faceCount=subdivideLevel[1]->face.size();
		for(size_t i=1;i<faceCount;i++)
		{
			if(subdivideLevel[1]->face[i]==NULL)
				continue;
			subdivideFace(subdivideLevel[1]->face[i]);
		}
	}
	updateAllSubNormal();
}

Vector Object::EAdjacentVertex(Vertex *theVertex)
{
	theVertex->edgeVertex=false;
	Vector result(0);
	size_t vertexCount=theVertex->adjacentEdge.size();
	for(size_t i=0;i<vertexCount;++i)
	{
		if(edgeArray[theVertex->adjacentEdge[i]]->left && edgeArray[theVertex->adjacentEdge[i]]->right && !theVertex->edgeVertex)
		{
			if(theVertex->index==edgeArray[theVertex->adjacentEdge[i]]->end)
			{
				result+=vertexArray[edgeArray[theVertex->adjacentEdge[i]]->start]->position;
			}
			else
			{
				result+=vertexArray[edgeArray[theVertex->adjacentEdge[i]]->end]->position;
			}
		}
		else
		{
			if(!theVertex->edgeVertex)
			{
				theVertex->edgeVertex=true;
				result.null();
			}
			if(!edgeArray[theVertex->adjacentEdge[i]]->left || !edgeArray[theVertex->adjacentEdge[i]]->right)
			{
				if(theVertex->index==edgeArray[theVertex->adjacentEdge[i]]->end)
				{
					result+=vertexArray[edgeArray[theVertex->adjacentEdge[i]]->start]->position;
				}
				else
				{
					result+=vertexArray[edgeArray[theVertex->adjacentEdge[i]]->end]->position;
				}
			}
		}
	}
	return result;
};

Vector Object::EAdjacentVertex(SubdivideVertex *theVertex,int level)
{
	theVertex->edgeVertex=false;
	Vector result(0);
	size_t vertexCount=theVertex->adjacentEdge.size();
	for(size_t i=0;i<vertexCount;++i)
	{
		if(subdivideLevel[level]->edge[theVertex->adjacentEdge[i]]->left!=0 && subdivideLevel[level]->edge[theVertex->adjacentEdge[i]]->right!=0 && !theVertex->edgeVertex)
		{
			if(theVertex->index==subdivideLevel[level]->edge[theVertex->adjacentEdge[i]]->end)
			{
				result+=subdivideLevel[level]->vertex[subdivideLevel[level]->edge[theVertex->adjacentEdge[i]]->start]->position;
			}
			else
			{
				result+=subdivideLevel[level]->vertex[subdivideLevel[level]->edge[theVertex->adjacentEdge[i]]->end]->position;
			}
		}
		else
		{
			if(!theVertex->edgeVertex)
			{
				theVertex->edgeVertex=true;
				result.null();
			}
			if(subdivideLevel[level]->edge[theVertex->adjacentEdge[i]]->left==0 || subdivideLevel[level]->edge[theVertex->adjacentEdge[i]]->right==0)
			{
				if(theVertex->index==subdivideLevel[level]->edge[theVertex->adjacentEdge[i]]->end)
				{
					result+=subdivideLevel[level]->vertex[subdivideLevel[level]->edge[theVertex->adjacentEdge[i]]->start]->position;
				}
				else
				{
					result+=subdivideLevel[level]->vertex[subdivideLevel[level]->edge[theVertex->adjacentEdge[i]]->end]->position;
				}
			}
		}
	}
	return result;
}

Vector Object::EAdjacentVertex(SubdivideVertex *theVertex)
{
	theVertex->edgeVertex=false;
	Vector result(0);
	size_t vertexCount=theVertex->adjacentEdge.size();
	for(size_t i=0;i<vertexCount;++i)
	{
		if(subdivideLevel[1]->edge[theVertex->adjacentEdge[i]]->left!=0 && subdivideLevel[1]->edge[theVertex->adjacentEdge[i]]->right!=0 && !theVertex->edgeVertex)
		{
			if(theVertex->index==subdivideLevel[1]->edge[theVertex->adjacentEdge[i]]->end)
			{
				result+=subdivideLevel[1]->vertex[subdivideLevel[1]->edge[theVertex->adjacentEdge[i]]->start]->position;
			}
			else
			{
				result+=subdivideLevel[1]->vertex[subdivideLevel[1]->edge[theVertex->adjacentEdge[i]]->end]->position;
			}
		}
		else
		{
			if(!theVertex->edgeVertex)
			{
				theVertex->edgeVertex=true;
				result.null();
			}
			if(subdivideLevel[1]->edge[theVertex->adjacentEdge[i]]->left==0 || subdivideLevel[1]->edge[theVertex->adjacentEdge[i]]->right==0)
			{
				if(theVertex->index==subdivideLevel[1]->edge[theVertex->adjacentEdge[i]]->end)
				{
					result+=subdivideLevel[1]->vertex[subdivideLevel[1]->edge[theVertex->adjacentEdge[i]]->start]->position;
				}
				else
				{
					result+=subdivideLevel[1]->vertex[subdivideLevel[1]->edge[theVertex->adjacentEdge[i]]->end]->position;
				}
			}
		}
	}
	return result;
}

void Object::subdivideFace(Face *theFace)
{
	//首先要得到这个面的端点
	//得到端点和边的数目
	size_t edgeCount=theFace->edge.size();
	//新建存放顶点的数组
	Vertex **theVertexList=new Vertex*[edgeCount];
	theFace->center=subdivideLevel[0]->vertex.add(new SubdivideVertex());
	subdivideLevel[0]->vertex[theFace->center]->adjacentEdge.reserve(4);
	for(size_t e=0;e<edgeCount;++e)
	{
		if(theFace->edge[e]<0)
		{
			theVertexList[e]=vertexArray[edgeArray[-theFace->edge[e]]->start];
		}
		else
		{
			theVertexList[e]=vertexArray[edgeArray[theFace->edge[e]]->end];
		}
		subdivideLevel[0]->vertex[theFace->center]->position+=theVertexList[e]->position;
		//subdivideLevel[0]->vertex[theFace->center]->normal+=theVertexList[e]->normal;
	}
	subdivideLevel[0]->vertex[theFace->center]->position/=(float)edgeCount;
	//subdivideLevel[0]->vertex[theFace->center]->normal/=edgeCount;
	//计算新的顶点
	for(size_t i=0;i<edgeCount;++i)
	{
		Vertex *theV=theVertexList[i];
		if(theV->subdivideId!=subdivideId)
		{
			//如果是第一次计算到这个点
			//初始化细分步数
			theV->subdivideId=subdivideId;
			theV->subdivideStep=0;
			//生成这个点的细分点
			theV->nextLevel=subdivideLevel[0]->vertex.add(new SubdivideVertex());
			subdivideLevel[0]->vertex[theV->nextLevel]->adjacentEdge.reserve(theV->adjacentEdge.size());
			//首先计算相邻点的坐标和Ev
			subdivideLevel[0]->vertex[theV->nextLevel]->position+=EAdjacentVertex(theV);
			if(theV->edgeVertex)
			{
				subdivideLevel[0]->vertex[theV->nextLevel]->position+=theV->position*6;
				subdivideLevel[0]->vertex[theV->nextLevel]->position/=8;
			}
			else
			{
				//加上这个面的中点
				subdivideLevel[0]->vertex[theV->nextLevel]->position+=subdivideLevel[0]->vertex[theFace->center]->position;
			}
			++(theV->subdivideStep);
		}
		else
		{
			if(!theV->edgeVertex)
			{
				subdivideLevel[0]->vertex[theV->nextLevel]->position+=subdivideLevel[0]->vertex[theFace->center]->position;
				++(theV->subdivideStep);
				size_t n=theV->adjacentEdge.size();
				if(n==(size_t)theV->subdivideStep)
				{
					subdivideLevel[0]->vertex[theV->nextLevel]->position+=theV->position*(float)(n*n-2*n);
					subdivideLevel[0]->vertex[theV->nextLevel]->position/=(float)(n*n);
				}
			}
		}
	}
	delete theVertexList;
	//之后处理每一个边
	for(size_t i=0;i<edgeCount;++i)
	{
		Edge *theEdge;
		if(theFace->edge[i]>0)
		{
			theEdge=edgeArray[theFace->edge[i]];
			if(theEdge->subdivideId!=subdivideId)
			{
				//如果是第一次处理这个边
				theEdge->subdivideId=subdivideId;
				Vector theN=vertexArray[theEdge->start]->position+vertexArray[theEdge->end]->position;
				theEdge->middle=subdivideLevel[0]->vertex.add(new SubdivideVertex(theN));
				subdivideLevel[0]->vertex[theEdge->middle]->adjacentEdge.reserve(4);
				if(theEdge->left && theEdge->right)
				{
					subdivideLevel[0]->vertex[theEdge->middle]->position+=subdivideLevel[0]->vertex[theFace->center]->position;
				}
				else
				{
					subdivideLevel[0]->vertex[theEdge->middle]->position/=2;
				}
				theEdge->startEdge=subdivideLevel[0]->edge.add(new SubdivideEdge(vertexArray[theEdge->start]->nextLevel,theEdge->middle));
				subdivideLevel[0]->vertex[vertexArray[theEdge->start]->nextLevel]->adjacentEdge.push_back(theEdge->startEdge);
				subdivideLevel[0]->vertex[theEdge->middle]->adjacentEdge.push_back(theEdge->startEdge);
				theEdge->endEdge=subdivideLevel[0]->edge.add(new SubdivideEdge(theEdge->middle,vertexArray[theEdge->end]->nextLevel));
				subdivideLevel[0]->vertex[vertexArray[theEdge->end]->nextLevel]->adjacentEdge.push_back(theEdge->endEdge);
				subdivideLevel[0]->vertex[theEdge->middle]->adjacentEdge.push_back(theEdge->endEdge);
				theEdge->rightEdge=subdivideLevel[0]->edge.add(new SubdivideEdge(theEdge->middle,theFace->center));
				subdivideLevel[0]->vertex[theEdge->middle]->adjacentEdge.push_back(theEdge->rightEdge);
				subdivideLevel[0]->vertex[theFace->center]->adjacentEdge.push_back(theEdge->rightEdge);
			}
			else
			{
				//如果不是第一次计算
				subdivideLevel[0]->vertex[theEdge->middle]->position+=subdivideLevel[0]->vertex[theFace->center]->position;
				subdivideLevel[0]->vertex[theEdge->middle]->position/=4;
				theEdge->rightEdge=subdivideLevel[0]->edge.add(new SubdivideEdge(theEdge->middle,theFace->center));
				subdivideLevel[0]->vertex[theEdge->middle]->adjacentEdge.push_back(theEdge->rightEdge);
				subdivideLevel[0]->vertex[theFace->center]->adjacentEdge.push_back(theEdge->rightEdge);
			}
		}
		else
		{
			theEdge=edgeArray[-theFace->edge[i]];
			if(theEdge->subdivideId!=subdivideId)
			{
				//如果是第一次处理这个边
				theEdge->subdivideId=subdivideId;
				Vector theN=vertexArray[theEdge->start]->position+vertexArray[theEdge->end]->position;
				theEdge->middle=subdivideLevel[0]->vertex.add(new SubdivideVertex(theN));
				subdivideLevel[0]->vertex[theEdge->middle]->adjacentEdge.reserve(4);
				if(theEdge->left!=0 && theEdge->right!=0)
				{
					subdivideLevel[0]->vertex[theEdge->middle]->position+=subdivideLevel[0]->vertex[theFace->center]->position;
				}
				else
				{
					subdivideLevel[0]->vertex[theEdge->middle]->position/=2;
				}
				theEdge->startEdge=subdivideLevel[0]->edge.add(new SubdivideEdge(vertexArray[theEdge->start]->nextLevel,theEdge->middle));
				subdivideLevel[0]->vertex[vertexArray[theEdge->start]->nextLevel]->adjacentEdge.push_back(theEdge->startEdge);
				subdivideLevel[0]->vertex[theEdge->middle]->adjacentEdge.push_back(theEdge->startEdge);
				theEdge->endEdge=subdivideLevel[0]->edge.add(new SubdivideEdge(theEdge->middle,vertexArray[theEdge->end]->nextLevel));
				subdivideLevel[0]->vertex[vertexArray[theEdge->end]->nextLevel]->adjacentEdge.push_back(theEdge->endEdge);
				subdivideLevel[0]->vertex[theEdge->middle]->adjacentEdge.push_back(theEdge->endEdge);
				theEdge->leftEdge=subdivideLevel[0]->edge.add(new SubdivideEdge(theEdge->middle,theFace->center));
				subdivideLevel[0]->vertex[theEdge->middle]->adjacentEdge.push_back(theEdge->leftEdge);
				subdivideLevel[0]->vertex[theFace->center]->adjacentEdge.push_back(theEdge->leftEdge);
			}
			else
			{
				//如果不是第一次计算
				subdivideLevel[0]->vertex[theEdge->middle]->position+=subdivideLevel[0]->vertex[theFace->center]->position;
				subdivideLevel[0]->vertex[theEdge->middle]->position/=4;
				theEdge->leftEdge=subdivideLevel[0]->edge.add(new SubdivideEdge(theEdge->middle,theFace->center));
				subdivideLevel[0]->vertex[theEdge->middle]->adjacentEdge.push_back(theEdge->leftEdge);
				subdivideLevel[0]->vertex[theFace->center]->adjacentEdge.push_back(theEdge->leftEdge);
			}
		}
	}
	//建立细分之后的面
	theFace->subdivideFace.reserve(edgeCount);
	for(size_t i=0;i<edgeCount;++i)
	{
		size_t i2=(i+1)%edgeCount;
		if(theFace->edge[i]>0)
		{
			Edge *theEdge1=edgeArray[theFace->edge[i]];
			if(theFace->edge[i2]>0)
			{
				Edge *theEdge2=edgeArray[theFace->edge[i2]];
				size_t faceId=
				subdivideLevel[0]->edge[theEdge1->rightEdge]->left=
				subdivideLevel[0]->edge[theEdge1->endEdge]->right=
				subdivideLevel[0]->edge[theEdge2->startEdge]->right=
				subdivideLevel[0]->edge[theEdge2->rightEdge]->right=subdivideLevel[0]->face.add(new SubdivideFace(-theEdge1->rightEdge,theEdge1->endEdge,theEdge2->startEdge,theEdge2->rightEdge));
				theFace->subdivideFace.push_back(faceId);
			}
			else
			{
				Edge *theEdge2=edgeArray[-theFace->edge[i2]];
				size_t faceId=
				subdivideLevel[0]->edge[theEdge1->rightEdge]->left=
				subdivideLevel[0]->edge[theEdge1->endEdge]->right=
				subdivideLevel[0]->edge[theEdge2->endEdge]->left=
				subdivideLevel[0]->edge[theEdge2->leftEdge]->right=subdivideLevel[0]->face.add(new SubdivideFace(-theEdge1->rightEdge,theEdge1->endEdge,-theEdge2->endEdge,theEdge2->leftEdge));
				theFace->subdivideFace.push_back(faceId);
			}
		}
		else
		{
			Edge *theEdge1=edgeArray[-theFace->edge[i]];
			if(theFace->edge[i2]>0)
			{
				Edge *theEdge2=edgeArray[theFace->edge[i2]];
				size_t faceId=
				subdivideLevel[0]->edge[theEdge1->leftEdge]->left=
				subdivideLevel[0]->edge[theEdge1->startEdge]->left=
				subdivideLevel[0]->edge[theEdge2->startEdge]->right=
				subdivideLevel[0]->edge[theEdge2->rightEdge]->right=subdivideLevel[0]->face.add(new SubdivideFace(-theEdge1->leftEdge,-theEdge1->startEdge,theEdge2->startEdge,theEdge2->rightEdge));
				theFace->subdivideFace.push_back(faceId);
			}
			else
			{
				Edge *theEdge2=edgeArray[-theFace->edge[i2]];
				size_t faceId=
				subdivideLevel[0]->edge[theEdge1->leftEdge]->left=
				subdivideLevel[0]->edge[theEdge1->startEdge]->left=
				subdivideLevel[0]->edge[theEdge2->endEdge]->left=
				subdivideLevel[0]->edge[theEdge2->leftEdge]->right=subdivideLevel[0]->face.add(new SubdivideFace(-theEdge1->leftEdge,-theEdge1->startEdge,-theEdge2->endEdge,theEdge2->leftEdge));
				theFace->subdivideFace.push_back(faceId);
			}
		}
	}
}

void Object::subdivideFace(SubdivideFace *theFace)
{
	//首先要得到这个面的端点
	//得到端点和边的数目
	size_t edgeCount=4;
	SubdivideLevel *target=subdivideLevel[0];
	SubdivideLevel *original=subdivideLevel[1];
	//新建存放顶点的数组
	SubdivideVertex **theVertexList=new SubdivideVertex*[edgeCount];
	theFace->center=target->vertex.add(new SubdivideVertex());
	target->vertex[theFace->center]->adjacentEdge.reserve(4);
	for(size_t e=0;e<edgeCount;++e)
	{
		if(theFace->edge[e]<0)
		{
			theVertexList[e]=original->vertex[original->edge[-theFace->edge[e]]->start];
		}
		else
		{
			theVertexList[e]=original->vertex[original->edge[theFace->edge[e]]->end];
		}
		target->vertex[theFace->center]->position+=theVertexList[e]->position;
		//target->vertex[theFace->center]->normal+=theVertexList[e]->normal;
	}
	//计算这个面的中点
	target->vertex[theFace->center]->position/=(float)edgeCount;
	//target->vertex[theFace->center]->normal/=edgeCount;
	//计算新的顶点
	for(size_t i=0;i<edgeCount;++i)
	{
		SubdivideVertex *theV=theVertexList[i];
		if(theV->subdivideId!=subdivideId)
		{
			//如果是第一次计算到这个点
			//初始化细分步数
			theV->subdivideId=subdivideId;
			theV->subdivideStep=0;
			//生成这个点的细分点
			theV->nextLevel=target->vertex.add(new SubdivideVertex());
			target->vertex[theV->nextLevel]->adjacentEdge.reserve(theV->adjacentEdge.size());
			//首先计算相邻点的坐标和Ev
			target->vertex[theV->nextLevel]->position+=EAdjacentVertex(theV);
			if(theV->edgeVertex)
			{
				target->vertex[theV->nextLevel]->position+=theV->position*6;
				target->vertex[theV->nextLevel]->position/=8;
			}
			else
			{
				//加上这个面的中点
				target->vertex[theV->nextLevel]->position+=target->vertex[theFace->center]->position;
			}
			++(theV->subdivideStep);
		}
		else
		{
			if(!theV->edgeVertex)
			{
				target->vertex[theV->nextLevel]->position+=target->vertex[theFace->center]->position;
				theV->subdivideStep++;
				int n=theV->adjacentEdge.size();
				if(n==theV->subdivideStep)
				{
					target->vertex[theV->nextLevel]->position+=theV->position*(float)(n*n-2*n);
					target->vertex[theV->nextLevel]->position/=(float)(n*n);
				}
			}
		}
	}
	delete theVertexList;
	//之后处理每一个边
	for(size_t i=0;i<edgeCount;++i)
	{
		SubdivideEdge *theEdge;
		if(theFace->edge[i]>0)
		{
			theEdge=original->edge[theFace->edge[i]];
			if(theEdge->subdivideId!=subdivideId)
			{
				//如果是第一次处理这个边
				theEdge->subdivideId=subdivideId;
				Vector theN=original->vertex[theEdge->start]->position+original->vertex[theEdge->end]->position;
				theEdge->middle=target->vertex.add(new SubdivideVertex(theN));
				target->vertex[theEdge->middle]->adjacentEdge.reserve(4);
				if(theEdge->left && theEdge->right)
				{
					target->vertex[theEdge->middle]->position+=target->vertex[theFace->center]->position;
				}
				else
				{
					target->vertex[theEdge->middle]->position/=2;
				}
				theEdge->startEdge=target->edge.add(new SubdivideEdge(original->vertex[theEdge->start]->nextLevel,theEdge->middle));
				target->vertex[original->vertex[theEdge->start]->nextLevel]->adjacentEdge.push_back(theEdge->startEdge);
				target->vertex[theEdge->middle]->adjacentEdge.push_back(theEdge->startEdge);
				theEdge->endEdge=target->edge.add(new SubdivideEdge(theEdge->middle,original->vertex[theEdge->end]->nextLevel));
				target->vertex[original->vertex[theEdge->end]->nextLevel]->adjacentEdge.push_back(theEdge->endEdge);
				target->vertex[theEdge->middle]->adjacentEdge.push_back(theEdge->endEdge);
				theEdge->rightEdge=target->edge.add(new SubdivideEdge(theEdge->middle,theFace->center));
				target->vertex[theEdge->middle]->adjacentEdge.push_back(theEdge->rightEdge);
				target->vertex[theFace->center]->adjacentEdge.push_back(theEdge->rightEdge);
			}
			else
			{
				//如果不是第一次计算
				target->vertex[theEdge->middle]->position+=target->vertex[theFace->center]->position;
				target->vertex[theEdge->middle]->position/=4;
				theEdge->rightEdge=target->edge.add(new SubdivideEdge(theEdge->middle,theFace->center));
				target->vertex[theEdge->middle]->adjacentEdge.push_back(theEdge->rightEdge);
				target->vertex[theFace->center]->adjacentEdge.push_back(theEdge->rightEdge);
			}
		}
		else
		{
			theEdge=original->edge[-theFace->edge[i]];
			if(theEdge->subdivideId!=subdivideId)
			{
				//如果是第一次处理这个边
				theEdge->subdivideId=subdivideId;
				Vector theN=original->vertex[theEdge->start]->position+original->vertex[theEdge->end]->position;
				theEdge->middle=target->vertex.add(new SubdivideVertex(theN));
				target->vertex[theEdge->middle]->adjacentEdge.reserve(4);
				if(theEdge->left && theEdge->right)
				{
					target->vertex[theEdge->middle]->position+=target->vertex[theFace->center]->position;
				}
				else
				{
					target->vertex[theEdge->middle]->position/=2;
				}
				theEdge->startEdge=target->edge.add(new SubdivideEdge(original->vertex[theEdge->start]->nextLevel,theEdge->middle));
				target->vertex[original->vertex[theEdge->start]->nextLevel]->adjacentEdge.push_back(theEdge->startEdge);
				target->vertex[theEdge->middle]->adjacentEdge.push_back(theEdge->startEdge);
				theEdge->endEdge=target->edge.add(new SubdivideEdge(theEdge->middle,original->vertex[theEdge->end]->nextLevel));
				target->vertex[original->vertex[theEdge->end]->nextLevel]->adjacentEdge.push_back(theEdge->endEdge);
				target->vertex[theEdge->middle]->adjacentEdge.push_back(theEdge->endEdge);
				theEdge->leftEdge=target->edge.add(new SubdivideEdge(theEdge->middle,theFace->center));
				target->vertex[theEdge->middle]->adjacentEdge.push_back(theEdge->leftEdge);
				target->vertex[theFace->center]->adjacentEdge.push_back(theEdge->leftEdge);
			}
			else
			{
				//如果不是第一次计算
				target->vertex[theEdge->middle]->position+=target->vertex[theFace->center]->position;
				target->vertex[theEdge->middle]->position/=4;
				theEdge->leftEdge=target->edge.add(new SubdivideEdge(theEdge->middle,theFace->center));
				target->vertex[theEdge->middle]->adjacentEdge.push_back(theEdge->leftEdge);
				target->vertex[theFace->center]->adjacentEdge.push_back(theEdge->leftEdge);
			}
		}
	}
	//建立细分之后的面
	for(size_t i=0;i<edgeCount;++i)
	{
		size_t i2=(i+1)%edgeCount;
		if(theFace->edge[i]>0)
		{
			SubdivideEdge *theEdge1=original->edge[theFace->edge[i]];
			if(theFace->edge[i2]>0)
			{
				SubdivideEdge *theEdge2=original->edge[theFace->edge[i2]];
				subdivideLevel[0]->edge[theEdge1->rightEdge]->left=
				subdivideLevel[0]->edge[theEdge1->endEdge]->right=
				subdivideLevel[0]->edge[theEdge2->startEdge]->right=
				subdivideLevel[0]->edge[theEdge2->rightEdge]->right=theFace->subFace[i]=
				target->face.add(new SubdivideFace(-theEdge1->rightEdge,theEdge1->endEdge,theEdge2->startEdge,theEdge2->rightEdge));
			}
			else
			{
				SubdivideEdge *theEdge2=original->edge[-theFace->edge[i2]];
				subdivideLevel[0]->edge[theEdge1->rightEdge]->left=
				subdivideLevel[0]->edge[theEdge1->endEdge]->right=
				subdivideLevel[0]->edge[theEdge2->endEdge]->left=
				subdivideLevel[0]->edge[theEdge2->leftEdge]->right=
				theFace->subFace[i]=
				target->face.add(new SubdivideFace(-theEdge1->rightEdge,theEdge1->endEdge,-theEdge2->endEdge,theEdge2->leftEdge));
			}
		}
		else
		{
			SubdivideEdge *theEdge1=original->edge[-theFace->edge[i]];
			if(theFace->edge[i2]>0)
			{
				SubdivideEdge *theEdge2=original->edge[theFace->edge[i2]];
				subdivideLevel[0]->edge[theEdge1->leftEdge]->left=
				subdivideLevel[0]->edge[theEdge1->startEdge]->left=
				subdivideLevel[0]->edge[theEdge2->startEdge]->right=
				subdivideLevel[0]->edge[theEdge2->rightEdge]->right=
				theFace->subFace[i]=
				target->face.add(new SubdivideFace(-theEdge1->leftEdge,-theEdge1->startEdge,theEdge2->startEdge,theEdge2->rightEdge));
			}
			else
			{
				SubdivideEdge *theEdge2=original->edge[-theFace->edge[i2]];
				subdivideLevel[0]->edge[theEdge1->leftEdge]->left=
				subdivideLevel[0]->edge[theEdge1->startEdge]->left=
				subdivideLevel[0]->edge[theEdge2->endEdge]->left=
				subdivideLevel[0]->edge[theEdge2->leftEdge]->right=
				theFace->subFace[i]=
				target->face.add(new SubdivideFace(-theEdge1->leftEdge,-theEdge1->startEdge,-theEdge2->endEdge,theEdge2->leftEdge));
			}
		}
	}
}

void Object::clearPSCache()
{
	size_t cacheSize=PSFaceCache.size();
	for(int i=cacheSize-1;i>-1;--i)
	{
		PSFaceCache[i]->isSub=false;
		PSFaceCache.pop_back();
	}
	PSFaceCache.clear();
	for(size_t e=0;e<5;e++)
	{
		cacheSize=PSSubFaceCache[e].size();
		for(int i=cacheSize-1;i>-1;--i)
		{
			(PSSubFaceCache[e])[i]->isSub=false;
			PSSubFaceCache[e].pop_back();
		}
		PSSubFaceCache[e].clear();
	}
}

void Object::mirror(MirrorMode type,Vector &theMirrorPosition)
{
	theMirrorMode=type;
	mirrorPosition=theMirrorPosition;
	isMirror=true;
}

void Object::unMirror()
	{
		theMirrorMode=MirrorMode::Nothing;
		mirrorPosition.null();
		isMirror=false;
	}

void Object::directPushVertex(Vertex *theVertex)
	{
		if(theVertex)
		{
			vertexArray.directPush(theVertex);
		}
		else
		{
			vertexArray.pushNullS();
		}
	};

void Object::directPushEdge(Edge *theEdge)
	{
		if(theEdge)
		{
			edgeArray.directPush(theEdge);
		}
		else
		{
			edgeArray.pushNullS();
		}
	};

void Object::directPushFace(Face *theFace)
		{
			if(theFace)
			{
				faceArray.directPush(theFace);
			}
			else
			{
				faceArray.pushNullS();
			}
		};

void Object::normalizeVertexNormal()
	{
		for(size_t i=1;i<subdivideLevel[0]->vertex.size();++i)
		{
			subdivideLevel[0]->vertex[i]->normal.normalize();
		}
	}

void Object::updateFNormal(Face *theFace)
	{
		size_t edgeCount=theFace->edge.size();
		Vector *theVector=new Vector[edgeCount];
		for(size_t i=0;i<edgeCount;++i)
		{
			theVector[i].null();
			if(theFace->edge[i]>0)
			{
				theVector[i]=vertexArray[edgeArray[theFace->edge[i]]->end]->position-vertexArray[edgeArray[theFace->edge[i]]->start]->position;
			}
			else
			{
				theVector[i]=vertexArray[edgeArray[-theFace->edge[i]]->start]->position-vertexArray[edgeArray[-theFace->edge[i]]->end]->position;
			}
		}
		--edgeCount;
		for(size_t i=0;i<edgeCount;++i)
		{
			theFace->normal+=perpendicular(theVector[i],theVector[i+1]);
		}
		theFace->normal+=perpendicular(theVector[edgeCount],theVector[0]);
		theFace->normal.normalize();
		delete theVector;
	}

void Object::updateFNormal(SubdivideFace *theFace,size_t level)
	{
		Vector theVector[4];
		for(size_t i=0;i<4;++i)
		{
			theVector[i].null();
			if(theFace->edge[i]>0)
			{
				theVector[i]=subdivideLevel[level]->vertex[subdivideLevel[level]->edge[theFace->edge[i]]->end]->position-subdivideLevel[level]->vertex[subdivideLevel[level]->edge[theFace->edge[i]]->start]->position;
			}
			else
			{
				theVector[i]=subdivideLevel[level]->vertex[subdivideLevel[level]->edge[-theFace->edge[i]]->start]->position-subdivideLevel[level]->vertex[subdivideLevel[level]->edge[-theFace->edge[i]]->end]->position;
			}
		}
		for(size_t i=0;i<3;++i)
		{
			theFace->normal+=perpendicular(theVector[i],theVector[i+1]);
		}
		theFace->normal+=perpendicular(theVector[3],theVector[0]);
		theFace->normal.normalize();
	}

void Object::updateAllSubNormal()
	{
		size_t faceCount=subdivideLevel[0]->face.size();
		for(size_t i=1;i<faceCount;++i)
		{
			updateFNormal(subdivideLevel[0]->face[i],0);
		}
		size_t vertexCount=subdivideLevel[0]->vertex.size();
		for(size_t i=1;i<vertexCount;++i)
		{
			updateVNormal(subdivideLevel[0]->vertex[i],0);
		}
	}

//得到边的数目
size_t Object::edgeCount()
	{
		return edgeArray.size();
	};

size_t Object::vertexCount()
	{
		return vertexArray.size();
	};

size_t Object::faceCount()
	{
		return faceArray.size();	
	};

void Object::vertexPositionChangeR(size_t vertexID,float nx,float ny,float nz)
	{
		Vertex *theVertex=vertexArray[vertexID];
		historyManager->record(new Log_VertexPositionChange(index,vertexID,theVertex->position.x,theVertex->position.y,theVertex->position.z));
		theVertex->position.x+=nx;
		theVertex->position.y+=ny;
		theVertex->position.z+=nz;
	};

void Object::vertexNormalChange(size_t vertexID,float nx,float ny,float nz)
	{
		Vertex *theVertex=vertexArray[vertexID];
		historyManager->record(new Log_VertexNormalChange(index,vertexID,theVertex->normal.x,theVertex->normal.y,theVertex->normal.z));
		theVertex->normal.x=nx;
		theVertex->normal.y=ny;
		theVertex->normal.z=nz;
	};

void Object::objectEdgeRemove(size_t edgeID)
	{
		if(!historyManager->record(new Log_ObjectEdgeRemove(index,edgeID,edgeArray[edgeID])))
		{
			delete edgeArray[edgeID];
		}
		edgeArray.remove(edgeID);
	};

void Object::objectVertexRemove(size_t vertexID)
	{
		if(!historyManager->record(new Log_ObjectVertexRemove(index,vertexID,vertexArray[vertexID])))
		{
			delete vertexArray[vertexID];
		}
		vertexArray.remove(vertexID);
	};

void Object::edgeRightChange(size_t edgeID,size_t nr)
	{
		historyManager->record(new Log_EdgeRightChange(index,edgeID,edgeArray[edgeID]->right));
		edgeArray[edgeID]->right=nr;
	};

void Object::edgeStartChange(size_t edgeID,size_t ns)
	{
		historyManager->record(new Log_EdgeStartChange(index,edgeID,edgeArray[edgeID]->start));
		edgeArray[edgeID]->start=ns;
	};

void Object::vertexAdjacentRemove(size_t vertexID,size_t adjID)
	{
		size_t adjEdgeCount=vertexArray[vertexID]->adjacentEdge.size()-1;
		historyManager->record(new Log_VertexAdjacentRemove(index,vertexID,adjID,vertexArray[vertexID]->adjacentEdge[adjID]));
		size_t temp=vertexArray[vertexID]->adjacentEdge[adjEdgeCount];
		vertexArray[vertexID]->adjacentEdge[adjEdgeCount]=vertexArray[vertexID]->adjacentEdge[adjID];
		vertexArray[vertexID]->adjacentEdge[adjID]=temp;
		vertexArray[vertexID]->adjacentEdge.pop_back();
	};

void Object::vertexAdjacentInsert(size_t vertexID,size_t adjID,size_t ne)
	{
		historyManager->record(new Log_VertexAdjacentInsert(index,vertexID,adjID));
		vertexArray[vertexID]->adjacentEdge.push_back(ne);
		size_t temp=vertexArray[vertexID]->adjacentEdge[adjID];
		size_t adjEdgeCount=vertexArray[vertexID]->adjacentEdge.size()-1;
		vertexArray[vertexID]->adjacentEdge[adjID]=vertexArray[vertexID]->adjacentEdge[adjEdgeCount];
		vertexArray[vertexID]->adjacentEdge[adjEdgeCount]=temp;
	};

void Object::faceEdgePush(size_t faceID,int nEdge)
	{
		historyManager->record(new Log_FaceEdgePush(index,faceID));
		faceArray[faceID]->edge.push_back(nEdge);
	};

void Object::faceEdgeChange(size_t faceID,size_t edgeID,int nEdge)
	{
		historyManager->record(new Log_FaceEdgeChange(index,faceID,edgeID,faceArray[faceID]->edge[edgeID]));
		faceArray[faceID]->edge[edgeID]=nEdge;
	};

void Object::faceEdgeInsert(size_t faceID,size_t edgeID,int nEdge)
	{
		historyManager->record(new Log_FaceEdgeInsert(index,faceID,edgeID));
		Face *theFace=faceArray[faceID];
		theFace->edge.push_back(0);
		for(size_t h=theFace->edge.size()-1;h>edgeID;--h)
		{
			theFace->edge[h]=theFace->edge[h-1];
		}
		theFace->edge[edgeID]=nEdge;
	};

void Object::vertexAdjacentPush(size_t vertexID,size_t ne)
	{
		historyManager->record(new Log_VertexAdjacentPush(index,vertexID));
		vertexArray[vertexID]->adjacentEdge.push_back(ne);
	}

void Object::vertexAdjacentChange(size_t vertexID,size_t edgeID,size_t ne)
	{
		historyManager->record(new Log_VertexAdjacentChange(index,vertexID,edgeID,vertexArray[vertexID]->adjacentEdge[edgeID]));
		vertexArray[vertexID]->adjacentEdge[edgeID]=ne;
	}

void Object::vertexAdjacentPop(size_t vertexID)
	{
		historyManager->record(new Log_VertexAdjacentPop(index,vertexID,vertexArray[vertexID]->adjacentEdge[vertexArray[vertexID]->adjacentEdge.size()-1]));
		vertexArray[vertexID]->adjacentEdge.pop_back();
	}
void Object::faceEdgeSwap(size_t faceID,size_t i1,size_t i2)
	{
		historyManager->record(new Log_FaceEdgeSwap(index,faceID,i1,i2));
		int temp=faceArray[faceID]->edge[i1];
		faceArray[faceID]->edge[i1]=faceArray[faceID]->edge[i2];
		faceArray[faceID]->edge[i2]=temp;
	}
void Object::faceEdgeRemove(size_t faceID,size_t edgeID)
	{
		Face *theFace=faceArray[faceID];
		historyManager->record(new Log_FaceEdgeRemove(index,faceID,edgeID,theFace->edge[edgeID]));
		size_t edgeCount=theFace->edge.size();
		for(size_t e=edgeID+1;e<edgeCount;++e)
		{
			theFace->edge[e-1]=theFace->edge[e];
		}
		theFace->edge.pop_back();
	}

void Object::redefineControlPoint()
	{
		vertexArray.~IndexArray();
		vertexArray.pushNull();
		size_t vertexCount=subdivideLevel[0]->vertex.size();
		vertexArray.reserve(vertexCount);
		for(size_t i=1;i<vertexCount;i++)
		{
			addVertex(subdivideLevel[0]->vertex[i]->position,subdivideLevel[0]->vertex[i]->normal);
		}
		edgeArray.~IndexArray();
		edgeArray.pushNull();
		size_t edgeCount=subdivideLevel[0]->edge.size();
		edgeArray.reserve(edgeCount);
		for(size_t i=1;i<edgeCount;i++)
		{
		//	printf("-%d-",subdivideLevel[0]->edge[i]->start);
		//	printf("-%d-",subdivideLevel[0]->edge[i]->end);
			addEdge(subdivideLevel[0]->edge[i]->start,subdivideLevel[0]->edge[i]->end);
		}
		faceArray.~IndexArray();
		faceArray.pushNull();
		size_t faceCount=subdivideLevel[0]->face.size();
		faceArray.reserve(faceCount);
		for(size_t i=1;i<faceCount;i++)
		{
			//printf("-%d--",i);
			SubdivideFace *theSF=subdivideLevel[0]->face[i];
			size_t tempEdge[4]={0};
			tempEdge[0]=theSF->edge[0]>0?theSF->edge[0]:-(theSF->edge[0]);
			tempEdge[1]=theSF->edge[1]>0?theSF->edge[1]:-(theSF->edge[1]);
			tempEdge[2]=theSF->edge[2]>0?theSF->edge[2]:-(theSF->edge[2]);
			tempEdge[3]=theSF->edge[3]>0?theSF->edge[3]:-(theSF->edge[3]);
			addFace(tempEdge,4);
		}

		for(size_t i=0;i<(size_t)subdivideLevelSize;i++)
		{
			delete subdivideLevel[i];
			subdivideLevel[i]=NULL;
		}
		subdivideLevelSize=0;
	};


void Object::testXMLOut(char *fileName)
	{
		FILE *fp=fopen(fileName,"w");
		fprintf(fp,"<O>\n");
		for(size_t i=0;i<vertexArray.size();++i)
		{
			if(!vertexArray[i]) continue;
			fprintf(fp,"<V i=\"%d\">\n",vertexArray[i]->index);
			for(size_t e=0;e<vertexArray[i]->adjacentEdge.size();e++)
			{
				fprintf(fp,"<AE a=\"%d\" />\n",vertexArray[i]->adjacentEdge[e]);
			}
			fprintf(fp,"</V>\n");
		}
		for(size_t i=0;i<edgeArray.size();i++)
		{
			if(!edgeArray[i]) continue;
			fprintf(fp,"<E i=\"%d\" s=\"%d\" e=\"%d\" l=\"%d\" r=\"%d\" />\n",edgeArray[i]->index,edgeArray[i]->start,edgeArray[i]->end,edgeArray[i]->left,edgeArray[i]->right);
		}
		for(size_t i=0;i<faceArray.size();i++)
		{
			if(!faceArray[i]) continue;
			fprintf(fp,"<F i=\"%d\">\n",faceArray[i]->index);
			for(size_t e=0;e<faceArray[i]->edge.size();e++)
				fprintf(fp,"<FE e=\"%d\" />\n",faceArray[i]->edge[e]);
			fprintf(fp,"</F>\n");
		}
		fprintf(fp,"</O>");
		fclose(fp);
	}


void Object::vertexAdjacentSwap(size_t vertexID,size_t i1,size_t i2)
	{
		historyManager->record(new Log_VertexAdjacentSwap(index,vertexID,i1,i2));
		size_t temp=vertexArray[vertexID]->adjacentEdge[i1];
		vertexArray[vertexID]->adjacentEdge[i1]=vertexArray[vertexID]->adjacentEdge[i2];
		vertexArray[vertexID]->adjacentEdge[i2]=temp;
	}

void Object::faceEdgePop(size_t faceID)
	{
		historyManager->record(new Log_FaceEdgePop(index,faceID,faceArray[faceID]->edge[faceArray[faceID]->edge.size()-1]));
		faceArray[faceID]->edge.pop_back();
	};

void Object::edgeEndChange(size_t edgeID,size_t ne)
	{
		historyManager->record(new Log_EdgeEndChange(index,edgeID,edgeArray[edgeID]->end));
		edgeArray[edgeID]->end=ne;
	};

void Object::edgeLeftChange(size_t edgeID,size_t nl)
	{
		historyManager->record(new Log_EdgeLeftChange(index,edgeID,edgeArray[edgeID]->left));
		edgeArray[edgeID]->left=nl;
	};

void Object::objectFaceRemove(size_t faceID)
	{
		if(!historyManager->record(new Log_ObjectFaceRemove(index,faceID,faceArray[faceID])))
		{
			delete faceArray[faceID];			
		}
		faceArray.remove(faceID);
	};

	void Object::vertexPositionChangeA(size_t vertexID,float nx,float ny,float nz)
	{
		Vertex *theVertex=vertexArray[vertexID];
		historyManager->record(new Log_VertexPositionChange(index,vertexID,theVertex->position.x,theVertex->position.y,theVertex->position.z));
		theVertex->position.x=nx;
		theVertex->position.y=ny;
		theVertex->position.z=nz;
	};


void Object::updateVNormal(Vertex *theVertex)
	{
		size_t adjCount=theVertex->adjacentEdge.size();
		theVertex->normal.null();
		for(size_t i=0;i<adjCount;++i)
		{
			if(edgeArray[theVertex->adjacentEdge[i]]->start==theVertex->index)
			{
				Face *theFace=faceArray[edgeArray[theVertex->adjacentEdge[i]]->left];
				if(theFace)
				theVertex->normal+=theFace->normal;
			}
			else
			{
				Face *theFace=faceArray[edgeArray[theVertex->adjacentEdge[i]]->right];
				if(theFace)
				theVertex->normal+=theFace->normal;
			}
		}
		theVertex->normal.normalize();
	}


void Object::updateVNormal(SubdivideVertex *theVertex,size_t level)
	{
		size_t adjCount=theVertex->adjacentEdge.size();
		theVertex->normal.null();
		for(size_t i=0;i<adjCount;++i)
		{
			if(subdivideLevel[level]->edge[theVertex->adjacentEdge[i]]->start==theVertex->index)
			{
				SubdivideFace *theFace=subdivideLevel[level]->face[subdivideLevel[level]->edge[theVertex->adjacentEdge[i]]->left];
				if(theFace)
				theVertex->normal+=theFace->normal;
			}
			else
			{
				SubdivideFace *theFace=subdivideLevel[level]->face[subdivideLevel[level]->edge[theVertex->adjacentEdge[i]]->right];
				if(theFace)
				theVertex->normal+=theFace->normal;
			}
		}
		theVertex->normal.normalize();
	}


void Object::draw()
	{
		if(!isHide)
		{
			glMatrixMode(GL_MODELVIEW);
			glPushMatrix();
			glTranslatef(position.x,position.y,position.z);
			glTranslatef(center.x,center.y,center.z);
			glRotatef(rotation.w,rotation.x,rotation.y,rotation.z);
			glScalef(scale.x,scale.y,scale.z);
			glTranslatef(-center.x,-center.y,-center.z);
		
			if(renderMode==RenderType::Faced)
			{
				drawFaced();
			}
			else if(renderMode==RenderType::Smooth)
			{
				drawSmooth();
			}
			else if(renderMode==RenderType::Wire)
			{
				drawWire();
			}
			else if(renderMode==RenderType::Wireframe)
			{
				drawWireframe();
			}
			else if(renderMode==RenderType::WireframeFaced)
			{
				drawWireframeFaced();
			}
			glPopMatrix();
		}
	};


void Object::drawObjectSelected()
	{
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glTranslatef(position.x,position.y,position.z);
		glTranslatef(center.x,center.y,center.z);
		glRotatef(rotation.w,rotation.x,rotation.y,rotation.z);
glScalef(scale.x,scale.y,scale.z);
		glTranslatef(-center.x,-center.y,-center.z);

		if(renderMode==RenderType::Smooth || renderMode==RenderType::Wireframe)
		{
		drawSmoothObjectSelected();
		}
		else if(renderMode==RenderType::WireframeFaced || renderMode==RenderType::Faced)
		{
		drawFacedObjectSelected();
		}
		else if(renderMode==RenderType::Wire)
		{
		drawWireObjectSelected();
		}
			glPopMatrix();
	};

void Object::drawWireVertexSelected()
{
	glLineWidth(1);
	glDisable(GL_LIGHTING);
	glColor3ub(114,150,250);
	glLineWidth(1);
	if(subdivideLevelSize==0)
	{
		glBegin(GL_LINES);
		for(size_t i=1;i<edgeArray.size();++i)
		{
			Edge *e=edgeArray[i];
			if(e)
			{
				Vertex *start=vertexArray[e->start];
				Vertex *end=vertexArray[e->end];
				glVertex3f(start->position.x,start->position.y,start->position.z);
				glVertex3f(end->position.x,end->position.y,end->position.z);
			}
		}
		glEnd();
	}
	else
	{
		glBegin(GL_LINES);
		for(size_t i=1;i<edgeArray.size();++i)
		{
			Edge *e=edgeArray[i];
			if(e)
			{
				Vertex *start=vertexArray[e->start];
				Vertex *end=vertexArray[e->end];
				glVertex3f(start->position.x,start->position.y,start->position.z);
				glVertex3f(end->position.x,end->position.y,end->position.z);
			}
		}
		glColor3ub(0,0,0);
		for(size_t i=1;i<subdivideLevel[0]->edge.size();++i)
		{
			SubdivideEdge *e=subdivideLevel[0]->edge[i];
			SubdivideVertex *start=subdivideLevel[0]->vertex[e->start];
			SubdivideVertex *end=subdivideLevel[0]->vertex[e->end];
			glVertex3f(start->position.x,start->position.y,start->position.z);
			glVertex3f(end->position.x,end->position.y,end->position.z);
		}
		glEnd();
	}
	glEnable(GL_LIGHTING);
}

void Object::drawWireEdgeSelected()
{
			glLineWidth(1);
		glDisable(GL_LIGHTING);
		glColor3ub(114,150,250);
		glLineWidth(1);
		if(subdivideLevelSize==0)
		{
			glBegin(GL_LINES);
			for(size_t i=1;i<edgeArray.size();++i)
			{
				Edge *e=edgeArray[i];
				if(e && !(e->isSelected))
				{
					Vertex *start=vertexArray[e->start];
					Vertex *end=vertexArray[e->end];
					glVertex3f(start->position.x,start->position.y,start->position.z);
					glVertex3f(end->position.x,end->position.y,end->position.z);
				}
			}
			glEnd();
		}
		else
		{
			glBegin(GL_LINES);
			for(size_t i=1;i<edgeArray.size();++i)
			{
				Edge *e=edgeArray[i];
				if(e)
				{
					Vertex *start=vertexArray[e->start];
					Vertex *end=vertexArray[e->end];
					glVertex3f(start->position.x,start->position.y,start->position.z);
					glVertex3f(end->position.x,end->position.y,end->position.z);
				}
			}
			glColor3ub(0,0,0);
			for(size_t i=1;i<subdivideLevel[0]->edge.size();++i)
			{
				SubdivideEdge *e=subdivideLevel[0]->edge[i];
				SubdivideVertex *start=subdivideLevel[0]->vertex[e->start];
				SubdivideVertex *end=subdivideLevel[0]->vertex[e->end];
				glVertex3f(start->position.x,start->position.y,start->position.z);
				glVertex3f(end->position.x,end->position.y,end->position.z);
			}
			glEnd();
		}
		glEnable(GL_LIGHTING);
	}

void Object::drawWireFaceSelected()
{
		glLineWidth(1);
		glDisable(GL_LIGHTING);
		glColor3ub(114,150,250);
		glLineWidth(1);
		if(subdivideLevelSize==0)
		{
			glBegin(GL_LINES);
			for(size_t i=1;i<edgeArray.size();++i)
			{
				Edge *e=edgeArray[i];
				if(e)
				{
					Vertex *start=vertexArray[e->start];
					Vertex *end=vertexArray[e->end];
					glVertex3f(start->position.x,start->position.y,start->position.z);
					glVertex3f(end->position.x,end->position.y,end->position.z);
				}
			}
			glEnd();
		}
		else
		{
			glBegin(GL_LINES);
			for(size_t i=1;i<edgeArray.size();++i)
			{
				Edge *e=edgeArray[i];
				if(e)
				{
					Vertex *start=vertexArray[e->start];
					Vertex *end=vertexArray[e->end];
					glVertex3f(start->position.x,start->position.y,start->position.z);
					glVertex3f(end->position.x,end->position.y,end->position.z);
				}
			}
			glColor3ub(0,0,0);
			for(size_t i=1;i<subdivideLevel[0]->edge.size();++i)
			{
				SubdivideEdge *e=subdivideLevel[0]->edge[i];
				SubdivideVertex *start=subdivideLevel[0]->vertex[e->start];
				SubdivideVertex *end=subdivideLevel[0]->vertex[e->end];
				glVertex3f(start->position.x,start->position.y,start->position.z);
				glVertex3f(end->position.x,end->position.y,end->position.z);
			}
			glEnd();
		}
		glEnable(GL_LIGHTING);
	}


void Object::drawWireObjectSelected()
	{
		glLineWidth(1);
		glDisable(GL_LIGHTING);
		glColor3ub(114,150,250);
		glLineWidth(1);
		if(subdivideLevelSize==0)
		{
			glBegin(GL_LINES);
			for(size_t i=1;i<edgeArray.size();++i)
			{
				Edge *e=edgeArray[i];
				if(e)
				{
					Vertex *start=vertexArray[e->start];
					Vertex *end=vertexArray[e->end];
					glVertex3f(start->position.x,start->position.y,start->position.z);
					glVertex3f(end->position.x,end->position.y,end->position.z);
				}
			}
			glEnd();
		}
		else
		{
			glBegin(GL_LINES);
			for(size_t i=1;i<edgeArray.size();++i)
			{
				Edge *e=edgeArray[i];
				if(e)
				{
					Vertex *start=vertexArray[e->start];
					Vertex *end=vertexArray[e->end];
					glVertex3f(start->position.x,start->position.y,start->position.z);
					glVertex3f(end->position.x,end->position.y,end->position.z);
				}
			}
			glColor3ub(0,0,0);
			for(size_t i=1;i<subdivideLevel[0]->edge.size();++i)
			{
				SubdivideEdge *e=subdivideLevel[0]->edge[i];
				SubdivideVertex *start=subdivideLevel[0]->vertex[e->start];
				SubdivideVertex *end=subdivideLevel[0]->vertex[e->end];
				glVertex3f(start->position.x,start->position.y,start->position.z);
				glVertex3f(end->position.x,end->position.y,end->position.z);
			}
			glEnd();
		}
		glEnable(GL_LIGHTING);
	}


void Object::drawSmoothObjectSelected()
	{
		glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
		glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
		glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
		glMaterialfv(GL_FRONT, GL_EMISSION, mat_emission);
		glEnable(GL_LIGHTING);
		if(subdivideLevelSize==0)
		{
			glEnable(GL_POLYGON_OFFSET_FILL);
			glPolygonOffset(1.0f,1.0f);
			for(size_t i=1;i<faceArray.size();++i)
			{
				if(faceArray[i])
				{
					Face *theFace=faceArray[i];
					glBegin(GL_POLYGON);
					for(size_t e=0;e<theFace->edge.size();++e)
					{
						if(theFace->edge[e]>0)
						{
							Vertex *v=vertexArray[edgeArray[theFace->edge[e]]->start];
							glNormal3f(v->normal.x,v->normal.y,v->normal.z);
							glVertex3f(v->position.x,v->position.y,v->position.z);
						}
						else
						{
							Vertex *v=vertexArray[edgeArray[-theFace->edge[e]]->end];
							glNormal3f(v->normal.x,v->normal.y,v->normal.z);
							glVertex3f(v->position.x,v->position.y,v->position.z);
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
			for(size_t i=1;i<edgeArray.size();++i)
			{
				Edge *e=edgeArray[i];
				if(e)
				{
					Vertex *start=vertexArray[e->start];
					Vertex *end=vertexArray[e->end];
					glVertex3f(start->position.x,start->position.y,start->position.z);
					glVertex3f(end->position.x,end->position.y,end->position.z);
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
			for(size_t i=1;i<subdivideLevel[0]->face.size();++i)
			{
				//glBegin(GL_POLYGON);
				SubdivideFace *theFace=subdivideLevel[0]->face[i];
			//	glNormal3f(theFace->normal.x,theFace->normal.y,theFace->normal.z);
				if(theFace->edge[0]>0)
				{
					SubdivideVertex *v=subdivideLevel[0]->vertex[subdivideLevel[0]->edge[theFace->edge[0]]->start];
					glNormal3f(v->normal.x,v->normal.y,v->normal.z);
					glVertex3f(v->position.x,v->position.y,v->position.z);
				}
				else
				{
					SubdivideVertex *v=subdivideLevel[0]->vertex[subdivideLevel[0]->edge[-theFace->edge[0]]->end];
					glNormal3f(v->normal.x,v->normal.y,v->normal.z);
					glVertex3f(v->position.x,v->position.y,v->position.z);
				}
				if(theFace->edge[1]>0)
				{
					SubdivideVertex *v=subdivideLevel[0]->vertex[subdivideLevel[0]->edge[theFace->edge[1]]->start];
					glNormal3f(v->normal.x,v->normal.y,v->normal.z);
					glVertex3f(v->position.x,v->position.y,v->position.z);
				}
				else
				{
					SubdivideVertex *v=subdivideLevel[0]->vertex[subdivideLevel[0]->edge[-theFace->edge[1]]->end];
					glNormal3f(v->normal.x,v->normal.y,v->normal.z);
					glVertex3f(v->position.x,v->position.y,v->position.z);
				}				
				if(theFace->edge[2]>0)
				{
					SubdivideVertex *v=subdivideLevel[0]->vertex[subdivideLevel[0]->edge[theFace->edge[2]]->start];
					glNormal3f(v->normal.x,v->normal.y,v->normal.z);
					glVertex3f(v->position.x,v->position.y,v->position.z);
				}
				else
				{
					SubdivideVertex *v=subdivideLevel[0]->vertex[subdivideLevel[0]->edge[-theFace->edge[2]]->end];
					glNormal3f(v->normal.x,v->normal.y,v->normal.z);
					glVertex3f(v->position.x,v->position.y,v->position.z);
				}
				if(theFace->edge[3]>0)
				{
					SubdivideVertex *v=subdivideLevel[0]->vertex[subdivideLevel[0]->edge[theFace->edge[3]]->start];
					glNormal3f(v->normal.x,v->normal.y,v->normal.z);
					glVertex3f(v->position.x,v->position.y,v->position.z);
				}
				else
				{
					SubdivideVertex *v=subdivideLevel[0]->vertex[subdivideLevel[0]->edge[-theFace->edge[3]]->end];
					glNormal3f(v->normal.x,v->normal.y,v->normal.z);
					glVertex3f(v->position.x,v->position.y,v->position.z);
				}
			//	glEnd();
			}
			glEnd();
			glDisable(GL_POLYGON_OFFSET_FILL);
			glLineWidth(1);
			glDisable(GL_LIGHTING);
			glColor3ub(0,0,0);
			glLineWidth(1);
			glBegin(GL_LINES);
			for(size_t i=1;i<subdivideLevel[0]->edge.size();++i)
			{
				SubdivideEdge *e=subdivideLevel[0]->edge[i];
				SubdivideVertex *start=subdivideLevel[0]->vertex[e->start];
				SubdivideVertex *end=subdivideLevel[0]->vertex[e->end];
				glVertex3f(start->position.x,start->position.y,start->position.z);
				glVertex3f(end->position.x,end->position.y,end->position.z);
			}
			glColor3ub(114,186,221);
			for(size_t i=1;i<edgeArray.size();++i)
			{
				Edge *e=edgeArray[i];
				if(e)
				{
					Vertex *start=vertexArray[e->start];
					Vertex *end=vertexArray[e->end];
					glVertex3f(start->position.x,start->position.y,start->position.z);
					glVertex3f(end->position.x,end->position.y,end->position.z);
				}
			}
			glEnd();
			glEnable(GL_LIGHTING);
		}

	}

void Object::drawFacedObjectSelected()
	{
		glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
		glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
		glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
		glMaterialfv(GL_FRONT, GL_EMISSION, mat_emission);
		glEnable(GL_LIGHTING);
		if(subdivideLevelSize==0)
		{
			glEnable(GL_POLYGON_OFFSET_FILL);
			glPolygonOffset(1.0f,1.0f);
			for(size_t i=1;i<faceArray.size();++i)
			{
				if(faceArray[i])
				{
					Face *theFace=faceArray[i];
					glBegin(GL_POLYGON);
					glNormal3f(theFace->normal.x,theFace->normal.y,theFace->normal.z);
					for(size_t e=0;e<theFace->edge.size();++e)
					{
						if(theFace->edge[e]>0)
						{
							Vertex *v=vertexArray[edgeArray[theFace->edge[e]]->start];
							glVertex3f(v->position.x,v->position.y,v->position.z);
						}
						else
						{
							Vertex *v=vertexArray[edgeArray[-theFace->edge[e]]->end];
							glVertex3f(v->position.x,v->position.y,v->position.z);
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
			for(size_t i=1;i<edgeArray.size();++i)
			{
				Edge *e=edgeArray[i];
				if(e)
				{
					Vertex *start=vertexArray[e->start];
					Vertex *end=vertexArray[e->end];
					glVertex3f(start->position.x,start->position.y,start->position.z);
					glVertex3f(end->position.x,end->position.y,end->position.z);
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
			for(size_t i=1;i<subdivideLevel[0]->face.size();++i)
			{
				SubdivideFace *theFace=subdivideLevel[0]->face[i];
				glNormal3f(theFace->normal.x,theFace->normal.y,theFace->normal.z);
				if(theFace->edge[0]>0)
				{
					SubdivideVertex *v=subdivideLevel[0]->vertex[subdivideLevel[0]->edge[theFace->edge[0]]->start];
					glVertex3f(v->position.x,v->position.y,v->position.z);
				}
				else
				{
					SubdivideVertex *v=subdivideLevel[0]->vertex[subdivideLevel[0]->edge[-theFace->edge[0]]->end];
					glVertex3f(v->position.x,v->position.y,v->position.z);
				}
				if(theFace->edge[1]>0)
				{
					SubdivideVertex *v=subdivideLevel[0]->vertex[subdivideLevel[0]->edge[theFace->edge[1]]->start];
					glVertex3f(v->position.x,v->position.y,v->position.z);
				}
				else
				{
					SubdivideVertex *v=subdivideLevel[0]->vertex[subdivideLevel[0]->edge[-theFace->edge[1]]->end];
					glVertex3f(v->position.x,v->position.y,v->position.z);
				}				
				if(theFace->edge[2]>0)
				{
					SubdivideVertex *v=subdivideLevel[0]->vertex[subdivideLevel[0]->edge[theFace->edge[2]]->start];
					glVertex3f(v->position.x,v->position.y,v->position.z);
				}
				else
				{
					SubdivideVertex *v=subdivideLevel[0]->vertex[subdivideLevel[0]->edge[-theFace->edge[2]]->end];
					glVertex3f(v->position.x,v->position.y,v->position.z);
				}
				if(theFace->edge[3]>0)
				{
					SubdivideVertex *v=subdivideLevel[0]->vertex[subdivideLevel[0]->edge[theFace->edge[3]]->start];
					glVertex3f(v->position.x,v->position.y,v->position.z);
				}
				else
				{
					SubdivideVertex *v=subdivideLevel[0]->vertex[subdivideLevel[0]->edge[-theFace->edge[3]]->end];
					glVertex3f(v->position.x,v->position.y,v->position.z);
				}
			}
			glEnd();
			glDisable(GL_POLYGON_OFFSET_FILL);
			glLineWidth(1);
			glDisable(GL_LIGHTING);
			glColor3ub(0,0,0);
			glLineWidth(1);
			glBegin(GL_LINES);
			for(size_t i=1;i<subdivideLevel[0]->edge.size();++i)
			{
				SubdivideEdge *e=subdivideLevel[0]->edge[i];
				SubdivideVertex *start=subdivideLevel[0]->vertex[e->start];
				SubdivideVertex *end=subdivideLevel[0]->vertex[e->end];
				glVertex3f(start->position.x,start->position.y,start->position.z);
				glVertex3f(end->position.x,end->position.y,end->position.z);
			}

						glColor3ub(114,186,221);
			for(size_t i=1;i<edgeArray.size();++i)
			{
				Edge *e=edgeArray[i];
				if(e)
				{
					Vertex *start=vertexArray[e->start];
					Vertex *end=vertexArray[e->end];
					glVertex3f(start->position.x,start->position.y,start->position.z);
					glVertex3f(end->position.x,end->position.y,end->position.z);
				}
			}

			glEnd();
			glEnable(GL_LIGHTING);
		}

	}

void Object::drawFacedVertexSelected()
	{
		glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
		glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
		glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
		glMaterialfv(GL_FRONT, GL_EMISSION, mat_emission);
		glEnable(GL_LIGHTING);
				glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
				glTranslatef(position.x,position.y,position.z);
		glTranslatef(center.x,center.y,center.z);
		glRotatef(rotation.w,rotation.x,rotation.y,rotation.z);
glScalef(scale.x,scale.y,scale.z);
		glTranslatef(-center.x,-center.y,-center.z);

		if(subdivideLevelSize==0)
		{
			glEnable(GL_POLYGON_OFFSET_FILL);
			glPolygonOffset(1.0f,1.0f);
			for(size_t i=1;i<faceArray.size();++i)
			{
				if(faceArray[i])
				{
					Face *theFace=faceArray[i];
					glBegin(GL_POLYGON);
					glNormal3f(theFace->normal.x,theFace->normal.y,theFace->normal.z);
					for(size_t e=0;e<theFace->edge.size();++e)
					{
						if(theFace->edge[e]>0)
						{
							Vertex *v=vertexArray[edgeArray[theFace->edge[e]]->start];
							glVertex3f(v->position.x,v->position.y,v->position.z);
						}
						else
						{
							Vertex *v=vertexArray[edgeArray[-theFace->edge[e]]->end];
							glVertex3f(v->position.x,v->position.y,v->position.z);
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
			for(size_t i=1;i<edgeArray.size();++i)
			{
				Edge *e=edgeArray[i];
				if(e)
				{
					Vertex *start=vertexArray[e->start];
					Vertex *end=vertexArray[e->end];
					glVertex3f(start->position.x,start->position.y,start->position.z);
					glVertex3f(end->position.x,end->position.y,end->position.z);
				}
			}
			glEnd();
			glColor3ub(0,200,0);
			glPointSize(5.0f);
			glBegin(GL_POINTS);
			
			for(size_t i=1;i<vertexArray.size();++i)
			{
				Vertex *v=vertexArray[i];
				if(v && !v->isSelected)
				{
					glVertex3f(v->position.x,v->position.y,v->position.z);
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
			for(size_t i=1;i<subdivideLevel[0]->face.size();++i)
			{
				SubdivideFace *theFace=subdivideLevel[0]->face[i];
				glNormal3f(theFace->normal.x,theFace->normal.y,theFace->normal.z);
				if(theFace->edge[0]>0)
				{
					SubdivideVertex *v=subdivideLevel[0]->vertex[subdivideLevel[0]->edge[theFace->edge[0]]->start];
					glVertex3f(v->position.x,v->position.y,v->position.z);
				}
				else
				{
					SubdivideVertex *v=subdivideLevel[0]->vertex[subdivideLevel[0]->edge[-theFace->edge[0]]->end];
					glVertex3f(v->position.x,v->position.y,v->position.z);
				}
				if(theFace->edge[1]>0)
				{
					SubdivideVertex *v=subdivideLevel[0]->vertex[subdivideLevel[0]->edge[theFace->edge[1]]->start];
					glVertex3f(v->position.x,v->position.y,v->position.z);
				}
				else
				{
					SubdivideVertex *v=subdivideLevel[0]->vertex[subdivideLevel[0]->edge[-theFace->edge[1]]->end];
					glVertex3f(v->position.x,v->position.y,v->position.z);
				}				
				if(theFace->edge[2]>0)
				{
					SubdivideVertex *v=subdivideLevel[0]->vertex[subdivideLevel[0]->edge[theFace->edge[2]]->start];
					glVertex3f(v->position.x,v->position.y,v->position.z);
				}
				else
				{
					SubdivideVertex *v=subdivideLevel[0]->vertex[subdivideLevel[0]->edge[-theFace->edge[2]]->end];
					glVertex3f(v->position.x,v->position.y,v->position.z);
				}
				if(theFace->edge[3]>0)
				{
					SubdivideVertex *v=subdivideLevel[0]->vertex[subdivideLevel[0]->edge[theFace->edge[3]]->start];
					glVertex3f(v->position.x,v->position.y,v->position.z);
				}
				else
				{
					SubdivideVertex *v=subdivideLevel[0]->vertex[subdivideLevel[0]->edge[-theFace->edge[3]]->end];
					glVertex3f(v->position.x,v->position.y,v->position.z);
				}
			}
			glEnd();
			glDisable(GL_POLYGON_OFFSET_FILL);
			glLineWidth(1);
			glDisable(GL_LIGHTING);
			glColor3ub(0,0,0);
			glLineWidth(1);
			glBegin(GL_LINES);
			for(size_t i=1;i<subdivideLevel[0]->edge.size();++i)
			{
				SubdivideEdge *e=subdivideLevel[0]->edge[i];
				SubdivideVertex *start=subdivideLevel[0]->vertex[e->start];
				SubdivideVertex *end=subdivideLevel[0]->vertex[e->end];
				glVertex3f(start->position.x,start->position.y,start->position.z);
				glVertex3f(end->position.x,end->position.y,end->position.z);
			}

						glColor3ub(114,186,221);
			for(size_t i=1;i<edgeArray.size();++i)
			{
				Edge *e=edgeArray[i];
				if(e)
				{
					Vertex *start=vertexArray[e->start];
					Vertex *end=vertexArray[e->end];
					glVertex3f(start->position.x,start->position.y,start->position.z);
					glVertex3f(end->position.x,end->position.y,end->position.z);
				}
			}

			glEnd();
			glColor3ub(0,200,0);
			glPointSize(5.0f);
			glBegin(GL_POINTS);
			for(size_t i=1;i<vertexArray.size();++i)
			{
				Vertex *v=vertexArray[i];
				if(v && !v->isSelected)
				{
					glVertex3f(v->position.x,v->position.y,v->position.z);
				}
			}
			glEnd();
			glEnable(GL_LIGHTING);
		}
		glPopMatrix();
	}


void Object::drawFacedFaceSelected()
	{
		glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
		glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
		glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
		glMaterialfv(GL_FRONT, GL_EMISSION, mat_emission);
		glEnable(GL_LIGHTING);
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
				glTranslatef(position.x,position.y,position.z);
		glTranslatef(center.x,center.y,center.z);
		glRotatef(rotation.w,rotation.x,rotation.y,rotation.z);
		glScalef(scale.x,scale.y,scale.z);
		glTranslatef(-center.x,-center.y,-center.z);
		
		if(subdivideLevelSize==0)
		{
			glEnable(GL_POLYGON_OFFSET_FILL);
			glPolygonOffset(1.0f,1.0f);
			for(size_t i=1;i<faceArray.size();++i)
			{
				if(faceArray[i] && !faceArray[i]->isSelected)
				{
					Face *theFace=faceArray[i];
					glBegin(GL_POLYGON);
					glNormal3f(theFace->normal.x,theFace->normal.y,theFace->normal.z);
					for(size_t e=0;e<theFace->edge.size();++e)
					{
						if(theFace->edge[e]>0)
						{
							Vertex *v=vertexArray[edgeArray[theFace->edge[e]]->start];
							glVertex3f(v->position.x,v->position.y,v->position.z);
						}
						else
						{
							Vertex *v=vertexArray[edgeArray[-theFace->edge[e]]->end];
							glVertex3f(v->position.x,v->position.y,v->position.z);
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
			for(size_t i=1;i<edgeArray.size();++i)
			{
				Edge *e=edgeArray[i];
				if(e)
				{
					Vertex *start=vertexArray[e->start];
					Vertex *end=vertexArray[e->end];
					glVertex3f(start->position.x,start->position.y,start->position.z);
					glVertex3f(end->position.x,end->position.y,end->position.z);
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
			for(size_t i=1;i<subdivideLevel[0]->face.size();++i)
			{
				SubdivideFace *theFace=subdivideLevel[0]->face[i];
				glNormal3f(theFace->normal.x,theFace->normal.y,theFace->normal.z);
				if(theFace->edge[0]>0)
				{
					SubdivideVertex *v=subdivideLevel[0]->vertex[subdivideLevel[0]->edge[theFace->edge[0]]->start];
					glVertex3f(v->position.x,v->position.y,v->position.z);
				}
				else
				{
					SubdivideVertex *v=subdivideLevel[0]->vertex[subdivideLevel[0]->edge[-theFace->edge[0]]->end];
					glVertex3f(v->position.x,v->position.y,v->position.z);
				}
				if(theFace->edge[1]>0)
				{
					SubdivideVertex *v=subdivideLevel[0]->vertex[subdivideLevel[0]->edge[theFace->edge[1]]->start];
					glVertex3f(v->position.x,v->position.y,v->position.z);
				}
				else
				{
					SubdivideVertex *v=subdivideLevel[0]->vertex[subdivideLevel[0]->edge[-theFace->edge[1]]->end];
					glVertex3f(v->position.x,v->position.y,v->position.z);
				}				
				if(theFace->edge[2]>0)
				{
					SubdivideVertex *v=subdivideLevel[0]->vertex[subdivideLevel[0]->edge[theFace->edge[2]]->start];
					glVertex3f(v->position.x,v->position.y,v->position.z);
				}
				else
				{
					SubdivideVertex *v=subdivideLevel[0]->vertex[subdivideLevel[0]->edge[-theFace->edge[2]]->end];
					glVertex3f(v->position.x,v->position.y,v->position.z);
				}
				if(theFace->edge[3]>0)
				{
					SubdivideVertex *v=subdivideLevel[0]->vertex[subdivideLevel[0]->edge[theFace->edge[3]]->start];
					glVertex3f(v->position.x,v->position.y,v->position.z);
				}
				else
				{
					SubdivideVertex *v=subdivideLevel[0]->vertex[subdivideLevel[0]->edge[-theFace->edge[3]]->end];
					glVertex3f(v->position.x,v->position.y,v->position.z);
				}
			}
			glEnd();
			glDisable(GL_POLYGON_OFFSET_FILL);
			glLineWidth(1);
			glDisable(GL_LIGHTING);
			glColor3ub(0,0,0);
			glLineWidth(1);
			glBegin(GL_LINES);
			for(size_t i=1;i<subdivideLevel[0]->edge.size();++i)
			{
				SubdivideEdge *e=subdivideLevel[0]->edge[i];
				SubdivideVertex *start=subdivideLevel[0]->vertex[e->start];
				SubdivideVertex *end=subdivideLevel[0]->vertex[e->end];
				glVertex3f(start->position.x,start->position.y,start->position.z);
				glVertex3f(end->position.x,end->position.y,end->position.z);
			}

						glColor3ub(100,0,0);
			for(size_t i=1;i<edgeArray.size();++i)
			{
				Edge *e=edgeArray[i];
				if(e)
				{
					Vertex *start=vertexArray[e->start];
					Vertex *end=vertexArray[e->end];
					glVertex3f(start->position.x,start->position.y,start->position.z);
					glVertex3f(end->position.x,end->position.y,end->position.z);
				}
			}

			glEnd();
			glEnable(GL_LIGHTING);
		}
		glPopMatrix();
	}

void Object::drawFacedEdgeSelected()
	{
		glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
		glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
		glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
		glMaterialfv(GL_FRONT, GL_EMISSION, mat_emission);
		glEnable(GL_LIGHTING);
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
				glTranslatef(position.x,position.y,position.z);
		glTranslatef(center.x,center.y,center.z);
		glRotatef(rotation.w,rotation.x,rotation.y,rotation.z);
		glScalef(scale.x,scale.y,scale.z);
		glTranslatef(-center.x,-center.y,-center.z);
		
		if(subdivideLevelSize==0)
		{
			glEnable(GL_POLYGON_OFFSET_FILL);
			glPolygonOffset(1.0f,1.0f);
			for(size_t i=1;i<faceArray.size();++i)
			{
				if(faceArray[i])
				{
					Face *theFace=faceArray[i];
					glBegin(GL_POLYGON);
					glNormal3f(theFace->normal.x,theFace->normal.y,theFace->normal.z);
					for(size_t e=0;e<theFace->edge.size();++e)
					{
						if(theFace->edge[e]>0)
						{
							Vertex *v=vertexArray[edgeArray[theFace->edge[e]]->start];
							glVertex3f(v->position.x,v->position.y,v->position.z);
						}
						else
						{
							Vertex *v=vertexArray[edgeArray[-theFace->edge[e]]->end];
							glVertex3f(v->position.x,v->position.y,v->position.z);
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
			for(size_t i=1;i<edgeArray.size();++i)
			{
				Edge *e=edgeArray[i];
				if(e && !e->isSelected)
				{
					Vertex *start=vertexArray[e->start];
					Vertex *end=vertexArray[e->end];
					glVertex3f(start->position.x,start->position.y,start->position.z);
					glVertex3f(end->position.x,end->position.y,end->position.z);
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
			for(size_t i=1;i<subdivideLevel[0]->face.size();++i)
			{
				SubdivideFace *theFace=subdivideLevel[0]->face[i];
				glNormal3f(theFace->normal.x,theFace->normal.y,theFace->normal.z);
				if(theFace->edge[0]>0)
				{
					SubdivideVertex *v=subdivideLevel[0]->vertex[subdivideLevel[0]->edge[theFace->edge[0]]->start];
					glVertex3f(v->position.x,v->position.y,v->position.z);
				}
				else
				{
					SubdivideVertex *v=subdivideLevel[0]->vertex[subdivideLevel[0]->edge[-theFace->edge[0]]->end];
					glVertex3f(v->position.x,v->position.y,v->position.z);
				}
				if(theFace->edge[1]>0)
				{
					SubdivideVertex *v=subdivideLevel[0]->vertex[subdivideLevel[0]->edge[theFace->edge[1]]->start];
					glVertex3f(v->position.x,v->position.y,v->position.z);
				}
				else
				{
					SubdivideVertex *v=subdivideLevel[0]->vertex[subdivideLevel[0]->edge[-theFace->edge[1]]->end];
					glVertex3f(v->position.x,v->position.y,v->position.z);
				}				
				if(theFace->edge[2]>0)
				{
					SubdivideVertex *v=subdivideLevel[0]->vertex[subdivideLevel[0]->edge[theFace->edge[2]]->start];
					glVertex3f(v->position.x,v->position.y,v->position.z);
				}
				else
				{
					SubdivideVertex *v=subdivideLevel[0]->vertex[subdivideLevel[0]->edge[-theFace->edge[2]]->end];
					glVertex3f(v->position.x,v->position.y,v->position.z);
				}
				if(theFace->edge[3]>0)
				{
					SubdivideVertex *v=subdivideLevel[0]->vertex[subdivideLevel[0]->edge[theFace->edge[3]]->start];
					glVertex3f(v->position.x,v->position.y,v->position.z);
				}
				else
				{
					SubdivideVertex *v=subdivideLevel[0]->vertex[subdivideLevel[0]->edge[-theFace->edge[3]]->end];
					glVertex3f(v->position.x,v->position.y,v->position.z);
				}
			}
			glEnd();
			glDisable(GL_POLYGON_OFFSET_FILL);
			glLineWidth(1);
			glDisable(GL_LIGHTING);
			glColor3ub(0,0,0);
			glLineWidth(1);
			glBegin(GL_LINES);
			for(size_t i=1;i<subdivideLevel[0]->edge.size();++i)
			{
				SubdivideEdge *e=subdivideLevel[0]->edge[i];
				SubdivideVertex *start=subdivideLevel[0]->vertex[e->start];
				SubdivideVertex *end=subdivideLevel[0]->vertex[e->end];
				glVertex3f(start->position.x,start->position.y,start->position.z);
				glVertex3f(end->position.x,end->position.y,end->position.z);
			}

			glColor3ub(100,0,0);
			for(size_t i=1;i<edgeArray.size();++i)
			{
				Edge *e=edgeArray[i];
				if(e && !e->isSelected)
				{
					Vertex *start=vertexArray[e->start];
					Vertex *end=vertexArray[e->end];
					glVertex3f(start->position.x,start->position.y,start->position.z);
					glVertex3f(end->position.x,end->position.y,end->position.z);
				}
			}
			glEnd();
			glEnable(GL_LIGHTING);
		}
		glPopMatrix();
	};

void Object::drawFaced()
	{
		glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
		glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
		glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
		glMaterialfv(GL_FRONT, GL_EMISSION, mat_emission);
		glEnable(GL_LIGHTING);
		if(subdivideLevelSize==0)
		{
			for(size_t i=1;i<faceArray.size();++i)
			{
				if(faceArray[i])
				{
					Face *theFace=faceArray[i];
					glBegin(GL_POLYGON);
					glNormal3f(theFace->normal.x,theFace->normal.y,theFace->normal.z);
					for(size_t e=0;e<theFace->edge.size();++e)
					{
						if(theFace->edge[e]>0)
						{
							Vertex *v=vertexArray[edgeArray[theFace->edge[e]]->start];
							glVertex3f(v->position.x,v->position.y,v->position.z);
						}
						else
						{
							Vertex *v=vertexArray[edgeArray[-theFace->edge[e]]->end];
							glVertex3f(v->position.x,v->position.y,v->position.z);
						}
					}
					glEnd();
				}
			}
		}
		else
		{
			glBegin(GL_QUADS);
			for(size_t i=1;i<subdivideLevel[0]->face.size();++i)
			{
				SubdivideFace *theFace=subdivideLevel[0]->face[i];
				glNormal3f(theFace->normal.x,theFace->normal.y,theFace->normal.z);
				if(theFace->edge[0]>0)
				{
					SubdivideVertex *v=subdivideLevel[0]->vertex[subdivideLevel[0]->edge[theFace->edge[0]]->start];
					glVertex3f(v->position.x,v->position.y,v->position.z);
				}
				else
				{
					SubdivideVertex *v=subdivideLevel[0]->vertex[subdivideLevel[0]->edge[-theFace->edge[0]]->end];
					glVertex3f(v->position.x,v->position.y,v->position.z);
				}
				if(theFace->edge[1]>0)
				{
					SubdivideVertex *v=subdivideLevel[0]->vertex[subdivideLevel[0]->edge[theFace->edge[1]]->start];
					glVertex3f(v->position.x,v->position.y,v->position.z);
				}
				else
				{
					SubdivideVertex *v=subdivideLevel[0]->vertex[subdivideLevel[0]->edge[-theFace->edge[1]]->end];
					glVertex3f(v->position.x,v->position.y,v->position.z);
				}				
				if(theFace->edge[2]>0)
				{
					SubdivideVertex *v=subdivideLevel[0]->vertex[subdivideLevel[0]->edge[theFace->edge[2]]->start];
					glVertex3f(v->position.x,v->position.y,v->position.z);
				}
				else
				{
					SubdivideVertex *v=subdivideLevel[0]->vertex[subdivideLevel[0]->edge[-theFace->edge[2]]->end];
					glVertex3f(v->position.x,v->position.y,v->position.z);
				}
				if(theFace->edge[3]>0)
				{
					SubdivideVertex *v=subdivideLevel[0]->vertex[subdivideLevel[0]->edge[theFace->edge[3]]->start];
					glVertex3f(v->position.x,v->position.y,v->position.z);
				}
				else
				{
					SubdivideVertex *v=subdivideLevel[0]->vertex[subdivideLevel[0]->edge[-theFace->edge[3]]->end];
					glVertex3f(v->position.x,v->position.y,v->position.z);
				}
			}
			glEnd();
		}

	};


void Object::drawNormal()
	{
		glDisable(GL_LIGHTING);
		glBegin(GL_LINES);
		for(size_t i=1;i<vertexArray.size();++i)
		{
			Vertex *v=vertexArray[i];
			Vector normal=v->normal*10+v->position;
			glVertex3f(v->position.x,v->position.y,v->position.z);
			glVertex3f(normal.x,normal.y,normal.z);
		}
		glEnd();
		glEnable(GL_LIGHTING);
	};

void Object::drawSmooth()
	{
		glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
		glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
		glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
		glMaterialfv(GL_FRONT, GL_EMISSION, mat_emission);
		glEnable(GL_LIGHTING);
		if(subdivideLevelSize==0)
		{
			for(size_t i=1;i<faceArray.size();++i)
			{
				if(faceArray[i])
				{
					Face *theFace=faceArray[i];
					if(theFace->isSelected)
					{
						GLfloat diffuse[4]={1.0f,0.0f,0.0f,1.0f};
						glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
					}else
					{
					glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
					}
					glBegin(GL_POLYGON);
					for(size_t e=0;e<theFace->edge.size();++e)
					{
						if(theFace->edge[e]>0)
						{
							Vertex *v=vertexArray[edgeArray[theFace->edge[e]]->start];
							glNormal3f(v->normal.x,v->normal.y,v->normal.z);
							glVertex3f(v->position.x,v->position.y,v->position.z);
						}
						else
						{
							Vertex *v=vertexArray[edgeArray[-theFace->edge[e]]->end];
							glNormal3f(v->normal.x,v->normal.y,v->normal.z);
							glVertex3f(v->position.x,v->position.y,v->position.z);
						}
					}
					glEnd();
				}
			}
		}
		else
		{
			glBegin(GL_QUADS);
			for(size_t i=1;i<subdivideLevel[0]->face.size();++i)
			{
				SubdivideFace *theFace=subdivideLevel[0]->face[i];
				if(theFace->edge[0]>0)
				{
					SubdivideVertex *v=subdivideLevel[0]->vertex[subdivideLevel[0]->edge[theFace->edge[0]]->start];
					glNormal3f(v->normal.x,v->normal.y,v->normal.z);
					glVertex3f(v->position.x,v->position.y,v->position.z);
				}
				else
				{
					SubdivideVertex *v=subdivideLevel[0]->vertex[subdivideLevel[0]->edge[-theFace->edge[0]]->end];
					glNormal3f(v->normal.x,v->normal.y,v->normal.z);
					glVertex3f(v->position.x,v->position.y,v->position.z);
				}
				if(theFace->edge[1]>0)
				{
					SubdivideVertex *v=subdivideLevel[0]->vertex[subdivideLevel[0]->edge[theFace->edge[1]]->start];
					glNormal3f(v->normal.x,v->normal.y,v->normal.z);
					glVertex3f(v->position.x,v->position.y,v->position.z);
				}
				else
				{
					SubdivideVertex *v=subdivideLevel[0]->vertex[subdivideLevel[0]->edge[-theFace->edge[1]]->end];
					glNormal3f(v->normal.x,v->normal.y,v->normal.z);
					glVertex3f(v->position.x,v->position.y,v->position.z);
				}				
				if(theFace->edge[2]>0)
				{
					SubdivideVertex *v=subdivideLevel[0]->vertex[subdivideLevel[0]->edge[theFace->edge[2]]->start];
					glNormal3f(v->normal.x,v->normal.y,v->normal.z);
					glVertex3f(v->position.x,v->position.y,v->position.z);
				}
				else
				{
					SubdivideVertex *v=subdivideLevel[0]->vertex[subdivideLevel[0]->edge[-theFace->edge[2]]->end];
					glNormal3f(v->normal.x,v->normal.y,v->normal.z);
					glVertex3f(v->position.x,v->position.y,v->position.z);
				}
				if(theFace->edge[3]>0)
				{
					SubdivideVertex *v=subdivideLevel[0]->vertex[subdivideLevel[0]->edge[theFace->edge[3]]->start];
					glNormal3f(v->normal.x,v->normal.y,v->normal.z);
					glVertex3f(v->position.x,v->position.y,v->position.z);
				}
				else
				{
					SubdivideVertex *v=subdivideLevel[0]->vertex[subdivideLevel[0]->edge[-theFace->edge[3]]->end];
					glNormal3f(v->normal.x,v->normal.y,v->normal.z);
					glVertex3f(v->position.x,v->position.y,v->position.z);
				}
			}
			glEnd();
		}

	};


void Object::drawWire(GLuint r,GLuint g,GLuint b)
	{
		glLineWidth(1);
		glDisable(GL_LIGHTING);
		glColor3ub((GLubyte)r,(GLubyte)g,(GLubyte)b);
		glLineWidth(1);
		if(subdivideLevelSize==0)
		{
			glBegin(GL_LINES);
			for(size_t i=1;i<edgeArray.size();++i)
			{
				Edge *e=edgeArray[i];
				if(e)
				{
					Vertex *start=vertexArray[e->start];
					Vertex *end=vertexArray[e->end];
					glVertex3f(start->position.x,start->position.y,start->position.z);
					glVertex3f(end->position.x,end->position.y,end->position.z);
				}
			}
			glEnd();
		}
		else
		{
			glBegin(GL_LINES);
			for(size_t i=1;i<subdivideLevel[0]->edge.size();++i)
			{
				SubdivideEdge *e=subdivideLevel[0]->edge[i];
				SubdivideVertex *start=subdivideLevel[0]->vertex[e->start];
				SubdivideVertex *end=subdivideLevel[0]->vertex[e->end];
				glVertex3f(start->position.x,start->position.y,start->position.z);
				glVertex3f(end->position.x,end->position.y,end->position.z);
			}
			glEnd();
		}
		glEnable(GL_LIGHTING);
	};


void Object::drawWireframe()
	{
		glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
		glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
		glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
		glMaterialfv(GL_FRONT, GL_EMISSION, mat_emission);
		glEnable(GL_LIGHTING);
		if(subdivideLevelSize==0)
		{
			glEnable(GL_POLYGON_OFFSET_FILL);
			glPolygonOffset(1.0f,1.0f);
			for(size_t i=1;i<faceArray.size();++i)
			{
				if(faceArray[i])
				{
					Face *theFace=faceArray[i];
					glBegin(GL_POLYGON);
					for(size_t e=0;e<theFace->edge.size();++e)
					{
						if(theFace->edge[e]>0)
						{
							Vertex *v=vertexArray[edgeArray[theFace->edge[e]]->start];
							glNormal3f(v->normal.x,v->normal.y,v->normal.z);
							glVertex3f(v->position.x,v->position.y,v->position.z);
						}
						else
						{
							Vertex *v=vertexArray[edgeArray[-theFace->edge[e]]->end];
							glNormal3f(v->normal.x,v->normal.y,v->normal.z);
							glVertex3f(v->position.x,v->position.y,v->position.z);
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
			for(size_t i=1;i<edgeArray.size();++i)
			{
				Edge *e=edgeArray[i];
				if(e)
				{
					Vertex *start=vertexArray[e->start];
					Vertex *end=vertexArray[e->end];
					glVertex3f(start->position.x,start->position.y,start->position.z);
					glVertex3f(end->position.x,end->position.y,end->position.z);
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
			for(size_t i=1;i<subdivideLevel[0]->face.size();++i)
			{
				//glBegin(GL_POLYGON);
				SubdivideFace *theFace=subdivideLevel[0]->face[i];
			//	glNormal3f(theFace->normal.x,theFace->normal.y,theFace->normal.z);
				if(theFace->edge[0]>0)
				{
					SubdivideVertex *v=subdivideLevel[0]->vertex[subdivideLevel[0]->edge[theFace->edge[0]]->start];
					glNormal3f(v->normal.x,v->normal.y,v->normal.z);
					glVertex3f(v->position.x,v->position.y,v->position.z);
				}
				else
				{
					SubdivideVertex *v=subdivideLevel[0]->vertex[subdivideLevel[0]->edge[-theFace->edge[0]]->end];
					glNormal3f(v->normal.x,v->normal.y,v->normal.z);
					glVertex3f(v->position.x,v->position.y,v->position.z);
				}
				if(theFace->edge[1]>0)
				{
					SubdivideVertex *v=subdivideLevel[0]->vertex[subdivideLevel[0]->edge[theFace->edge[1]]->start];
					glNormal3f(v->normal.x,v->normal.y,v->normal.z);
					glVertex3f(v->position.x,v->position.y,v->position.z);
				}
				else
				{
					SubdivideVertex *v=subdivideLevel[0]->vertex[subdivideLevel[0]->edge[-theFace->edge[1]]->end];
					glNormal3f(v->normal.x,v->normal.y,v->normal.z);
					glVertex3f(v->position.x,v->position.y,v->position.z);
				}				
				if(theFace->edge[2]>0)
				{
					SubdivideVertex *v=subdivideLevel[0]->vertex[subdivideLevel[0]->edge[theFace->edge[2]]->start];
					glNormal3f(v->normal.x,v->normal.y,v->normal.z);
					glVertex3f(v->position.x,v->position.y,v->position.z);
				}
				else
				{
					SubdivideVertex *v=subdivideLevel[0]->vertex[subdivideLevel[0]->edge[-theFace->edge[2]]->end];
					glNormal3f(v->normal.x,v->normal.y,v->normal.z);
					glVertex3f(v->position.x,v->position.y,v->position.z);
				}
				if(theFace->edge[3]>0)
				{
					SubdivideVertex *v=subdivideLevel[0]->vertex[subdivideLevel[0]->edge[theFace->edge[3]]->start];
					glNormal3f(v->normal.x,v->normal.y,v->normal.z);
					glVertex3f(v->position.x,v->position.y,v->position.z);
				}
				else
				{
					SubdivideVertex *v=subdivideLevel[0]->vertex[subdivideLevel[0]->edge[-theFace->edge[3]]->end];
					glNormal3f(v->normal.x,v->normal.y,v->normal.z);
					glVertex3f(v->position.x,v->position.y,v->position.z);
				}
			//	glEnd();
			}
			glEnd();
			glDisable(GL_POLYGON_OFFSET_FILL);
			glLineWidth(1);
			glDisable(GL_LIGHTING);
			glColor3ub(0,0,0);
			glLineWidth(1);
			glBegin(GL_LINES);
			for(size_t i=1;i<subdivideLevel[0]->edge.size();++i)
			{
				SubdivideEdge *e=subdivideLevel[0]->edge[i];
				SubdivideVertex *start=subdivideLevel[0]->vertex[e->start];
				SubdivideVertex *end=subdivideLevel[0]->vertex[e->end];
				glVertex3f(start->position.x,start->position.y,start->position.z);
				glVertex3f(end->position.x,end->position.y,end->position.z);
			}
			glEnd();
			glEnable(GL_LIGHTING);
		}

	};


void Object::selectionRenderObject()
	{
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glTranslatef(position.x,position.y,position.z);
		glTranslatef(center.x,center.y,center.z);
		glRotatef(rotation.w,rotation.x,rotation.y,rotation.z);
		glScalef(scale.x,scale.y,scale.z);
		glTranslatef(-center.x,-center.y,-center.z);
		
		if(subdivideLevelSize==0)
		{
			for(size_t i=1;i<faceArray.size();++i)
			{
				if(faceArray[i])
				{
					Face *theFace=faceArray[i];
					glBegin(GL_POLYGON);
					for(size_t e=0;e<theFace->edge.size();++e)
					{
						if(theFace->edge[e]>0)
						{
							Vertex *v=vertexArray[edgeArray[theFace->edge[e]]->start];
							glVertex3f(v->position.x,v->position.y,v->position.z);
						}
						else
						{
							Vertex *v=vertexArray[edgeArray[-theFace->edge[e]]->end];
							glVertex3f(v->position.x,v->position.y,v->position.z);
						}
					}
					glEnd();
				}
			}
		}
		else
		{
			glBegin(GL_QUADS);
			for(size_t i=1;i<subdivideLevel[0]->face.size();++i)
			{
				SubdivideFace *theFace=subdivideLevel[0]->face[i];
				if(theFace->edge[0]>0)
				{
					SubdivideVertex *v=subdivideLevel[0]->vertex[subdivideLevel[0]->edge[theFace->edge[0]]->start];
					glVertex3f(v->position.x,v->position.y,v->position.z);
				}
				else
				{
					SubdivideVertex *v=subdivideLevel[0]->vertex[subdivideLevel[0]->edge[-theFace->edge[0]]->end];
					glVertex3f(v->position.x,v->position.y,v->position.z);
				}
				if(theFace->edge[1]>0)
				{
					SubdivideVertex *v=subdivideLevel[0]->vertex[subdivideLevel[0]->edge[theFace->edge[1]]->start];
					glVertex3f(v->position.x,v->position.y,v->position.z);
				}
				else
				{
					SubdivideVertex *v=subdivideLevel[0]->vertex[subdivideLevel[0]->edge[-theFace->edge[1]]->end];
					glVertex3f(v->position.x,v->position.y,v->position.z);
				}				
				if(theFace->edge[2]>0)
				{
					SubdivideVertex *v=subdivideLevel[0]->vertex[subdivideLevel[0]->edge[theFace->edge[2]]->start];
					glVertex3f(v->position.x,v->position.y,v->position.z);
				}
				else
				{
					SubdivideVertex *v=subdivideLevel[0]->vertex[subdivideLevel[0]->edge[-theFace->edge[2]]->end];
					glVertex3f(v->position.x,v->position.y,v->position.z);
				}
				if(theFace->edge[3]>0)
				{
					SubdivideVertex *v=subdivideLevel[0]->vertex[subdivideLevel[0]->edge[theFace->edge[3]]->start];
					glVertex3f(v->position.x,v->position.y,v->position.z);
				}
				else
				{
					SubdivideVertex *v=subdivideLevel[0]->vertex[subdivideLevel[0]->edge[-theFace->edge[3]]->end];
					glVertex3f(v->position.x,v->position.y,v->position.z);
				}
			}
			glEnd();
		}
		glPopMatrix();
	}

void Object::drawWireframeFaced()
	{
		glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
		glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
		glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
		glMaterialfv(GL_FRONT, GL_EMISSION, mat_emission);
		glEnable(GL_LIGHTING);
		if(subdivideLevelSize==0)
		{
			glEnable(GL_POLYGON_OFFSET_FILL);
			glPolygonOffset(1.0f,1.0f);
			for(size_t i=1;i<faceArray.size();++i)
			{
				if(faceArray[i])
				{
					Face *theFace=faceArray[i];
					glBegin(GL_POLYGON);
					glNormal3f(theFace->normal.x,theFace->normal.y,theFace->normal.z);
					for(size_t e=0;e<theFace->edge.size();++e)
					{
						if(theFace->edge[e]>0)
						{
							Vertex *v=vertexArray[edgeArray[theFace->edge[e]]->start];
							glVertex3f(v->position.x,v->position.y,v->position.z);
						}
						else
						{
							Vertex *v=vertexArray[edgeArray[-theFace->edge[e]]->end];
							glVertex3f(v->position.x,v->position.y,v->position.z);
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
			for(size_t i=1;i<edgeArray.size();++i)
			{
				Edge *e=edgeArray[i];
				if(e)
				{
					Vertex *start=vertexArray[e->start];
					Vertex *end=vertexArray[e->end];
					glVertex3f(start->position.x,start->position.y,start->position.z);
					glVertex3f(end->position.x,end->position.y,end->position.z);
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
			for(size_t i=1;i<subdivideLevel[0]->face.size();++i)
			{
				SubdivideFace *theFace=subdivideLevel[0]->face[i];
				glNormal3f(theFace->normal.x,theFace->normal.y,theFace->normal.z);
				if(theFace->edge[0]>0)
				{
					SubdivideVertex *v=subdivideLevel[0]->vertex[subdivideLevel[0]->edge[theFace->edge[0]]->start];
					glVertex3f(v->position.x,v->position.y,v->position.z);
				}
				else
				{
					SubdivideVertex *v=subdivideLevel[0]->vertex[subdivideLevel[0]->edge[-theFace->edge[0]]->end];
					glVertex3f(v->position.x,v->position.y,v->position.z);
				}
				if(theFace->edge[1]>0)
				{
					SubdivideVertex *v=subdivideLevel[0]->vertex[subdivideLevel[0]->edge[theFace->edge[1]]->start];
					glVertex3f(v->position.x,v->position.y,v->position.z);
				}
				else
				{
					SubdivideVertex *v=subdivideLevel[0]->vertex[subdivideLevel[0]->edge[-theFace->edge[1]]->end];
					glVertex3f(v->position.x,v->position.y,v->position.z);
				}				
				if(theFace->edge[2]>0)
				{
					SubdivideVertex *v=subdivideLevel[0]->vertex[subdivideLevel[0]->edge[theFace->edge[2]]->start];
					glVertex3f(v->position.x,v->position.y,v->position.z);
				}
				else
				{
					SubdivideVertex *v=subdivideLevel[0]->vertex[subdivideLevel[0]->edge[-theFace->edge[2]]->end];
					glVertex3f(v->position.x,v->position.y,v->position.z);
				}
				if(theFace->edge[3]>0)
				{
					SubdivideVertex *v=subdivideLevel[0]->vertex[subdivideLevel[0]->edge[theFace->edge[3]]->start];
					glVertex3f(v->position.x,v->position.y,v->position.z);
				}
				else
				{
					SubdivideVertex *v=subdivideLevel[0]->vertex[subdivideLevel[0]->edge[-theFace->edge[3]]->end];
					glVertex3f(v->position.x,v->position.y,v->position.z);
				}
			}
			glEnd();
			glDisable(GL_POLYGON_OFFSET_FILL);
			glLineWidth(1);
			glDisable(GL_LIGHTING);
			glColor3ub(0,0,0);
			glLineWidth(1);
			glBegin(GL_LINES);
			for(size_t i=1;i<subdivideLevel[0]->edge.size();++i)
			{
				SubdivideEdge *e=subdivideLevel[0]->edge[i];
				SubdivideVertex *start=subdivideLevel[0]->vertex[e->start];
				SubdivideVertex *end=subdivideLevel[0]->vertex[e->end];
				glVertex3f(start->position.x,start->position.y,start->position.z);
				glVertex3f(end->position.x,end->position.y,end->position.z);
			}
			glEnd();
			glEnable(GL_LIGHTING);
		}
	};

struct ObjectInfo Object::getObjectInfo()
	{
		struct ObjectInfo result;
		result.index=index;
		result.centerX=center.x;
		result.centerY=center.y;
		result.centerZ=center.z;

		result.positionX=position.x;
		result.positionY=position.y;
		result.positionZ=position.z;

		result.rotationX=rotation.x;
		result.rotationY=rotation.y;
		result.rotationZ=rotation.z;

		result.scaleX=scale.x;
		result.scaleY=scale.y;
		result.scaleZ=scale.z;

		result.mat_ambient[0]=mat_ambient[0];
		result.mat_ambient[1]=mat_ambient[1];
		result.mat_ambient[2]=mat_ambient[2];
		result.mat_ambient[3]=mat_ambient[3];

		result.mat_diffuse[0]=mat_diffuse[0];
		result.mat_diffuse[1]=mat_diffuse[1];
		result.mat_diffuse[2]=mat_diffuse[2];
		result.mat_diffuse[3]=mat_diffuse[3];

		result.mat_specular[0]=mat_specular[0];
		result.mat_specular[1]=mat_specular[1];
		result.mat_specular[2]=mat_specular[2];
		result.mat_specular[3]=mat_specular[3];

		result.mat_emission[0]=mat_emission[0];
		result.mat_emission[1]=mat_emission[1];
		result.mat_emission[2]=mat_emission[2];
		result.mat_emission[3]=mat_emission[3];

		result.mat_shininess[0]=mat_shininess[0];
		result.mat_shininess[1]=mat_shininess[1];
		result.mat_shininess[2]=mat_shininess[2];
		result.mat_shininess[3]=mat_shininess[3];

		result.vertexCount=vertexArray.size();
		result.edgeCount=edgeArray.size();
		result.faceCount=faceArray.size();
		return result;
	};

void Object::buildPSCacheFromEID(std::vector<size_t> &edgeToBeSub)
{
	if(subdivideLevelSize>0)
	{
		std::vector<size_t> vertexToBeSub;
		vertexToBeSub.reserve(1000);
		for(size_t i=0;i<edgeToBeSub.size();++i)
		{
			Edge *theEdge=edgeArray[edgeToBeSub[i]];
			if(theEdge)
			{
				if(!vertexArray[theEdge->start]->isSub)
				{
					vertexToBeSub.push_back(theEdge->start);
					vertexArray[theEdge->start]->isSub=true;
				}

				if(!vertexArray[theEdge->end]->isSub)
				{
					vertexToBeSub.push_back(theEdge->end);
					vertexArray[theEdge->start]->isSub=true;
				}
			}
		}

		for(size_t i=0;i<vertexToBeSub.size();++i)
		{
			vertexArray[vertexToBeSub[i]]->isSub=false;
		}

		buildPSCacheFromVID(vertexToBeSub);
	}
}

void Object::buildPSCacheFromFID(std::vector<size_t> &faceToBeSub)
{
	if(subdivideLevelSize>0)
	{
		std::vector<size_t> vertexToBeSub;
		vertexToBeSub.reserve(1000);
		for(size_t i=0;i<faceToBeSub.size();++i)
		{
			Face *theFace=faceArray[faceToBeSub[i]];
			for(size_t e=0;e<theFace->edge.size();++e)
			{
				if(theFace->edge[e]>0)
				{
					Edge *theEdge=edgeArray[theFace->edge[e]];
					if(!vertexArray[theEdge->start]->isSub)
					{
						vertexToBeSub.push_back(theEdge->start);
						vertexArray[theEdge->start]->isSub=true;
					}

					if(!vertexArray[theEdge->end]->isSub)
					{
						vertexToBeSub.push_back(theEdge->end);
						vertexArray[theEdge->end]->isSub=true;					
					}
				}
				else
				{
					Edge *theEdge=edgeArray[-theFace->edge[e]];
					if(!vertexArray[theEdge->start]->isSub)
					{
						vertexToBeSub.push_back(theEdge->start);
						vertexArray[theEdge->start]->isSub=true;
					}

					if(!vertexArray[theEdge->end]->isSub)
					{
						vertexToBeSub.push_back(theEdge->end);
						vertexArray[theEdge->end]->isSub=true;					
					}					
				}
			}
		}

		for(size_t i=0;i<vertexToBeSub.size();++i)
		{
			vertexArray[vertexToBeSub[i]]->isSub=false;
		}

		buildPSCacheFromVID(vertexToBeSub);
	}
}

void Object::buildPSCacheFromVID(std::vector<size_t> &vertexToBeSub)
{
	if(subdivideLevelSize>0)
	{
		std::vector<Face *> faceToBeSub;
		faceToBeSub.reserve(1000);
		for(size_t i=0;i<vertexToBeSub.size();++i)
		{
			Vertex *theVertex=vertexArray[vertexToBeSub[i]];
			if(theVertex)
			{
				for(size_t e=0;e<theVertex->adjacentEdge.size();++e)			
				{
					Edge *theEdge=edgeArray[theVertex->adjacentEdge[e]];
					if(theEdge)
					{
						if(theEdge->start==theVertex->index)
						{
							if(theEdge->right && !faceArray[theEdge->right]->isSub)						
							{
								faceToBeSub.push_back(faceArray[theEdge->right]);
								faceArray[theEdge->right]->isSub=true;
							}
						}
						else
						{
							if(theEdge->left && !faceArray[theEdge->left]->isSub)						
							{
								faceToBeSub.push_back(faceArray[theEdge->left]);
								faceArray[theEdge->left]->isSub=true;
							}
						}
					}
				}
			}
		}
		buildPSCache(faceToBeSub);
	}
}

void Object::expandSubFace(std::vector<SubdivideFace*> &originalList,size_t level)
{
	//这里originalList应该分配好空间
	//originalList中的id应该是待细分的状态
	SubdivideLevel *theLevel=subdivideLevel[level];
	size_t originalCount=originalList.size();
	std::vector<SubdivideVertex *> tempVertexList;
	tempVertexList.reserve(originalCount*4);
	for(size_t i=0;i<originalCount;++i)
	{
		SubdivideFace *theFace=originalList[i];
		for(size_t e=0;e<4;++e)
		{
			if(theFace->edge[e]>0)
			{
				SubdivideVertex *theVertex=theLevel->vertex[theLevel->edge[theFace->edge[e]]->end];
				if(!theVertex->isSub)
				{
					tempVertexList.push_back(theVertex);
					theVertex->isSub=true;
				}
			}
			else
			{
				SubdivideVertex *theVertex=theLevel->vertex[theLevel->edge[-theFace->edge[e]]->start];
				if(!theVertex->isSub)
				{
					tempVertexList.push_back(theVertex);
					theVertex->isSub=true;
				}
			}
		}
	}

	for(size_t i=0;i<tempVertexList.size();++i)
	{
		SubdivideVertex *theVertex=tempVertexList[i];
		theVertex->isSub=false;
		for(size_t e=0;e<theVertex->adjacentEdge.size();++e)
		{
			SubdivideEdge *theEdge=theLevel->edge[theVertex->adjacentEdge[e]];
			if(theEdge->start==theVertex->index)
			{
				SubdivideFace *theFace=theLevel->face[theEdge->right];
				if(theFace && !theFace->isSub)
				{
					originalList.push_back(theFace);
					theFace->isSub=true;
				}
			}
			else
			{
				SubdivideFace *theFace=theLevel->face[theEdge->left];
				if(theFace && !theFace->isSub)
				{
					originalList.push_back(theFace);
					theFace->isSub=true;
				}
			}
		}
	}
}

/*void Object::expandSubFace(std::vector<SubdivideFace*> &originalList,size_t level)
{
	//这里originalList应该分配好空间
	//originalList中的id应该是待细分的状态
	size_t originalCount=originalList.size();
	for(size_t i=0;i<originalCount;++i)
	{
		SubdivideFace *theFace=originalList[i];
		for(size_t e=0;e<4;++e)
		{
			SubdivideVertex *theVertex;
			SubdivideLevel *theLevel=subdivideLevel[level];
			if(theFace->edge[e]>0)
			{
				if(theLevel->edge[theFace->edge[e]]->left && theLevel->face[theLevel->edge[theFace->edge[e]]->left]->isSub)
					continue;
				theVertex=theLevel->vertex[theLevel->edge[theFace->edge[e]]->end];
			}
			else
			{
				if(theLevel->edge[-theFace->edge[e]]->right>0 && theLevel->face[theLevel->edge[-theFace->edge[e]]->right]->isSub)
					continue;
				theVertex=theLevel->vertex[theLevel->edge[-theFace->edge[e]]->start];
			}
			size_t adjacentEdgeCount=theVertex->adjacentEdge.size();
			for(size_t h=0;h<adjacentEdgeCount;++h)
			{
				if(theLevel->edge[theVertex->adjacentEdge[h]]->right && !theLevel->face[theLevel->edge[theVertex->adjacentEdge[h]]->right]->isSub)
				{
					SubdivideFace *theSubFace=theLevel->face[theLevel->edge[theVertex->adjacentEdge[h]]->right];
					originalList.push_back(theSubFace);
					theSubFace->isSub=true;
				}					
				if(theLevel->edge[theVertex->adjacentEdge[h]]->left && !theLevel->face[theLevel->edge[theVertex->adjacentEdge[h]]->left]->isSub)
				{
					SubdivideFace *theSubFace=theLevel->face[theLevel->edge[theVertex->adjacentEdge[h]]->left];
					originalList.push_back(theSubFace);
					theSubFace->isSub=true;
				}
			}
		}
	}
}*/

void Object::expandSubFace(std::vector<Face*> &originalList)
{
	//这里originalList应该分配好空间
	//originalList中的id应该是待细分的状态
	size_t originalCount=originalList.size();
	std::vector<Vertex *> tempVertexList;
	tempVertexList.reserve(originalCount*5);
	for(size_t i=0;i<originalCount;++i)
	{
		Face *theFace=originalList[i];
		size_t edgeCount=theFace->edge.size();
		for(size_t e=0;e<edgeCount;++e)
		{
			if(theFace->edge[e]>0)
			{
				Vertex *theVertex=vertexArray[edgeArray[theFace->edge[e]]->end];
				if(!theVertex->isSub)
				{
					tempVertexList.push_back(theVertex);
				}
			}
			else
			{
				Vertex *theVertex=vertexArray[edgeArray[-theFace->edge[e]]->start];
				if(!theVertex->isSub)
				{
					tempVertexList.push_back(theVertex);
				}
			}
		}
	}
	for(size_t i=0;i<tempVertexList.size();++i)
	{
		Vertex *theVertex=tempVertexList[i];
		theVertex->isSub=false;
		for(size_t e=0;e<theVertex->adjacentEdge.size();++e)
		{
			Edge *theEdge=edgeArray[theVertex->adjacentEdge[e]];
			if(theEdge->start==theVertex->index)
			{
				Face *theFace=faceArray[theEdge->right];
				if(theFace && !theFace->isSub)
				originalList.push_back(theFace);
			}
			else
			{
				Face *theFace=faceArray[theEdge->left];
				if(theFace && !theFace->isSub)
				originalList.push_back(theFace);			
			}
		}
	}
}

/*void Object::expandSubFace(std::vector<Face*> &originalList)
{
	//这里originalList应该分配好空间
	//originalList中的id应该是待细分的状态
	size_t originalCount=originalList.size();
	for(size_t i=0;i<originalCount;++i)
	{
		Face *theFace=originalList[i];
		size_t edgeCount=theFace->edge.size();
		for(size_t e=0;e<edgeCount;++e)
		{
			Vertex *theVertex;
			if(theFace->edge[e]>0)
			{
				if(edgeArray[theFace->edge[e]]->left && faceArray[edgeArray[theFace->edge[e]]->left]->isSub)
					continue;
				theVertex=vertexArray[edgeArray[theFace->edge[e]]->end];
			}
			else
			{
				if(edgeArray[-theFace->edge[e]]->right>0 && faceArray[edgeArray[-theFace->edge[e]]->right]->isSub)
					continue;
				theVertex=vertexArray[edgeArray[-theFace->edge[e]]->start];
			}
			size_t adjacentEdgeCount=theVertex->adjacentEdge.size();
			for(size_t h=0;h<adjacentEdgeCount;++h)
			{
				if(edgeArray[theVertex->adjacentEdge[h]]->right && !faceArray[edgeArray[theVertex->adjacentEdge[h]]->right]->isSub)
				{
					Face *theSubFace=faceArray[edgeArray[theVertex->adjacentEdge[h]]->right];
					originalList.push_back(theSubFace);
					theSubFace->isSub=true;
				}					
				if(edgeArray[theVertex->adjacentEdge[h]]->left && !faceArray[edgeArray[theVertex->adjacentEdge[h]]->left]->isSub)
				{
					Face *theSubFace=faceArray[edgeArray[theVertex->adjacentEdge[h]]->left];
					originalList.push_back(theSubFace);
					theSubFace->isSub=true;
				}
			}
		}
	}
}*/

void Object::buildPSCache(std::vector<Face*> &faceToBeSub)
{
	if(subdivideLevelSize>0)
	{
		size_t level=subdivideLevelSize-1;
		size_t faceCount=faceToBeSub.size();
		PSFaceCache.reserve(faceCount);
		PSSubFaceCache[level].reserve(faceCount*5);
		for(size_t i=0;i<faceCount;++i)
		{
			PSFaceCache.push_back(faceToBeSub[i]);
			faceToBeSub[i]->isSub=true;
			size_t subFaceCount=faceToBeSub[i]->subdivideFace.size();
			for(size_t h=0;h<subFaceCount;++h)
			{
				SubdivideFace *theSubF=subdivideLevel[level]->face[faceToBeSub[i]->subdivideFace[h]];
				PSSubFaceCache[level].push_back(theSubF);
				theSubF->isSub=true;
			}
		}
		expandSubFace(PSSubFaceCache[level],level);
		for(int h=level-1;h>0;--h)
		{
			size_t e2=h;
			size_t e1=h+1;
			faceCount=PSSubFaceCache[e1].size();
			PSSubFaceCache[e2].reserve(faceCount*5);
			for(size_t i=0;i<faceCount;++i)
			{
				SubdivideFace *theSubFace=subdivideLevel[e2]->face[(PSSubFaceCache[e1])[i]->subFace[0]];
				PSSubFaceCache[e2].push_back(theSubFace);
				theSubFace->isSub=true;
				theSubFace=subdivideLevel[e2]->face[(PSSubFaceCache[e1])[i]->subFace[1]];
				PSSubFaceCache[e2].push_back(theSubFace);
				theSubFace->isSub=true;
				theSubFace=subdivideLevel[e2]->face[(PSSubFaceCache[e1])[i]->subFace[2]];
				PSSubFaceCache[e2].push_back(theSubFace);
				theSubFace->isSub=true;
				theSubFace=subdivideLevel[e2]->face[(PSSubFaceCache[e1])[i]->subFace[3]];
				PSSubFaceCache[e2].push_back(theSubFace);
				theSubFace->isSub=true;
			}
			expandSubFace(PSSubFaceCache[e2],e2);
		}
	}		
}

void Object::updateAllNormal()
	{
		size_t faceCount=faceArray.size();
		for(size_t i=1;i<faceCount;++i)
		{
			if(faceArray[i])
			{
				updateFNormal(faceArray[i]);
			}
		}
		size_t vertexCount=vertexArray.size();
		for(size_t i=1;i<vertexCount;++i)
		{
			if(vertexArray[i])
			{
				updateVNormal(vertexArray[i]);
			}
		}
	}

void Object::partialSubdivision()
{
	if(subdivideLevelSize)
	{
		size_t level=subdivideLevelSize-1;
		++subdivideId;
		size_t faceCount=PSFaceCache.size();
		for(size_t i=0;i<faceCount;++i)
		{
			partialSubdivideFace(PSFaceCache[i],level);
		}
		for(size_t e=level;e>0;--e)
		{
			faceCount=PSSubFaceCache[e].size();
			for(size_t i=0;i<faceCount;++i)
			{
				partialSubdivideFace((PSSubFaceCache[e])[i],e);
			}
		}
	}
}

void Object::updateSubdivision()
	{
		size_t subdivideCount=subdivideLevelSize;
		for(size_t i=0;i<5;++i)
		{
			unSubdivide();
		}
		for(size_t i=0;i<subdivideCount;++i)
		{
			subdivide();
		}
	};


void Object::unSubdivide()
{
	if(subdivideLevelSize)
	{
		delete subdivideLevel[0];
		for(int i=1;i<subdivideLevelSize;i++)
		{
			subdivideLevel[i-1]=subdivideLevel[i];
		}
		--subdivideLevelSize;
		subdivideLevel[subdivideLevelSize]=NULL;
	}
}

void Object::partialSubdivideFace(SubdivideFace *theFace,int level)
{
	SubdivideLevel *theSubLevel=subdivideLevel[level-1];
	SubdivideLevel *theLevel=subdivideLevel[level];
	//首先要得到这个面的端点
	//得到端点和边的数目
	size_t edgeCount=4;
	//新建存放顶点的数组
	SubdivideVertex **theVertexList=new SubdivideVertex*[edgeCount];
	//计算这个面的中点
	//将原始的中点清零
	theSubLevel->vertex[theFace->center]->position.null();
	theSubLevel->vertex[theFace->center]->normal.null();
	for(size_t e=0;e<edgeCount;++e)
	{
		if(theFace->edge[e]<0)
		{
			theVertexList[e]=theLevel->vertex[theLevel->edge[-theFace->edge[e]]->start];
		}
		else
		{
			theVertexList[e]=theLevel->vertex[theLevel->edge[theFace->edge[e]]->end];
		}
		theSubLevel->vertex[theFace->center]->position+=theVertexList[e]->position;
		theSubLevel->vertex[theFace->center]->normal+=theVertexList[e]->normal;
	}
	theSubLevel->vertex[theFace->center]->position/=(float)edgeCount;
	theSubLevel->vertex[theFace->center]->normal/=(float)edgeCount;
	//计算新的顶点
	for(size_t i=0;i<edgeCount;++i)
	{
		SubdivideVertex *theV=theVertexList[i];
		if(theV->subdivideId!=subdivideId)
		{
			//如果是第一次计算到这个点
			//初始化细分步数
			theV->subdivideId=subdivideId;
			theV->subdivideStep=0;
			//生成这个点的细分点
			//首先计算相邻点的坐标和Ev
			theSubLevel->vertex[theV->nextLevel]->position.null();
			theSubLevel->vertex[theV->nextLevel]->position+=EAdjacentVertex(theV,level);
			if(theV->edgeVertex)
			{
				theSubLevel->vertex[theV->nextLevel]->position+=theV->position*6;	
				theSubLevel->vertex[theV->nextLevel]->position/=8;
			}
			else
			{
				//加上这个面的中点
				theSubLevel->vertex[theV->nextLevel]->position+=theSubLevel->vertex[theFace->center]->position;
				theV->subdivideStep++;
				size_t adjEdgeCount=theV->adjacentEdge.size();
				for(size_t h=0;h<adjEdgeCount;++h)
				{
					if(theLevel->edge[theV->adjacentEdge[h]]->end==theV->index)
					{
						if(!theLevel->face[theLevel->edge[theV->adjacentEdge[h]]->left]->isSub)
						{
							theSubLevel->vertex[theV->nextLevel]->position+=theSubLevel->vertex[theLevel->face[theLevel->edge[theV->adjacentEdge[h]]->left]->center]->position;
							theV->subdivideStep++;
						}
					}
					else
					{
						if(!theLevel->face[theLevel->edge[theV->adjacentEdge[h]]->right]->isSub)
						{
							theSubLevel->vertex[theV->nextLevel]->position+=theSubLevel->vertex[theLevel->face[theLevel->edge[theV->adjacentEdge[h]]->right]->center]->position;
							theV->subdivideStep++;
						}			
					}
				}
				size_t n=theV->adjacentEdge.size();
				if(n==(size_t)(theV->subdivideStep))
				{
					theSubLevel->vertex[theV->nextLevel]->position+=theV->position*(float)(n*n-2*n);
					theSubLevel->vertex[theV->nextLevel]->position/=(float)(n*n);
				}
			}
		}
		else
		{
			if(!theV->edgeVertex)
			{
				theSubLevel->vertex[theV->nextLevel]->position+=theSubLevel->vertex[theFace->center]->position;
				theV->subdivideStep++;
				size_t n=theV->adjacentEdge.size();
				if(n==(size_t)(theV->subdivideStep))
				{
					theSubLevel->vertex[theV->nextLevel]->position+=theV->position*(float)(n*n-2*n);
					theSubLevel->vertex[theV->nextLevel]->position/=(float)(n*n);
				}
			}
		}
	}
	delete theVertexList;
	//之后处理每一个边
	for(size_t i=0;i<edgeCount;++i)
	{
		SubdivideEdge *theEdge;
		if(theFace->edge[i]>0)
		{
			theEdge=theLevel->edge[theFace->edge[i]];
			if(theEdge->subdivideId!=subdivideId)
			{
				//如果是第一次处理这个边
				theEdge->subdivideId=subdivideId;
				theSubLevel->vertex[theEdge->middle]->position.null();
				theSubLevel->vertex[theEdge->middle]->position+=theLevel->vertex[theEdge->start]->position+theLevel->vertex[theEdge->end]->position;
				if(theEdge->left!=0 )
				{
					if(theLevel->face[theEdge->left]->isSub)
					{
						//如果这个边的两边都在局部细分的范围之内
						theSubLevel->vertex[theEdge->middle]->position+=theSubLevel->vertex[theFace->center]->position;
					}
					else
					{
						theSubLevel->vertex[theEdge->middle]->position+=theSubLevel->vertex[theLevel->face[theEdge->left]->center]->position+theSubLevel->vertex[theFace->center]->position;
						theSubLevel->vertex[theEdge->middle]->position/=4;
					}
				}
				else
				{
					theSubLevel->vertex[theEdge->middle]->position/=2;
				}
			}
			else
			{
				//如果不是第一次计算
				theSubLevel->vertex[theEdge->middle]->position+=theSubLevel->vertex[theFace->center]->position;
				theSubLevel->vertex[theEdge->middle]->position/=4;
			}
		}
		else
		{
			theEdge=theLevel->edge[-theFace->edge[i]];
			if(theEdge->subdivideId!=subdivideId)
			{
				//如果是第一次处理这个边
				theEdge->subdivideId=subdivideId;
				theSubLevel->vertex[theEdge->middle]->position.null();
				theSubLevel->vertex[theEdge->middle]->position+=theLevel->vertex[theEdge->start]->position+theLevel->vertex[theEdge->end]->position;
				if( theEdge->right!=0)
				{
					if(theLevel->face[theEdge->right]->isSub)
					{
						theSubLevel->vertex[theEdge->middle]->position+=theSubLevel->vertex[theFace->center]->position;
					}
					else
					{
						theSubLevel->vertex[theEdge->middle]->position+=theSubLevel->vertex[theLevel->face[theEdge->right]->center]->position+theSubLevel->vertex[theFace->center]->position;
						theSubLevel->vertex[theEdge->middle]->position/=4;
					}
				}
				else
				{
					theSubLevel->vertex[theEdge->middle]->position/=2;
				}
			}
			else
			{
				//如果不是第一次计算
				theSubLevel->vertex[theEdge->middle]->position+=theSubLevel->vertex[theFace->center]->position;
				theSubLevel->vertex[theEdge->middle]->position/=4;
			}
		}
	}
}

void Object::buildPSCacheFast(std::vector<Face*> &faceToBeSub)
{
	if(subdivideLevelSize>0)
	{
		size_t subCount=faceToBeSub.size();
		PSFaceCache.reserve(subCount*2);
		for(size_t i=0;i<subCount;++i)
		{
			PSFaceCache.push_back(faceToBeSub[i]);
			faceToBeSub[i]->isSub=true;
		}
		expandSubFace(faceToBeSub);
		subCount=PSFaceCache.size();
		size_t level=subdivideLevelSize-1;
		PSSubFaceCache[level].reserve(5*subCount);
		for(size_t i=0;i<subCount;++i)
		{
			size_t subFaceCount=PSFaceCache[i]->subdivideFace.size();
			for(size_t h=0;h<subFaceCount;++h)
			{
				PSSubFaceCache[level].push_back(subdivideLevel[level]->face[PSFaceCache[i]->subdivideFace[h]]);
				subdivideLevel[level]->face[PSFaceCache[i]->subdivideFace[h]]->isSub=true;
			}
		}
		for(size_t e=level;e>0;--e)
		{
			size_t subCount=PSSubFaceCache[e].size();
			size_t e2=e-1;
			PSSubFaceCache[e2].reserve(subCount*2);
			for(size_t i=0;i<subCount;++i)
			{
				PSSubFaceCache[e2].push_back(subdivideLevel[e2]->face[(PSSubFaceCache[level])[i]->subFace[0]]);
				subdivideLevel[e2]->face[(PSSubFaceCache[level])[i]->subFace[0]]->isSub=true;
				PSSubFaceCache[e2].push_back(subdivideLevel[e2]->face[(PSSubFaceCache[level])[i]->subFace[1]]);
				subdivideLevel[e2]->face[(PSSubFaceCache[level])[i]->subFace[1]]->isSub=true;
				PSSubFaceCache[e2].push_back(subdivideLevel[e2]->face[(PSSubFaceCache[level])[i]->subFace[2]]);
				subdivideLevel[e2]->face[(PSSubFaceCache[level])[i]->subFace[2]]->isSub=true;
				PSSubFaceCache[e2].push_back(subdivideLevel[e2]->face[(PSSubFaceCache[level])[i]->subFace[3]]);
				subdivideLevel[e2]->face[(PSSubFaceCache[level])[i]->subFace[3]]->isSub=true;
			}
		}
	}
}

void Object::partialSubdivideFace(Face *theFace,int level)
{
	//首先要得到这个面的端点
	//得到端点和边的数目
	size_t edgeCount=theFace->edge.size();
	SubdivideLevel *theLevel=subdivideLevel[level];
	//新建存放顶点的数组
	Vertex **theVertexList=new Vertex*[edgeCount];
	theLevel->vertex[theFace->center]->position.null();
	theLevel->vertex[theFace->center]->normal.null();
	for(size_t e=0;e<edgeCount;++e)
	{
		if(theFace->edge[e]<0)
		{
			theVertexList[e]=vertexArray[edgeArray[-theFace->edge[e]]->start];
		}
		else
		{
			theVertexList[e]=vertexArray[edgeArray[theFace->edge[e]]->end];
		}
		theLevel->vertex[theFace->center]->position+=theVertexList[e]->position;
		theLevel->vertex[theFace->center]->normal+=theVertexList[e]->normal;
	}
	//计算这个面的中点
	theLevel->vertex[theFace->center]->position/=(float)edgeCount;
	theLevel->vertex[theFace->center]->normal/=(float)edgeCount;
	//计算新的顶点
	for(size_t i=0;i<edgeCount;++i)
	{
		Vertex *theV=theVertexList[i];
		if(theV->subdivideId!=subdivideId)
		{
			//如果是第一次计算到这个点
			//初始化细分步数
			theV->subdivideId=subdivideId;
			theV->subdivideStep=0;
			//生成这个点的细分点
			//首先计算相邻点的坐标和Ev
			theLevel->vertex[theV->nextLevel]->position.null();
			theLevel->vertex[theV->nextLevel]->position+=EAdjacentVertex(theV);
			if(theV->edgeVertex)
			{
				theLevel->vertex[theV->nextLevel]->position+=theV->position*6;	
				theLevel->vertex[theV->nextLevel]->position/=8;
			}
			else
			{
				//加上这个面的中点
				theLevel->vertex[theV->nextLevel]->position+=theLevel->vertex[theFace->center]->position;
				theV->subdivideStep++;
				size_t adjEdgeCount=theV->adjacentEdge.size();
				for(size_t h=0;h<adjEdgeCount;++h)
				{
					if(edgeArray[theV->adjacentEdge[h]]->end==theV->index)
					{
						if(!faceArray[edgeArray[theV->adjacentEdge[h]]->left]->isSub)
						{
							theLevel->vertex[theV->nextLevel]->position+=theLevel->vertex[faceArray[edgeArray[theV->adjacentEdge[h]]->left]->center]->position;
							++(theV->subdivideStep);
						}
					}
					else
					{
						if(!faceArray[edgeArray[theV->adjacentEdge[h]]->right]->isSub)
						{
							theLevel->vertex[theV->nextLevel]->position+=theLevel->vertex[faceArray[edgeArray[theV->adjacentEdge[h]]->right]->center]->position;
							++(theV->subdivideStep);
						}			
					}
				}
				size_t n=theV->adjacentEdge.size();
				if(n==(size_t)(theV->subdivideStep))
				{
					theLevel->vertex[theV->nextLevel]->position+=theV->position*(float)(n*n-2*n);
					theLevel->vertex[theV->nextLevel]->position/=(float)(n*n);
				}
			}
		}
		else
		{
			if(!theV->edgeVertex)
			{
				theLevel->vertex[theV->nextLevel]->position+=theLevel->vertex[theFace->center]->position;
				++(theV->subdivideStep);
				size_t n=theV->adjacentEdge.size();
				if(n==(size_t)(theV->subdivideStep))
				{
					theLevel->vertex[theV->nextLevel]->position+=theVertexList[i]->position*(float)(n*n-2*n);
					theLevel->vertex[theV->nextLevel]->position/=(float)(n*n);
				}
			}
		}
	}
	delete theVertexList;
	//之后处理每一个边
	for(size_t i=0;i<edgeCount;++i)
	{
		Edge *theEdge;
		if(theFace->edge[i]>0)
		{
			theEdge=edgeArray[theFace->edge[i]];
			if(theEdge->subdivideId!=subdivideId)
			{
				//如果是第一次处理这个边
				theEdge->subdivideId=subdivideId;
				theLevel->vertex[theEdge->middle]->position.null();
				theLevel->vertex[theEdge->middle]->position+=vertexArray[theEdge->start]->position+vertexArray[theEdge->end]->position;
				if(theEdge->left)
				{
					if(faceArray[theEdge->left]->isSub)
					{
						//如果这个边的两边都在局部细分的范围之内
						theLevel->vertex[theEdge->middle]->position+=theLevel->vertex[theFace->center]->position;
					}
					else
					{
						theLevel->vertex[theEdge->middle]->position+=theLevel->vertex[faceArray[theEdge->left]->center]->position+theLevel->vertex[theFace->center]->position;
						theLevel->vertex[theEdge->middle]->position/=4;
					}
				}
				else
				{
					theLevel->vertex[theEdge->middle]->position/=2;
				}
			}
			else
			{
				//如果不是第一次计算
				theLevel->vertex[theEdge->middle]->position+=theLevel->vertex[theFace->center]->position;
				theLevel->vertex[theEdge->middle]->position/=4;
			}
		}
		else
		{
			theEdge=edgeArray[-theFace->edge[i]];
			if(theEdge->subdivideId!=subdivideId)
			{
				//如果是第一次处理这个边
				theEdge->subdivideId=subdivideId;
				theLevel->vertex[theEdge->middle]->position.null();
				theLevel->vertex[theEdge->middle]->position+=vertexArray[theEdge->start]->position+vertexArray[theEdge->end]->position;
				if(theEdge->right)
				{
					if(faceArray[theEdge->right]->isSub)
					{
						theLevel->vertex[theEdge->middle]->position+=theLevel->vertex[theFace->center]->position;
					}
					else
					{
						theLevel->vertex[theEdge->middle]->position+=theLevel->vertex[faceArray[theEdge->right]->center]->position+theLevel->vertex[theFace->center]->position;
						theLevel->vertex[theEdge->middle]->position/=4;
					}
				}
				else
				{
					theLevel->vertex[theEdge->middle]->position/=2;
				}
			}
			else
			{
				//如果不是第一次计算
				theLevel->vertex[theEdge->middle]->position+=theLevel->vertex[theFace->center]->position;
				theLevel->vertex[theEdge->middle]->position/=4;
			}
		}
	}
}


Object::~Object(void)
{
}
