#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include "OpenGLCanvas.h"
#include "wx/wxprec.h"
#include "wx/wx.h"
#include <sys/timeb.h>
#include "Global.h"

BEGIN_EVENT_TABLE(OpenGLCanvas, wxGLCanvas)
    EVT_SIZE(OpenGLCanvas::onSize)
    EVT_PAINT(OpenGLCanvas::onPaint)
    EVT_ERASE_BACKGROUND(OpenGLCanvas::onEraseBackground)
    EVT_ENTER_WINDOW(OpenGLCanvas::onEnterWindow)
	EVT_MOUSE_EVENTS(OpenGLCanvas::onMouse)
	EVT_KEY_DOWN(OpenGLCanvas::onKeyDown)
	EVT_KEY_UP(OpenGLCanvas::onKeyUp)
END_EVENT_TABLE()

OpenGLCanvas::OpenGLCanvas(wxWindow *parent, wxWindowID id,const wxPoint& pos, const wxSize& size, long style, const wxString& name)
:parent(parent),wxGLCanvas(parent, (wxGLCanvas*) NULL, id, pos, size, style|wxFULL_REPAINT_ON_RESIZE , name )
{
    InitGL();
}

void OpenGLCanvas::initScreen()
{
	theScreen=new Screen(parent);
	theScreen->initialize();
	int w, h;
	GetClientSize(&w, &h);
	{
		SetCurrent();
		theScreen->width=w;
		theScreen->height=h;
		theScreen->updateScreen(w,h);
    }
	Refresh(false);
}

OpenGLCanvas::OpenGLCanvas(wxWindow *parent, const OpenGLCanvas *other,wxWindowID id, const wxPoint& pos, const wxSize& size, long style,const wxString& name)
:wxGLCanvas(parent, other->GetContext(), id, pos, size, style|wxFULL_REPAINT_ON_RESIZE , name),isALT(false)
{
     InitGL();
}

OpenGLCanvas::~OpenGLCanvas()
{
}

void OpenGLCanvas::Render()
{
	wxPaintDC dc(this);
    SetCurrent();
	theScreen->onPaint();
    glFlush();
    SwapBuffers();
}

void OpenGLCanvas::onEnterWindow( wxMouseEvent& WXUNUSED(event) )
{
    SetFocus();
}

void OpenGLCanvas::onKeyDown(wxKeyEvent& event)
{

	if(event.GetKeyCode()==WXK_ALT)
	{
		theScreen->onAltDown();
	}
	else if(event.GetKeyCode()==WXK_CONTROL)
	{
		theScreen->onCtrlDown();
	}
	else if(event.GetKeyCode()==90)//z
	{
		theScreen->onExtrudePress();
	}
}

void OpenGLCanvas::onKeyUp(wxKeyEvent& event)
{
	int keyCode=event.GetKeyCode();
	if(keyCode==WXK_ALT)
	{
		theScreen->onAltUp();
	}
	else if(keyCode==WXK_CONTROL)
	{
		theScreen->onCtrlUp();
	}
	else if(keyCode==87) //w
	{
		theScene->changeAxisCursorMode(AxisCursorMode::MoveAxis);
	}
	else if(keyCode==69) //e
	{
		theScene->changeAxisCursorMode(AxisCursorMode::RotateAxis);
	}
	else if(keyCode==82) //r
	{
		theScene->changeAxisCursorMode(AxisCursorMode::ScaleAxis);
	}
	else if(keyCode==67) //c
	{
		theScene->subdivide();
	}
	else if(keyCode==86) //v
	{
		theScene->unSubdivide();
	}
	else if(keyCode==90) //z
	{
		theScreen->onExtrudeRelease();
	}
	else if(keyCode==127)
	{
		theScene->onDelete();
	}
	else if(keyCode==49)
	{
		theScreen->changeCamera(CameraMode::Top);
	}
	else if(keyCode==50)
	{
		theScreen->changeCamera(CameraMode::Bottom);
	}
	else if(keyCode==51)
	{
		theScreen->changeCamera(CameraMode::Front);
	}
	else if(keyCode==52)
	{
		theScreen->changeCamera(CameraMode::Back);
	}
	else if(keyCode==53)
	{
		theScreen->changeCamera(CameraMode::Left);
	}
	else if(keyCode==54)
	{
		theScreen->changeCamera(CameraMode::Right);
	}
	else if(keyCode==55)
	{
		theScreen->changeCamera(CameraMode::Perspective);
	}
	Refresh(false);
}


void OpenGLCanvas::onPaint( wxPaintEvent& WXUNUSED(event) )
{
    Render();
}

void OpenGLCanvas::onSize(wxSizeEvent& event)
{
    // this is also necessary to update the context on some platforms
    wxGLCanvas::OnSize(event);
    // set GL viewport (not called by wxGLCanvas::OnSize on all platforms...)
    int w, h;
	GetClientSize(&w, &h);
	{
		SetCurrent();
		theScreen->width=w;
		theScreen->height=h;
		theScreen->updateScreen(w,h);
    }
	Refresh(false);
}

void OpenGLCanvas::onEraseBackground(wxEraseEvent& WXUNUSED(event))
{
}

void OpenGLCanvas::onMouse( wxMouseEvent& event )
{
	long x=0;
	long y=0;
	int wheel=0;
	event.GetPosition(&x,&y);
	if(event.Dragging())
	{
		if(event.m_leftDown)
		{
			if(theScreen->onLeftDrag(x,y))		
			{
				Refresh(false);
			}
		}
		else 
		if(event.m_middleDown)
		{
			if(theScreen->onMiddleDrag(x,y))		
			{
				Refresh(false);
			}
		}
	}
	else 
	if(event.ButtonDown(wxMOUSE_BTN_LEFT))
	{
		if(theScreen->onLeftPress(x,y))		
		{
			Refresh(false);
		}
	}
	else
	if(event.ButtonDown(wxMOUSE_BTN_MIDDLE))
	{
		if(theScreen->onMiddlePress(x,y))		
		{
			Refresh(false);
		}
	}
	else 
	if(event.ButtonUp(wxMOUSE_BTN_MIDDLE))
	{
		if(theScreen->onMiddleRelease(event.m_shiftDown))		
		{
			Refresh(false);
		}
	}
	else
	if(event.ButtonUp(wxMOUSE_BTN_LEFT))
	{
		if(theScreen->onLeftRelease(event.m_shiftDown))		
		{
			Refresh(false);
		}
	}
	else if(event.ButtonUp(wxMOUSE_BTN_RIGHT))
	{
		if(theScene->splitVertexID==0)
		{
			theScene->endSplit();
		}
		else
		{
			theScene->changeSelectionMode(SelectionMode::Edge);
		}
		Refresh(false);
	}
	else 
	if((wheel=event.GetWheelRotation())!=0)
	{
		if(theScreen->onWheel(wheel,x,y))		
		{
			Refresh(false);
		}
	}
	else if(event.Moving())
	{
		if(theScreen->onMoving(x,y))
		{
			Refresh(false);
		}
	}
}

void OpenGLCanvas::InitGL()
{
    SetCurrent();
    int w, h;
    GetClientSize(&w, &h);
    {
		SetCurrent();
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(25,(float)w/(float)h,1,8000);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		gluLookAt(100,100,100,0,0,0,0,0,1);
		glViewport(0, 0, (GLint) w, (GLint) h);
    }
	glLineWidth(3);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
}