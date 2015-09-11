#include "Button.h"
#include <OpenGL/gl3.h>

static GLuint vs = 0;

Button::Button(OpenGLBackend *backend, ButtonEventType::__Enum type, float texCx1, float texCy1, float texCx2, float texCy2, unsigned int sx, unsigned int sy)
    :m_backend(backend),
      m_type(type),
      m_texCx1(texCx1),
      m_texCy1(texCy1),
      m_texCx2(texCx2),
      m_texCy2(texCy2),
      m_isIn(false),
      m_x(0),
      m_y(0),
      m_sx(sx),
      m_sy(sy),
      m_isPressed(false),
      m_renderable(nullptr)
{
}

void Button::updatePosition(int x, int y)
{
    m_x = x;
    m_y = y;
}

void Button::updateSize(unsigned int sx, unsigned int sy)
{
    m_sx = sx;
    m_sy = sy;
}

void Button::updateGeometry()
{
    float points[] = {
        (float) m_x,        (float) m_y,        (float) m_texCx1, (float) m_texCy1,
        (float) m_x + m_sx, (float) m_y,        (float) m_texCx2, (float) m_texCy1,
        (float) m_x + m_sx, (float) m_y + m_sy, (float) m_texCx2, (float) m_texCy2,
        (float) m_x,        (float) m_y + m_sy, (float) m_texCx1, (float) m_texCy2
    };

    m_backend->updateButtonGeometry(&m_renderable, points);
}

bool Button::onIn(int mx, int my)
{
    if(mx > m_x && my > m_y && mx < m_x + (int) m_sx && my < m_y + (int) m_sy)
    {
        m_isIn = true;
        return true;
    }
    else
    {
        return false;
    }
}

bool Button::onLeftPress(int mx, int my)
{
    if(mx > m_x && my > m_y && mx < m_x + m_sx && my < m_y + m_sy)
    {
        m_isPressed = true;
        return true;
    }
    else
        return false;
}

ButtonEventType Button::onLeftRelease()
{
    if(m_isPressed)
    {
        m_isPressed=false;
        return m_type;
    }
    else
    {
        return ButtonEventType::Nothing;
    }
}

bool Button::onOut(int mx,int my)
{
    if(mx < m_x || my < m_y || mx > m_x + (int)m_sx || my > m_y + (int)m_sy)
    {
        m_isIn=false;
        return true;
    }
    else
    {
        return false;
    }
}

void Button::onPaint() const
{
    if(m_isPressed)
    {
       m_backend->drawButtonStripGeometry(m_renderable);
    }
    else
    {
        if(m_isIn)
        {
            m_backend->drawButtonStripGeometry(m_renderable, -2, -2);
        }
        else
        {
            m_backend->drawButtonStripGeometry(m_renderable);
        }
    }
}

Button::~Button(void)
{
    if (m_renderable)
    {
        m_backend->deleteRenderableGeometryData(m_renderable);
        m_renderable = nullptr;
    }
}
