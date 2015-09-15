#include "ToolStrip.h"
#include <QDebug>

ToolStrip::ToolStrip(OpenGLBackend *backend, int x, int y, unsigned int width, unsigned int height, bool isHorizontal, unsigned int buttonNum, float textLeftX1, float textLeftY1, float textLeftX2, float textLeftY2, float textBGX1, float textBGY1, float textBGX2, float textBGY2, float textRightX1, float textRightY1, float textRightX2, float textRightY2)
    :m_backend(backend),
      m_currentIn(NULL),
      m_x(x),
      m_y(y),
      m_width(width),
      m_height(height),
      m_spacer(10),
      m_hPadding(5),
      m_vPadding(5),
      m_bSY(32),
      m_bSX(32),
      m_isHorizontal(isHorizontal),
      m_textLeftX1(textLeftX1),
      m_textLeftY1(textLeftY1),
      m_textLeftX2(textLeftX2),
      m_textLeftY2(textLeftY2),
      m_textBGX1(textBGX1),
      m_textBGY1(textBGY1),
      m_textBGX2(textBGX2),
      m_textBGY2(textBGY2),
      m_textRightX1(textRightX1),
      m_textRightY1(textRightY1),
      m_textRightX2(textRightX2),
      m_textRightY2(textRightY2),
      m_renderable(nullptr)
{
    m_buttonSet.reserve(buttonNum);
}

void ToolStrip::add(Button *button)
{
    m_buttonSet.push_back(button);
    button->updateSize(m_bSX, m_bSY);
}

void ToolStrip::updateButtonPosition()
{
    if(m_isHorizontal)
    {
        int tempX=m_x+m_hPadding;
        int tempY=m_y+m_vPadding;
        for(unsigned int i=0;i<m_buttonSet.size();++i)
        {
            m_buttonSet[i]->updatePosition(tempX,tempY);
            tempX+=m_spacer+m_bSX;
        }
        m_width=m_buttonSet.size()*m_bSX+m_hPadding+m_hPadding+m_spacer*(m_buttonSet.size()-1);
        m_height=42;
    }
    else
    {
        int tempX=m_x+m_hPadding;
        int tempY=m_y+m_vPadding;
        for(unsigned int i=0;i<m_buttonSet.size();++i)
        {
            m_buttonSet[i]->updatePosition(tempX,tempY);
            tempY+=m_spacer+m_bSY;
        }
        m_width=42;
        m_height=m_buttonSet.size()*m_bSY+m_vPadding+m_vPadding+m_spacer*(m_buttonSet.size()-1);
    }
}

ButtonEventType ToolStrip::onLeftRelease()
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

bool ToolStrip::onLeftPress(int mx,int my)
{
    if(m_currentIn && m_currentIn->onLeftPress(mx, my))
    {
        return true;
    }
    return false;
}

bool ToolStrip::onIn(int mx, int my)
{
    if(mx > m_x && my > m_y && my < m_y + (int) m_height && mx < m_x + (int) m_width)
    {
        for(unsigned int i = 0; i < m_buttonSet.size(); ++i)
        {
            if(m_buttonSet[i]->onIn(mx, my))
            {
                m_currentIn = m_buttonSet[i];
            }
        }
        return true;
    }
    else
    {
        return false;
    }
}

bool ToolStrip::onOut(int mx,int my)
{
    if(m_currentIn)
    {
        if(m_currentIn->onOut(mx, my))
        {
            m_currentIn=NULL;
        }
        else
        {
            return false;
        }
    }

    if (mx < m_x || mx < m_y || mx > m_x + (int) m_width || my > m_y + (int) m_height)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void ToolStrip::updatePosition(int x, int y)
{
    m_x = x;
    m_y = y;
    updateButtonPosition();
    updateGeometry();
}

void ToolStrip::updateGeometry()
{
    if (m_isHorizontal)
    {
        float vertices[] =
        {
            (float) m_x,                  (float) m_y,            (float) m_textLeftX1, (float) m_textLeftY1,
            (float) m_x,                  (float) m_y + m_height, (float) m_textLeftX1, (float) m_textLeftY2,
            (float) (m_x + 10),           (float) m_y,            (float) m_textLeftX2, (float) m_textLeftY1,
            (float) (m_x + 10),           (float) m_y + m_height, (float) m_textLeftX2, (float) m_textLeftY2,
            (float) (m_x + m_width - 10), (float) m_y,            (float) m_textRightX1, (float) m_textRightY1,
            (float) (m_x + m_width - 10), (float) m_y + m_height, (float) m_textRightX1, (float) m_textRightY2,
            (float) (m_x + m_width),      (float) m_y,            (float) m_textRightX2, (float) m_textRightY1,
            (float) (m_x + m_width),      (float) m_y + m_height, (float) m_textRightX2, (float) m_textRightY2,
        };
        m_backend->updateGeometryWithVertices(&m_renderable, vertices, 32, 4);
    }
    else
    {
        float vertices[] =
        {
            (float) m_x,           (float) m_y, (float) m_textLeftX1, (float) m_textLeftY1,
            (float) m_x + m_width, (float) m_y, (float) m_textLeftX2, (float) m_textLeftY1,
            (float) m_x,           (float) m_y + 10, (float) m_textLeftX1, (float) m_textLeftY2,
            (float) m_x + m_width, (float) m_y + 10, (float) m_textLeftX2, (float) m_textLeftY2,
            (float) m_x,           (float) m_y + m_height - 10, (float) m_textRightX1, (float) m_textRightY1,
            (float) m_x + m_width, (float) m_y + m_height - 10, (float) m_textRightX2, (float) m_textRightY1,
            (float) m_x,           (float) m_y + m_height, (float) m_textRightX1, (float) m_textRightY2,
            (float) m_x + m_width, (float) m_y + m_height, (float) m_textRightX2, (float) m_textRightY2
        };
        m_backend->updateGeometryWithVertices(&m_renderable, vertices, 32, 4);
    }

    for(unsigned int i = 0; i < m_buttonSet.size(); ++i)
    {
        m_buttonSet[i]->updateGeometry();
    }
}

void ToolStrip::onPaint() const
{
    m_backend->drawToolStripGeometry(m_renderable);

    for (unsigned int i=0;i<m_buttonSet.size();++i)
    {
        m_buttonSet[i]->onPaint();
    }
}

ToolStrip::~ToolStrip(void)
{
    for (unsigned int i = 0; i < m_buttonSet.size(); ++i)
    {
        delete m_buttonSet[i];
    }
    m_buttonSet.clear();
    if (m_renderable)
    {
        m_backend->deleteRenderableGeometryData(m_renderable);
        m_renderable = nullptr;
    }
}
