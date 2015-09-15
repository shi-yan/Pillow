#pragma once
#include "../GraphicsBackend/OpenGLBackend.h"

class Grid
{
private:
    OpenGLBackend *m_backend;
    void *m_xyRenderable;
    void *m_xzRenderable;
    void *m_yzRenderable;

public:
    Grid(OpenGLBackend *m_backend);
    void initialize();

    void drawXY() const;
    void drawXZ() const;
    void drawYZ() const;

public:
    ~Grid(void);

    static Grid *grid;
};
