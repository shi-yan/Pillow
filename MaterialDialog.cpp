

#ifdef __BORLANDC__
#pragma hdrstop
#endif //__BORLANDC__

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif //WX_PRECOMP

#include <wx/colordlg.h>

#include "MaterialDialog.h"
#include "Scene.h"
///////////////////////////////////////////////////////////////////////////

BEGIN_EVENT_TABLE(MaterialDialog , wxDialog)
	EVT_BUTTON(onAmbientButtonMaterialEdit,MaterialDialog::onAmbientButton)
	EVT_BUTTON(onDiffuseButtonMaterialEdit,MaterialDialog::onDiffuseButton)
	EVT_BUTTON(onSpecularButtonMaterialEdit,MaterialDialog::onSpecularButton)
	EVT_BUTTON(onEmissionButtonMaterialEdit,MaterialDialog::onEmissionButton)
	EVT_BUTTON(onOKButtonPressMaterialEdit, MaterialDialog::onOKButton)
	EVT_BUTTON(onCancelButtonPressMaterialEdit,MaterialDialog::onCancelButton)
END_EVENT_TABLE()

MaterialDialog::MaterialDialog( wxWindow* parent, int id, wxString title, wxPoint pos, wxSize size, int style ) : wxDialog( parent, id, title, pos, size, style )
{
	wxBoxSizer* mainSizer;
	mainSizer = new wxBoxSizer( wxVERTICAL );
	
	wxStaticBoxSizer* settingSizer;
	settingSizer = new wxStaticBoxSizer( new wxStaticBox( this, -1, wxT("Material:") ), wxVERTICAL );
	
	wxGridSizer* settingGirdSizer;
	settingGirdSizer = new wxGridSizer( 2, 2, 0, 0 );
	
	ambientStaticText = new wxStaticText( this, ID_DEFAULT , wxT("ambient:"), wxDefaultPosition, wxDefaultSize, 0 );
	settingGirdSizer->Add( ambientStaticText, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	ambientButton = new wxButton( this,onAmbientButtonMaterialEdit, wxT(""), wxDefaultPosition, wxDefaultSize, 0 );
	ambientButton->SetBackgroundColour( wxColour( 38, 21, 249 ) );
	
	settingGirdSizer->Add( ambientButton, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL, 5 );
	
	diffuseStaticText = new wxStaticText( this, ID_DEFAULT, wxT("diffuse:"), wxDefaultPosition, wxDefaultSize, 0 );
	settingGirdSizer->Add( diffuseStaticText, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	diffuseButton = new wxButton( this,onDiffuseButtonMaterialEdit, wxT(""), wxDefaultPosition, wxDefaultSize, 0 );
	diffuseButton->SetBackgroundColour( wxColour( 247, 51, 40 ) );
	
	settingGirdSizer->Add( diffuseButton, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL, 5 );
	
	specularStaticText = new wxStaticText( this, ID_DEFAULT, wxT("specular:"), wxDefaultPosition, wxDefaultSize, 0 );
	settingGirdSizer->Add( specularStaticText, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	specularButton = new wxButton( this, onSpecularButtonMaterialEdit, wxT(""), wxDefaultPosition, wxDefaultSize, 0 );
	specularButton->SetBackgroundColour( wxColour( 253, 243, 55 ) );
	
	settingGirdSizer->Add( specularButton, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxEXPAND, 5 );
	
	emissionStaticText = new wxStaticText( this, ID_DEFAULT, wxT("emission:"), wxDefaultPosition, wxDefaultSize, 0 );
	settingGirdSizer->Add( emissionStaticText, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	emissionButton = new wxButton( this,onEmissionButtonMaterialEdit, wxT(""), wxDefaultPosition, wxDefaultSize, 0 );
	emissionButton->SetBackgroundColour( wxColour( 134, 248, 73 ) );
	
	settingGirdSizer->Add( emissionButton, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_CENTER_HORIZONTAL|wxEXPAND, 5 );
	
	shininessStaticText = new wxStaticText( this, ID_DEFAULT, wxT("shininess:"), wxDefaultPosition, wxDefaultSize, 0 );
	settingGirdSizer->Add( shininessStaticText, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	shininessSlider = new wxSlider( this, ID_DEFAULT, 10, 0, 20, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL );
	settingGirdSizer->Add( shininessSlider, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_CENTER_HORIZONTAL|wxEXPAND, 5 );
	
	settingSizer->Add( settingGirdSizer, 1, wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );
	
	mainSizer->Add( settingSizer, 1, wxEXPAND|wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );
	
	wxBoxSizer* buttonSizer;
	buttonSizer = new wxBoxSizer( wxHORIZONTAL );
	
	OKButton = new wxButton( this, onOKButtonPressMaterialEdit, wxT("OK"), wxDefaultPosition, wxDefaultSize, 0 );
	buttonSizer->Add( OKButton, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_CENTER_HORIZONTAL, 5 );
	
	cancelButton = new wxButton( this, onCancelButtonPressMaterialEdit, wxT("Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
	buttonSizer->Add( cancelButton, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_CENTER_HORIZONTAL, 5 );
	
	mainSizer->Add( buttonSizer, 0, wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT, 5 );
	
	this->SetSizer( mainSizer );
	this->Layout();
	Fit();
	initializeData();
}


//MaterialDialog::~MaterialDialog(void){}
void MaterialDialog::initializeData()
{
	float ambient[4]={0.0f};
	float diffuse[4]={0.0f};
	float specular[4]={0.0f};
	float emission[4]={0.0f};
	float shininess=0.0f;
	theScene->getSelectionMaterial(ambient,diffuse,specular,emission,&shininess);
	ambientButton->SetBackgroundColour( wxColour( 255.0f*ambient[0],255.0f*ambient[1],255.0f*ambient[2] ) );
	diffuseButton->SetBackgroundColour( wxColour( 255.0f*diffuse[0],255.0f*diffuse[1],255.0f*diffuse[2] ) );
	specularButton->SetBackgroundColour( wxColour( 255.0f*specular[0],255.0f*specular[1],255.0f*specular[2] ) );
	emissionButton->SetBackgroundColour( wxColour( 255.0f*emission[0],255.0f*emission[1],255.0f*emission[2] ) );
	shininessSlider->SetValue(shininess);
}

void MaterialDialog::onAmbientButton(wxCommandEvent& event)
{
	wxColourData colorData;
	colorData.SetColour(ambientButton->GetBackgroundColour());
    wxColourDialog dialog(this, &colorData);
	dialog.SetTitle(_T("Choose the ambient colour:"));
    if (dialog.ShowModal() == wxID_OK)
    {
        colorData = dialog.GetColourData();
        ambientButton->SetBackgroundColour(colorData.GetColour());
		Refresh(false);
    }
	event;
};

void MaterialDialog::onDiffuseButton(wxCommandEvent& event)
{
	wxColourData colorData;
	colorData.SetColour(diffuseButton->GetBackgroundColour());
    wxColourDialog dialog(this, &colorData);
	dialog.SetTitle(_T("Choose the diffuse colour:"));
    if (dialog.ShowModal() == wxID_OK)
    {
        colorData = dialog.GetColourData();
        diffuseButton->SetBackgroundColour(colorData.GetColour());
		Refresh(false);
    }
	event;
};

void MaterialDialog::onSpecularButton(wxCommandEvent& event)
{
	wxColourData colorData;
	colorData.SetColour(specularButton->GetBackgroundColour());
    wxColourDialog dialog(this, &colorData);
	dialog.SetTitle(_T("Choose the specular colour:"));
    if (dialog.ShowModal() == wxID_OK)
    {
        colorData = dialog.GetColourData();
        specularButton->SetBackgroundColour(colorData.GetColour());
		Refresh(false);
    }
	event;
};

void MaterialDialog::onEmissionButton(wxCommandEvent& event)
{
		wxColourData colorData;
	colorData.SetColour(emissionButton->GetBackgroundColour());
    wxColourDialog dialog(this, &colorData);
	dialog.SetTitle(_T("Choose the emission colour:"));
    if (dialog.ShowModal() == wxID_OK)
    {
        colorData = dialog.GetColourData();
        emissionButton->SetBackgroundColour(colorData.GetColour());
		Refresh(false);
    }
	event;
};

void MaterialDialog::onOKButton(wxCommandEvent& event)
{
	float ambient[4];
	float diffuse[4];
	float specular[4];
	float emission[4];
	float shininess=shininessSlider->GetValue();

	float factor=1.0f/255.0f;
	wxColour theColor(ambientButton->GetBackgroundColour());
	ambient[0]=((float)theColor.Red())*factor;
	ambient[1]=((float)theColor.Green())*factor;
	ambient[2]=((float)theColor.Blue())*factor;
	ambient[3]=1.0f;

	theColor=diffuseButton->GetBackgroundColour();
	diffuse[0]=((float)theColor.Red())*factor;
	diffuse[1]=((float)theColor.Green())*factor;
	diffuse[2]=((float)theColor.Blue())*factor;
	diffuse[3]=1.0f;

	theColor=specularButton->GetBackgroundColour();
	specular[0]=((float)theColor.Red())*factor;
	specular[1]=((float)theColor.Green())*factor;
	specular[2]=((float)theColor.Blue())*factor;
	specular[3]=1.0f;

	theColor=emissionButton->GetBackgroundColour();
	emission[0]=((float)theColor.Red())*factor;
	emission[1]=((float)theColor.Green())*factor;
	emission[2]=((float)theColor.Blue())*factor;
	emission[3]=1.0f;

	theScene->setSelectionMaterial(ambient,diffuse,specular,emission,shininess);
	
	Close();
	event;
};

void MaterialDialog::onCancelButton(wxCommandEvent& event)
{
	Close();
	event;
};