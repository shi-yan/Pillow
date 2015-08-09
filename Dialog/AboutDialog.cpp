

#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif //__BORLANDC__

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif //WX_PRECOMP
#include "wx/utils.h"
#include "AboutDialog.h"
///////////////////////////////////////////////////////////////////////////

BEGIN_EVENT_TABLE(AboutDialog, wxDialog)
	EVT_BUTTON(linkButtonPress, AboutDialog::onLinkButton)
	EVT_BUTTON(closeButtonPress,AboutDialog::onClose)
END_EVENT_TABLE()

AboutDialog::AboutDialog( wxWindow* parent, int id, wxString title, wxPoint pos, wxSize size, int style ) : wxDialog( parent, id, title, pos, size, style )
{
	wxBoxSizer* mainSizer;
	mainSizer = new wxBoxSizer( wxVERTICAL );
	
	wxStaticBoxSizer* aboutSizer;
	aboutSizer = new wxStaticBoxSizer( new wxStaticBox( this, -1, wxT("About:") ), wxHORIZONTAL );
	
	logoBitmap = new wxStaticBitmap( this, ID_DEFAULT, wxBitmap( wxT("about.dat"), wxBITMAP_TYPE_ANY ), wxDefaultPosition, wxDefaultSize, 0 );
	aboutSizer->Add( logoBitmap, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_CENTER_HORIZONTAL, 5 );
	
	wxBoxSizer* infoSizer;
	infoSizer = new wxBoxSizer( wxVERTICAL );
	
	infoStaticText = new wxStaticText( this, ID_DEFAULT, wxT("Pillow 0.1a \n Billconan Studio 2006 \n Blog:http://billconan.blogspot.com"), wxDefaultPosition, wxDefaultSize, 0 );
	infoSizer->Add( infoStaticText, 1, wxALL, 5 );
	
	linkButton = new wxButton( this, linkButtonPress, wxT("Go to the site!"), wxDefaultPosition, wxDefaultSize, 0 );
	infoSizer->Add( linkButton, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_CENTER_HORIZONTAL, 5 );
	
	aboutSizer->Add( infoSizer, 1, wxEXPAND|wxALIGN_CENTER_VERTICAL|wxALIGN_CENTER_HORIZONTAL, 5 );
	
	mainSizer->Add( aboutSizer, 1, wxEXPAND|wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_CENTER_HORIZONTAL, 5 );
	
	wxBoxSizer* buttonSizer;
	buttonSizer = new wxBoxSizer( wxVERTICAL );
	
	closeButton = new wxButton( this, closeButtonPress, wxT("Close"), wxDefaultPosition, wxDefaultSize, 0 );
	buttonSizer->Add( closeButton, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_CENTER_HORIZONTAL, 5 );
	
	mainSizer->Add( buttonSizer, 0, wxEXPAND|wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_CENTER_HORIZONTAL, 5 );
	
	this->SetSizer( mainSizer );
	this->Layout();
	this->Fit();
}

void AboutDialog::onLinkButton(wxCommandEvent& event)
{
	wxLaunchDefaultBrowser(_T("http://billconan.blogspot.com"));
	event;
};

void AboutDialog::onClose(wxCommandEvent& event)
{
	Close();	
	event;
};

//AboutDialog::~AboutDialog(void){}
