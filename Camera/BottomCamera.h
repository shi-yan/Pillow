#pragma once
#include "Camera.h"

class BottomCamera : public Camera
{
public:
	BottomCamera(float width,float height,float nearPlane,float farPlane,float startX,float startY,Vector target,Vector eye,Vector up);

    void setCamera() override;

    void setCameraForSelectionS() override;

    void setCameraForSelectionD(size_t x1,size_t y1,size_t x2,size_t y2,size_t h) override;

    void setReferenceImage(const char *path,GLuint imageID,Vector &position,size_t width,size_t height) override;

    void getViewportImage(GLuint &texID,std::string &path,Vector &position,size_t &width,size_t &height) override;

    void disableReference() override;

    Vector getEye() const override;

    Vector getHorizontalDir() const override;

    void drawGird() const override;

    void zoom(float step) override;

    void onPanPress(int x,int y) override;

    void onPanRelease(int x,int y) override;

    void pan(int x,int y) override;

    ~BottomCamera(void) override;
};
