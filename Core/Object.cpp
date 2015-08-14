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

    //vertexArray.clear();
	vertexArray.reserve(5000);
    //edgeArray.clear();
	edgeArray.reserve(5000);
    //faceArray.clear();
	faceArray.reserve(5000);
	subdivideLevel[0]=NULL;
	subdivideLevel[1]=NULL;
	subdivideLevel[2]=NULL;
	subdivideLevel[3]=NULL;
	subdivideLevel[4]=NULL;
	renderMode=RenderType::WireframeFaced;
}


unsigned int Object::addVertex(float p1,float p2,float p3)
{
    unsigned int vi=vertexArray.add(new Vertex(p1,p2,p3));
	historyManager->record(new Log_ObjectVertexAdd(index,vi));
	return vi;
}

unsigned int Object::addVertex(float p1,float p2,float p3,float n1,float n2,float n3)
{
    unsigned int vi=vertexArray.add(new Vertex(p1,p2,p3,n1,n2,n3));
	historyManager->record(new Log_ObjectVertexAdd(index,vi));
	return vi;
}

unsigned int Object::addEdge(unsigned int ei,Edge *theE)
	{
		edgeArray.addI(ei,theE);
		historyManager->record(new Log_ObjectEdgeAdd(index,ei));
		return ei;
    }

unsigned int Object::addFace(unsigned int ei,Face *theF)
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

unsigned int Object::addVertex(Vector &pos)
{
    unsigned int vi=vertexArray.add(new Vertex(pos));
	historyManager->record(new Log_ObjectVertexAdd(index,vi));
	return vi;
}

unsigned int Object::addVertex(unsigned int ei,Vertex *theV)
{
	vertexArray.addI(ei,theV);
	historyManager->record(new Log_ObjectVertexAdd(index,ei));
	return ei;
}

unsigned int Object::addVertex(Vector &pos,Vector &nor)
{
    unsigned int vi=vertexArray.add(new Vertex(pos,nor));
	historyManager->record(new Log_ObjectVertexAdd(index,vi));
	return vi;
}

void Object::testOut(char *fileName)
{
	FILE *fp=fopen(fileName,"w");
    for(unsigned int i=1;i<vertexArray.size();i++)
	{
		if(vertexArray[i]==NULL)
		{
			fprintf(fp,"v %f %f %f\n",0,0,0);
		}
		else
		{
            fprintf(fp,"v %f %f %f\n",vertexArray[i]->m_position.x,vertexArray[i]->m_position.y,vertexArray[i]->m_position.z);
		}
	}
	fprintf(fp,"g box01\n");
    for(unsigned int i=1;i<faceArray.size();i++)
	{
		if(faceArray[i]==NULL) continue;
		fprintf(fp,"f ");
        for(unsigned int e=0;e<faceArray[i]->edge.size();e++)
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
}

unsigned int Object::addEdge(int start,int end)
{
	//printf("cao0");
    unsigned int ei=edgeArray.add(new Edge(start,end));
	//printf("cao0.5");
//	printf("!%d,%d!",index,ei);
	Log_ObjectEdgeAdd *theLog=new Log_ObjectEdgeAdd(index,ei);
	//printf("cao0.6");
	historyManager->record(theLog);
	//printf("cao1");
    vertexArray[start]->m_adjacentEdgeList.push_back((int)ei);
	historyManager->record(new Log_VertexAdjacentPush(index,start));
	//printf("cao2");
    vertexArray[end]->m_adjacentEdgeList.push_back((int)ei);
	historyManager->record(new Log_VertexAdjacentPush(index,end));
	//printf("cao3");
	return ei;
}

Vertex * Object::vertex(unsigned int index)
{
	return vertexArray[index];
}

Edge * Object::edge(unsigned int index)
{
	return edgeArray[index];
}

Face * Object::face(unsigned int index)
{
	return faceArray[index];
}

unsigned int Object::addFace(unsigned int theEdgeArray[],unsigned int size)
{
	//printf("start\n");
	Face *theFace=new Face();
    unsigned int resultIndex=faceArray.add(theFace);
	historyManager->record(new Log_ObjectFaceAdd(index,resultIndex));
    for(unsigned int i=0;i<size;++i)
	{
        unsigned int i1=theEdgeArray[i];
        unsigned int i2=theEdgeArray[(i+1)%size];
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
}

void Object::SubTestOut(char *fileName,int level)
{
	FILE *fp=NULL;
	fp=fopen(fileName,"w");
	SubdivideLevel *theSub=subdivideLevel[level];
    unsigned int vertexCount=theSub->vertex.size();
    for(unsigned int i=1;i<vertexCount;++i)
	{
        fprintf(fp,"v %f %f %f\n",theSub->vertex[i]->m_position.x,theSub->vertex[i]->m_position.y,theSub->vertex[i]->m_position.z);
	}
	fprintf(fp,"g box01\n");
    for(unsigned int i=1;i<theSub->face.size();++i)
	{
		fprintf(fp,"f ");
        for(unsigned int e=0;e<4;e++)
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
	//µÝÔöÏ¸·ÖµÄ±àºÅ
	++subdivideId;
	if(!subdivideLevelSize)
	{
		subdivideLevel[0]=new SubdivideLevel(vertexArray.size()+edgeArray.size()+faceArray.size(),edgeArray.size()*4,edgeArray.size()*2);
		++subdivideLevelSize;
        unsigned int faceCount=  faceArray.size();
        for(unsigned int i=1;i<faceCount;++i)
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
        unsigned int faceCount=subdivideLevel[1]->face.size();
        for(unsigned int i=1;i<faceCount;i++)
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
    theVertex->m_edgeVertex=false;
	Vector result(0);
    unsigned int vertexCount=theVertex->m_adjacentEdgeList.size();
    for(unsigned int i=0;i<vertexCount;++i)
	{
        if(edgeArray[theVertex->m_adjacentEdgeList[i]]->left && edgeArray[theVertex->m_adjacentEdgeList[i]]->right && !theVertex->m_edgeVertex)
		{
            if(theVertex->index==edgeArray[theVertex->m_adjacentEdgeList[i]]->end)
			{
                result+=vertexArray[edgeArray[theVertex->m_adjacentEdgeList[i]]->start]->m_position;
			}
			else
			{
                result+=vertexArray[edgeArray[theVertex->m_adjacentEdgeList[i]]->end]->m_position;
			}
		}
		else
		{
            if(!theVertex->m_edgeVertex)
			{
                theVertex->m_edgeVertex=true;
				result.null();
			}
            if(!edgeArray[theVertex->m_adjacentEdgeList[i]]->left || !edgeArray[theVertex->m_adjacentEdgeList[i]]->right)
			{
                if(theVertex->index==edgeArray[theVertex->m_adjacentEdgeList[i]]->end)
				{
                    result+=vertexArray[edgeArray[theVertex->m_adjacentEdgeList[i]]->start]->m_position;
				}
				else
				{
                    result+=vertexArray[edgeArray[theVertex->m_adjacentEdgeList[i]]->end]->m_position;
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
        if(subdivideLevel[level]->edge[theVertex->m_adjacentEdgeList[i]]->left!=0 && subdivideLevel[level]->edge[theVertex->m_adjacentEdgeList[i]]->right!=0 && !theVertex->m_edgeVertex)
		{
            if(theVertex->index==subdivideLevel[level]->edge[theVertex->m_adjacentEdgeList[i]]->end)
			{
                result+=subdivideLevel[level]->vertex[subdivideLevel[level]->edge[theVertex->m_adjacentEdgeList[i]]->start]->m_position;
			}
			else
			{
                result+=subdivideLevel[level]->vertex[subdivideLevel[level]->edge[theVertex->m_adjacentEdgeList[i]]->end]->m_position;
			}
		}
		else
		{
            if(!theVertex->m_edgeVertex)
			{
                theVertex->m_edgeVertex=true;
				result.null();
			}
            if(subdivideLevel[level]->edge[theVertex->m_adjacentEdgeList[i]]->left==0 || subdivideLevel[level]->edge[theVertex->m_adjacentEdgeList[i]]->right==0)
			{
                if(theVertex->index==subdivideLevel[level]->edge[theVertex->m_adjacentEdgeList[i]]->end)
				{
                    result+=subdivideLevel[level]->vertex[subdivideLevel[level]->edge[theVertex->m_adjacentEdgeList[i]]->start]->m_position;
				}
				else
				{
                    result+=subdivideLevel[level]->vertex[subdivideLevel[level]->edge[theVertex->m_adjacentEdgeList[i]]->end]->m_position;
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
        if(subdivideLevel[1]->edge[theVertex->m_adjacentEdgeList[i]]->left!=0 && subdivideLevel[1]->edge[theVertex->m_adjacentEdgeList[i]]->right!=0 && !theVertex->m_edgeVertex)
		{
            if(theVertex->index==subdivideLevel[1]->edge[theVertex->m_adjacentEdgeList[i]]->end)
			{
                result+=subdivideLevel[1]->vertex[subdivideLevel[1]->edge[theVertex->m_adjacentEdgeList[i]]->start]->m_position;
			}
			else
			{
                result+=subdivideLevel[1]->vertex[subdivideLevel[1]->edge[theVertex->m_adjacentEdgeList[i]]->end]->m_position;
			}
		}
		else
		{
            if(!theVertex->m_edgeVertex)
			{
                theVertex->m_edgeVertex=true;
				result.null();
			}
            if(subdivideLevel[1]->edge[theVertex->m_adjacentEdgeList[i]]->left==0 || subdivideLevel[1]->edge[theVertex->m_adjacentEdgeList[i]]->right==0)
			{
                if(theVertex->index==subdivideLevel[1]->edge[theVertex->m_adjacentEdgeList[i]]->end)
				{
                    result+=subdivideLevel[1]->vertex[subdivideLevel[1]->edge[theVertex->m_adjacentEdgeList[i]]->start]->m_position;
				}
				else
				{
                    result+=subdivideLevel[1]->vertex[subdivideLevel[1]->edge[theVertex->m_adjacentEdgeList[i]]->end]->m_position;
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
    unsigned int edgeCount=theFace->edge.size();
	//ÐÂ½¨´æ·Å¶¥µãµÄÊý×é
	Vertex **theVertexList=new Vertex*[edgeCount];
	theFace->center=subdivideLevel[0]->vertex.add(new SubdivideVertex());
    //subdivideLevel[0]->vertex[theFace->center]->m_adjacentEdgeList.clear();
    subdivideLevel[0]->vertex[theFace->center]->m_adjacentEdgeList.reserve(4);
    for(unsigned int e=0;e<edgeCount;++e)
	{
		if(theFace->edge[e]<0)
		{
			theVertexList[e]=vertexArray[edgeArray[-theFace->edge[e]]->start];
		}
		else
		{
			theVertexList[e]=vertexArray[edgeArray[theFace->edge[e]]->end];
		}
        subdivideLevel[0]->vertex[theFace->center]->m_position+=theVertexList[e]->m_position;
		//subdivideLevel[0]->vertex[theFace->center]->normal+=theVertexList[e]->normal;
	}
    subdivideLevel[0]->vertex[theFace->center]->m_position/=(float)edgeCount;
	//subdivideLevel[0]->vertex[theFace->center]->normal/=edgeCount;
	//¼ÆËãÐÂµÄ¶¥µã
    for(unsigned int i=0;i<edgeCount;++i)
	{
		Vertex *theV=theVertexList[i];
        if(theV->m_subdivideId!=subdivideId)
		{
			//Èç¹ûÊÇµÚÒ»´Î¼ÆËãµ½Õâ¸öµã
			//³õÊ¼»¯Ï¸·Ö²½Êý
            theV->m_subdivideId=subdivideId;
            theV->m_subdivideStep=0;
			//Éú³ÉÕâ¸öµãµÄÏ¸·Öµã
            theV->m_nextLevel=subdivideLevel[0]->vertex.add(new SubdivideVertex());
            //subdivideLevel[0]->vertex[theV->m_nextLevel]->m_adjacentEdgeList.clear();
            subdivideLevel[0]->vertex[theV->m_nextLevel]->m_adjacentEdgeList.reserve(theV->m_adjacentEdgeList.size());
			//Ê×ÏÈ¼ÆËãÏàÁÚµãµÄ×ø±êºÍEv
            subdivideLevel[0]->vertex[theV->m_nextLevel]->m_position+=EAdjacentVertex(theV);
            if(theV->m_edgeVertex)
			{
                subdivideLevel[0]->vertex[theV->m_nextLevel]->m_position+=theV->m_position*6;
                subdivideLevel[0]->vertex[theV->m_nextLevel]->m_position/=8;
			}
			else
			{
				//¼ÓÉÏÕâ¸öÃæµÄÖÐµã
                subdivideLevel[0]->vertex[theV->m_nextLevel]->m_position+=subdivideLevel[0]->vertex[theFace->center]->m_position;
			}
            ++(theV->m_subdivideStep);
		}
		else
		{
            if(!theV->m_edgeVertex)
			{
                subdivideLevel[0]->vertex[theV->m_nextLevel]->m_position+=subdivideLevel[0]->vertex[theFace->center]->m_position;
                ++(theV->m_subdivideStep);
                unsigned int n=theV->m_adjacentEdgeList.size();
                if(n==(unsigned int)theV->m_subdivideStep)
				{
                    subdivideLevel[0]->vertex[theV->m_nextLevel]->m_position+=theV->m_position*(float)(n*n-2*n);
                    subdivideLevel[0]->vertex[theV->m_nextLevel]->m_position/=(float)(n*n);
				}
			}
		}
	}
	delete theVertexList;
	//Ö®ºó´¦ÀíÃ¿Ò»¸ö±ß
    for(unsigned int i=0;i<edgeCount;++i)
	{
		Edge *theEdge;
		if(theFace->edge[i]>0)
		{
			theEdge=edgeArray[theFace->edge[i]];
			if(theEdge->subdivideId!=subdivideId)
			{
				//Èç¹ûÊÇµÚÒ»´Î´¦ÀíÕâ¸ö±ß
				theEdge->subdivideId=subdivideId;
                Vector theN=vertexArray[theEdge->start]->m_position+vertexArray[theEdge->end]->m_position;
				theEdge->middle=subdivideLevel[0]->vertex.add(new SubdivideVertex(theN));
                //subdivideLevel[0]->vertex[theEdge->middle]->m_adjacentEdgeList.clear();
                subdivideLevel[0]->vertex[theEdge->middle]->m_adjacentEdgeList.reserve(4);
				if(theEdge->left && theEdge->right)
				{
                    subdivideLevel[0]->vertex[theEdge->middle]->m_position+=subdivideLevel[0]->vertex[theFace->center]->m_position;
				}
				else
				{
                    subdivideLevel[0]->vertex[theEdge->middle]->m_position/=2;
				}
                theEdge->startEdge=subdivideLevel[0]->edge.add(new SubdivideEdge(vertexArray[theEdge->start]->m_nextLevel,theEdge->middle));
                subdivideLevel[0]->vertex[vertexArray[theEdge->start]->m_nextLevel]->m_adjacentEdgeList.push_back(theEdge->startEdge);
                subdivideLevel[0]->vertex[theEdge->middle]->m_adjacentEdgeList.push_back(theEdge->startEdge);
                theEdge->endEdge=subdivideLevel[0]->edge.add(new SubdivideEdge(theEdge->middle,vertexArray[theEdge->end]->m_nextLevel));
                subdivideLevel[0]->vertex[vertexArray[theEdge->end]->m_nextLevel]->m_adjacentEdgeList.push_back(theEdge->endEdge);
                subdivideLevel[0]->vertex[theEdge->middle]->m_adjacentEdgeList.push_back(theEdge->endEdge);
				theEdge->rightEdge=subdivideLevel[0]->edge.add(new SubdivideEdge(theEdge->middle,theFace->center));
                subdivideLevel[0]->vertex[theEdge->middle]->m_adjacentEdgeList.push_back(theEdge->rightEdge);
                subdivideLevel[0]->vertex[theFace->center]->m_adjacentEdgeList.push_back(theEdge->rightEdge);
			}
			else
			{
				//Èç¹û²»ÊÇµÚÒ»´Î¼ÆËã
                subdivideLevel[0]->vertex[theEdge->middle]->m_position+=subdivideLevel[0]->vertex[theFace->center]->m_position;
                subdivideLevel[0]->vertex[theEdge->middle]->m_position/=4;
				theEdge->rightEdge=subdivideLevel[0]->edge.add(new SubdivideEdge(theEdge->middle,theFace->center));
                subdivideLevel[0]->vertex[theEdge->middle]->m_adjacentEdgeList.push_back(theEdge->rightEdge);
                subdivideLevel[0]->vertex[theFace->center]->m_adjacentEdgeList.push_back(theEdge->rightEdge);
			}
		}
		else
		{
			theEdge=edgeArray[-theFace->edge[i]];
			if(theEdge->subdivideId!=subdivideId)
			{
				//Èç¹ûÊÇµÚÒ»´Î´¦ÀíÕâ¸ö±ß
				theEdge->subdivideId=subdivideId;
                Vector theN=vertexArray[theEdge->start]->m_position+vertexArray[theEdge->end]->m_position;
				theEdge->middle=subdivideLevel[0]->vertex.add(new SubdivideVertex(theN));
                //subdivideLevel[0]->vertex[theEdge->middle]->m_adjacentEdgeList.clear();
                subdivideLevel[0]->vertex[theEdge->middle]->m_adjacentEdgeList.reserve(4);
				if(theEdge->left!=0 && theEdge->right!=0)
				{
                    subdivideLevel[0]->vertex[theEdge->middle]->m_position+=subdivideLevel[0]->vertex[theFace->center]->m_position;
				}
				else
				{
                    subdivideLevel[0]->vertex[theEdge->middle]->m_position/=2;
				}
                theEdge->startEdge=subdivideLevel[0]->edge.add(new SubdivideEdge(vertexArray[theEdge->start]->m_nextLevel,theEdge->middle));
                subdivideLevel[0]->vertex[vertexArray[theEdge->start]->m_nextLevel]->m_adjacentEdgeList.push_back(theEdge->startEdge);
                subdivideLevel[0]->vertex[theEdge->middle]->m_adjacentEdgeList.push_back(theEdge->startEdge);
                theEdge->endEdge=subdivideLevel[0]->edge.add(new SubdivideEdge(theEdge->middle,vertexArray[theEdge->end]->m_nextLevel));
                subdivideLevel[0]->vertex[vertexArray[theEdge->end]->m_nextLevel]->m_adjacentEdgeList.push_back(theEdge->endEdge);
                subdivideLevel[0]->vertex[theEdge->middle]->m_adjacentEdgeList.push_back(theEdge->endEdge);
				theEdge->leftEdge=subdivideLevel[0]->edge.add(new SubdivideEdge(theEdge->middle,theFace->center));
                subdivideLevel[0]->vertex[theEdge->middle]->m_adjacentEdgeList.push_back(theEdge->leftEdge);
                subdivideLevel[0]->vertex[theFace->center]->m_adjacentEdgeList.push_back(theEdge->leftEdge);
			}
			else
			{
				//Èç¹û²»ÊÇµÚÒ»´Î¼ÆËã
                subdivideLevel[0]->vertex[theEdge->middle]->m_position+=subdivideLevel[0]->vertex[theFace->center]->m_position;
                subdivideLevel[0]->vertex[theEdge->middle]->m_position/=4;
				theEdge->leftEdge=subdivideLevel[0]->edge.add(new SubdivideEdge(theEdge->middle,theFace->center));
                subdivideLevel[0]->vertex[theEdge->middle]->m_adjacentEdgeList.push_back(theEdge->leftEdge);
                subdivideLevel[0]->vertex[theFace->center]->m_adjacentEdgeList.push_back(theEdge->leftEdge);
			}
		}
	}
	//½¨Á¢Ï¸·ÖÖ®ºóµÄÃæ
    //theFace->subdivideFace.clear();
	theFace->subdivideFace.reserve(edgeCount);
    for(unsigned int i=0;i<edgeCount;++i)
	{
        unsigned int i2=(i+1)%edgeCount;
		if(theFace->edge[i]>0)
		{
			Edge *theEdge1=edgeArray[theFace->edge[i]];
			if(theFace->edge[i2]>0)
			{
				Edge *theEdge2=edgeArray[theFace->edge[i2]];
                unsigned int faceId=
				subdivideLevel[0]->edge[theEdge1->rightEdge]->left=
				subdivideLevel[0]->edge[theEdge1->endEdge]->right=
				subdivideLevel[0]->edge[theEdge2->startEdge]->right=
				subdivideLevel[0]->edge[theEdge2->rightEdge]->right=subdivideLevel[0]->face.add(new SubdivideFace(-theEdge1->rightEdge,theEdge1->endEdge,theEdge2->startEdge,theEdge2->rightEdge));
				theFace->subdivideFace.push_back(faceId);
			}
			else
			{
				Edge *theEdge2=edgeArray[-theFace->edge[i2]];
                unsigned int faceId=
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
                unsigned int faceId=
				subdivideLevel[0]->edge[theEdge1->leftEdge]->left=
				subdivideLevel[0]->edge[theEdge1->startEdge]->left=
				subdivideLevel[0]->edge[theEdge2->startEdge]->right=
				subdivideLevel[0]->edge[theEdge2->rightEdge]->right=subdivideLevel[0]->face.add(new SubdivideFace(-theEdge1->leftEdge,-theEdge1->startEdge,theEdge2->startEdge,theEdge2->rightEdge));
				theFace->subdivideFace.push_back(faceId);
			}
			else
			{
				Edge *theEdge2=edgeArray[-theFace->edge[i2]];
                unsigned int faceId=
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
	//Ê×ÏÈÒªµÃµ½Õâ¸öÃæµÄ¶Ëµã
	//µÃµ½¶ËµãºÍ±ßµÄÊýÄ¿
    unsigned int edgeCount=4;
	SubdivideLevel *target=subdivideLevel[0];
	SubdivideLevel *original=subdivideLevel[1];
	//ÐÂ½¨´æ·Å¶¥µãµÄÊý×é
	SubdivideVertex **theVertexList=new SubdivideVertex*[edgeCount];
	theFace->center=target->vertex.add(new SubdivideVertex());
    //target->vertex[theFace->center]->m_adjacentEdgeList.clear();
    target->vertex[theFace->center]->m_adjacentEdgeList.reserve(4);
    for(unsigned int e=0;e<edgeCount;++e)
	{
		if(theFace->edge[e]<0)
		{
			theVertexList[e]=original->vertex[original->edge[-theFace->edge[e]]->start];
		}
		else
		{
			theVertexList[e]=original->vertex[original->edge[theFace->edge[e]]->end];
		}
        target->vertex[theFace->center]->m_position+=theVertexList[e]->m_position;
		//target->vertex[theFace->center]->normal+=theVertexList[e]->normal;
	}
	//¼ÆËãÕâ¸öÃæµÄÖÐµã
    target->vertex[theFace->center]->m_position/=(float)edgeCount;
	//target->vertex[theFace->center]->normal/=edgeCount;
	//¼ÆËãÐÂµÄ¶¥µã
    for(unsigned int i=0;i<edgeCount;++i)
	{
		SubdivideVertex *theV=theVertexList[i];
        if(theV->m_subdivideId!=subdivideId)
		{
			//Èç¹ûÊÇµÚÒ»´Î¼ÆËãµ½Õâ¸öµã
			//³õÊ¼»¯Ï¸·Ö²½Êý
            theV->m_subdivideId=subdivideId;
            theV->m_subdivideStep=0;
			//Éú³ÉÕâ¸öµãµÄÏ¸·Öµã
            theV->m_nextLevel=target->vertex.add(new SubdivideVertex());
            //target->vertex[theV->m_nextLevel]->m_adjacentEdgeList.clear();
            target->vertex[theV->m_nextLevel]->m_adjacentEdgeList.reserve(theV->m_adjacentEdgeList.size());
			//Ê×ÏÈ¼ÆËãÏàÁÚµãµÄ×ø±êºÍEv
            target->vertex[theV->m_nextLevel]->m_position+=EAdjacentVertex(theV);
            if(theV->m_edgeVertex)
			{
                target->vertex[theV->m_nextLevel]->m_position+=theV->m_position*6;
                target->vertex[theV->m_nextLevel]->m_position/=8;
			}
			else
			{
				//¼ÓÉÏÕâ¸öÃæµÄÖÐµã
                target->vertex[theV->m_nextLevel]->m_position+=target->vertex[theFace->center]->m_position;
			}
            ++(theV->m_subdivideStep);
		}
		else
		{
            if(!theV->m_edgeVertex)
			{
                target->vertex[theV->m_nextLevel]->m_position+=target->vertex[theFace->center]->m_position;
                theV->m_subdivideStep++;
                int n=theV->m_adjacentEdgeList.size();
                if(n==theV->m_subdivideStep)
				{
                    target->vertex[theV->m_nextLevel]->m_position+=theV->m_position*(float)(n*n-2*n);
                    target->vertex[theV->m_nextLevel]->m_position/=(float)(n*n);
				}
			}
		}
	}
	delete theVertexList;
	//Ö®ºó´¦ÀíÃ¿Ò»¸ö±ß
    for(unsigned int i=0;i<edgeCount;++i)
	{
		SubdivideEdge *theEdge;
		if(theFace->edge[i]>0)
		{
			theEdge=original->edge[theFace->edge[i]];
			if(theEdge->subdivideId!=subdivideId)
			{
				//Èç¹ûÊÇµÚÒ»´Î´¦ÀíÕâ¸ö±ß
				theEdge->subdivideId=subdivideId;
                Vector theN=original->vertex[theEdge->start]->m_position+original->vertex[theEdge->end]->m_position;
				theEdge->middle=target->vertex.add(new SubdivideVertex(theN));
                //target->vertex[theEdge->middle]->m_adjacentEdgeList.clear();
                target->vertex[theEdge->middle]->m_adjacentEdgeList.reserve(4);
				if(theEdge->left && theEdge->right)
				{
                    target->vertex[theEdge->middle]->m_position+=target->vertex[theFace->center]->m_position;
				}
				else
				{
                    target->vertex[theEdge->middle]->m_position/=2;
				}
                theEdge->startEdge=target->edge.add(new SubdivideEdge(original->vertex[theEdge->start]->m_nextLevel,theEdge->middle));
                target->vertex[original->vertex[theEdge->start]->m_nextLevel]->m_adjacentEdgeList.push_back(theEdge->startEdge);
                target->vertex[theEdge->middle]->m_adjacentEdgeList.push_back(theEdge->startEdge);
                theEdge->endEdge=target->edge.add(new SubdivideEdge(theEdge->middle,original->vertex[theEdge->end]->m_nextLevel));
                target->vertex[original->vertex[theEdge->end]->m_nextLevel]->m_adjacentEdgeList.push_back(theEdge->endEdge);
                target->vertex[theEdge->middle]->m_adjacentEdgeList.push_back(theEdge->endEdge);
				theEdge->rightEdge=target->edge.add(new SubdivideEdge(theEdge->middle,theFace->center));
                target->vertex[theEdge->middle]->m_adjacentEdgeList.push_back(theEdge->rightEdge);
                target->vertex[theFace->center]->m_adjacentEdgeList.push_back(theEdge->rightEdge);
			}
			else
			{
				//Èç¹û²»ÊÇµÚÒ»´Î¼ÆËã
                target->vertex[theEdge->middle]->m_position+=target->vertex[theFace->center]->m_position;
                target->vertex[theEdge->middle]->m_position/=4;
				theEdge->rightEdge=target->edge.add(new SubdivideEdge(theEdge->middle,theFace->center));
                target->vertex[theEdge->middle]->m_adjacentEdgeList.push_back(theEdge->rightEdge);
                target->vertex[theFace->center]->m_adjacentEdgeList.push_back(theEdge->rightEdge);
			}
		}
		else
		{
			theEdge=original->edge[-theFace->edge[i]];
			if(theEdge->subdivideId!=subdivideId)
			{
				//Èç¹ûÊÇµÚÒ»´Î´¦ÀíÕâ¸ö±ß
				theEdge->subdivideId=subdivideId;
                Vector theN=original->vertex[theEdge->start]->m_position+original->vertex[theEdge->end]->m_position;
				theEdge->middle=target->vertex.add(new SubdivideVertex(theN));
                //target->vertex[theEdge->middle]->m_adjacentEdgeList.clear();
                target->vertex[theEdge->middle]->m_adjacentEdgeList.reserve(4);
				if(theEdge->left && theEdge->right)
				{
                    target->vertex[theEdge->middle]->m_position+=target->vertex[theFace->center]->m_position;
				}
				else
				{
                    target->vertex[theEdge->middle]->m_position/=2;
				}
                theEdge->startEdge=target->edge.add(new SubdivideEdge(original->vertex[theEdge->start]->m_nextLevel,theEdge->middle));
                target->vertex[original->vertex[theEdge->start]->m_nextLevel]->m_adjacentEdgeList.push_back(theEdge->startEdge);
                target->vertex[theEdge->middle]->m_adjacentEdgeList.push_back(theEdge->startEdge);
                theEdge->endEdge=target->edge.add(new SubdivideEdge(theEdge->middle,original->vertex[theEdge->end]->m_nextLevel));
                target->vertex[original->vertex[theEdge->end]->m_nextLevel]->m_adjacentEdgeList.push_back(theEdge->endEdge);
                target->vertex[theEdge->middle]->m_adjacentEdgeList.push_back(theEdge->endEdge);
				theEdge->leftEdge=target->edge.add(new SubdivideEdge(theEdge->middle,theFace->center));
                target->vertex[theEdge->middle]->m_adjacentEdgeList.push_back(theEdge->leftEdge);
                target->vertex[theFace->center]->m_adjacentEdgeList.push_back(theEdge->leftEdge);
			}
			else
			{
				//Èç¹û²»ÊÇµÚÒ»´Î¼ÆËã
                target->vertex[theEdge->middle]->m_position+=target->vertex[theFace->center]->m_position;
                target->vertex[theEdge->middle]->m_position/=4;
				theEdge->leftEdge=target->edge.add(new SubdivideEdge(theEdge->middle,theFace->center));
                target->vertex[theEdge->middle]->m_adjacentEdgeList.push_back(theEdge->leftEdge);
                target->vertex[theFace->center]->m_adjacentEdgeList.push_back(theEdge->leftEdge);
			}
		}
	}
	//½¨Á¢Ï¸·ÖÖ®ºóµÄÃæ
    for(unsigned int i=0;i<edgeCount;++i)
	{
        unsigned int i2=(i+1)%edgeCount;
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
    unsigned int cacheSize=PSFaceCache.size();
	for(int i=cacheSize-1;i>-1;--i)
	{
		PSFaceCache[i]->isSub=false;
		PSFaceCache.pop_back();
	}
	PSFaceCache.clear();
    for(unsigned int e=0;e<5;e++)
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
    }

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
    }

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
        }

void Object::normalizeVertexNormal()
	{
        for(unsigned int i=1;i<subdivideLevel[0]->vertex.size();++i)
		{
            subdivideLevel[0]->vertex[i]->m_normal.normalize();
		}
	}

void Object::updateFNormal(Face *theFace)
	{
        unsigned int edgeCount=theFace->edge.size();
		Vector *theVector=new Vector[edgeCount];
        for(unsigned int i=0;i<edgeCount;++i)
		{
			theVector[i].null();
			if(theFace->edge[i]>0)
			{
                theVector[i]=vertexArray[edgeArray[theFace->edge[i]]->end]->m_position-vertexArray[edgeArray[theFace->edge[i]]->start]->m_position;
			}
			else
			{
                theVector[i]=vertexArray[edgeArray[-theFace->edge[i]]->start]->m_position-vertexArray[edgeArray[-theFace->edge[i]]->end]->m_position;
			}
		}
		--edgeCount;
        for(unsigned int i=0;i<edgeCount;++i)
		{
			theFace->normal+=perpendicular(theVector[i],theVector[i+1]);
		}
		theFace->normal+=perpendicular(theVector[edgeCount],theVector[0]);
		theFace->normal.normalize();
		delete theVector;
	}

void Object::updateFNormal(SubdivideFace *theFace,unsigned int level)
	{
		Vector theVector[4];
        for(unsigned int i=0;i<4;++i)
		{
			theVector[i].null();
			if(theFace->edge[i]>0)
			{
                theVector[i]=subdivideLevel[level]->vertex[subdivideLevel[level]->edge[theFace->edge[i]]->end]->m_position-subdivideLevel[level]->vertex[subdivideLevel[level]->edge[theFace->edge[i]]->start]->m_position;
			}
			else
			{
                theVector[i]=subdivideLevel[level]->vertex[subdivideLevel[level]->edge[-theFace->edge[i]]->start]->m_position-subdivideLevel[level]->vertex[subdivideLevel[level]->edge[-theFace->edge[i]]->end]->m_position;
			}
		}
        for(unsigned int i=0;i<3;++i)
		{
			theFace->normal+=perpendicular(theVector[i],theVector[i+1]);
		}
		theFace->normal+=perpendicular(theVector[3],theVector[0]);
		theFace->normal.normalize();
	}

void Object::updateAllSubNormal()
	{
        unsigned int faceCount=subdivideLevel[0]->face.size();
        for(unsigned int i=1;i<faceCount;++i)
		{
			updateFNormal(subdivideLevel[0]->face[i],0);
		}
        unsigned int vertexCount=subdivideLevel[0]->vertex.size();
        for(unsigned int i=1;i<vertexCount;++i)
		{
			updateVNormal(subdivideLevel[0]->vertex[i],0);
		}
	}

//µÃµ½±ßµÄÊýÄ¿
unsigned int Object::edgeCount()
	{
		return edgeArray.size();
    }

unsigned int Object::vertexCount()
	{
		return vertexArray.size();
    }

unsigned int Object::faceCount()
	{
		return faceArray.size();	
    }

void Object::vertexPositionChangeR(unsigned int vertexID,float nx,float ny,float nz)
	{
		Vertex *theVertex=vertexArray[vertexID];
        historyManager->record(new Log_VertexPositionChange(index,vertexID,theVertex->m_position.x,theVertex->m_position.y,theVertex->m_position.z));
        theVertex->m_position.x+=nx;
        theVertex->m_position.y+=ny;
        theVertex->m_position.z+=nz;
    }

void Object::vertexNormalChange(unsigned int vertexID,float nx,float ny,float nz)
	{
		Vertex *theVertex=vertexArray[vertexID];
        historyManager->record(new Log_VertexNormalChange(index,vertexID,theVertex->m_normal.x,theVertex->m_normal.y,theVertex->m_normal.z));
        theVertex->m_normal.x=nx;
        theVertex->m_normal.y=ny;
        theVertex->m_normal.z=nz;
    }

void Object::objectEdgeRemove(unsigned int edgeID)
	{
		if(!historyManager->record(new Log_ObjectEdgeRemove(index,edgeID,edgeArray[edgeID])))
		{
			delete edgeArray[edgeID];
		}
		edgeArray.remove(edgeID);
    }

void Object::objectVertexRemove(unsigned int vertexID)
	{
		if(!historyManager->record(new Log_ObjectVertexRemove(index,vertexID,vertexArray[vertexID])))
		{
			delete vertexArray[vertexID];
		}
		vertexArray.remove(vertexID);
    }

void Object::edgeRightChange(unsigned int edgeID,unsigned int nr)
	{
		historyManager->record(new Log_EdgeRightChange(index,edgeID,edgeArray[edgeID]->right));
		edgeArray[edgeID]->right=nr;
    }

void Object::edgeStartChange(unsigned int edgeID,unsigned int ns)
	{
		historyManager->record(new Log_EdgeStartChange(index,edgeID,edgeArray[edgeID]->start));
		edgeArray[edgeID]->start=ns;
    }

void Object::vertexAdjacentRemove(unsigned int vertexID,unsigned int adjID)
	{
        unsigned int adjEdgeCount=vertexArray[vertexID]->m_adjacentEdgeList.size()-1;
        historyManager->record(new Log_VertexAdjacentRemove(index,vertexID,adjID,vertexArray[vertexID]->m_adjacentEdgeList[adjID]));
        unsigned int temp=vertexArray[vertexID]->m_adjacentEdgeList[adjEdgeCount];
        vertexArray[vertexID]->m_adjacentEdgeList[adjEdgeCount]=vertexArray[vertexID]->m_adjacentEdgeList[adjID];
        vertexArray[vertexID]->m_adjacentEdgeList[adjID]=temp;
        vertexArray[vertexID]->m_adjacentEdgeList.pop_back();
    }

void Object::vertexAdjacentInsert(unsigned int vertexID,unsigned int adjID,unsigned int ne)
	{
		historyManager->record(new Log_VertexAdjacentInsert(index,vertexID,adjID));
        vertexArray[vertexID]->m_adjacentEdgeList.push_back(ne);
        unsigned int temp=vertexArray[vertexID]->m_adjacentEdgeList[adjID];
        unsigned int adjEdgeCount=vertexArray[vertexID]->m_adjacentEdgeList.size()-1;
        vertexArray[vertexID]->m_adjacentEdgeList[adjID]=vertexArray[vertexID]->m_adjacentEdgeList[adjEdgeCount];
        vertexArray[vertexID]->m_adjacentEdgeList[adjEdgeCount]=temp;
    }

void Object::faceEdgePush(unsigned int faceID,int nEdge)
	{
		historyManager->record(new Log_FaceEdgePush(index,faceID));
		faceArray[faceID]->edge.push_back(nEdge);
    }

void Object::faceEdgeChange(unsigned int faceID,unsigned int edgeID,int nEdge)
	{
		historyManager->record(new Log_FaceEdgeChange(index,faceID,edgeID,faceArray[faceID]->edge[edgeID]));
		faceArray[faceID]->edge[edgeID]=nEdge;
    }

void Object::faceEdgeInsert(unsigned int faceID,unsigned int edgeID,int nEdge)
	{
		historyManager->record(new Log_FaceEdgeInsert(index,faceID,edgeID));
		Face *theFace=faceArray[faceID];
		theFace->edge.push_back(0);
        for(unsigned int h=theFace->edge.size()-1;h>edgeID;--h)
		{
			theFace->edge[h]=theFace->edge[h-1];
		}
		theFace->edge[edgeID]=nEdge;
    }

void Object::vertexAdjacentPush(unsigned int vertexID,unsigned int ne)
	{
		historyManager->record(new Log_VertexAdjacentPush(index,vertexID));
        vertexArray[vertexID]->m_adjacentEdgeList.push_back(ne);
	}

void Object::vertexAdjacentChange(unsigned int vertexID,unsigned int edgeID,unsigned int ne)
	{
        historyManager->record(new Log_VertexAdjacentChange(index,vertexID,edgeID,vertexArray[vertexID]->m_adjacentEdgeList[edgeID]));
        vertexArray[vertexID]->m_adjacentEdgeList[edgeID]=ne;
	}

void Object::vertexAdjacentPop(unsigned int vertexID)
	{
        historyManager->record(new Log_VertexAdjacentPop(index,vertexID,vertexArray[vertexID]->m_adjacentEdgeList[vertexArray[vertexID]->m_adjacentEdgeList.size()-1]));
        vertexArray[vertexID]->m_adjacentEdgeList.pop_back();
	}
void Object::faceEdgeSwap(unsigned int faceID,unsigned int i1,unsigned int i2)
	{
		historyManager->record(new Log_FaceEdgeSwap(index,faceID,i1,i2));
		int temp=faceArray[faceID]->edge[i1];
		faceArray[faceID]->edge[i1]=faceArray[faceID]->edge[i2];
		faceArray[faceID]->edge[i2]=temp;
	}
void Object::faceEdgeRemove(unsigned int faceID,unsigned int edgeID)
	{
		Face *theFace=faceArray[faceID];
		historyManager->record(new Log_FaceEdgeRemove(index,faceID,edgeID,theFace->edge[edgeID]));
        unsigned int edgeCount=theFace->edge.size();
        for(unsigned int e=edgeID+1;e<edgeCount;++e)
		{
			theFace->edge[e-1]=theFace->edge[e];
		}
		theFace->edge.pop_back();
	}

void Object::redefineControlPoint()
	{
		vertexArray.~IndexArray();
		vertexArray.pushNull();
        unsigned int vertexCount=subdivideLevel[0]->vertex.size();
        //vertexArray.clear();
		vertexArray.reserve(vertexCount);
        for(unsigned int i=1;i<vertexCount;i++)
		{
            addVertex(subdivideLevel[0]->vertex[i]->m_position,subdivideLevel[0]->vertex[i]->m_normal);
		}
		edgeArray.~IndexArray();
		edgeArray.pushNull();
        unsigned int edgeCount=subdivideLevel[0]->edge.size();
        //edgeArray.clear();
		edgeArray.reserve(edgeCount);
        for(unsigned int i=1;i<edgeCount;i++)
		{
		//	printf("-%d-",subdivideLevel[0]->edge[i]->start);
		//	printf("-%d-",subdivideLevel[0]->edge[i]->end);
			addEdge(subdivideLevel[0]->edge[i]->start,subdivideLevel[0]->edge[i]->end);
		}
		faceArray.~IndexArray();
		faceArray.pushNull();
        unsigned int faceCount=subdivideLevel[0]->face.size();
        //faceArray.clear();
		faceArray.reserve(faceCount);
        for(unsigned int i=1;i<faceCount;i++)
		{
			//printf("-%d--",i);
			SubdivideFace *theSF=subdivideLevel[0]->face[i];
            unsigned int tempEdge[4]={0};
			tempEdge[0]=theSF->edge[0]>0?theSF->edge[0]:-(theSF->edge[0]);
			tempEdge[1]=theSF->edge[1]>0?theSF->edge[1]:-(theSF->edge[1]);
			tempEdge[2]=theSF->edge[2]>0?theSF->edge[2]:-(theSF->edge[2]);
			tempEdge[3]=theSF->edge[3]>0?theSF->edge[3]:-(theSF->edge[3]);
			addFace(tempEdge,4);
		}

        for(unsigned int i=0;i<(unsigned int)subdivideLevelSize;i++)
		{
			delete subdivideLevel[i];
			subdivideLevel[i]=NULL;
		}
		subdivideLevelSize=0;
    }


void Object::testXMLOut(char *fileName)
	{
		FILE *fp=fopen(fileName,"w");
		fprintf(fp,"<O>\n");
        for(unsigned int i=0;i<vertexArray.size();++i)
		{
			if(!vertexArray[i]) continue;
			fprintf(fp,"<V i=\"%d\">\n",vertexArray[i]->index);
            for(unsigned int e=0;e<vertexArray[i]->m_adjacentEdgeList.size();e++)
			{
                fprintf(fp,"<AE a=\"%d\" />\n",vertexArray[i]->m_adjacentEdgeList[e]);
			}
			fprintf(fp,"</V>\n");
		}
        for(unsigned int i=0;i<edgeArray.size();i++)
		{
			if(!edgeArray[i]) continue;
			fprintf(fp,"<E i=\"%d\" s=\"%d\" e=\"%d\" l=\"%d\" r=\"%d\" />\n",edgeArray[i]->index,edgeArray[i]->start,edgeArray[i]->end,edgeArray[i]->left,edgeArray[i]->right);
		}
        for(unsigned int i=0;i<faceArray.size();i++)
		{
			if(!faceArray[i]) continue;
			fprintf(fp,"<F i=\"%d\">\n",faceArray[i]->index);
            for(unsigned int e=0;e<faceArray[i]->edge.size();e++)
				fprintf(fp,"<FE e=\"%d\" />\n",faceArray[i]->edge[e]);
			fprintf(fp,"</F>\n");
		}
		fprintf(fp,"</O>");
		fclose(fp);
	}


void Object::vertexAdjacentSwap(unsigned int vertexID,unsigned int i1,unsigned int i2)
	{
		historyManager->record(new Log_VertexAdjacentSwap(index,vertexID,i1,i2));
        unsigned int temp=vertexArray[vertexID]->m_adjacentEdgeList[i1];
        vertexArray[vertexID]->m_adjacentEdgeList[i1]=vertexArray[vertexID]->m_adjacentEdgeList[i2];
        vertexArray[vertexID]->m_adjacentEdgeList[i2]=temp;
	}

void Object::faceEdgePop(unsigned int faceID)
	{
		historyManager->record(new Log_FaceEdgePop(index,faceID,faceArray[faceID]->edge[faceArray[faceID]->edge.size()-1]));
		faceArray[faceID]->edge.pop_back();
    }

void Object::edgeEndChange(unsigned int edgeID,unsigned int ne)
	{
		historyManager->record(new Log_EdgeEndChange(index,edgeID,edgeArray[edgeID]->end));
		edgeArray[edgeID]->end=ne;
    }

void Object::edgeLeftChange(unsigned int edgeID,unsigned int nl)
	{
		historyManager->record(new Log_EdgeLeftChange(index,edgeID,edgeArray[edgeID]->left));
		edgeArray[edgeID]->left=nl;
    }

void Object::objectFaceRemove(unsigned int faceID)
	{
		if(!historyManager->record(new Log_ObjectFaceRemove(index,faceID,faceArray[faceID])))
		{
			delete faceArray[faceID];			
		}
		faceArray.remove(faceID);
    }

    void Object::vertexPositionChangeA(unsigned int vertexID,float nx,float ny,float nz)
	{
		Vertex *theVertex=vertexArray[vertexID];
        historyManager->record(new Log_VertexPositionChange(index,vertexID,theVertex->m_position.x,theVertex->m_position.y,theVertex->m_position.z));
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
            if(edgeArray[theVertex->m_adjacentEdgeList[i]]->start==theVertex->index)
			{
                Face *theFace=faceArray[edgeArray[theVertex->m_adjacentEdgeList[i]]->left];
				if(theFace)
                theVertex->m_normal+=theFace->normal;
			}
			else
			{
                Face *theFace=faceArray[edgeArray[theVertex->m_adjacentEdgeList[i]]->right];
				if(theFace)
                theVertex->m_normal+=theFace->normal;
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
            if(subdivideLevel[level]->edge[theVertex->m_adjacentEdgeList[i]]->start==theVertex->index)
			{
                SubdivideFace *theFace=subdivideLevel[level]->face[subdivideLevel[level]->edge[theVertex->m_adjacentEdgeList[i]]->left];
				if(theFace)
                theVertex->m_normal+=theFace->normal;
			}
			else
			{
                SubdivideFace *theFace=subdivideLevel[level]->face[subdivideLevel[level]->edge[theVertex->m_adjacentEdgeList[i]]->right];
				if(theFace)
                theVertex->m_normal+=theFace->normal;
			}
		}
        theVertex->m_normal.normalize();
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
    }


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
    }

void Object::drawWireVertexSelected()
{
	glLineWidth(1);
	glDisable(GL_LIGHTING);
	glColor3ub(114,150,250);
	glLineWidth(1);
	if(subdivideLevelSize==0)
	{
		glBegin(GL_LINES);
        for(unsigned int i=1;i<edgeArray.size();++i)
		{
			Edge *e=edgeArray[i];
			if(e)
			{
				Vertex *start=vertexArray[e->start];
				Vertex *end=vertexArray[e->end];
                glVertex3f(start->m_position.x,start->m_position.y,start->m_position.z);
                glVertex3f(end->m_position.x,end->m_position.y,end->m_position.z);
			}
		}
		glEnd();
	}
	else
	{
		glBegin(GL_LINES);
        for(unsigned int i=1;i<edgeArray.size();++i)
		{
			Edge *e=edgeArray[i];
			if(e)
			{
				Vertex *start=vertexArray[e->start];
				Vertex *end=vertexArray[e->end];
                glVertex3f(start->m_position.x,start->m_position.y,start->m_position.z);
                glVertex3f(end->m_position.x,end->m_position.y,end->m_position.z);
			}
		}
		glColor3ub(0,0,0);
        for(unsigned int i=1;i<subdivideLevel[0]->edge.size();++i)
		{
			SubdivideEdge *e=subdivideLevel[0]->edge[i];
			SubdivideVertex *start=subdivideLevel[0]->vertex[e->start];
			SubdivideVertex *end=subdivideLevel[0]->vertex[e->end];
            glVertex3f(start->m_position.x,start->m_position.y,start->m_position.z);
            glVertex3f(end->m_position.x,end->m_position.y,end->m_position.z);
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
            for(unsigned int i=1;i<edgeArray.size();++i)
			{
				Edge *e=edgeArray[i];
				if(e && !(e->isSelected))
				{
					Vertex *start=vertexArray[e->start];
					Vertex *end=vertexArray[e->end];
                    glVertex3f(start->m_position.x,start->m_position.y,start->m_position.z);
                    glVertex3f(end->m_position.x,end->m_position.y,end->m_position.z);
				}
			}
			glEnd();
		}
		else
		{
			glBegin(GL_LINES);
            for(unsigned int i=1;i<edgeArray.size();++i)
			{
				Edge *e=edgeArray[i];
				if(e)
				{
					Vertex *start=vertexArray[e->start];
					Vertex *end=vertexArray[e->end];
                    glVertex3f(start->m_position.x,start->m_position.y,start->m_position.z);
                    glVertex3f(end->m_position.x,end->m_position.y,end->m_position.z);
				}
			}
			glColor3ub(0,0,0);
            for(unsigned int i=1;i<subdivideLevel[0]->edge.size();++i)
			{
				SubdivideEdge *e=subdivideLevel[0]->edge[i];
				SubdivideVertex *start=subdivideLevel[0]->vertex[e->start];
				SubdivideVertex *end=subdivideLevel[0]->vertex[e->end];
                glVertex3f(start->m_position.x,start->m_position.y,start->m_position.z);
                glVertex3f(end->m_position.x,end->m_position.y,end->m_position.z);
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
            for(unsigned int i=1;i<edgeArray.size();++i)
			{
				Edge *e=edgeArray[i];
				if(e)
				{
					Vertex *start=vertexArray[e->start];
					Vertex *end=vertexArray[e->end];
                    glVertex3f(start->m_position.x,start->m_position.y,start->m_position.z);
                    glVertex3f(end->m_position.x,end->m_position.y,end->m_position.z);
				}
			}
			glEnd();
		}
		else
		{
			glBegin(GL_LINES);
            for(unsigned int i=1;i<edgeArray.size();++i)
			{
				Edge *e=edgeArray[i];
				if(e)
				{
					Vertex *start=vertexArray[e->start];
					Vertex *end=vertexArray[e->end];
                    glVertex3f(start->m_position.x,start->m_position.y,start->m_position.z);
                    glVertex3f(end->m_position.x,end->m_position.y,end->m_position.z);
				}
			}
			glColor3ub(0,0,0);
            for(unsigned int i=1;i<subdivideLevel[0]->edge.size();++i)
			{
				SubdivideEdge *e=subdivideLevel[0]->edge[i];
				SubdivideVertex *start=subdivideLevel[0]->vertex[e->start];
				SubdivideVertex *end=subdivideLevel[0]->vertex[e->end];
                glVertex3f(start->m_position.x,start->m_position.y,start->m_position.z);
                glVertex3f(end->m_position.x,end->m_position.y,end->m_position.z);
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
            for(unsigned int i=1;i<edgeArray.size();++i)
			{
				Edge *e=edgeArray[i];
				if(e)
				{
					Vertex *start=vertexArray[e->start];
					Vertex *end=vertexArray[e->end];
                    glVertex3f(start->m_position.x,start->m_position.y,start->m_position.z);
                    glVertex3f(end->m_position.x,end->m_position.y,end->m_position.z);
				}
			}
			glEnd();
		}
		else
		{
			glBegin(GL_LINES);
            for(unsigned int i=1;i<edgeArray.size();++i)
			{
				Edge *e=edgeArray[i];
				if(e)
				{
					Vertex *start=vertexArray[e->start];
					Vertex *end=vertexArray[e->end];
                    glVertex3f(start->m_position.x,start->m_position.y,start->m_position.z);
                    glVertex3f(end->m_position.x,end->m_position.y,end->m_position.z);
				}
			}
			glColor3ub(0,0,0);
            for(unsigned int i=1;i<subdivideLevel[0]->edge.size();++i)
			{
				SubdivideEdge *e=subdivideLevel[0]->edge[i];
				SubdivideVertex *start=subdivideLevel[0]->vertex[e->start];
				SubdivideVertex *end=subdivideLevel[0]->vertex[e->end];
                glVertex3f(start->m_position.x,start->m_position.y,start->m_position.z);
                glVertex3f(end->m_position.x,end->m_position.y,end->m_position.z);
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
            for(unsigned int i=1;i<faceArray.size();++i)
			{
				if(faceArray[i])
				{
					Face *theFace=faceArray[i];
					glBegin(GL_POLYGON);
                    for(unsigned int e=0;e<theFace->edge.size();++e)
					{
						if(theFace->edge[e]>0)
						{
							Vertex *v=vertexArray[edgeArray[theFace->edge[e]]->start];
                            glNormal3f(v->m_normal.x,v->m_normal.y,v->m_normal.z);
                            glVertex3f(v->m_position.x,v->m_position.y,v->m_position.z);
						}
						else
						{
							Vertex *v=vertexArray[edgeArray[-theFace->edge[e]]->end];
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
            for(unsigned int i=1;i<edgeArray.size();++i)
			{
				Edge *e=edgeArray[i];
				if(e)
				{
					Vertex *start=vertexArray[e->start];
					Vertex *end=vertexArray[e->end];
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
            for(unsigned int i=1;i<subdivideLevel[0]->face.size();++i)
			{
				//glBegin(GL_POLYGON);
				SubdivideFace *theFace=subdivideLevel[0]->face[i];
			//	glNormal3f(theFace->normal.x,theFace->normal.y,theFace->normal.z);
				if(theFace->edge[0]>0)
				{
					SubdivideVertex *v=subdivideLevel[0]->vertex[subdivideLevel[0]->edge[theFace->edge[0]]->start];
                    glNormal3f(v->m_normal.x,v->m_normal.y,v->m_normal.z);
                    glVertex3f(v->m_position.x,v->m_position.y,v->m_position.z);
				}
				else
				{
					SubdivideVertex *v=subdivideLevel[0]->vertex[subdivideLevel[0]->edge[-theFace->edge[0]]->end];
                    glNormal3f(v->m_normal.x,v->m_normal.y,v->m_normal.z);
                    glVertex3f(v->m_position.x,v->m_position.y,v->m_position.z);
				}
				if(theFace->edge[1]>0)
				{
					SubdivideVertex *v=subdivideLevel[0]->vertex[subdivideLevel[0]->edge[theFace->edge[1]]->start];
                    glNormal3f(v->m_normal.x,v->m_normal.y,v->m_normal.z);
                    glVertex3f(v->m_position.x,v->m_position.y,v->m_position.z);
				}
				else
				{
					SubdivideVertex *v=subdivideLevel[0]->vertex[subdivideLevel[0]->edge[-theFace->edge[1]]->end];
                    glNormal3f(v->m_normal.x,v->m_normal.y,v->m_normal.z);
                    glVertex3f(v->m_position.x,v->m_position.y,v->m_position.z);
				}				
				if(theFace->edge[2]>0)
				{
					SubdivideVertex *v=subdivideLevel[0]->vertex[subdivideLevel[0]->edge[theFace->edge[2]]->start];
                    glNormal3f(v->m_normal.x,v->m_normal.y,v->m_normal.z);
                    glVertex3f(v->m_position.x,v->m_position.y,v->m_position.z);
				}
				else
				{
					SubdivideVertex *v=subdivideLevel[0]->vertex[subdivideLevel[0]->edge[-theFace->edge[2]]->end];
                    glNormal3f(v->m_normal.x,v->m_normal.y,v->m_normal.z);
                    glVertex3f(v->m_position.x,v->m_position.y,v->m_position.z);
				}
				if(theFace->edge[3]>0)
				{
					SubdivideVertex *v=subdivideLevel[0]->vertex[subdivideLevel[0]->edge[theFace->edge[3]]->start];
                    glNormal3f(v->m_normal.x,v->m_normal.y,v->m_normal.z);
                    glVertex3f(v->m_position.x,v->m_position.y,v->m_position.z);
				}
				else
				{
					SubdivideVertex *v=subdivideLevel[0]->vertex[subdivideLevel[0]->edge[-theFace->edge[3]]->end];
                    glNormal3f(v->m_normal.x,v->m_normal.y,v->m_normal.z);
                    glVertex3f(v->m_position.x,v->m_position.y,v->m_position.z);
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
            for(unsigned int i=1;i<subdivideLevel[0]->edge.size();++i)
			{
				SubdivideEdge *e=subdivideLevel[0]->edge[i];
				SubdivideVertex *start=subdivideLevel[0]->vertex[e->start];
				SubdivideVertex *end=subdivideLevel[0]->vertex[e->end];
                glVertex3f(start->m_position.x,start->m_position.y,start->m_position.z);
                glVertex3f(end->m_position.x,end->m_position.y,end->m_position.z);
			}
			glColor3ub(114,186,221);
            for(unsigned int i=1;i<edgeArray.size();++i)
			{
				Edge *e=edgeArray[i];
				if(e)
				{
					Vertex *start=vertexArray[e->start];
					Vertex *end=vertexArray[e->end];
                    glVertex3f(start->m_position.x,start->m_position.y,start->m_position.z);
                    glVertex3f(end->m_position.x,end->m_position.y,end->m_position.z);
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
            for(unsigned int i=1;i<faceArray.size();++i)
			{
				if(faceArray[i])
				{
					Face *theFace=faceArray[i];
					glBegin(GL_POLYGON);
					glNormal3f(theFace->normal.x,theFace->normal.y,theFace->normal.z);
                    for(unsigned int e=0;e<theFace->edge.size();++e)
					{
						if(theFace->edge[e]>0)
						{
							Vertex *v=vertexArray[edgeArray[theFace->edge[e]]->start];
                            glVertex3f(v->m_position.x,v->m_position.y,v->m_position.z);
						}
						else
						{
							Vertex *v=vertexArray[edgeArray[-theFace->edge[e]]->end];
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
            for(unsigned int i=1;i<edgeArray.size();++i)
			{
				Edge *e=edgeArray[i];
				if(e)
				{
					Vertex *start=vertexArray[e->start];
					Vertex *end=vertexArray[e->end];
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
            for(unsigned int i=1;i<subdivideLevel[0]->face.size();++i)
			{
				SubdivideFace *theFace=subdivideLevel[0]->face[i];
				glNormal3f(theFace->normal.x,theFace->normal.y,theFace->normal.z);
				if(theFace->edge[0]>0)
				{
					SubdivideVertex *v=subdivideLevel[0]->vertex[subdivideLevel[0]->edge[theFace->edge[0]]->start];
                    glVertex3f(v->m_position.x,v->m_position.y,v->m_position.z);
				}
				else
				{
					SubdivideVertex *v=subdivideLevel[0]->vertex[subdivideLevel[0]->edge[-theFace->edge[0]]->end];
                    glVertex3f(v->m_position.x,v->m_position.y,v->m_position.z);
				}
				if(theFace->edge[1]>0)
				{
					SubdivideVertex *v=subdivideLevel[0]->vertex[subdivideLevel[0]->edge[theFace->edge[1]]->start];
                    glVertex3f(v->m_position.x,v->m_position.y,v->m_position.z);
				}
				else
				{
					SubdivideVertex *v=subdivideLevel[0]->vertex[subdivideLevel[0]->edge[-theFace->edge[1]]->end];
                    glVertex3f(v->m_position.x,v->m_position.y,v->m_position.z);
				}				
				if(theFace->edge[2]>0)
				{
					SubdivideVertex *v=subdivideLevel[0]->vertex[subdivideLevel[0]->edge[theFace->edge[2]]->start];
                    glVertex3f(v->m_position.x,v->m_position.y,v->m_position.z);
				}
				else
				{
					SubdivideVertex *v=subdivideLevel[0]->vertex[subdivideLevel[0]->edge[-theFace->edge[2]]->end];
                    glVertex3f(v->m_position.x,v->m_position.y,v->m_position.z);
				}
				if(theFace->edge[3]>0)
				{
					SubdivideVertex *v=subdivideLevel[0]->vertex[subdivideLevel[0]->edge[theFace->edge[3]]->start];
                    glVertex3f(v->m_position.x,v->m_position.y,v->m_position.z);
				}
				else
				{
					SubdivideVertex *v=subdivideLevel[0]->vertex[subdivideLevel[0]->edge[-theFace->edge[3]]->end];
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
            for(unsigned int i=1;i<subdivideLevel[0]->edge.size();++i)
			{
				SubdivideEdge *e=subdivideLevel[0]->edge[i];
				SubdivideVertex *start=subdivideLevel[0]->vertex[e->start];
				SubdivideVertex *end=subdivideLevel[0]->vertex[e->end];
                glVertex3f(start->m_position.x,start->m_position.y,start->m_position.z);
                glVertex3f(end->m_position.x,end->m_position.y,end->m_position.z);
			}

						glColor3ub(114,186,221);
            for(unsigned int i=1;i<edgeArray.size();++i)
			{
				Edge *e=edgeArray[i];
				if(e)
				{
					Vertex *start=vertexArray[e->start];
					Vertex *end=vertexArray[e->end];
                    glVertex3f(start->m_position.x,start->m_position.y,start->m_position.z);
                    glVertex3f(end->m_position.x,end->m_position.y,end->m_position.z);
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
            for(unsigned int i=1;i<faceArray.size();++i)
			{
				if(faceArray[i])
				{
					Face *theFace=faceArray[i];
					glBegin(GL_POLYGON);
					glNormal3f(theFace->normal.x,theFace->normal.y,theFace->normal.z);
                    for(unsigned int e=0;e<theFace->edge.size();++e)
					{
						if(theFace->edge[e]>0)
						{
							Vertex *v=vertexArray[edgeArray[theFace->edge[e]]->start];
                            glVertex3f(v->m_position.x,v->m_position.y,v->m_position.z);
						}
						else
						{
							Vertex *v=vertexArray[edgeArray[-theFace->edge[e]]->end];
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
            for(unsigned int i=1;i<edgeArray.size();++i)
			{
				Edge *e=edgeArray[i];
				if(e)
				{
					Vertex *start=vertexArray[e->start];
					Vertex *end=vertexArray[e->end];
                    glVertex3f(start->m_position.x,start->m_position.y,start->m_position.z);
                    glVertex3f(end->m_position.x,end->m_position.y,end->m_position.z);
				}
			}
			glEnd();
			glColor3ub(0,200,0);
			glPointSize(5.0f);
			glBegin(GL_POINTS);
			
            for(unsigned int i=1;i<vertexArray.size();++i)
			{
				Vertex *v=vertexArray[i];
				if(v && !v->isSelected)
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
            for(unsigned int i=1;i<subdivideLevel[0]->face.size();++i)
			{
				SubdivideFace *theFace=subdivideLevel[0]->face[i];
				glNormal3f(theFace->normal.x,theFace->normal.y,theFace->normal.z);
				if(theFace->edge[0]>0)
				{
					SubdivideVertex *v=subdivideLevel[0]->vertex[subdivideLevel[0]->edge[theFace->edge[0]]->start];
                    glVertex3f(v->m_position.x,v->m_position.y,v->m_position.z);
				}
				else
				{
					SubdivideVertex *v=subdivideLevel[0]->vertex[subdivideLevel[0]->edge[-theFace->edge[0]]->end];
                    glVertex3f(v->m_position.x,v->m_position.y,v->m_position.z);
				}
				if(theFace->edge[1]>0)
				{
					SubdivideVertex *v=subdivideLevel[0]->vertex[subdivideLevel[0]->edge[theFace->edge[1]]->start];
                    glVertex3f(v->m_position.x,v->m_position.y,v->m_position.z);
				}
				else
				{
					SubdivideVertex *v=subdivideLevel[0]->vertex[subdivideLevel[0]->edge[-theFace->edge[1]]->end];
                    glVertex3f(v->m_position.x,v->m_position.y,v->m_position.z);
				}				
				if(theFace->edge[2]>0)
				{
					SubdivideVertex *v=subdivideLevel[0]->vertex[subdivideLevel[0]->edge[theFace->edge[2]]->start];
                    glVertex3f(v->m_position.x,v->m_position.y,v->m_position.z);
				}
				else
				{
					SubdivideVertex *v=subdivideLevel[0]->vertex[subdivideLevel[0]->edge[-theFace->edge[2]]->end];
                    glVertex3f(v->m_position.x,v->m_position.y,v->m_position.z);
				}
				if(theFace->edge[3]>0)
				{
					SubdivideVertex *v=subdivideLevel[0]->vertex[subdivideLevel[0]->edge[theFace->edge[3]]->start];
                    glVertex3f(v->m_position.x,v->m_position.y,v->m_position.z);
				}
				else
				{
					SubdivideVertex *v=subdivideLevel[0]->vertex[subdivideLevel[0]->edge[-theFace->edge[3]]->end];
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
            for(unsigned int i=1;i<subdivideLevel[0]->edge.size();++i)
			{
				SubdivideEdge *e=subdivideLevel[0]->edge[i];
				SubdivideVertex *start=subdivideLevel[0]->vertex[e->start];
				SubdivideVertex *end=subdivideLevel[0]->vertex[e->end];
                glVertex3f(start->m_position.x,start->m_position.y,start->m_position.z);
                glVertex3f(end->m_position.x,end->m_position.y,end->m_position.z);
			}

						glColor3ub(114,186,221);
            for(unsigned int i=1;i<edgeArray.size();++i)
			{
				Edge *e=edgeArray[i];
				if(e)
				{
					Vertex *start=vertexArray[e->start];
					Vertex *end=vertexArray[e->end];
                    glVertex3f(start->m_position.x,start->m_position.y,start->m_position.z);
                    glVertex3f(end->m_position.x,end->m_position.y,end->m_position.z);
				}
			}

			glEnd();
			glColor3ub(0,200,0);
			glPointSize(5.0f);
			glBegin(GL_POINTS);
            for(unsigned int i=1;i<vertexArray.size();++i)
			{
				Vertex *v=vertexArray[i];
				if(v && !v->isSelected)
				{
                    glVertex3f(v->m_position.x,v->m_position.y,v->m_position.z);
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
            for(unsigned int i=1;i<faceArray.size();++i)
			{
				if(faceArray[i] && !faceArray[i]->isSelected)
				{
					Face *theFace=faceArray[i];
					glBegin(GL_POLYGON);
					glNormal3f(theFace->normal.x,theFace->normal.y,theFace->normal.z);
                    for(unsigned int e=0;e<theFace->edge.size();++e)
					{
						if(theFace->edge[e]>0)
						{
							Vertex *v=vertexArray[edgeArray[theFace->edge[e]]->start];
                            glVertex3f(v->m_position.x,v->m_position.y,v->m_position.z);
						}
						else
						{
							Vertex *v=vertexArray[edgeArray[-theFace->edge[e]]->end];
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
            for(unsigned int i=1;i<edgeArray.size();++i)
			{
				Edge *e=edgeArray[i];
				if(e)
				{
					Vertex *start=vertexArray[e->start];
					Vertex *end=vertexArray[e->end];
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
            for(unsigned int i=1;i<subdivideLevel[0]->face.size();++i)
			{
				SubdivideFace *theFace=subdivideLevel[0]->face[i];
				glNormal3f(theFace->normal.x,theFace->normal.y,theFace->normal.z);
				if(theFace->edge[0]>0)
				{
					SubdivideVertex *v=subdivideLevel[0]->vertex[subdivideLevel[0]->edge[theFace->edge[0]]->start];
                    glVertex3f(v->m_position.x,v->m_position.y,v->m_position.z);
				}
				else
				{
					SubdivideVertex *v=subdivideLevel[0]->vertex[subdivideLevel[0]->edge[-theFace->edge[0]]->end];
                    glVertex3f(v->m_position.x,v->m_position.y,v->m_position.z);
				}
				if(theFace->edge[1]>0)
				{
					SubdivideVertex *v=subdivideLevel[0]->vertex[subdivideLevel[0]->edge[theFace->edge[1]]->start];
                    glVertex3f(v->m_position.x,v->m_position.y,v->m_position.z);
				}
				else
				{
					SubdivideVertex *v=subdivideLevel[0]->vertex[subdivideLevel[0]->edge[-theFace->edge[1]]->end];
                    glVertex3f(v->m_position.x,v->m_position.y,v->m_position.z);
				}				
				if(theFace->edge[2]>0)
				{
					SubdivideVertex *v=subdivideLevel[0]->vertex[subdivideLevel[0]->edge[theFace->edge[2]]->start];
                    glVertex3f(v->m_position.x,v->m_position.y,v->m_position.z);
				}
				else
				{
					SubdivideVertex *v=subdivideLevel[0]->vertex[subdivideLevel[0]->edge[-theFace->edge[2]]->end];
                    glVertex3f(v->m_position.x,v->m_position.y,v->m_position.z);
				}
				if(theFace->edge[3]>0)
				{
					SubdivideVertex *v=subdivideLevel[0]->vertex[subdivideLevel[0]->edge[theFace->edge[3]]->start];
                    glVertex3f(v->m_position.x,v->m_position.y,v->m_position.z);
				}
				else
				{
					SubdivideVertex *v=subdivideLevel[0]->vertex[subdivideLevel[0]->edge[-theFace->edge[3]]->end];
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
            for(unsigned int i=1;i<subdivideLevel[0]->edge.size();++i)
			{
				SubdivideEdge *e=subdivideLevel[0]->edge[i];
				SubdivideVertex *start=subdivideLevel[0]->vertex[e->start];
				SubdivideVertex *end=subdivideLevel[0]->vertex[e->end];
                glVertex3f(start->m_position.x,start->m_position.y,start->m_position.z);
                glVertex3f(end->m_position.x,end->m_position.y,end->m_position.z);
			}

						glColor3ub(100,0,0);
            for(unsigned int i=1;i<edgeArray.size();++i)
			{
				Edge *e=edgeArray[i];
				if(e)
				{
					Vertex *start=vertexArray[e->start];
					Vertex *end=vertexArray[e->end];
                    glVertex3f(start->m_position.x,start->m_position.y,start->m_position.z);
                    glVertex3f(end->m_position.x,end->m_position.y,end->m_position.z);
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
            for(unsigned int i=1;i<faceArray.size();++i)
			{
				if(faceArray[i])
				{
					Face *theFace=faceArray[i];
					glBegin(GL_POLYGON);
					glNormal3f(theFace->normal.x,theFace->normal.y,theFace->normal.z);
                    for(unsigned int e=0;e<theFace->edge.size();++e)
					{
						if(theFace->edge[e]>0)
						{
							Vertex *v=vertexArray[edgeArray[theFace->edge[e]]->start];
                            glVertex3f(v->m_position.x,v->m_position.y,v->m_position.z);
						}
						else
						{
							Vertex *v=vertexArray[edgeArray[-theFace->edge[e]]->end];
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
            for(unsigned int i=1;i<edgeArray.size();++i)
			{
				Edge *e=edgeArray[i];
				if(e && !e->isSelected)
				{
					Vertex *start=vertexArray[e->start];
					Vertex *end=vertexArray[e->end];
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
            for(unsigned int i=1;i<subdivideLevel[0]->face.size();++i)
			{
				SubdivideFace *theFace=subdivideLevel[0]->face[i];
				glNormal3f(theFace->normal.x,theFace->normal.y,theFace->normal.z);
				if(theFace->edge[0]>0)
				{
					SubdivideVertex *v=subdivideLevel[0]->vertex[subdivideLevel[0]->edge[theFace->edge[0]]->start];
                    glVertex3f(v->m_position.x,v->m_position.y,v->m_position.z);
				}
				else
				{
					SubdivideVertex *v=subdivideLevel[0]->vertex[subdivideLevel[0]->edge[-theFace->edge[0]]->end];
                    glVertex3f(v->m_position.x,v->m_position.y,v->m_position.z);
				}
				if(theFace->edge[1]>0)
				{
					SubdivideVertex *v=subdivideLevel[0]->vertex[subdivideLevel[0]->edge[theFace->edge[1]]->start];
                    glVertex3f(v->m_position.x,v->m_position.y,v->m_position.z);
				}
				else
				{
					SubdivideVertex *v=subdivideLevel[0]->vertex[subdivideLevel[0]->edge[-theFace->edge[1]]->end];
                    glVertex3f(v->m_position.x,v->m_position.y,v->m_position.z);
				}				
				if(theFace->edge[2]>0)
				{
					SubdivideVertex *v=subdivideLevel[0]->vertex[subdivideLevel[0]->edge[theFace->edge[2]]->start];
                    glVertex3f(v->m_position.x,v->m_position.y,v->m_position.z);
				}
				else
				{
					SubdivideVertex *v=subdivideLevel[0]->vertex[subdivideLevel[0]->edge[-theFace->edge[2]]->end];
                    glVertex3f(v->m_position.x,v->m_position.y,v->m_position.z);
				}
				if(theFace->edge[3]>0)
				{
					SubdivideVertex *v=subdivideLevel[0]->vertex[subdivideLevel[0]->edge[theFace->edge[3]]->start];
                    glVertex3f(v->m_position.x,v->m_position.y,v->m_position.z);
				}
				else
				{
					SubdivideVertex *v=subdivideLevel[0]->vertex[subdivideLevel[0]->edge[-theFace->edge[3]]->end];
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
            for(unsigned int i=1;i<subdivideLevel[0]->edge.size();++i)
			{
				SubdivideEdge *e=subdivideLevel[0]->edge[i];
				SubdivideVertex *start=subdivideLevel[0]->vertex[e->start];
				SubdivideVertex *end=subdivideLevel[0]->vertex[e->end];
                glVertex3f(start->m_position.x,start->m_position.y,start->m_position.z);
                glVertex3f(end->m_position.x,end->m_position.y,end->m_position.z);
			}

			glColor3ub(100,0,0);
            for(unsigned int i=1;i<edgeArray.size();++i)
			{
				Edge *e=edgeArray[i];
				if(e && !e->isSelected)
				{
					Vertex *start=vertexArray[e->start];
					Vertex *end=vertexArray[e->end];
                    glVertex3f(start->m_position.x,start->m_position.y,start->m_position.z);
                    glVertex3f(end->m_position.x,end->m_position.y,end->m_position.z);
				}
			}
			glEnd();
			glEnable(GL_LIGHTING);
		}
		glPopMatrix();
    }

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
            for(unsigned int i=1;i<faceArray.size();++i)
			{
				if(faceArray[i])
				{
					Face *theFace=faceArray[i];
					glBegin(GL_POLYGON);
					glNormal3f(theFace->normal.x,theFace->normal.y,theFace->normal.z);
                    for(unsigned int e=0;e<theFace->edge.size();++e)
					{
						if(theFace->edge[e]>0)
						{
							Vertex *v=vertexArray[edgeArray[theFace->edge[e]]->start];
                            glVertex3f(v->m_position.x,v->m_position.y,v->m_position.z);
						}
						else
						{
							Vertex *v=vertexArray[edgeArray[-theFace->edge[e]]->end];
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
            for(unsigned int i=1;i<subdivideLevel[0]->face.size();++i)
			{
				SubdivideFace *theFace=subdivideLevel[0]->face[i];
				glNormal3f(theFace->normal.x,theFace->normal.y,theFace->normal.z);
				if(theFace->edge[0]>0)
				{
					SubdivideVertex *v=subdivideLevel[0]->vertex[subdivideLevel[0]->edge[theFace->edge[0]]->start];
                    glVertex3f(v->m_position.x,v->m_position.y,v->m_position.z);
				}
				else
				{
					SubdivideVertex *v=subdivideLevel[0]->vertex[subdivideLevel[0]->edge[-theFace->edge[0]]->end];
                    glVertex3f(v->m_position.x,v->m_position.y,v->m_position.z);
				}
				if(theFace->edge[1]>0)
				{
					SubdivideVertex *v=subdivideLevel[0]->vertex[subdivideLevel[0]->edge[theFace->edge[1]]->start];
                    glVertex3f(v->m_position.x,v->m_position.y,v->m_position.z);
				}
				else
				{
					SubdivideVertex *v=subdivideLevel[0]->vertex[subdivideLevel[0]->edge[-theFace->edge[1]]->end];
                    glVertex3f(v->m_position.x,v->m_position.y,v->m_position.z);
				}				
				if(theFace->edge[2]>0)
				{
					SubdivideVertex *v=subdivideLevel[0]->vertex[subdivideLevel[0]->edge[theFace->edge[2]]->start];
                    glVertex3f(v->m_position.x,v->m_position.y,v->m_position.z);
				}
				else
				{
					SubdivideVertex *v=subdivideLevel[0]->vertex[subdivideLevel[0]->edge[-theFace->edge[2]]->end];
                    glVertex3f(v->m_position.x,v->m_position.y,v->m_position.z);
				}
				if(theFace->edge[3]>0)
				{
					SubdivideVertex *v=subdivideLevel[0]->vertex[subdivideLevel[0]->edge[theFace->edge[3]]->start];
                    glVertex3f(v->m_position.x,v->m_position.y,v->m_position.z);
				}
				else
				{
					SubdivideVertex *v=subdivideLevel[0]->vertex[subdivideLevel[0]->edge[-theFace->edge[3]]->end];
                    glVertex3f(v->m_position.x,v->m_position.y,v->m_position.z);
				}
			}
			glEnd();
		}

    }


void Object::drawNormal()
	{
		glDisable(GL_LIGHTING);
		glBegin(GL_LINES);
        for(unsigned int i=1;i<vertexArray.size();++i)
		{
			Vertex *v=vertexArray[i];
            Vector normal=v->m_normal*10+v->m_position;
            glVertex3f(v->m_position.x,v->m_position.y,v->m_position.z);
			glVertex3f(normal.x,normal.y,normal.z);
		}
		glEnd();
		glEnable(GL_LIGHTING);
    }

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
            for(unsigned int i=1;i<faceArray.size();++i)
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
                    for(unsigned int e=0;e<theFace->edge.size();++e)
					{
						if(theFace->edge[e]>0)
						{
							Vertex *v=vertexArray[edgeArray[theFace->edge[e]]->start];
                            glNormal3f(v->m_normal.x,v->m_normal.y,v->m_normal.z);
                            glVertex3f(v->m_position.x,v->m_position.y,v->m_position.z);
						}
						else
						{
							Vertex *v=vertexArray[edgeArray[-theFace->edge[e]]->end];
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
            for(unsigned int i=1;i<subdivideLevel[0]->face.size();++i)
			{
				SubdivideFace *theFace=subdivideLevel[0]->face[i];
				if(theFace->edge[0]>0)
				{
					SubdivideVertex *v=subdivideLevel[0]->vertex[subdivideLevel[0]->edge[theFace->edge[0]]->start];
                    glNormal3f(v->m_normal.x,v->m_normal.y,v->m_normal.z);
                    glVertex3f(v->m_position.x,v->m_position.y,v->m_position.z);
				}
				else
				{
					SubdivideVertex *v=subdivideLevel[0]->vertex[subdivideLevel[0]->edge[-theFace->edge[0]]->end];
                    glNormal3f(v->m_normal.x,v->m_normal.y,v->m_normal.z);
                    glVertex3f(v->m_position.x,v->m_position.y,v->m_position.z);
				}
				if(theFace->edge[1]>0)
				{
					SubdivideVertex *v=subdivideLevel[0]->vertex[subdivideLevel[0]->edge[theFace->edge[1]]->start];
                    glNormal3f(v->m_normal.x,v->m_normal.y,v->m_normal.z);
                    glVertex3f(v->m_position.x,v->m_position.y,v->m_position.z);
				}
				else
				{
					SubdivideVertex *v=subdivideLevel[0]->vertex[subdivideLevel[0]->edge[-theFace->edge[1]]->end];
                    glNormal3f(v->m_normal.x,v->m_normal.y,v->m_normal.z);
                    glVertex3f(v->m_position.x,v->m_position.y,v->m_position.z);
				}				
				if(theFace->edge[2]>0)
				{
					SubdivideVertex *v=subdivideLevel[0]->vertex[subdivideLevel[0]->edge[theFace->edge[2]]->start];
                    glNormal3f(v->m_normal.x,v->m_normal.y,v->m_normal.z);
                    glVertex3f(v->m_position.x,v->m_position.y,v->m_position.z);
				}
				else
				{
					SubdivideVertex *v=subdivideLevel[0]->vertex[subdivideLevel[0]->edge[-theFace->edge[2]]->end];
                    glNormal3f(v->m_normal.x,v->m_normal.y,v->m_normal.z);
                    glVertex3f(v->m_position.x,v->m_position.y,v->m_position.z);
				}
				if(theFace->edge[3]>0)
				{
					SubdivideVertex *v=subdivideLevel[0]->vertex[subdivideLevel[0]->edge[theFace->edge[3]]->start];
                    glNormal3f(v->m_normal.x,v->m_normal.y,v->m_normal.z);
                    glVertex3f(v->m_position.x,v->m_position.y,v->m_position.z);
				}
				else
				{
					SubdivideVertex *v=subdivideLevel[0]->vertex[subdivideLevel[0]->edge[-theFace->edge[3]]->end];
                    glNormal3f(v->m_normal.x,v->m_normal.y,v->m_normal.z);
                    glVertex3f(v->m_position.x,v->m_position.y,v->m_position.z);
				}
			}
			glEnd();
		}

    }


void Object::drawWire(GLuint r,GLuint g,GLuint b)
	{
		glLineWidth(1);
		glDisable(GL_LIGHTING);
		glColor3ub((GLubyte)r,(GLubyte)g,(GLubyte)b);
		glLineWidth(1);
		if(subdivideLevelSize==0)
		{
			glBegin(GL_LINES);
            for(unsigned int i=1;i<edgeArray.size();++i)
			{
				Edge *e=edgeArray[i];
				if(e)
				{
					Vertex *start=vertexArray[e->start];
					Vertex *end=vertexArray[e->end];
                    glVertex3f(start->m_position.x,start->m_position.y,start->m_position.z);
                    glVertex3f(end->m_position.x,end->m_position.y,end->m_position.z);
				}
			}
			glEnd();
		}
		else
		{
			glBegin(GL_LINES);
            for(unsigned int i=1;i<subdivideLevel[0]->edge.size();++i)
			{
				SubdivideEdge *e=subdivideLevel[0]->edge[i];
				SubdivideVertex *start=subdivideLevel[0]->vertex[e->start];
				SubdivideVertex *end=subdivideLevel[0]->vertex[e->end];
                glVertex3f(start->m_position.x,start->m_position.y,start->m_position.z);
                glVertex3f(end->m_position.x,end->m_position.y,end->m_position.z);
			}
			glEnd();
		}
		glEnable(GL_LIGHTING);
    }


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
            for(unsigned int i=1;i<faceArray.size();++i)
			{
				if(faceArray[i])
				{
					Face *theFace=faceArray[i];
					glBegin(GL_POLYGON);
                    for(unsigned int e=0;e<theFace->edge.size();++e)
					{
						if(theFace->edge[e]>0)
						{
							Vertex *v=vertexArray[edgeArray[theFace->edge[e]]->start];
                            glNormal3f(v->m_normal.x,v->m_normal.y,v->m_normal.z);
                            glVertex3f(v->m_position.x,v->m_position.y,v->m_position.z);
						}
						else
						{
							Vertex *v=vertexArray[edgeArray[-theFace->edge[e]]->end];
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
            for(unsigned int i=1;i<edgeArray.size();++i)
			{
				Edge *e=edgeArray[i];
				if(e)
				{
					Vertex *start=vertexArray[e->start];
					Vertex *end=vertexArray[e->end];
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
            for(unsigned int i=1;i<subdivideLevel[0]->face.size();++i)
			{
				//glBegin(GL_POLYGON);
				SubdivideFace *theFace=subdivideLevel[0]->face[i];
			//	glNormal3f(theFace->normal.x,theFace->normal.y,theFace->normal.z);
				if(theFace->edge[0]>0)
				{
					SubdivideVertex *v=subdivideLevel[0]->vertex[subdivideLevel[0]->edge[theFace->edge[0]]->start];
                    glNormal3f(v->m_normal.x,v->m_normal.y,v->m_normal.z);
                    glVertex3f(v->m_position.x,v->m_position.y,v->m_position.z);
				}
				else
				{
					SubdivideVertex *v=subdivideLevel[0]->vertex[subdivideLevel[0]->edge[-theFace->edge[0]]->end];
                    glNormal3f(v->m_normal.x,v->m_normal.y,v->m_normal.z);
                    glVertex3f(v->m_position.x,v->m_position.y,v->m_position.z);
				}
				if(theFace->edge[1]>0)
				{
					SubdivideVertex *v=subdivideLevel[0]->vertex[subdivideLevel[0]->edge[theFace->edge[1]]->start];
                    glNormal3f(v->m_normal.x,v->m_normal.y,v->m_normal.z);
                    glVertex3f(v->m_position.x,v->m_position.y,v->m_position.z);
				}
				else
				{
					SubdivideVertex *v=subdivideLevel[0]->vertex[subdivideLevel[0]->edge[-theFace->edge[1]]->end];
                    glNormal3f(v->m_normal.x,v->m_normal.y,v->m_normal.z);
                    glVertex3f(v->m_position.x,v->m_position.y,v->m_position.z);
				}				
				if(theFace->edge[2]>0)
				{
					SubdivideVertex *v=subdivideLevel[0]->vertex[subdivideLevel[0]->edge[theFace->edge[2]]->start];
                    glNormal3f(v->m_normal.x,v->m_normal.y,v->m_normal.z);
                    glVertex3f(v->m_position.x,v->m_position.y,v->m_position.z);
				}
				else
				{
					SubdivideVertex *v=subdivideLevel[0]->vertex[subdivideLevel[0]->edge[-theFace->edge[2]]->end];
                    glNormal3f(v->m_normal.x,v->m_normal.y,v->m_normal.z);
                    glVertex3f(v->m_position.x,v->m_position.y,v->m_position.z);
				}
				if(theFace->edge[3]>0)
				{
					SubdivideVertex *v=subdivideLevel[0]->vertex[subdivideLevel[0]->edge[theFace->edge[3]]->start];
                    glNormal3f(v->m_normal.x,v->m_normal.y,v->m_normal.z);
                    glVertex3f(v->m_position.x,v->m_position.y,v->m_position.z);
				}
				else
				{
					SubdivideVertex *v=subdivideLevel[0]->vertex[subdivideLevel[0]->edge[-theFace->edge[3]]->end];
                    glNormal3f(v->m_normal.x,v->m_normal.y,v->m_normal.z);
                    glVertex3f(v->m_position.x,v->m_position.y,v->m_position.z);
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
            for(unsigned int i=1;i<subdivideLevel[0]->edge.size();++i)
			{
				SubdivideEdge *e=subdivideLevel[0]->edge[i];
				SubdivideVertex *start=subdivideLevel[0]->vertex[e->start];
				SubdivideVertex *end=subdivideLevel[0]->vertex[e->end];
                glVertex3f(start->m_position.x,start->m_position.y,start->m_position.z);
                glVertex3f(end->m_position.x,end->m_position.y,end->m_position.z);
			}
			glEnd();
			glEnable(GL_LIGHTING);
		}

    }


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
            for(unsigned int i=1;i<faceArray.size();++i)
			{
				if(faceArray[i])
				{
					Face *theFace=faceArray[i];
					glBegin(GL_POLYGON);
                    for(unsigned int e=0;e<theFace->edge.size();++e)
					{
						if(theFace->edge[e]>0)
						{
							Vertex *v=vertexArray[edgeArray[theFace->edge[e]]->start];
                            glVertex3f(v->m_position.x,v->m_position.y,v->m_position.z);
						}
						else
						{
							Vertex *v=vertexArray[edgeArray[-theFace->edge[e]]->end];
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
            for(unsigned int i=1;i<subdivideLevel[0]->face.size();++i)
			{
				SubdivideFace *theFace=subdivideLevel[0]->face[i];
				if(theFace->edge[0]>0)
				{
					SubdivideVertex *v=subdivideLevel[0]->vertex[subdivideLevel[0]->edge[theFace->edge[0]]->start];
                    glVertex3f(v->m_position.x,v->m_position.y,v->m_position.z);
				}
				else
				{
					SubdivideVertex *v=subdivideLevel[0]->vertex[subdivideLevel[0]->edge[-theFace->edge[0]]->end];
                    glVertex3f(v->m_position.x,v->m_position.y,v->m_position.z);
				}
				if(theFace->edge[1]>0)
				{
					SubdivideVertex *v=subdivideLevel[0]->vertex[subdivideLevel[0]->edge[theFace->edge[1]]->start];
                    glVertex3f(v->m_position.x,v->m_position.y,v->m_position.z);
				}
				else
				{
					SubdivideVertex *v=subdivideLevel[0]->vertex[subdivideLevel[0]->edge[-theFace->edge[1]]->end];
                    glVertex3f(v->m_position.x,v->m_position.y,v->m_position.z);
				}				
				if(theFace->edge[2]>0)
				{
					SubdivideVertex *v=subdivideLevel[0]->vertex[subdivideLevel[0]->edge[theFace->edge[2]]->start];
                    glVertex3f(v->m_position.x,v->m_position.y,v->m_position.z);
				}
				else
				{
					SubdivideVertex *v=subdivideLevel[0]->vertex[subdivideLevel[0]->edge[-theFace->edge[2]]->end];
                    glVertex3f(v->m_position.x,v->m_position.y,v->m_position.z);
				}
				if(theFace->edge[3]>0)
				{
					SubdivideVertex *v=subdivideLevel[0]->vertex[subdivideLevel[0]->edge[theFace->edge[3]]->start];
                    glVertex3f(v->m_position.x,v->m_position.y,v->m_position.z);
				}
				else
				{
					SubdivideVertex *v=subdivideLevel[0]->vertex[subdivideLevel[0]->edge[-theFace->edge[3]]->end];
                    glVertex3f(v->m_position.x,v->m_position.y,v->m_position.z);
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
            for(unsigned int i=1;i<faceArray.size();++i)
			{
				if(faceArray[i])
				{
					Face *theFace=faceArray[i];
					glBegin(GL_POLYGON);
					glNormal3f(theFace->normal.x,theFace->normal.y,theFace->normal.z);
                    for(unsigned int e=0;e<theFace->edge.size();++e)
					{
						if(theFace->edge[e]>0)
						{
							Vertex *v=vertexArray[edgeArray[theFace->edge[e]]->start];
                            glVertex3f(v->m_position.x,v->m_position.y,v->m_position.z);
						}
						else
						{
							Vertex *v=vertexArray[edgeArray[-theFace->edge[e]]->end];
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
            for(unsigned int i=1;i<edgeArray.size();++i)
			{
				Edge *e=edgeArray[i];
				if(e)
				{
					Vertex *start=vertexArray[e->start];
					Vertex *end=vertexArray[e->end];
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
            for(unsigned int i=1;i<subdivideLevel[0]->face.size();++i)
			{
				SubdivideFace *theFace=subdivideLevel[0]->face[i];
				glNormal3f(theFace->normal.x,theFace->normal.y,theFace->normal.z);
				if(theFace->edge[0]>0)
				{
					SubdivideVertex *v=subdivideLevel[0]->vertex[subdivideLevel[0]->edge[theFace->edge[0]]->start];
                    glVertex3f(v->m_position.x,v->m_position.y,v->m_position.z);
				}
				else
				{
					SubdivideVertex *v=subdivideLevel[0]->vertex[subdivideLevel[0]->edge[-theFace->edge[0]]->end];
                    glVertex3f(v->m_position.x,v->m_position.y,v->m_position.z);
				}
				if(theFace->edge[1]>0)
				{
					SubdivideVertex *v=subdivideLevel[0]->vertex[subdivideLevel[0]->edge[theFace->edge[1]]->start];
                    glVertex3f(v->m_position.x,v->m_position.y,v->m_position.z);
				}
				else
				{
					SubdivideVertex *v=subdivideLevel[0]->vertex[subdivideLevel[0]->edge[-theFace->edge[1]]->end];
                    glVertex3f(v->m_position.x,v->m_position.y,v->m_position.z);
				}				
				if(theFace->edge[2]>0)
				{
					SubdivideVertex *v=subdivideLevel[0]->vertex[subdivideLevel[0]->edge[theFace->edge[2]]->start];
                    glVertex3f(v->m_position.x,v->m_position.y,v->m_position.z);
				}
				else
				{
					SubdivideVertex *v=subdivideLevel[0]->vertex[subdivideLevel[0]->edge[-theFace->edge[2]]->end];
                    glVertex3f(v->m_position.x,v->m_position.y,v->m_position.z);
				}
				if(theFace->edge[3]>0)
				{
					SubdivideVertex *v=subdivideLevel[0]->vertex[subdivideLevel[0]->edge[theFace->edge[3]]->start];
                    glVertex3f(v->m_position.x,v->m_position.y,v->m_position.z);
				}
				else
				{
					SubdivideVertex *v=subdivideLevel[0]->vertex[subdivideLevel[0]->edge[-theFace->edge[3]]->end];
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
            for(unsigned int i=1;i<subdivideLevel[0]->edge.size();++i)
			{
				SubdivideEdge *e=subdivideLevel[0]->edge[i];
				SubdivideVertex *start=subdivideLevel[0]->vertex[e->start];
				SubdivideVertex *end=subdivideLevel[0]->vertex[e->end];
                glVertex3f(start->m_position.x,start->m_position.y,start->m_position.z);
                glVertex3f(end->m_position.x,end->m_position.y,end->m_position.z);
			}
			glEnd();
			glEnable(GL_LIGHTING);
		}
}

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
}

void Object::buildPSCacheFromEID(std::vector<unsigned int> &edgeToBeSub)
{
	if(subdivideLevelSize>0)
	{
        std::vector<unsigned int> vertexToBeSub;
        //vertexToBeSub.clear();
		vertexToBeSub.reserve(1000);
        for(unsigned int i=0;i<edgeToBeSub.size();++i)
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

        for(unsigned int i=0;i<vertexToBeSub.size();++i)
		{
			vertexArray[vertexToBeSub[i]]->isSub=false;
		}

		buildPSCacheFromVID(vertexToBeSub);
	}
}

void Object::buildPSCacheFromFID(std::vector<unsigned int> &faceToBeSub)
{
	if(subdivideLevelSize>0)
	{
        std::vector<unsigned int> vertexToBeSub;
        //vertexToBeSub.clear();
		vertexToBeSub.reserve(1000);
        for(unsigned int i=0;i<faceToBeSub.size();++i)
		{
			Face *theFace=faceArray[faceToBeSub[i]];
            for(unsigned int e=0;e<theFace->edge.size();++e)
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

        for(unsigned int i=0;i<vertexToBeSub.size();++i)
		{
			vertexArray[vertexToBeSub[i]]->isSub=false;
		}

		buildPSCacheFromVID(vertexToBeSub);
	}
}

void Object::buildPSCacheFromVID(std::vector<unsigned int> &vertexToBeSub)
{
	if(subdivideLevelSize>0)
	{
		std::vector<Face *> faceToBeSub;
        //faceToBeSub.clear();
		faceToBeSub.reserve(1000);
        for(unsigned int i=0;i<vertexToBeSub.size();++i)
		{
			Vertex *theVertex=vertexArray[vertexToBeSub[i]];
			if(theVertex)
			{
                for(unsigned int e=0;e<theVertex->m_adjacentEdgeList.size();++e)
				{
                    Edge *theEdge=edgeArray[theVertex->m_adjacentEdgeList[e]];
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

void Object::expandSubFace(std::vector<SubdivideFace*> &originalList,unsigned int level)
{
	//ÕâÀïoriginalListÓ¦¸Ã·ÖÅäºÃ¿Õ¼ä
	//originalListÖÐµÄidÓ¦¸ÃÊÇ´ýÏ¸·ÖµÄ×´Ì¬
	SubdivideLevel *theLevel=subdivideLevel[level];
    unsigned int originalCount=originalList.size();
	std::vector<SubdivideVertex *> tempVertexList;
    //tempVertexList.clear();
	tempVertexList.reserve(originalCount*4);
    for(unsigned int i=0;i<originalCount;++i)
	{
		SubdivideFace *theFace=originalList[i];
        for(unsigned int e=0;e<4;++e)
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

    for(unsigned int i=0;i<tempVertexList.size();++i)
	{
		SubdivideVertex *theVertex=tempVertexList[i];
		theVertex->isSub=false;
        for(unsigned int e=0;e<theVertex->m_adjacentEdgeList.size();++e)
		{
            SubdivideEdge *theEdge=theLevel->edge[theVertex->m_adjacentEdgeList[e]];
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
            unsigned int adjacentEdgeCount=theVertex->adjacentEdge.size();
            for(unsigned int h=0;h<adjacentEdgeCount;++h)
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
	//ÕâÀïoriginalListÓ¦¸Ã·ÖÅäºÃ¿Õ¼ä
	//originalListÖÐµÄidÓ¦¸ÃÊÇ´ýÏ¸·ÖµÄ×´Ì¬
    unsigned int originalCount=originalList.size();
	std::vector<Vertex *> tempVertexList;
    //tempVertexList.clear();
	tempVertexList.reserve(originalCount*5);
    for(unsigned int i=0;i<originalCount;++i)
	{
		Face *theFace=originalList[i];
        unsigned int edgeCount=theFace->edge.size();
        for(unsigned int e=0;e<edgeCount;++e)
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
    for(unsigned int i=0;i<tempVertexList.size();++i)
	{
		Vertex *theVertex=tempVertexList[i];
		theVertex->isSub=false;
        for(unsigned int e=0;e<theVertex->m_adjacentEdgeList.size();++e)
		{
            Edge *theEdge=edgeArray[theVertex->m_adjacentEdgeList[e]];
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
	//ÕâÀïoriginalListÓ¦¸Ã·ÖÅäºÃ¿Õ¼ä
	//originalListÖÐµÄidÓ¦¸ÃÊÇ´ýÏ¸·ÖµÄ×´Ì¬
    unsigned int originalCount=originalList.size();
    for(unsigned int i=0;i<originalCount;++i)
	{
		Face *theFace=originalList[i];
        unsigned int edgeCount=theFace->edge.size();
        for(unsigned int e=0;e<edgeCount;++e)
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
            unsigned int adjacentEdgeCount=theVertex->adjacentEdge.size();
            for(unsigned int h=0;h<adjacentEdgeCount;++h)
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
        unsigned int level=subdivideLevelSize-1;
        unsigned int faceCount=faceToBeSub.size();
        //PSFaceCache.clear();
		PSFaceCache.reserve(faceCount);

        //PSSubFaceCache[level].clear();
		PSSubFaceCache[level].reserve(faceCount*5);
        for(unsigned int i=0;i<faceCount;++i)
		{
			PSFaceCache.push_back(faceToBeSub[i]);
			faceToBeSub[i]->isSub=true;
            unsigned int subFaceCount=faceToBeSub[i]->subdivideFace.size();
            for(unsigned int h=0;h<subFaceCount;++h)
			{
				SubdivideFace *theSubF=subdivideLevel[level]->face[faceToBeSub[i]->subdivideFace[h]];
				PSSubFaceCache[level].push_back(theSubF);
				theSubF->isSub=true;
			}
		}
		expandSubFace(PSSubFaceCache[level],level);
		for(int h=level-1;h>0;--h)
		{
            unsigned int e2=h;
            unsigned int e1=h+1;
			faceCount=PSSubFaceCache[e1].size();
            //PSSubFaceCache[e2].clear();
			PSSubFaceCache[e2].reserve(faceCount*5);
            for(unsigned int i=0;i<faceCount;++i)
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
        unsigned int faceCount=faceArray.size();
        for(unsigned int i=1;i<faceCount;++i)
		{
			if(faceArray[i])
			{
				updateFNormal(faceArray[i]);
			}
		}
        unsigned int vertexCount=vertexArray.size();
        for(unsigned int i=1;i<vertexCount;++i)
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
        unsigned int level=subdivideLevelSize-1;
		++subdivideId;
        unsigned int faceCount=PSFaceCache.size();
        for(unsigned int i=0;i<faceCount;++i)
		{
			partialSubdivideFace(PSFaceCache[i],level);
		}
        for(unsigned int e=level;e>0;--e)
		{
			faceCount=PSSubFaceCache[e].size();
            for(unsigned int i=0;i<faceCount;++i)
			{
				partialSubdivideFace((PSSubFaceCache[e])[i],e);
			}
		}
	}
}

void Object::updateSubdivision()
	{
        unsigned int subdivideCount=subdivideLevelSize;
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
	//Ê×ÏÈÒªµÃµ½Õâ¸öÃæµÄ¶Ëµã
	//µÃµ½¶ËµãºÍ±ßµÄÊýÄ¿
    unsigned int edgeCount=4;
	//ÐÂ½¨´æ·Å¶¥µãµÄÊý×é
	SubdivideVertex **theVertexList=new SubdivideVertex*[edgeCount];
	//¼ÆËãÕâ¸öÃæµÄÖÐµã
	//½«Ô­Ê¼µÄÖÐµãÇåÁã
    theSubLevel->vertex[theFace->center]->m_position.null();
    theSubLevel->vertex[theFace->center]->m_normal.null();
    for(unsigned int e=0;e<edgeCount;++e)
	{
		if(theFace->edge[e]<0)
		{
			theVertexList[e]=theLevel->vertex[theLevel->edge[-theFace->edge[e]]->start];
		}
		else
		{
			theVertexList[e]=theLevel->vertex[theLevel->edge[theFace->edge[e]]->end];
		}
        theSubLevel->vertex[theFace->center]->m_position+=theVertexList[e]->m_position;
        theSubLevel->vertex[theFace->center]->m_normal+=theVertexList[e]->m_normal;
	}
    theSubLevel->vertex[theFace->center]->m_position/=(float)edgeCount;
    theSubLevel->vertex[theFace->center]->m_normal/=(float)edgeCount;
	//¼ÆËãÐÂµÄ¶¥µã
    for(unsigned int i=0;i<edgeCount;++i)
	{
		SubdivideVertex *theV=theVertexList[i];
        if(theV->m_subdivideId!=subdivideId)
		{
			//Èç¹ûÊÇµÚÒ»´Î¼ÆËãµ½Õâ¸öµã
			//³õÊ¼»¯Ï¸·Ö²½Êý
            theV->m_subdivideId=subdivideId;
            theV->m_subdivideStep=0;
			//Éú³ÉÕâ¸öµãµÄÏ¸·Öµã
			//Ê×ÏÈ¼ÆËãÏàÁÚµãµÄ×ø±êºÍEv
            theSubLevel->vertex[theV->m_nextLevel]->m_position.null();
            theSubLevel->vertex[theV->m_nextLevel]->m_position+=EAdjacentVertex(theV,level);
            if(theV->m_edgeVertex)
			{
                theSubLevel->vertex[theV->m_nextLevel]->m_position+=theV->m_position*6;
                theSubLevel->vertex[theV->m_nextLevel]->m_position/=8;
			}
			else
			{
				//¼ÓÉÏÕâ¸öÃæµÄÖÐµã
                theSubLevel->vertex[theV->m_nextLevel]->m_position+=theSubLevel->vertex[theFace->center]->m_position;
                theV->m_subdivideStep++;
                unsigned int adjEdgeCount=theV->m_adjacentEdgeList.size();
                for(unsigned int h=0;h<adjEdgeCount;++h)
				{
                    if(theLevel->edge[theV->m_adjacentEdgeList[h]]->end==theV->index)
					{
                        if(!theLevel->face[theLevel->edge[theV->m_adjacentEdgeList[h]]->left]->isSub)
						{
                            theSubLevel->vertex[theV->m_nextLevel]->m_position+=theSubLevel->vertex[theLevel->face[theLevel->edge[theV->m_adjacentEdgeList[h]]->left]->center]->m_position;
                            theV->m_subdivideStep++;
						}
					}
					else
					{
                        if(!theLevel->face[theLevel->edge[theV->m_adjacentEdgeList[h]]->right]->isSub)
						{
                            theSubLevel->vertex[theV->m_nextLevel]->m_position+=theSubLevel->vertex[theLevel->face[theLevel->edge[theV->m_adjacentEdgeList[h]]->right]->center]->m_position;
                            theV->m_subdivideStep++;
						}			
					}
				}
                unsigned int n=theV->m_adjacentEdgeList.size();
                if(n==(unsigned int)(theV->m_subdivideStep))
				{
                    theSubLevel->vertex[theV->m_nextLevel]->m_position+=theV->m_position*(float)(n*n-2*n);
                    theSubLevel->vertex[theV->m_nextLevel]->m_position/=(float)(n*n);
				}
			}
		}
		else
		{
            if(!theV->m_edgeVertex)
			{
                theSubLevel->vertex[theV->m_nextLevel]->m_position+=theSubLevel->vertex[theFace->center]->m_position;
                theV->m_subdivideStep++;
                unsigned int n=theV->m_adjacentEdgeList.size();
                if(n==(unsigned int)(theV->m_subdivideStep))
				{
                    theSubLevel->vertex[theV->m_nextLevel]->m_position+=theV->m_position*(float)(n*n-2*n);
                    theSubLevel->vertex[theV->m_nextLevel]->m_position/=(float)(n*n);
				}
			}
		}
	}
	delete theVertexList;
	//Ö®ºó´¦ÀíÃ¿Ò»¸ö±ß
    for(unsigned int i=0;i<edgeCount;++i)
	{
		SubdivideEdge *theEdge;
		if(theFace->edge[i]>0)
		{
			theEdge=theLevel->edge[theFace->edge[i]];
			if(theEdge->subdivideId!=subdivideId)
			{
				//Èç¹ûÊÇµÚÒ»´Î´¦ÀíÕâ¸ö±ß
				theEdge->subdivideId=subdivideId;
                theSubLevel->vertex[theEdge->middle]->m_position.null();
                theSubLevel->vertex[theEdge->middle]->m_position+=theLevel->vertex[theEdge->start]->m_position+theLevel->vertex[theEdge->end]->m_position;
				if(theEdge->left!=0 )
				{
					if(theLevel->face[theEdge->left]->isSub)
					{
						//Èç¹ûÕâ¸ö±ßµÄÁ½±ß¶¼ÔÚ¾Ö²¿Ï¸·ÖµÄ·¶Î§Ö®ÄÚ
                        theSubLevel->vertex[theEdge->middle]->m_position+=theSubLevel->vertex[theFace->center]->m_position;
					}
					else
					{
                        theSubLevel->vertex[theEdge->middle]->m_position+=theSubLevel->vertex[theLevel->face[theEdge->left]->center]->m_position+theSubLevel->vertex[theFace->center]->m_position;
                        theSubLevel->vertex[theEdge->middle]->m_position/=4;
					}
				}
				else
				{
                    theSubLevel->vertex[theEdge->middle]->m_position/=2;
				}
			}
			else
			{
				//Èç¹û²»ÊÇµÚÒ»´Î¼ÆËã
                theSubLevel->vertex[theEdge->middle]->m_position+=theSubLevel->vertex[theFace->center]->m_position;
                theSubLevel->vertex[theEdge->middle]->m_position/=4;
			}
		}
		else
		{
			theEdge=theLevel->edge[-theFace->edge[i]];
			if(theEdge->subdivideId!=subdivideId)
			{
				//Èç¹ûÊÇµÚÒ»´Î´¦ÀíÕâ¸ö±ß
				theEdge->subdivideId=subdivideId;
                theSubLevel->vertex[theEdge->middle]->m_position.null();
                theSubLevel->vertex[theEdge->middle]->m_position+=theLevel->vertex[theEdge->start]->m_position+theLevel->vertex[theEdge->end]->m_position;
				if( theEdge->right!=0)
				{
					if(theLevel->face[theEdge->right]->isSub)
					{
                        theSubLevel->vertex[theEdge->middle]->m_position+=theSubLevel->vertex[theFace->center]->m_position;
					}
					else
					{
                        theSubLevel->vertex[theEdge->middle]->m_position+=theSubLevel->vertex[theLevel->face[theEdge->right]->center]->m_position+theSubLevel->vertex[theFace->center]->m_position;
                        theSubLevel->vertex[theEdge->middle]->m_position/=4;
					}
				}
				else
				{
                    theSubLevel->vertex[theEdge->middle]->m_position/=2;
				}
			}
			else
			{
				//Èç¹û²»ÊÇµÚÒ»´Î¼ÆËã
                theSubLevel->vertex[theEdge->middle]->m_position+=theSubLevel->vertex[theFace->center]->m_position;
                theSubLevel->vertex[theEdge->middle]->m_position/=4;
			}
		}
	}
}

void Object::buildPSCacheFast(std::vector<Face*> &faceToBeSub)
{
	if(subdivideLevelSize>0)
	{
        unsigned int subCount=faceToBeSub.size();
        //PSFaceCache.clear();
		PSFaceCache.reserve(subCount*2);
        for(unsigned int i=0;i<subCount;++i)
		{
			PSFaceCache.push_back(faceToBeSub[i]);
			faceToBeSub[i]->isSub=true;
		}
		expandSubFace(faceToBeSub);
		subCount=PSFaceCache.size();
        unsigned int level=subdivideLevelSize-1;
        //PSSubFaceCache[level].clear();
		PSSubFaceCache[level].reserve(5*subCount);
        for(unsigned int i=0;i<subCount;++i)
		{
            unsigned int subFaceCount=PSFaceCache[i]->subdivideFace.size();
            for(unsigned int h=0;h<subFaceCount;++h)
			{
				PSSubFaceCache[level].push_back(subdivideLevel[level]->face[PSFaceCache[i]->subdivideFace[h]]);
				subdivideLevel[level]->face[PSFaceCache[i]->subdivideFace[h]]->isSub=true;
			}
		}
        for(unsigned int e=level;e>0;--e)
		{
            unsigned int subCount=PSSubFaceCache[e].size();
            unsigned int e2=e-1;
            //PSSubFaceCache[e2].clear();
			PSSubFaceCache[e2].reserve(subCount*2);
            for(unsigned int i=0;i<subCount;++i)
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
	//Ê×ÏÈÒªµÃµ½Õâ¸öÃæµÄ¶Ëµã
	//µÃµ½¶ËµãºÍ±ßµÄÊýÄ¿
    unsigned int edgeCount=theFace->edge.size();
	SubdivideLevel *theLevel=subdivideLevel[level];
	//ÐÂ½¨´æ·Å¶¥µãµÄÊý×é
	Vertex **theVertexList=new Vertex*[edgeCount];
    theLevel->vertex[theFace->center]->m_position.null();
    theLevel->vertex[theFace->center]->m_normal.null();
    for(unsigned int e=0;e<edgeCount;++e)
	{
		if(theFace->edge[e]<0)
		{
			theVertexList[e]=vertexArray[edgeArray[-theFace->edge[e]]->start];
		}
		else
		{
			theVertexList[e]=vertexArray[edgeArray[theFace->edge[e]]->end];
		}
        theLevel->vertex[theFace->center]->m_position+=theVertexList[e]->m_position;
        theLevel->vertex[theFace->center]->m_normal+=theVertexList[e]->m_normal;
	}
	//¼ÆËãÕâ¸öÃæµÄÖÐµã
    theLevel->vertex[theFace->center]->m_position/=(float)edgeCount;
    theLevel->vertex[theFace->center]->m_normal/=(float)edgeCount;
	//¼ÆËãÐÂµÄ¶¥µã
    for(unsigned int i=0;i<edgeCount;++i)
	{
		Vertex *theV=theVertexList[i];
        if(theV->m_subdivideId!=subdivideId)
		{
			//Èç¹ûÊÇµÚÒ»´Î¼ÆËãµ½Õâ¸öµã
			//³õÊ¼»¯Ï¸·Ö²½Êý
            theV->m_subdivideId=subdivideId;
            theV->m_subdivideStep=0;
			//Éú³ÉÕâ¸öµãµÄÏ¸·Öµã
			//Ê×ÏÈ¼ÆËãÏàÁÚµãµÄ×ø±êºÍEv
            theLevel->vertex[theV->m_nextLevel]->m_position.null();
            theLevel->vertex[theV->m_nextLevel]->m_position+=EAdjacentVertex(theV);
            if(theV->m_edgeVertex)
			{
                theLevel->vertex[theV->m_nextLevel]->m_position+=theV->m_position*6;
                theLevel->vertex[theV->m_nextLevel]->m_position/=8;
			}
			else
			{
				//¼ÓÉÏÕâ¸öÃæµÄÖÐµã
                theLevel->vertex[theV->m_nextLevel]->m_position+=theLevel->vertex[theFace->center]->m_position;
                theV->m_subdivideStep++;
                unsigned int adjEdgeCount=theV->m_adjacentEdgeList.size();
                for(unsigned int h=0;h<adjEdgeCount;++h)
				{
                    if(edgeArray[theV->m_adjacentEdgeList[h]]->end==theV->index)
					{
                        if(!faceArray[edgeArray[theV->m_adjacentEdgeList[h]]->left]->isSub)
						{
                            theLevel->vertex[theV->m_nextLevel]->m_position+=theLevel->vertex[faceArray[edgeArray[theV->m_adjacentEdgeList[h]]->left]->center]->m_position;
                            ++(theV->m_subdivideStep);
						}
					}
					else
					{
                        if(!faceArray[edgeArray[theV->m_adjacentEdgeList[h]]->right]->isSub)
						{
                            theLevel->vertex[theV->m_nextLevel]->m_position+=theLevel->vertex[faceArray[edgeArray[theV->m_adjacentEdgeList[h]]->right]->center]->m_position;
                            ++(theV->m_subdivideStep);
						}			
					}
				}
                unsigned int n=theV->m_adjacentEdgeList.size();
                if(n==(unsigned int)(theV->m_subdivideStep))
				{
                    theLevel->vertex[theV->m_nextLevel]->m_position+=theV->m_position*(float)(n*n-2*n);
                    theLevel->vertex[theV->m_nextLevel]->m_position/=(float)(n*n);
				}
			}
		}
		else
		{
            if(!theV->m_edgeVertex)
			{
                theLevel->vertex[theV->m_nextLevel]->m_position+=theLevel->vertex[theFace->center]->m_position;
                ++(theV->m_subdivideStep);
                unsigned int n=theV->m_adjacentEdgeList.size();
                if(n==(unsigned int)(theV->m_subdivideStep))
				{
                    theLevel->vertex[theV->m_nextLevel]->m_position+=theVertexList[i]->m_position*(float)(n*n-2*n);
                    theLevel->vertex[theV->m_nextLevel]->m_position/=(float)(n*n);
				}
			}
		}
	}
	delete theVertexList;
	//Ö®ºó´¦ÀíÃ¿Ò»¸ö±ß
    for(unsigned int i=0;i<edgeCount;++i)
	{
		Edge *theEdge;
		if(theFace->edge[i]>0)
		{
			theEdge=edgeArray[theFace->edge[i]];
			if(theEdge->subdivideId!=subdivideId)
			{
				//Èç¹ûÊÇµÚÒ»´Î´¦ÀíÕâ¸ö±ß
				theEdge->subdivideId=subdivideId;
                theLevel->vertex[theEdge->middle]->m_position.null();
                theLevel->vertex[theEdge->middle]->m_position+=vertexArray[theEdge->start]->m_position+vertexArray[theEdge->end]->m_position;
				if(theEdge->left)
				{
					if(faceArray[theEdge->left]->isSub)
					{
						//Èç¹ûÕâ¸ö±ßµÄÁ½±ß¶¼ÔÚ¾Ö²¿Ï¸·ÖµÄ·¶Î§Ö®ÄÚ
                        theLevel->vertex[theEdge->middle]->m_position+=theLevel->vertex[theFace->center]->m_position;
					}
					else
					{
                        theLevel->vertex[theEdge->middle]->m_position+=theLevel->vertex[faceArray[theEdge->left]->center]->m_position+theLevel->vertex[theFace->center]->m_position;
                        theLevel->vertex[theEdge->middle]->m_position/=4;
					}
				}
				else
				{
                    theLevel->vertex[theEdge->middle]->m_position/=2;
				}
			}
			else
			{
				//Èç¹û²»ÊÇµÚÒ»´Î¼ÆËã
                theLevel->vertex[theEdge->middle]->m_position+=theLevel->vertex[theFace->center]->m_position;
                theLevel->vertex[theEdge->middle]->m_position/=4;
			}
		}
		else
		{
			theEdge=edgeArray[-theFace->edge[i]];
			if(theEdge->subdivideId!=subdivideId)
			{
				//Èç¹ûÊÇµÚÒ»´Î´¦ÀíÕâ¸ö±ß
				theEdge->subdivideId=subdivideId;
                theLevel->vertex[theEdge->middle]->m_position.null();
                theLevel->vertex[theEdge->middle]->m_position+=vertexArray[theEdge->start]->m_position+vertexArray[theEdge->end]->m_position;
				if(theEdge->right)
				{
					if(faceArray[theEdge->right]->isSub)
					{
                        theLevel->vertex[theEdge->middle]->m_position+=theLevel->vertex[theFace->center]->m_position;
					}
					else
					{
                        theLevel->vertex[theEdge->middle]->m_position+=theLevel->vertex[faceArray[theEdge->right]->center]->m_position+theLevel->vertex[theFace->center]->m_position;
                        theLevel->vertex[theEdge->middle]->m_position/=4;
					}
				}
				else
				{
                    theLevel->vertex[theEdge->middle]->m_position/=2;
				}
			}
			else
			{
				//Èç¹û²»ÊÇµÚÒ»´Î¼ÆËã
                theLevel->vertex[theEdge->middle]->m_position+=theLevel->vertex[theFace->center]->m_position;
                theLevel->vertex[theEdge->middle]->m_position/=4;
			}
		}
	}
}


Object::~Object(void)
{
}
