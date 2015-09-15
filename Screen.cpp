#include "Screen.h"
#include "Core/Scene.h"
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

Screen *Screen::screen = nullptr;

extern Scene *theScene;

Screen::Screen()
    :m_isSelectionMode(false),
      m_isAxisMode(false),
      m_isAltMode(false),
      m_isCtrlMode(false),
      m_isExtrude(false),
      m_width(0),
      m_height(0)
{
    m_graphicsBackend = new OpenGLBackend();
}

void Screen::initialize()
{
    Grid::grid=new Grid(m_graphicsBackend);
    Grid::grid->initialize();
    theScene->initialize();

    m_graphicsBackend->initialize();
    glClearColor(128.0f/255.0f, 128.0f/255.0f, 128.0f/255.0f, 1.0f);

    m_fourView=new FourView(m_graphicsBackend, 2, 2, 4, 4);
    m_threeView=new ThreeView(m_graphicsBackend, 2, 2, 4, 4);
    m_currentView=m_singleView=new SingleView(m_graphicsBackend, 2, 2, 4, 4);
    m_twoView=new TwoView(m_graphicsBackend, 2, 2, 4, 4);
    m_uiLayer=new UILayer(m_graphicsBackend, m_width, m_height);
}

void Screen::updateScreenSize(unsigned int width, unsigned int height)
{
    m_width = width;
    m_height = height;
    m_singleView->update(m_width/2, m_height/2, m_width, m_height);
    m_fourView->update(m_width/2, m_height/2, m_width, m_height);
    m_threeView->update(m_width/2, m_height/2, m_width, m_height);
    m_twoView->update(m_width/2, m_height/2, m_width, m_height);
    m_uiLayer->updateSize(m_width, m_height);
}

bool Screen::onLeftDrag(int x, int y)
{
    if(m_isAltMode)
    {
        return m_currentView->onRotateDrag(x, y);
    }
    else
    {
        if(m_isAxisMode)
        {
            return m_currentView->onAxisDrag(x, y);
        }
        else if(m_isCtrlMode)
        {
            return m_currentView->onCtrlDrag(x, y);
        }
        else if(m_isSelectionMode)
        {
            return m_currentView->onSelectionDrag(x, y);
        }
        else
        {
            return false;
        }
    }
}

bool Screen::onMiddleDrag(int x,int y)
{
    if(m_isAltMode)
    {
        return m_currentView->onPanDrag(x,y);
    }
    else
    {
        return m_currentView->onSelectionDrag(x, y);
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
    glViewport(0,0,(GLint) m_width, (GLint) m_height);

    for(int i=0;i<(int)m_currentView->m_viewCount;i++)
    {
        CameraMode::__Enum cameraMode = m_currentView->setView(i);
        //theScene->onPaint();
        theScene->drawCursor(cameraMode,m_currentView->getEye(i));
    }


    m_graphicsBackend->beginUI(m_width, m_height);
    //glColor3ub(255,255,255);
  //  glLineWidth(3);
   // currentView->onPaint();
    m_uiLayer->onPaint();
    m_graphicsBackend->endUI();
}

void Screen::changeCamera(CameraMode::__Enum type)
{
    m_currentView->changeCamera(type);
}

void Screen::disableReference()
{
    m_currentView->disableReference();
}

void Screen::getViewportImage(GLuint &textID, std::string &path, Vector &position, unsigned int &width, unsigned int &height)
{
    m_currentView->getViewportImage(textID, path, position, width, height);
}

void Screen::setViewportImage(const char *path, GLuint imageID, Vector &position, unsigned int width, unsigned int height)
{
    m_currentView->setViewportImage(path, imageID, position, width, height);
}

void Screen::onCtrlDown()
{
    m_isCtrlMode = true;
}

void Screen::onCtrlUp()
{
    m_isCtrlMode = false;
}

void Screen::changeView(int type)
{
    switch(type)
    {
    case Four:
        m_currentView = m_fourView;
        break;
    case Three:
        m_currentView = m_threeView;
        break;
    case Two:
        m_currentView = m_twoView;
        break;
    case Single:
        m_currentView = m_singleView;
        break;
    default:
        qDebug() << "error";
    }
}

void Screen::onAltDown()
{
    if(!m_isSelectionMode)
    {
        m_isAltMode = true;
    }
}

void Screen::onAltUp()
{
    m_isAltMode = false;
}

bool Screen::onMiddleRelease(bool isAppend)
{
    if(m_isAltMode)
    {
        m_currentView->onPanRelease();
        m_isAltMode = false;
        return true;
    }
    else
    {
        qDebug() << "dual side selection";
        m_currentView->onDualSideSelectionRelease(isAppend);
        m_isSelectionMode = false;
        return true;
    }
}

bool Screen::onLeftPress(int x, int y)
{
    if(m_isAltMode)
    {
        m_currentView->onRotatePress(x, y);
        return true;
    }
    else
    {
        if(m_uiLayer->onLeftPress(x,y))
        {
            return true;
        }
        else
        {
            if(m_currentView->onLeftDown(x, y))
            {
                m_isAltMode = false;
                m_isAxisMode = false;
                m_isSelectionMode = false;
                m_isCtrlMode = false;
                return true;
            }
            else
            {
                if(m_currentView->onAxisPress(x, y))
                {
                    m_isAxisMode = true;
                    return true;
                }
                else
                {
                    if(m_isCtrlMode)
                    {
                        m_currentView->onCtrlPress(x, y);
                        return true;
                    }
                    else
                    {
                        qDebug() << "single side selection";

                        m_currentView->onSelectionPress(x, y);
                        m_isSelectionMode = true;
                        m_isAxisMode = false;
                        return true;
                    }
                }
            }
        }
    }
}

bool Screen::onMoving(int x, int y)
{
    return m_uiLayer->onMoving(x, y);
}

void Screen::onExtrudePress()
{
    if(!m_isExtrude)
    {
        m_currentView->onExtrudeDown();
        m_isExtrude = true;
    }
}

void Screen::onExtrudeRelease()
{
    m_currentView->onExtrudeRelease();
    m_isExtrude = false;
}

bool Screen::onLeftRelease(bool isAppend)
{
    if(m_isAltMode)
    {
        m_currentView->onRotateRelease();
        m_isAltMode = false;
        return true;
    }
    else
    {
        ButtonEventType eventType = m_uiLayer->onLeftRelease();
        if(eventType!=ButtonEventType::Nothing)
        {
            switch(eventType)
            {
            case ButtonEventType::Smooth:
                toolButtonSmooth();
                break;
            case ButtonEventType::Wire:
                toolButtonWire();
                break;
            case ButtonEventType::Faced:
                toolButtonFaced();
                break;
            case ButtonEventType::Wireframe:
                toolButtonWireframe();
                break;
            case ButtonEventType::WireframeFaced:
                toolButtonWireframeFaced();
                break;
            case ButtonEventType::SingleView:
                toolButtonSingleView();
                break;
            case ButtonEventType::TwoView:
                toolButtonTwoView();
                break;
            case ButtonEventType::ThreeView:
                toolButtonThreeView();
                break;
            case ButtonEventType::FourView:
                toolButtonFourView();
                break;
            case ButtonEventType::VertexSelectionMode:
                theScene->changeSelectionMode(SelectionMode::Vertex);
                break;
            case ButtonEventType::EdgeSelectionMode:
                theScene->changeSelectionMode(SelectionMode::Edge);
                break;
            case ButtonEventType::FaceSelectionMode:
                theScene->changeSelectionMode(SelectionMode::Face);
                break;
            case ButtonEventType::ObjectSelectionMode:
                theScene->changeSelectionMode(SelectionMode::Object);
                break;
            case ButtonEventType::Help:
                //wxLaunchDefaultBrowser(_T("http://billconan.blogspot.com"));
                break;
            case ButtonEventType::CreateCube:
                theScene->newCube(0, 0, 0, 100, 100, 100, 1, 1, 1);
                break;
            case ButtonEventType::CreateCylinder:
                theScene->newCylinder(0, 0, 0, 20, 80, AxisMode::axisZ, 8, 1, 1);
                break;
            case ButtonEventType::CreateSphere:
                theScene->newSphere(0,0,0,50,AxisMode::axisZ,8,4);
                break;
            case ButtonEventType::CreatePlane:
                theScene->newPlane(0, 0, 0, 100, 100, AxisMode::axisZ, 1, 1);
                break;
            case ButtonEventType::CameraBack:
                changeCamera(CameraMode::Back);
                break;
            case ButtonEventType::CameraFront:
                changeCamera(CameraMode::Front);
                break;
            case ButtonEventType::CameraBottom:
                changeCamera(CameraMode::Bottom);
                break;
            case ButtonEventType::CameraTop:
                changeCamera(CameraMode::Top);
                break;
            case ButtonEventType::CameraLeft:
                changeCamera(CameraMode::Left);
                break;
            case ButtonEventType::CameraRight:
                changeCamera(CameraMode::Right);
                break;
            case ButtonEventType::CameraPerspective:
                changeCamera(CameraMode::Perspective);
                break;
            case ButtonEventType::Screenshot:
            {
                //wxString filename;
                //filename<<wxDateTime::GetTimeNow()<<_T(".jpg");
                //screenShot(filename.c_str());
            }
                break;
            case ButtonEventType::About:
            {
                //AboutDialog *aboutDialog=new AboutDialog(mainFrame);
                //aboutDialog->ShowModal();
                //aboutDialog->Destroy();
            }
                break;
            case ButtonEventType::New:
                theScene->clearScene();
                break;
           case ButtonEventType::Open:
           {
                /*wxString wildcards=wxString::Format(_T("PWB files (*.pwb)|*.pwb"),wxFileSelectorDefaultWildcardStr,wxFileSelectorDefaultWildcardStr);
                wxFileDialog dialog(mainFrame, _T("Open file:"),wxEmptyString, wxEmptyString, wildcards,wxOPEN);
                if(dialog.ShowModal()==wxID_OK)
                {
                    wxArrayString paths, filenames;
                    dialog.GetPaths(paths);
                    dialog.GetFilenames(filenames);
                    if(dialog.GetFilterIndex()==0)
                    {
                        theScene->clearScene();
                        theScene->loadFromPWB(paths[0].c_str());
                    }
                }*/
            }
                break;
            case ButtonEventType::Save:
            {
                if(theScene->fileName=="")
                {
                    /*wxFileDialog dialog(mainFrame,_T("Save file:"),wxEmptyString,_T("untitle"),_T("Pillow Binary Files (*.pwb)|*.pwb"),wxSAVE|wxOVERWRITE_PROMPT);
                    dialog.SetFilterIndex(1);
                    if (dialog.ShowModal() == wxID_OK)
                    {
                        //wxLogMessage(_T("%s, filter %d"),dialog.GetPath().c_str(), dialog.GetFilterIndex());
                        if(dialog.GetFilterIndex()==0)
                        {
                            theScene->fileName=dialog.GetPath().c_str();
                            theScene->saveToFilePWB(dialog.GetPath().c_str());
                        }
                    }*/
                }
                else
                {
                    theScene->saveToFilePWB(theScene->fileName.c_str());
                }
            }
                break;
            case ButtonEventType::SaveAs:
            {
                /*wxFileDialog dialog(mainFrame,_T("Save file:"),wxEmptyString,_T("untitle"),_T("Pillow Binary Files (*.pwb)|*.pwb|OBJ Files (*.obj)|*.obj"),wxSAVE|wxOVERWRITE_PROMPT);
                dialog.SetFilterIndex(0);
                if (dialog.ShowModal() == wxID_OK)
                {
                    //wxLogMessage(_T("%s, filter %d"),dialog.GetPath().c_str(), dialog.GetFilterIndex());
                    if(dialog.GetFilterIndex()==0)
                    {
                        theScene->fileName=dialog.GetPath().c_str();
                        theScene->saveToFilePWB(dialog.GetPath().c_str());
                    }
                    else if(dialog.GetFilterIndex()==1)
                    {
                        theScene->fileName=dialog.GetPath().c_str();
                        theScene->saveToFileOBJ(dialog.GetPath().c_str());
                    }
                }*/
            }
                break;
            case ButtonEventType::DeleteVertex:
                theScene->deleteVertex();
                break;
            case ButtonEventType::Split:
                if(!theScene->isSplitMode)
                {
                    theScene->beginSplit();
                }
                else
                {
                    theScene->endSplit();
                }
                break;
            case ButtonEventType::Extrude:
                theScene->extrudeFaceGroup(1, 1, 1);
                break;
            case ButtonEventType::Collapse:
                theScene->weldVertex();
                break;
            default:
                qDebug() << "error: undefined event";
                return false;
            }
            return true;
        }
        else
        {
            if(m_currentView->onAxisRelease())
            {
                m_isAxisMode = false;
                return true;
            }
            else
            {
                if(m_currentView->onCtrlRelease())
                {
                    m_isCtrlMode = false;
                    return true;
                }
                else
                {
                    m_currentView->onSingleSideSelectionRelease(isAppend);
                    m_isSelectionMode = false;
                    return true;
                }
            }
        }
    }
}

void Screen::toolButtonWireframeFaced()
{
    theScene->changeShadeMode(RenderType::WireframeFaced);
}

void Screen::toggleGrid()
{
    m_currentView->toggleGrid();
}

void Screen::toolButtonSmooth()
{
    theScene->changeShadeMode(RenderType::Smooth);
}

void Screen::toolButtonSingleView()
{
    changeView(Single);
}

void Screen::toolButtonTwoView()
{
    changeView(Two);
}

void Screen::toolButtonThreeView()
{
    changeView(Three);
}

void Screen::toolButtonFourView()
{
    changeView(Four);
}

void Screen::toolButtonFaced()
{
    theScene->changeShadeMode(RenderType::Faced);
}

void Screen::toolButtonWireframe()
{
    theScene->changeShadeMode(RenderType::Wireframe);
}

void Screen::toolButtonWire()
{
    theScene->changeShadeMode(RenderType::Wire);
}

bool Screen::onMiddlePress(int x, int y)
{
    if(m_isAltMode)
    {
        m_currentView->onPanPress(x, y);
        return true;
    }
    else
    {
        m_currentView->onSelectionPress(x, y);
        m_isSelectionMode = true;
        return true;
    }
}

bool Screen::onWheel(float step, int x, int y)
{
    if(m_isAltMode)
    {
        m_currentView->onWheel(step, x, y);
        return true;
    }
    else
    {
        return false;
    }
}

Screen::~Screen(void)
{
    delete m_fourView;
    delete m_twoView;
    delete m_threeView;
    delete m_singleView;
    delete m_uiLayer;
}
