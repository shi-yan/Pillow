#include "TopCamera.h"
#include "../UI/Grid.h"
#include "../Core/Matrix.h"
#include "../Core/Vector.cpp"
#include "../Core/Quaternion.h"

ViewportImage *topReference=NULL;

TopCamera::TopCamera(float width,float height,float nearPlane,float farPlane,float startX,float startY,Vector target,Vector eye,Vector up)
    :Camera(CameraMode::Top, width, height, nearPlane, farPlane, startX, startY, target, eye, up)
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
    m_lightPosition[2]=500;
    m_lightPosition[3]=1;
}

TopCamera::~TopCamera(void)
{
}

void TopCamera::setCamera()
{
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt( m_eye.x, m_eye.y,990, m_target.x, m_target.y, m_target.z, m_up.x, m_up.y, m_up.z);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-m_width*m_eye.z*0.005f,m_width*m_eye.z*0.005f,-m_height*m_eye.z*0.005f,m_height*m_eye.z*0.005f,m_nearPlane,m_farPlane);
    glViewport((GLint) m_startX,(GLint) m_startY,(GLint) m_width,(GLint) m_height);
    drawGird();
    if(topReference && topReference->isShow)
    {
        topReference->onPaint();
    }
    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0, GL_AMBIENT, m_lightAmbient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, m_lightDiffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, m_lightSpecular);
    glLightfv(GL_LIGHT0, GL_POSITION, m_lightPosition);
};

void TopCamera::setCameraForSelectionS()
{
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt( m_eye.x, m_eye.y,990, m_target.x, m_target.y, m_target.z, m_up.x, m_up.y, m_up.z);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-m_width*m_eye.z*0.005f,m_width*m_eye.z*0.005f,-m_height*m_eye.z*0.005f,m_height*m_eye.z*0.005f,m_nearPlane,m_farPlane);
    glViewport((GLint) m_startX,(GLint) m_startY,(GLint) m_width,(GLint) m_height);
};

void TopCamera::setCameraForSelectionD(unsigned int x1,unsigned int y1,unsigned int x2,unsigned int y2,unsigned int h)
{
    glViewport((GLint)m_startX,(GLint)m_startY,(GLint)m_width,(GLint)m_height);
    GLint viewport[4];
    glGetIntegerv (GL_VIEWPORT, viewport);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    unsigned int sw=(x2-x1)>5?(x2-x1):5;
    unsigned int sh=(y2-y1)>5?(y2-y1):5;
    gluPickMatrix((GLdouble) (x1+x2)/2,(GLdouble) (h - (y1+y2)/2), (GLdouble)sw,(GLdouble)sh, viewport);
    glOrtho(-m_width*m_eye.z*0.005f,m_width*m_eye.z*0.005f,-m_height*m_eye.z*0.005f,m_height*m_eye.z*0.005f,m_nearPlane,m_farPlane);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(m_eye.x,m_eye.y,990,m_target.x,m_target.y,m_target.z,m_up.x,m_up.y,m_up.z);
};

void TopCamera::setReferenceImage(const char *path,GLuint imageID,Vector &position,unsigned int width,unsigned int height)
{
        if(topReference)
        {
            delete topReference;
        }

        topReference=new ViewportImage(path,imageID,m_width,m_height,m_type._value,position);
        topReference->initialize();
        topReference->isShow=true;
};

void TopCamera::getViewportImage(GLuint &texID,std::string &path,Vector &position,unsigned int &width,unsigned int &height)
{
    if(topReference)
    {
        texID=topReference->imageID;
        path=topReference->path;
        position=topReference->position;
        width=topReference->width;
        height=topReference->height;
    }
    else
    {
        texID=0;
        path="";
        position.vec(0);
        width=0;
        height=0;
    }
};

    void TopCamera::disableReference()
{
    topReference->isShow=false;
}


    Vector TopCamera::getEye() const
{
    return m_eye*10.0f;
}

Vector TopCamera::getHorizontalDir() const
{
    return Vector(-1,0,0);
}

void TopCamera::drawGird() const
{
    if(m_showGird)
    {
        theGird->drawXY();
    }
};

void TopCamera::zoom(float step)
{
    Vector cameraDirection(0,0,-1);
     m_eye+=cameraDirection*(float)(step*0.1f);
    if( m_eye.z<2)  m_eye.z=2;
};

void TopCamera::onPanPress(int x,int y)
{
     m_isDraging=true;
     m_old.x=(float)x;
     m_old.y=(float)y;
};

void TopCamera::onPanRelease(int x,int y)
{
    x;
    y;
    m_isDraging=false;
    m_old.null();
};

void TopCamera::pan(int x,int y)
{
    float dx=(float)(m_old.x-x)* m_eye.z*0.005f;
    float dy=(float)(y-m_old.y)* m_eye.z*0.005f;
    m_old.x=(float)x;
    m_old.y=(float)y;

     m_eye.x+=dx;
     m_eye.y+=dy;
     m_target.x+=dx;
     m_target.y+=dy;
};

