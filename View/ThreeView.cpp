#include "ThreeView.h"

ThreeView::ThreeView(unsigned int splitX,unsigned int splitY,unsigned int width,unsigned int height)
:SplitedView(splitX,splitY,width,height)
{
	viewCount=3;
	selected=0;
	camera[0]=new PerspectiveCamera(25.0f,(float)width,(float)height,1.0f,8000.0f,0.0f,0.0f,Vector(0.0f,0.0f,0.0f),Vector(200.0f,200.0f,0.0f),Vector(0.0f,0.0f,1.0f));
	camera[1]=new FrontCamera((float)width,(float)height,1.0f,8000.0f,0.0f,0.0f,Vector(0.0f,0.0f,0.0f),Vector(0.0f,200.0f,0.0f),Vector(0.0f,0.0f,1.0f));
	camera[2]=new RightCamera((float)width,(float)height,1.0f,8000.0f,0.0f,0.0f,Vector(0.0f,0.0f,0.0f),Vector(200.0f,0.0f,0.0f),Vector(0.0f,0.0f,1.0f));
}

bool ThreeView::onLeftDown(unsigned int x,unsigned int y)
{
    if(x< splitX)
	{
		if(selected==0)
		{
			return false;
		}
		else
		{
             selected=0;
			return true;
		}
	}
	else
    if(x> splitX&&y< splitY)
	{
		if(selected==1)
		{
			return false;
		}
		else
		{
             selected=1;
			return true;
		}
	}
	else
    if(x> splitX&&y> splitY)
	{
		if(selected==2)
		{
			return false;
		}
		else
		{
             selected=2;
			return true;
		}
	}
	else
	{
		return false;
	}
}

void ThreeView::onPaint(void)
{
	glColor3ub(255,255,255);
	glBegin(GL_QUADS);
        glVertex2i( splitX-1,0);
        glVertex2i( splitX+1,0);
        glVertex2i( splitX+1, height);
        glVertex2i( splitX-1, height);
        glVertex2i( splitX-1, splitY-1);
        glVertex2i( width, splitY-1);
        glVertex2i( width, splitY+1);
        glVertex2i( splitX-1, splitY+1);
	glEnd();
	glColor3ub(255,255,0);
    if( selected==0)
	{
		glBegin(GL_LINES);
			glVertex2i(2,2);
            glVertex2i( splitX-2,2);
            glVertex2i( splitX-2,2);
            glVertex2i( splitX-2, height-2);
            glVertex2i( splitX-2, height-2);
            glVertex2i(2, height-2);
            glVertex2i(2, height-2);
			glVertex2i(2,2);
		glEnd();		
	}
	else
    if( selected==1)
	{
		glBegin(GL_LINES);
            glVertex2i( splitX+2,2);
            glVertex2i( width-2,2);
            glVertex2i( width-2,2);
            glVertex2i( width-2, splitY-2);
            glVertex2i( width-2, splitY-2);
            glVertex2i( splitX+2, splitY-2);
            glVertex2i( splitX+2, splitY-2);
            glVertex2i( splitX+2,2);
		glEnd();		
	}
	else
    if( selected==2)
	{
		glBegin(GL_LINES);
            glVertex2i( splitX+2, splitY+2);
            glVertex2i( width-2, splitY+2);
            glVertex2i( width-2, splitY+2);
            glVertex2i( width-2, height-2);
            glVertex2i( width-2, height-2);
            glVertex2i( splitX+2, height-2);
            glVertex2i( splitX+2, height-2);
            glVertex2i( splitX+2, splitY+2);
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
