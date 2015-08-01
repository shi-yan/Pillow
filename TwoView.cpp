#include "TwoView.h"

TwoView::TwoView(unsigned int splitX,unsigned int splitY,unsigned int width,unsigned int height)
:SplitedView(splitX,splitY,width,height)
{
	viewCount=2;
	selected=0;
	camera[0]=new PerspectiveCamera(25.0f,(float)width,(float)height,1.0f,8000.0f,0.0f,0.0f,Vector(0.0f,0.0f,0.0f),Vector(200.0f,200.0f,0.0f),Vector(0.0f,0.0f,1.0f));
	camera[1]=new LeftCamera((float)width,(float)height,1.0f,8000.0f,0.0f,0.0f,Vector(0.0f,0.0f,0.0f),Vector(200.0f,0.0f,0.0f),Vector(0.0f,0.0f,1.0f));
}

void TwoView::onPaint(void)
{
	glColor3ub(255,255,255);
	glBegin(GL_QUADS);
		glVertex2i(this->splitX-1,0);
		glVertex2i(this->splitX+1,0);
		glVertex2i(this->splitX+1,this->height);
		glVertex2i(this->splitX-1,this->height);
	glEnd();
	glColor3ub(255,255,0);
	if(this->selected==0)
	{		glBegin(GL_LINES);
			glVertex2i(2,2);
			glVertex2i(this->splitX-2,2);
			glVertex2i(this->splitX-2,2);
			glVertex2i(this->splitX-2,this->height-2);
			glVertex2i(this->splitX-2,this->height-2);
			glVertex2i(2,this->height-2);
			glVertex2i(2,this->height-2);
			glVertex2i(2,2);
		glEnd();		
	}
	else
	if(this->selected==1)
	{
		glBegin(GL_LINES);
			glVertex2i(this->splitX+2,2);
			glVertex2i(this->width-2,2);
			glVertex2i(this->width-2,2);
			glVertex2i(this->width-2,this->height-2);
			glVertex2i(this->width-2,this->height-2);
			glVertex2i(this->splitX+2,this->height-2);
			glVertex2i(this->splitX+2,this->height-2);
			glVertex2i(this->splitX+2,2);
		glEnd();		
	}
//	glColor3ub(255,255,255);

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

TwoView::~TwoView(void)
{
	delete camera[0];
	delete camera[1];
}