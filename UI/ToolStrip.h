#pragma once
#include "Button.h"
#include <vector>
#include "../GraphicsBackend/OpenGLBackend.h"

class ToolStrip
{
public:
    Button *m_currentIn;
    int m_x;
    int m_y;
    unsigned int m_spacer;
    unsigned int m_hPadding;
    unsigned int m_vPadding;
    float m_textLeftX1;
    float m_textLeftY1;
    float m_textLeftX2;
    float m_textLeftY2;
    float m_textBGX1;
    float m_textBGY1;
    float m_textBGX2;
    float m_textBGY2;
    float m_textRightX1;
    float m_textRightY1;
    float m_textRightX2;
    float m_textRightY2;
    unsigned int m_width;
    unsigned int m_height;
    unsigned int m_bSX;
    unsigned int m_bSY;
    bool m_isHorizontal;
    std::vector<Button*> m_buttonSet;
    OpenGLBackend *m_backend;
    void *m_renderable;

public:
    ToolStrip(OpenGLBackend *backend, int x, int y, unsigned int width, unsigned int height, bool isH, unsigned int buttonNum, float textLeftX1, float textLeftY1, float textLeftX2, float textLeftY2, float textBGX1, float textBGY1, float textBGX2, float textBGY2, float textRightX1, float textRightY1, float textRightX2, float textRightY2);

    void add(Button *button);

    ButtonEventType onLeftRelease();

    bool onLeftPress(int mx, int my);

    bool onIn(int mx, int my);

    bool onOut(int mx, int my);

    void updatePosition(int x,int y);

    void onPaint() const;

    ~ToolStrip(void);

private:
    void updateButtonPosition();
    void updateGeometry();
};
