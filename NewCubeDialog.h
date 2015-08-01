#pragma once

///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Aug 16 2006)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

// Define WX_GCH in order to support precompiled headers with GCC compiler.
// You have to create the header "wx_pch.h" and include all files needed
// for compile your gui inside it.
// Then, compile it and place the file "wx_pch.h.gch" into the same
// directory that "wx_pch.h".
#ifdef WX_GCH
#include <wx_pch.h>
#else
#include <wx/wx.h>
#endif

#include <wx/button.h>

///////////////////////////////////////////////////////////////////////////

#define ID_DEFAULT wxID_ANY // Default


/**
 * Class NewCubeDialog
 */
class NewCubeDialog : public wxDialog
{
	private:
	
	protected:
		wxStaticText* xStaticText;
		wxTextCtrl* xTextCtrl;
		wxStaticText* yStaticText;
		wxTextCtrl* yTextCtrl;
		wxStaticText* ZStaticText;
		wxTextCtrl* zTextCtrl;
		wxStaticText* lengthStaticText;
		wxTextCtrl* lengthTextCtrl;
		wxStaticText* widthStaticText;
		wxTextCtrl* widthTextCtrl;
		wxStaticText* heightStaticText;
		wxTextCtrl* heightTextCtrl;
		wxStaticText* segmentXStaticText;
		wxTextCtrl* segmentXTextCtrl;
		wxStaticText* segmentYStaticText;
		wxTextCtrl* segmentYTextCtrl;
		wxStaticText* segmentZStaticText;
		wxTextCtrl* segmentZTextCtrl;
		wxButton* OKButton;
		wxButton* cancelButton;
	
	public:
		NewCubeDialog( wxWindow* parent, int id = -1, wxString title = wxT("New Cube"), wxPoint pos = wxDefaultPosition, wxSize size = wxSize( -1,-1 ), int style = wxDEFAULT_DIALOG_STYLE );
		void initializeData()
		{
		(*xTextCtrl)<<0.0f;
		(*yTextCtrl)<<0.0f;
		(*zTextCtrl)<<0.0f;
		(*lengthTextCtrl)<<100.0f;
		(*widthTextCtrl)<<100.0f;
		(*heightTextCtrl)<<100.0f;
		(*segmentXTextCtrl)<<1;
		(*segmentYTextCtrl)<<1;
		(*segmentZTextCtrl)<<1;
	};
		void onOKButton(wxCommandEvent& event);
		void onCancelButton(wxCommandEvent& event);
	DECLARE_EVENT_TABLE()
};

enum
{
	onOKButtonPressCube=234,
	onCancelButtonPressCube
};