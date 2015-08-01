

#ifdef __BORLANDC__
#pragma hdrstop
#endif //__BORLANDC__

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif //WX_PRECOMP

#include "NewPlaneDialog.h"
#include "Scene.h"

///////////////////////////////////////////////////////////////////////////
BEGIN_EVENT_TABLE(NewPlaneDialog, wxDialog)
	EVT_BUTTON(onOKButtonPressPlane, NewPlaneDialog::onOKButton)
	EVT_BUTTON(onCancelButtonPressPlane,NewPlaneDialog::onCancelButton)
END_EVENT_TABLE()

NewPlaneDialog::NewPlaneDialog( wxWindow* parent, int id, wxString title, wxPoint pos, wxSize size, int style ) : wxDialog( parent, id, title, pos, size, style )
{
	wxBoxSizer* mainSizer;
	mainSizer = new wxBoxSizer( wxVERTICAL );
	
	wxStaticBoxSizer* newPlaneStaticSizer;
	newPlaneStaticSizer = new wxStaticBoxSizer( new wxStaticBox( this, -1, wxT("New Plane:") ), wxVERTICAL );
	
	wxGridSizer* mainGirdSizer;
	mainGirdSizer = new wxGridSizer( 2, 2, 0, 0 );
	
	xStaticText = new wxStaticText( this, ID_DEFAULT, wxT("x:"), wxDefaultPosition, wxDefaultSize, 0 );
	mainGirdSizer->Add( xStaticText, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	xTextCtrl = new wxTextCtrl( this, ID_DEFAULT, wxT(""), wxDefaultPosition, wxDefaultSize, 0 ,wxTextValidator(wxFILTER_NUMERIC,NULL));
	mainGirdSizer->Add( xTextCtrl, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_CENTER_HORIZONTAL, 5 );
	
	yStaticText = new wxStaticText( this, ID_DEFAULT, wxT("y:"), wxDefaultPosition, wxDefaultSize, 0 );
	mainGirdSizer->Add( yStaticText, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	yTextCtrl = new wxTextCtrl( this, ID_DEFAULT, wxT(""), wxDefaultPosition, wxDefaultSize, 0 ,wxTextValidator(wxFILTER_NUMERIC,NULL));
	mainGirdSizer->Add( yTextCtrl, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_CENTER_HORIZONTAL, 5 );
	
	zStaticText = new wxStaticText( this, ID_DEFAULT, wxT("z:"), wxDefaultPosition, wxDefaultSize, 0 );
	mainGirdSizer->Add( zStaticText, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	zTextCtrl = new wxTextCtrl( this, ID_DEFAULT, wxT(""), wxDefaultPosition, wxDefaultSize, 0 ,wxTextValidator(wxFILTER_NUMERIC,NULL));
	mainGirdSizer->Add( zTextCtrl, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_CENTER_HORIZONTAL, 5 );
	
	lengthStaticText = new wxStaticText( this, ID_DEFAULT, wxT("length:"), wxDefaultPosition, wxDefaultSize, 0 );
	mainGirdSizer->Add( lengthStaticText, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	lengthTextCtrl = new wxTextCtrl( this, ID_DEFAULT, wxT(""), wxDefaultPosition, wxDefaultSize, 0 ,wxTextValidator(wxFILTER_NUMERIC,NULL));
	mainGirdSizer->Add( lengthTextCtrl, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_CENTER_HORIZONTAL, 5 );
	
	widthStaticText = new wxStaticText( this, ID_DEFAULT, wxT("width:"), wxDefaultPosition, wxDefaultSize, 0 );
	mainGirdSizer->Add( widthStaticText, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	widthTextCtrl = new wxTextCtrl( this, ID_DEFAULT, wxT(""), wxDefaultPosition, wxDefaultSize, 0 ,wxTextValidator(wxFILTER_NUMERIC,NULL));
	mainGirdSizer->Add( widthTextCtrl, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_CENTER_HORIZONTAL, 5 );
	
	segmentLStaticText = new wxStaticText( this, ID_DEFAULT, wxT("segment length:"), wxDefaultPosition, wxDefaultSize, 0 );
	mainGirdSizer->Add( segmentLStaticText, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	segmentLTextCtrl = new wxTextCtrl( this, ID_DEFAULT, wxT(""), wxDefaultPosition, wxDefaultSize, 0 ,wxTextValidator(wxFILTER_NUMERIC,NULL));
	mainGirdSizer->Add( segmentLTextCtrl, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_CENTER_HORIZONTAL, 5 );
	
	segmentWStaticText = new wxStaticText( this, ID_DEFAULT, wxT("segment width:"), wxDefaultPosition, wxDefaultSize, 0 );
	mainGirdSizer->Add( segmentWStaticText, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	segmentWTextCtrl = new wxTextCtrl( this, ID_DEFAULT, wxT(""), wxDefaultPosition, wxDefaultSize, 0 ,wxTextValidator(wxFILTER_NUMERIC,NULL));
	mainGirdSizer->Add( segmentWTextCtrl, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_CENTER_HORIZONTAL, 5 );
	
	axisStaticText = new wxStaticText( this, ID_DEFAULT, wxT("axis:"), wxDefaultPosition, wxDefaultSize, 0 );
	mainGirdSizer->Add( axisStaticText, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	wxString axisChoiceChoices[] = { wxT("x"), wxT("y"), wxT("z") };
	int axisChoiceNChoices = sizeof( axisChoiceChoices ) / sizeof( wxString );
	axisChoice = new wxChoice( this, ID_DEFAULT, wxDefaultPosition, wxDefaultSize, axisChoiceNChoices, axisChoiceChoices, 0 );
	mainGirdSizer->Add( axisChoice, 0, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL|wxALIGN_CENTER_HORIZONTAL, 5 );
	
	newPlaneStaticSizer->Add( mainGirdSizer, 1, wxEXPAND|wxALIGN_CENTER_VERTICAL|wxALIGN_CENTER_HORIZONTAL, 5 );
	
	mainSizer->Add( newPlaneStaticSizer, 1, wxEXPAND|wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );
	
	wxBoxSizer* buttonSizer;
	buttonSizer = new wxBoxSizer( wxHORIZONTAL );
	
	OKButton = new wxButton( this, onOKButtonPressPlane, wxT("OK"), wxDefaultPosition, wxDefaultSize, 0 );
	buttonSizer->Add( OKButton, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_CENTER_HORIZONTAL, 5 );
	
	cancelButton = new wxButton( this, onCancelButtonPressPlane, wxT("Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
	buttonSizer->Add( cancelButton, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_CENTER_HORIZONTAL, 5 );
	
	mainSizer->Add( buttonSizer, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT, 5 );
	
	this->SetSizer( mainSizer );
	this->Layout();
	Fit();
	initializeData();
}

void NewPlaneDialog::onOKButton(wxCommandEvent& event)
{
	double x;
	double y;
	double z;
	double length;
	double width;
	unsigned long segmentL;
	unsigned long segmentW;

	wxString xString=xTextCtrl->GetValue();
	wxString yString=yTextCtrl->GetValue();
	wxString zString=zTextCtrl->GetValue();
	wxString lengthString=lengthTextCtrl->GetValue();
	wxString widthString=widthTextCtrl->GetValue();
	wxString segmentLString=segmentLTextCtrl->GetValue();
	wxString segmentWString=segmentWTextCtrl->GetValue();

	if(xString.ToDouble(&x) && yString.ToDouble(&y) && zString.ToDouble(&z) && lengthString.ToDouble(&length) && widthString.ToDouble(&width) && segmentLString.ToULong(&segmentL) && segmentWString.ToULong(&segmentW))
	{
		if(length>0 && width>0 && segmentL>0 && segmentW>0)
		{
			if(axisChoice->GetCurrentSelection()==0)
			{
				theScene->newPlane(x,y,z,length,width,AxisMode::axisX,segmentL,segmentW);
				Close();
			}
			else if(axisChoice->GetCurrentSelection()==1)
			{
				theScene->newPlane(x,y,z,length,width,AxisMode::axisY,segmentL,segmentW);
				Close();
			}
			else if(axisChoice->GetCurrentSelection()==2)
			{
				theScene->newPlane(x,y,z,length,width,AxisMode::axisZ,segmentL,segmentW);
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

void NewPlaneDialog::onCancelButton(wxCommandEvent& event)
{
	Close();
	event;
}
