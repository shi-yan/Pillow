#pragma once
#include "ToolStrip.h"
#include <QImage>

class UILayer
{
public:
    unsigned int width;
    unsigned int height;
    ToolStrip *fileStrip;
    ToolStrip *modifyModeStrip;
    ToolStrip *layoutModeStrip;
    ToolStrip *shadeModeStrip;
    ToolStrip *createStrip;
    ToolStrip *modifyStrip;
    ToolStrip *helpStrip;
    ToolStrip *cameraStrip;
    ToolStrip *currentIn;
    GLuint textureID;
public:
    UILayer(unsigned int theWidth,unsigned int theHeight):currentIn(NULL),width(theWidth),height(theHeight)
    {
        //wxImage::AddHandler(new wxPNGHandler);
        textureID=loadTexture("://ui.dat");
        modifyModeStrip=new ToolStrip(1,1,42,42,false,4,0,0,42.0f/256.0f,11.0f/256.0f,0,12.0f/256.0f,0,30.0f/256.0f,0,31.0f/256.0f,42.0f/256.0f,42.0f/256.0f);
        Button *vertexButton=new Button(ButtonEventType::VertexSelectionMode,109.0f/256.0f,117.0f/256.0f,141.0f/256.0f,149.0f/256.0f,32,32);
        Button *edgeButton=new Button(ButtonEventType::EdgeSelectionMode,73.0f/256.0f,117.0f/256.0f,105.0f/256.0f,149.0f/256.0f,32,32);
        Button *faceButton=new Button(ButtonEventType::FaceSelectionMode,37.0f/256.0f,117.0f/256.0f,69.0f/256.0f,149.0f/256.0f,32,32);
        Button *objectButton=new Button(ButtonEventType::ObjectSelectionMode,1.0f/256.0f,117.0f/256.0f,33.0f/256.0f,149.0f/256.0f,32,32);
        modifyModeStrip->add(vertexButton);
        modifyModeStrip->add(edgeButton);
        modifyModeStrip->add(faceButton);
        modifyModeStrip->add(objectButton);
        layoutModeStrip=new ToolStrip(1,1,42,42,false,4,0,0,42.0f/256.0f,11.0f/256.0f,0,12.0f/256.0f,0,30.0f/256.0f,0,31.0f/256.0f,42.0f/256.0f,42.0f/256.0f);
        Button *singleViewButton=new Button(ButtonEventType::SingleView,1.0f/256.0f,45.0f/256.0f,33.0f/256.0f,77.0f/256.0f,32,32);
        Button *twoViewButton=new Button(ButtonEventType::TwoView,37.0f/256.0f,45.0f/256.0f,69.0f/256.0f,77.0f/256.0f,32,32);
        Button *threeViewButton=new Button(ButtonEventType::ThreeView,73.0f/256.0f,45.0f/256.0f,105.0f/256.0f,77.0f/256.0f,32,32);
        Button *fourViewButton=new Button(ButtonEventType::FourView,109.0f/256.0f,45.0f/256.0f,141.0f/256.0f,77.0f/256.0f,32,32);
        layoutModeStrip->add(singleViewButton);
        layoutModeStrip->add(twoViewButton);
        layoutModeStrip->add(threeViewButton);
        layoutModeStrip->add(fourViewButton);
        shadeModeStrip=new ToolStrip(1,1,42,42,true,4,0,0,11.0f/256.0f,42.0f/256.0f,12.0f/256.0f,0,30.0f/256.0f,0,31.0f/256.0f,0,42.0f/256.0f,42.0f/256.0f);
        Button *facedButton=new Button(ButtonEventType::Faced,1.0f/256.0f,81.0f/256.0f,33.0f/256.0f,113.0f/256.0f,32,32);
        Button *wireframeButton=new Button(ButtonEventType::Wireframe,37.0f/256.0f,81.0f/256.0f,69.0f/256.0f,113.0f/256.0f,32,32);
        Button *wireframeFacedButton=new Button(ButtonEventType::WireframeFaced,73.0f/256.0f,9.0f/256.0f,105.0f/256.0f,41.0f/256.0f,32,32);
        Button *wireButton=new Button(ButtonEventType::Wire,73.0f/256.0f,81.0f/256.0f,105.0f/256.0f,113.0f/256.0f,32,32);
        Button *smoothButton=new Button(ButtonEventType::Smooth,109.0f/256.0f,81.0f/256.0f,141.0f/256.0f,113.0f/256.0f,32,32);
        shadeModeStrip->add(facedButton);
        shadeModeStrip->add(wireframeButton);
        shadeModeStrip->add(wireframeFacedButton);
        shadeModeStrip->add(wireButton);
        shadeModeStrip->add(smoothButton);
        fileStrip=new ToolStrip(1,1,42,42,true,4,0,0,11.0f/256.0f,42.0f/256.0f,12.0f/256.0f,0,30.0f/256.0f,0,31.0f/256.0f,0,42.0f/256.0f,42.0f/256.0f);
        Button* newButton=new Button(ButtonEventType::New,1.0f/256.0f,153.0f/256.0f,33.0f/256.0f,185.0f/256.0f,32,32);
        Button* openButton=new Button(ButtonEventType::Open,37.0f/256.0f,153.0f/256.0f,69.0f/256.0f,185.0f/256.0f,32,32);
        Button* saveButton=new Button(ButtonEventType::Save,73.0f/256.0f,153.0f/256.0f,105.0f/256.0f,185.0f/256.0f,32,32);
        Button* saveAsButton=new Button(ButtonEventType::SaveAs,145.0f/256.0f,117.0f/256.0f,177.0f/256.0f,149.0f/256.0f,32,32);
        Button* undoButton=new Button(ButtonEventType::Nothing,109.0f/256.0f,153.0f/256.0f,141.0f/256.0f,185.0f/256.0f,32,32);
        Button* redoButton=new Button(ButtonEventType::Nothing,145.0f/256.0f,153.0f/256.0f,177.0f/256.0f,185.0f/256.0f,32,32);
        fileStrip->add(newButton);
        fileStrip->add(openButton);
        fileStrip->add(saveButton);
        fileStrip->add(saveAsButton);
        fileStrip->add(undoButton);
        fileStrip->add(redoButton);
        createStrip=new ToolStrip(1,1,42,42,true,4,0,0,11.0f/256.0f,42.0f/256.0f,12.0f/256.0f,0,30.0f/256.0f,0,31.0f/256.0f,0,42.0f/256.0f,42.0f/256.0f);
        Button* boxButton=new Button(ButtonEventType::CreateCube,1.0f/256.0f,189.0f/256.0f,33.0f/256.0f,221.0f/256.0f,32,32);
        Button* ballButton=new Button(ButtonEventType::CreateSphere,37.0f/256.0f,189.0f/256.0f,69.0f/256.0f,221.0f/256.0f,32,32);
        Button* cylinderButton=new Button(ButtonEventType::CreateCylinder,73.0f/256.0f,189.0f/256.0f,105.0f/256.0f,221.0f/256.0f,32,32);
        Button* planeButton=new Button(ButtonEventType::CreatePlane,109.0f/256.0f,189.0f/256.0f,141.0f/256.0f,221.0f/256.0f,32,32);
        createStrip->add(boxButton);
        createStrip->add(ballButton);
        createStrip->add(cylinderButton);
        createStrip->add(planeButton);
        modifyStrip=new ToolStrip(1,1,42,42,true,4,0,0,11.0f/256.0f,42.0f/256.0f,12.0f/256.0f,0,30.0f/256.0f,0,31.0f/256.0f,0,42.0f/256.0f,42.0f/256.0f);
        Button* deleteVertexButton=new Button(ButtonEventType::DeleteVertex,217.0f/256.0f,189.0f/256.0f,249.0f/256.0f,221.0f/256.0f,32,32);
        Button* splitButton=new Button(ButtonEventType::Split,217.0f/256.0f,153.0f/256.0f,249.0f/256.0f,185.0f/256.0f,32,32);
        Button* extrudeFaceButton=new Button(ButtonEventType::Extrude,217.0f/256.0f,117.0f/256.0f,249.0f/256.0f,149.0f/256.0f,32,32);
        Button* weldButton=new Button(ButtonEventType::Collapse,217.0f/256.0f,81.0f/256.0f,249.0f/256.0f,113.0f/256.0f,32,32);
        modifyStrip->add(deleteVertexButton);
        modifyStrip->add(splitButton);
        modifyStrip->add(extrudeFaceButton);
        modifyStrip->add(weldButton);
        helpStrip=new ToolStrip(1,1,42,42,true,4,0,0,11.0f/256.0f,42.0f/256.0f,12.0f/256.0f,0,30.0f/256.0f,0,31.0f/256.0f,0,42.0f/256.0f,42.0f/256.0f);
        Button* screenshotButton=new Button(ButtonEventType::Screenshot,145.0f/256.0f,81.0f/256.0f,177.0f/256.0f,113.0f/256.0f,32,32);
        Button* aboutButton=new Button(ButtonEventType::About,145.0f/256.0f,45.0f/256.0f,177.0f/256.0f,77.0f/256.0f,32,32);
        helpStrip->add(screenshotButton);
        helpStrip->add(aboutButton);
        cameraStrip=new ToolStrip(1,1,42,42,true,4,0,0,11.0f/256.0f,42.0f/256.0f,12.0f/256.0f,0,30.0f/256.0f,0,31.0f/256.0f,0,42.0f/256.0f,42.0f/256.0f);
        Button* bottomButton=new Button(ButtonEventType::CameraBottom,217.0f/256.0f,45.0f/256.0f,249.0f/256.0f,77.0f/256.0f,32,32);
        Button* topButton=new Button(ButtonEventType::CameraTop,181.0f/256.0f,81.0f/256.0f,213.0f/256.0f,113.0f/256.0f,32,32);
        Button* leftButton=new Button(ButtonEventType::CameraLeft,181.0f/256.0f,117.0f/256.0f,213.0f/256.0f,149.0f/256.0f,32,32);
        Button* rightButton=new Button(ButtonEventType::CameraRight,181.0f/256.0f,153.0f/256.0f,213.0f/256.0f,185.0f/256.0f,32,32);
        Button* frontButton=new Button(ButtonEventType::CameraFront,181.0f/256.0f,189.0f/256.0f,213.0f/256.0f,221.0f/256.0f,32,32);
        Button* backButton=new Button(ButtonEventType::CameraBack,145.0f/256.0f,189.0f/256.0f,177.0f/256.0f,221.0f/256.0f,32,32);
        Button* perspectiveButton=new Button(ButtonEventType::CameraPerspective,181.0f/256.0f,45.0f/256.0f,213.0f/256.0f,77.0f/256.0f,32,32);
        cameraStrip->add(bottomButton);
        cameraStrip->add(topButton);
        cameraStrip->add(leftButton);
        cameraStrip->add(rightButton);
        cameraStrip->add(frontButton);
        cameraStrip->add(backButton);
        cameraStrip->add(perspectiveButton);
    }

    void updateCameraStrip()
    {
        cameraStrip->updatePosition((int)(((int)(width-shadeModeStrip->width-10-cameraStrip->width))*0.5f)+shadeModeStrip->width+10,height-shadeModeStrip->height-1);
    }

    bool onMoving(int x,int y)
    {
        if(currentIn)
        {
            if(currentIn->onOut(x,y)) currentIn=NULL;
        }
        if(fileStrip->onIn(x,y))
        {
            currentIn=fileStrip;
            return true;
        }
        else
        if(modifyModeStrip->onIn(x,y))
        {
            currentIn=modifyModeStrip;
            return true;
        }
        else
        if(layoutModeStrip->onIn(x,y))
        {
            currentIn=layoutModeStrip;
            return true;
        }
        else
        if(shadeModeStrip->onIn(x,y))
        {
            currentIn=shadeModeStrip;
            return true;
        }
        else
        if(createStrip->onIn(x,y))
        {
            currentIn=createStrip;
            return true;
        }
        else
        if(modifyStrip->onIn(x,y))
        {
            currentIn=modifyStrip;
            return true;
        }
        else
        if(helpStrip->onIn(x,y))
        {
            currentIn=helpStrip;
            return true;
        }
        else
        if(cameraStrip->onIn(x,y))
        {
            currentIn=cameraStrip;
            return true;
        }
        else
        return false;
    }

    ButtonEventType onLeftRelease()
    {
        if(currentIn)
        {
            return currentIn->onLeftRelease();
        }
        else
            return ButtonEventType::Nothing;
    }

        bool onLeftPress(int mx,int my)
        {
            if(currentIn)
            {
                if(currentIn->onLeftPress(mx,my))
                    return true;
                return false;
            }
            else
            return false;
        }

    void updateModifyStrip()
    {
        modifyStrip->updatePosition((int)(((int)(width-(fileStrip->width+10+createStrip->width+10+modifyStrip->width+10+helpStrip->width)))*0.5f)+fileStrip->width+20+createStrip->width,1);
    }

    void updateHelpStrip()
    {
        helpStrip->updatePosition((int)(((int)(width-(fileStrip->width+10+createStrip->width+10+modifyStrip->width+10+helpStrip->width)))*0.5f)+fileStrip->width+30+createStrip->width+modifyStrip->width,1);
    }

    void updateCreateStrip()
    {
        createStrip->updatePosition((int)(((int)(width-(fileStrip->width+10+createStrip->width+10+modifyStrip->width+10+helpStrip->width)))*0.5f)+fileStrip->width+10,1);
    }

    void updateModifyModeStrip()
    {
        modifyModeStrip->updatePosition(1,height*0.5f-modifyModeStrip->height*0.5f);
    }

    void updateLayoutModeStrip()
    {
        layoutModeStrip->updatePosition(width-layoutModeStrip->width-1,height*0.5f-layoutModeStrip->height*0.5f);
    }

    void updateShadeModeStrip()
    {
        shadeModeStrip->updatePosition((int)(((int)(width-shadeModeStrip->width-10-cameraStrip->width))*0.5f),height-shadeModeStrip->height-1);
    }

    void updateFileStrip()
    {
        fileStrip->updatePosition(((int)((int)(width-(fileStrip->width+10+createStrip->width+10+modifyStrip->width+10+helpStrip->width)))*0.5f),1);
    }

    void onPaint()
    {
        glBindTexture(GL_TEXTURE_2D,textureID);
        modifyModeStrip->onPaint();
        layoutModeStrip->onPaint();
        shadeModeStrip->onPaint();
        fileStrip->onPaint();
        createStrip->onPaint();
        modifyStrip->onPaint();
        helpStrip->onPaint();
        cameraStrip->onPaint();
    }

    void updateSize(unsigned int theWidth,unsigned int theHeight)
    {
        width=theWidth;
        height=theHeight;
        updateModifyModeStrip();
        updateLayoutModeStrip();
        updateShadeModeStrip();
        updateFileStrip();
        updateCreateStrip();
        updateModifyStrip();
        updateHelpStrip();
        updateCameraStrip();
    }

    GLuint loadTexture(char *fileName)
    {
        GLuint txtnumber;
        QImage texture(fileName);
        texture = texture.convertToFormat(QImage::Format_RGBA8888);
        int theHeight=texture.height();
        int theWidth=texture.width();
        glGenTextures(1, &txtnumber);
        glBindTexture(GL_TEXTURE_2D, txtnumber);
        glTexImage2D(GL_TEXTURE_2D, 0, 4, theWidth, theHeight, 0,GL_RGBA,GL_UNSIGNED_BYTE, texture.bits());
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
        return txtnumber;

    }


public:
    ~UILayer(void)
    {
        delete fileStrip;
        delete modifyModeStrip;
        delete layoutModeStrip;
        delete shadeModeStrip;
        delete createStrip;
    }
};
