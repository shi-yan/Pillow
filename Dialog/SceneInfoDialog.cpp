#include "SceneInfoDialog.h"

#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif //__BORLANDC__

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif //WX_PRECOMP
#include "scene.h"
///////////////////////////////////////////////////////////////////////////
BEGIN_EVENT_TABLE(SceneInfoDialog, wxDialog)
	EVT_BUTTON(onCloseButtonPressSceneInfo,SceneInfoDialog::onCloseButton)
END_EVENT_TABLE()
SceneInfoDialog::SceneInfoDialog( wxWindow* parent, int id, wxString title, wxPoint pos, wxSize size, int style ) : wxDialog( parent, id, title, pos, size, style )
{
	wxBoxSizer* mainSizer;
	mainSizer = new wxBoxSizer( wxVERTICAL );
	
	wxStaticBoxSizer* sceneInfoStaticSizer;
	sceneInfoStaticSizer = new wxStaticBoxSizer( new wxStaticBox( this, -1, wxT("Scene Info:") ), wxVERTICAL );
	
	wxGridSizer* mainGirdSizer;
	mainGirdSizer = new wxGridSizer( 2, 2, 0, 0 );
	
	objectCountStaticText = new wxStaticText( this, ID_DEFAULT, wxT("object count:"), wxDefaultPosition, wxDefaultSize, 0 );
	mainGirdSizer->Add( objectCountStaticText, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	objectCountTextCtrl = new wxTextCtrl( this, ID_DEFAULT, wxT(""), wxDefaultPosition, wxDefaultSize, wxTE_READONLY );
	mainGirdSizer->Add( objectCountTextCtrl, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_CENTER_HORIZONTAL, 5 );
	
	vertexCountStaticText = new wxStaticText( this, ID_DEFAULT, wxT("vertex count:"), wxDefaultPosition, wxDefaultSize, 0 );
	mainGirdSizer->Add( vertexCountStaticText, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	vertexCountTextCtrl = new wxTextCtrl( this, ID_DEFAULT, wxT(""), wxDefaultPosition, wxDefaultSize, wxTE_READONLY );
	mainGirdSizer->Add( vertexCountTextCtrl, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_CENTER_HORIZONTAL, 5 );
	
	countStaticText = new wxStaticText( this, ID_DEFAULT, wxT("edge count:"), wxDefaultPosition, wxDefaultSize, 0 );
	mainGirdSizer->Add( countStaticText, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	edgeCountTextCtrl = new wxTextCtrl( this, ID_DEFAULT, wxT(""), wxDefaultPosition, wxDefaultSize, wxTE_READONLY );
	mainGirdSizer->Add( edgeCountTextCtrl, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_CENTER_HORIZONTAL, 5 );
	
	faceCountStaticText = new wxStaticText( this, ID_DEFAULT, wxT("face count:"), wxDefaultPosition, wxDefaultSize, 0 );
	mainGirdSizer->Add( faceCountStaticText, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	faceCountTextCtrl = new wxTextCtrl( this, ID_DEFAULT, wxT(""), wxDefaultPosition, wxDefaultSize, wxTE_READONLY );
	mainGirdSizer->Add( faceCountTextCtrl, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_CENTER_HORIZONTAL, 5 );
	
	sceneInfoStaticSizer->Add( mainGirdSizer, 1, wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );
	
	mainSizer->Add( sceneInfoStaticSizer, 1, wxEXPAND|wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );
	
	wxBoxSizer* buttonSizer;
	buttonSizer = new wxBoxSizer( wxHORIZONTAL );
	
	closeButton = new wxButton( this, onCloseButtonPressSceneInfo, wxT("Close"), wxDefaultPosition, wxDefaultSize, 0 );
	buttonSizer->Add( closeButton, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_CENTER_HORIZONTAL, 5 );
	
	mainSizer->Add( buttonSizer, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5 );
	
	this->SetSizer( mainSizer );
	this->Layout();
	Fit();
	initializeData();
}

//SceneInfoDialog::~SceneInfoDialog(void){}

void SceneInfoDialog::initializeData()
{
	size_t objectCount=0;
	size_t vertexCount=0;
	size_t edgeCount=0;
	size_t faceCount=0;
	
	theScene->getSceneInfo(objectCount,vertexCount,edgeCount,faceCount);

	(*objectCountTextCtrl)<<(int)objectCount;
	(*vertexCountTextCtrl)<<(int)vertexCount;
	(*edgeCountTextCtrl)<<(int)edgeCount;
	(*faceCountTextCtrl)<<(int)faceCount;
		

};

void SceneInfoDialog::onCloseButton(wxCommandEvent& event)
{
	Close();
	event;
};
