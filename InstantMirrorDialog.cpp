#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif //__BORLANDC__

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif //WX_PRECOMP

#include "InstantMirrorDialog.h"
#include "Scene.h"

///////////////////////////////////////////////////////////////////////////
BEGIN_EVENT_TABLE(InstantMirrorDialog, wxDialog)
	EVT_BUTTON(onOKButtonPressInstantMirror, InstantMirrorDialog::onOKButton)
	EVT_BUTTON(onCancelButtonPressInstantMirror,InstantMirrorDialog::onCancelButton)
END_EVENT_TABLE()

InstantMirrorDialog::InstantMirrorDialog( wxWindow* parent, int id, wxString title, wxPoint pos, wxSize size, int style ) : wxDialog( parent, id, title, pos, size, style )
{
	wxBoxSizer* mainSizer;
	mainSizer = new wxBoxSizer( wxVERTICAL );
	
	wxStaticBoxSizer* instantMirrorStaticSizer;
	instantMirrorStaticSizer = new wxStaticBoxSizer( new wxStaticBox( this, -1, wxT("Instant Mirror:") ), wxVERTICAL );
	
	wxBoxSizer* settingSizer;
	settingSizer = new wxBoxSizer( wxVERTICAL );
	
	wxGridSizer* mainGirdSizer;
	mainGirdSizer = new wxGridSizer( 2, 2, 0, 0 );
	
	xStaticText = new wxStaticText( this, ID_DEFAULT, wxT("x:"), wxDefaultPosition, wxDefaultSize, 0 );
	mainGirdSizer->Add( xStaticText, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	xTextCtrl = new wxTextCtrl( this, ID_DEFAULT, wxT(""), wxDefaultPosition, wxDefaultSize, 0,wxTextValidator(wxFILTER_NUMERIC,NULL) );
	mainGirdSizer->Add( xTextCtrl, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_CENTER_HORIZONTAL, 5 );
	
	yStaticText = new wxStaticText( this, ID_DEFAULT, wxT("y:"), wxDefaultPosition, wxDefaultSize, 0 );
	mainGirdSizer->Add( yStaticText, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	yTextCtrl = new wxTextCtrl( this, ID_DEFAULT, wxT(""), wxDefaultPosition, wxDefaultSize, 0 ,wxTextValidator(wxFILTER_NUMERIC,NULL));
	mainGirdSizer->Add( yTextCtrl, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_CENTER_HORIZONTAL, 5 );
	
	zStaticText = new wxStaticText( this, ID_DEFAULT, wxT("z:"), wxDefaultPosition, wxDefaultSize, 0 );
	mainGirdSizer->Add( zStaticText, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	zTextCtrl = new wxTextCtrl( this, ID_DEFAULT, wxT(""), wxDefaultPosition, wxDefaultSize, 0 ,wxTextValidator(wxFILTER_NUMERIC,NULL));
	mainGirdSizer->Add( zTextCtrl, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_CENTER_HORIZONTAL, 5 );
	
	mirrorPlaneStaticText = new wxStaticText( this, ID_DEFAULT, wxT("mirror plane:"), wxDefaultPosition, wxDefaultSize, 0 );
	mainGirdSizer->Add( mirrorPlaneStaticText, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	wxString mirrorPlaneChoiceChoices[] = { wxT("xy"), wxT("xz"), wxT("yz") };
	int mirrorPlaneChoiceNChoices = sizeof( mirrorPlaneChoiceChoices ) / sizeof( wxString );
	mirrorPlaneChoice = new wxChoice( this, ID_DEFAULT, wxDefaultPosition, wxDefaultSize, mirrorPlaneChoiceNChoices, mirrorPlaneChoiceChoices, 0 );
	mainGirdSizer->Add( mirrorPlaneChoice, 0, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );
	
	settingSizer->Add( mainGirdSizer, 1, wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );
	
	instantMirrorStaticSizer->Add( settingSizer, 1, wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );
	
	mainSizer->Add( instantMirrorStaticSizer, 1, wxEXPAND|wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );
	
	wxBoxSizer* buttonSizer;
	buttonSizer = new wxBoxSizer( wxHORIZONTAL );
	
	okButton = new wxButton( this,onOKButtonPressInstantMirror, wxT("OK"), wxDefaultPosition, wxDefaultSize, 0 );
	buttonSizer->Add( okButton, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_CENTER_HORIZONTAL, 5 );
	
	cancelButton = new wxButton( this,onCancelButtonPressInstantMirror, wxT("Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
	buttonSizer->Add( cancelButton, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_CENTER_HORIZONTAL, 5 );
	
	mainSizer->Add( buttonSizer, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	this->SetSizer( mainSizer );
	this->Layout();
	Fit();
	initializeData();
}


void InstantMirrorDialog::onOKButton(wxCommandEvent& event)
{
	double x;
	double y;
	double z;

	wxString xString=xTextCtrl->GetValue();
	wxString yString=yTextCtrl->GetValue();
	wxString zString=zTextCtrl->GetValue();

	if(xString.ToDouble(&x) && yString.ToDouble(&y) && zString.ToDouble(&z))
	{
			if(mirrorPlaneChoice->GetSelection()==0)
			{
				theScene->onMirror(MirrorMode::MirrorXY,x,y,z);
				Close();
			}else
				if(mirrorPlaneChoice->GetSelection()==1)
			{
				theScene->onMirror(MirrorMode::MirrorXZ,x,y,z);
				Close();
			}else
				if(mirrorPlaneChoice->GetSelection()==2)
			{
				theScene->onMirror(MirrorMode::MirrorYZ,x,y,z);
				Close();
				}else
				{
				wxMessageBox(wxT("Wrong parameters!"),wxT("Wrong Parameters"),wxICON_ERROR | wxOK, this);
		return;		
				}
	}
	else
	{
				wxMessageBox(wxT("Wrong parameters!"),wxT("Wrong Parameters"),wxICON_ERROR | wxOK, this);
		return;
	}
	event;
}

void InstantMirrorDialog::onCancelButton(wxCommandEvent& event)
{
	Close();
	event;
}