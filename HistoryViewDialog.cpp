#include "HistoryViewDialog.h"

HistoryViewDialog::HistoryViewDialog( wxWindow* parent, int id, wxString title, wxPoint pos, wxSize size, int style ) : wxDialog( parent, id, title, pos, size, style )
{
	wxBoxSizer* mainBoxSizer;
	mainBoxSizer = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* historyBoxSizer;
	historyBoxSizer = new wxBoxSizer( wxHORIZONTAL );
	
	wxStaticBoxSizer* undoStaticBoxSizer;
	undoStaticBoxSizer = new wxStaticBoxSizer( new wxStaticBox( this, -1, wxT("Undo List:") ), wxVERTICAL );
	
	undoListBox = new wxListBox( this, ID_DEFAULT, wxDefaultPosition, wxDefaultSize, 0, NULL, 0 ); 
	undoStaticBoxSizer->Add( undoListBox, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_CENTER_HORIZONTAL, 5 );
	
	historyBoxSizer->Add( undoStaticBoxSizer, 1, wxEXPAND|wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_CENTER_HORIZONTAL, 5 );
	
	wxStaticBoxSizer* redoStaticBoxSizer;
	redoStaticBoxSizer = new wxStaticBoxSizer( new wxStaticBox( this, -1, wxT("Redo List:") ), wxVERTICAL );
	
	redoListBox = new wxListBox( this, ID_DEFAULT, wxDefaultPosition, wxDefaultSize, 0, NULL, 0 ); 
	redoStaticBoxSizer->Add( redoListBox, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_CENTER_HORIZONTAL, 5 );
	
	historyBoxSizer->Add( redoStaticBoxSizer, 1, wxEXPAND|wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_CENTER_HORIZONTAL, 5 );
	
	mainBoxSizer->Add( historyBoxSizer, 1, wxEXPAND|wxALIGN_CENTER_VERTICAL|wxALIGN_CENTER_HORIZONTAL, 5 );
	
	wxBoxSizer* buttonSizer;
	buttonSizer = new wxBoxSizer( wxHORIZONTAL );
	
	undoButton = new wxButton( this, ID_DEFAULT, wxT("Undo"), wxDefaultPosition, wxDefaultSize, 0 );
	buttonSizer->Add( undoButton, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_CENTER_HORIZONTAL, 5 );
	
	redoButton = new wxButton( this, ID_DEFAULT, wxT("Redo"), wxDefaultPosition, wxDefaultSize, 0 );
	buttonSizer->Add( redoButton, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_CENTER_HORIZONTAL, 5 );
	
	closeButton = new wxButton( this, ID_DEFAULT, wxT("Close"), wxDefaultPosition, wxDefaultSize, 0 );
	buttonSizer->Add( closeButton, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_CENTER_HORIZONTAL, 5 );
	
	mainBoxSizer->Add( buttonSizer, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5 );
	
	this->SetSizer( mainBoxSizer );
	this->Layout();
}

HistoryViewDialog::~HistoryViewDialog(void)
{
			delete undoListBox;
		delete redoListBox;
		delete undoButton;
		delete redoButton;
		delete closeButton;
}
