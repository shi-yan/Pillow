#pragma once

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
 * Class InstantMirrorDialog
 */
class InstantMirrorDialog : public wxDialog
{
	private:
	
	protected:
		wxStaticText* xStaticText;
		wxTextCtrl* xTextCtrl;
		wxStaticText* yStaticText;
		wxTextCtrl* yTextCtrl;
		wxStaticText* zStaticText;
		wxTextCtrl* zTextCtrl;
		wxStaticText* mirrorPlaneStaticText;
		wxChoice* mirrorPlaneChoice;
		wxButton* okButton;
		wxButton* cancelButton;
	
	public:
		void initializeData()
		{
			(*xTextCtrl)<<0.0f;
			(*yTextCtrl)<<0.0f;
			(*zTextCtrl)<<0.0f;
			mirrorPlaneChoice->SetSelection(2);
		}
		InstantMirrorDialog( wxWindow* parent, int id = -1, wxString title = wxT(""), wxPoint pos = wxDefaultPosition, wxSize size = wxDefaultSize, int style = wxDEFAULT_DIALOG_STYLE );
				void onOKButton(wxCommandEvent& event);
		void onCancelButton(wxCommandEvent& event);
		DECLARE_EVENT_TABLE()
};

enum
{
	onOKButtonPressInstantMirror=234,
	onCancelButtonPressInstantMirror
};