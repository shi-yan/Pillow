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
 * Class AboutDialog
 */
class AboutDialog : public wxDialog
{
	private:
	
	protected:
		wxStaticBitmap* logoBitmap;
		wxStaticText* infoStaticText;
		wxButton* linkButton;
		wxButton* closeButton;
	
	public:
		AboutDialog( wxWindow* parent, int id = -1, wxString title = wxT(""), wxPoint pos = wxDefaultPosition, wxSize size = wxDefaultSize, int style = wxDEFAULT_DIALOG_STYLE );
		void onLinkButton(wxCommandEvent& event);
		void onClose(wxCommandEvent& event);
	DECLARE_EVENT_TABLE()
};

enum
{
	linkButtonPress=2423,
	closeButtonPress
};