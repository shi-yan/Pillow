#include "Camera.h"
#include "../UI/Grid.h"
#include "../Core/Matrix.h"
#include "../Core/Vector.cpp"
#include "../Core/Quaternion.h"

Camera::Camera(OpenGLBackend *backend, CameraMode::__Enum type, float width, float height, float nearPlane, float farPlane, float startX, float startY, const Vector &target, const Vector &eye, const Vector &up)
    : m_backend(backend),
      m_old(0),
      m_isDraging(false),
      m_type(type),
      m_width(width),
      m_height(height),
      m_nearPlane(nearPlane),
      m_farPlane(farPlane),
      m_startX(startX),
      m_startY(startY),
      m_target(target),
      m_eye(eye),
      m_up(up),
      m_showGrid(true)
{
}

Camera::~Camera(void)
{
}

void Camera::zoom(float step)
{
    UNUSED(step);
}

void Camera::onPanPress(int x, int y)
{
    UNUSED(x);
    UNUSED(y);
}

void Camera::disableReference() {}

void Camera::getViewportImage(GLuint &texID,std::string &path,Vector &position,unsigned int &width,unsigned int &height)
{
    UNUSED(texID);
    UNUSED(path);
    UNUSED(position);
    UNUSED(width);
    UNUSED(height);
}

void Camera::setReferenceImage(const char *path,GLuint imageID,Vector &position,unsigned int width,unsigned int height)
{
    UNUSED(path);
    UNUSED(imageID);
    UNUSED(position);
    UNUSED(width);
    UNUSED(height);
}

void Camera::onRotatePress(int x,int y)
{
    UNUSED(x);
    UNUSED(y);
}

Vector Camera::getEye() const
{
    return Vector(0);
}

void Camera::rotate(int x,int y)
{
    UNUSED(x);
    UNUSED(y);
}

void Camera::onRotateRelease()
{
}

void Camera::onPanRelease(int x,int y)
{
    UNUSED(x);
    UNUSED(y);
}

void Camera::setCamera()
{
}

void Camera::setCameraForSelectionD(unsigned int x1,unsigned int y1,unsigned int x2,unsigned int y2,unsigned int h)
{
    UNUSED(x1);
    UNUSED(y1);
    UNUSED(x2);
    UNUSED(y2);
    UNUSED(h);
}

void Camera::setCameraForSelectionS()
{
}

void Camera::drawGird() const
{
}

Vector Camera::getHorizontalDir() const
{
    return Vector(0,0,0);
}

void Camera::pan(int x,int y)
{
    UNUSED(x);
    UNUSED(y);
}

void Camera::updateSize(float theStartX, float theStartY, float theWidth, float theHeight)
{
    m_startX = theStartX;
    m_startY = theStartY;
    m_width = theWidth;
    m_height = theHeight;
}
