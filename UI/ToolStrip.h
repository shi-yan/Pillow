#pragma once
#include "Button.h"
#include <vector>

class ToolStrip
{
public:
    unsigned int callID;
	Button *currentIn;
	int x;
	int y;
    unsigned int spacer;
    unsigned int hPadding;
    unsigned int vPadding;
	float textLeftX1;
	float textLeftY1;
	float textLeftX2;
	float textLeftY2;
	float textBGX1;
	float textBGY1;
	float textBGX2;
	float textBGY2;
	float textRightX1;
	float textRightY1;
	float textRightX2;
	float textRightY2;
    unsigned int width;
    unsigned int height;
    unsigned int bSX;
    unsigned int bSY;
	bool isH;
	std::vector<Button*> buttonSet;
	GLuint textureID;
public:
    ToolStrip(int theX,int theY,unsigned int theWidth,unsigned int theHeight,bool theIsH,unsigned int buttonNum,float theTextLeftX1,float theTextLeftY1,float theTextLeftX2,float theTextLeftY2,float theTextBGX1,float theTextBGY1,float theTextBGX2,float theTextBGY2,float theTextRightX1,float theTextRightY1,float theTextRightX2,float theTextRightY2)
		:currentIn(NULL),x(theX),y(theY),width(theWidth),height(theHeight),isH(theIsH),spacer(10),hPadding(5),vPadding(5),bSY(32),bSX(32),textLeftX1(theTextLeftX1),textLeftY1(theTextLeftY1),textLeftX2(theTextLeftX2),textLeftY2(theTextLeftY2),textBGX1(theTextBGX1),textBGY1(theTextBGY1),textBGX2(theTextBGX2),textBGY2(theTextBGY2),textRightX1(theTextRightX1),textRightY1(theTextRightY1),textRightX2(theTextRightX2),textRightY2(theTextRightY2)
	{
		callID=glGenLists(1);
		buttonSet.reserve(buttonNum);
	};
	void add(Button *theButton)
	{
		buttonSet.push_back(theButton);
		theButton->updateSize(bSX,bSY);
	};
	void updateButtonPosition()
	{
		if(isH)
		{
			int tempX=x+hPadding;
			int tempY=y+vPadding;
            for(unsigned int i=0;i<buttonSet.size();++i)
			{
				buttonSet[i]->updatePosition(tempX,tempY);
				tempX+=spacer+bSX;
			}
			width=buttonSet.size()*bSX+hPadding+hPadding+spacer*(buttonSet.size()-1);
			height=42;
		}
		else
		{
			int tempX=x+hPadding;
			int tempY=y+vPadding;
            for(unsigned int i=0;i<buttonSet.size();++i)
			{
				buttonSet[i]->updatePosition(tempX,tempY);
				tempY+=spacer+bSY;
			}
			width=42;
			height=buttonSet.size()*bSY+vPadding+vPadding+spacer*(buttonSet.size()-1);
		}
	};


	ButtonEventType onLeftRelease()
	{
		if(currentIn)
		{
			return currentIn->onLeftRelease();
		}
		else
		{
			return ButtonEventType::Nothing;
		}
	};

	bool onLeftPress(int mx,int my)
	{
		if(currentIn)
		{
			if(currentIn->onLeftPress(mx,my))
			{
				return true;
			}
			else
			{
				return false;
			}
		}
		else
		{
			return false;
		}
	};

	bool onIn(int mx,int my)
	{
		if(mx>x && my>y && my<y+(int)height && mx<x+(int)width)
		{
            for(unsigned int i=0;i<buttonSet.size();++i)
			{
				if(buttonSet[i]->onIn(mx,my))
				{
					currentIn=buttonSet[i];
				}
			}
			return true;	
		}
		else
			return false;
	}

	bool onOut(int mx,int my)
	{
		if(currentIn)
		{
			if(currentIn->onOut(mx,my))
			{
				currentIn=NULL;
			}
			else
			{
				return false;
			}
		}

		if(mx<x || mx<y || mx>x+(int)width || my>y+(int)height)
		{
			return true;
		}else
		{
			return false;
		}
	}

	void updatePosition(int theX,int theY)
	{
		x=theX;
		y=theY;
		updateButtonPosition();
		initializeButton();
		initialize();
	};
	void initializeButton()
	{
        for(unsigned int i=0;i<buttonSet.size();++i)
		{
			buttonSet[i]->initialize();
		}
	};
	void initialize()
	{
		if(isH)
		{
			glNewList(callID,GL_COMPILE);
			glBegin(GL_QUADS);
			glTexCoord2f(textLeftX1,textLeftY1);
			glVertex2i(x,y);
			glTexCoord2f(textLeftX2,textLeftY1);
			glVertex2i(x+10,y);
			glTexCoord2f(textLeftX2,textLeftY2);
			glVertex2i(x+10,y+height);
			glTexCoord2f(textLeftX1,textLeftY2);
			glVertex2i(x,y+height);

			glTexCoord2f(textBGX1,textBGY1);
			glVertex2i(x+10,y);
			glTexCoord2f(textBGX2,textBGY1);
			glVertex2i(x+width-10,y);
			glTexCoord2f(textBGX2,textBGY2);
			glVertex2i(x+width-10,y+height);
			glTexCoord2f(textBGX1,textBGY2);
			glVertex2i(x+10,y+height);

			glTexCoord2f(textRightX1,textRightY1);
			glVertex2i(x+width-10,y);
			glTexCoord2f(textRightX2,textRightY1);
			glVertex2i(x+width,y);
			glTexCoord2f(textRightX2,textRightY2);
			glVertex2i(x+width,y+height);
			glTexCoord2f(textRightX1,textRightY2);
			glVertex2i(x+width-10,y+height);
			glEnd();
			glEndList();
		}
		else
		{
		glNewList(callID,GL_COMPILE);
			glBegin(GL_QUADS);
			glTexCoord2f(textLeftX1,textLeftY1);
			glVertex2i(x,y);
			glTexCoord2f(textLeftX2,textLeftY1);
			glVertex2i(x+width,y);
			glTexCoord2f(textLeftX2,textLeftY2);
			glVertex2i(x+width,y+10);
			glTexCoord2f(textLeftX1,textLeftY2);
			glVertex2i(x,y+10);

			glTexCoord2f(textBGX1,textBGY1);
			glVertex2i(x,y+10);
			glTexCoord2f(textBGX2,textBGY1);
			glVertex2i(x+width,y+10);
			glTexCoord2f(textBGX2,textBGY2);
			glVertex2i(x+width,y-10+height);
			glTexCoord2f(textBGX1,textBGY2);
			glVertex2i(x,y-10+height);

			glTexCoord2f(textRightX1,textRightY1);
			glVertex2i(x,y+height-10);
			glTexCoord2f(textRightX2,textRightY1);
			glVertex2i(x+width,y+height-10);
			glTexCoord2f(textRightX2,textRightY2);
			glVertex2i(x+width,y+height);
			glTexCoord2f(textRightX1,textRightY2);
			glVertex2i(x,y+height);
			glEnd();
			glEndList();
		}
	};
	void onPaint()
	{
		glCallList(callID);
        for(unsigned int i=0;i<buttonSet.size();++i)
		{
			buttonSet[i]->onPaint();
		}
		
	};
public:
	~ToolStrip(void)
	{
        for(unsigned int i=0;i<buttonSet.size();++i)
		{
			delete buttonSet[i];
		}
		buttonSet.clear();
	};
};
