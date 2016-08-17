#include <Core/Scene.h>

void Scene::onPaint()
     {
         if(m_mode==SelectionMode::Object)
         {
             for(unsigned int i=1;i<m_objectList.size();++i)
             {
                 if(m_objectList[i])
                 {
                     if(m_objectList[i]->m_isSelected)
                     {

                             m_objectList[i]->drawObjectSelected();
                         m_objectList[i]->drawMirror();
                     }
                     else
                     {
                         m_objectList[i]->draw();
                         m_objectList[i]->drawMirror();
                     }
                 }
             }
         }
         else
         {
             for(unsigned int i=1;i<m_objectList.size();++i)
             {
                 if(m_objectList[i])
                 {
                     if(m_target!=i)
                     {
                         m_objectList[i]->draw();
                         m_objectList[i]->drawMirror();
                     }
                     else
                     {
                         if(m_mode==SelectionMode::Face)
                         {
                             if(m_objectList[m_target]->m_renderMode==RenderType::Wire)
                             {
                                 m_objectList[m_target]->drawWireFaceSelected();
                             }
                             else
                             {
                                 m_objectList[m_target]->drawFacedFaceSelected();
                             }
                             drawSelectedFace();
                             m_objectList[i]->drawMirror();
                         }
                         else if(m_mode==SelectionMode::Edge)
                         {
                             if(m_objectList[m_target]->m_renderMode==RenderType::Wire)
                             {
                                 m_objectList[m_target]->drawWireEdgeSelected();
                             }
                             else
                             {
                                 m_objectList[m_target]->drawFacedEdgeSelected();
                             }
                             drawSelectedEdge();
                             m_objectList[i]->drawMirror();
                         }
                         else if(m_mode==SelectionMode::Vertex)
                         {
                             if(m_objectList[m_target]->m_renderMode==RenderType::Wire)
                             {
                                 m_objectList[m_target]->drawWireVertexSelected();
                             }
                             else
                             {
                                 m_objectList[m_target]->drawFacedVertexSelected();
                             }
                             drawSelectedVertex();
                             m_objectList[i]->drawMirror();
                         }
                         else if(m_mode==SelectionMode::Split)
                         {
                             if(m_objectList[m_target]->m_renderMode==RenderType::Wire)
                             {
                                 m_objectList[m_target]->drawWireVertexSelected();
                             }
                             else
                             {
                                 m_objectList[m_target]->drawFacedVertexSelected();
                             }
                             drawCutVertex();
                             m_objectList[i]->drawMirror();
                         }
                     }
                 }
             }
         }
     }


     void Scene::drawCutVertex()
     {
         if(m_target && m_mode==SelectionMode::Split)
         {
             Object *object=m_objectList[m_target];
             if(object)
             {
                 /*glMatrixMode(GL_MODELVIEW);
                 glPushMatrix();
                 glTranslatef(object->m_position.x,object->m_position.y,object->m_position.z);
                 glTranslatef(object->m_center.x,object->m_center.y,object->m_center.z);
                 glRotatef(object->m_rotation.w,object->m_rotation.x,object->m_rotation.y,object->m_rotation.z);
                 glScalef(object->m_scale.x,object->m_scale.y,object->m_scale.z);
                 glTranslatef(-object->m_center.x,-object->m_center.y,-object->m_center.z);
                 glDisable(GL_LIGHTING);
                 glColor3ub(255,0,255);
                 glPointSize(8.0f);
                 glBegin(GL_POINTS);
                 Vertex *vertex=object->vertex(splitVertexID);
                 if(vertex)
                 {
                     glVertex3f(vertex->m_position.x,vertex->m_position.y,vertex->m_position.z);
                 }
                 glEnd();
                 glEnable(GL_LIGHTING);
                 glPopMatrix();*/
             }
         }
     }

     void Scene::updateAxisCursor()
     {
         m_axisCursor->m_rotation.null();
         if(m_mode==SelectionMode::Object && m_target==0)
         {
             if(m_selection.empty())
             {
                 m_axisCursor->m_mode=AxisCursorMode::NoAxis;
                 m_axisCursor->m_position.null();
             }
             else
             {
                 Vector *center=new Vector();
                 unsigned int selectionSize=m_selection.size();
                 for(unsigned int i=0;i<selectionSize;++i)
                 {
                     Object *object=m_objectList[m_selection[i]];
                     if(object)
                     (*center)+=object->m_center+object->m_position;
                 }
                 (*center)/=(float)selectionSize;
                 m_axisCursor->m_position=*center;
                 delete center;
                 m_axisCursor->m_mode=m_currentACMode;
             }
         }
         else if(m_mode==SelectionMode::Vertex && m_target)
         {
             if(m_selection.empty())
             {
                 m_axisCursor->m_mode=AxisCursorMode::NoAxis;
                 m_axisCursor->m_position.null();
             }
             else
             {
                 Vector *center=new Vector();
                 unsigned int selectionSize=m_selection.size();
                 Object *object=m_objectList[m_target];
                 for(unsigned int i=0;i<selectionSize;++i)
                 {
                     Vertex *vertex=object->vertex(m_selection[i]);
                     if(vertex)
                         (*center)+=vertex->m_position;
                 }
                 (*center)/=(float)selectionSize;
                 //ÕâÀïÐý×ª
                 Vector rotateAxis=Vector(object->m_rotation.x,object->m_rotation.y,object->m_rotation.z);
                 Quaternion rotateQuaternion(object->m_rotation.w,rotateAxis);
                 Matrix rotateMatrix;
                 rotateQuaternion.getMatrix(rotateMatrix);
                 *center=rotateMatrix*(*center-object->m_center);
                 //ÕâÀïÆ½ÒÆ
                 m_axisCursor->m_position=*center+object->m_position+object->m_center;
                 delete center;
                 m_axisCursor->m_mode=m_currentACMode;
             }
         }
         else if(m_mode==SelectionMode::Edge && m_target)
         {
             if(m_selection.empty())
             {
                 m_axisCursor->m_mode=AxisCursorMode::NoAxis;
                 m_axisCursor->m_position.null();
             }
             else
             {
                 Vector *center=new Vector();
                 unsigned int selectionSize=m_selection.size();
                 Object *object=m_objectList[m_target];
                 for(unsigned int i=0;i<selectionSize;++i)
                 {
                     Edge *edge=object->edge(m_selection[i]);
                     if(edge)
                     {
                         if(!object->vertex(edge->m_start)->m_isIn)
                         {
                             (*center)+=object->vertex(edge->m_start)->m_position;
                             object->vertex(edge->m_start)->m_isIn=true;
                             m_isInCache.push_back(object->vertex(edge->m_start));
                         }
                         if(!object->vertex(edge->m_end)->m_isIn)
                         {
                             (*center)+=object->vertex(edge->m_end)->m_position;
                             object->vertex(edge->m_end)->m_isIn=true;
                             m_isInCache.push_back(object->vertex(edge->m_end));
                         }
                     }
                 }
                 (*center)/=(float)m_isInCache.size();
                 clearIsInCache();
                 //ÕâÀïÐý×ª
                 Vector rotateAxis=Vector(object->m_rotation.x,object->m_rotation.y,object->m_rotation.z);
                 Quaternion rotateQuaternion(object->m_rotation.w,rotateAxis);
                 Matrix rotateMatrix;
                 rotateQuaternion.getMatrix(rotateMatrix);
                 *center=rotateMatrix*(*center-object->m_center);
                 //ÕâÀïÆ½ÒÆ
                 m_axisCursor->m_position=*center+object->m_position+object->m_center;
                 delete center;
                 m_axisCursor->m_mode=m_currentACMode;

             }
         }
         else if(m_mode==SelectionMode::Face && m_target)
         {
             if(m_selection.empty())
             {
                 m_axisCursor->m_mode=AxisCursorMode::NoAxis;
                 m_axisCursor->m_position.null();
             }
             else
             {
                 Vector *center=new Vector();
                 unsigned int selectionSize=m_selection.size();
                 Object *object=m_objectList[m_target];
                 for(unsigned int i=0;i<selectionSize;++i)
                 {
                     Face *face=object->face(m_selection[i]);
                     if(face)
                     {
                         unsigned int edgeCount=face->m_edge.size();
                         for(unsigned int e=0;e<edgeCount;++e)
                         {
                             if(face->m_edge[e]>0)
                             {
                                 Vertex *start=object->vertex(object->edge(face->m_edge[e])->m_start);
                                 if(!start->m_isIn)
                                 {
                                     (*center)+=start->m_position;
                                     start->m_isIn=true;
                                     m_isInCache.push_back(start);
                                 }
                             }
                             else
                             {
                                 Vertex *end=object->vertex(object->edge(-face->m_edge[e])->m_end);
                                 if(!end->m_isIn)
                                 {
                                     (*center)+=end->m_position;
                                     end->m_isIn=true;
                                     m_isInCache.push_back(end);
                                 }
                             }
                         }
                     }
                 }
                 (*center)/=(float)m_isInCache.size();
                 clearIsInCache();
                 //ÕâÀïÐý×ª
                 Vector rotateAxis=Vector(object->m_rotation.x,object->m_rotation.y,object->m_rotation.z);
                 Quaternion rotateQuaternion(object->m_rotation.w,rotateAxis);
                 Matrix rotateMatrix;
                 rotateQuaternion.getMatrix(rotateMatrix);
                 *center=rotateMatrix*(*center-object->m_center);
                 //ÕâÀïÆ½ÒÆ
                 m_axisCursor->m_position=*center+object->m_position+object->m_center;
                 delete center;
                 m_axisCursor->m_mode=m_currentACMode;
             }
         }
     }

     void Scene::drawCursor(CameraMode::__Enum cameraMode,const Vector &eye)
     {
         m_axisCursor->onPaint(eye,cameraMode);
     }

     void Scene::drawSelectedVertex()
     {
         if(m_target && m_mode==SelectionMode::Vertex)
         {
             Object *object=m_objectList[m_target];
             if(object)
             {
                                 /*glMatrixMode(GL_MODELVIEW);
         glPushMatrix();
                 glTranslatef(object->m_position.x,object->m_position.y,object->m_position.z);
         glTranslatef(object->m_center.x,object->m_center.y,object->m_center.z);
         glRotatef(object->m_rotation.w,object->m_rotation.x,object->m_rotation.y,object->m_rotation.z);
         glScalef(object->m_scale.x,object->m_scale.y,object->m_scale.z);
         glTranslatef(-object->m_center.x,-object->m_center.y,-object->m_center.z);

         glDisable(GL_LIGHTING);
                 glColor3ub(255,0,0);
                 glPointSize(5.0f);
                 glBegin(GL_POINTS);
                 for(unsigned int i=0;i<selection.size();++i)
                 {
                     Vertex *vertex=object->vertex(selection[i]);
                     if(vertex)
                     {
                         glVertex3f(vertex->m_position.x,vertex->m_position.y,vertex->m_position.z);
                     }
                 }
                 glEnd();
                 glEnable(GL_LIGHTING);
                 glPopMatrix();*/
             }
             else
             {
                 return;
             }
         }
     }

     void Scene::drawSelectedEdge()
     {
         if(m_target && m_mode==SelectionMode::Edge)
         {
             Object *object=m_objectList[m_target];
             if(object)
             {
                                 /*glMatrixMode(GL_MODELVIEW);
         glPushMatrix();
                 glTranslatef(object->m_position.x,object->m_position.y,object->m_position.z);
         glTranslatef(object->m_center.x,object->m_center.y,object->m_center.z);
         glRotatef(object->m_rotation.w,object->m_rotation.x,object->m_rotation.y,object->m_rotation.z);
         glScalef(object->m_scale.x,object->m_scale.y,object->m_scale.z);
         glTranslatef(-object->m_center.x,-object->m_center.y,-object->m_center.z);

         glDisable(GL_LIGHTING);
                 glColor3ub(255,252,0);
                 glBegin(GL_LINES);
                 for(unsigned int i=0;i<selection.size();++i)
                 {
                     Edge *edge=object->edge(selection[i]);
                     if(edge)
                     {
                         Vertex *start=object->vertex(edge->m_start);
                         Vertex *end=object->vertex(edge->m_end);
                         glVertex3f(start->m_position.x,start->m_position.y,start->m_position.z);
                         glVertex3f(end->m_position.x,end->m_position.y,end->m_position.z);
                     }
                 }
                 glEnd();
                 glEnable(GL_LIGHTING);
                 glPopMatrix();*/
             }
             else
             {
                 return;
             }
         }
     }


 void Scene::drawSelectedFace()
     {
         if(m_target && m_mode==SelectionMode::Face)
         {
             Object *object=m_objectList[m_target];
             if(object)
             {
                                 /*glMatrixMode(GL_MODELVIEW);
         glPushMatrix();
                 glTranslatef(object->m_position.x,object->m_position.y,object->m_position.z);
         glTranslatef(object->m_center.x,object->m_center.y,object->m_center.z);
         glRotatef(object->m_rotation.w,object->m_rotation.x,object->m_rotation.y,object->m_rotation.z);
         glScalef(object->m_scale.x,object->m_scale.y,object->m_scale.z);
         glTranslatef(-object->m_center.x,-object->m_center.y,-object->m_center.z);

         glDisable(GL_LIGHTING);
                 glEnable(GL_POLYGON_OFFSET_FILL);
                 glPolygonOffset(1.0f,1.0f);
                 //glEnable(GL_ALPHA_TEST);
                 glColor3ub(48,92,235);
                 for(unsigned int i=0;i<selection.size();++i)
                 {
                     Face *face=object->face(selection[i]);
                     if(face)
                     {
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
                 //glDisable(GL_ALPHA_TEST);
                 glDisable(GL_POLYGON_OFFSET_FILL);
                 glEnable(GL_LIGHTING);
                 glPopMatrix();*/
             }
             else
             {
                 return;
             }
         }
     }
