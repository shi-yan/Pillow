#include "Global.h"
#include "SingleView.h"

SingleView::SingleView(unsigned int splitX,unsigned int splitY,unsigned int width,unsigned int height)
:SplitedView(splitX,splitY,width,height)
{
	viewCount=1;
	selected=0;
     camera[0]=new PerspectiveCamera(25.0f,(float)width,(float)height,1.0f,8000.0f,0.0f,0.0f,Vector(0.0f,0.0f,0.0f),Vector(200.0f,200.0f,0.0f),Vector(0.0f,0.0f,1.0f));
    // camera[0]=new BottomCamera( width, height,1,8000,0,0,Vector(0,0,0),Vector(0,0,200),Vector(0,1,0));
    // camera[0]=new FrontCamera( width, height,1,8000,0,0,Vector(0,0,0),Vector(0,200,0),Vector(0,0,1));
    // camera[0]=new BackCamera( width, height,1,8000,0,0,Vector(0,0,0),Vector(0,200,0),Vector(0,0,1));
    // camera[0]=new RightCamera( width, height,1,8000,0,0,Vector(0,0,0),Vector(200,0,0),Vector(0,0,1));
    // camera[0]=new LeftCamera( width, height,1,8000,0,0,Vector(0,0,0),Vector(200,0,0),Vector(0,0,1));
    // camera[0]=new TopCamera( width, height,1,8000,0,0,Vector(0,0,0),Vector(0,0,200),Vector(0,1,0));
}

SingleView::~SingleView(void)
{
    delete  camera[0];
}
