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
 * Class SceneInfoDialog
 */
class SceneInfoDialog : public wxDialog
{
	private:
	
	protected:
		wxStaticText* objectCountStaticText;
		wxTextCtrl* objectCountTextCtrl;
		wxStaticText* vertexCountStaticText;
		wxTextCtrl* vertexCountTextCtrl;
		wxStaticText* countStaticText;
		wxTextCtrl* edgeCountTextCtrl;
		wxStaticText* faceCountStaticText;
		wxTextCtrl* faceCountTextCtrl;
		wxButton* closeButton;
	
	public:
		void initializeData();
		SceneInfoDialog( wxWindow* parent, int id = -1, wxString title = wxT(""), wxPoint pos = wxDefaultPosition, wxSize size = wxDefaultSize, int style = wxDEFAULT_DIALOG_STYLE );
		void onCloseButton(wxCommandEvent& event);
	DECLARE_EVENT_TABLE()
};

enum
{
	onCloseButtonPressSceneInfo=234
};

