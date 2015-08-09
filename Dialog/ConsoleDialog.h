#pragma once
#include "wx/wx.h"
#include "wx/wxprec.h"

class ConsoleDialog :public wxDialog
{
private:
	wxTextCtrl *consoleView;
	wxTextCtrl *commandLine;
public:
	ConsoleDialog(wxWindow *parent);
public:

	~ConsoleDialog(void);
	void onButton(wxCommandEvent& event);
	void onClose(wxCloseEvent& event);

	void refresh();
	

	DECLARE_EVENT_TABLE()
};

enum
{
	ConsoleDialogSubmitButton=300
};
