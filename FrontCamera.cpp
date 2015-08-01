#include "FrontCamera.h"

ViewportImage *frontReference=NULL;

FrontCamera::FrontCamera(float width,float height,float nearPlane,float farPlane,float startX,float startY,Vector target,Vector eye,Vector up):
Camera(CameraMode::Front,width,height,nearPlane,farPlane,startX,startY,target,eye,up)
{
	light_ambient[0]=0.4f;
	light_ambient[1]=0.4f;
	light_ambient[2]=0.4f;
	light_ambient[3]=1;

	light_diffuse[0]=0.6f;
	light_diffuse[1]=0.6f;
	light_diffuse[2]=0.6f;
	light_diffuse[3]=1;

	light_specular[0]=0.7f;
	light_specular[1]=0.7f;
	light_specular[2]=0.7f;
	light_specular[3]=1;

	light_position[0]=0;
	light_position[1]=500;
	light_position[2]=0;
	light_position[3]=1;

//	referenceImage=frontReference;
}

FrontCamera::~FrontCamera(void)
{
}
