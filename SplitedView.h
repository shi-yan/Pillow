#pragma once
#include "LeftCamera.h"
#include "RightCamera.h"
#include "BackCamera.h"
#include "FrontCamera.h"
#include "BottomCamera.h"
#include "TopCamera.h"
#include "PerspectiveCamera.h"

#include <GL/gl.h>
#include <GL/glu.h>

class SplitedView
{
protected:
	size_t splitX;
	size_t splitY;
	size_t width;
	size_t height;
	size_t selected;
	int panIndex;
	int zoomIndex;
	int rotateIndex;
	Camera *camera[4];
	size_t selectionSX;
	size_t selectionSY;
	size_t selectionEX;
	size_t selectionEY;
	size_t axisDragSX;
	size_t axisDragSY;
	size_t axisDragEX;
	size_t axisDragEY;
	bool isSelectionMode;
	bool isAxisMode;
	bool isExtrude;
	Vector cursorDir;
	int cursorMode;
	bool isCtrlMode;
	size_t ctrlSX;
	size_t ctrlSY;
public:
	size_t viewCount;

public:
	void changeCamera(CameraMode::__Enum type)
	{
		size_t cameraWidth=(size_t)camera[selected]->width;
		size_t cameraHeight=(size_t)camera[selected]->height;
		size_t cameraStartX=(size_t)camera[selected]->startX;
		size_t cameraStartY=(size_t)camera[selected]->startY;
		delete camera[this->selected];
		camera[selected]=NULL;
		if(type==CameraMode::Perspective)
		{
			camera[selected]=new PerspectiveCamera(25,(float)cameraWidth,(float)cameraHeight,1,8000,(float)cameraStartX,(float)cameraStartY,Vector(0,0,0),Vector(200,200,0),Vector(0,0,1));
		}
		else if(type==CameraMode::Top)
		{
			camera[selected]=new TopCamera((float)cameraWidth,(float)cameraHeight,1,8000,(float)cameraStartX,(float)cameraStartY,Vector(0,0,0),Vector(0,0,200),Vector(0,1,0));
		}
		else if(type==CameraMode::Bottom)
		{
			camera[selected]=new BottomCamera((float)cameraWidth,(float)cameraHeight,1,8000,(float)cameraStartX,(float)cameraStartY,Vector(0,0,0),Vector(0,0,200),Vector(0,1,0));
		}
		else if(type==CameraMode::Front)
		{
			camera[selected]=new FrontCamera((float)cameraWidth,(float)cameraHeight,1,8000,(float)cameraStartX,(float)cameraStartY,Vector(0,0,0),Vector(0,200,0),Vector(0,0,1));
		}
		else if(type==CameraMode::Back)
		{
			camera[selected]=new BackCamera((float)cameraWidth,(float)cameraHeight,1,8000,(float)cameraStartX,(float)cameraStartY,Vector(0,0,0),Vector(0,200,0),Vector(0,0,1));
		}
		else if(type==CameraMode::Left)
		{
			camera[selected]=new LeftCamera((float)cameraWidth,(float)cameraHeight,1,8000,(float)cameraStartX,(float)cameraStartY,Vector(0,0,0),Vector(200,0,0),Vector(0,0,1));
		}
		else if(type==CameraMode::Right)
		{
			camera[selected]=new RightCamera((float)cameraWidth,(float)cameraHeight,1,8000,(float)cameraStartX,(float)cameraStartY,Vector(0,0,0),Vector(200,0,0),Vector(0,0,1));
		}
	};

	void getViewportImage(GLuint &texID,std::string &path,Vector &position,size_t &width,size_t &height)
	{
		camera[selected]->getViewportImage(texID,path,position,width,height);
	}

	void setViewportImage(const char *path,GLuint imageID,Vector &position,size_t width,size_t height)
	{
		camera[selected]->setReferenceImage(path,imageID,position,width,height);
	}

	void disableReference()
	{
		camera[selected]->disableReference();
	}

	void onExtrudeDown()
	{
		isExtrude=true;
	};

	void onExtrudeRelease()
	{
		isExtrude=false;
	};

	void switchGird()
	{
		camera[selected]->showGird=!(camera[selected]->showGird);
	};

	virtual bool onCtrlPress(size_t x,size_t y)
	{
		x;y;
		return false;
	};

	virtual bool onCtrlDrag(size_t x,size_t y)
	{
		x;y;
		return false;
	};

	virtual bool onPanPress(size_t x,size_t y)
	{
		x;
		y;
		return false;
	};

	virtual bool onAxisPress(size_t x,size_t y)
	{
		x;y;
		return false;
	};

	virtual bool onAxisDrag(size_t x,size_t y)
	{
		x;y;
		return false;
	};

	virtual bool onAxisRelease()
	{
		return false;
	};

	virtual bool onCtrlRelease()
	{
		return false;
	};

	virtual bool onPanDrag(size_t x,size_t y)
	{
		x;
		y;
		return false;
	};

	virtual bool onPanRelease()
	{
		return false;
	};

	virtual bool onRotateDrag(size_t x,size_t y)
	{
		x;
		y;
		return false;
	};

	virtual bool onRotateRelease()
	{
		return false;
	};
	
	virtual bool onRotatePress(size_t x,size_t y)
	{
		x;
		y;
		return false;
	};

	virtual bool onSelectionPress(size_t x,size_t y)
	{
		x;y;
		return false;
	};
	
	virtual CameraMode::__Enum setView(size_t i)
	{
		return CameraMode::Perspective;
		i;
	};

	Vector getEye(size_t i)
	{
		return camera[i]->getEye();
	};
	
	virtual void onWheel(float step,size_t x,size_t y)
	{
		step;
		x;
		y;
	};
	
	virtual void update(size_t splitX,size_t splitY,size_t width,size_t height)
	{
		splitX;
		splitY;
		width;
		height;
	};

	virtual void setViewport(size_t index)
	{
		index;
	};

	virtual bool onDrag(size_t x,size_t y)
	{
		x;y;
		return false;
	}

	virtual bool onLeftDown(size_t x,size_t y)
	{
		x;
		y;
		return false;
	}

	virtual void onPaint(void)
	{
	}

	virtual bool onSelectionDrag(size_t x,size_t y)
	{
		x;
		y;
		return false;
	};

	virtual bool onSingleSideSelectionRelease(bool isAppend)
	{
		isAppend;
		return false;
	};

	virtual bool onDualSideSelectionRelease(bool isAppend)
	{
		isAppend;
		return false;
	};

    SplitedView(unsigned int splitX,unsigned int splitY,unsigned int width,unsigned int height);

public:
	~SplitedView(void)
	{
		delete this->camera[0];
		delete this->camera[1];
		delete this->camera[2];
		delete this->camera[3];
	}
};
