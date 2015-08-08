#include "FourView.h"
#include "Global.h"
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>


FourView::FourView(unsigned int splitX,unsigned int splitY,unsigned int width,unsigned int height):SplitedView(splitX,splitY,width,height)
{
	this->viewCount=4;
	this->selected=3;
	camera[0]=new TopCamera((float)width,(float)height,1.0f,8000.0f,0.0f,0.0f,Vector(0.0f,0.0f,0.0f),Vector(0.0f,0.0f,200.0f),Vector(0.0f,1.0f,0.0f));
	camera[1]=new FrontCamera((float)width,(float)height,1.0f,8000.0f,0.0f,0.0f,Vector(0.0f,0.0f,0.0f),Vector(0.0f,200.0f,0.0f),Vector(0.0f,0.0f,1.0f));
	camera[2]=new LeftCamera((float)width,(float)height,1.0f,8000.0f,0.0f,0.0f,Vector(0.0f,0.0f,0.0f),Vector(200.0f,0.0f,0.0f),Vector(0.0f,0.0f,1.0f));
	camera[3]=new PerspectiveCamera(25.0f,(float)width,(float)height,1.0f,8000.0f,0.0f,0.0f,Vector(0.0f,0.0f,0.0f),Vector(200.0f,200.0f,0.0f),Vector(0.0f,0.0f,1.0f));
}

bool FourView::onLeftDown(unsigned int x,unsigned int y)
{
//	int tempView;
	if(x<this->splitX&&y<this->splitY)
	{
		if(selected==0)
		{
			return false;
		}
		else
		{
		this->selected=0;
		return true;
		}
	}
	else
	if(x>this->splitX&&y<this->splitY)
	{
		if(selected==1)
		{
			return false;
		}
		else
		{
			this->selected=1;
			return true;
		}
	}
	else
	if(x<this->splitX&&y>this->splitY)
	{
		if(selected==2)
		{
			return false;
		}
		else
		{
			this->selected=2;
			return true;
		}
	}
	else
	if(x>this->splitX&&y>this->splitY)
	{
		if(selected==3)
		{
			return false;
		}
		else
		{
			this->selected=3;
			return true;
		}
	}
	else
	{
		return false;
	}
}

void FourView::onPaint(void)
{
	glColor3ub(255,255,255);
	glBegin(GL_QUADS);
		glVertex2i(this->splitX-1,0);
		glVertex2i(this->splitX+1,0);
		glVertex2i(this->splitX+1,this->height);
		glVertex2i(this->splitX-1,this->height);
		glVertex2i(0,this->splitY-1);
		glVertex2i(this->width,this->splitY-1);
		glVertex2i(this->width,this->splitY+1);
		glVertex2i(0,this->splitY+1);
	glEnd();
	glColor3ub(255,255,0);
	if(this->selected==2)
	{		
		glBegin(GL_LINES);
			glVertex2i(2,this->splitY+2);
			glVertex2i(this->splitX-2,this->splitY+2);
			glVertex2i(this->splitX-2,this->splitY+2);
			glVertex2i(this->splitX-2,this->height-2);
			glVertex2i(this->splitX-2,this->height-2);
			glVertex2i(2,this->height-2);
			glVertex2i(2,this->height-2);
			glVertex2i(2,this->splitY+2);
		glEnd();		
	}
	else
	if(this->selected==3)
	{
		glBegin(GL_LINES);
			glVertex2i(this->splitX+2,this->splitY+2);
			glVertex2i(this->width-2,this->splitY+2);
			glVertex2i(this->width-2,this->splitY+2);
			glVertex2i(this->width-2,this->height-2);
			glVertex2i(this->width-2,this->height-2);
			glVertex2i(this->splitX+2,this->height-2);
			glVertex2i(this->splitX+2,this->height-2);
			glVertex2i(this->splitX+2,this->splitY+2);
		glEnd();		
	}
	else
	if(this->selected==0)
	{
		glBegin(GL_LINES);
			glVertex2i(0,0);
			glVertex2i(this->splitX-2,0);
			glVertex2i(this->splitX-2,0);
			glVertex2i(this->splitX-2,this->splitY-2);
			glVertex2i(this->splitX-2,this->splitY-2);
			glVertex2i(0,this->splitY-2);
			glVertex2i(0,this->splitY-2);
			glVertex2i(0,0);
		glEnd();		
	}
	else
	if(this->selected==1)
	{
		glBegin(GL_LINES);
			glVertex2i(this->splitX+2,2);
			glVertex2i(this->width-2,2);
			glVertex2i(this->width-2,2);
			glVertex2i(this->width-2,this->splitY-2);
			glVertex2i(this->width-2,this->splitY-2);
			glVertex2i(this->splitX+2,this->splitY-2);
			glVertex2i(this->splitX+2,this->splitY-2);
			glVertex2i(this->splitX+2,2);
		glEnd();		
	}
			if(isSelectionMode)
		{
			glEnable(GL_ALPHA_TEST);
			glColor4ub(0,0,255,125);
			glBegin(GL_QUADS);
				glVertex2i(selectionSX,selectionSY);
				glVertex2i(selectionEX,selectionSY);
				glVertex2i(selectionEX,selectionEY);
				glVertex2i(selectionSX,selectionEY);
			glEnd();
			glDisable(GL_ALPHA_TEST);
			
		}

	glColor3ub(255,255,255);
}

FourView::~FourView(void)
{
}
