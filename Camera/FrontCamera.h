#pragma once
#include "Camera.h"

class FrontCamera :
	public Camera
{
public:
	FrontCamera(float width,float height,float nearPlane,float farPlane,float startX,float startY,Vector target,Vector eye,Vector up);

    void drawGird() const override;

    Vector getHorizontalDir() const override;

    void setCamera() override;

    void setReferenceImage(const char *path,GLuint imageID,Vector &position,size_t width,size_t height) override;

    void getViewportImage(GLuint &texID,std::string &path,Vector &position,size_t &width,size_t &height) override;

    void disableReference() override;
	
    void setCameraForSelectionD(size_t x1,size_t y1,size_t x2,size_t y2,size_t h) override;

    void setCameraForSelectionS() override;
	
    void zoom(float step) override;
		
    void onPanPress(int x,int y) override;

    void onPanRelease(int x,int y) override;

    Vector getEye() const override;

    void pan(int x,int y) override;

    ~FrontCamera(void) override;
};
