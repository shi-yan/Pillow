#pragma once
#include "Enum.h"

#include "../Core/Vector.h"
#include "Camera/Camera.h"

#include "OpenGLBackend.h"

DECLARE_ENUM(AxisCursorMode)
    MoveAxis=982,
    RotateAxis,
    ScaleAxis,
    NoAxis
END_ENUM()

class AxisCursor
{   
    OpenGLBackend *m_backend;
    void *m_moveXRenderable;
    void *m_moveYRenderable;
    void *m_moveZRenderable;

    void *m_rotateXRenderable;
    void *m_rotateYRenderable;
    void *m_rotateZRenderable;

    void *m_scaleXRenderable;
    void *m_scaleYRenderable;
    void *m_scaleZRenderable;
    void *m_scaleCRenderable;

public:
    AxisCursorMode m_mode;
    Vector m_position;
    Vector m_rotation;

public:
    AxisCursor(OpenGLBackend *backend);
    void initialize();
    void onPaint(const Vector &eye, CameraMode::__Enum cameraMode);

public:
    ~AxisCursor(void);
};
