#pragma once
#include "Camera.h"
#include <iostream>

#ifdef __APPLE__
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#endif

class BackCamera : public Camera
{
public:
    BackCamera(float width, float height, float nearPlane, float farPlane, float startX, float startY, Vector target, Vector eye, Vector up);

    void setCamera() override;

    void setCameraForSelectionS() override;

    void setCameraForSelectionD(unsigned int x1,unsigned int y1,unsigned int x2,unsigned int y2,unsigned int h) override;

    void setReferenceImage(const char *path,GLuint imageID,Vector &position,unsigned int width,unsigned int height) override;

    void getViewportImage(GLuint &texID,std::string &path,Vector &position,unsigned int &width,unsigned int &height) override;

    void disableReference() override;

    Vector getHorizontalDir() const override;

    Vector getEye() const override;

    void zoom(float step) override;

    void drawGird() const override;
		
    void onPanPress(int x,int y) override;

    void onPanRelease(int x,int y) override;

    void pan(int x,int y) override;

public:
    ~BackCamera(void) override;
};
