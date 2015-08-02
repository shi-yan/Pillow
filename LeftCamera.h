#pragma once
#include "Camera.h"

class LeftCamera :
	public Camera
{
public:
	LeftCamera(float width,float height,float nearPlane,float farPlane,float startX,float startY,Vector target,Vector eye,Vector up);
	void setCamera()
	{
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		gluLookAt(-990,eye.y,eye.z,target.x,target.y,target.z,up.x,up.y,up.z);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		//gluPerspective(this->angle,this->width/this->height,this->nearPlane,this->farPlane);
		glOrtho(-width*(eye.x)*0.005f,width*(eye.x)*0.005f,-height*(eye.x)*0.005f,height*(eye.x)*0.005f,nearPlane,farPlane);
		glViewport((GLint)startX,(GLint)startY,(GLint)width,(GLint)height);
				drawGird();
				if(leftReference && leftReference->isShow)
		{
			leftReference->onPaint();
		}
		glEnable(GL_LIGHT0);
		glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
		glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
		glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
		glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	};

							void setReferenceImage(const char *path,GLuint imageID,Vector &position,size_t width,size_t height)
	{
			if(leftReference)
			{
				delete leftReference;
			}
		
			leftReference=new ViewportImage(path,imageID,width,height,type._value,position);
			leftReference->initialize();
			leftReference->isShow=true;
	};

	void getViewportImage(GLuint &texID,std::string &path,Vector &position,size_t &width,size_t &height)
	{
		if(leftReference)
		{
			texID=leftReference->imageID;
			path=leftReference->path;
			position=leftReference->position;
			width=leftReference->width;
			height=leftReference->height;
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
		leftReference->isShow=false;
	}

		void drawGird()
	{
		if(showGird)
		{
			theGird->drawYZ();
		}
	};

			void zoom(float step)
	{
		Vector cameraDirection(-1,0,0);
		
		//cameraDirection.normalize();
		//this->target+=cameraDirection*step;
		eye+=cameraDirection*(float)(step*0.1f);
		if(eye.x<2) eye.x=2;
	};

	void setCameraForSelectionS()
	{
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		gluLookAt(-990,eye.y,eye.z,target.x,target.y,target.z,up.x,up.y,up.z);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		//gluPerspective(this->angle,this->width/this->height,this->nearPlane,this->farPlane);
		glOrtho(-width*(eye.x)*0.005f,width*(eye.x)*0.005f,-height*(eye.x)*0.005f,height*(eye.x)*0.005f,nearPlane,farPlane);
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
		glOrtho(-width*(eye.x)*0.005f,width*(eye.x)*0.005f,-height*(eye.x)*0.005f,height*(eye.x)*0.005f,nearPlane,farPlane+100);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		gluLookAt(-990,eye.y,eye.z,target.x,target.y,target.z,up.x,up.y,up.z);
	};
		Vector getEye()
	{
		return eye*10.0f;
	}
			Vector getHorizontalDir()
	{
		return Vector(0,1,0);
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
		float dy=(float)(x-old.x)*eye.x*0.01f;
		float dz=(float)(y-old.y)*eye.x*0.01f;
		old.x=(float)x;
		old.y=(float)y;
		eye.y+=dy;
		eye.z+=dz;
		target.y+=dy;
		target.z+=dz;
	};
public:
	~LeftCamera(void);
};
