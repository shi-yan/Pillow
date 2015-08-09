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
 * Class HistoryViewDialog
 */
class HistoryViewDialog : public wxDialog
{
	private:
	
	protected:
		wxListBox* undoListBox;
		wxListBox* redoListBox;
		wxButton* undoButton;
		wxButton* redoButton;
		wxButton* closeButton;
	
	public:
		HistoryViewDialog( wxWindow* parent, int id = -1, wxString title = wxT(""), wxPoint pos = wxDefaultPosition, wxSize size = wxDefaultSize, int style = wxDEFAULT_DIALOG_STYLE );
	~HistoryViewDialog(void);
};
