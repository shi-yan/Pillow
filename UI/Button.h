#pragma once
#ifdef __APPLE__
#include <OpenGL/gl3.h>
#include <OpenGL/gl3ext.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#endif

#include "Enum.h"
#include "OpenGLBackend.h"

DECLARE_ENUM(ButtonEventType)
    Nothing=823,
    Wireframe,
    Faced,
    WireframeFaced,
    Smooth,
    Wire,
    SingleView,
    TwoView,
    ThreeView,
    FourView,
    ObjectSelectionMode,
    FaceSelectionMode,
    EdgeSelectionMode,
    VertexSelectionMode,
    Help,
    CreateCube,
    CreateCylinder,
    CreateSphere,
    CreatePlane,
    CameraPerspective,
    CameraFront,
    CameraBack,
    CameraLeft,
    CameraRight,
    CameraTop,
    CameraBottom,
    Screenshot,
    About,
    Open,
    Save,
    SaveAs,
    New,
    DeleteVertex,
    Split,
    Extrude,
    Collapse
END_ENUM()

class Button
{
    OpenGLBackend *m_backend;
    ButtonEventType m_type;
    bool m_isIn;
    int m_x;
    int m_y;
    unsigned int m_sx;
    unsigned int m_sy;
    float m_texCx1;
    float m_texCx2;
    float m_texCy1;
    float m_texCy2;
    bool m_isPressed;
    void *m_renderable;

public:
    Button(OpenGLBackend *backend, ButtonEventType::__Enum type, float texCx1, float texCy1, float texCx2, float texCy2, unsigned int sx, unsigned int sy);

    void updatePosition(int x, int y);

    void updateSize(unsigned int sx, unsigned int sy);

    void updateGeometry();

    bool onIn(int mx, int my);

    bool onLeftPress(int mx, int my);

    ButtonEventType onLeftRelease();

    bool onOut(int mx, int my);

    void onPaint() const;

public:
    ~Button(void);
};
