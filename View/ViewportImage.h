#pragma once
#ifdef __APPLE__
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#endif

#include "../Core/Vector.h"
#include "Enum.h"
#include <string>

DECLARE_ENUM(CameraMode)
    Front=1,
    Back,
    Top,
    Bottom,
    Left,
    Right,
    Perspective
END_ENUM()

class ViewportImage
{
public:
	bool isShow;
	std::string path;
	GLuint imageID;
	GLuint callID;
    unsigned int width;
    unsigned int height;
	CameraMode cameraMode;
	Vector position;
    ViewportImage(const char *thePath,GLuint theImageID,unsigned int width,unsigned int height,CameraMode::__Enum theCameraMode,Vector &thePosition);
	void initialize()
	{
		callID=glGenLists(1);
		glNewList(callID,GL_COMPILE);
		if(cameraMode==CameraMode::Front)
		{
			glBegin(GL_QUADS);
			glTexCoord2f(0,0);
			glVertex3f(position.x+width,position.y,position.z+height);
			glTexCoord2f(1,0);
			glVertex3f(position.x,position.y,position.z+height);
			glTexCoord2f(1,1);
			glVertex3f(position.x,position.y,position.z);
			glTexCoord2f(0,1);
			glVertex3f(position.x+width,position.y,position.z);
			glEnd();
		}
		else if(cameraMode==CameraMode::Back)
		{
			glBegin(GL_QUADS);
			glTexCoord2f(0,0);
			glVertex3f(position.x,position.y,position.z+height);
			glTexCoord2f(1,0);
			glVertex3f(position.x+width,position.y,position.z+height);
			glTexCoord2f(1,1);
			glVertex3f(position.x+width,position.y,position.z);
			glTexCoord2f(0,1);
			glVertex3f(position.x,position.y,position.z);
			glEnd();			
		}
		else if(cameraMode==CameraMode::Right)
		{
			glBegin(GL_QUADS);
			glTexCoord2f(0,0);
			glVertex3f(position.x,position.y,position.z+height);
			glTexCoord2f(1,0);
			glVertex3f(position.x,position.y+width,position.z+height);
			glTexCoord2f(1,1);
			glVertex3f(position.x,position.y+width,position.z);
			glTexCoord2f(0,1);
			glVertex3f(position.x,position.y,position.z);
			glEnd();
		}
		else if(cameraMode==CameraMode::Left)
		{
			glBegin(GL_QUADS);
			glTexCoord2f(0,0);
			glVertex3f(position.x,position.y+width,position.z+height);
			glTexCoord2f(1,0);
			glVertex3f(position.x,position.y,position.z+height);
			glTexCoord2f(1,1);
			glVertex3f(position.x,position.y,position.z);
			glTexCoord2f(0,1);
			glVertex3f(position.x,position.y+width,position.z);
			glEnd();
		}
		else if(cameraMode==CameraMode::Top)
		{
			glBegin(GL_QUADS);
			glTexCoord2f(0,0);
			glVertex3f(position.x,position.y+height,position.z);
			glTexCoord2f(1,0);
			glVertex3f(position.x+width,position.y+height,position.z);
			glTexCoord2f(1,1);
			glVertex3f(position.x+width,position.y,position.z);
			glTexCoord2f(0,1);
			glVertex3f(position.x,position.y,position.z);
			glEnd();			
		}
		else if(cameraMode==CameraMode::Bottom)
		{
			glBegin(GL_QUADS);
			glTexCoord2f(0,0);
			glVertex3f(position.x+width,position.y,position.z);
			glTexCoord2f(1,0);
			glVertex3f(position.x,position.y,position.z);
			glTexCoord2f(1,1);
			glVertex3f(position.x,position.y+height,position.z);
			glTexCoord2f(0,1);
			glVertex3f(position.x+width,position.y+height,position.z);
			glEnd();						
		}
		glEndList();
    }
	void onPaint()
	{
		glDisable(GL_LIGHTING);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D,imageID);
		glColor3ub(255,255,255);
		glCallList(callID);
		glEnable(GL_LIGHTING);
		glDisable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D,0);
    }
public:
	~ViewportImage(void);
};

extern ViewportImage *frontReference;
extern ViewportImage *backReference;
extern ViewportImage *topReference;
extern ViewportImage *bottomReference;
extern ViewportImage *rightReference;
extern ViewportImage *leftReference;
