#include "FrontCamera.h"
#include "../UI/Grid.h"
#include "../Core/Matrix.h"
#include "../Core/Vector.cpp"
#include "../Core/Quaternion.h"

ViewportImage *frontReference=NULL;

FrontCamera::FrontCamera(OpenGLBackend *backend, float width, float height, float nearPlane, float farPlane, float startX, float startY, const Vector &target, const Vector &eye, const Vector &up)
    :Camera(backend, CameraMode::Front, width, height, nearPlane, farPlane, startX, startY, target, eye, up)
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
    m_lightPosition[1]=500;
    m_lightPosition[2]=0;
    m_lightPosition[3]=1;
}

FrontCamera::~FrontCamera(void)
{
}

void FrontCamera::drawGird() const
{
    if(m_showGrid)
    {
        Grid::grid->drawXZ();
    }
}

Vector FrontCamera::getHorizontalDir() const
{
    return Vector(1,0,0);
}

void FrontCamera::setCamera()
{
    /*glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(m_eye.x,990,m_eye.z,m_target.x,m_target.y,m_target.z,m_up.x,m_up.y,m_up.z);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-m_width*(m_eye.y)*0.005f,m_width*(m_eye.y)*0.005f,-m_height*(m_eye.y)*0.005f,m_height*(m_eye.y)*0.005f,m_nearPlane,m_farPlane);
    glViewport((GLint)m_startX,(GLint)m_startY,(GLint)m_width,(GLint)m_height);
    drawGird();
    if(frontReference && frontReference->isShow)
    {
        frontReference->onPaint();
    }
    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0, GL_AMBIENT, m_lightAmbient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, m_lightDiffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, m_lightSpecular);
    glLightfv(GL_LIGHT0, GL_POSITION, m_lightPosition);*/
}

void FrontCamera::setReferenceImage(const char *path,GLuint imageID,Vector &position,unsigned int width,unsigned int height)
{
    if(frontReference)
    {
        delete frontReference;
    }

    frontReference=new ViewportImage(path,imageID,m_width,m_height,m_type._value,position);
    frontReference->initialize();
    frontReference->isShow=true;
}

void FrontCamera::getViewportImage(GLuint &texID,std::string &path,Vector &position,unsigned int &width,unsigned int &height)
{
    if(frontReference)
    {
        texID=frontReference->imageID;
        path=frontReference->path;
        position=frontReference->position;
        width=frontReference->width;
        height=frontReference->height;
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

void FrontCamera::disableReference()
{
    frontReference->isShow=false;
}

void FrontCamera::setCameraForSelectionD(unsigned int x1,unsigned int y1,unsigned int x2,unsigned int y2,unsigned int h)
{
    glViewport((GLint)m_startX,(GLint)m_startY,(GLint)m_width,(GLint)m_height);
    GLint viewport[4];
    glGetIntegerv (GL_VIEWPORT, viewport);
    /*glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    unsigned int sw=(x2-x1)>5?(x2-x1):5;
    unsigned int sh=(y2-y1)>5?(y2-y1):5;
    gluPickMatrix((GLdouble) (x1+x2)/2,(GLdouble) (h - (y1+y2)/2), (GLdouble)sw,(GLdouble)sh, viewport);
    glOrtho(-m_width*(m_eye.y)*0.005f,m_width*(m_eye.y)*0.005f,-m_height*(m_eye.y)*0.005f,m_height*(m_eye.y)*0.005f,m_nearPlane,m_farPlane);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(m_eye.x,990,m_eye.z,m_target.x,m_target.y,m_target.z,m_up.x,m_up.y,m_up.z);*/
}

void FrontCamera::setCameraForSelectionS()
{
    /*glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(m_eye.x,990,m_eye.z,m_target.x,m_target.y,m_target.z,m_up.x,m_up.y,m_up.z);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-m_width*(m_eye.y)*0.005f,m_width*(m_eye.y)*0.005f,-m_height*(m_eye.y)*0.005f,m_height*(m_eye.y)*0.005f,m_nearPlane,m_farPlane);
    glViewport((GLint)m_startX,(GLint)m_startY,(GLint)m_width,(GLint)m_height);*/
}

void FrontCamera::zoom(float step)
{
    Vector cameraDirection(0,-1,0);
    m_eye+=cameraDirection*(float)(step*0.1f);
    if(m_eye.y<2) m_eye.y=2;
}

void FrontCamera::onPanPress(int x,int y)
{
    m_isDraging=true;
    m_old.x=(float)x;
    m_old.y=(float)y;
}

void FrontCamera::onPanRelease(int x,int y)
{
    x;
    y;
    m_isDraging=false;
    m_old.null();
}

Vector FrontCamera::getEye() const
{
    return m_eye * 10.0f;
}

void FrontCamera::pan(int x,int y)
{
    float dx=(float)(x-m_old.x)*m_eye.y*0.01f;
    float dz=(float)(y-m_old.y)*m_eye.y*0.01f;
    m_old.x=(float)x;
    m_old.y=(float)y;
    //dx=10;
    m_eye.x+=dx;
    m_eye.z+=dz;
    m_target.x+=dx;
    m_target.z+=dz;
}
