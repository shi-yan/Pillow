#pragma once
#include "Camera.h"

class TopCamera : public Camera
{
public:
    TopCamera(OpenGLBackend *backend, float width, float height, float nearPlane, float farPlane, float startX, float startY, const Vector &target, const Vector &eye, const Vector &up);

    void setCamera() override;

    void setCameraForSelectionS() override;

    void setCameraForSelectionD(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2, unsigned int h) override;

    void setReferenceImage(const char *path, GLuint imageID, Vector &position, unsigned int width, unsigned int height) override;

    void getViewportImage(GLuint &texID, std::string &path, Vector &position, unsigned int &width, unsigned int &height) override;

    void disableReference() override;

    Vector getEye() const override;

    Vector getHorizontalDir() const override;

    void drawGird() const override;

    void zoom(float step) override;

    void onPanPress(int x, int y) override;

    void onPanRelease(int x, int y) override;

    void pan(int x, int y) override;

    ~TopCamera(void) override;
};
