#include "Screen.h"
#include "Scene.h"
#include "View/SingleView.h"
#include "View/FourView.h"
#include "View/ThreeView.h"
#include "View/TwoView.h"
#include "Camera/Camera.h"
#include "UI/Grid.h"
#include "Core/Matrix.h"
#include "Core/Vector.cpp"
#include "Core/Quaternion.h"
#include "View/ViewportImage.h"

extern Scene *theScene;

Screen::Screen(/*wxWindow *theMain*/)
    :/*mainFrame(theMain),*/isSelectionMode(false),isAxisMode(false),isAltMode(false),isCtrlMode(false),isExtrude(false)
{
    m_graphicsBackend = new OpenGLBackend();
}

void Screen::initialize()
{
    Grid::grid=new Grid(m_graphicsBackend);
    Grid::grid->initialize();
    theScene->initialize();
    width=0;
    height=0;

    m_graphicsBackend->initialize();
    glClearColor(128.0f/255.0f,128.0f/255.0f,128.0f/255.0f,1.0f);

  //  glClearColor(1,0,0,1);

    fourView=new FourView(m_graphicsBackend, 2,2,4,4);
    threeView=new ThreeView(m_graphicsBackend, 2,2,4,4);
    currentView=singleView=new SingleView(m_graphicsBackend, 2,2,4,4);
    twoView=new TwoView(m_graphicsBackend, 2,2,4,4);
    uiLayer=new UILayer(m_graphicsBackend, width, height);
}

void Screen::updateScreen(unsigned int theWidth,unsigned int theHeight)
{
    width=theWidth;
    height=theHeight;
    singleView->update(width/2,height/2,width,height);
    fourView->update(width/2,height/2,width,height);
    threeView->update(width/2,height/2,width,height);
    twoView->update(width/2,height/2,width,height);
    uiLayer->updateSize(width,height);
}

bool Screen::onLeftDrag(int x,int y)
{
    if(isAltMode)
    {
        return currentView->onRotateDrag(x,y);
    }
    else
    {
        if(isAxisMode)
        {
            return currentView->onAxisDrag(x,y);
        }
        else if(isCtrlMode)
        {
            return currentView->onCtrlDrag(x,y);
        }
        else if(isSelectionMode)
        {
            return currentView->onSelectionDrag(x,y);
        }
        else
        {
            return false;
        }
    }
}

bool Screen::onMiddleDrag(int x,int y)
{
    if(isAltMode)
    {
        return currentView->onPanDrag(x,y);
    }
    else
    {
        return currentView->onSelectionDrag(x,y);
        //return currentView->onDrag();
    }
}

void Screen::screenShot(const char *fileName)
{
        /*glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);
        for(int i=0;i<(int)  currentView->viewCount;++i)
        {
            CameraMode::__Enum cameraMode=currentView->setView(i);
            theScene->onPaint();
            theScene->drawCursor(cameraMode,currentView->getEye(i));
        }
        glViewport(0,0,(GLint)width,(GLint)height);
        glFlush();
        unsigned char *pixel=(unsigned char *)malloc(sizeof(unsigned char)*width*height*3);
        glReadPixels(0,0,width,height, GL_RGB, GL_UNSIGNED_BYTE, pixel);
        wxImage theScreenShot(width,height,true);
        theScreenShot.SetData(pixel);
        theScreenShot=theScreenShot.Mirror(false);
        theScreenShot.SaveFile(_T(fileName),wxBITMAP_TYPE_JPEG);*/
        //delete pixel;
}

void Screen::onPaint()
{ 
    glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);
    glViewport(0,0,(GLint)  width,(GLint)  height);

    for(int i=0;i<(int)currentView->m_viewCount;i++)
    {
        CameraMode::__Enum cameraMode = currentView->setView(i);
        //theScene->onPaint();
        //theScene->drawCursor(cameraMode,currentView->getEye(i));
    }


    m_graphicsBackend->beginUI(width, height);
  //  Begin2D();
    //glColor3ub(255,255,255);
  //  glLineWidth(3);
   // currentView->onPaint();
    uiLayer->onPaint();
    m_graphicsBackend->endUI();
  //  End2D();
}
