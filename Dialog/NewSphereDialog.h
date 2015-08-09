#pragma once

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
#include <wx/choice.h>
#include <wx/slider.h>
#include <wx/statbmp.h>
#include <wx/statline.h>

///////////////////////////////////////////////////////////////////////////

#define ID_DEFAULT wxID_ANY // Default

/**
 * Class NewSphereDialog
 */
class NewSphereDialog : public wxDialog
{
	private:
	
	protected:
		wxStaticText* xStaticText;
		wxTextCtrl* xTextCtrl;
		wxStaticText* yStaticText;
		wxTextCtrl* yTextCtrl;
		wxStaticText* zStaticText;
		wxTextCtrl* zTextCtrl;
		wxStaticText* radiusStaticText;
		wxTextCtrl* radiusTextCtrl;
		wxStaticText* axisStaticText;
		wxChoice* axisChoice;
		wxStaticText* segmentAStaticText;
		wxTextCtrl* segmentATextCtrl;
		wxStaticText* segmentRStaticText;
		wxTextCtrl* segmentRTextCtrl;
		wxButton* OKButton;
		wxButton* cancelButton;
	
	public:

		void initializeData()
		{
			(*xTextCtrl)<<0.0f;
			(*yTextCtrl)<<0.0f;
			(*zTextCtrl)<<0.0f;
			(*radiusTextCtrl)<<100.0f;
			axisChoice->SetSelection(2);
			(*segmentATextCtrl)<<8;
			(*segmentRTextCtrl)<<4;			
		};

		NewSphereDialog( wxWindow* parent, int id = -1, wxString title = wxT(""), wxPoint pos = wxDefaultPosition, wxSize size = wxDefaultSize, int style = wxDEFAULT_DIALOG_STYLE );
	void onOKButton(wxCommandEvent& event);
		void onCancelButton(wxCommandEvent& event);
	DECLARE_EVENT_TABLE()
};

enum
{
	onOKButtonPressSphere=234,
	onCancelButtonPressSphere
};

