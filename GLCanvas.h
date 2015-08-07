#ifndef GLCANVAS_H
#define GLCANVAS_H

#include <QOpenGLWidget>

class GLCanvas : public QOpenGLWidget
{
public:
    GLCanvas(QWidget *parent);
    
    void initializeGL();

    void paintGL();
};

#endif // GLCANVAS_H
