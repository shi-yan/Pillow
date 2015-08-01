#pragma once
#ifdef WX_GCH
#include <wx_pch.h>
#else
#include <wx/wx.h>
#endif

#include <wx/button.h>
#include <wx/choice.h>
#include <wx/slider.h>
#include <wx/statbmp.h>
#include <wx/statline.h>

///////////////////////////////////////////////////////////////////////////

#define ID_DEFAULT wxID_ANY // Default

/**
 * Class MaterialDialog
 */
class MaterialDialog : public wxDialog
{
	private:
	
	protected:
		wxStaticText* ambientStaticText;
		wxButton* ambientButton;
		wxStaticText* diffuseStaticText;
		wxButton* diffuseButton;
		wxStaticText* specularStaticText;
		wxButton* specularButton;
		wxStaticText* emissionStaticText;
		wxButton* emissionButton;
		wxStaticText* shininessStaticText;
		wxSlider* shininessSlider;
		wxButton* OKButton;
		wxButton* cancelButton;
	
	public:
		void initializeData();
		MaterialDialog( wxWindow* parent, int id = -1, wxString title = wxT(""), wxPoint pos = wxDefaultPosition, wxSize size = wxDefaultSize, int style = wxDEFAULT_DIALOG_STYLE );
		void onAmbientButton(wxCommandEvent& event);
		void onDiffuseButton(wxCommandEvent& event);
		void onSpecularButton(wxCommandEvent& event);
		void onEmissionButton(wxCommandEvent& event);
		void onOKButton(wxCommandEvent& event);
		void onCancelButton(wxCommandEvent& event);
	DECLARE_EVENT_TABLE()
};

enum
{
	onAmbientButtonMaterialEdit=234,
	onDiffuseButtonMaterialEdit,
	onSpecularButtonMaterialEdit,
	onEmissionButtonMaterialEdit,
	onOKButtonMaterialEdit,
	onCancelButtonMaterialEdit,
	onOKButtonPressMaterialEdit,
	onCancelButtonPressMaterialEdit
};

