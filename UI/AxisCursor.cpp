#include "AxisCursor.h"
#include <QMatrix4x4>

AxisCursor::AxisCursor(OpenGLBackend *backend)
    :m_backend(backend),
      m_moveXRenderable(nullptr),
      m_moveYRenderable(nullptr),
      m_moveZRenderable(nullptr),
      m_rotateXRenderable(nullptr),
      m_rotateYRenderable(nullptr),
      m_rotateZRenderable(nullptr),
      m_scaleXRenderable(nullptr),
      m_scaleYRenderable(nullptr),
      m_scaleZRenderable(nullptr),
      m_scaleCRenderable(nullptr),
      m_mode(AxisCursorMode::NoAxis),
      m_position(),
      m_rotation()
{
}

AxisCursor::~AxisCursor(void)
{
    if (m_moveXRenderable)
    {
        m_backend->deleteRenderableGeometryData(m_moveXRenderable);
        m_moveXRenderable = nullptr;
    }

    if (m_moveYRenderable)
    {
        m_backend->deleteRenderableGeometryData(m_moveYRenderable);
        m_moveYRenderable = nullptr;
    }

    if (m_moveZRenderable)
    {
        m_backend->deleteRenderableGeometryData(m_moveZRenderable);
        m_moveZRenderable = nullptr;
    }

    if (m_rotateXRenderable)
    {
        m_backend->deleteRenderableGeometryData(m_rotateXRenderable);
        m_rotateXRenderable = nullptr;
    }

    if (m_rotateYRenderable)
    {
        m_backend->deleteRenderableGeometryData(m_rotateYRenderable);
        m_rotateYRenderable = nullptr;
    }

    if (m_rotateZRenderable)
    {
        m_backend->deleteRenderableGeometryData(m_rotateZRenderable);
        m_rotateZRenderable = nullptr;
    }

    if (m_scaleXRenderable)
    {
        m_backend->deleteRenderableGeometryData(m_scaleXRenderable);
        m_scaleXRenderable = nullptr;
    }

    if (m_scaleYRenderable)
    {
        m_backend->deleteRenderableGeometryData(m_scaleYRenderable);
        m_scaleYRenderable = nullptr;
    }

    if (m_scaleZRenderable)
    {
        m_backend->deleteRenderableGeometryData(m_scaleZRenderable);
        m_scaleZRenderable = nullptr;
    }

    if (m_scaleCRenderable)
    {
        m_backend->deleteRenderableGeometryData(m_scaleCRenderable);
        m_scaleCRenderable = nullptr;
    }
}

void AxisCursor::initialize()
{
    float moveX[] =
    {
        20,0,0,
        10,1,1,
        10,1,-1,

        20,0,0,
        10,-1,1,
        10,1,1,

        20,0,0,
        10,-1,-1,
        10,-1,1,

        20,0,0,
        10,1,-1,
        10,-1,-1,

        10,-1,-1,
        10,1,-1,
        10,1,1,
        10,-1,1,

        0,0,0,
        10,0,0
    };

    m_backend->updateGeometryWithVertices(&m_moveXRenderable, moveX, 18 * 3, 3);

    float moveY[] =
    {
        0,20,0,
        1,10,-1,
        1,10,1,

        0,20,0,
        1,10,1,
        -1,10,1,

        0,20,0,
        -1,10,1,
        -1,10,-1,

        0,20,0,
        -1,10,-1,
        1,10,-1,

        -1,10,1,
        1,10,1,
        1,10,-1,
        -1,10,-1,

        0,0,0,
        0,10,0
    };

    m_backend->updateGeometryWithVertices(&m_moveYRenderable, moveY, 18 * 3, 3);

    float moveZ[] =
    {
        0,0,20,
        1,1,10,
        1,-1,10,

        0,0,20,
        -1,1,10,
        1,1,10,

        0,0,20,
        -1,-1,10,
        -1,1,10,

        0,0,20,
        1,-1,10,
        -1,-1,10,

        1,1,10,
        -1,1,10,
        -1,-1,10,
        1,-1,10,

        0,0,0,
        0,0,10
    };

    m_backend->updateGeometryWithVertices(&m_moveZRenderable, moveZ, 18 * 3, 3);

    float rotateX[] =
    {
        10,0,0,
        12,1,1,
        12,-1,1,
        10,0,0,
        12,1,1,
        12,1,-1,
        10,0,0,
        12,1,-1,
        12,-1,-1,
        14,0,0,
        12,-1,1,
        12,1,1,
        14,0,0,
        12,1,1,
        12,1,-1,
        14,0,0,
        12,1,-1,
        12,-1,-1,
        14,0,0,
        12,-1,-1,
        12,-1,1,

        0,0,0,
        10,0,0
    };

    m_backend->updateGeometryWithVertices(&m_rotateXRenderable, rotateX, 23 * 3, 3);

    float rotateY[] =
    {
        0,10,0,
        1,12,1,
        -1,12,1,
        0,10,0,
        1,12,1,
        1,12,-1,
        0,10,0,
        1,12,-1,
        -1,12,-1,
        0,14,0,
        -1,12,1,
        1,12,1,
        0,14,0,
        1,12,1,
        1,12,-1,
        0,14,0,
        1,12,-1,
        -1,12,-1,
        0,14,0,
        -1,12,-1,
        -1,12,1,
        0,0,0,
        0,10,0
    };

    m_backend->updateGeometryWithVertices(&m_rotateYRenderable, rotateY, 23 * 3, 3);

    float rotateZ[] =
    {
        0,0,10,
        1,1,12,
        -1,1,12,
        0,0,10,
        1,1,12,
        1,-1,12,
        0,0,10,
        1,-1,12,
        -1,-1,12,
        0,0,14,
        -1,1,12,
        1,1,12,
        0,0,14,
        1,1,12,
        1,-1,12,
        0,0,14,
        1,-1,12,
        -1,-1,12,
        0,0,14,
        -1,-1,12,
        -1,1,12,
        0,0,0,
        0,0,10
    };

    m_backend->updateGeometryWithVertices(&m_rotateZRenderable, rotateZ, 23 * 3, 3);

    float scaleX[] =
    {
        10,-1,-1,
        10,-1,1,
        10,1,1,
        10,1,-1,
        10,-1,1,
        10,1,1,
        12,1,1,
        12,-1,1,
        12,1,1,
        10,1,1,
        10,1,-1,
        12,1,-1,
        12,1,-1,
        10,1,-1,
        10,-1,-1,
        12,-1,-1,
        12,-1,-1,
        10,-1,-1,
        10,-1,1,
        12,-1,1,
        12,-1,1,
        12,1,1,
        12,1,-1,
        12,-1,-1,

        0,0,0,
        10,0,0
    };

    m_backend->updateGeometryWithVertices(&m_scaleXRenderable, scaleX, 26 * 3, 3);

    float scaleY[] =
    {
        -1,10,-1,
        -1,10,1,
        1,10,1,
        1,10,-1,
        -1,10,1,
        1,10,1,
        1,12,1,
        -1,12,1,
        1,12,1,
        1,10,1,
        1,10,-1,
        1,12,-1,
        1,12,-1,
        1,10,-1,
        -1,10,-1,
        -1,12,-1,
        -1,12,-1,
        -1,10,-1,
        -1,10,1,
        -1,12,1,
        -1,12,1,
        1,12,1,
        1,12,-1,
        -1,12,-1,

        0,0,0,
        0,10,0
    };

    m_backend->updateGeometryWithVertices(&m_scaleYRenderable, scaleY, 26 * 3, 3);

    float scaleZ[] =
    {
        -1,-1,10,
        -1,1,10,
        1,1,10,
        1,-1,10,
        -1,1,10,
        1,1,10,
        1,1,12,
        -1,1,12,
        1,1,12,
        1,1,10,
        1,-1,10,
        1,-1,12,
        1,-1,12,
        1,-1,10,
        -1,-1,10,
        -1,-1,12,
        -1,-1,12,
        -1,-1,10,
        -1,1,10,
        -1,1,12,
        -1,1,12,
        1,1,12,
        1,-1,12,
        -1,-1,12,

        0,0,0,
        0,0,10
    };

    m_backend->updateGeometryWithVertices(&m_scaleZRenderable, scaleZ, 26 * 3, 3);

    float scaleC[] =
    {
        1,-1,1,
        1,1,1,
        1,1,-1,
        1,-1,-1,
        -1,-1,-1,
        -1,1,-1,
        -1,1,1,
        -1,-1,1,
        -1,1,1,
        1,1,1,
        1,1,-1,
        -1,1,-1,
        -1,-1,-1,
        1,-1,-1,
        1,-1,1,
        -1,-1,1,
        -1,1,1,
        1,1,1,
        1,-1,1,
        -1,-1,1,
        -1,-1,-1,
        1,-1,-1,
        1,1,-1,
        -1,1,-1
    };

    m_backend->updateGeometryWithVertices(&m_scaleCRenderable, scaleC, 26 * 3, 3);
}

void AxisCursor::onPaint(const Vector &eye, CameraMode::__Enum cameraMode)
{
    QMatrix4x4 transform;
    float scaleFactor = m_position.distto(eye) * 0.004f;
    transform.translate(m_position.x, m_position.y, m_position.z);
    transform.rotate(m_rotation.w, m_rotation.x, m_rotation.y, m_rotation.z);
    transform.scale(scaleFactor, scaleFactor, scaleFactor);

    Vector red(1.0,0.0,0.0);
    Vector blue(0.0,0.0,1.0);
    Vector yellow(1.0,1.0,0.0);

    Matrix transformMatrix;
    int offset = 0;
    for(int y = 0; y<4; ++y)
    {
        for(int x =0;x<4;++x)
        {
            transformMatrix.m[y][x] = *(transform.data() + offset);
            offset++;
        }
    }

    switch(m_mode)
    {
    case AxisCursorMode::MoveAxis:
        switch(cameraMode)
        {
        case CameraMode::Perspective:
            m_backend->drawAxisCursorMove(m_moveXRenderable, transformMatrix, red);
            m_backend->drawAxisCursorMove(m_moveYRenderable, transformMatrix, blue);
            m_backend->drawAxisCursorMove(m_moveZRenderable, transformMatrix, yellow);
            break;
        case CameraMode::Front:
        case CameraMode::Back:
            m_backend->drawAxisCursorMove(m_moveXRenderable, transformMatrix, red);
            m_backend->drawAxisCursorMove(m_moveZRenderable, transformMatrix, yellow);
            break;
        case CameraMode::Top:
        case CameraMode::Bottom:
            m_backend->drawAxisCursorMove(m_moveXRenderable, transformMatrix, red);
            m_backend->drawAxisCursorMove(m_moveYRenderable, transformMatrix, blue);
            break;
        case CameraMode::Left:
        case CameraMode::Right:
            m_backend->drawAxisCursorMove(m_moveYRenderable, transformMatrix, blue);
            m_backend->drawAxisCursorMove(m_moveZRenderable, transformMatrix, yellow);
            break;
        }
        break;
    case AxisCursorMode::RotateAxis:
        switch(cameraMode)
        {
        case CameraMode::Perspective:
            m_backend->drawAxisCursorRotate(m_rotateXRenderable, transformMatrix, red);
            m_backend->drawAxisCursorRotate(m_rotateYRenderable, transformMatrix, blue);
            m_backend->drawAxisCursorRotate(m_rotateZRenderable, transformMatrix, yellow);
            break;
        case CameraMode::Front:
        case CameraMode::Back:
            m_backend->drawAxisCursorRotate(m_rotateXRenderable, transformMatrix, red);
            m_backend->drawAxisCursorRotate(m_rotateZRenderable, transformMatrix, yellow);
            break;
        case CameraMode::Top:
        case CameraMode::Bottom:
            m_backend->drawAxisCursorRotate(m_rotateXRenderable, transformMatrix, red);
            m_backend->drawAxisCursorRotate(m_rotateYRenderable, transformMatrix, blue);
            break;
        case CameraMode::Left:
        case CameraMode::Right:
            m_backend->drawAxisCursorRotate(m_rotateYRenderable, transformMatrix, blue);
            m_backend->drawAxisCursorRotate(m_rotateZRenderable, transformMatrix, yellow);
            break;
        }
        break;
    case AxisCursorMode::ScaleAxis:
        switch(cameraMode)
        {
        case CameraMode::Perspective:
            m_backend->drawAxisCursorScale(m_scaleXRenderable, transformMatrix, red);
            m_backend->drawAxisCursorScale(m_scaleYRenderable, transformMatrix, blue);
            m_backend->drawAxisCursorScale(m_scaleZRenderable, transformMatrix, yellow);
            break;
        case CameraMode::Front:
        case CameraMode::Back:
            m_backend->drawAxisCursorScale(m_scaleXRenderable, transformMatrix, red);
            m_backend->drawAxisCursorScale(m_scaleZRenderable, transformMatrix, yellow);
            break;
        case CameraMode::Top:
        case CameraMode::Bottom:
            m_backend->drawAxisCursorScale(m_scaleXRenderable, transformMatrix, red);
            m_backend->drawAxisCursorScale(m_scaleYRenderable, transformMatrix, blue);
            break;

        case CameraMode::Left:
        case CameraMode::Right:
            m_backend->drawAxisCursorScale(m_scaleYRenderable, transformMatrix, blue);
            m_backend->drawAxisCursorScale(m_scaleZRenderable, transformMatrix, yellow);
            break;
        }
        break;
    default:
        qDebug() << "error";
    }
}
