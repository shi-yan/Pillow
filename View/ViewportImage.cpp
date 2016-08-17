#include "ViewportImage.h"


ViewportImage::ViewportImage(const char *path, GLuint imageID, unsigned int width, unsigned int height, CameraMode::__Enum cameraMode, Vector &position)
        :m_path(path),
          m_imageID(imageID),
          m_width(width),
          m_height(height),
          m_cameraMode(cameraMode),
          m_position(position),
          m_isShow(true)
{
}

ViewportImage::~ViewportImage(void)
{
//    glDeleteLists(callID,1);
}
