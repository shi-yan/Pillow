#pragma once
#include "../Camera/LeftCamera.h"
#include "../Camera/RightCamera.h"
#include "../Camera/BackCamera.h"
#include "../Camera/FrontCamera.h"
#include "../Camera/BottomCamera.h"
#include "../Camera/TopCamera.h"
#include "../Camera/PerspectiveCamera.h"

#include <OpenGL/gl.h>
#include <OpenGL/glu.h>

class SplitedView
{
protected:
    size_t m_splitX;
    size_t m_splitY;
    size_t m_width;
    size_t m_height;
    size_t m_selected;
    int m_panIndex;
    int m_zoomIndex;
    int m_rotateIndex;
    Camera *m_camera[4];
    size_t m_selectionSX;
    size_t m_selectionSY;
    size_t m_selectionEX;
    size_t m_selectionEY;
    size_t m_axisDragSX;
    size_t m_axisDragSY;
    size_t m_axisDragEX;
    size_t m_axisDragEY;
    bool m_isSelectionMode;
    bool m_isAxisMode;
    bool m_isExtrude;
    Vector m_cursorDir;
    int m_cursorMode;
    bool m_isCtrlMode;
    size_t m_ctrlSX;
    size_t m_ctrlSY;
public:
    size_t m_viewCount;

    void changeCamera(CameraMode::__Enum type);

    void getViewportImage(GLuint &texID,std::string &path,Vector &position,size_t &width,size_t &height);

    void setViewportImage(const char *path,GLuint imageID,Vector &position,size_t width,size_t height);

    void disableReference();

    void onExtrudeDown();

    void onExtrudeRelease();

    void switchGird();

    virtual bool onCtrlPress(size_t x,size_t y);

    virtual bool onCtrlDrag(size_t x,size_t y);

    virtual bool onPanPress(size_t x,size_t y);

    virtual bool onAxisPress(size_t x,size_t y);

    virtual bool onAxisDrag(size_t x,size_t y);

    virtual bool onAxisRelease();

    virtual bool onCtrlRelease();

    virtual bool onPanDrag(size_t x,size_t y);

    virtual bool onPanRelease();

    virtual bool onRotateDrag(size_t x,size_t y);

    virtual bool onRotateRelease();
	
    virtual bool onRotatePress(size_t x,size_t y);

    virtual bool onSelectionPress(size_t x,size_t y);
	
    virtual CameraMode::__Enum setView(size_t i);

    Vector getEye(size_t i) const;
	
    virtual void onWheel(float step,size_t x,size_t y);
	
    virtual void update(size_t splitX,size_t splitY,size_t width,size_t height);

    virtual void setViewport(size_t index);

    virtual bool onDrag(size_t x,size_t y);

    virtual bool onLeftDown(size_t x,size_t y);

    virtual void onPaint(void);

    virtual bool onSelectionDrag(size_t x,size_t y);

    virtual bool onSingleSideSelectionRelease(bool isAppend);

    virtual bool onDualSideSelectionRelease(bool isAppend);

    SplitedView(unsigned int splitX,unsigned int splitY,unsigned int width,unsigned int height);

public:
    virtual ~SplitedView(void);
};
