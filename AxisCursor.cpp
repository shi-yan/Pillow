#include "AxisCursor.h"

AxisCursor::~AxisCursor(void)
{
	glDeleteLists(moveX,1);
	glDeleteLists(rotateX,1);
	glDeleteLists(scaleX,1);

	glDeleteLists(moveY,1);
	glDeleteLists(rotateY,1);
	glDeleteLists(scaleY,1);

	glDeleteLists(moveZ,1);
	glDeleteLists(rotateZ,1);
	glDeleteLists(scaleZ,1);
}
