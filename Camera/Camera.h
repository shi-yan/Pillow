#pragma once
#include "Enum.h"
#include <string>
#include "../Core/Vector.h"
#include "../View/ViewportImage.h"
#include "../GraphicsBackend/OpenGLBackend.h"
#define Epsilon 1.0e-5

class Camera
{
protected:
    Vector m_old;
    bool m_isDraging;
    OpenGLBackend *m_backend;

public:
    CameraMode m_type;
    float m_width;
    float m_height;
    float m_nearPlane;
    float m_farPlane;
    float m_startX;
    float m_startY;
    Vector m_target;
    Vector m_eye;
    Vector m_up;
    bool m_showGrid;

    GLfloat m_lightAmbient[4];
    GLfloat m_lightDiffuse[4];
    GLfloat m_lightSpecular[4];
    GLfloat m_lightPosition[4];

    virtual void zoom(float step);

    virtual void onPanPress(int x, int y);

    virtual void disableReference();

    virtual void getViewportImage(GLuint &texID, std::string &path, Vector &position, unsigned int &width, unsigned int &height);

    virtual void setReferenceImage(const char *path, GLuint imageID, Vector &position, unsigned int width, unsigned int height);

    virtual void onRotatePress(int x, int y);

    virtual Vector getEye() const;

    virtual void rotate(int x, int y);

    virtual void onRotateRelease();

    virtual void onPanRelease(int x, int y);

    virtual void setCamera();

    virtual void setCameraForSelectionD(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2, unsigned int h);

    virtual void setCameraForSelectionS();

    virtual void drawGird() const;

    virtual Vector getHorizontalDir() const;

    virtual void pan(int x, int y);

    void updateSize(float theStartX, float theStartY, float theWidth, float theHeight);

    Camera(OpenGLBackend *backend, CameraMode::__Enum type, float width, float height, float nearPlane, float farPlane, float startX, float startY, const Vector &target, const Vector &eye, const Vector &up);
    virtual ~Camera(void);
};
