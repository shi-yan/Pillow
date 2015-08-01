#include "ConsoleDialog.h"
#include "Global.h"
#include "Vector.h"
#include "PerspectiveCamera.h"
#include <math.h>

extern Console *theConsole;

BEGIN_EVENT_TABLE(ConsoleDialog, wxDialog)
	EVT_BUTTON(ConsoleDialogSubmitButton, ConsoleDialog::onButton)
	EVT_CLOSE(ConsoleDialog::onClose)
END_EVENT_TABLE()

void ConsoleDialog::refresh()
{
	if(IsShown())
	{
		consoleView->Clear();
		*consoleView<<theConsole->outString();
	}
}

ConsoleDialog::ConsoleDialog(wxWindow *parent):wxDialog(parent, wxID_ANY, wxString(_T("Modeless dialog")), wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE|wxMAXIMIZE_BOX|wxMINIMIZE_BOX, "ConsoleDialog")
{
	this->consoleView = new wxTextCtrl( this, wxID_ANY, _T(""),wxDefaultPosition, wxDefaultSize,wxTE_MULTILINE|wxTE_READONLY);
	wxButton *submitButton = new wxButton(this, ConsoleDialogSubmitButton, _T("Press me"));
	wxBoxSizer *sizerCommandLine = new wxBoxSizer(wxHORIZONTAL);
	commandLine=new wxTextCtrl( this, wxID_ANY, _T("Single line."),wxDefaultPosition, wxDefaultSize,wxTE_PROCESS_ENTER);
	wxBoxSizer *sizerTop = new wxBoxSizer(wxVERTICAL);
	sizerTop->Add(consoleView,5, wxEXPAND | wxALL, 5);
	sizerCommandLine->Add(commandLine, 8, wxEXPAND | wxALL, 5);
	sizerCommandLine->Add(submitButton, 1, wxEXPAND | wxALL, 5);
	sizerTop->Add(sizerCommandLine,1, wxEXPAND | wxALL, 5);
    SetSizer(sizerTop);
    sizerTop->SetSizeHints(this);
    sizerTop->Fit(this);
	theConsole->cprintf("fuck your ass hole");
	theConsole->cprintf("%d",1232);
	*consoleView<<"fuck";
	*consoleView<<theConsole->outString();
}

ConsoleDialog::~ConsoleDialog(void)
{
	delete this->consoleView;
	if(theConsole->dialogInstance)
	{
		theConsole->dialogInstance=0;
	}
}

void ConsoleDialog::onButton(wxCommandEvent& WXUNUSED(event))
{
	theConsole->dprintf("%s\n",this->commandLine->GetLineText(0).c_str());
}

void ConsoleDialog::onClose(wxCloseEvent& event)
{
	Hide();
	event;
}