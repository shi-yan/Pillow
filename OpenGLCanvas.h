#pragma once
#include "wx/glcanvas.h"
#include "Screen.h"
extern Screen *theScreen;
class OpenGLCanvas :
	public wxGLCanvas
{
	friend class MyFrame;
	wxWindow *parent;
public:
	OpenGLCanvas(void);
public:
	~OpenGLCanvas(void);
	bool isALT;
	    
public:
    OpenGLCanvas( wxWindow *parent, wxWindowID id = wxID_ANY,
        const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize,
        long style = 0, const wxString& name = _T("OpenGLCanvas") );

    OpenGLCanvas( wxWindow *parent, const OpenGLCanvas *other,
        wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize, long style = 0,
        const wxString& name = _T("OpenGLCanvas") );

    void onPaint(wxPaintEvent& event);
    void onSize(wxSizeEvent& event);
    void onEraseBackground(wxEraseEvent& event);
    void onKeyDown(wxKeyEvent& event);
    void onKeyUp(wxKeyEvent& event);
    void onEnterWindow(wxMouseEvent& event);
	void onMouse( wxMouseEvent& event );
	void initScreen();

    void Render();
    void InitGL();

	
DECLARE_EVENT_TABLE()

};




