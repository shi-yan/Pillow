#pragma once
#include "Enum.h"
#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include "vector.h"
#include "Camera.h"

DECLARE_ENUM(AxisCursorMode)
	MoveAxis=982,
	RotateAxis,
	ScaleAxis,
	NoAxis
END_ENUM()

class AxisCursor
{
GLuint moveX;
GLuint moveY;
GLuint moveZ;
GLuint rotateX;
GLuint rotateY;
GLuint rotateZ;
GLuint scaleX;
GLuint scaleY;
GLuint scaleZ;
GLuint scaleC;
public:
Vector position;
Vector rotation;
AxisCursorMode mode;


public:
	AxisCursor(void):mode(AxisCursorMode::NoAxis),position(),rotation()
	{
	};

	void initialize()
	{
		moveX=glGenLists(1);
		moveY=glGenLists(1);
		moveZ=glGenLists(1);

		rotateX=glGenLists(1);
		rotateY=glGenLists(1);
		rotateZ=glGenLists(1);

		scaleX=glGenLists(1);
		scaleY=glGenLists(1);
		scaleZ=glGenLists(1);
		scaleC=glGenLists(1);

		glNewList(moveX,GL_COMPILE);
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

		glNewList(moveY,GL_COMPILE);
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

		glNewList(moveZ,GL_COMPILE);
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

		glNewList(rotateX,GL_COMPILE);
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

		glNewList(rotateY,GL_COMPILE);
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

		glNewList(rotateZ,GL_COMPILE);
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

		glNewList(scaleX,GL_COMPILE);
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

		glNewList(scaleY,GL_COMPILE);
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

		glNewList(scaleZ,GL_COMPILE);
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

		glNewList(scaleC,GL_COMPILE);
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
		glEndList();
	};

	void onPaint(const Vector &eye,CameraMode::__Enum cameraMode)
	{
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_LIGHTING);
		float scaleFactor=position.distto(eye)*0.004f;
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glTranslatef(position.x,position.y,position.z);
		glRotatef(rotation.w,rotation.x,rotation.y,rotation.z);
		glScalef(scaleFactor,scaleFactor,scaleFactor);
		if(mode==AxisCursorMode::MoveAxis)
		{
			if(cameraMode==CameraMode::Perspective)
			{
				glCallList(moveX);
				glCallList(moveY);
				glCallList(moveZ);
			}
			else if(cameraMode==CameraMode::Front)
			{
				glCallList(moveX);
				glCallList(moveZ);
			}
			else if(cameraMode==CameraMode::Back)
			{
				glCallList(moveX);
				glCallList(moveZ);
			}
			else if(cameraMode==CameraMode::Top)
			{
				glCallList(moveX);
				glCallList(moveY);
			}
			else if(cameraMode==CameraMode::Bottom)
			{
				glCallList(moveX);
				glCallList(moveY);
			}
			else if(cameraMode==CameraMode::Left)
			{
				glCallList(moveY);
				glCallList(moveZ);				
			}
			else if(cameraMode==CameraMode::Right)
			{
				glCallList(moveY);
				glCallList(moveZ);
			}

		}
		else if(mode==AxisCursorMode::RotateAxis)
		 {
			if(cameraMode==CameraMode::Perspective)
			{
				glCallList(rotateX);
				glCallList(rotateY);
				glCallList(rotateZ);
			}
			else if(cameraMode==CameraMode::Front)
			{
				glCallList(rotateX);
				glCallList(rotateZ);
			}
			else if(cameraMode==CameraMode::Back)
			{
				glCallList(rotateX);
				glCallList(rotateZ);
			}
			else if(cameraMode==CameraMode::Top)
			{
				glCallList(rotateX);
				glCallList(rotateY);
			}
			else if(cameraMode==CameraMode::Bottom)
			{
				glCallList(rotateX);
				glCallList(rotateY);
			}
			else if(cameraMode==CameraMode::Left)
			{
				glCallList(rotateY);
				glCallList(rotateZ);				
			}
			else if(cameraMode==CameraMode::Right)
			{
				glCallList(rotateY);
				glCallList(rotateZ);
			}
		 }
		 else if(mode==AxisCursorMode::ScaleAxis)
		 {
			if(cameraMode==CameraMode::Perspective)
			{
				glCallList(scaleX);
				glCallList(scaleY);
				glCallList(scaleZ);
			}
			else if(cameraMode==CameraMode::Front)
			{
				glCallList(scaleX);
				glCallList(scaleZ);
			}
			else if(cameraMode==CameraMode::Back)
			{
				glCallList(scaleX);
				glCallList(scaleZ);
			}
			else if(cameraMode==CameraMode::Top)
			{
				glCallList(scaleX);
				glCallList(scaleY);
			}
			else if(cameraMode==CameraMode::Bottom)
			{
				glCallList(scaleX);
				glCallList(scaleY);
			}
			else if(cameraMode==CameraMode::Left)
			{
				glCallList(scaleY);
				glCallList(scaleZ);				
			}
			else if(cameraMode==CameraMode::Right)
			{
				glCallList(scaleY);
				glCallList(scaleZ);
			}
		 }
		 glPopMatrix();
		 glEnable(GL_LIGHTING);
		 glEnable(GL_DEPTH_TEST);
	};

public:
	~AxisCursor(void);
};
