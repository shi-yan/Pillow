#pragma once
#include "View/FourView.h"
#include "View/ThreeView.h"
#include "View/TwoView.h"
#include "View/SingleView.h"
#include "UI/UILayer.h"
//#include "AboutDialog.h"
#include "GraphicsBackend/OpenGLBackend.h"
enum
{
    Four=32,
    Three,
    Two,
    Single
};

class Screen
{
private:
    FourView *m_fourView;
    ThreeView *m_threeView;
    SingleView *m_singleView;
    TwoView *m_twoView;
    SplitedView *m_currentView;
    UILayer *m_uiLayer;
    bool m_isSelectionMode;
    bool m_isAxisMode;
    bool m_isAltMode;
    bool m_isCtrlMode;
    bool m_isExtrude;
    int m_width;
    int m_height;

public:
    OpenGLBackend *m_graphicsBackend;

    Screen();
    void updateScreenSize(unsigned int width, unsigned int height);
    void onPaint();
    void screenShot(const char *fileName);

    void initialize();

    void disableReference();

    void getViewportImage(GLuint &textID, std::string &path, Vector &position, unsigned int &width, unsigned int &height);
    void setViewportImage(const char *path, GLuint imageID, Vector &position, unsigned int width, unsigned int height);

    void onCtrlDown();
    void onCtrlUp();
    void onAltDown();
    void onAltUp();
    bool onLeftDrag(int x,int y);
    bool onMiddleDrag(int x,int y);
    bool onMiddleRelease(bool isAppend);
    bool onLeftPress(int x,int y);
    bool onMoving(int x,int y);
    void onExtrudePress();
    void onExtrudeRelease();
    bool onLeftRelease(bool isAppend);
    bool onMiddlePress(int x,int y);
    bool onWheel(float step,int x,int y);

    void toggleGrid();

    void toolButtonSingleView();
    void toolButtonTwoView();
    void toolButtonThreeView();
    void toolButtonFourView();

    void changeView(int type);
    void changeCamera(CameraMode::__Enum type);

private:
    void toolButtonSmooth();
    void toolButtonFaced();
    void toolButtonWireframe();
    void toolButtonWire();
    void toolButtonWireframeFaced();

public:
    ~Screen(void);
    static Screen *screen;
};
