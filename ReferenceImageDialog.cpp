

#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif //__BORLANDC__

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif //WX_PRECOMP
#include <wx/filedlg.h>
#include "ReferenceImageDialog.h"
#include "Screen.h"
///////////////////////////////////////////////////////////////////////////

BEGIN_EVENT_TABLE(ReferenceImageDialog, wxDialog)
	EVT_BUTTON(onOpenButtonPressReferenceImage, ReferenceImageDialog::onOpenButton)
	EVT_BUTTON(onOKButtonPressReferenceImage, ReferenceImageDialog::onOKButton)
	EVT_BUTTON(onCancelButtonPressReferenceImage,ReferenceImageDialog::onCancelButton)
END_EVENT_TABLE()

ReferenceImageDialog::ReferenceImageDialog( wxWindow* parent, int id, wxString title, wxPoint pos, wxSize size, int style ) : wxDialog( parent, id, title, pos, size, style ),referenceID(0),width(0),height(0)
{
	wxBoxSizer* mainBoxSizer;
	mainBoxSizer = new wxBoxSizer( wxVERTICAL );
	
	wxStaticBoxSizer* mainStaticBoxSizer;
	mainStaticBoxSizer = new wxStaticBoxSizer( new wxStaticBox( this, -1, wxT("Reference Image:") ), wxVERTICAL );
	
	wxBoxSizer* settingSizer;
	settingSizer = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* fileBoxSizer;
	fileBoxSizer = new wxBoxSizer( wxHORIZONTAL );
	
	pathTextCtrl = new wxTextCtrl( this, ID_DEFAULT, wxT(""), wxDefaultPosition, wxDefaultSize, wxTE_READONLY );
	fileBoxSizer->Add( pathTextCtrl, 1, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_CENTER_HORIZONTAL|wxEXPAND, 5 );
	
	openButton = new wxButton( this, onOpenButtonPressReferenceImage, wxT("Open"), wxDefaultPosition, wxDefaultSize, 0 );
	fileBoxSizer->Add( openButton, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_CENTER_HORIZONTAL, 5 );
	
	settingSizer->Add( fileBoxSizer, 0, wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );
	
	mainStaticLine = new wxStaticLine( this, ID_DEFAULT, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	settingSizer->Add( mainStaticLine, 0, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL|wxALIGN_CENTER_HORIZONTAL, 5 );
	
	wxGridSizer* settingGirdSizer;
	settingGirdSizer = new wxGridSizer( 2, 2, 0, 0 );
	
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
	
	widthStaticText = new wxStaticText( this, ID_DEFAULT, wxT("width:"), wxDefaultPosition, wxDefaultSize, 0 );
	settingGirdSizer->Add( widthStaticText, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	widthTextCtrl = new wxTextCtrl( this, ID_DEFAULT, wxT(""), wxDefaultPosition, wxDefaultSize, 0 ,wxTextValidator(wxFILTER_NUMERIC,NULL));
	settingGirdSizer->Add( widthTextCtrl, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_CENTER_HORIZONTAL, 5 );
	
	heightStaticText = new wxStaticText( this, ID_DEFAULT, wxT("height:"), wxDefaultPosition, wxDefaultSize, 0 );
	settingGirdSizer->Add( heightStaticText, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	heightTextCtrl = new wxTextCtrl( this, ID_DEFAULT, wxT(""), wxDefaultPosition, wxDefaultSize, 0 ,wxTextValidator(wxFILTER_NUMERIC,NULL));
	settingGirdSizer->Add( heightTextCtrl, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );
	
	settingSizer->Add( settingGirdSizer, 1, wxEXPAND|wxALIGN_CENTER_VERTICAL|wxALIGN_CENTER_HORIZONTAL, 5 );
	
	mainStaticBoxSizer->Add( settingSizer, 1, wxEXPAND|wxALIGN_CENTER_VERTICAL|wxALIGN_CENTER_HORIZONTAL, 5 );
	
	mainBoxSizer->Add( mainStaticBoxSizer, 1, wxEXPAND|wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_CENTER_HORIZONTAL, 5 );
	
	wxBoxSizer* buttonSizer;
	buttonSizer = new wxBoxSizer( wxHORIZONTAL );
	
	OKButton = new wxButton( this, onOKButtonPressReferenceImage, wxT("OK"), wxDefaultPosition, wxDefaultSize, 0 );
	buttonSizer->Add( OKButton, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_CENTER_HORIZONTAL, 5 );
	
	CancelButton = new wxButton( this, onCancelButtonPressReferenceImage, wxT("Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
	buttonSizer->Add( CancelButton, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_CENTER_HORIZONTAL, 5 );
	
	mainBoxSizer->Add( buttonSizer, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5 );
	
	this->SetSizer( mainBoxSizer );
	this->Layout();
	Fit();
	initialize();
}

void ReferenceImageDialog::initialize()
{
	Vector position;
	std::string path;
	theScreen->getViewportImage(referenceID,path,position,this->width,this->height);
	(*pathTextCtrl)<<path.c_str();
	(*xTextCtrl)<<position.x;
	(*yTextCtrl)<<position.y;
	(*zTextCtrl)<<position.z;
	(*widthTextCtrl)<<(int)width;
	(*heightTextCtrl)<<(int)height;
}

void ReferenceImageDialog::onOpenButton(wxCommandEvent& event)
{
	wxString wildcards=wxString::Format(_T("PNG files (*.png)|*.png|JPEG files (*.jpg)|*.jpg"),wxFileSelectorDefaultWildcardStr,wxFileSelectorDefaultWildcardStr);
	wxFileDialog dialog(this, _T("Open file:"),wxEmptyString, wxEmptyString, wildcards,wxOPEN);
	if(dialog.ShowModal()==wxID_OK)
	{
		wxArrayString paths, filenames;
		dialog.GetPaths(paths);
		dialog.GetFilenames(filenames);
		(*pathTextCtrl)<<paths[0];
		if(dialog.GetFilterIndex()==0)
		{
			referenceID=loadTexture(paths[0].c_str(),wxBITMAP_TYPE_PNG);
			(*xTextCtrl).SetValue(_T("0"));
			(*yTextCtrl).SetValue(_T("0"));
			(*zTextCtrl).SetValue(_T("0"));
			(*widthTextCtrl).SetValue(_T(""));
			(*widthTextCtrl)<<(int)width;
			(*heightTextCtrl).SetValue(_T(""));
			(*heightTextCtrl)<<(int)height;
		}
		else if(dialog.GetFilterIndex()==1)
		{
			referenceID=loadTexture(paths[0].c_str(),wxBITMAP_TYPE_JPEG);
			(*xTextCtrl).SetValue(_T("0"));
			(*yTextCtrl).SetValue(_T("0"));
			(*zTextCtrl).SetValue(_T("0"));
			(*widthTextCtrl).SetValue(_T(""));
			(*widthTextCtrl)<<(int)width;
			(*heightTextCtrl).SetValue(_T(""));
			(*heightTextCtrl)<<(int)height;
		}
		Refresh(false);
	}
	event;
}

void ReferenceImageDialog::onOKButton(wxCommandEvent& event)
{
	double x;
	double y;
	double z;

	wxString pathString=pathTextCtrl->GetValue();
	wxString xString=xTextCtrl->GetValue();
	wxString yString=yTextCtrl->GetValue();
	wxString zString=zTextCtrl->GetValue();
	
	if(referenceID>0)
	{
		if(xString.ToDouble(&x) && yString.ToDouble(&y) && zString.ToDouble(&z))
		{
			Vector position(x,y,z);
			theScreen->setViewportImage(pathString.c_str(),referenceID,position,width,height);
			Close();
		}
		else
		{
					wxMessageBox(wxT("Wrong parameters!"),wxT("Wrong Parameters"),wxICON_ERROR | wxOK, this);
		return;
		}
	}
	Close();
	event;
}

void ReferenceImageDialog::onCancelButton(wxCommandEvent& event)
{
	Close();
	event;
}