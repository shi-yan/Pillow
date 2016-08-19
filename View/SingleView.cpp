//#include "Global.h"
#include "SingleView.h"
#include <Core/Scene.h>

SingleView::SingleView(OpenGLBackend *backend, unsigned int splitX,unsigned int splitY,unsigned int width,unsigned int height)
    :SplitedView(backend, splitX,splitY,width,height)
{
    m_viewCount=1;
    m_selected=0;
    m_camera[0]=new PerspectiveCamera(backend, 25.0f,(float)width,(float)height,1.0f,8000.0f,0.0f,0.0f,Vector(0.0f,0.0f,0.0f),Vector(200.0f,200.0f,0.0f),Vector(0.0f,0.0f,1.0f));
    // camera[0]=new BottomCamera( width, height,1,8000,0,0,Vector(0,0,0),Vector(0,0,200),Vector(0,1,0));
    // camera[0]=new FrontCamera( width, height,1,8000,0,0,Vector(0,0,0),Vector(0,200,0),Vector(0,0,1));
    // camera[0]=new BackCamera( width, height,1,8000,0,0,Vector(0,0,0),Vector(0,200,0),Vector(0,0,1));
    // camera[0]=new RightCamera( width, height,1,8000,0,0,Vector(0,0,0),Vector(200,0,0),Vector(0,0,1));
    // camera[0]=new LeftCamera( width, height,1,8000,0,0,Vector(0,0,0),Vector(200,0,0),Vector(0,0,1));
    // camera[0]=new TopCamera( width, height,1,8000,0,0,Vector(0,0,0),Vector(0,0,200),Vector(0,1,0));
}

SingleView::~SingleView(void)
{
    delete  m_camera[0];
}

CameraMode::__Enum SingleView::setView(unsigned int index)
{
    index;
    m_camera[0]->setCamera();
    return m_camera[0]->m_type._value;
}

bool SingleView::onPanPress(unsigned int x,unsigned int y)
{
    m_camera[m_selected]->onPanPress(x,y);
    return true;
}

bool SingleView::onPanDrag(unsigned int x,unsigned int y)
{
    m_camera[m_selected]->pan(x,y);
    return true;
}

void SingleView::onWheel(float step,unsigned int x,unsigned int y)
{
    x;
    y;
    m_camera[m_selected]->zoom(step);
}

bool SingleView::onRotateDrag(unsigned int x,unsigned int y)
{
    m_camera[m_selected]->rotate(x,y);
    return true;
}

bool SingleView::onRotatePress(unsigned int x,unsigned int y)
{
    m_camera[m_selected]->onRotatePress(x,y);
    return true;
}

bool SingleView::onCtrlPress(unsigned int x,unsigned int y)
{
    m_isCtrlMode=true;
    m_ctrlSX=x;
    m_ctrlSY=y;
    return true;
}

bool SingleView::onCtrlRelease()
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

bool SingleView::onCtrlDrag(unsigned int x,unsigned int y)
{
    if(m_isCtrlMode)
    {
        m_camera[m_selected]->setCamera();
        Vector horizontalDir(m_camera[m_selected]->getHorizontalDir());
        Scene::scene->ctrlDrag(horizontalDir,m_camera[m_selected]->m_up,x-m_ctrlSX,y-m_ctrlSY,m_isExtrude);
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

bool SingleView::onRotateRelease()
{
    m_camera[m_selected]->onRotateRelease();
    return true;
}

bool SingleView::onPanRelease()
{
    m_camera[m_selected]->onPanRelease(0,0);
    return true;
}

void SingleView::update(unsigned int splitX, unsigned int splitY, unsigned int width, unsigned int height)
{
    m_splitX = splitX;
    m_splitY = splitY;
    m_width = width;
    m_height = height;
    m_camera[0]->updateSize(0.0f,0.0f,(float)m_width,(float)m_height);
}

bool SingleView::onSelectionPress(unsigned int x,unsigned int y)
{
    m_isSelectionMode=true;
    m_selectionSX=x;
    m_selectionSY=y;
    m_selectionEX=x;
    m_selectionEY=y;
    return true;
}

bool SingleView::onSelectionDrag(unsigned int x,unsigned int y)
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

bool SingleView::onSingleSideSelectionRelease(bool isAppend)
{
    if(m_isSelectionMode)
    {
        qDebug() << "single selection mode =";
        unsigned int x1=(m_selectionSX>m_selectionEX)?m_selectionEX:m_selectionSX;
        unsigned int y1=(m_selectionSY>m_selectionEY)?m_selectionEY:m_selectionSY;
        unsigned int x2=(m_selectionSX>m_selectionEX)?m_selectionSX:m_selectionEX;
        unsigned int y2=(m_selectionSY>m_selectionEY)?m_selectionSY:m_selectionEY;
        m_camera[m_selected]->setCameraForSelectionS();
        Scene::scene->selectSingleSide(x1,y1,x2,y2,m_height,isAppend);
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

bool SingleView::onDualSideSelectionRelease(bool isAppend)
{
    if(m_isSelectionMode)
    {
        unsigned int x1=(m_selectionSX>m_selectionEX)?m_selectionEX:m_selectionSX;
        unsigned int y1=(m_selectionSY>m_selectionEY)?m_selectionEY:m_selectionSY;
        unsigned int x2=(m_selectionSX>m_selectionEX)?m_selectionSX:m_selectionEX;
        unsigned int y2=(m_selectionSY>m_selectionEY)?m_selectionSY:m_selectionEY;
        m_camera[m_selected]->setCameraForSelectionD(x1,y1,x2,y2,m_height);
        Scene::scene->selectDualSide(isAppend);
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

bool SingleView::onAxisDrag(unsigned int x,unsigned int y)
{
    if(m_isAxisMode)
    {
        m_camera[0]->setCamera();
        Vector m((float)((int)x-(int)m_axisDragSX),(float)((int)y-(int)m_axisDragSY),0.0f);
        float work=psVECDOT(m_cursorDir,m);
        Scene::scene->axisDrag(work,m_cursorMode,m_cursorDir.x,m_cursorDir.y);
        m_axisDragSX=x;
        m_axisDragSY=y;
        return true;
    }
    else
    {
        return false;
    }
}

bool SingleView::onAxisRelease()
{
    if(m_isAxisMode)
    {
        m_isAxisMode=false;
        Scene::scene->axisRelease();
        m_axisDragSX=0;
        m_axisDragSY=0;
        return true;
    }
    else
    {
        return false;
    }
}

bool SingleView::onAxisPress(unsigned int x,unsigned int y)
{
    m_camera[m_selected]->setCameraForSelectionS();
    m_cursorDir.z=0;
    bool result=Scene::scene->isAxisSelected(m_camera[m_selected]->m_type._value,m_camera[m_selected]->getEye(),m_height,x,y,m_cursorDir.x,m_cursorDir.y,m_cursorMode);
    if(result)
    {
        m_axisDragSX=x;
        m_axisDragSY=y;
        m_cursorDir.normalize();
        m_isAxisMode=true;
    }
    return result;
}

void SingleView::onPaint()
{
    if(m_isSelectionMode)
    {
        /*glEnable(GL_ALPHA_TEST);
        glColor4ub(0,0,255,125);
        glBegin(GL_QUADS);
            glVertex2i(m_selectionSX,m_selectionSY);
            glVertex2i(m_selectionEX,m_selectionSY);
            glVertex2i(m_selectionEX,m_selectionEY);
            glVertex2i(m_selectionSX,m_selectionEY);
        glEnd();
        glDisable(GL_ALPHA_TEST);
        glColor3ub(255,255,255);*/
    }
}

