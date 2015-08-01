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
 * Class NewPlaneDialog
 */
class NewPlaneDialog : public wxDialog
{
	private:
	
	protected:
		wxStaticText* xStaticText;
		wxTextCtrl* xTextCtrl;
		wxStaticText* yStaticText;
		wxTextCtrl* yTextCtrl;
		wxStaticText* zStaticText;
		wxTextCtrl* zTextCtrl;
		wxStaticText* lengthStaticText;
		wxTextCtrl* lengthTextCtrl;
		wxStaticText* widthStaticText;
		wxTextCtrl* widthTextCtrl;
		wxStaticText* segmentLStaticText;
		wxTextCtrl* segmentLTextCtrl;
		wxStaticText* segmentWStaticText;
		wxTextCtrl* segmentWTextCtrl;
		wxStaticText* axisStaticText;
		wxChoice* axisChoice;
		wxButton* OKButton;
		wxButton* cancelButton;
	
		void initializeData()
		{
			(*xTextCtrl)<<0.0f;
			(*yTextCtrl)<<0.0f;
			(*zTextCtrl)<<0.0f;
			(*lengthTextCtrl)<<100.0f;
			(*widthTextCtrl)<<100.0f;
			axisChoice->SetSelection(2);
			(*segmentLTextCtrl)<<1;
			(*segmentWTextCtrl)<<1;			
		};
	public:
		NewPlaneDialog( wxWindow* parent, int id = -1, wxString title = wxT(""), wxPoint pos = wxDefaultPosition, wxSize size = wxDefaultSize, int style = wxDEFAULT_DIALOG_STYLE );
			void onOKButton(wxCommandEvent& event);
		void onCancelButton(wxCommandEvent& event);
	DECLARE_EVENT_TABLE()
};

enum
{
	onOKButtonPressPlane=234,
	onCancelButtonPressPlane
};
