#include "Camera.h"

Camera::Camera(CameraMode::__Enum type,float width,float height,float nearPlane,float farPlane,float startX,float startY,Vector target,Vector eye,Vector up):
type(type),showGird(true),width(width),height(height),nearPlane(nearPlane),farPlane(farPlane),startX(startX),startY(startY),target(target),eye(eye),up(up),old(0),isDraging(false)//,isShowViewportImage(false)//,referenceImage(NULL)
{
}

Camera::~Camera(void)
{
//	delete referenceImage;
}
