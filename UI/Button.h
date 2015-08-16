#pragma once
#ifdef __APPLE__
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#endif

#include "Enum.h"

DECLARE_ENUM(ButtonEventType)
	Nothing=823,
	Wireframe,
	Faced,
	WireframeFaced,
	Smooth,
	Wire,
	SingleView,
	TwoView,
	ThreeView,
	FourView,
	ObjectSelectionMode,
	FaceSelectionMode,
	EdgeSelectionMode,
	VertexSelectionMode,
	Help,
	CreateCube,
	CreateCylinder,
	CreateSphere,
	CreatePlane,
	CameraPerspective,
	CameraFront,
	CameraBack,
	CameraLeft,
	CameraRight,
	CameraTop,
	CameraBottom,
	Screenshot,
	About,
	Open,
	Save,
	SaveAs,
	New,
	DeleteVertex,
	Split,
	Extrude,
	Collapse
END_ENUM()

class Button
{
	ButtonEventType type;
    unsigned int callID;
	bool isIn;
	int x;
	int y;
    unsigned int sx;
    unsigned int sy;
	float texCx1;
	float texCx2;
	float texCy1;
	float texCy2;
	bool isPressed;
public:
    Button(ButtonEventType::__Enum theType,float theTexCx1,float theTexCy1,float theTexCx2,float theTexCy2,unsigned int theSX,unsigned int theSY):type(theType),texCx1(theTexCx1),texCy1(theTexCy1),texCx2(theTexCx2),texCy2(theTexCy2),isIn(false),x(0),y(0),sx(theSX),sy(theSY),isPressed(false)
	{
		callID=glGenLists(1);	
    }

	void updatePosition(int theX,int theY)
	{
		x=theX;
		y=theY;
    }

    void updateSize(unsigned int theSx,unsigned int theSy)
	{
		sx=theSx;
		sy=theSy;
    }

	void initialize()
	{
		glNewList(callID,GL_COMPILE);
		glBegin(GL_QUADS);
		glTexCoord2f(texCx1,texCy1);
		glVertex2i(x,y);
		glTexCoord2f(texCx2,texCy1);
		glVertex2i(x+sx,y);
		glTexCoord2f(texCx2,texCy2);
		glVertex2i(x+sx,y+sy);
		glTexCoord2f(texCx1,texCy2);
		glVertex2i(x,y+sy);
		glEnd();
		glEndList();
    }

	bool onIn(int mx,int my)
	{
		if(mx>x && my>y && mx<x+(int)sx && my<y+(int)sy)
		{
			isIn=true;
			return true;
		}
		else
		{
			return false;
		}
	}

	bool onLeftPress(int mx,int my)
	{
		//if(mx>x && my>y && mx<x+sx && my<y+sy)
	//	{
			isPressed=true;
			return true;
			mx;
			my;
	//	}
		//else
		//	return false;
	}
	
	ButtonEventType onLeftRelease()
	{
		if(isPressed)
		{
			isPressed=false;
			return type;
		}
		else
			return ButtonEventType::Nothing;
	}

	bool onOut(int mx,int my)
	{
		if(mx<x || my<y || mx>x+(int)sx || my>y+(int)sy)
		{
			isIn=false;
			return true;
		}
		else
		{
			return false;
		}
	}

	void onPaint()
	{
		if(isPressed)
		{
			glCallList(callID);
		}
		else
		{
			if(isIn)
			{
				glMatrixMode(GL_MODELVIEW);
				glPushMatrix();
				glTranslatef(-2,-2,0);
				glCallList(callID);
				glPopMatrix();
			}
			else
			{
				glCallList(callID);
			}
		}
    }

public:
	~Button(void)
	{
		glDeleteLists(callID,1);
    }
};
