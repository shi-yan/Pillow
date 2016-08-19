#include "GLCanvas.h"
#include <QOpenGLContext>
#include <QDebug>

GLCanvas::GLCanvas(QWidget *parent) : QOpenGLWidget(parent),isDragging(false)
{
    setMouseTracking(true);
    setFocusPolicy(Qt::StrongFocus);
}

void GLCanvas::initializeGL()
{
    qDebug() << this->context()->isOpenGLES();
    Screen::screen = new Screen();
    Scene::scene=new Scene(Screen::screen->m_graphicsBackend);
   // Scene::scene->initialize();
    Screen::screen->initialize();
    int w = 800, h = 600;


        Screen::screen->updateScreenSize(w,h);


        {

          /*  glMatrixMode(GL_PROJECTION);
            glLoadIdentity();
            gluPerspective(25,(float)w/(float)h,1,8000);
            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();
            gluLookAt(100,100,100,0,0,0,0,0,1);
            glViewport(0, 0, (GLint) w, (GLint) h);*/
        }
        glLineWidth(3);
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_LIGHTING);
        glEnable(GL_LIGHT0);

}

void GLCanvas::paintGL()
{
    Screen::screen->onPaint();
}

void GLCanvas::mouseMoveEvent(QMouseEvent *e)
{
    if (e->buttons() == Qt::NoButton)
    {
        Screen::screen->onMoving(e->x()*1.0, e->y()*1.0);
    }
    else
    {
        if (e->buttons() == Qt::LeftButton)
        {
            Screen::screen->onLeftDrag(e->x()*1.0,e->y()*1.0);
        }
        else if (e->buttons() == Qt::MiddleButton)
        {
            Screen::screen->onMiddleDrag(e->x()*1.0, e->y()*1.0);
        }
    }
    update();
}

void GLCanvas::mousePressEvent(QMouseEvent *e)
{
    isDragging = true;
    Screen::screen->onLeftPress(e->x()*1.0, e->y()*1.0);

    update();
}

void GLCanvas::mouseReleaseEvent(QMouseEvent *e)
{
    isDragging = false;

    makeCurrent();
    Screen::screen->onLeftRelease(false);

    update();
}

void GLCanvas::resizeGL(int w, int h)
{
    Screen::screen->updateScreenSize(w*1.0, h*1.0);
}

void GLCanvas::keyPressEvent(QKeyEvent *e)
{
    if(e->key() == Qt::Key_Alt)
    {
        Screen::screen->onAltDown();
    }
    else if(e->key() == Qt::Key_Control)
    {
        Screen::screen->onCtrlDown();
    }
    else if(e->key() == Qt::Key_Z)//z
    {
        Screen::screen->onExtrudePress();
    }
}

void GLCanvas::keyReleaseEvent(QKeyEvent *e)
{
    int keyCode=e->key();
    if(keyCode==Qt::Key_Alt)
    {
        Screen::screen->onAltUp();
    }
    else if(keyCode==Qt::Key_Control)
    {
        Screen::screen->onCtrlUp();
    }
    else if(keyCode==Qt::Key_W) //w
    {
        Scene::scene->changeAxisCursorMode(AxisCursorMode::MoveAxis);
    }
    else if(keyCode==Qt::Key_E) //e
    {
        Scene::scene->changeAxisCursorMode(AxisCursorMode::RotateAxis);
    }
    else if(keyCode==Qt::Key_R) //r
    {
        Scene::scene->changeAxisCursorMode(AxisCursorMode::ScaleAxis);
    }
    else if(keyCode==Qt::Key_C) //c
    {
        Scene::scene->subdivide();
    }
    else if(keyCode==Qt::Key_V) //v
    {
        Scene::scene->unSubdivide();
    }
    else if(keyCode==Qt::Key_Z) //z
    {
        Screen::screen->onExtrudeRelease();
    }
    else if(keyCode==Qt::Key_Delete)
    {
        Scene::scene->onDelete();
    }
    else if(keyCode==49)
    {
        Screen::screen->changeCamera(CameraMode::Top);
    }
    else if(keyCode==50)
    {
        Screen::screen->changeCamera(CameraMode::Bottom);
    }
    else if(keyCode==51)
    {
        Screen::screen->changeCamera(CameraMode::Front);
    }
    else if(keyCode==52)
    {
        Screen::screen->changeCamera(CameraMode::Back);
    }
    else if(keyCode==53)
    {
        Screen::screen->changeCamera(CameraMode::Left);
    }
    else if(keyCode==54)
    {
        Screen::screen->changeCamera(CameraMode::Right);
    }
    else if(keyCode==55)
    {
        Screen::screen->changeCamera(CameraMode::Perspective);
    }

    update();
}


void GLCanvas::enterEvent(QEvent *e)
{
    setFocus();
}

void GLCanvas::wheelEvent(QWheelEvent *e)
{
  if( Screen::screen->onWheel(((float)e->angleDelta().y())*0.1,e->x()*1.0,e->y()*1.0))
  {
      update();
  }
}
