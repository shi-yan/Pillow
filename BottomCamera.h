#pragma once
#include "camera.h"

class BottomCamera :
	public Camera
{
public:
	BottomCamera(float width,float height,float nearPlane,float farPlane,float startX,float startY,Vector target,Vector eye,Vector up);
			void setCamera()
	{
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		gluLookAt(eye.x,eye.y,-990,target.x,target.y,target.z,up.x,up.y,up.z);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		//gluPerspective(this->angle,this->width/this->height,this->nearPlane,this->farPlane);
		glOrtho(-width*(eye.z)*0.005f,width*(eye.z)*0.005f,-height*(eye.z)*0.005f,height*(eye.z)*0.005f,nearPlane,farPlane);
		//glOrtho(-100,100,-100,100,nearPlane,farPlane);
		glViewport((GLint)startX,(GLint)startY,(GLint)width,(GLint)height);
						drawGird();
						if(bottomReference && bottomReference->isShow)
		{
			bottomReference->onPaint();
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
		gluLookAt(eye.x,eye.y,-990,target.x,target.y,target.z,up.x,up.y,up.z);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		//gluPerspective(this->angle,this->width/this->height,this->nearPlane,this->farPlane);
		glOrtho(-width*(eye.z)*0.005f,width*(eye.z)*0.005f,-height*(eye.z)*0.005f,height*(eye.z)*0.005f,nearPlane,farPlane);
		//glOrtho(-100,100,-100,100,nearPlane,farPlane);
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
		glOrtho(-width*(eye.z)*0.005f,width*(eye.z)*0.005f,-height*(eye.z)*0.005f,height*(eye.z)*0.005f,nearPlane,farPlane+100);
			glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		gluLookAt(eye.x,eye.y,-990,target.x,target.y,target.z,up.x,up.y,up.z);
	};

							void setReferenceImage(const char *path,GLuint imageID,Vector &position,size_t width,size_t height)
	{
			if(bottomReference)
			{
				delete bottomReference;
			}
		
			bottomReference=new ViewportImage(path,imageID,width,height,type._value,position);
			bottomReference->initialize();
			bottomReference->isShow=true;
	};

	void getViewportImage(GLuint &texID,std::string &path,Vector &position,size_t &width,size_t &height)
	{
		if(bottomReference)
		{
			texID=bottomReference->imageID;
			path=bottomReference->path;
			position=bottomReference->position;
			width=bottomReference->width;
			height=bottomReference->height;
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
		bottomReference->isShow=false;
	}

		Vector getEye()
	{
		return eye*10.0f;
	}
			Vector getHorizontalDir()
	{
		return Vector(1,0,0);
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
		eye+=cameraDirection*(float)(step*0.1f);
		if(eye.z<2) eye.z=2;
	};

		
	void onPanPress(int x,int y)
	{
		isDraging=true;
		old.x=(float)x;
		old.y=(float)y;
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
		float dx=(float)(x-old.x)*eye.z*0.01f;
		float dy=(float)(y-old.y)*eye.z*0.01f;
		old.x=(float)x;
		old.y=(float)y;
		//dx=10;
		eye.x+=dx;
		eye.y+=dy;
		target.x+=dx;
		target.y+=dy;
	};

public:
	~BottomCamera(void);
};
