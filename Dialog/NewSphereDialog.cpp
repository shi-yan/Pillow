#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif //__BORLANDC__

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif //WX_PRECOMP

#include "NewSphereDialog.h"
#include "Scene.h"
///////////////////////////////////////////////////////////////////////////
BEGIN_EVENT_TABLE(NewSphereDialog, wxDialog)
	EVT_BUTTON(onOKButtonPressSphere, NewSphereDialog::onOKButton)
	EVT_BUTTON(onCancelButtonPressSphere,NewSphereDialog::onCancelButton)
END_EVENT_TABLE()

NewSphereDialog::NewSphereDialog( wxWindow* parent, int id, wxString title, wxPoint pos, wxSize size, int style ) : wxDialog( parent, id, title, pos, size, style )
{
	wxBoxSizer* mainBoxSizer;
	mainBoxSizer = new wxBoxSizer( wxVERTICAL );
	
	wxStaticBoxSizer* mainStaticSizer;
	mainStaticSizer = new wxStaticBoxSizer( new wxStaticBox( this, -1, wxT("New Sphere") ), wxVERTICAL );
	
	wxGridSizer* settingSizer;
	settingSizer = new wxGridSizer( 2, 2, 0, 0 );
	
	xStaticText = new wxStaticText( this, ID_DEFAULT, wxT("x:"), wxDefaultPosition, wxDefaultSize, 0 );
	settingSizer->Add( xStaticText, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	xTextCtrl = new wxTextCtrl( this, ID_DEFAULT, wxT(""), wxDefaultPosition, wxDefaultSize, 0 ,wxTextValidator(wxFILTER_NUMERIC,NULL));
	settingSizer->Add( xTextCtrl, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_CENTER_HORIZONTAL, 5 );
	
	yStaticText = new wxStaticText( this, ID_DEFAULT, wxT("y:"), wxDefaultPosition, wxDefaultSize, 0 );
	settingSizer->Add( yStaticText, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	yTextCtrl = new wxTextCtrl( this, ID_DEFAULT, wxT(""), wxDefaultPosition, wxDefaultSize, 0 ,wxTextValidator(wxFILTER_NUMERIC,NULL));
	settingSizer->Add( yTextCtrl, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_CENTER_HORIZONTAL, 5 );
	
	zStaticText = new wxStaticText( this, ID_DEFAULT, wxT("z:"), wxDefaultPosition, wxDefaultSize, 0 );
	settingSizer->Add( zStaticText, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	zTextCtrl = new wxTextCtrl( this, ID_DEFAULT, wxT(""), wxDefaultPosition, wxDefaultSize, 0 ,wxTextValidator(wxFILTER_NUMERIC,NULL));
	settingSizer->Add( zTextCtrl, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_CENTER_HORIZONTAL, 5 );
	
	radiusStaticText = new wxStaticText( this, ID_DEFAULT, wxT("radius:"), wxDefaultPosition, wxDefaultSize, 0 );
	settingSizer->Add( radiusStaticText, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	radiusTextCtrl = new wxTextCtrl( this, ID_DEFAULT, wxT(""), wxDefaultPosition, wxDefaultSize, 0 ,wxTextValidator(wxFILTER_NUMERIC,NULL));
	settingSizer->Add( radiusTextCtrl, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_CENTER_HORIZONTAL, 5 );
	
	axisStaticText = new wxStaticText( this, ID_DEFAULT, wxT("axis:"), wxDefaultPosition, wxDefaultSize, 0 );
	settingSizer->Add( axisStaticText, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	wxString axisChoiceChoices[] = { wxT("x"), wxT("y"), wxT("z") };
	int axisChoiceNChoices = sizeof( axisChoiceChoices ) / sizeof( wxString );
	axisChoice = new wxChoice( this, ID_DEFAULT, wxDefaultPosition, wxDefaultSize, axisChoiceNChoices, axisChoiceChoices, 0 );
	axisChoice->SetSelection(2);
	settingSizer->Add( axisChoice, 0, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL|wxALIGN_CENTER_HORIZONTAL, 5 );
	
	segmentAStaticText = new wxStaticText( this, ID_DEFAULT, wxT("segment a:"), wxDefaultPosition, wxDefaultSize, 0 );
	settingSizer->Add( segmentAStaticText, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	segmentATextCtrl = new wxTextCtrl( this, ID_DEFAULT, wxT(""), wxDefaultPosition, wxDefaultSize, 0 ,wxTextValidator(wxFILTER_NUMERIC,NULL));
	settingSizer->Add( segmentATextCtrl, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_CENTER_HORIZONTAL, 5 );
	
	segmentRStaticText = new wxStaticText( this, ID_DEFAULT, wxT("segment r:"), wxDefaultPosition, wxDefaultSize, 0 );
	settingSizer->Add( segmentRStaticText, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	segmentRTextCtrl = new wxTextCtrl( this, ID_DEFAULT, wxT(""), wxDefaultPosition, wxDefaultSize, 0 ,wxTextValidator(wxFILTER_NUMERIC,NULL));
	settingSizer->Add( segmentRTextCtrl, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_CENTER_HORIZONTAL, 5 );
	
	mainStaticSizer->Add( settingSizer, 1, wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );
	
	mainBoxSizer->Add( mainStaticSizer, 1, wxEXPAND|wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );
	
	wxBoxSizer* buttonSizer;
	buttonSizer = new wxBoxSizer( wxHORIZONTAL );
	
	OKButton = new wxButton( this,onOKButtonPressSphere, wxT("OK"), wxDefaultPosition, wxDefaultSize, 0 );
	buttonSizer->Add( OKButton, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_CENTER_HORIZONTAL, 5 );
	
	cancelButton = new wxButton( this, onCancelButtonPressSphere, wxT("Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
	buttonSizer->Add( cancelButton, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_CENTER_HORIZONTAL, 5 );
	
	mainBoxSizer->Add( buttonSizer, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5 );
	
	this->SetSizer( mainBoxSizer );
	this->Layout();
	this->Fit();
	initializeData();
}

void NewSphereDialog::onOKButton(wxCommandEvent& event)
{
	double x;
	double y;
	double z;
	double radius;
	unsigned long segmentA;
	unsigned long segmentR;

	wxString xString=xTextCtrl->GetValue();
	wxString yString=yTextCtrl->GetValue();
	wxString zString=zTextCtrl->GetValue();
	wxString radiusString=radiusTextCtrl->GetValue();
	wxString segmentAString=segmentATextCtrl->GetValue();
	wxString segmentRString=segmentRTextCtrl->GetValue();

	if(xString.ToDouble(&x) && yString.ToDouble(&y) && zString.ToDouble(&z) && radiusString.ToDouble(&radius) && segmentAString.ToULong(&segmentA)&&segmentRString.ToULong(&segmentR))
	{
		if(radius>0 && segmentR>1 && segmentA>2)
		{
			if(axisChoice->GetCurrentSelection()==0)
			{
				theScene->newSphere(x,y,z,radius,AxisMode::axisX,segmentA,segmentR);
				Close();
			}
			else if(axisChoice->GetCurrentSelection()==1)
			{
				theScene->newSphere(x,y,z,radius,AxisMode::axisY,segmentA,segmentR);
				Close();
			}
			else if(axisChoice->GetCurrentSelection()==2)
			{
				theScene->newSphere(x,y,z,radius,AxisMode::axisZ,segmentA,segmentR);
				Close();
			}
			else
			{
				wxMessageBox(wxT("Wrong axis mode!"),wxT("Wrong Axis Mode"),wxICON_ERROR | wxOK, this);
				return;
			}
		}
		else
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

void NewSphereDialog::onCancelButton(wxCommandEvent& event)
{
	Close();
	event;
}