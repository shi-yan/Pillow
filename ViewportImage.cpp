#include "ViewportImage.h"

	
ViewportImage::ViewportImage(const char *thePath,GLuint theImageID,size_t theWidth,size_t theHeight,CameraMode::__Enum theCameraMode,Vector &thePosition)
		:path(thePath),imageID(theImageID),width(theWidth),height(theHeight),cameraMode(theCameraMode),position(thePosition),isShow(true)
{
}

ViewportImage::~ViewportImage(void)
{
//	glDeleteLists(callID,1);
}
