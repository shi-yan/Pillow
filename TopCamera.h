#pragma once
#include "Camera.h"

class TopCamera :
	public Camera
{
//private:float scale;
public:
	TopCamera(float width,float height,float nearPlane,float farPlane,float startX,float startY,Vector target,Vector eye,Vector up);

	void setCamera()
	{
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		gluLookAt(this->eye.x,this->eye.y,990,this->target.x,this->target.y,this->target.z,this->up.x,this->up.y,this->up.z);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(-width*eye.z*0.005f,width*eye.z*0.005f,-height*eye.z*0.005f,height*eye.z*0.005f,nearPlane,farPlane);
		glViewport((GLint)this->startX,(GLint)this->startY,(GLint)this->width,(GLint)this->height);
		drawGird();
		if(topReference && topReference->isShow)
		{
			topReference->onPaint();
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
		gluLookAt(this->eye.x,this->eye.y,990,this->target.x,this->target.y,this->target.z,this->up.x,this->up.y,this->up.z);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(-width*eye.z*0.005f,width*eye.z*0.005f,-height*eye.z*0.005f,height*eye.z*0.005f,nearPlane,farPlane);
		glViewport((GLint)this->startX,(GLint)this->startY,(GLint)this->width,(GLint)this->height);
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
		glOrtho(-width*eye.z*0.005f,width*eye.z*0.005f,-height*eye.z*0.005f,height*eye.z*0.005f,nearPlane,farPlane);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		gluLookAt(eye.x,eye.y,990,target.x,target.y,target.z,up.x,up.y,up.z);
	};

	void setReferenceImage(const char *path,GLuint imageID,Vector &position,size_t width,size_t height)
	{
			if(topReference)
			{
				delete topReference;
			}
		
			topReference=new ViewportImage(path,imageID,width,height,type._value,position);
			topReference->initialize();
			topReference->isShow=true;
	};

	void getViewportImage(GLuint &texID,std::string &path,Vector &position,size_t &width,size_t &height)
	{
		if(topReference)
		{
			texID=topReference->imageID;
			path=topReference->path;
			position=topReference->position;
			width=topReference->width;
			height=topReference->height;
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
		topReference->isShow=false;
	}


		Vector getEye()
	{
		return eye*10.0f;
	}
			Vector getHorizontalDir()
	{
		return Vector(-1,0,0);
	};
	void drawGird()
	{
		if(showGird)
		{
			theGird->drawXY();
		}
	};

	void zoom(float step)
	{
		Vector cameraDirection(0,0,-1);
		this->eye+=cameraDirection*(float)(step*0.1f);
		if(this->eye.z<2) this->eye.z=2;
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
		float dx=(float)(old.x-x)*this->eye.z*0.005f;
		float dy=(float)(y-old.y)*this->eye.z*0.005f;
		old.x=(float)x;
		old.y=(float)y;
		
		this->eye.x+=dx;
		this->eye.y+=dy;
		this->target.x+=dx;
		this->target.y+=dy;
	};


public:
	~TopCamera(void);
};
