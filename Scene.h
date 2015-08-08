#pragma once
 #pragma warning(disable : 4996)

#include "Enum.h"
#include "IndexArray.h"
#include "Object.h"
#include "Log_SceneObjectAdd.h"
#include "Log_SceneObjectRemove.h"
#include "AxisCursor.h"
#include <fstream>
#include "Matrix.h"
#include "Quaternion.h"

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
	size_t objectCount;
};

class Scene
{
private:
	std::vector<ElementBase *> isInCache;
	AxisCursor *theAxisCursor;
	AxisCursorMode currentACMode;
	IndexArray<Object> theObjectList;
	std::vector<size_t> selection;
	size_t target;
	SelectionMode mode;

public:
	std::string fileName;
	size_t splitVertexID;
	bool isSplitMode;
	//size_t splitVX;
	//size_t splitVY;
	
	Scene(void);
	size_t newCube(float x,float y,float z,float lx,float ly,float lz,size_t sx,size_t sy,size_t sz);
	void newCube(Object *theCube,float x,float y,float z,float lx,float ly,float lz,size_t sx,size_t sy,size_t sz);
	size_t newCylinder(float x,float y,float z,float r,float h,AxisMode::__Enum axis,size_t sa,size_t sr,size_t sh);
	void newCylinder(Object *theCylinder,float x,float y,float z,float r,float h,AxisMode::__Enum axis,size_t sa,size_t sr,size_t sh);
	size_t newSphere(float x,float y,float z,float r,AxisMode::__Enum axis,size_t sa,size_t sr);
	void newSphere(Object *theSphere,float x,float y,float z,float r,AxisMode::__Enum axis,size_t sa,size_t sr);
	void moveVertex(float x,float y,float z);
	void moveEdge(float x,float y,float z);
	void moveFace(float x,float y,float z);
	size_t newPlane(float x,float y,float z,float length,float width,AxisMode::__Enum axis,size_t sl,size_t sw);
	void newPlane(Object *thePlane,float x,float y,float z,float length,float width,AxisMode::__Enum axis,size_t sl,size_t sw);
	//注意，清除无用的端点但没有边的左右面!!!!!!!!!!!
	void deleteEdgeH(size_t t,size_t edgeID);
	void deleteVertex();
	void deleteFace();
	void extrudeEdge(float x,float y,float z);
	size_t insertVertex(int edgeID,float pos);
	void split(int startV,int endV);
	void removeEdge();
	void clearIsInCache();
	void targetWeldVertex(size_t vertexA,size_t vertexB);
	void extrudeFaceGroup(float x,float y,float z);
	void sceneObjectAdd(char *name);
	void sceneObjectAdd(Object *newObject);
	void detach(Object *newObject);
	void attach(size_t original,size_t toBeAttached);
	void clearSelection();
	void initialize();
	void changeAxisCursorMode(AxisCursorMode::__Enum newMode);
	void clearDualEdge(size_t tt,size_t vertexID);

	void mirrorClone()
	{
		if(mode==SelectionMode::Object)
		{
			for(size_t i=0;i<selection.size();++i)
			{
				Object *theOriginal=theObjectList[selection[i]];
				if(theOriginal)
				{
					if(theOriginal->isMirror && theOriginal->theMirrorMode!=MirrorMode::Nothing)
					{
						Matrix transform;
						glMatrixMode(GL_MODELVIEW);
						glPushMatrix();
						glLoadIdentity();
						if(theOriginal->theMirrorMode==MirrorMode::MirrorXY)
						{
							glScalef(1,1,-1);
						}
						else if(theOriginal->theMirrorMode==MirrorMode::MirrorYZ)
						{
							glScalef(-1,1,1);
						}
						else if(theOriginal->theMirrorMode==MirrorMode::MirrorXZ)
						{
							glScalef(1,-1,1);
						}
						glGetFloatv(GL_MODELVIEW_MATRIX,(GLfloat*)(&transform.m));
						glPopMatrix();
						Object *newObject=new Object("Cloned");
						for(size_t e=1;e<theOriginal->vertexCount();++e)
						{
							Vertex *theVertex=theOriginal->vertex(e);
							if(theVertex)
							{
								Vector theNV=transform*(theVertex->position);
								theVertex->clone=newObject->addVertex(theNV);
							}
						}
						for(size_t e=1;e<theOriginal->edgeCount();++e)
						{
							Edge *theEdge=theOriginal->edge(e);
							if(theEdge)
							{
								theEdge->clone=newObject->addEdge(theOriginal->vertex(theEdge->start)->clone,theOriginal->vertex(theEdge->end)->clone);
							}
						}
						for(size_t e=1;e<theOriginal->faceCount();++e)
						{
							Face *theFace=theOriginal->face(e);
							if(theFace)
							{
								size_t *tempEdgeSet=new size_t[theFace->edge.size()];
								for(size_t h=0;h<theFace->edge.size();++h)
								{
									tempEdgeSet[h]=theFace->edge[h]>0?theFace->edge[h]:-theFace->edge[h];
								}
								newObject->addFace(tempEdgeSet,theFace->edge.size());
								delete tempEdgeSet;
							}
						}
						sceneObjectAdd(newObject);
					}
				}	
			}
		}
	}

	void clone()
	{
		if(mode==SelectionMode::Object)
		{
			for(size_t i=0;i<selection.size();++i)
			{
				Object *theOriginal=theObjectList[selection[i]];
				if(theOriginal)
				{
					Object *newObject=new Object("Cloned");
					for(size_t e=1;e<theOriginal->vertexCount();++e)
					{
						Vertex *theVertex=theOriginal->vertex(e);
						if(theVertex)
						{
							theVertex->clone=newObject->addVertex(theVertex->position);
						}
					}

					for(size_t e=1;e<theOriginal->edgeCount();++e)
					{
						Edge *theEdge=theOriginal->edge(e);
						if(theEdge)
						{
							theEdge->clone=newObject->addEdge(theOriginal->vertex(theEdge->start)->clone,theOriginal->vertex(theEdge->end)->clone);
						}
					}

					for(size_t e=1;e<theOriginal->faceCount();++e)
					{
						Face *theFace=theOriginal->face(e);
						if(theFace)
						{
							size_t *tempEdgeSet=new size_t[theFace->edge.size()];
							for(size_t h=0;h<theFace->edge.size();++h)
							{
								tempEdgeSet[h]=theFace->edge[h]>0?theFace->edge[h]:-theFace->edge[h];
							}
							newObject->addFace(tempEdgeSet,theFace->edge.size());
							delete tempEdgeSet;
						}
					}
					sceneObjectAdd(newObject);
				}
			}
		}
	}

	void getSceneInfo(size_t &objectCount,size_t &vertexCount,size_t &edgeCount,size_t &faceCount)
	{
		objectCount=theObjectList.size();
		for(size_t i=1;i<objectCount;++i)
		{
			if(theObjectList[i])
			{
				vertexCount+=theObjectList[i]->vertexCount()-1;
				edgeCount+=theObjectList[i]->edgeCount()-1;
				faceCount+=theObjectList[i]->faceCount()-1;
			}
		}
		--objectCount;
	}

	void onMerge()
	{
		if(mode==SelectionMode::Object)
		{
			for(size_t i=1;i<selection.size();++i)
			{
				attach(selection[0],selection[i]);
			}
			clearSelection();
		}
	}

	void onDetach()
	{
		Object *newObject=new Object("Detached");
		if(mode==SelectionMode::Face && target>0)
		{
			detach(newObject);
		}
		clearSelection();
	}

	void onMirror(MirrorMode type,float x,float y,float z)
	{
		Vector mirrorPosition(x,y,z);
		if(mode==SelectionMode::Object)
		{
			for(size_t i=0;i<selection.size();++i)
			{
				if(theObjectList[selection[i]])
				{
					theObjectList[selection[i]]->mirror(type,mirrorPosition);
				}
			}
		}
		else if(target)
		{
			if(theObjectList[target])
			{
				theObjectList[target]->mirror(type,mirrorPosition);
			}
		}
	}

	void unMirror()
	{
		if(mode==SelectionMode::Object)
		{
			for(size_t i=0;i<selection.size();++i)
			{
				if(theObjectList[selection[i]])
				{
					theObjectList[selection[i]]->unMirror();
				}
			}
		}
		else if(target)
		{
			if(theObjectList[target])
			{
				theObjectList[target]->unMirror();
			}
		}
	}

	void setSelectionMaterial(float ambient[4],float diffuse[4],float specular[4],float emission[4],float shininess)
	{
		if(mode==SelectionMode::Object)
		{
			if(selection.size()==1)
			{
				if(theObjectList[selection[0]])
				{
					theObjectList[selection[0]]->mat_ambient[0]=ambient[0];
					theObjectList[selection[0]]->mat_ambient[1]=ambient[1];
					theObjectList[selection[0]]->mat_ambient[2]=ambient[2];
					theObjectList[selection[0]]->mat_ambient[3]=ambient[3];
					theObjectList[selection[0]]->mat_diffuse[0]=diffuse[0];
					theObjectList[selection[0]]->mat_diffuse[1]=diffuse[1];
					theObjectList[selection[0]]->mat_diffuse[2]=diffuse[2];
					theObjectList[selection[0]]->mat_diffuse[3]=diffuse[3];
					theObjectList[selection[0]]->mat_specular[0]=specular[0];
					theObjectList[selection[0]]->mat_specular[1]=specular[1];
					theObjectList[selection[0]]->mat_specular[2]=specular[2];
					theObjectList[selection[0]]->mat_specular[3]=specular[3];
					theObjectList[selection[0]]->mat_emission[0]=emission[0];
					theObjectList[selection[0]]->mat_emission[1]=emission[1];
					theObjectList[selection[0]]->mat_emission[2]=emission[2];
					theObjectList[selection[0]]->mat_emission[3]=emission[3];
					theObjectList[selection[0]]->mat_shininess[0]=shininess;
					theObjectList[selection[0]]->mat_shininess[1]=shininess;
					theObjectList[selection[0]]->mat_shininess[2]=shininess;
					theObjectList[selection[0]]->mat_shininess[3]=shininess;
				}
			}
		}
		else if(target>0 && theObjectList[target])
		{
			theObjectList[target]->mat_ambient[0]=ambient[0];
			theObjectList[target]->mat_ambient[1]=ambient[1];
			theObjectList[target]->mat_ambient[2]=ambient[2];
			theObjectList[target]->mat_ambient[3]=ambient[3];
			theObjectList[target]->mat_diffuse[0]=diffuse[0];
			theObjectList[target]->mat_diffuse[1]=diffuse[1];
			theObjectList[target]->mat_diffuse[2]=diffuse[2];
			theObjectList[target]->mat_diffuse[3]=diffuse[3];
			theObjectList[target]->mat_specular[0]=specular[0];
			theObjectList[target]->mat_specular[1]=specular[1];
			theObjectList[target]->mat_specular[2]=specular[2];
			theObjectList[target]->mat_specular[3]=specular[3];
			theObjectList[target]->mat_emission[0]=emission[0];
			theObjectList[target]->mat_emission[1]=emission[1];
			theObjectList[target]->mat_emission[2]=emission[2];
			theObjectList[target]->mat_emission[3]=emission[3];
			theObjectList[target]->mat_shininess[0]=shininess;
			theObjectList[target]->mat_shininess[1]=shininess;
			theObjectList[target]->mat_shininess[2]=shininess;
			theObjectList[target]->mat_shininess[3]=shininess;
		}

	};

	void getSelectionMaterial(float *ambient,float *diffuse,float *specular,float *emission,float *shininess)
	{
		if(mode==SelectionMode::Object)
		{
			if(selection.size()==1)
			{
				if(theObjectList[selection[0]])
				{
					ambient[0]=theObjectList[selection[0]]->mat_ambient[0];
					ambient[1]=theObjectList[selection[0]]->mat_ambient[1];
					ambient[2]=theObjectList[selection[0]]->mat_ambient[2];
					ambient[3]=theObjectList[selection[0]]->mat_ambient[3];
					diffuse[0]=theObjectList[selection[0]]->mat_diffuse[0];
					diffuse[1]=theObjectList[selection[0]]->mat_diffuse[1];
					diffuse[2]=theObjectList[selection[0]]->mat_diffuse[2];
					diffuse[3]=theObjectList[selection[0]]->mat_diffuse[3];
					specular[0]=theObjectList[selection[0]]->mat_specular[0];
					specular[1]=theObjectList[selection[0]]->mat_specular[1];
					specular[2]=theObjectList[selection[0]]->mat_specular[2];
					specular[3]=theObjectList[selection[0]]->mat_specular[3];
					emission[0]=theObjectList[selection[0]]->mat_emission[0];
					emission[1]=theObjectList[selection[0]]->mat_emission[1];
					emission[2]=theObjectList[selection[0]]->mat_emission[2];
					emission[3]=theObjectList[selection[0]]->mat_emission[3];
					(*shininess)=*(theObjectList[selection[0]]->mat_shininess);
				}
			}
		}
		else if(target>0 && theObjectList[target])
		{
			ambient[0]=theObjectList[target]->mat_ambient[0];
			ambient[1]=theObjectList[target]->mat_ambient[1];
			ambient[2]=theObjectList[target]->mat_ambient[2];
			ambient[3]=theObjectList[target]->mat_ambient[3];
			diffuse[0]=theObjectList[target]->mat_diffuse[0];
			diffuse[1]=theObjectList[target]->mat_diffuse[1];
			diffuse[2]=theObjectList[target]->mat_diffuse[2];
			diffuse[3]=theObjectList[target]->mat_diffuse[3];
			specular[0]=theObjectList[target]->mat_specular[0];
			specular[1]=theObjectList[target]->mat_specular[1];
			specular[2]=theObjectList[target]->mat_specular[2];
			specular[3]=theObjectList[target]->mat_specular[3];
			emission[0]=theObjectList[target]->mat_emission[0];
			emission[1]=theObjectList[target]->mat_emission[1];
			emission[2]=theObjectList[target]->mat_emission[2];
			emission[3]=theObjectList[target]->mat_emission[3];
			(*shininess)=*(theObjectList[target]->mat_shininess);
		}
	};

	void updateNormal()
	{
		if(mode==SelectionMode::Object)
		{
			for(size_t i=0;i<selection.size();++i)
			{
				if(theObjectList[selection[i]])
				{
					theObjectList[selection[i]]->updateAllNormal();
				}
			}
		}
		else if(target>0 && theObjectList[target])
		{
			theObjectList[target]->updateAllNormal();
		}
	}

	void onDelete()
	{
		if(mode==SelectionMode::Object && target==0)
		{
			for(size_t i=0;i<selection.size();++i)
			{
				sceneObjectRemove(selection[i]);
			}
			clearSelection();
		}
		else if(target>0)
		{
			if(mode==SelectionMode::Vertex)
			{
				for(size_t i=0;i<selection.size();++i)
				{
					removeVertex(target,selection[i]);
				}
				clearSelection();
			}
			else if(mode==SelectionMode::Edge)
			{
				removeEdge();
				clearSelection();
			}
			else if(mode==SelectionMode::Face)
			{
				deleteFace();
				clearSelection();
			}
		}
		
	}

	void removeVertex(size_t tt,size_t vertexID)
	{
		if(tt && vertexID)
		{
			if(theObjectList[tt]->vertex(vertexID)->adjacentEdge.size()==2)
			{
				Edge *edgeA=theObjectList[tt]->edge(theObjectList[tt]->vertex(vertexID)->adjacentEdge[0]);
				Edge *edgeB=theObjectList[tt]->edge(theObjectList[tt]->vertex(vertexID)->adjacentEdge[1]);
				if(edgeA->start==vertexID)
				{
					if(edgeB->end==vertexID)
					{//printf("@@@@@@@@1@@@@@@@@@@@@@");
						size_t newEdge=theObjectList[tt]->addEdge(edgeB->start,edgeA->end);
						if(edgeA->right>0 && edgeB->right>0 && edgeA->right==edgeB->right)
						{
							Face *theFace=theObjectList[tt]->face(edgeA->right);
							size_t edgeCount=theFace->edge.size();
							for(int e=edgeCount-1;e>-1;--e)
							{
								if(theFace->edge[e]==(int)(edgeB->index))
								{
									theObjectList[tt]->faceEdgeChange(theFace->index,e,newEdge);
								}
								else
									if(theFace->edge[e]==(int)(edgeA->index))
								{
									theObjectList[tt]->faceEdgeRemove(theFace->index,e);
								}
							}
						}
						if(edgeA->left>0 && edgeB->left>0 && edgeA->left==edgeB->left)
						{
							Face *theFace=theObjectList[tt]->face(edgeA->left);
							size_t edgeCount=theFace->edge.size();
							for(int e=edgeCount-1;e>-1;--e)
							{
								if(theFace->edge[e]==-(int)(edgeA->index))
								{
									theObjectList[tt]->faceEdgeChange(theFace->index,e,-(int)(newEdge));
								}
								else
									if(theFace->edge[e]==-((int)edgeB->index))
								{
									theObjectList[tt]->faceEdgeRemove(theFace->index,e);
								}
							}
						}
						deleteEdgeH(tt,edgeA->index);
						deleteEdgeH(tt,edgeB->index);
						return;
					}
					else
					{//printf("@@@@@@@@@@2@@@@@@@@@@@");
						size_t newEdge=theObjectList[tt]->addEdge(edgeB->end,edgeA->end);
						if(edgeA->right>0 && edgeB->left>0 && edgeA->right==edgeB->left)
						{
							Face *theFace=theObjectList[tt]->face(edgeA->right);
							size_t edgeCount=theFace->edge.size();
							for(int e=edgeCount-1;e>-1;--e)
							{
								if(theFace->edge[e]==-((int)edgeB->index))
								{
									theObjectList[tt]->faceEdgeChange(theFace->index,e,newEdge);
								}
								else
									if(theFace->edge[e]==((int)edgeA->index))
								{
									theObjectList[tt]->faceEdgeRemove(theFace->index,e);
								}
							}
						}
						if(edgeA->left>0 && edgeB->right>0 && edgeA->left==edgeB->right)
						{
							Face *theFace=theObjectList[tt]->face(edgeA->left);
							size_t edgeCount=theFace->edge.size();
							for(int e=edgeCount-1;e>-1;--e)
							{
								if(theFace->edge[e]==-((int)edgeA->index))
								{
									theObjectList[tt]->faceEdgeChange(theFace->index,e,-((int)newEdge));
								}
								else
									if(theFace->edge[e]==((int)edgeB->index))
								{
									theObjectList[tt]->faceEdgeRemove(theFace->index,e);
								}
							}
						}
						deleteEdgeH(tt,edgeA->index);
						deleteEdgeH(tt,edgeB->index);
						return;
					}
				}
				else
				{
					if(edgeB->end==vertexID)
					{//printf("@@@@@@@@@3@@@@@@@@@@@@");
						size_t newEdge=theObjectList[tt]->addEdge(edgeB->start,edgeA->start);
						if(edgeA->left>0 && edgeB->right>0 && edgeA->left==edgeB->right)
						{
							Face *theFace=theObjectList[tt]->face(edgeA->left);
							size_t edgeCount=theFace->edge.size();
							for(int e=edgeCount-1;e>-1;--e)
							{
								if(theFace->edge[e]==((int)edgeB->index))
								{
									theObjectList[tt]->faceEdgeChange(theFace->index,e,newEdge);
								}
								else
									if(theFace->edge[e]==-((int)edgeA->index))
								{
									theObjectList[tt]->faceEdgeRemove(theFace->index,e);
								}
							}
						}
						if(edgeA->right>0 && edgeB->left>0 && edgeA->right==edgeB->left)
						{
							Face *theFace=theObjectList[tt]->face(edgeA->right);
							size_t edgeCount=theFace->edge.size();
							for(int e=edgeCount-1;e>-1;--e)
							{
								if(theFace->edge[e]==((int)edgeA->index))
								{
									theObjectList[tt]->faceEdgeChange(theFace->index,e,-((int)newEdge));
								}
								else
									if(theFace->edge[e]==-((int)edgeB->index))
								{
									theObjectList[tt]->faceEdgeRemove(theFace->index,e);
								}
							}
						}
						deleteEdgeH(tt,edgeA->index);
						deleteEdgeH(tt,edgeB->index);
						return;
					}
					else
					{
						//printf("@@@@@@@@@@@4@@@@@@@@@@");
						size_t newEdge=theObjectList[tt]->addEdge(edgeB->end,edgeA->start);
						if(edgeA->left>0 && edgeB->left>0 && edgeA->left==edgeB->left)
						{
							Face *theFace=theObjectList[tt]->face(edgeA->left);
							size_t edgeCount=theFace->edge.size();
							for(int e=edgeCount-1;e>-1;--e)
							{
								if(theFace->edge[e]==-((int)edgeB->index))
								{
									theObjectList[tt]->faceEdgeChange(theFace->index,e,-((int)newEdge));
								}
								else
									if(theFace->edge[e]==-((int)edgeA->index))
								{
									theObjectList[tt]->faceEdgeRemove(theFace->index,e);
								}
							}
						}
						if(edgeA->right>0 && edgeB->right>0 && edgeA->right==edgeB->right)
						{//printf("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
							Face *theFace=theObjectList[tt]->face(edgeA->right);
							int edgeCount=theFace->edge.size();
							//printf("#%d#",edgeCount);
							for(int e=edgeCount-1;e>-1;--e)
							{
								//printf("^^^%%%%%%%%%%%%%%%%%%%%%%%^^^^^^^^^^^^^^^^^^^");
								if(theFace->edge[e]==(int)(edgeA->index))
								{
									//printf("^^^^^^^^^^^^^^^^^^^^^^^^^");
									theObjectList[tt]->faceEdgeChange(theFace->index,e,-(int)newEdge);
								}
								else
									if(theFace->edge[e]==(int)edgeB->index)
								{
									theObjectList[tt]->faceEdgeRemove(theFace->index,e);
								}
							}
						}
						deleteEdgeH(tt,edgeA->index);
						deleteEdgeH(tt,edgeB->index);
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
		HistoryRecord &theRecord=historyManager->redoBegin();
		historyManager->recordBeginR(theRecord.name);
		size_t recordSize=theRecord.size();
		for(int i=recordSize-1;i>-1;--i)
		{
			parseLog(theRecord[i]);
		}
		historyManager->recordEnd();
		historyManager->redoEnd();
	}

	void undo()
	{
		HistoryRecord &theRecord=historyManager->undoBegin();
		historyManager->redoRecordBegin(theRecord.name);
		size_t recordSize=theRecord.size();
		for(int i=recordSize-1;i>-1;--i)
		{
			parseLog(theRecord[i]);
		}
		historyManager->redoRecordEnd();
		historyManager->undoEnd();
	}

	void parseLog(HistoryLog *theLog)
	{

			if(theLog->type==LogType::Vertex_Position_Change)
			{
				Log_VertexPositionChange *original((Log_VertexPositionChange*)theLog);
				theObjectList[original->target]->vertexPositionChangeA(original->index,original->x,original->y,original->z);
			}
			else
			if(theLog->type==LogType::Vertex_Normal_Change)
			{
				Log_VertexNormalChange *original((Log_VertexNormalChange*)theLog);
				theObjectList[original->target]->vertexNormalChange(original->index,original->x,original->y,original->z);
			}
			else
			if(theLog->type==LogType::Vertex_Adjacent_Push)
			{
				Log_VertexAdjacentPush *original((Log_VertexAdjacentPush*)theLog);
				theObjectList[original->target]->vertexAdjacentPop(original->index);
			}
			else
			if(theLog->type==LogType::Vertex_Adjacent_Change)
			{
				Log_VertexAdjacentChange *original((Log_VertexAdjacentChange*)theLog);
				theObjectList[original->target]->vertexAdjacentChange(original->index,original->adjIndex,original->original);
			}
			else
			if(theLog->type==LogType::Vertex_Adjacent_Pop)
			{
				Log_VertexAdjacentPop *original((Log_VertexAdjacentPop*)theLog);
				theObjectList[original->target]->vertexAdjacentPush(original->index,original->o);
			}
			else
			if(theLog->type==LogType::Vertex_Adjacent_Swap)
			{
				Log_VertexAdjacentSwap *original((Log_VertexAdjacentSwap*)theLog);
				theObjectList[original->target]->vertexAdjacentSwap(original->index,original->tIndex,original->oIndex);
			}
			else
			if(theLog->type==LogType::Vertex_Adjacent_Remove)
			{
				Log_VertexAdjacentRemove *original((Log_VertexAdjacentRemove*)theLog);
				theObjectList[original->target]->vertexAdjacentInsert(original->index,original->adjIndex,original->o);
			}
			else
			if(theLog->type==LogType::Vertex_Adjacent_Insert)
			{
				Log_VertexAdjacentInsert *original((Log_VertexAdjacentInsert*)theLog);
				theObjectList[original->target]->vertexAdjacentRemove(original->index,original->adjIndex);
			}
			else
			if(theLog->type==LogType::Edge_Start_Change)
			{
				Log_EdgeStartChange *original((Log_EdgeStartChange*)theLog);
				theObjectList[original->target]->edgeStartChange(original->index,original->start);
			}
			else
			if(theLog->type==LogType::Edge_End_Change)
			{
				Log_EdgeEndChange *original((Log_EdgeEndChange*)theLog);
				theObjectList[original->target]->edgeEndChange(original->index,original->end);
			}
			else
			if(theLog->type==LogType::Edge_Left_Change)
			{
				Log_EdgeLeftChange *original((Log_EdgeLeftChange*)theLog);
				theObjectList[original->target]->edgeLeftChange(original->index,original->left);
			}
			else
			if(theLog->type==LogType::Edge_Right_Change)
			{
				Log_EdgeRightChange *original((Log_EdgeRightChange*)theLog);
				theObjectList[original->target]->edgeRightChange(original->index,original->right);
			}
			else
			if(theLog->type==LogType::Face_Edge_Push)
			{
				Log_FaceEdgePush *original((Log_FaceEdgePush*)theLog);
				theObjectList[original->target]->faceEdgePop(original->index);
			}
			else
			if(theLog->type==LogType::Face_Edge_Change)
			{
				Log_FaceEdgeChange *original((Log_FaceEdgeChange*)theLog);
				theObjectList[original->target]->faceEdgeChange(original->index,original->edgeIndex,original->o);
			}
			else
			if(theLog->type==LogType::Face_Edge_Pop)
			{
				Log_FaceEdgePop *original((Log_FaceEdgePop*)theLog);
				theObjectList[original->target]->faceEdgePush(original->index,original->o);
			}
			else
			if(theLog->type==LogType::Face_Edge_Swap)
			{
				Log_FaceEdgeSwap *original((Log_FaceEdgeSwap*)theLog);
				theObjectList[original->target]->faceEdgeSwap(original->index,original->tIndex,original->oIndex);
			}
			else
			if(theLog->type==LogType::Face_Edge_Remove)
			{
				Log_FaceEdgeRemove *original((Log_FaceEdgeRemove*)theLog);
				theObjectList[original->target]->faceEdgeInsert(original->index,original->edgeIndex,original->o);
			}
			else
			if(theLog->type==LogType::Face_Edge_Insert)
			{
				Log_FaceEdgeInsert *original((Log_FaceEdgeInsert*)theLog);
				theObjectList[original->target]->faceEdgeRemove(original->index,original->edgeIndex);
			}
			else
			if(theLog->type==LogType::Object_Position_Change)
			{
				Log_ObjectPositionChange *original((Log_ObjectPositionChange*)theLog);
				original;
			}
			else
			if(theLog->type==LogType::Object_Rotation_Change)
			{
				Log_ObjectRotationChange *original((Log_ObjectRotationChange*)theLog);
				original;
			}
			else
			if(theLog->type==LogType::Object_Scale_Change)
			{
				Log_ObjectScaleChange *original((Log_ObjectScaleChange*)theLog);
				original;
			}
			else
			if(theLog->type==LogType::Object_Color_Change)
			{
				Log_ObjectColorChange *original((Log_ObjectColorChange*)theLog);
				original;
			}
			else
			if(theLog->type==LogType::Object_Vertex_Add)
			{
				Log_ObjectVertexAdd *original((Log_ObjectVertexAdd*)theLog);
				theObjectList[original->target]->objectVertexRemove(original->index);
			}
			else
			if(theLog->type==LogType::Object_Vertex_Remove)
			{
				Log_ObjectVertexRemove *original((Log_ObjectVertexRemove*)theLog);
				theObjectList[original->target]->addVertex(original->index,original->o);
			}
			else
			if(theLog->type==LogType::Object_Edge_Add)
			{
				Log_ObjectEdgeAdd *original((Log_ObjectEdgeAdd*)theLog);
				theObjectList[original->target]->objectEdgeRemove(original->index);
			}
			else
			if(theLog->type==LogType::Object_Edge_Remove)
			{
				Log_ObjectEdgeRemove *original((Log_ObjectEdgeRemove*)theLog);
				theObjectList[original->target]->addEdge(original->index,original->o);
			}
			else
			if(theLog->type==LogType::Object_Face_Add)
			{
				Log_ObjectFaceAdd *original((Log_ObjectFaceAdd*)theLog);
				theObjectList[original->target]->objectFaceRemove(original->index);
			}
			else
			if(theLog->type==LogType::Object_Face_Remove)
			{
				Log_ObjectFaceRemove *original((Log_ObjectFaceRemove*)theLog);
				theObjectList[original->target]->addFace(original->index,original->o);
			}
			else
			if(theLog->type==LogType::Scene_Object_Add)
			{
				Log_SceneObjectAdd *original((Log_SceneObjectAdd*)theLog);
				sceneObjectRemove(original->target);
			}
			else
			if(theLog->type==LogType::Scene_Object_Remove)
			{
				Log_SceneObjectRemove *original((Log_SceneObjectRemove*)theLog);
				sceneObjectAdd(original->target,original->o);
			}
			else
			if(theLog->type==LogType::Scene_Selection_Add)
			{

			}
			else
			if(theLog->type==LogType::Scene_Selection_Remove)
			{
			}
	}

	void sceneObjectRemove(size_t objectID)
	{
		if(!historyManager->record(new Log_SceneObjectRemove(objectID,theObjectList[objectID])))
		{
			delete theObjectList[objectID];			
		}
		theObjectList.remove(objectID);
	};

	void clearScene()
	{
		theObjectList.clear();
	};

	size_t sceneObjectAdd(size_t objectID,Object *theO)
	{
		theObjectList.addI(objectID,theO);
		historyManager->record(new Log_SceneObjectAdd(objectID));
		return objectID;
	};


	void selectDualSideObject(bool isAppend=false)
	{
		if(!isAppend)
		{
			clearSelection();
		}
		for(size_t i=1;i<theObjectList.size();++i)
		{
			struct SelectionResult selectBuffer[512];
			glSelectBuffer(2048, (GLuint*)selectBuffer);
			glRenderMode(GL_SELECT);
			glInitNames();
			glPushName(0);
			size_t objectCount=0;
			while(i<theObjectList.size() && objectCount<512)
			{
				if(theObjectList[i])
				{
					Object *theObject=theObjectList[i];
					glLoadName(i);
					theObject->selectionRenderObject();
				}
				++i;
				++objectCount;
			}
			--i;
			glFlush();
			GLuint hits = glRenderMode(GL_RENDER);

			for (size_t e=0; e<hits; e++) 
			{
				struct SelectionResult result=selectBuffer[e];
				if(result.size==1)
				{
					selectionPush(theObjectList[result.id]);
				}
			}
		}
		updateAxisCursor();
	};

	void selectDualSideVertex(bool isAppend=false)
	{
		if(!isAppend)
		{
			clearSelection();
		}
		Object *theObject=theObjectList[target];
				glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glTranslatef(theObject->position.x,theObject->position.y,theObject->position.z);
		glTranslatef(theObject->center.x,theObject->center.y,theObject->center.z);
		glRotatef(theObject->rotation.w,theObject->rotation.x,theObject->rotation.y,theObject->rotation.z);
glScalef(theObject->scale.x,theObject->scale.y,theObject->scale.z);	
		glTranslatef(-theObject->center.x,-theObject->center.y,-theObject->center.z);

		for(size_t i=1;i<theObject->vertexCount();++i)
		{
			struct SelectionResult selectBuffer[512];
			glSelectBuffer(2048, (GLuint*)selectBuffer);
			glRenderMode(GL_SELECT);
			glInitNames();
			glPushName(0);
			size_t objectCount=0;
			while(i<theObject->vertexCount() && objectCount<512)
			{
				Vertex *theVertex=theObject->vertex(i);
				if(theVertex)
				{
					glLoadName(i);
					glBegin(GL_POINTS);
					glVertex3f(theVertex->position.x,theVertex->position.y,theVertex->position.z);
					glEnd();
				}
				++i;
				++objectCount;
			}
			--i;
			glFlush();

			GLuint hits = glRenderMode(GL_RENDER);

			for (size_t e=0; e<hits; ++e) 
			{
				struct SelectionResult result=selectBuffer[e];
				if(result.size==1)
				{
					selectionPush(theObject->vertex(result.id));
				}
			}
		}
					glPopMatrix();
		updateAxisCursor();
			if(!selection.empty())
			{
				theObject->buildPSCacheFromVID(selection);
			}
	};

	void selectDualSideEdge(bool isAppend=false)
	{
		if(!isAppend)
		{
			clearSelection();
		}
		Object *theObject=theObjectList[target];
	glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glTranslatef(theObject->position.x,theObject->position.y,theObject->position.z);
				glTranslatef(theObject->center.x,theObject->center.y,theObject->center.z);
		glRotatef(theObject->rotation.w,theObject->rotation.x,theObject->rotation.y,theObject->rotation.z);
glScalef(theObject->scale.x,theObject->scale.y,theObject->scale.z);	
		glTranslatef(-theObject->center.x,-theObject->center.y,-theObject->center.z);

		for(size_t i=1;i<theObject->edgeCount();++i)
		{
			struct SelectionResult selectBuffer[512];
			glSelectBuffer(2048, (GLuint*)selectBuffer);
			glRenderMode(GL_SELECT);
			glInitNames();
			glPushName(0);
			size_t objectCount=0;
			while(i<theObject->edgeCount() && objectCount<512)
			{
				Edge *theEdge=theObject->edge(i);
				if(theEdge)
				{
					glLoadName(i);
					glBegin(GL_LINES);
					Vertex *start=theObject->vertex(theEdge->start);
					Vertex *end=theObject->vertex(theEdge->end);
					glVertex3f(start->position.x,start->position.y,start->position.z);
					glVertex3f(end->position.x,end->position.y,end->position.z);
					glEnd();
				}
				++i;
				++objectCount;
			}
			--i;
			glFlush();
	
			GLuint hits = glRenderMode(GL_RENDER);

			for (size_t e=0; e<hits; ++e) 
			{
				struct SelectionResult result=selectBuffer[e];
				if(result.size==1)
				{
					selectionPush(theObject->edge(result.id));
				}
				else
				{
					return;
				}
			}
		}
				glPopMatrix();
		updateAxisCursor();
					if(!selection.empty())
			{
				theObject->buildPSCacheFromEID(selection);
			}
	};

	void redefineControlPoint()
	{
		if(mode==SelectionMode::Object)
		{
			for(size_t i=0;i<selection.size();++i)
			{
				if(theObjectList[selection[i]])
				{
					theObjectList[selection[i]]->redefineControlPoint();
				}
			}
		}
		else if(target>0)
		{
			clearSelection();
			theObjectList[target]->clearPSCache();
			theObjectList[target]->redefineControlPoint();
		}
	}

	void selectDualSideFace(bool isAppend=false)
	{
		if(!isAppend)
		{
			clearSelection();
		}
		Object *theObject=theObjectList[target];
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glTranslatef(theObject->position.x,theObject->position.y,theObject->position.z);
				glTranslatef(theObject->center.x,theObject->center.y,theObject->center.z);
		glRotatef(theObject->rotation.w,theObject->rotation.x,theObject->rotation.y,theObject->rotation.z);
glScalef(theObject->scale.x,theObject->scale.y,theObject->scale.z);	
		glTranslatef(-theObject->center.x,-theObject->center.y,-theObject->center.z);

		for(size_t i=1;i<theObject->faceCount();++i)
		{
			struct SelectionResult selectBuffer[512];
			glSelectBuffer(2048, (GLuint*)selectBuffer);
			glRenderMode(GL_SELECT);
			glInitNames();
			glPushName(0);
			size_t objectCount=0;
			while(i<theObject->faceCount() && objectCount<512)
			{
				Face *theFace=theObject->face(i);
				if(theFace)
				{
					glLoadName(i);
					glBegin(GL_POLYGON);
					for(size_t e=0;e<theFace->edge.size();++e)
					{
						if(theFace->edge[e]>0)
						{
							Vertex *v=theObject->vertex(theObject->edge(theFace->edge[e])->start);
							glVertex3f(v->position.x,v->position.y,v->position.z);
						}
						else
						{
							Vertex *v=theObject->vertex(theObject->edge(-theFace->edge[e])->end);
							glVertex3f(v->position.x,v->position.y,v->position.z);
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

			for (size_t e=0; e<hits; ++e) 
			{
				struct SelectionResult result=selectBuffer[e];
				if(result.size==1)
				{
					selectionPush(theObject->face(result.id));
				}
			}
		}
		glPopMatrix();
		updateAxisCursor();
								if(!selection.empty())
			{
				theObject->buildPSCacheFromFID(selection);
			}
	};

	void selectionPush(ElementBase *theElement)
	{
		if(!theElement->isSelected)
		{
			theElement->isSelected=true;
			selection.push_back(theElement->index);
		}
	}

	void selectDualSide(bool isAppend=false)
	{
		if(mode==SelectionMode::Object && target==0)
		{
			selectDualSideObject(isAppend);
		}
		else if(mode==SelectionMode::Face && target)
		{
			selectDualSideFace(isAppend);
		}
		else if(mode==SelectionMode::Edge && target)
		{
			selectDualSideEdge(isAppend);
		}
		else if(mode==SelectionMode::Vertex && target)
		{
			selectDualSideVertex(isAppend);
		}
	};

	void changeShadeMode(RenderType::__Enum type)
	{
		if(mode==SelectionMode::Object && target==0)
		{
			size_t selectionSize=selection.size();
			for(size_t i=0;i<selectionSize;++i)
			{
				if(theObjectList[selection[i]])
				{
					theObjectList[selection[i]]->renderMode=type;
				}
			}
		}
		else
		if(target)
		{
			if(theObjectList[target])
			{
				theObjectList[target]->renderMode=type;
			}
		}
	}

	void splitPress(size_t x1,size_t y1,size_t height)
	{
		Object *theObject=theObjectList[target];
		if(mode==SelectionMode::Split && theObject->vertexCount()<16777215 && theObject->edgeCount()<16777215)
		{
			size_t newSplitVX;
			size_t newSplitVY;
			size_t cutVertex=0;
			glClearColor(1.0f,1.0f,1.0f,1.0f);
			glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);
			glDisable(GL_DITHER);
			glDisable(GL_LIGHTING);
			glDisable(GL_TEXTURE_2D);
			glDisable(GL_ALPHA_TEST);
			glDisable(GL_BLEND);
			glMatrixMode(GL_MODELVIEW);
			glPushMatrix();
			glTranslatef(theObject->position.x,theObject->position.y,theObject->position.z);
			glTranslatef(theObject->center.x,theObject->center.y,theObject->center.z);
			glRotatef(theObject->rotation.w,theObject->rotation.x,theObject->rotation.y,theObject->rotation.z);
			glScalef(theObject->scale.x,theObject->scale.y,theObject->scale.z);	
			glTranslatef(-theObject->center.x,-theObject->center.y,-theObject->center.z);
			glColorMask(GL_FALSE,GL_FALSE,GL_FALSE,GL_FALSE);
			glEnable(GL_POLYGON_OFFSET_FILL);
			glPolygonOffset(1.0f,1.0f);
			for(size_t i=1;i<theObject->faceCount();++i)
			{
				Face *theFace=theObject->face(i);
				if(theFace)
				{
					struct ColorID colorID=(*(struct ColorID*)&(i));
					glColor4ub(colorID.r, colorID.g,colorID.b,colorID.a);
					glBegin(GL_POLYGON);
					for(size_t e=0;e<theFace->edge.size();++e)
					{
						if(theFace->edge[e]>0)
						{
							Vertex *v=theObject->vertex(theObject->edge(theFace->edge[e])->start);
							glVertex3f(v->position.x,v->position.y,v->position.z);
						}
						else
						{
							Vertex *v=theObject->vertex(theObject->edge(-theFace->edge[e])->end);
							glVertex3f(v->position.x,v->position.y,v->position.z);
						}
					}
					glEnd();
				}
			}
			glDisable(GL_POLYGON_OFFSET_FILL);
			glColorMask(GL_TRUE,GL_TRUE,GL_TRUE,GL_TRUE);
			glPointSize(5.0f);
			glBegin(GL_POINTS);
			for(size_t i=0;i<theObject->vertexCount();++i)
			{
				Vertex *theVertex=theObject->vertex(i);
				if(theVertex)
				{
					struct ColorID colorID=(*(struct ColorID*)&(i));
					glColor4ub(colorID.r, colorID.g,colorID.b,colorID.a);
					glVertex3f(theVertex->position.x,theVertex->position.y,theVertex->position.z);
				}
			}
			glEnd();
			glFlush();
			glPopMatrix();
			size_t sw=5;
			size_t sh=5;
			size_t length=sw*sh;
			struct ColorID *pixel=new struct ColorID[length];
			glReadPixels(x1,height-y1,sw,sh, GL_RGBA, GL_UNSIGNED_BYTE, pixel);
			for(size_t e=0;e<length;++e)
			{
				pixel[e].a=0;
				size_t result=(*(size_t*)&(pixel[e]));
				if(result<16777215)
				{
					if(result<theObject->vertexCount()&&theObject->vertex(result))
					{
						//selectionPush(theObject->vertex(result));
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
//				splitVX=newSplitVX;
	//			splitVY=newSplitVY;
			}
			else if(splitVertexID>0 && cutVertex>0)
			{
				split(splitVertexID,cutVertex);
				splitVertexID=cutVertex;
//				splitVX=newSplitVX;
			//	splitVY=newSplitVY;
			}
			else if(cutVertex==0)
			{
				size_t edgeBeCuted=0;
				glClearColor(1.0f,1.0f,1.0f,1.0f);
				glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);
				glDisable(GL_DITHER);
				glDisable(GL_LIGHTING);
				glDisable(GL_TEXTURE_2D);
				glDisable(GL_ALPHA_TEST);
				glDisable(GL_BLEND);
				glMatrixMode(GL_MODELVIEW);
				glPushMatrix();
				glTranslatef(theObject->position.x,theObject->position.y,theObject->position.z);
				glTranslatef(theObject->center.x,theObject->center.y,theObject->center.z);
				glRotatef(theObject->rotation.w,theObject->rotation.x,theObject->rotation.y,theObject->rotation.z);
				glScalef(theObject->scale.x,theObject->scale.y,theObject->scale.z);	
				glTranslatef(-theObject->center.x,-theObject->center.y,-theObject->center.z);
				glColorMask(GL_FALSE,GL_FALSE,GL_FALSE,GL_FALSE);
				glEnable(GL_POLYGON_OFFSET_FILL);
				glPolygonOffset(1.0f,1.0f);
				for(size_t i=1;i<theObject->faceCount();++i)
				{
					Face *theFace=theObject->face(i);
					if(theFace)
					{
						struct ColorID colorID=(*(struct ColorID*)&(i));
						glColor4ub(colorID.r, colorID.g,colorID.b,colorID.a);
						glBegin(GL_POLYGON);
						for(size_t e=0;e<theFace->edge.size();++e)
						{
							if(theFace->edge[e]>0)
							{
								Vertex *v=theObject->vertex(theObject->edge(theFace->edge[e])->start);
								glVertex3f(v->position.x,v->position.y,v->position.z);
							}
							else
							{
								Vertex *v=theObject->vertex(theObject->edge(-theFace->edge[e])->end);
								glVertex3f(v->position.x,v->position.y,v->position.z);
							}
						}
						glEnd();
					}
				}
				glDisable(GL_POLYGON_OFFSET_FILL);
				glColorMask(GL_TRUE,GL_TRUE,GL_TRUE,GL_TRUE);
				glBegin(GL_LINES);
				for(size_t i=1;i<theObject->edgeCount();++i)
				{
					Edge *theEdge=theObject->edge(i);
					if(theEdge)
					{
						struct ColorID colorID=(*(struct ColorID*)&(i));
						glColor4ub(colorID.r, colorID.g,colorID.b,colorID.a);
						Vertex *start=theObject->vertex(theEdge->start);
						Vertex *end=theObject->vertex(theEdge->end);
						glVertex3f(start->position.x,start->position.y,start->position.z);
						glVertex3f(end->position.x,end->position.y,end->position.z);
					}
				}
				glEnd();
				glFlush();
				glPopMatrix();
				size_t sw=5;
				size_t sh=5;
				size_t length=sw*sh;
				struct ColorID *pixel=new struct ColorID[length];
				glReadPixels(x1,height-y1,sw,sh, GL_RGBA, GL_UNSIGNED_BYTE, pixel);
				for(size_t e=0;e<length;++e)
				{
					pixel[e].a=0;
					size_t result=(*(size_t*)&(pixel[e]));
					if(result<16777215)
					{
						if(result<theObject->edgeCount()&&theObject->edge(result))
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
			}
		}
	};

	void weldVertex()
	{
		if(mode==SelectionMode::Vertex && target && selection.size()>1)
		{
			size_t targetV=selection[0];
			for(size_t i=selection.size()-1;i>0;--i)
			{
				targetWeldVertex(selection[i],targetV);
			}
			clearSelection();
			theObjectList[target]->clearPSCache();
		}
	};

	void beginSplit()
	{
		isSplitMode=true;
		changeSelectionMode(SelectionMode::Split);
	};

	void nextSplit()
	{
		splitVertexID=0;
	};

	void endSplit()
	{
		isSplitMode=false;
		splitVertexID=0;
	};

	void selectSingleSide(size_t x1,size_t y1,size_t x2,size_t y2,size_t height,bool isAppend=false)
	{
		if(mode==SelectionMode::Object && target==0)
		{
			selectSingleSideObject(x1,y1,x2,y2,height,isAppend);
		}
		else if(mode==SelectionMode::Face && target)
		{
			selectSingleSideFace(x1,y1,x2,y2,height,isAppend);
		}
		else if(mode==SelectionMode::Edge && target)
		{
			selectSingleSideEdge(x1,y1,x2,y2,height,isAppend);
		}
		else if(mode==SelectionMode::Vertex && target)
		{
			selectSingleSideVertex(x1,y1,x2,y2,height,isAppend);
		}
		else if(mode==SelectionMode::Split && target)
		{
			splitPress(x1,y1,height);
		}
	};

	void selectSingleSideVertex(size_t x1,size_t y1,size_t x2,size_t y2,size_t height,bool isAppend=false)
	{
		if(!isAppend)
		{
			clearSelection();
		}
		Object *theObject=theObjectList[target];

		if(theObject->vertexCount()<16777215)
		{
			glClearColor(1.0f,1.0f,1.0f,1.0f);
			glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);
			glDisable(GL_DITHER);
			glDisable(GL_LIGHTING);
			glDisable(GL_TEXTURE_2D);
			glDisable(GL_ALPHA_TEST);
			glDisable(GL_BLEND);
						glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glTranslatef(theObject->position.x,theObject->position.y,theObject->position.z);
				glTranslatef(theObject->center.x,theObject->center.y,theObject->center.z);
		glRotatef(theObject->rotation.w,theObject->rotation.x,theObject->rotation.y,theObject->rotation.z);
glScalef(theObject->scale.x,theObject->scale.y,theObject->scale.z);	
		glTranslatef(-theObject->center.x,-theObject->center.y,-theObject->center.z);

			glColorMask(GL_FALSE,GL_FALSE,GL_FALSE,GL_FALSE);
			glEnable(GL_POLYGON_OFFSET_FILL);
			glPolygonOffset(1.0f,1.0f);
			for(size_t i=1;i<theObject->faceCount();++i)
			{
				Face *theFace=theObject->face(i);
				if(theFace)
				{
					struct ColorID colorID=(*(struct ColorID*)&(i));
					glColor4ub(colorID.r, colorID.g,colorID.b,colorID.a);
					glBegin(GL_POLYGON);
					for(size_t e=0;e<theFace->edge.size();++e)
					{
						if(theFace->edge[e]>0)
						{
							Vertex *v=theObject->vertex(theObject->edge(theFace->edge[e])->start);
							glVertex3f(v->position.x,v->position.y,v->position.z);
						}
						else
						{
							Vertex *v=theObject->vertex(theObject->edge(-theFace->edge[e])->end);
							glVertex3f(v->position.x,v->position.y,v->position.z);
						}
					}
					glEnd();
				}
			}
			glDisable(GL_POLYGON_OFFSET_FILL);
			glColorMask(GL_TRUE,GL_TRUE,GL_TRUE,GL_TRUE);
			glPointSize(5.0f);
			glBegin(GL_POINTS);
			for(size_t i=0;i<theObject->vertexCount();++i)
			{
				Vertex *theVertex=theObject->vertex(i);
				if(theVertex)
				{
					struct ColorID colorID=(*(struct ColorID*)&(i));
					glColor4ub(colorID.r, colorID.g,colorID.b,colorID.a);
					glVertex3f(theVertex->position.x,theVertex->position.y,theVertex->position.z);
				}
			}
			glEnd();
			glFlush();
			glPopMatrix();
			size_t sw=(x2-x1)>5?(x2-x1):5;
			size_t sh=(y2-y1)>5?(y2-y1):5;
			size_t length=sw*sh;
			struct ColorID *pixel=new struct ColorID[length];
			glReadPixels(x1,height-y2,sw,sh, GL_RGBA, GL_UNSIGNED_BYTE, pixel);
			for(size_t e=0;e<length;++e)
			{
				pixel[e].a=0;
				size_t result=(*(size_t*)&(pixel[e]));
				if(result<16777215)
				{
					if(result<theObject->vertexCount()&&theObject->vertex(result))
					{
						selectionPush(theObject->vertex(result));
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
			theObjectList[target]->clearPSCache();
			if(!selection.empty())
			{
				theObject->buildPSCacheFromVID(selection);
			}
		}
	};

	void subdivide()
	{
		if(mode==SelectionMode::Object && target==0)
		{
			if(!selection.empty())
			{
				for(size_t i=0;i<selection.size();++i)
				{
					theObjectList[selection[i]]->subdivide();
				}
			}
		}
		else
		{
			if(mode==SelectionMode::Vertex && target)
			{
				theObjectList[target]->subdivide();
				if(!selection.empty())
				{
					theObjectList[target]->clearPSCache();
					theObjectList[target]->buildPSCacheFromVID(selection);
				}
			}
		}
	};

	bool isAxisSelected(CameraMode::__Enum cameraMode,const Vector &eye,const size_t height,const size_t x,const size_t y,float &rx,float &ry,int &cursorMode)
	{
		if(selection.empty())
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
			glDisable(GL_LIGHTING);
			glDisable(GL_TEXTURE_2D);
			glDisable(GL_ALPHA_TEST);
			glDisable(GL_BLEND);
			theAxisCursor->onPaint(eye,cameraMode);
			glFlush();
			struct ColorID pixel;
			glReadPixels(x,height-y,1,1, GL_RGBA, GL_UNSIGNED_BYTE,&pixel);
			if(pixel.r==255 && pixel.g==0 && pixel.b==0)
			{
				result=true;
				cursorMode=1;//x轴
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
				if(theAxisCursor->mode==AxisCursorMode::MoveAxis)
				{
					gluProject(0,0,0,modelMatrix,projMatrix,viewport,&ox,&oy,&oz);
					oy=viewport[3]-(GLint)oy;
					gluProject(10,0,0,modelMatrix,projMatrix,viewport,&winx,&winy,&winz);
					winy=viewport[3]-(GLint)winy;
					rx=(float)(winx-ox);
					ry=(float)(winy-oy);
				}
				else if(theAxisCursor->mode==AxisCursorMode::RotateAxis)
				{
					gluProject(0,0,0,modelMatrix,projMatrix,viewport,&ox,&oy,&oz);
					oy=viewport[3]-(GLint)oy;
					gluProject(0,10,0,modelMatrix,projMatrix,viewport,&winx,&winy,&winz);
					winy=viewport[3]-(GLint)winy;
					rx=(float)(winx-ox);
					ry=(float)(winy-oy);
				}
				else if(theAxisCursor->mode==AxisCursorMode::ScaleAxis)
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
				if(theAxisCursor->mode==AxisCursorMode::MoveAxis)
				{
					gluProject(0,0,0,modelMatrix,projMatrix,viewport,&ox,&oy,&oz);
					oy=viewport[3]-(GLint)oy;
					gluProject(0,10,0,modelMatrix,projMatrix,viewport,&winx,&winy,&winz);
					winy=viewport[3]-(GLint)winy;
					rx=(float)(winx-ox);
					ry=(float)(winy-oy);
				}
				else if(theAxisCursor->mode==AxisCursorMode::RotateAxis)
				{
					gluProject(0,0,0,modelMatrix,projMatrix,viewport,&ox,&oy,&oz);
					oy=viewport[3]-(GLint)oy;
					gluProject(0,0,10,modelMatrix,projMatrix,viewport,&winx,&winy,&winz);
					winy=viewport[3]-(GLint)winy;
					rx=(float)(winx-ox);
					ry=(float)(winy-oy);
				}
				else if(theAxisCursor->mode==AxisCursorMode::ScaleAxis)
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
				if(theAxisCursor->mode==AxisCursorMode::MoveAxis)
				{
					gluProject(0,0,0,modelMatrix,projMatrix,viewport,&ox,&oy,&oz);
					oy=viewport[3]-(GLint)oy;
					gluProject(0,0,10,modelMatrix,projMatrix,viewport,&winx,&winy,&winz);
					winy=viewport[3]-(GLint)winy;
					rx=(float)(winx-ox);
					ry=(float)(winy-oy);
				}
				else if(theAxisCursor->mode==AxisCursorMode::RotateAxis)
				{
					gluProject(0,0,0,modelMatrix,projMatrix,viewport,&ox,&oy,&oz);
					oy=viewport[3]-(GLint)oy;
					gluProject(10,0,0,modelMatrix,projMatrix,viewport,&winx,&winy,&winz);
					winy=viewport[3]-(GLint)winy;
					rx=(float)(winx-ox);
					ry=(float)(winy-oy);
				}
				else if(theAxisCursor->mode==AxisCursorMode::ScaleAxis)
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
			glClearColor(128.0f/255.0f,128.0f/255.0f,128.0f/255.0f,1.0f);
			return result;
		}
	};

	void axisXMove(float step)
	{
		theAxisCursor->position.x+=step;
		if(mode==SelectionMode::Object && target==0)
		{
			for(size_t i=0;i<selection.size();++i)
			{
				Object *theObject=theObjectList[selection[i]];
				if(theObject)
				{
					theObject->position.x+=step;
				}
			}
		}
		else if(mode==SelectionMode::Vertex && target)
		{
			Object *theObject=theObjectList[target];
			Quaternion angle(-theObject->rotation.w,Vector(theObject->rotation));
			Matrix rotateMatrix;
			angle.getMatrix(rotateMatrix);
			Vector xAxis(1,0,0);
			xAxis=(xAxis)*(rotateMatrix)*step;
			moveVertex(xAxis.x,xAxis.y,xAxis.z);
			theObject->partialSubdivision();
		}
		else if(mode==SelectionMode::Edge && target)
		{
			Object *theObject=theObjectList[target];
			Quaternion angle(-theObject->rotation.w,Vector(theObject->rotation));
			Matrix rotateMatrix;
			angle.getMatrix(rotateMatrix);
			Vector xAxis(1,0,0);
			xAxis=(xAxis)*(rotateMatrix)*step;
			moveEdge(xAxis.x,xAxis.y,xAxis.z);
			theObject->partialSubdivision();
		}
		else if(mode==SelectionMode::Face && target)
		{
			Object *theObject=theObjectList[target];
			Quaternion angle(-theObject->rotation.w,Vector(theObject->rotation));
			Matrix rotateMatrix;
			angle.getMatrix(rotateMatrix);
			Vector xAxis(1,0,0);
			xAxis=(xAxis)*(rotateMatrix)*step;
			moveFace(xAxis.x,xAxis.y,xAxis.z);
			theObject->partialSubdivision();			
		}
	};

	void axisXRotate(float step)
	{
		theAxisCursor->rotation*=theAxisCursor->rotation.w;
		theAxisCursor->rotation.z+=step;
		float angle=theAxisCursor->rotation.length();
		theAxisCursor->rotation.normalize();
		theAxisCursor->rotation.w=angle;

		Vector rotateAxis=Vector(0,0,1);
		Quaternion rotateQuaternion(step,rotateAxis);
		Matrix rotateMatrix;
		rotateQuaternion.getMatrix(rotateMatrix);

		if(mode==SelectionMode::Object && target==0)
		{
			for(size_t i=0;i<selection.size();++i)
			{
				Object *theObject=theObjectList[selection[i]];
				if(theObject)
				{
					//旋转分成两部，第一步是更新位置，第二步是更新角度
					//首先要更新位置
					//对于每个物体首先求出中心点到旋转轴的一个向量
					//new position;
					Vector CToR(theObject->center+theObject->position-theAxisCursor->position);
					//将这个向量沿着旋转轴进行旋转
					CToR=(rotateMatrix)*(CToR);
					//旋转之后还原新的中心点
					theObject->position=theAxisCursor->position+CToR-theObject->center;
					//之后要旋转物体的角度
					Quaternion original(theObject->rotation.w,Vector(theObject->rotation));
					Quaternion add(step,Vector(0,0,1));
					Quaternion result=add*original;
					result.getRotate(theObject->rotation.w,theObject->rotation);
					/*theObject->rotation*=theObject->rotation.w;
					theObject->rotation.z+=step;
					float angle=theObject->rotation.length();
					theObject->rotation.normalize();
					theObject->rotation.w=angle;*/
				}
			}
		}
		else if(mode==SelectionMode::Vertex)
		{
			Object *theObject=theObjectList[target];
			Quaternion original(-theObject->rotation.w,Vector(theObject->rotation));
			Matrix originalMatrix;
			Matrix rotateMatrix;
			rotateQuaternion.getMatrix(rotateMatrix);
			original.getMatrix(originalMatrix);
			rotateVertex(originalMatrix*(theAxisCursor->position-theObject->center)+theObject->center,rotateMatrix);
			theObject->partialSubdivision();
		}
		else if(mode==SelectionMode::Edge)
		{
			Object *theObject=theObjectList[target];
			Quaternion original(-theObject->rotation.w,Vector(theObject->rotation));
			Matrix originalMatrix;
			Matrix rotateMatrix;
			rotateQuaternion.getMatrix(rotateMatrix);
			original.getMatrix(originalMatrix);
			rotateEdge(originalMatrix*(theAxisCursor->position-theObject->center)+theObject->center,rotateMatrix);
			theObject->partialSubdivision();
		}
		else if(mode==SelectionMode::Face)
		{
			Object *theObject=theObjectList[target];
			Quaternion original(-theObject->rotation.w,Vector(theObject->rotation));
			Matrix originalMatrix;
			Matrix rotateMatrix;
			rotateQuaternion.getMatrix(rotateMatrix);
			original.getMatrix(originalMatrix);
			rotateFace(originalMatrix*(theAxisCursor->position-theObject->center)+theObject->center,rotateMatrix);
			theObject->partialSubdivision();			
		}
	};

	void axisYRotate(float step)
	{
		theAxisCursor->rotation*=theAxisCursor->rotation.w;
		theAxisCursor->rotation.x+=step;
		float angle=theAxisCursor->rotation.length();
		theAxisCursor->rotation.normalize();
		theAxisCursor->rotation.w=angle;

		Vector rotateAxis=Vector(1,0,0);
		Quaternion rotateQuaternion(step,rotateAxis);
		Matrix rotateMatrix;
		rotateQuaternion.getMatrix(rotateMatrix);

		if(mode==SelectionMode::Object && target==0)
		{
			for(size_t i=0;i<selection.size();++i)
			{
				Object *theObject=theObjectList[selection[i]];
				if(theObject)
				{
					//旋转分成两部，第一步是更新位置，第二步是更新角度
					//首先要更新位置
					//对于每个物体首先求出中心点到旋转轴的一个向量
					//new position;
					Vector CToR(theObject->center+theObject->position-theAxisCursor->position);
					//将这个向量沿着旋转轴进行旋转
					CToR=(rotateMatrix)*(CToR);
					//旋转之后还原新的中心点
					theObject->position=theAxisCursor->position+CToR-theObject->center;
					//之后要旋转物体的角度
/*					theObject->rotation*=theObject->rotation.w;
					theObject->rotation.x+=step;
					float angle=theObject->rotation.length();
					theObject->rotation.normalize();
					theObject->rotation.w=angle;*/

					Quaternion original(theObject->rotation.w,Vector(theObject->rotation));
					Quaternion add(step,Vector(1,0,0));
					Quaternion result=add*original;
					result.getRotate(theObject->rotation.w,theObject->rotation);

				}
			}
		}
		else if(mode==SelectionMode::Vertex)
		{
			Object *theObject=theObjectList[target];
			Quaternion original(-theObject->rotation.w,Vector(theObject->rotation));
			Matrix originalMatrix;
			Matrix rotateMatrix;
			rotateQuaternion.getMatrix(rotateMatrix);
			original.getMatrix(originalMatrix);
			rotateVertex(originalMatrix*(theAxisCursor->position-theObject->center)+theObject->center,rotateMatrix);
			theObject->partialSubdivision();
		}
		else if(mode==SelectionMode::Edge)
		{
			Object *theObject=theObjectList[target];
			Quaternion original(-theObject->rotation.w,Vector(theObject->rotation));
			Matrix originalMatrix;
			Matrix rotateMatrix;
			rotateQuaternion.getMatrix(rotateMatrix);
			original.getMatrix(originalMatrix);
			rotateEdge(originalMatrix*(theAxisCursor->position-theObject->center)+theObject->center,rotateMatrix);
			theObject->partialSubdivision();
		}
		else if(mode==SelectionMode::Face)
		{
			Object *theObject=theObjectList[target];
			Quaternion original(-theObject->rotation.w,Vector(theObject->rotation));
			Matrix originalMatrix;
			Matrix rotateMatrix;
			rotateQuaternion.getMatrix(rotateMatrix);
			original.getMatrix(originalMatrix);
			rotateFace(originalMatrix*(theAxisCursor->position-theObject->center)+theObject->center,rotateMatrix);
			theObject->partialSubdivision();		
		}
	};

	void rotateVertex(const Vector &center, Matrix &angle)
	{
		size_t selectionCount=selection.size();
		for(size_t i=0;i<selectionCount;++i)
		{
			Vertex *theVertex=theObjectList[target]->vertex(selection[i]);
			if(theVertex)
			{
				Vector PToC=(theVertex->position-center);
				PToC=angle*PToC+center;
				theObjectList[target]->vertexPositionChangeA(theVertex->index,PToC.x,PToC.y,PToC.z);
			}
		}
	};

	void rotateEdge(const Vector &center,Matrix &angle)
	{
		size_t selectionCount=selection.size();
		std::vector<Vertex *> vertexToBeRotate;
		vertexToBeRotate.reserve(selectionCount*2);
		for(size_t i=0;i<selectionCount;++i)
		{
			Edge *theEdge=theObjectList[target]->edge(selection[i]);
			if(theEdge)
			{
				Vertex *theStart=theObjectList[target]->vertex(theEdge->start);
				Vertex *theEnd=theObjectList[target]->vertex(theEdge->end);
				if(!theStart->isIn)
				{
					vertexToBeRotate.push_back(theStart);
					isInCache.push_back(theStart);
					theStart->isIn=true;
				}
				if(!theEnd->isIn)
				{
					vertexToBeRotate.push_back(theEnd);
					isInCache.push_back(theEnd);
					theEnd->isIn=true;
				}
			}
		}

		clearIsInCache();

		for(size_t i=0;i<vertexToBeRotate.size();++i)
		{
			Vertex *theVertex=vertexToBeRotate[i];
			if(theVertex)
			{
				Vector PToC=(theVertex->position-center);
				PToC=angle*PToC+center;
				theObjectList[target]->vertexPositionChangeA(theVertex->index,PToC.x,PToC.y,PToC.z);
			}
		}
	};

	void rotateFace(const Vector &center,Matrix &angle)
	{
		size_t selectionCount=selection.size();
		std::vector<Vertex *> vertexToBeRotate;
		vertexToBeRotate.reserve(selectionCount*2);
		for(size_t i=0;i<selectionCount;++i)
		{
			Face *theFace=theObjectList[target]->face(selection[i]);
			size_t edgeCount=theFace->edge.size();
			for(size_t e=0;e<edgeCount;++e)
			{
				if(theFace->edge[e]>0)
				{
					Edge *theEdge=theObjectList[target]->edge(theFace->edge[e]);
					Vertex *theVertex=theObjectList[target]->vertex(theEdge->end);
					if(!theVertex->isIn)
					{
						vertexToBeRotate.push_back(theVertex);
						theVertex->isIn=true;
						isInCache.push_back(theVertex);
					}
				}
				else
				{
					Edge *theEdge=theObjectList[target]->edge(-theFace->edge[e]);
					Vertex *theVertex=theObjectList[target]->vertex(theEdge->start);
					if(!theVertex->isIn)
					{
						vertexToBeRotate.push_back(theVertex);
						theVertex->isIn=true;
						isInCache.push_back(theVertex);
					}
				}
			}
		}

		clearIsInCache();

		for(size_t i=0;i<vertexToBeRotate.size();++i)
		{
			Vertex *theVertex=vertexToBeRotate[i];
			if(theVertex)
			{
				Vector PToC=(theVertex->position-center);
				PToC=angle*PToC+center;
				theObjectList[target]->vertexPositionChangeA(theVertex->index,PToC.x,PToC.y,PToC.z);
			}
		}
	}

	void scaleVertex(Vector &center,Matrix &forward,Matrix &backward,float scale,int smode)
	{
		size_t selectionCount=selection.size();
		for(size_t i=0;i<selectionCount;++i)
		{
			Vertex *theVertex=theObjectList[target]->vertex(selection[i]);
			if(theVertex)
			{
				Vector newPosition(theVertex->position-theObjectList[target]->center);
				newPosition=forward*newPosition;
				newPosition=newPosition-(center-theObjectList[target]->center);
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
				newPosition+=(center-theObjectList[target]->center);
				newPosition=backward*newPosition;
				newPosition+=theObjectList[target]->center;
				theObjectList[target]->vertexPositionChangeA(theVertex->index,newPosition.x,newPosition.y,newPosition.z);
			}
		}
	};

	void scaleEdge(Vector &center,Matrix &forward,Matrix &backward,float scale,int smode)
	{
		size_t selectionCount=selection.size();
		std::vector<Vertex *> vertexToBeScale;
		vertexToBeScale.reserve(selectionCount*2);
		for(size_t i=0;i<selectionCount;++i)
		{
			Edge *theEdge=theObjectList[target]->edge(selection[i]);
			if(theEdge)
			{
				Vertex *theStart=theObjectList[target]->vertex(theEdge->start);
				Vertex *theEnd=theObjectList[target]->vertex(theEdge->end);
				if(!theStart->isIn)
				{
					vertexToBeScale.push_back(theStart);
					isInCache.push_back(theStart);
					theStart->isIn=true;
				}
				if(!theEnd->isIn)
				{
					vertexToBeScale.push_back(theEnd);
					isInCache.push_back(theEnd);
					theEnd->isIn=true;
				}
			}
		}

		clearIsInCache();

		size_t vertexCount=vertexToBeScale.size();
		for(size_t i=0;i<vertexCount;++i)
		{
			Vertex *theVertex=vertexToBeScale[i];
			if(theVertex)
			{
				Vector newPosition(theVertex->position-theObjectList[target]->center);
				newPosition=forward*newPosition;
				newPosition=newPosition-(center-theObjectList[target]->center);
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
				newPosition+=(center-theObjectList[target]->center);
				newPosition=backward*newPosition;
				newPosition+=theObjectList[target]->center;
				theObjectList[target]->vertexPositionChangeA(theVertex->index,newPosition.x,newPosition.y,newPosition.z);
			}
		}
	}

	void scaleFace(Vector &center,Matrix &forward,Matrix &backward,float scale,int smode)
	{
		size_t selectionCount=selection.size();
		std::vector<Vertex *> vertexToBeScale;
		vertexToBeScale.reserve(selectionCount*2);
		for(size_t i=0;i<selectionCount;++i)
		{
			Face *theFace=theObjectList[target]->face(selection[i]);
			size_t edgeCount=theFace->edge.size();
			for(size_t e=0;e<edgeCount;++e)
			{
				if(theFace->edge[e]>0)
				{
					Edge *theEdge=theObjectList[target]->edge(theFace->edge[e]);
					Vertex *theVertex=theObjectList[target]->vertex(theEdge->end);
					if(!theVertex->isIn)
					{
						vertexToBeScale.push_back(theVertex);
						theVertex->isIn=true;
						isInCache.push_back(theVertex);
					}
				}
				else
				{
					Edge *theEdge=theObjectList[target]->edge(-theFace->edge[e]);
					Vertex *theVertex=theObjectList[target]->vertex(theEdge->start);
					if(!theVertex->isIn)
					{
						vertexToBeScale.push_back(theVertex);
						theVertex->isIn=true;
						isInCache.push_back(theVertex);
					}
				}
			}
		}

		clearIsInCache();

		size_t vertexCount=vertexToBeScale.size();
		for(size_t i=0;i<vertexCount;++i)
		{
			Vertex *theVertex=vertexToBeScale[i];
			if(theVertex)
			{
				Vector newPosition(theVertex->position-theObjectList[target]->center);
				newPosition=forward*newPosition;
				newPosition=newPosition-(center-theObjectList[target]->center);
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
				newPosition+=(center-theObjectList[target]->center);
				newPosition=backward*newPosition;
				newPosition+=theObjectList[target]->center;
				theObjectList[target]->vertexPositionChangeA(theVertex->index,newPosition.x,newPosition.y,newPosition.z);
			}
		}
	}

	void axisZRotate(float step)
	{
		theAxisCursor->rotation*=theAxisCursor->rotation.w;
		theAxisCursor->rotation.y+=step;
		float angle=theAxisCursor->rotation.length();
		theAxisCursor->rotation.normalize();
		theAxisCursor->rotation.w=angle;

		Vector rotateAxis=Vector(0,1,0);
		Quaternion rotateQuaternion(step,rotateAxis);
		Matrix rotateMatrix;
		rotateQuaternion.getMatrix(rotateMatrix);

		if(mode==SelectionMode::Object && target==0)
		{
			for(size_t i=0;i<selection.size();++i)
			{
				Object *theObject=theObjectList[selection[i]];
				if(theObject)
				{
					//旋转分成两部，第一步是更新位置，第二步是更新角度
					//首先要更新位置
					//对于每个物体首先求出中心点到旋转轴的一个向量
					//new position;
					Vector CToR(theObject->center+theObject->position-theAxisCursor->position);
					//将这个向量沿着旋转轴进行旋转
					CToR=(rotateMatrix)*(CToR);
					//旋转之后还原新的中心点
					theObject->position=theAxisCursor->position+CToR-theObject->center;
					//之后要旋转物体的角度
				/*	theObject->rotation*=theObject->rotation.w;
					theObject->rotation.y+=step;
					float angle=theObject->rotation.length();
					theObject->rotation.normalize();
					theObject->rotation.w=angle;*/

					Quaternion original(theObject->rotation.w,Vector(theObject->rotation));
					Quaternion add(step,Vector(0,1,0));
					Quaternion result=add*original;
					result.getRotate(theObject->rotation.w,theObject->rotation);
				}
			}
		}
		else if(mode==SelectionMode::Vertex)
		{
			Object *theObject=theObjectList[target];
			Quaternion original(-theObject->rotation.w,Vector(theObject->rotation));
			Matrix originalMatrix;
			Matrix rotateMatrix;
			rotateQuaternion.getMatrix(rotateMatrix);
			original.getMatrix(originalMatrix);
			rotateVertex(originalMatrix*(theAxisCursor->position-theObject->center)+theObject->center,rotateMatrix);
			theObject->partialSubdivision();
		}		
		else if(mode==SelectionMode::Edge)
		{
			Object *theObject=theObjectList[target];
			Quaternion original(-theObject->rotation.w,Vector(theObject->rotation));
			Matrix originalMatrix;
			Matrix rotateMatrix;
			rotateQuaternion.getMatrix(rotateMatrix);
			original.getMatrix(originalMatrix);
			rotateEdge(originalMatrix*(theAxisCursor->position-theObject->center)+theObject->center,rotateMatrix);
			theObject->partialSubdivision();
		}
		else if(mode==SelectionMode::Face)
		{
			Object *theObject=theObjectList[target];
			Quaternion original(-theObject->rotation.w,Vector(theObject->rotation));
			Matrix originalMatrix;
			Matrix rotateMatrix;
			rotateQuaternion.getMatrix(rotateMatrix);
			original.getMatrix(originalMatrix);
			rotateFace(originalMatrix*(theAxisCursor->position-theObject->center)+theObject->center,rotateMatrix);
			theObject->partialSubdivision();			
		}
	};

	void axisYMove(float step)
	{
		theAxisCursor->position.y+=step;
		if(mode==SelectionMode::Object && target==0)
		{
			for(size_t i=0;i<selection.size();++i)
			{
				Object *theObject=theObjectList[selection[i]];
				if(theObject)
				{
					theObject->position.y+=step;
				}
			}
		}
		else if(mode==SelectionMode::Vertex && target)
		{
			Object *theObject=theObjectList[target];
			Quaternion angle(-theObject->rotation.w,Vector(theObject->rotation));
			Matrix rotateMatrix;
			angle.getMatrix(rotateMatrix);
			Vector xAxis(0,1,0);
			xAxis=(xAxis)*(rotateMatrix)*step;
			moveVertex(xAxis.x,xAxis.y,xAxis.z);
			theObject->partialSubdivision();
		}
		else if(mode==SelectionMode::Edge && target)
		{
						Object *theObject=theObjectList[target];
			Quaternion angle(-theObject->rotation.w,Vector(theObject->rotation));
			Matrix rotateMatrix;
			angle.getMatrix(rotateMatrix);
			Vector xAxis(0,1,0);
			xAxis=(xAxis)*(rotateMatrix)*step;
			moveEdge(xAxis.x,xAxis.y,xAxis.z);
			theObject->partialSubdivision();
		}
		else if(mode==SelectionMode::Face && target)
		{
						Object *theObject=theObjectList[target];
			Quaternion angle(-theObject->rotation.w,Vector(theObject->rotation));
			Matrix rotateMatrix;
			angle.getMatrix(rotateMatrix);
			Vector xAxis(0,1,0);
			xAxis=(xAxis)*(rotateMatrix)*step;
			moveFace(xAxis.x,xAxis.y,xAxis.z);
			theObject->partialSubdivision();
		}
	};

	void axisZMove(float step)
	{
		theAxisCursor->position.z+=step;
		if(mode==SelectionMode::Object && target==0)
		{
			for(size_t i=0;i<selection.size();++i)
			{
				Object *theObject=theObjectList[selection[i]];
				if(theObject)
				{
					theObject->position.z+=step;
				}
			}
		}
		else if(mode==SelectionMode::Vertex && target)
		{
			Object *theObject=theObjectList[target];
			Quaternion angle(-theObject->rotation.w,Vector(theObject->rotation));
			Matrix rotateMatrix;
			angle.getMatrix(rotateMatrix);
			Vector xAxis(0,0,1);
			xAxis=(xAxis)*(rotateMatrix)*step;
			moveVertex(xAxis.x,xAxis.y,xAxis.z);
			theObject->partialSubdivision();
		}
		else if(mode==SelectionMode::Edge && target)
		{
			Object *theObject=theObjectList[target];
			Quaternion angle(-theObject->rotation.w,Vector(theObject->rotation));
			Matrix rotateMatrix;
			angle.getMatrix(rotateMatrix);
			Vector xAxis(0,0,1);
			xAxis=(xAxis)*(rotateMatrix)*step;
			moveEdge(xAxis.x,xAxis.y,xAxis.z);
			theObject->partialSubdivision();
		}
				else if(mode==SelectionMode::Face && target)
		{
			Object *theObject=theObjectList[target];
			Quaternion angle(-theObject->rotation.w,Vector(theObject->rotation));
			Matrix rotateMatrix;
			angle.getMatrix(rotateMatrix);
			Vector xAxis(0,0,1);
			xAxis=(xAxis)*(rotateMatrix)*step;
			moveFace(xAxis.x,xAxis.y,xAxis.z);
			theObject->partialSubdivision();
		}

	};

	void axisXScale(float step)
	{
		if(mode==SelectionMode::Object && target==0)
		{
			for(size_t i=0;i<selection.size();++i)
			{
				Object *theObject=theObjectList[selection[i]];
				if(theObject)
				{
					Quaternion angle(-theObject->rotation.w,Vector(theObject->rotation));
					Matrix rotateMatrix;
					angle.getMatrix(rotateMatrix);
					Vector xAxis(1,0,0);
					xAxis=(xAxis)*(rotateMatrix);
					theObject->scale+=step*xAxis*0.01f;
				}
			}
		}
		else if(mode==SelectionMode::Vertex && target)
		{
			Object *theObject=theObjectList[target];
			Quaternion original(theObject->rotation.w,Vector(theObject->rotation));
			Quaternion back(-theObject->rotation.w,Vector(theObject->rotation));
			Matrix originalMatrix;
			Matrix backMatrix;
			original.getMatrix(originalMatrix);
			back.getMatrix(backMatrix);
			scaleVertex(theAxisCursor->position,originalMatrix,backMatrix,(1+step*0.1f),1);
			theObject->partialSubdivision();
		}
		else if(mode==SelectionMode::Edge && target)
		{
			Object *theObject=theObjectList[target];
			Quaternion original(theObject->rotation.w,Vector(theObject->rotation));
			Quaternion back(-theObject->rotation.w,Vector(theObject->rotation));
			Matrix originalMatrix;
			Matrix backMatrix;
			original.getMatrix(originalMatrix);
			back.getMatrix(backMatrix);
			scaleEdge(theAxisCursor->position,originalMatrix,backMatrix,(1+step*0.1f),1);
			theObject->partialSubdivision();		
		}
		else if(mode==SelectionMode::Face && target)
		{
			Object *theObject=theObjectList[target];
			Quaternion original(theObject->rotation.w,Vector(theObject->rotation));
			Quaternion back(-theObject->rotation.w,Vector(theObject->rotation));
			Matrix originalMatrix;
			Matrix backMatrix;
			original.getMatrix(originalMatrix);
			back.getMatrix(backMatrix);
			scaleFace(theAxisCursor->position,originalMatrix,backMatrix,(1+step*0.1f),1);
			theObject->partialSubdivision();					
		}
	};

	void axisYScale(float step)
	{
		if(mode==SelectionMode::Object && target==0)
		{
			for(size_t i=0;i<selection.size();++i)
			{
				Object *theObject=theObjectList[selection[i]];
				if(theObject)
				{
					Quaternion angle(-theObject->rotation.w,Vector(theObject->rotation));
					Matrix rotateMatrix;
					angle.getMatrix(rotateMatrix);
					Vector yAxis(0,1,0);
					yAxis=(yAxis)*(rotateMatrix);
					theObject->scale+=step*yAxis*0.01f;
				}
			}
		}		
		else if(mode==SelectionMode::Vertex && target)
		{
			Object *theObject=theObjectList[target];
			Quaternion original(theObject->rotation.w,Vector(theObject->rotation));
			Quaternion back(-theObject->rotation.w,Vector(theObject->rotation));
			Matrix originalMatrix;
			Matrix backMatrix;
			original.getMatrix(originalMatrix);
			back.getMatrix(backMatrix);
			//scaleVertex(originalMatrix*(theAxisCursor->position-theObject->center)+theObject->center,xAxis,(step*0.1));
			scaleVertex(theAxisCursor->position,originalMatrix,backMatrix,(1+step*0.1f),2);
			theObject->partialSubdivision();
		}
		else if(mode==SelectionMode::Edge && target)
		{
			Object *theObject=theObjectList[target];
			Quaternion original(theObject->rotation.w,Vector(theObject->rotation));
			Quaternion back(-theObject->rotation.w,Vector(theObject->rotation));
			Matrix originalMatrix;
			Matrix backMatrix;
			original.getMatrix(originalMatrix);
			back.getMatrix(backMatrix);
			//scaleVertex(originalMatrix*(theAxisCursor->position-theObject->center)+theObject->center,xAxis,(step*0.1));
			scaleEdge(theAxisCursor->position,originalMatrix,backMatrix,(1+step*0.1f),2);
			theObject->partialSubdivision();			
		}
		else if(mode==SelectionMode::Face && target)
		{
			Object *theObject=theObjectList[target];
			Quaternion original(theObject->rotation.w,Vector(theObject->rotation));
			Quaternion back(-theObject->rotation.w,Vector(theObject->rotation));
			Matrix originalMatrix;
			Matrix backMatrix;
			original.getMatrix(originalMatrix);
			back.getMatrix(backMatrix);
			//scaleVertex(originalMatrix*(theAxisCursor->position-theObject->center)+theObject->center,xAxis,(step*0.1));
			scaleFace(theAxisCursor->position,originalMatrix,backMatrix,(1+step*0.1f),2);
			theObject->partialSubdivision();						
		}
	};

	void axisZScale(float step)
	{
		if(mode==SelectionMode::Object && target==0)
		{
			for(size_t i=0;i<selection.size();++i)
			{
				Object *theObject=theObjectList[selection[i]];
				if(theObject)
				{
					Quaternion angle(-theObject->rotation.w,Vector(theObject->rotation));
					Matrix rotateMatrix;
					angle.getMatrix(rotateMatrix);
					Vector zAxis(0,0,1);
					zAxis=(zAxis)*(rotateMatrix);
					theObject->scale+=step*zAxis*0.01f;
				}
			}
		}		
		else if(mode==SelectionMode::Vertex && target)
		{
			Object *theObject=theObjectList[target];
			Quaternion original(theObject->rotation.w,Vector(theObject->rotation));
					Quaternion back(-theObject->rotation.w,Vector(theObject->rotation));
			Matrix originalMatrix;
			Matrix backMatrix;
			original.getMatrix(originalMatrix);
			back.getMatrix(backMatrix);

			//scaleVertex(originalMatrix*(theAxisCursor->position-theObject->center)+theObject->center,xAxis,(step*0.1));
			scaleVertex(theAxisCursor->position,originalMatrix,backMatrix,(1+step*0.1f),3);
			theObject->partialSubdivision();
		}
		else if(mode==SelectionMode::Edge && target)
		{
			Object *theObject=theObjectList[target];
			Quaternion original(theObject->rotation.w,Vector(theObject->rotation));
					Quaternion back(-theObject->rotation.w,Vector(theObject->rotation));
			Matrix originalMatrix;
			Matrix backMatrix;
			original.getMatrix(originalMatrix);
			back.getMatrix(backMatrix);

			//scaleVertex(originalMatrix*(theAxisCursor->position-theObject->center)+theObject->center,xAxis,(step*0.1));
			scaleEdge(theAxisCursor->position,originalMatrix,backMatrix,(1+step*0.1f),3);
			theObject->partialSubdivision();			
		}
		else if(mode==SelectionMode::Face && target)
		{
			Object *theObject=theObjectList[target];
			Quaternion original(theObject->rotation.w,Vector(theObject->rotation));
					Quaternion back(-theObject->rotation.w,Vector(theObject->rotation));
			Matrix originalMatrix;
			Matrix backMatrix;
			original.getMatrix(originalMatrix);
			back.getMatrix(backMatrix);

			//scaleVertex(originalMatrix*(theAxisCursor->position-theObject->center)+theObject->center,xAxis,(step*0.1));
			scaleFace(theAxisCursor->position,originalMatrix,backMatrix,(1+step*0.1f),3);
			theObject->partialSubdivision();			
		}
	};

	void axisRelease()
	{
		theAxisCursor->rotation.null();
	};

	void ctrlDrag(Vector &horizontal,Vector &vertical,int x,int y,bool isExtrude)
	{
		if(!selection.empty())
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
			glGetDoublev(GL_MODELVIEW_MATRIX,modelMatrix);
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
			if(mode==SelectionMode::Object)
			{
				for(size_t i=0;i<selection.size();++i)
				{
					theObjectList[selection[i]]->position-=offset;
				}
			}
			else if(mode==SelectionMode::Vertex && target)
			{
				moveVertex(-offset.x,-offset.y,-offset.z);
				theObjectList[target]->partialSubdivision();
			}
			else if(mode==SelectionMode::Edge && target)
			{
				if(isExtrude)
				{
					theObjectList[target]->clearPSCache();
					extrudeEdge(-offset.x,-offset.y,-offset.z);
					theObjectList[target]->updateSubdivision();
					theObjectList[target]->buildPSCacheFromEID(selection);
				}
				else
				{
					moveEdge(-offset.x,-offset.y,-offset.z);
					theObjectList[target]->partialSubdivision();
				}
			}
			else if(mode==SelectionMode::Face && target)
			{
				if(isExtrude)
				{
					theObjectList[target]->clearPSCache();
					extrudeFaceGroup(-offset.x,-offset.y,-offset.z);
					theObjectList[target]->updateSubdivision();
					theObjectList[target]->buildPSCacheFromFID(selection);
				}
				else
				{
					moveFace(-offset.x,-offset.y,-offset.z);
					theObjectList[target]->partialSubdivision();
				}
			}
		}
	}

	void axisDrag(const float &step,const int &cursorMode,float &rx,float &ry)
	{
		GLint viewport[4];
		glGetIntegerv(GL_VIEWPORT, viewport);
		GLdouble modelMatrix[16];
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glTranslatef(theAxisCursor->position.x,theAxisCursor->position.y,theAxisCursor->position.z);
		glRotatef(theAxisCursor->rotation.w,theAxisCursor->rotation.x,theAxisCursor->rotation.y,theAxisCursor->rotation.z);
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
		if(cursorMode==1)
		{
			//x轴
			if(theAxisCursor->mode==AxisCursorMode::MoveAxis)
			{
				gluProject(0,0,0,modelMatrix,projMatrix,viewport,&ox,&oy,&oz);
				oy=(GLdouble)viewport[3]-(GLint)oy;
				gluProject(1,0,0,modelMatrix,projMatrix,viewport,&winx,&winy,&winz);
				winy=(GLdouble)viewport[3]-winy;
				Vector projUnit((float)(winx-ox),(float)(winy-oy),0.0f);
				axisXMove(step/projUnit.length());
			}
			else if(theAxisCursor->mode==AxisCursorMode::RotateAxis)
			{
				gluProject(0,0,0,modelMatrix,projMatrix,viewport,&ox,&oy,&oz);
				oy=(GLdouble)viewport[3]-(GLint)oy;
				gluProject(0,1,0,modelMatrix,projMatrix,viewport,&winx,&winy,&winz);
				winy=(GLdouble)viewport[3]-winy;
				Vector projUnit((float)(winx-ox),(float)(winy-oy),0.0f);
				axisXRotate(step/projUnit.length());
				//这里修改新向量
				rx=(float)(winx-ox);
				ry=(float)(winy-oy);
			}
			else if(theAxisCursor->mode==AxisCursorMode::ScaleAxis)
			{
				gluProject(0,0,0,modelMatrix,projMatrix,viewport,&ox,&oy,&oz);
				oy=(GLdouble)viewport[3]-(GLint)oy;
				gluProject(1,0,0,modelMatrix,projMatrix,viewport,&winx,&winy,&winz);
				winy=(GLdouble)viewport[3]-winy;
				Vector projUnit((float)(winx-ox),(float)(winy-oy),0.0f);
				//theAxisCursor->position.x+=step/projUnit.length();
				axisXScale(step/projUnit.length());
			}
		}
		else if(cursorMode==2)
		{
			if(theAxisCursor->mode==AxisCursorMode::MoveAxis)
			{
				gluProject(0,0,0,modelMatrix,projMatrix,viewport,&ox,&oy,&oz);
				oy=(GLdouble)viewport[3]-oy;
				gluProject(0,1,0,modelMatrix,projMatrix,viewport,&winx,&winy,&winz);
				winy=(GLdouble)viewport[3]-winy;
				Vector projUnit((float)(winx-ox),(float)(winy-oy),0.0f);
				axisYMove(step/projUnit.length());
			}
			else if(theAxisCursor->mode==AxisCursorMode::RotateAxis)
			{
				gluProject(0,0,0,modelMatrix,projMatrix,viewport,&ox,&oy,&oz);
				oy=(GLdouble)viewport[3]-(GLint)oy;
				gluProject(0,0,1,modelMatrix,projMatrix,viewport,&winx,&winy,&winz);
				winy=(GLdouble)viewport[3]-winy;
				Vector projUnit((float)(winx-ox),(float)(winy-oy),0.0f);
				axisYRotate(step/projUnit.length());
				//这里修改新向量
				rx=(float)(winx-ox);
				ry=(float)(winy-oy);
			}
			else if(theAxisCursor->mode==AxisCursorMode::ScaleAxis)
			{
				gluProject(0,0,0,modelMatrix,projMatrix,viewport,&ox,&oy,&oz);
				oy=(GLdouble)viewport[3]-(GLint)oy;
				gluProject(0,1,0,modelMatrix,projMatrix,viewport,&winx,&winy,&winz);
				winy=(GLdouble)viewport[3]-winy;
				Vector projUnit((float)(winx-ox),(float)(winy-oy),0.0f);
				//theAxisCursor->position.y+=step/projUnit.length();
				axisYScale(step/projUnit.length());
			}
		}
		else if(cursorMode==3)
		{
			if(theAxisCursor->mode==AxisCursorMode::MoveAxis)
			{
				gluProject(0,0,0,modelMatrix,projMatrix,viewport,&ox,&oy,&oz);
				oy=(GLdouble)viewport[3]-oy;
				gluProject(0,0,1,modelMatrix,projMatrix,viewport,&winx,&winy,&winz);
				winy=(GLdouble)viewport[3]-winy;
				Vector projUnit((float)(winx-ox),(float)(winy-oy),0.0f);
				axisZMove(step/projUnit.length());
			}
			else if(theAxisCursor->mode==AxisCursorMode::RotateAxis)
			{
				gluProject(0,0,0,modelMatrix,projMatrix,viewport,&ox,&oy,&oz);
				oy=(GLdouble)viewport[3]-(GLint)oy;
				gluProject(1,0,0,modelMatrix,projMatrix,viewport,&winx,&winy,&winz);
				winy=(GLdouble)viewport[3]-winy;
				Vector projUnit((float)(winx-ox),(float)(winy-oy),0.0f);
		axisZRotate(step/projUnit.length());
				//这里修改新向量
				rx=(float)(winx-ox);
				ry=(float)(winy-oy);
			}
			else if(theAxisCursor->mode==AxisCursorMode::ScaleAxis)
			{
				gluProject(0,0,0,modelMatrix,projMatrix,viewport,&ox,&oy,&oz);
				oy=(GLdouble)viewport[3]-(GLint)oy;
				gluProject(0,0,1,modelMatrix,projMatrix,viewport,&winx,&winy,&winz);
				winy=(GLdouble)viewport[3]-winy;
				Vector projUnit((float)(winx-ox),(float)(winy-oy),0.0f);
				//theAxisCursor->position.z+=step/projUnit.length();
				axisZScale(step/projUnit.length());
			}
		}
		else if(cursorMode==4)
		{
		
		}
	};

	void unSubdivide()
	{
		if(mode==SelectionMode::Object && target==0)
		{
			if(!selection.empty())
			{
				for(size_t i=0;i<selection.size();++i)
				{
					theObjectList[selection[i]]->unSubdivide();
				}
			}
		}
		else
		{
			if(mode==SelectionMode::Vertex && target)
			{
				theObjectList[target]->clearPSCache();
				theObjectList[target]->unSubdivide();
				if(!selection.empty())
				{
					theObjectList[target]->buildPSCacheFromVID(selection);
				}
			}
		}
	}

	void selectSingleSideEdge(size_t x1,size_t y1,size_t x2,size_t y2,size_t height,bool isAppend=false)
	{
		if(!isAppend)
		{
			clearSelection();
		}
		Object *theObject=theObjectList[target];
		if(theObject->edgeCount()<16777215)
		{
			glClearColor(1.0f,1.0f,1.0f,1.0f);
			glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);
			glDisable(GL_DITHER);
			glDisable(GL_LIGHTING);
			glDisable(GL_TEXTURE_2D);
			glDisable(GL_ALPHA_TEST);
			glDisable(GL_BLEND);
				glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glTranslatef(theObject->position.x,theObject->position.y,theObject->position.z);
		glTranslatef(theObject->center.x,theObject->center.y,theObject->center.z);
		glRotatef(theObject->rotation.w,theObject->rotation.x,theObject->rotation.y,theObject->rotation.z);
		glScalef(theObject->scale.x,theObject->scale.y,theObject->scale.z);	
		glTranslatef(-theObject->center.x,-theObject->center.y,-theObject->center.z);
		
			
			glColorMask(GL_FALSE,GL_FALSE,GL_FALSE,GL_FALSE);
			glEnable(GL_POLYGON_OFFSET_FILL);
			glPolygonOffset(1.0f,1.0f);
			for(size_t i=1;i<theObject->faceCount();++i)
			{
				Face *theFace=theObject->face(i);
				if(theFace)
				{
					struct ColorID colorID=(*(struct ColorID*)&(i));
					glColor4ub(colorID.r, colorID.g,colorID.b,colorID.a);
					glBegin(GL_POLYGON);
					for(size_t e=0;e<theFace->edge.size();++e)
					{
						if(theFace->edge[e]>0)
						{
							Vertex *v=theObject->vertex(theObject->edge(theFace->edge[e])->start);
							glVertex3f(v->position.x,v->position.y,v->position.z);
						}
						else
						{
							Vertex *v=theObject->vertex(theObject->edge(-theFace->edge[e])->end);
							glVertex3f(v->position.x,v->position.y,v->position.z);
						}
					}
					glEnd();
				}
			}
			glDisable(GL_POLYGON_OFFSET_FILL);
			glColorMask(GL_TRUE,GL_TRUE,GL_TRUE,GL_TRUE);
			glBegin(GL_LINES);
			for(size_t i=1;i<theObject->edgeCount();++i)
			{
				Edge *theEdge=theObject->edge(i);
				if(theEdge)
				{
					struct ColorID colorID=(*(struct ColorID*)&(i));
					glColor4ub(colorID.r, colorID.g,colorID.b,colorID.a);
					Vertex *start=theObject->vertex(theEdge->start);
					Vertex *end=theObject->vertex(theEdge->end);
					glVertex3f(start->position.x,start->position.y,start->position.z);
					glVertex3f(end->position.x,end->position.y,end->position.z);
				}
			}
			glEnd();
			glFlush();
			glPopMatrix();
			size_t sw=(x2-x1)>5?(x2-x1):5;
			size_t sh=(y2-y1)>5?(y2-y1):5;
			size_t length=sw*sh;
			struct ColorID *pixel=new struct ColorID[length];
			glReadPixels(x1,height-y2,sw,sh, GL_RGBA, GL_UNSIGNED_BYTE, pixel);
			for(size_t e=0;e<length;++e)
			{
				pixel[e].a=0;
				size_t result=(*(size_t*)&(pixel[e]));
				if(result<16777215)
				{
					if(result<theObject->edgeCount()&&theObject->edge(result))
					{
						selectionPush(theObject->edge(result));
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
						if(!selection.empty())
			{
				theObject->buildPSCacheFromEID(selection);
			}
		}
	};

	void selectSingleSideFace(size_t x1,size_t y1,size_t x2,size_t y2,size_t height,bool isAppend=false)
	{
		if(!isAppend)
		{
			clearSelection();
		}
		Object *theObject=theObjectList[target];
		if(theObject->faceCount()<16777215)
		{
			glClearColor(1.0f,1.0f,1.0f,1.0f);
			glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);
			glDisable(GL_DITHER);
			glDisable(GL_LIGHTING);
			glDisable(GL_TEXTURE_2D);
			glDisable(GL_ALPHA_TEST);
			glDisable(GL_BLEND);
							glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glTranslatef(theObject->position.x,theObject->position.y,theObject->position.z);
				glTranslatef(theObject->center.x,theObject->center.y,theObject->center.z);
		glRotatef(theObject->rotation.w,theObject->rotation.x,theObject->rotation.y,theObject->rotation.z);
glScalef(theObject->scale.x,theObject->scale.y,theObject->scale.z);	
		glTranslatef(-theObject->center.x,-theObject->center.y,-theObject->center.z);

			for(size_t i=1;i<theObject->faceCount();++i)
			{
				Face *theFace=theObject->face(i);
				if(theFace)
				{
					struct ColorID colorID=(*(struct ColorID*)&(i));
					glColor4ub(colorID.r, colorID.g,colorID.b,colorID.a);
					glBegin(GL_POLYGON);
					for(size_t e=0;e<theFace->edge.size();++e)
					{
						if(theFace->edge[e]>0)
						{
							Vertex *v=theObject->vertex(theObject->edge(theFace->edge[e])->start);
							glVertex3f(v->position.x,v->position.y,v->position.z);
						}
						else
						{
							Vertex *v=theObject->vertex(theObject->edge(-theFace->edge[e])->end);
							glVertex3f(v->position.x,v->position.y,v->position.z);
						}
					}
					glEnd();
				}
			}
			glFlush();
			glPopMatrix();
			size_t sw=(x2-x1)>5?(x2-x1):5;
			size_t sh=(y2-y1)>5?(y2-y1):5;
			size_t length=sw*sh;
			struct ColorID *pixel=new struct ColorID[length];
			glReadPixels(x1,height-y2,sw,sh, GL_RGBA, GL_UNSIGNED_BYTE, pixel);
			for(size_t e=0;e<length;e+=2)
			{
				pixel[e].a=0;
				size_t result=(*(size_t*)&(pixel[e]));
				if(result<16777215)
				{
					if(result<theObject->faceCount()&&theObject->face(result))
					{
						selectionPush(theObject->face(result));
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
									if(!selection.empty())
			{
				theObject->buildPSCacheFromFID(selection);
			}
		}
	};

	void selectSingleSideObject(size_t x1,size_t y1,size_t x2,size_t y2,size_t height,bool isAppend=false)
	{
        qDebug() << "single side object select";
		if(!isAppend)
		{
			clearSelection();
		}
		if(theObjectList.size()<16777215)
		{
			glClearColor(1.0f,1.0f,1.0f,1.0f);
			glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);
			glDisable(GL_DITHER);
			glDisable(GL_LIGHTING);
			glDisable(GL_TEXTURE_2D);
			glDisable(GL_ALPHA_TEST);
			glDisable(GL_BLEND);
			for(size_t i=1;i<theObjectList.size();++i)
			{
				if(theObjectList[i])
				{
					Object *theObject=theObjectList[i];
					struct ColorID colorID=(*(struct ColorID*)&(i));
					glColor4ub(colorID.r, colorID.g,colorID.b,colorID.a);
					theObject->selectionRenderObject();
				}
			}
			glFlush();
			size_t sw=(x2-x1)>5?(x2-x1):5;
			size_t sh=(y2-y1)>5?(y2-y1):5;
			size_t length=sw*sh;
			struct ColorID *pixel=new struct ColorID[length];
			glReadPixels(x1,height-y2,sw,sh, GL_RGBA, GL_UNSIGNED_BYTE, pixel);

         //   QImage image((uchar*) pixel, sw, sh,QImage::Format_RGBA8888);
         //   image.save("haha.png");

			for(size_t e=0;e<length;e+=2)
			{
				pixel[e].a=0;
				size_t result=(*(size_t*)&(pixel[e]));
				if(result<16777215)
				{
					if(result<theObjectList.size()&&theObjectList[result])
					{
						selectionPush(theObjectList[result]);
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
		}
	};

	void changeSelectionMode(SelectionMode::__Enum theMode)
	{
		if(mode==SelectionMode::Object && target==0)
		{
			if(selection.empty())
			{
				
			}
			else
			{
				target=selection[0];
				clearSelection();//因为是和选择类型相关的，记住一定要先清空，否则如果类型改变，清空就不正常了
				mode=theMode;
			}
		}
		else
		{
			if(target)
			{
				if(mode==SelectionMode::Split)
				{
					endSplit();
				}
				clearSelection();
				theObjectList[target]->clearPSCache();
				if((mode=theMode)==SelectionMode::Object)
				{
					target=0;
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
		for(size_t i=1;i<theObjectList.size();++i)
		{
			if(theObjectList[i])
			{
				theObjectList[i]->isHide=false;
			}
		}
	}

	void selectAll()
	{
		if(mode==SelectionMode::Object)
		{
			clearSelection();
			for(size_t i=1;i<theObjectList.size();++i)
			{
				if(theObjectList[i])
				{
					selectionPush(theObjectList[i]);
				}
			}
		}
	}

	void hideUnselection()
	{
		if(mode==SelectionMode::Object)
		{
			for(size_t i=1;i<theObjectList.size();++i)
			{
				if(theObjectList[i] && !(theObjectList[i]->isSelected))
				{
					theObjectList[i]->isHide=true;
				}
			}
		}
		else if(target>0)
		{
			for(size_t i=1;i<theObjectList.size();++i)
			{
				if(theObjectList[i])
				{
					theObjectList[i]->isHide=true;
				}
			}

			if(theObjectList[target])
			{
				theObjectList[target]->isHide=false;
			}
		}
	}

	void hideSelection()
	{
		if(mode==SelectionMode::Object)
		{
			for(size_t i=0;i<selection.size();++i)
			{
				if(theObjectList[selection[i]])
				{
					theObjectList[selection[i]]->isHide=true;
				}
			}
			clearSelection();
		}
		else if(target>0 && theObjectList[target])
		{
			theObjectList[target]->isHide=true;
			clearSelection();
			theObjectList[target]->clearPSCache();
			changeSelectionMode(SelectionMode::Object);
		}
	}

	void onPaint()
	{
		if(mode==SelectionMode::Object)
		{
			for(size_t i=1;i<theObjectList.size();++i)
			{
				if(theObjectList[i])
				{
					if(theObjectList[i]->isSelected)
					{
				
							theObjectList[i]->drawObjectSelected();
						theObjectList[i]->drawMirror();
					}
					else
					{
						theObjectList[i]->draw();
						theObjectList[i]->drawMirror();
					}
				}
			}
		}
		else
		{
			for(size_t i=1;i<theObjectList.size();++i)
			{
				if(theObjectList[i])
				{
					if(target!=i)
					{
						theObjectList[i]->draw();
						theObjectList[i]->drawMirror();
					}
					else
					{
						if(mode==SelectionMode::Face)
						{
							if(theObjectList[target]->renderMode==RenderType::Wire)
							{
								theObjectList[target]->drawWireFaceSelected();
							}
							else
							{
								theObjectList[target]->drawFacedFaceSelected();
							}
							drawSelectedFace();
							theObjectList[i]->drawMirror();
						}
						else if(mode==SelectionMode::Edge)
						{
							if(theObjectList[target]->renderMode==RenderType::Wire)
							{
								theObjectList[target]->drawWireEdgeSelected();
							}
							else
							{
								theObjectList[target]->drawFacedEdgeSelected();
							}
							drawSelectedEdge();
							theObjectList[i]->drawMirror();
						}
						else if(mode==SelectionMode::Vertex)
						{
							if(theObjectList[target]->renderMode==RenderType::Wire)
							{
								theObjectList[target]->drawWireVertexSelected();
							}
							else
							{
								theObjectList[target]->drawFacedVertexSelected();
							}
							drawSelectedVertex();
							theObjectList[i]->drawMirror();
						}
						else if(mode==SelectionMode::Split)
						{
							if(theObjectList[target]->renderMode==RenderType::Wire)
							{
								theObjectList[target]->drawWireVertexSelected();
							}
							else
							{
								theObjectList[target]->drawFacedVertexSelected();
							}
							drawCutVertex();
							theObjectList[i]->drawMirror();
						}
					}
				}
			}
		}
	};


	void drawCutVertex()
	{
		if(target && mode==SelectionMode::Split)
		{
			Object *theObject=theObjectList[target];
			if(theObject)
			{
				glMatrixMode(GL_MODELVIEW);
				glPushMatrix();
				glTranslatef(theObject->position.x,theObject->position.y,theObject->position.z);
				glTranslatef(theObject->center.x,theObject->center.y,theObject->center.z);
				glRotatef(theObject->rotation.w,theObject->rotation.x,theObject->rotation.y,theObject->rotation.z);
				glScalef(theObject->scale.x,theObject->scale.y,theObject->scale.z);	
				glTranslatef(-theObject->center.x,-theObject->center.y,-theObject->center.z);
				glDisable(GL_LIGHTING);
				glColor3ub(255,0,255);
				glPointSize(8.0f);
				glBegin(GL_POINTS);
				Vertex *theVertex=theObject->vertex(splitVertexID);
				if(theVertex)
				{
					glVertex3f(theVertex->position.x,theVertex->position.y,theVertex->position.z);
				}
				glEnd();
				glEnable(GL_LIGHTING);
				glPopMatrix();
			}
		}
	};

	void updateAxisCursor()
	{
		theAxisCursor->rotation.null();
		if(mode==SelectionMode::Object && target==0)
		{
			if(selection.empty())
			{
				theAxisCursor->mode=AxisCursorMode::NoAxis;
				theAxisCursor->position.null();
			}
			else
			{
				Vector *center=new Vector();
				size_t selectionSize=selection.size();
				for(size_t i=0;i<selectionSize;++i)
				{
					Object *theObject=theObjectList[selection[i]];
					if(theObject)
					(*center)+=theObject->center+theObject->position;
				}
				(*center)/=(float)selectionSize;
				theAxisCursor->position=*center;
				delete center;
				theAxisCursor->mode=currentACMode;
			}
		}
		else if(mode==SelectionMode::Vertex && target)
		{
			if(selection.empty())
			{
				theAxisCursor->mode=AxisCursorMode::NoAxis;
				theAxisCursor->position.null();
			}
			else
			{
				Vector *center=new Vector();
				size_t selectionSize=selection.size();
				Object *theObject=theObjectList[target];
				for(size_t i=0;i<selectionSize;++i)
				{
					Vertex *theVertex=theObject->vertex(selection[i]);
					if(theVertex)
						(*center)+=theVertex->position;
				}
				(*center)/=(float)selectionSize;
				//这里旋转
				Vector rotateAxis=Vector(theObject->rotation.x,theObject->rotation.y,theObject->rotation.z);
				Quaternion rotateQuaternion(theObject->rotation.w,rotateAxis);
				Matrix rotateMatrix;
				rotateQuaternion.getMatrix(rotateMatrix);
				*center=rotateMatrix*(*center-theObject->center);
				//这里平移
				theAxisCursor->position=*center+theObject->position+theObject->center;
				delete center;
				theAxisCursor->mode=currentACMode;
			}
		}
		else if(mode==SelectionMode::Edge && target)
		{
			if(selection.empty())
			{
				theAxisCursor->mode=AxisCursorMode::NoAxis;
				theAxisCursor->position.null();
			}
			else
			{
				Vector *center=new Vector();
				size_t selectionSize=selection.size();
				Object *theObject=theObjectList[target];
				for(size_t i=0;i<selectionSize;++i)
				{
					Edge *theEdge=theObject->edge(selection[i]);
					if(theEdge)
					{
						if(!theObject->vertex(theEdge->start)->isIn)
						{
							(*center)+=theObject->vertex(theEdge->start)->position;
							theObject->vertex(theEdge->start)->isIn=true;
							isInCache.push_back(theObject->vertex(theEdge->start));
						}
						if(!theObject->vertex(theEdge->end)->isIn)
						{
							(*center)+=theObject->vertex(theEdge->end)->position;
							theObject->vertex(theEdge->end)->isIn=true;
							isInCache.push_back(theObject->vertex(theEdge->end));						
						}
					}
				}
				(*center)/=(float)isInCache.size();
				clearIsInCache();
				//这里旋转
				Vector rotateAxis=Vector(theObject->rotation.x,theObject->rotation.y,theObject->rotation.z);
				Quaternion rotateQuaternion(theObject->rotation.w,rotateAxis);
				Matrix rotateMatrix;
				rotateQuaternion.getMatrix(rotateMatrix);
				*center=rotateMatrix*(*center-theObject->center);
				//这里平移
				theAxisCursor->position=*center+theObject->position+theObject->center;
				delete center;
				theAxisCursor->mode=currentACMode;

			}
		}
		else if(mode==SelectionMode::Face && target)
		{
			if(selection.empty())
			{
				theAxisCursor->mode=AxisCursorMode::NoAxis;
				theAxisCursor->position.null();
			}
			else
			{
				Vector *center=new Vector();
				size_t selectionSize=selection.size();
				Object *theObject=theObjectList[target];
				for(size_t i=0;i<selectionSize;++i)
				{
					Face *theFace=theObject->face(selection[i]);
					if(theFace)
					{
						size_t edgeCount=theFace->edge.size();
						for(size_t e=0;e<edgeCount;++e)
						{
							if(theFace->edge[e]>0)
							{
								Vertex *theStart=theObject->vertex(theObject->edge(theFace->edge[e])->start);
								if(!theStart->isIn)
								{
									(*center)+=theStart->position;
									theStart->isIn=true;
									isInCache.push_back(theStart);						
								}
							}
							else
							{
								Vertex *theEnd=theObject->vertex(theObject->edge(-theFace->edge[e])->end);
								if(!theEnd->isIn)
								{
									(*center)+=theEnd->position;
									theEnd->isIn=true;
									isInCache.push_back(theEnd);						
								}																
							}
						}
					}
				}
				(*center)/=(float)isInCache.size();
				clearIsInCache();
				//这里旋转
				Vector rotateAxis=Vector(theObject->rotation.x,theObject->rotation.y,theObject->rotation.z);
				Quaternion rotateQuaternion(theObject->rotation.w,rotateAxis);
				Matrix rotateMatrix;
				rotateQuaternion.getMatrix(rotateMatrix);
				*center=rotateMatrix*(*center-theObject->center);
				//这里平移
				theAxisCursor->position=*center+theObject->position+theObject->center;
				delete center;
				theAxisCursor->mode=currentACMode;
			}
		}
	};

	void drawCursor(CameraMode::__Enum cameraMode,const Vector &eye)
	{
		theAxisCursor->onPaint(eye,cameraMode);	
	};

	void drawSelectedVertex()
	{
		if(target && mode==SelectionMode::Vertex)
		{
			Object *theObject=theObjectList[target];
			if(theObject)
			{
								glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
				glTranslatef(theObject->position.x,theObject->position.y,theObject->position.z);
		glTranslatef(theObject->center.x,theObject->center.y,theObject->center.z);
		glRotatef(theObject->rotation.w,theObject->rotation.x,theObject->rotation.y,theObject->rotation.z);
		glScalef(theObject->scale.x,theObject->scale.y,theObject->scale.z);	
		glTranslatef(-theObject->center.x,-theObject->center.y,-theObject->center.z);
				
		glDisable(GL_LIGHTING);
				glColor3ub(255,0,0);
				glPointSize(5.0f);
				glBegin(GL_POINTS);
				for(size_t i=0;i<selection.size();++i)
				{
					Vertex *theVertex=theObject->vertex(selection[i]);
					if(theVertex)
					{
						glVertex3f(theVertex->position.x,theVertex->position.y,theVertex->position.z);
					}
				}
				glEnd();
				glEnable(GL_LIGHTING);
				glPopMatrix();
			}
			else
			{
				return;
			}
		}
	};

	void drawSelectedEdge()
	{
		if(target && mode==SelectionMode::Edge)
		{
			Object *theObject=theObjectList[target];
			if(theObject)
			{
								glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
				glTranslatef(theObject->position.x,theObject->position.y,theObject->position.z);
		glTranslatef(theObject->center.x,theObject->center.y,theObject->center.z);
		glRotatef(theObject->rotation.w,theObject->rotation.x,theObject->rotation.y,theObject->rotation.z);
		glScalef(theObject->scale.x,theObject->scale.y,theObject->scale.z);	
		glTranslatef(-theObject->center.x,-theObject->center.y,-theObject->center.z);
			
		glDisable(GL_LIGHTING);
				glColor3ub(255,252,0);
				glBegin(GL_LINES);
				for(size_t i=0;i<selection.size();++i)
				{
					Edge *theEdge=theObject->edge(selection[i]);
					if(theEdge)
					{
						Vertex *start=theObject->vertex(theEdge->start);
						Vertex *end=theObject->vertex(theEdge->end);
						glVertex3f(start->position.x,start->position.y,start->position.z);
						glVertex3f(end->position.x,end->position.y,end->position.z);
					}
				}
				glEnd();
				glEnable(GL_LIGHTING);
				glPopMatrix();
			}
			else
			{
				return;
			}
		}
	};


void drawSelectedFace()
	{
		if(target && mode==SelectionMode::Face)
		{
			Object *theObject=theObjectList[target];
			if(theObject)
			{
								glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
				glTranslatef(theObject->position.x,theObject->position.y,theObject->position.z);
		glTranslatef(theObject->center.x,theObject->center.y,theObject->center.z);
		glRotatef(theObject->rotation.w,theObject->rotation.x,theObject->rotation.y,theObject->rotation.z);
		glScalef(theObject->scale.x,theObject->scale.y,theObject->scale.z);	
		glTranslatef(-theObject->center.x,-theObject->center.y,-theObject->center.z);
			
		glDisable(GL_LIGHTING);
				glEnable(GL_POLYGON_OFFSET_FILL);
				glPolygonOffset(1.0f,1.0f);
				//glEnable(GL_ALPHA_TEST);
				glColor3ub(48,92,235);
				for(size_t i=0;i<selection.size();++i)
				{
					Face *theFace=theObject->face(selection[i]);
					if(theFace)
					{
						glBegin(GL_POLYGON);
						for(size_t e=0;e<theFace->edge.size();++e)
						{
							if(theFace->edge[e]>0)
							{
								Vertex *v=theObject->vertex(theObject->edge(theFace->edge[e])->start);
								glVertex3f(v->position.x,v->position.y,v->position.z);
							}
							else
							{
								Vertex *v=theObject->vertex(theObject->edge(-theFace->edge[e])->end);
								glVertex3f(v->position.x,v->position.y,v->position.z);
							}
						}
						glEnd();
					}
				}
				//glDisable(GL_ALPHA_TEST);
				glDisable(GL_POLYGON_OFFSET_FILL);
				glEnable(GL_LIGHTING);
				glPopMatrix();
			}
			else
			{
				return;
			}
		}
	};

void openFromPWB(const char *fileName)
{
	clearScene();
	loadFromPWB(fileName);
}

void loadFromPWB(const char *fileName)
{
	FILE *fp=fopen(fileName,"rb");
	struct SceneInfo sceneInfo;
	fread(&sceneInfo,sizeof(sceneInfo),1,fp);
	for(size_t i=1;i<sceneInfo.objectCount;++i)
	{
		struct ObjectInfo objectInfo;
		fread(&objectInfo,sizeof(objectInfo),1,fp);
		if(objectInfo.index>0)
		{
			Object *theObject=new Object("new object");
			theObject->index=objectInfo.index;
			theObject->center.vec(objectInfo.centerX,objectInfo.centerY,objectInfo.centerZ);
			theObject->position.vec(objectInfo.positionX,objectInfo.positionY,objectInfo.positionZ);
			theObject->rotation.vec(objectInfo.rotationX,objectInfo.rotationY,objectInfo.rotationZ);
			theObject->scale.vec(objectInfo.scaleX,objectInfo.scaleY,objectInfo.scaleZ);
			theObject->mat_ambient[0]=objectInfo.mat_ambient[0];
			theObject->mat_ambient[1]=objectInfo.mat_ambient[1];
			theObject->mat_ambient[2]=objectInfo.mat_ambient[2];
			theObject->mat_ambient[3]=objectInfo.mat_ambient[3];

			theObject->mat_diffuse[0]=objectInfo.mat_diffuse[0];
			theObject->mat_diffuse[1]=objectInfo.mat_diffuse[1];
			theObject->mat_diffuse[2]=objectInfo.mat_diffuse[2];
			theObject->mat_diffuse[3]=objectInfo.mat_diffuse[3];

			theObject->mat_specular[0]=objectInfo.mat_specular[0];
			theObject->mat_specular[1]=objectInfo.mat_specular[1];
			theObject->mat_specular[2]=objectInfo.mat_specular[2];
			theObject->mat_specular[3]=objectInfo.mat_specular[3];

			theObject->mat_emission[0]=objectInfo.mat_emission[0];
			theObject->mat_emission[1]=objectInfo.mat_emission[1];
			theObject->mat_emission[2]=objectInfo.mat_emission[2];
			theObject->mat_emission[3]=objectInfo.mat_emission[3];

			theObject->mat_shininess[0]=objectInfo.mat_shininess[0];
			theObject->mat_shininess[1]=objectInfo.mat_shininess[1];
			theObject->mat_shininess[2]=objectInfo.mat_shininess[2];
			theObject->mat_shininess[3]=objectInfo.mat_shininess[3];

			for(size_t e=1;e<objectInfo.vertexCount;++e)
			{
				struct VertexInfo vertexInfo;
				fread(&vertexInfo,sizeof(vertexInfo),1,fp);
				if(vertexInfo.index>0)
				{
					Vertex *theVertex=new Vertex();
					theVertex->index=vertexInfo.index;
					theVertex->position.vec(vertexInfo.x,vertexInfo.y,vertexInfo.z);
					theVertex->normal.vec(vertexInfo.nx,vertexInfo.ny,vertexInfo.nz);
					for(size_t h=0;h<vertexInfo.adjacentCount;++h)
					{
						size_t theAdj;
						fread(&theAdj,sizeof(size_t),1,fp);
						theVertex->adjacentEdge.push_back(theAdj);
					}
					theObject->directPushVertex(theVertex);
				}
				else
				{
					theObject->directPushVertex(NULL);
				}
			}

			for(size_t e=1;e<objectInfo.edgeCount;++e)
			{
				struct EdgeInfo edgeInfo;
				fread(&edgeInfo,sizeof(edgeInfo),1,fp);
				if(edgeInfo.index>0)
				{
					Edge *theEdge=new Edge(edgeInfo.start,edgeInfo.end);
					theEdge->index=edgeInfo.index;
					theEdge->left=edgeInfo.left;
					theEdge->right=edgeInfo.right;
					theObject->directPushEdge(theEdge);
				}
				else
				{
					theObject->directPushEdge(NULL);
				}
			}

			for(size_t e=1;e<objectInfo.faceCount;++e)
			{
				struct FaceInfo faceInfo;
				fread(&faceInfo,sizeof(faceInfo),1,fp);
				if(faceInfo.index>0)
				{
					Face *theFace=new Face();					
					theFace->index=faceInfo.index;
					theFace->normal.vec(faceInfo.nx,faceInfo.ny,faceInfo.nz);
					for(size_t h=0;h<faceInfo.edgeCount;++h)
					{
						int theE;
						fread(&theE,sizeof(int),1,fp);
						theFace->edge.push_back(theE);
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

void invertSelection()
{
	if(mode==SelectionMode::Object)
	{
		std::vector<size_t> objectToBeSelected;
		objectToBeSelected.reserve(100);
		for(size_t i=1;i<theObjectList.size();++i)
		{
			if(theObjectList[i] && !(theObjectList[i]->isSelected))
			{
				objectToBeSelected.push_back(i);
			}
		}

		clearSelection();

		for(size_t i=0;i<objectToBeSelected.size();++i)
		{
			selectionPush(theObjectList[objectToBeSelected[i]]);
		}

		updateAxisCursor();
	}
};

void saveToFileOBJ(const char *fileName)
{
	FILE *fp=fopen(fileName,"w");
	size_t vertexBase=0;
	for(size_t h=1;h<theObjectList.size();++h)
	{
		Object *theObject=theObjectList[h];
		if(theObject)
		{
			size_t i=1;
			for(i=1;i<theObject->vertexCount();i++)
			{
				if(theObject->vertex(i)==NULL)
				{
					fprintf(fp,"v %f %f %f\n",0,0,0);
				}
				else
				{
					Vertex *theVertex=theObject->vertex(i);
					fprintf(fp,"v %f %f %f\n",theVertex->position.x,theVertex->position.y,theVertex->position.z);
				}
			}
			size_t tempBase=i-1;
			fprintf(fp,"g box01\n");
			for(size_t i=1;i<theObject->faceCount();i++)
			{
				Face *theFace=theObject->face(i);
				if(theFace)
				{
					fprintf(fp,"f ");
					for(size_t e=0;e<theFace->edge.size();e++)
					{
						if(theFace->edge[e]<0)
						{
							fprintf(fp,"%d ",theObject->edge(-theFace->edge[e])->end+vertexBase);
						}
						else
						{
							fprintf(fp,"%d ",theObject->edge(theFace->edge[e])->start+vertexBase);
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
};

void saveToFilePWB(const char *fileName)
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
	for(size_t i=1;i<sceneInfo.objectCount;++i)
	{
		Object *theObject=theObjectList[i];
		if(theObject)
		{
			struct ObjectInfo objectInfo=theObjectList[i]->getObjectInfo();
			fwrite(&objectInfo,sizeof(objectInfo),1,fp);
			for(size_t e=1;e<objectInfo.vertexCount;++e)
			{
				Vertex *theVertex=theObject->vertex(e);
				if(theVertex)
				{
					struct VertexInfo vertexInfo=theVertex->getVertexInfo();
					fwrite(&vertexInfo,sizeof(vertexInfo),1,fp);
					fwrite(&(theVertex->adjacentEdge[0]),sizeof(size_t),vertexInfo.adjacentCount,fp);
				}
				else
				{
					struct VertexInfo vertexInfo;
					vertexInfo.index=0;
					vertexInfo.adjacentCount=0;
					fwrite(&vertexInfo,sizeof(vertexInfo),1,fp);
				}
			}

			for(size_t e=1;e<objectInfo.edgeCount;++e)
			{
				Edge *theEdge=theObject->edge(e);
				if(theEdge)
				{
					struct EdgeInfo edgeInfo=theEdge->getEdgeInfo();
					fwrite(&edgeInfo,sizeof(edgeInfo),1,fp);
				}
				else
				{
					struct EdgeInfo edgeInfo;
					edgeInfo.index=0;
					fwrite(&edgeInfo,sizeof(edgeInfo),1,fp);
				}
			}

			for(size_t e=1;e<objectInfo.faceCount;++e)
			{
				Face *theFace=theObject->face(e);
				if(theFace)
				{
					struct FaceInfo faceInfo=theFace->getFaceInfo();
					fwrite(&faceInfo,sizeof(faceInfo),1,fp);
					fwrite(&(theFace->edge[0]),sizeof(size_t),faceInfo.edgeCount,fp);
				}
				else
				{
					struct FaceInfo faceInfo;
					faceInfo.index=0;
					fwrite(&faceInfo,sizeof(faceInfo),1,fp);
				}
			}
		}
		else
		{
			struct ObjectInfo objectInfo;
			objectInfo.index=0;
			objectInfo.vertexCount=0;
			objectInfo.edgeCount=0;
			objectInfo.faceCount=0;
			fwrite(&objectInfo,sizeof(objectInfo),1,fp);
		}
	}
	fclose(fp);
};
public:
//	void initialize();
	~Scene(void);
};

extern Scene *theScene;
