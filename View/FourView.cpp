#include "FourView.h"
#include "Global.h"
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>


FourView::FourView(unsigned int splitX,unsigned int splitY,unsigned int width,unsigned int height):SplitedView(splitX,splitY,width,height)
{
     m_viewCount=4;
     m_selected=3;
    m_camera[0]=new TopCamera((float)width,(float)height,1.0f,8000.0f,0.0f,0.0f,Vector(0.0f,0.0f,0.0f),Vector(0.0f,0.0f,200.0f),Vector(0.0f,1.0f,0.0f));
    m_camera[1]=new FrontCamera((float)width,(float)height,1.0f,8000.0f,0.0f,0.0f,Vector(0.0f,0.0f,0.0f),Vector(0.0f,200.0f,0.0f),Vector(0.0f,0.0f,1.0f));
    m_camera[2]=new LeftCamera((float)width,(float)height,1.0f,8000.0f,0.0f,0.0f,Vector(0.0f,0.0f,0.0f),Vector(200.0f,0.0f,0.0f),Vector(0.0f,0.0f,1.0f));
    m_camera[3]=new PerspectiveCamera(25.0f,(float)width,(float)height,1.0f,8000.0f,0.0f,0.0f,Vector(0.0f,0.0f,0.0f),Vector(200.0f,200.0f,0.0f),Vector(0.0f,0.0f,1.0f));
}

bool FourView::onLeftDown(unsigned int x,unsigned int y)
{
    if(x< m_splitX&&y< m_splitY)
	{
        if(m_selected==0)
		{
			return false;
		}
		else
		{
            m_selected=0;
            return true;
		}
	}
    else if(x> m_splitX&&y< m_splitY)
	{
        if(m_selected==1)
		{
			return false;
		}
		else
		{
            m_selected=1;
			return true;
		}
	}
    else if(x< m_splitX&&y> m_splitY)
	{
        if(m_selected==2)
		{
			return false;
		}
		else
		{
            m_selected=2;
			return true;
		}
	}
    else if(x> m_splitX&&y> m_splitY)
	{
        if(m_selected==3)
		{
			return false;
		}
		else
		{
            m_selected=3;
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
        glVertex2i( m_splitX-1,0);
        glVertex2i( m_splitX+1,0);
        glVertex2i( m_splitX+1, m_height);
        glVertex2i( m_splitX-1, m_height);
        glVertex2i(0, m_splitY-1);
        glVertex2i( m_width, m_splitY-1);
        glVertex2i( m_width, m_splitY+1);
        glVertex2i(0, m_splitY+1);
	glEnd();
	glColor3ub(255,255,0);
    if( m_selected==2)
	{		
		glBegin(GL_LINES);
            glVertex2i(2, m_splitY+2);
            glVertex2i( m_splitX-2, m_splitY+2);
            glVertex2i( m_splitX-2, m_splitY+2);
            glVertex2i( m_splitX-2, m_height-2);
            glVertex2i( m_splitX-2, m_height-2);
            glVertex2i(2, m_height-2);
            glVertex2i(2, m_height-2);
            glVertex2i(2, m_splitY+2);
		glEnd();		
	}
    else if( m_selected==3)
	{
		glBegin(GL_LINES);
            glVertex2i( m_splitX+2, m_splitY+2);
            glVertex2i( m_width-2, m_splitY+2);
            glVertex2i( m_width-2, m_splitY+2);
            glVertex2i( m_width-2, m_height-2);
            glVertex2i( m_width-2, m_height-2);
            glVertex2i( m_splitX+2, m_height-2);
            glVertex2i( m_splitX+2, m_height-2);
            glVertex2i( m_splitX+2, m_splitY+2);
		glEnd();		
	}
    else if( m_selected==0)
	{
		glBegin(GL_LINES);
			glVertex2i(0,0);
            glVertex2i( m_splitX-2,0);
            glVertex2i( m_splitX-2,0);
            glVertex2i( m_splitX-2, m_splitY-2);
            glVertex2i( m_splitX-2, m_splitY-2);
            glVertex2i(0, m_splitY-2);
            glVertex2i(0, m_splitY-2);
			glVertex2i(0,0);
		glEnd();		
	}
    else if( m_selected==1)
	{
		glBegin(GL_LINES);
            glVertex2i( m_splitX+2,2);
            glVertex2i( m_width-2,2);
            glVertex2i( m_width-2,2);
            glVertex2i( m_width-2, m_splitY-2);
            glVertex2i( m_width-2, m_splitY-2);
            glVertex2i( m_splitX+2, m_splitY-2);
            glVertex2i( m_splitX+2, m_splitY-2);
            glVertex2i( m_splitX+2,2);
		glEnd();		
	}

    if(m_isSelectionMode)
    {
        glEnable(GL_ALPHA_TEST);
        glColor4ub(0,0,255,125);
        glBegin(GL_QUADS);
            glVertex2i(m_selectionSX, m_selectionSY);
            glVertex2i(m_selectionEX, m_selectionSY);
            glVertex2i(m_selectionEX, m_selectionEY);
            glVertex2i(m_selectionSX, m_selectionEY);
        glEnd();
        glDisable(GL_ALPHA_TEST);
    }
	glColor3ub(255,255,255);
}

FourView::~FourView(void)
{
}
