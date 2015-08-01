
///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Aug 16 2006)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif //__BORLANDC__

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif //WX_PRECOMP

#include "NewCubeDialog.h"
#include "Scene.h"

///////////////////////////////////////////////////////////////////////////

BEGIN_EVENT_TABLE(NewCubeDialog, wxDialog)
	EVT_BUTTON(onOKButtonPressCube, NewCubeDialog::onOKButton)
	EVT_BUTTON(onCancelButtonPressCube,NewCubeDialog::onCancelButton)
END_EVENT_TABLE()

NewCubeDialog::NewCubeDialog( wxWindow* parent, int id, wxString title, wxPoint pos, wxSize size, int style ) : wxDialog( parent, id, title, pos, size, style )
{
	wxBoxSizer* mainBoxSizer;
	mainBoxSizer = new wxBoxSizer( wxVERTICAL );
	
	wxStaticBoxSizer* settingSizer;
	settingSizer = new wxStaticBoxSizer( new wxStaticBox( this, -1, wxT("New Cube") ), wxVERTICAL );
	
	wxGridSizer* mainGirdSizer;
	mainGirdSizer = new wxGridSizer( 9, 2, 0, 0 );
	
	xStaticText = new wxStaticText( this, ID_DEFAULT, wxT("x:"), wxDefaultPosition, wxDefaultSize, 0 );
	mainGirdSizer->Add( xStaticText, 0, wxALL, 5 );
	
	xTextCtrl = new wxTextCtrl( this, ID_DEFAULT, wxT(""), wxDefaultPosition, wxDefaultSize, 0,wxTextValidator(wxFILTER_NUMERIC,NULL) );
	mainGirdSizer->Add( xTextCtrl, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	yStaticText = new wxStaticText( this, ID_DEFAULT, wxT("y:"), wxDefaultPosition, wxDefaultSize, 0 );
	mainGirdSizer->Add( yStaticText, 0, wxALL, 5 );
	
	yTextCtrl = new wxTextCtrl( this, ID_DEFAULT, wxT(""), wxDefaultPosition, wxDefaultSize, 0 ,wxTextValidator(wxFILTER_NUMERIC,NULL));
	mainGirdSizer->Add( yTextCtrl, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	ZStaticText = new wxStaticText( this, ID_DEFAULT, wxT("z:"), wxDefaultPosition, wxDefaultSize, 0 );
	mainGirdSizer->Add( ZStaticText, 0, wxALL, 5 );
	
	zTextCtrl = new wxTextCtrl( this, ID_DEFAULT, wxT(""), wxDefaultPosition, wxDefaultSize, 0 ,wxTextValidator(wxFILTER_NUMERIC,NULL));
	mainGirdSizer->Add( zTextCtrl, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	lengthStaticText = new wxStaticText( this, ID_DEFAULT, wxT("length(x):"), wxDefaultPosition, wxDefaultSize, 0 );
	mainGirdSizer->Add( lengthStaticText, 0, wxALL, 5 );
	
	lengthTextCtrl = new wxTextCtrl( this, ID_DEFAULT, wxT(""), wxDefaultPosition, wxDefaultSize, 0 ,wxTextValidator(wxFILTER_NUMERIC,NULL));
	mainGirdSizer->Add( lengthTextCtrl, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	widthStaticText = new wxStaticText( this, ID_DEFAULT, wxT("width(y):"), wxDefaultPosition, wxDefaultSize, 0 );
	mainGirdSizer->Add( widthStaticText, 0, wxALL, 5 );
	
	widthTextCtrl = new wxTextCtrl( this, ID_DEFAULT, wxT(""), wxDefaultPosition, wxDefaultSize, 0 ,wxTextValidator(wxFILTER_NUMERIC,NULL));
	mainGirdSizer->Add( widthTextCtrl, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	heightStaticText = new wxStaticText( this, ID_DEFAULT, wxT("height(z):"), wxDefaultPosition, wxDefaultSize, 0 );
	mainGirdSizer->Add( heightStaticText, 0, wxALL, 5 );
	
	heightTextCtrl = new wxTextCtrl( this, ID_DEFAULT, wxT(""), wxDefaultPosition, wxDefaultSize, 0 ,wxTextValidator(wxFILTER_NUMERIC,NULL));
	mainGirdSizer->Add( heightTextCtrl, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	segmentXStaticText = new wxStaticText( this, ID_DEFAULT, wxT("segment x:"), wxDefaultPosition, wxDefaultSize, 0 );
	mainGirdSizer->Add( segmentXStaticText, 0, wxALL, 5 );
	
	segmentXTextCtrl = new wxTextCtrl( this, ID_DEFAULT, wxT(""), wxDefaultPosition, wxDefaultSize, 0,wxTextValidator(wxFILTER_NUMERIC,NULL) );
	mainGirdSizer->Add( segmentXTextCtrl, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	segmentYStaticText = new wxStaticText( this, ID_DEFAULT, wxT("segment y:"), wxDefaultPosition, wxDefaultSize, 0 );
	mainGirdSizer->Add( segmentYStaticText, 0, wxALL, 5 );
	
	segmentYTextCtrl = new wxTextCtrl( this, ID_DEFAULT, wxT(""), wxDefaultPosition, wxDefaultSize, 0 ,wxTextValidator(wxFILTER_NUMERIC,NULL));
	mainGirdSizer->Add( segmentYTextCtrl, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	segmentZStaticText = new wxStaticText( this, ID_DEFAULT, wxT("segment z:"), wxDefaultPosition, wxDefaultSize, 0 );
	mainGirdSizer->Add( segmentZStaticText, 0, wxALL, 5 );
	
	segmentZTextCtrl = new wxTextCtrl( this, ID_DEFAULT, wxT(""), wxDefaultPosition, wxDefaultSize, 0,wxTextValidator(wxFILTER_NUMERIC,NULL) );
	mainGirdSizer->Add( segmentZTextCtrl, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	settingSizer->Add( mainGirdSizer, 0, wxALIGN_CENTER_VERTICAL|wxALIGN_CENTER_HORIZONTAL, 5 );
	
	mainBoxSizer->Add( settingSizer, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_CENTER_HORIZONTAL, 5 );
	
	wxBoxSizer* buttonBoxSizer;
	buttonBoxSizer = new wxBoxSizer( wxHORIZONTAL );
	
	OKButton = new wxButton( this, onOKButtonPressCube, wxT("OK"), wxDefaultPosition, wxDefaultSize, 0 );
	buttonBoxSizer->Add( OKButton, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5 );
	
	cancelButton = new wxButton( this, onCancelButtonPressCube, wxT("Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
	buttonBoxSizer->Add( cancelButton, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5 );
	
	mainBoxSizer->Add( buttonBoxSizer, 0, wxBOTTOM|wxLEFT|wxRIGHT|wxTOP|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5 );
	
	this->SetSizer( mainBoxSizer );
	this->Layout();
	this->Fit();
	initializeData();
}

void NewCubeDialog::onOKButton(wxCommandEvent& event)
{
	double x;
	double y;
	double z;
	double length;
	double width;
	double height;
	unsigned long segmentX;
	unsigned long segmentY;
	unsigned long segmentZ;

	wxString xString=xTextCtrl->GetValue();
	wxString yString=yTextCtrl->GetValue();
	wxString zString=zTextCtrl->GetValue();
	wxString lengthString=lengthTextCtrl->GetValue();
	wxString widthString=widthTextCtrl->GetValue();
	wxString heightString=heightTextCtrl->GetValue();
	wxString segmentXString=segmentXTextCtrl->GetValue();
	wxString segmentYString=segmentYTextCtrl->GetValue();
	wxString segmentZString=segmentZTextCtrl->GetValue();

	if(xString.ToDouble(&x) && yString.ToDouble(&y) && zString.ToDouble(&z) && lengthString.ToDouble(&length)&& widthString.ToDouble(&width)&& heightString.ToDouble(&height)&&segmentXString.ToULong(&segmentX) && segmentYString.ToULong(&segmentY)&&segmentZString.ToULong(&segmentZ))
	{
		if(length>0 && width>0 && height>0 && segmentX>0 && segmentY>0 && segmentZ>0)
		{
			theScene->newCube(x,y,z,length,width,height,segmentX,segmentY,segmentZ);
			Close();
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

void NewCubeDialog::onCancelButton(wxCommandEvent& event)
{
	Close();
	event;
}