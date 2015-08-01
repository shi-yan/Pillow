#pragma once
#ifdef WX_GCH
#include <wx_pch.h>
#else
#include <wx/wx.h>
#endif

#include <wx/button.h>

///////////////////////////////////////////////////////////////////////////

#define ID_DEFAULT wxID_ANY // Default

/**
 * Class ExtrudeDialog
 */
class ExtrudeDialog : public wxDialog
{
	private:
	
	protected:
		wxStaticText* amountStaticText;
		wxTextCtrl* amountTextCtrl;
		wxStaticText* xStaticText;
		wxTextCtrl* xTextCtrl;
		wxStaticText* yStaticText;
		wxTextCtrl* yTextCtrl;
		wxStaticText* zStaticText;
		wxTextCtrl* zTextCtrl;
		wxButton* OKButton;
		wxButton* cancelButton;
	
	public:
		ExtrudeDialog( wxWindow* parent, int id = -1, wxString title = wxT(""), wxPoint pos = wxDefaultPosition, wxSize size = wxDefaultSize, int style = wxDEFAULT_DIALOG_STYLE );
			void initializeData()
		{
		(*xTextCtrl)<<1.0f;
		(*yTextCtrl)<<1.0f;
		(*zTextCtrl)<<1.0f;
		(*amountTextCtrl)<<10.0f;
	};
		void onOKButton(wxCommandEvent& event);
		void onCancelButton(wxCommandEvent& event);
		DECLARE_EVENT_TABLE()
};

enum
{
	onOKButtonPressExtrude=234,
	onCancelButtonPressExtrude
};