#pragma once
#include "ToolStrip.h"
#include "../GraphicsBackend/OpenGLBackend.h"

class UILayer
{
private:
    OpenGLBackend *m_backend;
    unsigned int m_width;
    unsigned int m_height;

    ToolStrip *m_fileStrip;
    ToolStrip *m_modifyModeStrip;
    ToolStrip *m_layoutModeStrip;
    ToolStrip *m_shadeModeStrip;
    ToolStrip *m_createStrip;
    ToolStrip *m_modifyStrip;
    ToolStrip *m_helpStrip;
    ToolStrip *m_cameraStrip;
    ToolStrip *m_currentIn;

public:
    UILayer(OpenGLBackend *backend, unsigned int width, unsigned int height);

    bool onMoving(int x, int y);

    ButtonEventType onLeftRelease();

    bool onLeftPress(int mx, int my);

    void onPaint();

    void updateSize(unsigned int width, unsigned int height);

    ~UILayer(void);

private:
    void updateCameraStrip();

    void updateModifyStrip();

    void updateHelpStrip();

    void updateCreateStrip();

    void updateModifyModeStrip();

    void updateLayoutModeStrip();

    void updateShadeModeStrip();

    void updateFileStrip();
};
