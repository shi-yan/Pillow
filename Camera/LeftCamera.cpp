#include "LeftCamera.h"
#include "UI/Grid.h"
#include "../Core/Matrix.h"
#include "../Core/Vector.cpp"
#include "../Core/Quaternion.h"

ViewportImage *leftReference=NULL;

LeftCamera::LeftCamera(OpenGLBackend *backend, float width,float height,float nearPlane,float farPlane,float startX,float startY,Vector target,Vector eye,Vector up)
    :Camera(backend, CameraMode::Left, width, height, nearPlane, farPlane, startX, startY, target, eye, up)
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

    m_lightPosition[0]=-500;
    m_lightPosition[1]=0;
    m_lightPosition[2]=0;
    m_lightPosition[3]=1;
}

LeftCamera::~LeftCamera(void)
{
}

void LeftCamera::setCamera()
{
    /*glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(-990,m_eye.y,m_eye.z,m_target.x,m_target.y,m_target.z,m_up.x,m_up.y,m_up.z);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-m_width*(m_eye.x)*0.005f,m_width*(m_eye.x)*0.005f,-m_height*(m_eye.x)*0.005f,m_height*(m_eye.x)*0.005f,m_nearPlane,m_farPlane);
    glViewport((GLint)m_startX,(GLint)m_startY,(GLint)m_width,(GLint)m_height);
            drawGird();
            if(leftReference && leftReference->isShow)
    {
        leftReference->onPaint();
    }
    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0, GL_AMBIENT, m_lightAmbient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, m_lightDiffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, m_lightSpecular);
    glLightfv(GL_LIGHT0, GL_POSITION, m_lightPosition);*/
}

void LeftCamera::setReferenceImage(const char *path,GLuint imageID,Vector &position,unsigned int width,unsigned int height)
{
        if(leftReference)
        {
            delete leftReference;
        }

        leftReference=new ViewportImage(path,imageID,m_width,m_height,m_type._value,position);
        leftReference->initialize();
        leftReference->isShow=true;
}

void LeftCamera::getViewportImage(GLuint &texID,std::string &path,Vector &position,unsigned int &width,unsigned int &height)
{
    if(leftReference)
    {
        texID=leftReference->imageID;
        path=leftReference->path;
        position=leftReference->position;
        width=leftReference->width;
        height=leftReference->height;
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

void LeftCamera::disableReference()
{
    leftReference->isShow=false;
}

void LeftCamera::drawGird() const
{
    if(m_showGrid)
    {
        Grid::grid->drawYZ();
    }
}

void LeftCamera::zoom(float step)
{
    Vector cameraDirection(-1,0,0);
    m_eye+=cameraDirection*(float)(step*0.1f);
    if(m_eye.x<2) m_eye.x=2;
}

void LeftCamera::setCameraForSelectionS()
{
    /*glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(-990,m_eye.y,m_eye.z,m_target.x,m_target.y,m_target.z,m_up.x,m_up.y,m_up.z);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-m_width*(m_eye.x)*0.005f,m_width*(m_eye.x)*0.005f,-m_height*(m_eye.x)*0.005f,m_height*(m_eye.x)*0.005f,m_nearPlane,m_farPlane);
    glViewport((GLint)m_startX,(GLint)m_startY,(GLint)m_width,(GLint)m_height);*/
}

void LeftCamera::setCameraForSelectionD(unsigned int x1,unsigned int y1,unsigned int x2,unsigned int y2,unsigned int h)
{
    glViewport((GLint)m_startX,(GLint)m_startY,(GLint)m_width,(GLint)m_height);
    GLint viewport[4];
    glGetIntegerv (GL_VIEWPORT, viewport);
    /*glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    unsigned int sw=(x2-x1)>5?(x2-x1):5;
    unsigned int sh=(y2-y1)>5?(y2-y1):5;
    gluPickMatrix((GLdouble) (x1+x2)/2,(GLdouble) (h - (y1+y2)/2), (GLdouble)sw,(GLdouble)sh, viewport);
    glOrtho(-m_width*(m_eye.x)*0.005f,m_width*(m_eye.x)*0.005f,-m_height*(m_eye.x)*0.005f,m_height*(m_eye.x)*0.005f,m_nearPlane,m_farPlane+100);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(-990,m_eye.y,m_eye.z,m_target.x,m_target.y,m_target.z,m_up.x,m_up.y,m_up.z);*/
}

Vector LeftCamera::getEye() const
{
    return m_eye*10.0f;
}

Vector LeftCamera::getHorizontalDir() const
{
    return Vector(0,1,0);
}

void LeftCamera::onPanPress(int x,int y)
{
    m_isDraging=true;
    m_old.x=(float)x;
    m_old.y=(float)y;
}

void LeftCamera::onPanRelease(int x,int y)
{
    x;
    y;
    m_isDraging=false;
    m_old.null();
}

void LeftCamera::pan(int x,int y)
{
    float dy=(float)(x-m_old.x)*m_eye.x*0.01f;
    float dz=(float)(y-m_old.y)*m_eye.x*0.01f;
    m_old.x=(float)x;
    m_old.y=(float)y;
    m_eye.y+=dy;
    m_eye.z+=dz;
    m_target.y+=dy;
    m_target.z+=dz;
}
