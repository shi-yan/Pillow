#pragma once
#include "Camera.h"

class FrontCamera : public Camera
{
public:
    FrontCamera(OpenGLBackend *backend, float width,float height,float nearPlane,float farPlane,float startX,float startY,Vector target,Vector eye,Vector up);

    void drawGird() const override;

    Vector getHorizontalDir() const override;

    void setCamera() override;

    void setReferenceImage(const char *path,GLuint imageID,Vector &position,unsigned int width,unsigned int height) override;

    void getViewportImage(GLuint &texID,std::string &path,Vector &position,unsigned int &width,unsigned int &height) override;

    void disableReference() override;

    void setCameraForSelectionD(unsigned int x1,unsigned int y1,unsigned int x2,unsigned int y2,unsigned int h) override;

    void setCameraForSelectionS() override;

    void zoom(float step) override;

    void onPanPress(int x,int y) override;

    void onPanRelease(int x,int y) override;

    Vector getEye() const override;

    void pan(int x,int y) override;

    ~FrontCamera(void) override;
};
