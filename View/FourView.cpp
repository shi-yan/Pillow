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
    delete m_camera[0];
    delete m_camera[1];
    delete m_camera[2];
    delete m_camera[3];
}

CameraMode::__Enum FourView::setView(unsigned int index)
{
    m_camera[index]->setCamera();
    return m_camera[index]->m_type._value;
}

bool FourView::onAxisPress(unsigned int x,unsigned int y)
{
    //if((selected==0 && x<splitX)||(selected==1 && x>splitX)){
    m_camera[m_selected]->setCameraForSelectionS();
    m_cursorDir.z=0;
    bool result=theScene->isAxisSelected(m_camera[m_selected]->m_type._value, m_camera[m_selected]->getEye(), m_height, x, y, m_cursorDir.x, m_cursorDir.y, m_cursorMode);
    if(result)
    {
        m_axisDragSX=x;
        m_axisDragSY=y;
        m_cursorDir.normalize();
        m_isAxisMode=true;
    }
    return result;
}

bool FourView::onAxisDrag(unsigned int x,unsigned int y)
{
    if(m_isAxisMode)
    {
        m_camera[m_selected]->setCamera();
        Vector m((float)((int)x-(int)m_axisDragSX),(float)((int)y-(int)m_axisDragSY),0.0f);
        float work=psVECDOT(m_cursorDir,m);
        theScene->axisDrag(work,m_cursorMode,m_cursorDir.x,m_cursorDir.y);
        m_axisDragSX=x;
        m_axisDragSY=y;
        return true;
    }
    else
    {
        return false;
    }
}

bool FourView::onAxisRelease()
{
    if(m_isAxisMode)
    {
        m_isAxisMode=false;
        theScene->axisRelease();
        m_axisDragSX=0;
        m_axisDragSY=0;
        return true;
    }
    else
    {
        return false;
    }
}

bool FourView::onCtrlDrag(unsigned int x,unsigned int y)
{
    if(m_isCtrlMode)
    {
        m_camera[m_selected]->setCamera();
        Vector horizontalDir(m_camera[m_selected]->getHorizontalDir());
        theScene->ctrlDrag(horizontalDir,m_camera[m_selected]->m_up,x-m_ctrlSX,y-m_ctrlSY,m_isExtrude);
        m_isExtrude=false;
        m_ctrlSX=x;
        m_ctrlSY=y;
        return true;
    }
    else
    {
        return false;
    }
}

bool FourView::onCtrlPress(unsigned int x,unsigned int y)
{
    m_isCtrlMode=true;
    m_ctrlSX=x;
    m_ctrlSY=y;
    return true;
}

bool FourView::onCtrlRelease()
{
    if(m_isCtrlMode)
    {
        m_isCtrlMode=false;
        m_ctrlSX=0;
        m_ctrlSY=0;
        return true;
    }
    else
    {
        return false;
    }
}

bool FourView::onPanPress(unsigned int x,unsigned int y)
{
    if(m_selected==0 && x<m_splitX && y<m_splitY)
    {
        m_camera[m_selected]->onPanPress(x,y);
    }
    else if(m_selected==2 && x<m_splitX && y>m_splitY)
    {
        m_camera[m_selected]->onPanPress(x,y);
    }
    else if(m_selected==1 && x>m_splitX && y<m_splitY)
    {
        m_camera[m_selected]->onPanPress(x,y);
    }
    else if(m_selected==3 && x>m_splitX && y>m_splitY)
    {
        m_camera[m_selected]->onPanPress(x,y);
    }
    return true;
}

bool FourView::onPanDrag(unsigned int x,unsigned int y)
{
    m_camera[m_selected]->pan(x,y);
    return true;
}

bool FourView::onSelectionPress(unsigned int x,unsigned int y)
{
    m_isSelectionMode=true;
    m_selectionSX=x;
    m_selectionSY=y;
    m_selectionEX=x;
    m_selectionEY=y;
    return true;
}

bool FourView::onSelectionDrag(unsigned int x,unsigned int y)
{
    if(m_isSelectionMode)
    {
        m_selectionEX=x;
        m_selectionEY=y;
        return true;
    }
    else
    {
        return false;
    }
}

bool FourView::onSingleSideSelectionRelease(bool isAppend)
{
    if(m_isSelectionMode)
    {
        unsigned int x1=(m_selectionSX>m_selectionEX)?m_selectionEX:m_selectionSX;
        unsigned int y1=(m_selectionSY>m_selectionEY)?m_selectionEY:m_selectionSY;
        unsigned int x2=(m_selectionSX>m_selectionEX)?m_selectionSX:m_selectionEX;
        unsigned int y2=(m_selectionSY>m_selectionEY)?m_selectionSY:m_selectionEY;
        m_camera[m_selected]->setCameraForSelectionS();
        theScene->selectSingleSide(x1,y1,x2,y2,m_height,isAppend);
        m_isSelectionMode=false;
        m_selectionSX=0;
        m_selectionSY=0;
        m_selectionEX=0;
        m_selectionEY=0;
        return true;
    }
    else
    {
        return false;
    }
}

bool FourView::onDualSideSelectionRelease(bool isAppend)
{
    if(m_isSelectionMode)
    {
        unsigned int x1=(m_selectionSX>m_selectionEX)?m_selectionEX:m_selectionSX;
        unsigned int y1=(m_selectionSY>m_selectionEY)?m_selectionEY:m_selectionSY;
        unsigned int x2=(m_selectionSX>m_selectionEX)?m_selectionSX:m_selectionEX;
        unsigned int y2=(m_selectionSY>m_selectionEY)?m_selectionSY:m_selectionEY;
        m_camera[m_selected]->setCameraForSelectionD(x1,y1,x2,y2,m_height);
        theScene->selectDualSide(isAppend);
        m_isSelectionMode=false;
        m_selectionSX=0;
        m_selectionSY=0;
        m_selectionEX=0;
        m_selectionEY=0;
        return true;
    }
    else
    {
        return false;
    }
}

void FourView::onWheel(float step,unsigned int x,unsigned int y)
{
    if(m_selected==0 && x<m_splitX && y<m_splitY)
    {
        m_camera[m_selected]->zoom(step);
    }
    else if(m_selected==2 && x<m_splitX && y>m_splitY)
    {
        m_camera[m_selected]->zoom(step);
    }
    else if(m_selected==1 && x>m_splitX && y<m_splitY)
    {
        m_camera[m_selected]->zoom(step);
    }
    else if(m_selected==3 && x>m_splitX && y>m_splitY)
    {
        m_camera[m_selected]->zoom(step);
    }
}

bool FourView::onRotateDrag(unsigned int x,unsigned int y)
{
    m_camera[m_selected]->rotate(x,y);
    return true;
}

bool FourView::onRotatePress(unsigned int x,unsigned int y)
{
    if(m_selected==0 && x<m_splitX && y<m_splitY)
    {
        m_camera[m_selected]->onRotatePress(x,y);
    }
    else if(m_selected==2 && x<m_splitX && y>m_splitY)
    {
        m_camera[m_selected]->onRotatePress(x,y);
    }
    else if(m_selected==1 && x>m_splitX && y<m_splitY)
    {
        m_camera[m_selected]->onRotatePress(x,y);
    }
    else if(m_selected==3 && x>m_splitX && y>m_splitY)
    {
        m_camera[m_selected]->onRotatePress(x,y);
    }
    return true;
}

bool FourView::onRotateRelease()
{
    m_camera[m_selected]->onRotateRelease();
    return true;
}

bool FourView::onPanRelease()
{
    m_camera[m_selected]->onPanRelease(0,0);
    return true;
}

void FourView::update(unsigned int theSplitX,unsigned int theSplitY,unsigned int theWidth,unsigned int theHeight)
{
    m_splitX=theSplitX;
    m_splitY=theSplitY;
    m_width=theWidth;
    m_height=theHeight;
    m_camera[2]->updateSize(0.0f,0.0f,(float)m_splitX,(float)m_splitY);
    m_camera[0]->updateSize(0.0f,(float)m_splitY,(float)m_splitX,(float)(m_height-m_splitY));
    m_camera[3]->updateSize((float)m_splitX,0.0f,(float)(m_width-m_splitX),(float)m_splitY);
    m_camera[1]->updateSize((float)m_splitX,(float)m_splitY,(float)(m_width-m_splitX),(float)(m_height-m_splitY));
}
