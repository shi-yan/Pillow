#pragma once
#include "Enum.h"
#include "Gird.h"
#include "Matrix.h"
#include "Vector.cpp"
#include "Quaternion.h"
#include "ViewportImage.h"
#define Epsilon 1.0e-5


class Camera
{
protected:
	Vector old;
	bool isDraging;

public:
	//ViewportImage *referenceImage;
	CameraMode type;
	float width;
	float height;
	float nearPlane;
	float farPlane;
	float startX;
	float startY;
	Vector target;
	Vector eye;
	Vector up;
	bool showGird;
	//bool isShowViewportImage;

	GLfloat light_ambient[4];
	GLfloat light_diffuse[4];
	GLfloat light_specular[4];
	GLfloat light_position[4];

	virtual void zoom(float step)
	{
		step;
	};

	virtual void onPanPress(int x,int y)
	{
		x;
		y;
	};

	virtual void disableReference(){};

	virtual void getViewportImage(GLuint &texID,std::string &path,Vector &position,size_t &width,size_t &height)
	{
		texID;
		path;
		position;
		width;
		height;
	};

	virtual void setReferenceImage(const char *path,GLuint imageID,Vector &position,size_t width,size_t height)
	{
		path;
		imageID;
		position;
		width;
		height;
	};

	virtual void onRotatePress(int x,int y)
	{
		x;
		y;
	};
	virtual Vector getEye()
	{
		return Vector(0);
	};
	virtual void rotate(int x,int y)
	{
		x;
		y;
	};
	virtual void onRotateRelease()
	{
	};

	virtual void onPanRelease(int x,int y)
	{
		x;
		y;
	};

	virtual void setCamera()
	{
	};

	virtual void setCameraForSelectionD(size_t x1,size_t y1,size_t x2,size_t y2,size_t h)
	{
		x1;y1;x2;y2;h;
	};

	virtual void setCameraForSelectionS()
	{
		
	};

	virtual void drawGird()
	{
	};

	virtual Vector getHorizontalDir()
	{
		return Vector(0,0,0);
	};
	virtual void pan(int x,int y)
	{
		x;
		y;
	};

	void updateSize(float theStartX,float theStartY,float theWidth,float theHeight)
	{
		startX=theStartX;
		startY=theStartY;
		width=theWidth;
		height=theHeight;
	};

public:
	Camera(CameraMode::__Enum type,float width,float height,float nearPlane,float farPlane,float startX,float startY,Vector target,Vector eye,Vector up);
public:
	virtual ~Camera(void);
};