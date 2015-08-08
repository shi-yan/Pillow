#include "PerspectiveCamera.h"
#include "Gird.h"
#include "Matrix.h"
#include "Vector.cpp"
#include "Quaternion.h"

PerspectiveCamera::PerspectiveCamera(float angle,float width,float height,float nearPlane,float farPlane,float startX,float startY,Vector target,Vector eye,Vector up):
angle(angle),Camera(CameraMode::Perspective,width,height,nearPlane,farPlane,startX,startY,target,eye,up)
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

    m_lightPosition[0]=500;
    m_lightPosition[1]=500;
    m_lightPosition[2]=500;
    m_lightPosition[3]=1;

	rotate45V();
}


PerspectiveCamera::~PerspectiveCamera(void)
{
}

void PerspectiveCamera::zoom(float step)
{
    Vector cameraDirection(0);
    cameraDirection=m_target-m_eye;
    cameraDirection.normalize();
    m_eye+=cameraDirection*step;
}

void PerspectiveCamera::onPanPress(int x,int y)
{
    m_isDraging=true;
    m_old.x=(float)x;
    m_old.y=(float)y;
}

void PerspectiveCamera::onRotatePress(int x,int y)
{
        m_isDraging=true;
        m_old.x=(float)x;
        m_old.y=(float)y;
}

void PerspectiveCamera::drawGird() const
{
    if(m_showGird)
    {
        theGird->drawXY();
    }
    else
    {
        glDisable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D,0);
    }
}

void PerspectiveCamera::rotate45V()
{
        Vector perp=perpendicular(m_eye-m_target,m_up);
        perp.normalize();
        Quaternion rotateQuaternion((float)-10.0f,perp);
        Matrix rotateMatrix;
        rotateQuaternion.getMatrix(rotateMatrix);
        Vector cameraDirection=m_eye-m_target;
        Vector upDirection=cameraDirection+m_up;
        cameraDirection=(rotateMatrix)*(cameraDirection);
        upDirection=(rotateMatrix)*upDirection;
        m_eye=cameraDirection+m_target;
        m_lightPosition[0]=cameraDirection.x;
        m_lightPosition[1]=cameraDirection.y;
        m_lightPosition[2]=cameraDirection.z;
        m_lightPosition[3]=1;
        m_up=upDirection-cameraDirection;
}

void PerspectiveCamera::rotate(int x,int y)
{

    int dx=(int)(m_old.x-(float)x);
    int dy=(int)(m_old.y-(float)y);
    //newRotation.normalize();

    if(dy!=0)
    {
        Vector perp=perpendicular(m_eye-m_target,m_up);
        perp.normalize();
        Quaternion rotateQuaternion((float)dy,perp);
    //	Matrix *rotateMatrix=new Matrix();
        Matrix rotateMatrix;
        rotateQuaternion.getMatrix(rotateMatrix);
        Vector cameraDirection=m_eye-m_target;
        Vector upDirection=cameraDirection+m_up;
        cameraDirection=(rotateMatrix)*(cameraDirection);
        upDirection=(rotateMatrix)*upDirection;
        m_eye=cameraDirection+m_target;
        m_lightPosition[0]=cameraDirection.x;
        m_lightPosition[1]=cameraDirection.y;
        m_lightPosition[2]=cameraDirection.z;
        m_lightPosition[3]=1;
        m_up=upDirection-cameraDirection;
        //delete rotateMatrix;
    }
    if(dx!=0)
    {
        Vector perp(0,0,1);
        //perp.normalize();
        Quaternion rotateQuaternion((float)dx,perp);
        Matrix *rotateMatrix=new Matrix();
        rotateQuaternion.getMatrix(*rotateMatrix);
        Vector cameraDirection= m_eye- m_target;
        Vector upDirection=cameraDirection+ m_up;
        cameraDirection=(*rotateMatrix)*(cameraDirection);
        upDirection=(*rotateMatrix)*upDirection;
         m_eye=cameraDirection+ m_target;
        m_lightPosition[0]=cameraDirection.x;
        m_lightPosition[1]=cameraDirection.y;
        m_lightPosition[2]=cameraDirection.z;
        m_lightPosition[3]=1;
         m_up=upDirection-cameraDirection;
        delete rotateMatrix;
    }
     m_old.x=(float)x;
     m_old.y=(float)y;
}

Vector PerspectiveCamera::getEye() const
{
    return m_eye;
}

Vector PerspectiveCamera::rotateFromP2P(int x1,int y1,int x2,int y2)
{
    if(y1==y2)
        return 0;
    Vector oldRotation(0);
    Vector newRotation(0);
    oldRotation=mapToSphere(x1,y1);
    newRotation=mapToSphere(x2,y2);
    oldRotation.normalize();
    newRotation.normalize();
    Vector perp=perpendicular( m_target- m_eye, m_up);
    if (perp.length2() > Epsilon)
    {
        Quaternion rotateQuaternion(acos(psVECDOT(oldRotation,newRotation)),perp);
        Matrix *rotateMatrix=new Matrix();
        rotateQuaternion.getMatrix(*rotateMatrix);
        Vector cameraDirection= m_eye- m_target;
        Vector upDirection=cameraDirection+ m_up;
        cameraDirection=(*rotateMatrix)*(cameraDirection);
        upDirection=(*rotateMatrix)*upDirection;
         m_eye=cameraDirection+ m_target;
         m_up=upDirection-cameraDirection;
        delete rotateMatrix;
        return rotateQuaternion;
    }
}

void PerspectiveCamera::onRotateRelease()
{
     m_isDraging=false;
     m_old.null();
}

void PerspectiveCamera::onPanRelease(int x,int y)
{
    x;
    y;
    m_isDraging=false;
    m_old.null();
}

void PerspectiveCamera::setCamera()
{
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt( m_eye.x, m_eye.y, m_eye.z, m_target.x, m_target.y, m_target.z, m_up.x, m_up.y, m_up.z);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective( angle, m_width/ m_height, m_nearPlane, m_farPlane);
    glViewport((GLint) m_startX,(GLint) m_startY,(GLint) m_width,(GLint) m_height);
    drawGird();
    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0, GL_AMBIENT, m_lightAmbient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, m_lightDiffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, m_lightSpecular);
    glLightfv(GL_LIGHT0, GL_POSITION, m_lightPosition);
}

void PerspectiveCamera::setCameraForSelectionD(size_t x1,size_t y1,size_t x2,size_t y2,size_t h)
{
    glViewport((GLint)m_startX,(GLint)m_startY,(GLint)m_width,(GLint)m_height);
    GLint viewport[4];
    glGetIntegerv (GL_VIEWPORT, viewport);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    size_t sw=(x2-x1)>5?(x2-x1):5;
    size_t sh=(y2-y1)>5?(y2-y1):5;
    gluPickMatrix((GLdouble) (x1+x2)/2,(GLdouble) (h - (y1+y2)/2), (GLdouble)sw,(GLdouble)sh, viewport);
    gluPerspective(angle,m_width/m_height,m_nearPlane,m_farPlane);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(m_eye.x,m_eye.y,m_eye.z,m_target.x,m_target.y,m_target.z,m_up.x,m_up.y,m_up.z);
}

void PerspectiveCamera::setCameraForSelectionS()
{
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt( m_eye.x, m_eye.y, m_eye.z, m_target.x, m_target.y, m_target.z, m_up.x, m_up.y, m_up.z);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective( angle, m_width/ m_height, m_nearPlane, m_farPlane);
    glViewport((GLint) m_startX,(GLint) m_startY,(GLint) m_width,(GLint) m_height);
}

void PerspectiveCamera::pan(int x,int y)
{
    int dx=x-(int)m_old.x;
    int dy=y-(int)m_old.y;
    m_old.x=(float)x;
    m_old.y=(float)y;
    Vector cameraDirection(0);
    cameraDirection=m_target-m_eye;
    Vector horizontalDirection(0);
    horizontalDirection=perpendicular(cameraDirection,m_up);
    horizontalDirection.normalize();
     m_target+=horizontalDirection*(float)dx*0.5f+ m_up*(float)dy*0.5f;
}

Vector PerspectiveCamera::getHorizontalDir() const
{
    Vector cameraDirection(m_target-m_eye);
    Vector horizontalDirection(perpendicular(cameraDirection,m_up));
    horizontalDirection.normalize();
    return horizontalDirection;
}

Vector PerspectiveCamera::mapToSphere(const int x,const int y) const
{
    float tempX;
    float tempY;
    tempX=((float)x/ m_width-0.5f)*1.414213562f;
    tempY=((float)y/ m_height-0.5f)*1.414213562f;
    Vector result(0);
    result.x = 0;
    result.z = sqrtf(1.0f - tempY*tempY);
    result.y = tempY;
    return result;
}

