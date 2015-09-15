#ifndef GLCANVAS_H
#define GLCANVAS_H

#include <QOpenGLWidget>
#include <QMouseEvent>
#include "Core/Scene.h"
#include "Screen.h"
extern Scene *theScene;

class GLCanvas : public QOpenGLWidget
{
private:
    bool isDragging;
public:
    GLCanvas(QWidget *parent);
    
    void initializeGL();

    void paintGL();

    void resizeGL(int w, int h);

    void mouseMoveEvent(QMouseEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);

    void wheelEvent(QWheelEvent *e);

    void keyPressEvent(QKeyEvent *e);
    void keyReleaseEvent(QKeyEvent *e);

    void enterEvent(QEvent *e);
};

#endif // GLCANVAS_H
