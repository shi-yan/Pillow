#pragma once
#include "Enum.h"
#ifdef __APPLE__
#include <OpenGL/gl3.h>
#include <OpenGL/gl3ext.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#endif

#include "../Core/Vector.h"
#include "Camera/Camera.h"

DECLARE_ENUM(AxisCursorMode)
    MoveAxis=982,
    RotateAxis,
    ScaleAxis,
    NoAxis
END_ENUM()

class AxisCursor
{
    GLuint m_moveX;
    GLuint m_moveY;
    GLuint m_moveZ;
    GLuint m_rotateX;
    GLuint m_rotateY;
    GLuint m_rotateZ;
    GLuint m_scaleX;
    GLuint m_scaleY;
    GLuint m_scaleZ;
    GLuint m_scaleC;

public:
    Vector m_position;
    Vector m_rotation;
    AxisCursorMode m_mode;

public:
    AxisCursor(void)
        :m_mode(AxisCursorMode::NoAxis),
          m_position(),
          m_rotation()
    {
    }

    void initialize()
    {
        /*m_moveX=glGenLists(1);
        m_moveY=glGenLists(1);
        m_moveZ=glGenLists(1);

        m_rotateX=glGenLists(1);
        m_rotateY=glGenLists(1);
        m_rotateZ=glGenLists(1);

        m_scaleX=glGenLists(1);
        m_scaleY=glGenLists(1);
        m_scaleZ=glGenLists(1);
        m_scaleC=glGenLists(1);*/

      /*  glNewList(m_moveX,GL_COMPILE);
        glColor3ub(255,0,0);
        glBegin(GL_TRIANGLES);
        glVertex3i(20,0,0);
        glVertex3i(10,1,1);
        glVertex3i(10,1,-1);
        glVertex3i(20,0,0);
        glVertex3i(10,-1,1);
        glVertex3i(10,1,1);
        glVertex3i(20,0,0);
        glVertex3i(10,-1,-1);
        glVertex3i(10,-1,1);
        glVertex3i(20,0,0);
        glVertex3i(10,-1,-1);
        glVertex3i(10,-1,1);
        glVertex3i(20,0,0);
        glVertex3i(10,1,-1);
        glVertex3i(10,-1,-1);
        glEnd();
        glBegin(GL_QUADS);
        glVertex3i(10,-1,-1);
        glVertex3i(10,1,-1);
        glVertex3i(10,1,1);
        glVertex3i(10,-1,1);
        glEnd();
        glBegin(GL_LINES);
        glVertex3i(0,0,0);
        glVertex3i(10,0,0);
        glEnd();
        glEndList();

        glNewList(m_moveY,GL_COMPILE);
        glColor3ub(0,0,255);
        glBegin(GL_TRIANGLES);
        glVertex3i(0,20,0);
        glVertex3i(1,10,-1);
        glVertex3i(1,10,1);

        glVertex3i(0,20,0);
        glVertex3i(1,10,1);
        glVertex3i(-1,10,1);

        glVertex3i(0,20,0);
        glVertex3i(-1,10,1);
        glVertex3i(-1,10,-1);

        glVertex3i(0,20,0);
        glVertex3i(-1,10,-1);
        glVertex3i(1,10,-1);
        glEnd();
        glBegin(GL_QUADS);
        glVertex3i(-1,10,1);
        glVertex3i(1,10,1);
        glVertex3i(1,10,-1);
        glVertex3i(-1,10,-1);

        glEnd();
        glBegin(GL_LINES);
        glVertex3i(0,0,0);
        glVertex3i(0,10,0);
        glEnd();
        glEndList();

        glNewList(m_moveZ,GL_COMPILE);
        glColor3ub(255,252,0);
        glBegin(GL_QUADS);
        glVertex3i(0,0,20);
        glVertex3i(1,1,10);
        glVertex3i(1,-1,10);
        glVertex3i(0,0,20);
        glVertex3i(-1,1,10);
        glVertex3i(1,1,10);
        glVertex3i(0,0,20);
        glVertex3i(-1,-1,10);
        glVertex3i(-1,1,10);
        glVertex3i(0,0,20);
        glVertex3i(1,-1,10);
        glVertex3i(-1,-1,10);
        glVertex3i(1,1,10);
        glVertex3i(-1,1,10);
        glVertex3i(-1,-1,10);
        glVertex3i(1,-1,10);
        glEnd();
        glBegin(GL_LINES);
        glVertex3i(0,0,0);
        glVertex3i(0,0,10);
        glEnd();
        glEndList();

        glNewList(m_rotateX,GL_COMPILE);
        glColor3ub(255,0,0);
        glBegin(GL_TRIANGLES);
        glVertex3i(10,0,0);
        glVertex3i(12,1,1);
        glVertex3i(12,-1,1);
        glVertex3i(10,0,0);
        glVertex3i(12,1,1);
        glVertex3i(12,1,-1);
        glVertex3i(10,0,0);
        glVertex3i(12,1,-1);
        glVertex3i(12,-1,-1);
        glVertex3i(14,0,0);
        glVertex3i(12,-1,1);
        glVertex3i(12,1,1);
        glVertex3i(14,0,0);
        glVertex3i(12,1,1);
        glVertex3i(12,1,-1);
        glVertex3i(14,0,0);
        glVertex3i(12,1,-1);
        glVertex3i(12,-1,-1);
        glVertex3i(14,0,0);
        glVertex3i(12,-1,-1);
        glVertex3i(12,-1,1);
        glEnd();
        glBegin(GL_LINES);
        glVertex3i(0,0,0);
        glVertex3i(10,0,0);
        glEnd();
        glEndList();

        glNewList(m_rotateY,GL_COMPILE);
        glColor3ub(0,0,255);
        glBegin(GL_TRIANGLES);
        glVertex3i(0,10,0);
        glVertex3i(1,12,1);
        glVertex3i(-1,12,1);
        glVertex3i(0,10,0);
        glVertex3i(1,12,1);
        glVertex3i(1,12,-1);
        glVertex3i(0,10,0);
        glVertex3i(1,12,-1);
        glVertex3i(-1,12,-1);
        glVertex3i(0,14,0);
        glVertex3i(-1,12,1);
        glVertex3i(1,12,1);
        glVertex3i(0,14,0);
        glVertex3i(1,12,1);
        glVertex3i(1,12,-1);
        glVertex3i(0,14,0);
        glVertex3i(1,12,-1);
        glVertex3i(-1,12,-1);
        glVertex3i(0,14,0);
        glVertex3i(-1,12,-1);
        glVertex3i(-1,12,1);
        glEnd();
        glBegin(GL_LINES);
        glVertex3i(0,0,0);
        glVertex3i(0,10,0);
        glEnd();
        glEndList();

        glNewList(m_rotateZ,GL_COMPILE);
        glColor3ub(255,252,0);
        glBegin(GL_TRIANGLES);
        glVertex3i(0,0,10);
        glVertex3i(1,1,12);
        glVertex3i(-1,1,12);
        glVertex3i(0,0,10);
        glVertex3i(1,1,12);
        glVertex3i(1,-1,12);
        glVertex3i(0,0,10);
        glVertex3i(1,-1,12);
        glVertex3i(-1,-1,12);
        glVertex3i(0,0,14);
        glVertex3i(-1,1,12);
        glVertex3i(1,1,12);
        glVertex3i(0,0,14);
        glVertex3i(1,1,12);
        glVertex3i(1,-1,12);
        glVertex3i(0,0,14);
        glVertex3i(1,-1,12);
        glVertex3i(-1,-1,12);
        glVertex3i(0,0,14);
        glVertex3i(-1,-1,12);
        glVertex3i(-1,1,12);
        glEnd();
        glBegin(GL_LINES);
        glVertex3i(0,0,0);
        glVertex3i(0,0,10);
        glEnd();
        glEndList();

        glNewList(m_scaleX,GL_COMPILE);
        glColor3ub(255,0,0);
        glBegin(GL_QUADS);
        glVertex3i(10,-1,-1);
        glVertex3i(10,-1,1);
        glVertex3i(10,1,1);
        glVertex3i(10,1,-1);
        glVertex3i(10,-1,1);
        glVertex3i(10,1,1);
        glVertex3i(12,1,1);
        glVertex3i(12,-1,1);
        glVertex3i(12,1,1);
        glVertex3i(10,1,1);
        glVertex3i(10,1,-1);
        glVertex3i(12,1,-1);
        glVertex3i(12,1,-1);
        glVertex3i(10,1,-1);
        glVertex3i(10,-1,-1);
        glVertex3i(12,-1,-1);
        glVertex3i(12,-1,-1);
        glVertex3i(10,-1,-1);
        glVertex3i(10,-1,1);
        glVertex3i(12,-1,1);
        glVertex3i(12,-1,1);
        glVertex3i(12,1,1);
        glVertex3i(12,1,-1);
        glVertex3i(12,-1,-1);
        glEnd();
        glBegin(GL_LINES);
        glVertex3i(0,0,0);
        glVertex3i(10,0,0);
        glEnd();
        glEndList();

        glNewList(m_scaleY,GL_COMPILE);
        glColor3ub(0,0,255);
        glBegin(GL_QUADS);
        glVertex3i(-1,10,-1);
        glVertex3i(-1,10,1);
        glVertex3i(1,10,1);
        glVertex3i(1,10,-1);
        glVertex3i(-1,10,1);
        glVertex3i(1,10,1);
        glVertex3i(1,12,1);
        glVertex3i(-1,12,1);
        glVertex3i(1,12,1);
        glVertex3i(1,10,1);
        glVertex3i(1,10,-1);
        glVertex3i(1,12,-1);
        glVertex3i(1,12,-1);
        glVertex3i(1,10,-1);
        glVertex3i(-1,10,-1);
        glVertex3i(-1,12,-1);
        glVertex3i(-1,12,-1);
        glVertex3i(-1,10,-1);
        glVertex3i(-1,10,1);
        glVertex3i(-1,12,1);
        glVertex3i(-1,12,1);
        glVertex3i(1,12,1);
        glVertex3i(1,12,-1);
        glVertex3i(-1,12,-1);
        glEnd();
        glBegin(GL_LINES);
        glVertex3i(0,0,0);
        glVertex3i(0,10,0);
        glEnd();
        glEndList();

        glNewList(m_scaleZ,GL_COMPILE);
        glColor3ub(255,252,0);
        glBegin(GL_QUADS);
        glVertex3i(-1,-1,10);
        glVertex3i(-1,1,10);
        glVertex3i(1,1,10);
        glVertex3i(1,-1,10);
        glVertex3i(-1,1,10);
        glVertex3i(1,1,10);
        glVertex3i(1,1,12);
        glVertex3i(-1,1,12);
        glVertex3i(1,1,12);
        glVertex3i(1,1,10);
        glVertex3i(1,-1,10);
        glVertex3i(1,-1,12);
        glVertex3i(1,-1,12);
        glVertex3i(1,-1,10);
        glVertex3i(-1,-1,10);
        glVertex3i(-1,-1,12);
        glVertex3i(-1,-1,12);
        glVertex3i(-1,-1,10);
        glVertex3i(-1,1,10);
        glVertex3i(-1,1,12);
        glVertex3i(-1,1,12);
        glVertex3i(1,1,12);
        glVertex3i(1,-1,12);
        glVertex3i(-1,-1,12);
        glEnd();
        glBegin(GL_LINES);
        glVertex3i(0,0,0);
        glVertex3i(0,0,10);
        glEnd();
        glEndList();

        glNewList(m_scaleC,GL_COMPILE);
        glColor3ub(0,252,0);
        glBegin(GL_QUADS);
        glVertex3i(1,-1,1);
        glVertex3i(1,1,1);
        glVertex3i(1,1,-1);
        glVertex3i(1,-1,-1);
        glVertex3i(-1,-1,-1);
        glVertex3i(-1,1,-1);
        glVertex3i(-1,1,1);
        glVertex3i(-1,-1,1);
        glVertex3i(-1,1,1);
        glVertex3i(1,1,1);
        glVertex3i(1,1,-1);
        glVertex3i(-1,1,-1);
        glVertex3i(-1,-1,-1);
        glVertex3i(1,-1,-1);
        glVertex3i(1,-1,1);
        glVertex3i(-1,-1,1);
        glVertex3i(-1,1,1);
        glVertex3i(1,1,1);
        glVertex3i(1,-1,1);
        glVertex3i(-1,-1,1);
        glVertex3i(-1,-1,-1);
        glVertex3i(1,-1,-1);
        glVertex3i(1,1,-1);
        glVertex3i(-1,1,-1);
        glEnd();
        glEndList();*/
    };

    void onPaint(const Vector &eye,CameraMode::__Enum cameraMode)
    {
     /*   glDisable(GL_DEPTH_TEST);
        glDisable(GL_LIGHTING);
        float scaleFactor=m_position.distto(eye)*0.004f;
        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glTranslatef(m_position.x,m_position.y,m_position.z);
        glRotatef(m_rotation.w,m_rotation.x,m_rotation.y,m_rotation.z);
        glScalef(scaleFactor,scaleFactor,scaleFactor);
        if(m_mode==AxisCursorMode::MoveAxis)
        {
            if(cameraMode==CameraMode::Perspective)
            {
                glCallList(m_moveX);
                glCallList(m_moveY);
                glCallList(m_moveZ);
            }
            else if(cameraMode==CameraMode::Front)
            {
                glCallList(m_moveX);
                glCallList(m_moveZ);
            }
            else if(cameraMode==CameraMode::Back)
            {
                glCallList(m_moveX);
                glCallList(m_moveZ);
            }
            else if(cameraMode==CameraMode::Top)
            {
                glCallList(m_moveX);
                glCallList(m_moveY);
            }
            else if(cameraMode==CameraMode::Bottom)
            {
                glCallList(m_moveX);
                glCallList(m_moveY);
            }
            else if(cameraMode==CameraMode::Left)
            {
                glCallList(m_moveY);
                glCallList(m_moveZ);
            }
            else if(cameraMode==CameraMode::Right)
            {
                glCallList(m_moveY);
                glCallList(m_moveZ);
            }

        }
        else if(m_mode==AxisCursorMode::RotateAxis)
         {
            if(cameraMode==CameraMode::Perspective)
            {
                glCallList(m_rotateX);
                glCallList(m_rotateY);
                glCallList(m_rotateZ);
            }
            else if(cameraMode==CameraMode::Front)
            {
                glCallList(m_rotateX);
                glCallList(m_rotateZ);
            }
            else if(cameraMode==CameraMode::Back)
            {
                glCallList(m_rotateX);
                glCallList(m_rotateZ);
            }
            else if(cameraMode==CameraMode::Top)
            {
                glCallList(m_rotateX);
                glCallList(m_rotateY);
            }
            else if(cameraMode==CameraMode::Bottom)
            {
                glCallList(m_rotateX);
                glCallList(m_rotateY);
            }
            else if(cameraMode==CameraMode::Left)
            {
                glCallList(m_rotateY);
                glCallList(m_rotateZ);
            }
            else if(cameraMode==CameraMode::Right)
            {
                glCallList(m_rotateY);
                glCallList(m_rotateZ);
            }
         }
         else if(m_mode==AxisCursorMode::ScaleAxis)
         {
            if(cameraMode==CameraMode::Perspective)
            {
                glCallList(m_scaleX);
                glCallList(m_scaleY);
                glCallList(m_scaleZ);
            }
            else if(cameraMode==CameraMode::Front)
            {
                glCallList(m_scaleX);
                glCallList(m_scaleZ);
            }
            else if(cameraMode==CameraMode::Back)
            {
                glCallList(m_scaleX);
                glCallList(m_scaleZ);
            }
            else if(cameraMode==CameraMode::Top)
            {
                glCallList(m_scaleX);
                glCallList(m_scaleY);
            }
            else if(cameraMode==CameraMode::Bottom)
            {
                glCallList(m_scaleX);
                glCallList(m_scaleY);
            }
            else if(cameraMode==CameraMode::Left)
            {
                glCallList(m_scaleY);
                glCallList(m_scaleZ);
            }
            else if(cameraMode==CameraMode::Right)
            {
                glCallList(m_scaleY);
                glCallList(m_scaleZ);
            }
         }
         glPopMatrix();
         glEnable(GL_LIGHTING);
         glEnable(GL_DEPTH_TEST);*/
    }

public:
    ~AxisCursor(void);
};
