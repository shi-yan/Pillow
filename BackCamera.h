#pragma once
#include "Camera.h"
#include <iostream>
#include <GL/glu.h>

class BackCamera :
	public Camera
{
public:
	BackCamera(float width,float height,float nearPlane,float farPlane,float startX,float startY,Vector target,Vector eye,Vector up);

	void setCamera()
	{
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		gluLookAt(eye.x,-990,eye.z,target.x,target.y,target.z,up.x,up.y,up.z);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		//gluPerspective(this->angle,this->width/this->height,this->nearPlane,this->farPlane);
		glOrtho(-width*(eye.y)*0.005f,width*(eye.y)*0.005f,-height*(eye.y)*0.005f,height*(eye.y)*0.005f,nearPlane,farPlane);
		glViewport((GLint)startX,(GLint)startY,(GLint)width,(GLint)height);
				drawGird();
				if(backReference && backReference->isShow)
		{
			backReference->onPaint();
		}
		glEnable(GL_LIGHT0);
		glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
		glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
		glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
		glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	};

	void setCameraForSelectionS()
	{
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		gluLookAt(eye.x,-990,eye.z,target.x,target.y,target.z,up.x,up.y,up.z);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(-width*(eye.y)*0.005f,width*(eye.y)*0.005f,-height*(eye.y)*0.005f,height*(eye.y)*0.005f,nearPlane,farPlane);
		glViewport((GLint)startX,(GLint)startY,(GLint)width,(GLint)height);
	};

	void setCameraForSelectionD(size_t x1,size_t y1,size_t x2,size_t y2,size_t h)
	{
		glViewport((GLint)startX,(GLint)startY,(GLint)width,(GLint)height);
		GLint viewport[4];
		glGetIntegerv (GL_VIEWPORT, viewport);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		size_t sw=(x2-x1)>5?(x2-x1):5;
		size_t sh=(y2-y1)>5?(y2-y1):5;
		gluPickMatrix((GLdouble) (x1+x2)/2,(GLdouble) (h - (y1+y2)/2), (GLdouble)sw,(GLdouble)sh, viewport);
		glOrtho(-width*(eye.y)*0.005f,width*(eye.y)*0.005f,-height*(eye.y)*0.005f,height*(eye.y)*0.005f,nearPlane,farPlane+100);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		gluLookAt(eye.x,-990,eye.z,target.x,target.y,target.z,up.x,up.y,up.z);
	};

		void setReferenceImage(const char *path,GLuint imageID,Vector &position,size_t width,size_t height)
	{
			if(backReference)
			{
				delete backReference;
			}
		
			backReference=new ViewportImage(path,imageID,width,height,type._value,position);
			backReference->initialize();
			backReference->isShow=true;
	};

	void getViewportImage(GLuint &texID,std::string &path,Vector &position,size_t &width,size_t &height)
	{
		if(backReference)
		{
			texID=backReference->imageID;
			path=backReference->path;
			position=backReference->position;
			width=backReference->width;
			height=backReference->height;
		}
		else
		{
			texID=0;
			path="";
			position.vec(0);
			width=0;
			height=0;
		}
	};

		void disableReference()
	{
		backReference->isShow=false;
	}

		Vector getHorizontalDir()
	{
		return Vector(-1,0,0);
	};
		Vector getEye()
	{
		return eye*10.0f;
	}


			void zoom(float step)
	{
		Vector cameraDirection(0,-1,0);
		
		//cameraDirection.normalize();
		//this->target+=cameraDirection*step;
		eye+=cameraDirection*(float)(step*0.1f);
		if(eye.y<2) eye.y=2;
	};

	void drawGird()
	{
		if(showGird)
		{
			theGird->drawXZ();
		}
	};
		
	void onPanPress(int x,int y)
	{
		this->isDraging=true;
		this->old.x=(float)x;
		this->old.y=(float)y;
	};

	void onPanRelease(int x,int y)
	{
		x;
		y;
		isDraging=false;
		old.null();
	};

	void pan(int x,int y)
	{
		float dx=(float)(old.x-x)*eye.y*0.01f;
		float dz=(float)(y-old.y)*eye.y*0.01f;
		old.x=(float)x;
		old.y=(float)y;
		//dx=10;
		eye.x+=dx;
		eye.z+=dz;
		target.x+=dx;
		target.z+=dz;
	};

public:
	~BackCamera(void);
};
