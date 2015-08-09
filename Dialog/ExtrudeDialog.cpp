

#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif //__BORLANDC__

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif //WX_PRECOMP

#include "ExtrudeDialog.h"
#include "Scene.h"

BEGIN_EVENT_TABLE(ExtrudeDialog, wxDialog)
	EVT_BUTTON(onOKButtonPressExtrude, ExtrudeDialog::onOKButton)
	EVT_BUTTON(onCancelButtonPressExtrude,ExtrudeDialog::onCancelButton)
END_EVENT_TABLE()

ExtrudeDialog::ExtrudeDialog( wxWindow* parent, int id, wxString title, wxPoint pos, wxSize size, int style ) : wxDialog( parent, id, title, pos, size, style )
{
	wxBoxSizer* mainSizer;
	mainSizer = new wxBoxSizer( wxVERTICAL );
	
	wxStaticBoxSizer* mainStaticSizer;
	mainStaticSizer = new wxStaticBoxSizer( new wxStaticBox( this, -1, wxT("Extrude:") ), wxVERTICAL );
	
	wxGridSizer* settingGirdSizer;
	settingGirdSizer = new wxGridSizer( 2, 2, 0, 0 );
	
	amountStaticText = new wxStaticText( this, ID_DEFAULT, wxT("amount:"), wxDefaultPosition, wxDefaultSize, 0 );
	settingGirdSizer->Add( amountStaticText, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	amountTextCtrl = new wxTextCtrl( this, ID_DEFAULT, wxT(""), wxDefaultPosition, wxDefaultSize, 0 ,wxTextValidator(wxFILTER_NUMERIC,NULL));
	settingGirdSizer->Add( amountTextCtrl, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_CENTER_HORIZONTAL, 5 );
	
	xStaticText = new wxStaticText( this, ID_DEFAULT, wxT("x:"), wxDefaultPosition, wxDefaultSize, 0 );
	settingGirdSizer->Add( xStaticText, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	xTextCtrl = new wxTextCtrl( this, ID_DEFAULT, wxT(""), wxDefaultPosition, wxDefaultSize, 0 ,wxTextValidator(wxFILTER_NUMERIC,NULL));
	settingGirdSizer->Add( xTextCtrl, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_CENTER_HORIZONTAL, 5 );
	
	yStaticText = new wxStaticText( this, ID_DEFAULT, wxT("y:"), wxDefaultPosition, wxDefaultSize, 0 );
	settingGirdSizer->Add( yStaticText, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	yTextCtrl = new wxTextCtrl( this, ID_DEFAULT, wxT(""), wxDefaultPosition, wxDefaultSize, 0 ,wxTextValidator(wxFILTER_NUMERIC,NULL));
	settingGirdSizer->Add( yTextCtrl, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_CENTER_HORIZONTAL, 5 );
	
	zStaticText = new wxStaticText( this, ID_DEFAULT, wxT("z:"), wxDefaultPosition, wxDefaultSize, 0 );
	settingGirdSizer->Add( zStaticText, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	zTextCtrl = new wxTextCtrl( this, ID_DEFAULT, wxT(""), wxDefaultPosition, wxDefaultSize, 0 ,wxTextValidator(wxFILTER_NUMERIC,NULL));
	settingGirdSizer->Add( zTextCtrl, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_CENTER_HORIZONTAL, 5 );
	
	mainStaticSizer->Add( settingGirdSizer, 1, wxEXPAND, 5 );
	
	mainSizer->Add( mainStaticSizer, 0, wxEXPAND|wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_CENTER_HORIZONTAL, 5 );
	
	wxBoxSizer* buttonSizer;
	buttonSizer = new wxBoxSizer( wxHORIZONTAL );
	
	OKButton = new wxButton( this, onOKButtonPressExtrude, wxT("OK"), wxDefaultPosition, wxDefaultSize, 0 );
	buttonSizer->Add( OKButton, 0, wxALL, 5 );
	
	cancelButton = new wxButton( this, onCancelButtonPressExtrude, wxT("Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
	buttonSizer->Add( cancelButton, 0, wxALL, 5 );
	
	mainSizer->Add( buttonSizer, 0, wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT|wxALL, 5 );
	
	this->SetSizer( mainSizer );
	this->Layout();
	Fit();
	initializeData();
}

void ExtrudeDialog::onOKButton(wxCommandEvent& event)
{
	double x;
	double y;
	double z;
	double amount;
	wxString xString=xTextCtrl->GetValue();
	wxString yString=yTextCtrl->GetValue();
	wxString zString=zTextCtrl->GetValue();
	wxString amountString=amountTextCtrl->GetValue();

	if(xString.ToDouble(&x) && yString.ToDouble(&y) && zString.ToDouble(&z) && amountString.ToDouble(&amount))
	{
		Vector extrude(x,y,z);
		extrude.normalize();
		extrude*=amount;
		theScene->extrudeFaceGroup(extrude.x,extrude.y,extrude.z);
		Close();
	}
	else
	{
				wxMessageBox(wxT("Wrong parameters!"),wxT("Wrong Parameters"),wxICON_ERROR | wxOK, this);
		return;
	}
	event;
}

void ExtrudeDialog::onCancelButton(wxCommandEvent& event)
{
	Close();
	event;
}