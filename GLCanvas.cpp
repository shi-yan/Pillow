#include "GLCanvas.h"
#include "Scene.h"
#include "Screen.h"

Scene *theScene;
Screen *theScreen;

GLCanvas::GLCanvas(QWidget *parent) : QOpenGLWidget(parent)
{
    theScene=new Scene();
    theScene->initialize();
}

void GLCanvas::initializeGL()
{
    theScreen=new Screen();
	theScreen->initialize();
    int w = 800, h = 600;
//	GetClientSize(&w, &h);
	{
//		SetCurrent();
		theScreen->width=w;
		theScreen->height=h;
		theScreen->updateScreen(w,h);
    }
}

void GLCanvas::paintGL()
{
    theScreen->onPaint();
}

