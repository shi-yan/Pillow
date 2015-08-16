#include "GLCanvas.h"

#include <QDebug>

Scene *theScene = NULL;
Screen *theScreen = NULL;

GLCanvas::GLCanvas(QWidget *parent) : QOpenGLWidget(parent),isDragging(false)
{
    setMouseTracking(true);
    setFocusPolicy(Qt::StrongFocus);
}

void GLCanvas::initializeGL()
{
    theScene=new Scene();
    theScene->initialize();
    theScreen=new Screen();
	theScreen->initialize();
    int w = 800, h = 600;

		theScreen->width=w;
		theScreen->height=h;
		theScreen->updateScreen(w,h);


        {

            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();
            gluPerspective(25,(float)w/(float)h,1,8000);
            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();
            gluLookAt(100,100,100,0,0,0,0,0,1);
            glViewport(0, 0, (GLint) w, (GLint) h);
        }
        glLineWidth(3);
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_LIGHTING);
        glEnable(GL_LIGHT0);

}

void GLCanvas::paintGL()
{
    theScreen->onPaint();
}


void GLCanvas::mouseMoveEvent(QMouseEvent *e)
{
    if (e->buttons() == Qt::NoButton)
    {
        theScreen->onMoving(e->x()*2.0, e->y()*2.0);
    }
    else
    {
        if (e->buttons() == Qt::LeftButton)
        {
            theScreen->onLeftDrag(e->x()*2.0,e->y()*2.0);
        }
        else if (e->buttons() == Qt::MiddleButton)
        {
            theScreen->onMiddleDrag(e->x()*2.0, e->y()*2.0);
        }
    }
    update();
}

void GLCanvas::mousePressEvent(QMouseEvent *e)
{
    isDragging = true;
    theScreen->onLeftPress(e->x()*2.0, e->y()*2.0);
}

void GLCanvas::mouseReleaseEvent(QMouseEvent *e)
{
    isDragging = false;

    makeCurrent();
    theScreen->onLeftRelease(false);


    update();
}

void GLCanvas::resizeGL(int w, int h)
{
    theScreen->updateScreen(w*2.0, h*2.0);
}

void GLCanvas::keyPressEvent(QKeyEvent *e)
{
    if(e->key() == Qt::Key_Alt)
    {
        theScreen->onAltDown();
    }
    else if(e->key() == Qt::Key_Control)
    {
        theScreen->onCtrlDown();
    }
    else if(e->key() == Qt::Key_Z)//z
    {
        theScreen->onExtrudePress();
    }
}

void GLCanvas::keyReleaseEvent(QKeyEvent *e)
{
    int keyCode=e->key();
    if(keyCode==Qt::Key_Alt)
    {
        theScreen->onAltUp();
    }
    else if(keyCode==Qt::Key_Control)
    {
        theScreen->onCtrlUp();
    }
    else if(keyCode==Qt::Key_W) //w
    {
        theScene->changeAxisCursorMode(AxisCursorMode::MoveAxis);
    }
    else if(keyCode==Qt::Key_E) //e
    {
        theScene->changeAxisCursorMode(AxisCursorMode::RotateAxis);
    }
    else if(keyCode==Qt::Key_R) //r
    {
        theScene->changeAxisCursorMode(AxisCursorMode::ScaleAxis);
    }
    else if(keyCode==Qt::Key_C) //c
    {
        theScene->subdivide();
    }
    else if(keyCode==Qt::Key_V) //v
    {
        theScene->unSubdivide();
    }
    else if(keyCode==Qt::Key_Z) //z
    {
        theScreen->onExtrudeRelease();
    }
    else if(keyCode==Qt::Key_Delete)
    {
        theScene->onDelete();
    }
    else if(keyCode==49)
    {
        theScreen->changeCamera(CameraMode::Top);
    }
    else if(keyCode==50)
    {
        theScreen->changeCamera(CameraMode::Bottom);
    }
    else if(keyCode==51)
    {
        theScreen->changeCamera(CameraMode::Front);
    }
    else if(keyCode==52)
    {
        theScreen->changeCamera(CameraMode::Back);
    }
    else if(keyCode==53)
    {
        theScreen->changeCamera(CameraMode::Left);
    }
    else if(keyCode==54)
    {
        theScreen->changeCamera(CameraMode::Right);
    }
    else if(keyCode==55)
    {
        theScreen->changeCamera(CameraMode::Perspective);
    }

    update();
}


void GLCanvas::enterEvent(QEvent *e)
{
    setFocus();
}

void GLCanvas::wheelEvent(QWheelEvent *e)
{
    qDebug() << e->angleDelta();
  if( theScreen->onWheel(((float)e->angleDelta().y())*0.1,e->x()*2.0,e->y()*2.0))
  {
      update();
  }
}
