#pragma once

#include "Camera.h"
#include <QDebug>

class PerspectiveCamera:public Camera
{
public:
	float angle;

public:
	void zoom(float step)
	{
		Vector cameraDirection(0);
		cameraDirection=this->target-this->eye;
		cameraDirection.normalize();
		//this->target+=cameraDirection*step;
        qDebug() << "dir" << cameraDirection.x << cameraDirection.y << cameraDirection.z << step;
		this->eye+=cameraDirection*step;

        qDebug() << "zoom" << this->eye.x << this->eye.y << this->eye.z;
	};

	void onPanPress(int x,int y)
	{
		this->isDraging=true;
		this->old.x=(float)x;
		this->old.y=(float)y;
	};

	void onRotatePress(int x,int y)
	{
			this->isDraging=true;
			this->old.x=(float)x;
			this->old.y=(float)y;
			//this->old=this->mapToSphere(x,y);
			//this->old.normalize();
	};

	void drawGird()
	{
		if(showGird)
		{
			theGird->drawXY();
		}
		else
		{
			glDisable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D,0);
		}
	};

	void rotate45V()
	{
	
			Vector perp=perpendicular(this->eye-this->target,this->up);
			perp.normalize();
			Quaternion rotateQuaternion((float)-10.0f,perp);
			Matrix rotateMatrix;
			rotateQuaternion.getMatrix(rotateMatrix);
			Vector cameraDirection=this->eye-this->target;
			Vector upDirection=cameraDirection+this->up;
			cameraDirection=(rotateMatrix)*(cameraDirection);
			upDirection=(rotateMatrix)*upDirection;
			this->eye=cameraDirection+this->target;
			light_position[0]=cameraDirection.x;
			light_position[1]=cameraDirection.y;
			light_position[2]=cameraDirection.z;
			light_position[3]=1;
			this->up=upDirection-cameraDirection;
	
	};

	void rotate(int x,int y)
	{
		//Vector newRotation(0);
		//newRotation=mapToSphere(x,y);
		int dx=(int)(this->old.x-(float)x);
		int dy=(int)(this->old.y-(float)y);
		//newRotation.normalize();
		
		if(dy!=0)  
		{
			Vector perp=perpendicular(this->eye-this->target,this->up);
			perp.normalize();
			Quaternion rotateQuaternion((float)dy,perp);
		//	Matrix *rotateMatrix=new Matrix();
			Matrix rotateMatrix;
			rotateQuaternion.getMatrix(rotateMatrix);
			Vector cameraDirection=this->eye-this->target;
			Vector upDirection=cameraDirection+this->up;
			cameraDirection=(rotateMatrix)*(cameraDirection);
			upDirection=(rotateMatrix)*upDirection;
			this->eye=cameraDirection+this->target;
			light_position[0]=cameraDirection.x;
			light_position[1]=cameraDirection.y;
			light_position[2]=cameraDirection.z;
			light_position[3]=1;
			this->up=upDirection-cameraDirection;
			//delete rotateMatrix;
		}
		if(dx!=0)
		{
			Vector perp(0,0,1);
			//perp.normalize();
			Quaternion rotateQuaternion((float)dx,perp);
			Matrix *rotateMatrix=new Matrix();
			rotateQuaternion.getMatrix(*rotateMatrix);
			Vector cameraDirection=this->eye-this->target;
			Vector upDirection=cameraDirection+this->up;
			cameraDirection=(*rotateMatrix)*(cameraDirection);
			upDirection=(*rotateMatrix)*upDirection;
			this->eye=cameraDirection+this->target;
			light_position[0]=cameraDirection.x;
			light_position[1]=cameraDirection.y;
			light_position[2]=cameraDirection.z;
			light_position[3]=1;
			this->up=upDirection-cameraDirection;
			delete rotateMatrix;
		}
		this->old.x=(float)x;
		this->old.y=(float)y;
	};

	Vector getEye()
	{
		return eye;
	}

	Vector rotateFromP2P(int x1,int y1,int x2,int y2)
	{
		if(y1==y2) 
			return 0;
		Vector oldRotation(0);
		Vector newRotation(0);
		oldRotation=mapToSphere(x1,y1);
		newRotation=mapToSphere(x2,y2);
		oldRotation.normalize();
		newRotation.normalize();
		Vector perp=perpendicular(this->target-this->eye,this->up);
		if (perp.length2() > Epsilon)
		{	
			Quaternion rotateQuaternion(acos(psVECDOT(oldRotation,newRotation)),perp);
			Matrix *rotateMatrix=new Matrix();
			rotateQuaternion.getMatrix(*rotateMatrix);
			Vector cameraDirection=this->eye-this->target;
			Vector upDirection=cameraDirection+this->up;
			cameraDirection=(*rotateMatrix)*(cameraDirection);
			upDirection=(*rotateMatrix)*upDirection;
			this->eye=cameraDirection+this->target;
			this->up=upDirection-cameraDirection;
			delete rotateMatrix;
			return rotateQuaternion;
		}
	};

	void onRotateRelease()
	{
		this->isDraging=false;
		this->old.null();
	};

	void onPanRelease(int x,int y)
	{
		x;
		y;
		isDraging=false;
		old.null();
	};

	void setCamera()
	{
	
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		gluLookAt(this->eye.x,this->eye.y,this->eye.z,this->target.x,this->target.y,this->target.z,this->up.x,this->up.y,this->up.z);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(this->angle,this->width/this->height,this->nearPlane,this->farPlane);
		glViewport((GLint)this->startX,(GLint)this->startY,(GLint)this->width,(GLint)this->height);
		drawGird();
		glEnable(GL_LIGHT0);
		glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
		glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
		glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
		glLightfv(GL_LIGHT0, GL_POSITION, light_position);

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
		gluPerspective(angle,width/height,nearPlane,farPlane);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		gluLookAt(eye.x,eye.y,eye.z,target.x,target.y,target.z,up.x,up.y,up.z);
	};

	void setCameraForSelectionS()
	{
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		gluLookAt(this->eye.x,this->eye.y,this->eye.z,this->target.x,this->target.y,this->target.z,this->up.x,this->up.y,this->up.z);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(this->angle,this->width/this->height,this->nearPlane,this->farPlane);
		glViewport((GLint)this->startX,(GLint)this->startY,(GLint)this->width,(GLint)this->height);			
	};
	
	void pan(int x,int y)
	{
		int dx=x-(int)old.x;
		int dy=y-(int)old.y;
		old.x=(float)x;
		old.y=(float)y;
		Vector cameraDirection(0);
		cameraDirection=target-eye;
		Vector horizontalDirection(0);
		horizontalDirection=perpendicular(cameraDirection,up);
		horizontalDirection.normalize();
		this->target+=horizontalDirection*(float)dx*0.5f+this->up*(float)dy*0.5f;
	};

	Vector getHorizontalDir()
	{
		Vector cameraDirection(target-eye);
		Vector horizontalDirection(perpendicular(cameraDirection,up));
		horizontalDirection.normalize();
		return horizontalDirection;
	}

	PerspectiveCamera(float angle,float width,float height,float nearPlane,float farPlane,float startX,float startY,Vector target,Vector eye,Vector up);

private:
	Vector mapToSphere(const int x,const int y) const
	{
		float tempX;
		float tempY;
		tempX=((float)x/this->width-0.5f)*1.414213562f;
		tempY=((float)y/this->height-0.5f)*1.414213562f;
		Vector result(0);
		result.x = 0;
		result.z = sqrtf(1.0f - tempY*tempY);
		result.y = tempY;
		return result;
	};

public:
	~PerspectiveCamera(void);
};
