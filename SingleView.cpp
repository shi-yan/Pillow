#include "Global.h"
#include "SingleView.h"

SingleView::SingleView(unsigned int splitX,unsigned int splitY,unsigned int width,unsigned int height)
:SplitedView(splitX,splitY,width,height)
{
	viewCount=1;
	selected=0;
	this->camera[0]=new PerspectiveCamera(25.0f,(float)width,(float)height,1.0f,8000.0f,0.0f,0.0f,Vector(0.0f,0.0f,0.0f),Vector(200.0f,200.0f,0.0f),Vector(0.0f,0.0f,1.0f));
	//this->camera[0]=new BottomCamera(this->width,this->height,1,8000,0,0,Vector(0,0,0),Vector(0,0,200),Vector(0,1,0));
	//this->camera[0]=new FrontCamera(this->width,this->height,1,8000,0,0,Vector(0,0,0),Vector(0,200,0),Vector(0,0,1));
	//this->camera[0]=new BackCamera(this->width,this->height,1,8000,0,0,Vector(0,0,0),Vector(0,200,0),Vector(0,0,1));
	//this->camera[0]=new RightCamera(this->width,this->height,1,8000,0,0,Vector(0,0,0),Vector(200,0,0),Vector(0,0,1));
	//this->camera[0]=new LeftCamera(this->width,this->height,1,8000,0,0,Vector(0,0,0),Vector(200,0,0),Vector(0,0,1));
	//this->camera[0]=new TopCamera(this->width,this->height,1,8000,0,0,Vector(0,0,0),Vector(0,0,200),Vector(0,1,0));
}

SingleView::~SingleView(void)
{
	delete this->camera[0];
}
