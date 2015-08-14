#pragma once
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>


class Gird
{
private:
	GLuint xy;
	GLuint xz;
	GLuint yz;
public:
	Gird(void)
	{
    }

	void initialize()
	{
		xy=glGenLists(1);
		xz=glGenLists(1);
		yz=glGenLists(1);
		glNewList(xy,GL_COMPILE);
		glBegin(GL_LINES);
		glColor3ub(172,43,40);
		glVertex3f(200,0,0);
		glColor3ub(180,180,180);
		glVertex3f(-200,0,0);

		glColor3ub(91,103,147);
		glVertex3f(0,200,0);
		glColor3ub(180,180,180);
		glVertex3f(0,-200,0);
		for(int x=20;x<=200;x+=20)
		{
			glVertex3i(x,200,0);
			glVertex3i(x,-200,0);
			glVertex3i(-x,200,0);
			glVertex3i(-x,-200,0);
		}
		for(int y=20;y<=200;y+=20)
		{
			glVertex3i(200,y,0);
			glVertex3i(-200,y,0);
			glVertex3i(200,-y,0);
			glVertex3i(-200,-y,0);
		}
		glEnd();
		glEndList();

		glNewList(xz,GL_COMPILE);
		glBegin(GL_LINES);
		glColor3ub(172,43,40);
		glVertex3f(200,0,0);
		glColor3ub(180,180,180);
		glVertex3f(-200,0,0);
				glColor3ub(255,255,128);
			glVertex3f(0,0,200);
			glColor3ub(180,180,180);
			glVertex3f(0,0,-200);
		for(int x=20;x<=200;x+=20)
		{
			glVertex3i(x,0,200);
			glVertex3i(x,0,-200);
			glVertex3i(-x,0,200);
			glVertex3i(-x,0,-200);
		}
		for(int z=20;z<=200;z+=20)
		{
			glVertex3i(200,0,z);
			glVertex3i(-200,0,z);
			glVertex3i(200,0,-z);
			glVertex3i(-200,0,-z);
		}
		glEnd();
		glEndList();

		glNewList(yz,GL_COMPILE);
		glBegin(GL_LINES);
		glColor3ub(91,103,147);
		glVertex3f(0,200,0);
		glColor3ub(180,180,180);
		glVertex3f(0,-200,0);
				glColor3ub(255,255,128);
			glVertex3f(0,0,200);
			glColor3ub(180,180,180);
			glVertex3f(0,0,-200);
		for(int y=20;y<=200;y+=20)
		{
			glVertex3i(0,y,200);
			glVertex3i(0,y,-200);
			glVertex3i(0,-y,200);
			glVertex3i(0,-y,-200);
		}
		for(int z=20;z<=200;z+=20)
		{
			glVertex3i(0,200,z);
			glVertex3i(0,-200,z);
			glVertex3i(0,200,-z);
			glVertex3i(0,-200,-z);
		}
		glEnd();
		glEndList();
	}

    void drawXY() const
	{
		glDisable(GL_LIGHTING);
		glDisable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D,0);
		glLineWidth(1);
		glCallList(xy);
		glEnable(GL_LIGHTING);
	}

    void drawXZ() const
	{
		glDisable(GL_LIGHTING);
		glDisable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D,0);
		glLineWidth(1);
		glCallList(xz);
		glEnable(GL_LIGHTING);
	}

    void drawYZ() const
	{
		glDisable(GL_LIGHTING);
		glDisable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D,0);
		glLineWidth(1);
		glCallList(yz);
		glEnable(GL_LIGHTING);
	}
public:
	~Gird(void)
	{
		glDeleteLists(xy,1);
		glDeleteLists(xz,1);
		glDeleteLists(yz,1);
    }
};

extern Gird *theGird;
