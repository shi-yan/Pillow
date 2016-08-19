#include "UILayer.h"
#include <QDebug>

UILayer::UILayer(OpenGLBackend *backend, unsigned int width,unsigned int height)
    :m_backend(backend),
      m_currentIn(NULL),
      m_width(width),
      m_height(height)
{

    m_modifyModeStrip = new ToolStrip(m_backend,1,1,42,42,false,4,0,0,42.0f/256.0f,11.0f/256.0f,0,12.0f/256.0f,0,30.0f/256.0f,0,31.0f/256.0f,42.0f/256.0f,42.0f/256.0f);
    Button *vertexButton=new Button(m_backend,ButtonEventType::VertexSelectionMode,109.0f/256.0f,117.0f/256.0f,141.0f/256.0f,149.0f/256.0f,32,32);
    Button *edgeButton=new Button(m_backend,ButtonEventType::EdgeSelectionMode,73.0f/256.0f,117.0f/256.0f,105.0f/256.0f,149.0f/256.0f,32,32);
    Button *faceButton=new Button(m_backend,ButtonEventType::FaceSelectionMode,37.0f/256.0f,117.0f/256.0f,69.0f/256.0f,149.0f/256.0f,32,32);
    Button *objectButton=new Button(m_backend,ButtonEventType::ObjectSelectionMode,1.0f/256.0f,117.0f/256.0f,33.0f/256.0f,149.0f/256.0f,32,32);
    m_modifyModeStrip->add(vertexButton);
    m_modifyModeStrip->add(edgeButton);
    m_modifyModeStrip->add(faceButton);
    m_modifyModeStrip->add(objectButton);

    m_layoutModeStrip=new ToolStrip(m_backend,1,1,42,42,false,4,0,0,42.0f/256.0f,11.0f/256.0f,0,12.0f/256.0f,0,30.0f/256.0f,0,31.0f/256.0f,42.0f/256.0f,42.0f/256.0f);
    Button *singleViewButton=new Button(m_backend,ButtonEventType::SingleView,1.0f/256.0f,45.0f/256.0f,33.0f/256.0f,77.0f/256.0f,32,32);
    Button *twoViewButton=new Button(m_backend,ButtonEventType::TwoView,37.0f/256.0f,45.0f/256.0f,69.0f/256.0f,77.0f/256.0f,32,32);
    Button *threeViewButton=new Button(m_backend,ButtonEventType::ThreeView,73.0f/256.0f,45.0f/256.0f,105.0f/256.0f,77.0f/256.0f,32,32);
    Button *fourViewButton=new Button(m_backend,ButtonEventType::FourView,109.0f/256.0f,45.0f/256.0f,141.0f/256.0f,77.0f/256.0f,32,32);
    m_layoutModeStrip->add(singleViewButton);
    m_layoutModeStrip->add(twoViewButton);
    m_layoutModeStrip->add(threeViewButton);
    m_layoutModeStrip->add(fourViewButton);

    m_shadeModeStrip=new ToolStrip(m_backend,1,1,42,42,true,4,0,0,11.0f/256.0f,42.0f/256.0f,12.0f/256.0f,0,30.0f/256.0f,0,31.0f/256.0f,0,42.0f/256.0f,42.0f/256.0f);
    Button *facedButton=new Button(m_backend,ButtonEventType::Faced,1.0f/256.0f,81.0f/256.0f,33.0f/256.0f,113.0f/256.0f,32,32);
    Button *wireframeButton=new Button(m_backend,ButtonEventType::Wireframe,37.0f/256.0f,81.0f/256.0f,69.0f/256.0f,113.0f/256.0f,32,32);
    Button *wireframeFacedButton=new Button(m_backend,ButtonEventType::WireframeFaced,73.0f/256.0f,9.0f/256.0f,105.0f/256.0f,41.0f/256.0f,32,32);
    Button *wireButton=new Button(m_backend,ButtonEventType::Wire,73.0f/256.0f,81.0f/256.0f,105.0f/256.0f,113.0f/256.0f,32,32);
    Button *smoothButton=new Button(m_backend,ButtonEventType::Smooth,109.0f/256.0f,81.0f/256.0f,141.0f/256.0f,113.0f/256.0f,32,32);
    m_shadeModeStrip->add(facedButton);
    m_shadeModeStrip->add(wireframeButton);
    m_shadeModeStrip->add(wireframeFacedButton);
    m_shadeModeStrip->add(wireButton);
    m_shadeModeStrip->add(smoothButton);

    m_fileStrip=new ToolStrip(m_backend,1,1,42,42,true,4,0,0,11.0f/256.0f,42.0f/256.0f,12.0f/256.0f,0,30.0f/256.0f,0,31.0f/256.0f,0,42.0f/256.0f,42.0f/256.0f);
    Button* newButton=new Button(m_backend,ButtonEventType::New,1.0f/256.0f,153.0f/256.0f,33.0f/256.0f,185.0f/256.0f,32,32);
    Button* openButton=new Button(m_backend,ButtonEventType::Open,37.0f/256.0f,153.0f/256.0f,69.0f/256.0f,185.0f/256.0f,32,32);
    Button* saveButton=new Button(m_backend,ButtonEventType::Save,73.0f/256.0f,153.0f/256.0f,105.0f/256.0f,185.0f/256.0f,32,32);
    Button* saveAsButton=new Button(m_backend,ButtonEventType::SaveAs,145.0f/256.0f,117.0f/256.0f,177.0f/256.0f,149.0f/256.0f,32,32);
    Button* undoButton=new Button(m_backend,ButtonEventType::Nothing,109.0f/256.0f,153.0f/256.0f,141.0f/256.0f,185.0f/256.0f,32,32);
    Button* redoButton=new Button(m_backend,ButtonEventType::Nothing,145.0f/256.0f,153.0f/256.0f,177.0f/256.0f,185.0f/256.0f,32,32);
    m_fileStrip->add(newButton);
    m_fileStrip->add(openButton);
    m_fileStrip->add(saveButton);
    m_fileStrip->add(saveAsButton);
    m_fileStrip->add(undoButton);
    m_fileStrip->add(redoButton);

    m_createStrip=new ToolStrip(m_backend,1,1,42,42,true,4,0,0,11.0f/256.0f,42.0f/256.0f,12.0f/256.0f,0,30.0f/256.0f,0,31.0f/256.0f,0,42.0f/256.0f,42.0f/256.0f);
    Button* boxButton=new Button(m_backend,ButtonEventType::CreateCube,1.0f/256.0f,189.0f/256.0f,33.0f/256.0f,221.0f/256.0f,32,32);
    Button* ballButton=new Button(m_backend,ButtonEventType::CreateSphere,37.0f/256.0f,189.0f/256.0f,69.0f/256.0f,221.0f/256.0f,32,32);
    Button* cylinderButton=new Button(m_backend,ButtonEventType::CreateCylinder,73.0f/256.0f,189.0f/256.0f,105.0f/256.0f,221.0f/256.0f,32,32);
    Button* planeButton=new Button(m_backend,ButtonEventType::CreatePlane,109.0f/256.0f,189.0f/256.0f,141.0f/256.0f,221.0f/256.0f,32,32);
    m_createStrip->add(boxButton);
    m_createStrip->add(ballButton);
    m_createStrip->add(cylinderButton);
    m_createStrip->add(planeButton);

    m_modifyStrip=new ToolStrip(m_backend,1,1,42,42,true,4,0,0,11.0f/256.0f,42.0f/256.0f,12.0f/256.0f,0,30.0f/256.0f,0,31.0f/256.0f,0,42.0f/256.0f,42.0f/256.0f);
    Button* deleteVertexButton=new Button(m_backend,ButtonEventType::DeleteVertex,217.0f/256.0f,189.0f/256.0f,249.0f/256.0f,221.0f/256.0f,32,32);
    Button* splitButton=new Button(m_backend,ButtonEventType::Split,217.0f/256.0f,153.0f/256.0f,249.0f/256.0f,185.0f/256.0f,32,32);
    Button* extrudeFaceButton=new Button(m_backend,ButtonEventType::Extrude,217.0f/256.0f,117.0f/256.0f,249.0f/256.0f,149.0f/256.0f,32,32);
    Button* weldButton=new Button(m_backend,ButtonEventType::Collapse,217.0f/256.0f,81.0f/256.0f,249.0f/256.0f,113.0f/256.0f,32,32);
    m_modifyStrip->add(deleteVertexButton);
    m_modifyStrip->add(splitButton);
    m_modifyStrip->add(extrudeFaceButton);
    m_modifyStrip->add(weldButton);

    m_helpStrip=new ToolStrip(m_backend,1,1,42,42,true,4,0,0,11.0f/256.0f,42.0f/256.0f,12.0f/256.0f,0,30.0f/256.0f,0,31.0f/256.0f,0,42.0f/256.0f,42.0f/256.0f);
    Button* screenshotButton=new Button(m_backend,ButtonEventType::Screenshot,145.0f/256.0f,81.0f/256.0f,177.0f/256.0f,113.0f/256.0f,32,32);
    Button* aboutButton=new Button(m_backend,ButtonEventType::About,145.0f/256.0f,45.0f/256.0f,177.0f/256.0f,77.0f/256.0f,32,32);
    m_helpStrip->add(screenshotButton);
    m_helpStrip->add(aboutButton);

    m_cameraStrip=new ToolStrip(m_backend,1,1,42,42,true,4,0,0,11.0f/256.0f,42.0f/256.0f,12.0f/256.0f,0,30.0f/256.0f,0,31.0f/256.0f,0,42.0f/256.0f,42.0f/256.0f);
    Button* bottomButton=new Button(m_backend,ButtonEventType::CameraBottom,217.0f/256.0f,45.0f/256.0f,249.0f/256.0f,77.0f/256.0f,32,32);
    Button* topButton=new Button(m_backend,ButtonEventType::CameraTop,181.0f/256.0f,81.0f/256.0f,213.0f/256.0f,113.0f/256.0f,32,32);
    Button* leftButton=new Button(m_backend,ButtonEventType::CameraLeft,181.0f/256.0f,117.0f/256.0f,213.0f/256.0f,149.0f/256.0f,32,32);
    Button* rightButton=new Button(m_backend,ButtonEventType::CameraRight,181.0f/256.0f,153.0f/256.0f,213.0f/256.0f,185.0f/256.0f,32,32);
    Button* frontButton=new Button(m_backend,ButtonEventType::CameraFront,181.0f/256.0f,189.0f/256.0f,213.0f/256.0f,221.0f/256.0f,32,32);
    Button* backButton=new Button(m_backend,ButtonEventType::CameraBack,145.0f/256.0f,189.0f/256.0f,177.0f/256.0f,221.0f/256.0f,32,32);
    Button* perspectiveButton=new Button(m_backend,ButtonEventType::CameraPerspective,181.0f/256.0f,45.0f/256.0f,213.0f/256.0f,77.0f/256.0f,32,32);
    m_cameraStrip->add(bottomButton);
    m_cameraStrip->add(topButton);
    m_cameraStrip->add(leftButton);
    m_cameraStrip->add(rightButton);
    m_cameraStrip->add(frontButton);
    m_cameraStrip->add(backButton);
    m_cameraStrip->add(perspectiveButton);
}

void UILayer::updateCameraStrip()
{
    m_cameraStrip->updatePosition((int)(((int)(m_width-m_shadeModeStrip->m_width-10-m_cameraStrip->m_width))*0.5f)+m_shadeModeStrip->m_width+10,m_height-m_shadeModeStrip->m_height-1);
}

bool UILayer::onMoving(int x,int y)
{
    if(m_currentIn)
    {
        if(m_currentIn->onOut(x,y))
        {
            m_currentIn = NULL;
        }
    }

    if(m_fileStrip->onIn(x,y))
    {
        m_currentIn = m_fileStrip;
    }
    else if(m_modifyModeStrip->onIn(x,y))
    {
        m_currentIn = m_modifyModeStrip;
    }
    else if(m_layoutModeStrip->onIn(x,y))
    {
        m_currentIn = m_layoutModeStrip;
    }
    else if(m_shadeModeStrip->onIn(x,y))
    {
        m_currentIn = m_shadeModeStrip;
    }
    else if(m_createStrip->onIn(x,y))
    {
        m_currentIn = m_createStrip;
    }
    else if(m_modifyStrip->onIn(x,y))
    {
        m_currentIn = m_modifyStrip;
    }
    else if(m_helpStrip->onIn(x,y))
    {
        m_currentIn = m_helpStrip;
    }
    else if(m_cameraStrip->onIn(x,y))
    {
        m_currentIn = m_cameraStrip;
    }
    else
    {
        return false;
    }
    return true;
}

ButtonEventType UILayer::onLeftRelease()
{
    if(m_currentIn)
    {
        return m_currentIn->onLeftRelease();
    }
    else
    {
        return ButtonEventType::Nothing;
    }
}

bool UILayer::onLeftPress(int mx,int my)
{
    if(m_currentIn && m_currentIn->onLeftPress(mx,my))
    {
        return true;
    }
    return false;
}

void UILayer::updateModifyStrip()
{
    m_modifyStrip->updatePosition((int)(((int)(m_width-(m_fileStrip->m_width+10+m_createStrip->m_width+10+m_modifyStrip->m_width+10+m_helpStrip->m_width)))*0.5f)+m_fileStrip->m_width+20+m_createStrip->m_width,1);
}

void UILayer::updateHelpStrip()
{
    m_helpStrip->updatePosition((int)(((int)(m_width-(m_fileStrip->m_width+10+m_createStrip->m_width+10+m_modifyStrip->m_width+10+m_helpStrip->m_width)))*0.5f)+m_fileStrip->m_width+30+m_createStrip->m_width+m_modifyStrip->m_width,1);
}

void UILayer::updateCreateStrip()
{
    m_createStrip->updatePosition((int)(((int)(m_width-(m_fileStrip->m_width+10+m_createStrip->m_width+10+m_modifyStrip->m_width+10+m_helpStrip->m_width)))*0.5f)+m_fileStrip->m_width+10,1);
}

void UILayer::updateModifyModeStrip()
{
    m_modifyModeStrip->updatePosition(1, m_height*0.5f - m_modifyModeStrip->m_height*0.5f);
}

void UILayer::updateLayoutModeStrip()
{
    m_layoutModeStrip->updatePosition(m_width-m_layoutModeStrip->m_width-1,m_height*0.5f-m_layoutModeStrip->m_height*0.5f);
}

void UILayer::updateShadeModeStrip()
{
    m_shadeModeStrip->updatePosition((int)(((int)(m_width-m_shadeModeStrip->m_width-10-m_cameraStrip->m_width))*0.5f),m_height-m_shadeModeStrip->m_height-1);
}

void UILayer::updateFileStrip()
{
    m_fileStrip->updatePosition(((int)((int)(m_width-(m_fileStrip->m_width+10+m_createStrip->m_width+10+m_modifyStrip->m_width+10+m_helpStrip->m_width)))*0.5f),1);
}

void UILayer::onPaint()
{
    m_modifyModeStrip->onPaint();
   /* m_layoutModeStrip->onPaint();
    m_shadeModeStrip->onPaint();
    m_fileStrip->onPaint();
    m_createStrip->onPaint();
    m_modifyStrip->onPaint();
    m_helpStrip->onPaint();
    m_cameraStrip->onPaint();*/
}

void UILayer::updateSize(unsigned int width, unsigned int height)
{
    m_width=width;
    m_height=height;

    updateModifyModeStrip();
 /*   updateLayoutModeStrip();
    updateShadeModeStrip();
    updateFileStrip();
    updateCreateStrip();
    updateModifyStrip();
    updateHelpStrip();
    updateCameraStrip();*/
}

UILayer::~UILayer(void)
{
    delete m_fileStrip;
    delete m_modifyModeStrip;
    delete m_layoutModeStrip;
    delete m_shadeModeStrip;
    delete m_createStrip;
    delete m_modifyStrip;
    delete m_helpStrip;
    delete m_cameraStrip;
}
