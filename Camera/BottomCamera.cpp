#include "BottomCamera.h"
#include "../UI/Grid.h"
#include "../Core/Matrix.h"
#include "../Core/Vector.cpp"
#include "../Core/Quaternion.h"

ViewportImage *bottomReference=NULL;

BottomCamera::BottomCamera(OpenGLBackend *backend, float width, float height, float nearPlane, float farPlane, float startX, float startY, const Vector &target, const Vector &eye, const Vector &up)
    :Camera(backend, CameraMode::Bottom, width, height, nearPlane, farPlane, startX, startY, target, eye, up)
{
    m_lightAmbient[0]=0.4f;
    m_lightAmbient[1]=0.4f;
    m_lightAmbient[2]=0.4f;
    m_lightAmbient[3]=1;

    m_lightDiffuse[0]=0.6f;
    m_lightDiffuse[1]=0.6f;
    m_lightDiffuse[2]=0.6f;
    m_lightDiffuse[3]=1;

    m_lightSpecular[0]=0.7f;
    m_lightSpecular[1]=0.7f;
    m_lightSpecular[2]=0.7f;
    m_lightSpecular[3]=1;

    m_lightPosition[0]=0;
    m_lightPosition[1]=0;
    m_lightPosition[2]=-500;
    m_lightPosition[3]=1;
}

BottomCamera::~BottomCamera(void)
{
}

void BottomCamera::setCamera()
{
    /*glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(m_eye.x,m_eye.y,-990,m_target.x,m_target.y,m_target.z,m_up.x,m_up.y,m_up.z);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-m_width*(m_eye.z)*0.005f,m_width*(m_eye.z)*0.005f,-m_height*(m_eye.z)*0.005f,m_height*(m_eye.z)*0.005f,m_nearPlane,m_farPlane);
    glViewport((GLint)m_startX,(GLint)m_startY,(GLint)m_width,(GLint)m_height);
    drawGird();
    if(bottomReference && bottomReference->isShow)
    {
        bottomReference->onPaint();
    }
    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0, GL_AMBIENT, m_lightAmbient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, m_lightDiffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, m_lightSpecular);
    glLightfv(GL_LIGHT0, GL_POSITION, m_lightPosition);*/
}

void BottomCamera::setCameraForSelectionS()
{
 /*   glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(m_eye.x,m_eye.y,-990,m_target.x,m_target.y,m_target.z,m_up.x,m_up.y,m_up.z);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-m_width*(m_eye.z)*0.005f,m_width*(m_eye.z)*0.005f,-m_height*(m_eye.z)*0.005f,m_height*(m_eye.z)*0.005f,m_nearPlane,m_farPlane);
    glViewport((GLint)m_startX,(GLint)m_startY,(GLint)m_width,(GLint)m_height);*/
}

void BottomCamera::setCameraForSelectionD(unsigned int x1,unsigned int y1,unsigned int x2,unsigned int y2,unsigned int h)
{
 /*   glViewport((GLint)m_startX,(GLint)m_startY,(GLint)m_width,(GLint)m_height);
    GLint viewport[4];
    glGetIntegerv (GL_VIEWPORT, viewport);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    unsigned int sw=(x2-x1)>5?(x2-x1):5;
    unsigned int sh=(y2-y1)>5?(y2-y1):5;
    gluPickMatrix((GLdouble) (x1+x2)/2,(GLdouble) (h - (y1+y2)/2), (GLdouble)sw,(GLdouble)sh, viewport);
    glOrtho(-m_width*(m_eye.z)*0.005f,m_width*(m_eye.z)*0.005f,-m_height*(m_eye.z)*0.005f,m_height*(m_eye.z)*0.005f,m_nearPlane,m_farPlane+100);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(m_eye.x,m_eye.y,-990,m_target.x,m_target.y,m_target.z,m_up.x,m_up.y,m_up.z);*/
}

void BottomCamera::setReferenceImage(const char *path,GLuint imageID,Vector &position,unsigned int width,unsigned int height)
{
    if(bottomReference)
    {
        delete bottomReference;
    }

    bottomReference=new ViewportImage(path,imageID,m_width,m_height,m_type._value,position);
    bottomReference->initialize();
    bottomReference->m_isShow=true;
}

void BottomCamera::getViewportImage(GLuint &texID,std::string &path,Vector &position,unsigned int &width,unsigned int &height)
{
    if(bottomReference)
    {
        texID=bottomReference->m_imageID;
        path=bottomReference->m_path;
        position=bottomReference->m_position;
        width=bottomReference->m_width;
        height=bottomReference->m_height;
    }
    else
    {
        texID=0;
        path="";
        position.vec(0);
        width=0;
        height=0;
    }
}

void BottomCamera::disableReference()
{
    bottomReference->m_isShow=false;
}

Vector BottomCamera::getEye() const
{
    return m_eye*10.0f;
}

Vector BottomCamera::getHorizontalDir() const
{
    return Vector(1,0,0);
}

void BottomCamera::drawGird() const
{
    if(m_showGrid)
    {
        Grid::grid->drawXY();
    }
}

void BottomCamera::zoom(float step)
{
    Vector cameraDirection(0,0,-1);
    m_eye+=cameraDirection*(float)(step*0.1f);
    if(m_eye.z<2) m_eye.z=2;
}

void BottomCamera::onPanPress(int x,int y)
{
    m_isDraging=true;
    m_old.x=(float)x;
    m_old.y=(float)y;
}

void BottomCamera::onPanRelease(int x,int y)
{
    x;
    y;
    m_isDraging=false;
    m_old.null();
}

void BottomCamera::pan(int x,int y)
{
    float dx=(float)(x-m_old.x)*m_eye.z*0.01f;
    float dy=(float)(y-m_old.y)*m_eye.z*0.01f;
    m_old.x=(float)x;
    m_old.y=(float)y;
    //dx=10;
    m_eye.x+=dx;
    m_eye.y+=dy;
    m_target.x+=dx;
    m_target.y+=dy;
}
