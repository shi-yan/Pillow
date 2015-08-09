#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif //__BORLANDC__

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif //WX_PRECOMP

#include "NewCylinderDialog.h"
#include "scene.h"

///////////////////////////////////////////////////////////////////////////

BEGIN_EVENT_TABLE(NewCylinderDialog, wxDialog)
	EVT_BUTTON(onOKButtonPressCylinder, NewCylinderDialog::onOKButton)
	EVT_BUTTON(onCancelButtonPressCylinder,NewCylinderDialog::onCancelButton)
END_EVENT_TABLE()

NewCylinderDialog::NewCylinderDialog( wxWindow* parent, int id, wxString title, wxPoint pos, wxSize size, int style ) : wxDialog( parent, id, title, pos, size, style )
{
	wxBoxSizer* mainBoxSizer;
	mainBoxSizer = new wxBoxSizer( wxVERTICAL );
	
	wxStaticBoxSizer* staticBoxSizer;
	staticBoxSizer = new wxStaticBoxSizer( new wxStaticBox( this, -1, wxT("New Cylinder:") ), wxVERTICAL );
	
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
	
	heightStaticText = new wxStaticText( this, ID_DEFAULT, wxT("height:"), wxDefaultPosition, wxDefaultSize, 0 );
	settingSizer->Add( heightStaticText, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	heightTextCtrl = new wxTextCtrl( this, ID_DEFAULT, wxT(""), wxDefaultPosition, wxDefaultSize, 0 ,wxTextValidator(wxFILTER_NUMERIC,NULL));
	settingSizer->Add( heightTextCtrl, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_CENTER_HORIZONTAL, 5 );
	
	axisStaticText = new wxStaticText( this, ID_DEFAULT, wxT("Axis:"), wxDefaultPosition, wxDefaultSize, 0 );
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
	
	segmentHStaticText = new wxStaticText( this, ID_DEFAULT, wxT("segment h:"), wxDefaultPosition, wxDefaultSize, 0 );
	settingSizer->Add( segmentHStaticText, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	segmentHTextCtrl = new wxTextCtrl( this, ID_DEFAULT, wxT(""), wxDefaultPosition, wxDefaultSize, 0 ,wxTextValidator(wxFILTER_NUMERIC,NULL));
	settingSizer->Add( segmentHTextCtrl, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_CENTER_HORIZONTAL, 5 );
	
	staticBoxSizer->Add( settingSizer, 1, wxEXPAND|wxALIGN_CENTER_VERTICAL|wxALIGN_CENTER_HORIZONTAL, 5 );
	
	mainBoxSizer->Add( staticBoxSizer, 1, wxEXPAND|wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_CENTER_HORIZONTAL, 5 );
	
	wxBoxSizer* buttonBoxSizer;
	buttonBoxSizer = new wxBoxSizer( wxHORIZONTAL );
	
	OKButton = new wxButton( this, onOKButtonPressCylinder, wxT("OK"), wxDefaultPosition, wxDefaultSize, 0 );
	buttonBoxSizer->Add( OKButton, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_CENTER_HORIZONTAL, 5 );
	
	cancelButton = new wxButton( this, onCancelButtonPressCylinder, wxT("Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
	buttonBoxSizer->Add( cancelButton, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_CENTER_HORIZONTAL, 5 );
	
	mainBoxSizer->Add( buttonBoxSizer, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT, 5 );
	
	this->SetSizer( mainBoxSizer );
	this->Layout();
	this->Fit();
	initializeData();
}

//NewCylinderDialog::~NewCylinderDialog(void){}

void NewCylinderDialog::onOKButton(wxCommandEvent& event)
{
	double x;
	double y;
	double z;
	double radius;
	double height;
	unsigned long segmentA;
	unsigned long segmentR;
	unsigned long segmentH;

	wxString xString=xTextCtrl->GetValue();
	wxString yString=yTextCtrl->GetValue();
	wxString zString=zTextCtrl->GetValue();
	wxString radiusString=radiusTextCtrl->GetValue();
	wxString heightString=heightTextCtrl->GetValue();
	wxString segmentAString=segmentATextCtrl->GetValue();
	wxString segmentRString=segmentRTextCtrl->GetValue();
	wxString segmentHString=segmentHTextCtrl->GetValue();

	if(xString.ToDouble(&x) && yString.ToDouble(&y) && zString.ToDouble(&z) && radiusString.ToDouble(&radius) && heightString.ToDouble(&height) && segmentAString.ToULong(&segmentA) && segmentRString.ToULong(&segmentR) && segmentHString.ToULong(&segmentH))
	{
		if(radius>0 && height>0 && segmentA>2 && segmentH>0 && segmentR>0)
		{
			if(axisChoice->GetCurrentSelection()==0)
			{
				theScene->newCylinder(x,y,z,radius,height,AxisMode::axisX,segmentA,segmentR,segmentH);
				Close();
			}
			else if(axisChoice->GetCurrentSelection()==1)
			{
				theScene->newCylinder(x,y,z,radius,height,AxisMode::axisY,segmentA,segmentR,segmentH);
				Close();
			}
			else if(axisChoice->GetCurrentSelection()==2)
			{
				theScene->newCylinder(x,y,z,radius,height,AxisMode::axisZ,segmentA,segmentR,segmentH);
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
};

void NewCylinderDialog::onCancelButton(wxCommandEvent& event)
{
	Close();
	event;
};
