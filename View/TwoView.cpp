#include "TwoView.h"

TwoView::TwoView(unsigned int splitX,unsigned int splitY,unsigned int width,unsigned int height)
:SplitedView(splitX,splitY,width,height)
{
    m_viewCount=2;
    m_selected=0;
    m_camera[0]=new PerspectiveCamera(25.0f,(float)m_width,(float)m_height,1.0f,8000.0f,0.0f,0.0f,Vector(0.0f,0.0f,0.0f),Vector(200.0f,200.0f,0.0f),Vector(0.0f,0.0f,1.0f));
    m_camera[1]=new LeftCamera((float)m_width,(float)m_height,1.0f,8000.0f,0.0f,0.0f,Vector(0.0f,0.0f,0.0f),Vector(200.0f,0.0f,0.0f),Vector(0.0f,0.0f,1.0f));
}

void TwoView::onPaint(void)
{
	glColor3ub(255,255,255);
	glBegin(GL_QUADS);
        glVertex2i(m_splitX-1,0);
        glVertex2i(m_splitX+1,0);
        glVertex2i(m_splitX+1, m_height);
        glVertex2i(m_splitX-1, m_height);
	glEnd();
	glColor3ub(255,255,0);
    if( m_selected==0)
	{		glBegin(GL_LINES);
			glVertex2i(2,2);
            glVertex2i( m_splitX-2,2);
            glVertex2i( m_splitX-2,2);
            glVertex2i( m_splitX-2, m_height-2);
            glVertex2i( m_splitX-2, m_height-2);
            glVertex2i(2, m_height-2);
            glVertex2i(2, m_height-2);
			glVertex2i(2,2);
		glEnd();		
	}
    else if( m_selected==1)
	{
		glBegin(GL_LINES);
            glVertex2i( m_splitX+2,2);
            glVertex2i( m_width-2,2);
            glVertex2i( m_width-2,2);
            glVertex2i( m_width-2, m_height-2);
            glVertex2i( m_width-2, m_height-2);
            glVertex2i( m_splitX+2, m_height-2);
            glVertex2i( m_splitX+2, m_height-2);
            glVertex2i( m_splitX+2,2);
		glEnd();		
	}

    if(m_isSelectionMode)
    {
        glEnable(GL_ALPHA_TEST);
        glColor4ub(0,0,255,125);
        glBegin(GL_QUADS);
            glVertex2i(m_selectionSX,m_selectionSY);
            glVertex2i(m_selectionEX,m_selectionSY);
            glVertex2i(m_selectionEX,m_selectionEY);
            glVertex2i(m_selectionSX,m_selectionEY);
        glEnd();
        glDisable(GL_ALPHA_TEST);
    }
	glColor3ub(255,255,255);
}

TwoView::~TwoView(void)
{
    delete m_camera[0];
    delete m_camera[1];
}
