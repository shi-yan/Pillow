#pragma once
#include "../Camera/LeftCamera.h"
#include "../Camera/RightCamera.h"
#include "../Camera/BackCamera.h"
#include "../Camera/FrontCamera.h"
#include "../Camera/BottomCamera.h"
#include "../Camera/TopCamera.h"
#include "../Camera/PerspectiveCamera.h"
#ifdef __APPLE__
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#endif

class SplitedView
{
protected:
    unsigned int m_splitX;
    unsigned int m_splitY;
    unsigned int m_width;
    unsigned int m_height;
    unsigned int m_selected;
    int m_panIndex;
    int m_zoomIndex;
    int m_rotateIndex;
    Camera *m_camera[4];
    unsigned int m_selectionSX;
    unsigned int m_selectionSY;
    unsigned int m_selectionEX;
    unsigned int m_selectionEY;
    unsigned int m_axisDragSX;
    unsigned int m_axisDragSY;
    unsigned int m_axisDragEX;
    unsigned int m_axisDragEY;
    bool m_isSelectionMode;
    bool m_isAxisMode;
    bool m_isExtrude;
    Vector m_cursorDir;
    int m_cursorMode;
    bool m_isCtrlMode;
    unsigned int m_ctrlSX;
    unsigned int m_ctrlSY;

public:
    unsigned int m_viewCount;

    void changeCamera(CameraMode::__Enum type);

    void getViewportImage(GLuint &texID,std::string &path,Vector &position,unsigned int &width,unsigned int &height);

    void setViewportImage(const char *path,GLuint imageID,Vector &position,unsigned int width,unsigned int height);

    void disableReference();

    void onExtrudeDown();

    void onExtrudeRelease();

    void switchGird();

    virtual bool onCtrlPress(unsigned int x,unsigned int y);

    virtual bool onCtrlDrag(unsigned int x,unsigned int y);

    virtual bool onPanPress(unsigned int x,unsigned int y);

    virtual bool onAxisPress(unsigned int x,unsigned int y);

    virtual bool onAxisDrag(unsigned int x,unsigned int y);

    virtual bool onAxisRelease();

    virtual bool onCtrlRelease();

    virtual bool onPanDrag(unsigned int x,unsigned int y);

    virtual bool onPanRelease();

    virtual bool onRotateDrag(unsigned int x,unsigned int y);

    virtual bool onRotateRelease();
	
    virtual bool onRotatePress(unsigned int x,unsigned int y);

    virtual bool onSelectionPress(unsigned int x,unsigned int y);
	
    virtual CameraMode::__Enum setView(unsigned int i);

    Vector getEye(unsigned int i) const;
	
    virtual void onWheel(float step,unsigned int x,unsigned int y);
	
    virtual void update(unsigned int splitX,unsigned int splitY,unsigned int width,unsigned int height);

    virtual void setViewport(unsigned int index);

    virtual bool onDrag(unsigned int x,unsigned int y);

    virtual bool onLeftDown(unsigned int x, unsigned int y);

    virtual void onPaint(void);

    virtual bool onSelectionDrag(unsigned int x,unsigned int y);

    virtual bool onSingleSideSelectionRelease(bool isAppend);

    virtual bool onDualSideSelectionRelease(bool isAppend);

    SplitedView(unsigned int splitX,unsigned int splitY,unsigned int width,unsigned int height);

public:
    virtual ~SplitedView(void);
};
