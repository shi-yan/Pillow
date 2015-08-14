#include "SplitedView.h"

SplitedView::SplitedView(unsigned int splitX,unsigned int splitY,unsigned int width,unsigned height)
    :m_isAxisMode(false),
      m_isSelectionMode(false),
      m_selectionSX(0),
      m_selectionSY(0),
      m_selectionEX(0),
      m_selectionEY(0),
      m_cursorMode(0),
      m_axisDragSX(0),
      m_axisDragSY(0),
      m_cursorDir(0),
      m_isCtrlMode(false),
      m_ctrlSX(0),
      m_ctrlSY(0),
      m_isExtrude(false)
{
      m_splitX=splitX;
      m_splitY=splitY;
      m_width=width;
      m_height=height;
      m_selected=0;
    m_panIndex=-1;
    m_rotateIndex=-1;
    m_zoomIndex=-1;
}

SplitedView::~SplitedView(void)
{
    delete   m_camera[0];
    delete   m_camera[1];
    delete   m_camera[2];
    delete   m_camera[3];
}

void SplitedView::changeCamera(CameraMode::__Enum type)
{
    unsigned int cameraWidth=(unsigned int)m_camera[m_selected]->m_width;
    unsigned int cameraHeight=(unsigned int)m_camera[m_selected]->m_height;
    unsigned int cameraStartX=(unsigned int)m_camera[m_selected]->m_startX;
    unsigned int cameraStartY=(unsigned int)m_camera[m_selected]->m_startY;
    delete m_camera[m_selected];
    m_camera[m_selected]=NULL;
    if(type==CameraMode::Perspective)
    {
        m_camera[m_selected]=new PerspectiveCamera(25,(float)cameraWidth,(float)cameraHeight,1,8000,(float)cameraStartX,(float)cameraStartY,Vector(0,0,0),Vector(200,200,0),Vector(0,0,1));
    }
    else if(type==CameraMode::Top)
    {
        m_camera[m_selected]=new TopCamera((float)cameraWidth,(float)cameraHeight,1,8000,(float)cameraStartX,(float)cameraStartY,Vector(0,0,0),Vector(0,0,200),Vector(0,1,0));
    }
    else if(type==CameraMode::Bottom)
    {
        m_camera[m_selected]=new BottomCamera((float)cameraWidth,(float)cameraHeight,1,8000,(float)cameraStartX,(float)cameraStartY,Vector(0,0,0),Vector(0,0,200),Vector(0,1,0));
    }
    else if(type==CameraMode::Front)
    {
        m_camera[m_selected]=new FrontCamera((float)cameraWidth,(float)cameraHeight,1,8000,(float)cameraStartX,(float)cameraStartY,Vector(0,0,0),Vector(0,200,0),Vector(0,0,1));
    }
    else if(type==CameraMode::Back)
    {
        m_camera[m_selected]=new BackCamera((float)cameraWidth,(float)cameraHeight,1,8000,(float)cameraStartX,(float)cameraStartY,Vector(0,0,0),Vector(0,200,0),Vector(0,0,1));
    }
    else if(type==CameraMode::Left)
    {
        m_camera[m_selected]=new LeftCamera((float)cameraWidth,(float)cameraHeight,1,8000,(float)cameraStartX,(float)cameraStartY,Vector(0,0,0),Vector(200,0,0),Vector(0,0,1));
    }
    else if(type==CameraMode::Right)
    {
        m_camera[m_selected]=new RightCamera((float)cameraWidth,(float)cameraHeight,1,8000,(float)cameraStartX,(float)cameraStartY,Vector(0,0,0),Vector(200,0,0),Vector(0,0,1));
    }
}

void SplitedView::getViewportImage(GLuint &texID,std::string &path,Vector &position,unsigned int &width,unsigned int &height)
{
    m_camera[m_selected]->getViewportImage(texID,path,position,width,height);
}

void SplitedView::setViewportImage(const char *path,GLuint imageID,Vector &position,unsigned int width,unsigned int height)
{
    m_camera[m_selected]->setReferenceImage(path,imageID,position,width,height);
}

void SplitedView::disableReference()
{
    m_camera[m_selected]->disableReference();
}

void SplitedView::onExtrudeDown()
{
    m_isExtrude=true;
}

void SplitedView::onExtrudeRelease()
{
    m_isExtrude=false;
}

void SplitedView::switchGird()
{
    m_camera[m_selected]->m_showGird=!(m_camera[m_selected]->m_showGird);
}

bool SplitedView::onCtrlPress(unsigned int x,unsigned int y)
{
    x;y;
    return false;
}

bool SplitedView::onCtrlDrag(unsigned int x,unsigned int y)
{
    x;y;
    return false;
}

bool SplitedView::onPanPress(unsigned int x,unsigned int y)
{
    x;
    y;
    return false;
}

bool SplitedView::onAxisPress(unsigned int x,unsigned int y)
{
    x;y;
    return false;
}

bool SplitedView::onAxisDrag(unsigned int x,unsigned int y)
{
    x;y;
    return false;
}

bool SplitedView::onAxisRelease()
{
    return false;
}

bool SplitedView::onCtrlRelease()
{
    return false;
}

bool SplitedView::onPanDrag(unsigned int x,unsigned int y)
{
    x;
    y;
    return false;
}

bool SplitedView::onPanRelease()
{
    return false;
}

bool SplitedView::onRotateDrag(unsigned int x,unsigned int y)
{
    x;
    y;
    return false;
}

bool SplitedView::onRotateRelease()
{
    return false;
}

bool SplitedView::onRotatePress(unsigned int x,unsigned int y)
{
    x;
    y;
    return false;
}

bool SplitedView::onSelectionPress(unsigned int x,unsigned int y)
{
    x;y;
    return false;
}

CameraMode::__Enum SplitedView::setView(unsigned int i)
{
    return CameraMode::Perspective;
    i;
}

Vector SplitedView::getEye(unsigned int i) const
{
    return m_camera[i]->getEye();
}

void SplitedView::onWheel(float step,unsigned int x,unsigned int y)
{
    step;
    x;
    y;
}

void SplitedView::update(unsigned int splitX,unsigned int splitY,unsigned int width,unsigned int height)
{
    splitX;
    splitY;
    width;
    height;
}

void SplitedView::setViewport(unsigned int index)
{
    index;
}

bool SplitedView::onDrag(unsigned int x,unsigned int y)
{
    x;y;
    return false;
}

bool SplitedView::onLeftDown(unsigned int x, unsigned int y)
{
    x;
    y;
    return false;
}

void SplitedView::onPaint(void)
{
}

bool SplitedView::onSelectionDrag(unsigned int x, unsigned int y)
{
    x;
    y;
    return false;
}

bool SplitedView::onSingleSideSelectionRelease(bool isAppend)
{
    isAppend;
    return false;
}

bool SplitedView::onDualSideSelectionRelease(bool isAppend)
{
    isAppend;
    return false;
}

