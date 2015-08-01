#pragma once

#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>

#ifdef WX_GCH
#include <wx_pch.h>
#else
#include <wx/wx.h>
#endif
#include <wx/image.h>
#include <wx/button.h>
#include <wx/statline.h>

///////////////////////////////////////////////////////////////////////////

#define ID_DEFAULT wxID_ANY // Default

/**
 * Class referenceImageDialog
 */
class ReferenceImageDialog : public wxDialog
{
	private:
		GLuint referenceID;
		size_t width;
		size_t height;
	protected:
		wxTextCtrl* pathTextCtrl;
		wxButton* openButton;
		wxStaticLine* mainStaticLine;
		wxStaticText* xStaticText;
		wxTextCtrl* xTextCtrl;
		wxStaticText* yStaticText;
		wxTextCtrl* yTextCtrl;
		wxStaticText* zStaticText;
		wxTextCtrl* zTextCtrl;
		wxStaticText* widthStaticText;
		wxTextCtrl* widthTextCtrl;
		wxStaticText* heightStaticText;
		wxTextCtrl* heightTextCtrl;
		wxButton* OKButton;
		wxButton* CancelButton;
	
	public:
		ReferenceImageDialog( wxWindow* parent, int id = -1, wxString title = wxT(""), wxPoint pos = wxDefaultPosition, wxSize size = wxDefaultSize, int style = wxDEFAULT_DIALOG_STYLE );
		void initialize();
		void onOpenButton(wxCommandEvent& event);
			void onOKButton(wxCommandEvent& event);
		void onCancelButton(wxCommandEvent& event);

		

		GLuint loadTexture(const char *fileName,enum wxBitmapType theType)
	{
		GLuint txtnumber;
		wxImage texture(_T(fileName),theType,-1);
		int theHeight=texture.GetHeight();
		int theWidth=texture.GetWidth();
		unsigned char *RGBData=texture.GetData();
		unsigned char *alphaData=texture.GetAlpha();
		size_t pixelCount=theHeight*theWidth;
		size_t dataSize=pixelCount*4;
		unsigned char *data=new unsigned char[dataSize];
		for(size_t i=0;i<pixelCount;++i)
		{
			data[i*4]=RGBData[i*3];
			data[i*4+1]=RGBData[i*3+1];
			data[i*4+2]=RGBData[i*3+2];
			data[i*4+3]=255;
		}
		glGenTextures(1, &txtnumber);
		glBindTexture(GL_TEXTURE_2D, txtnumber);
		glTexImage2D(GL_TEXTURE_2D, 0, 4, theWidth, theHeight, 0,GL_RGBA,GL_UNSIGNED_BYTE, data);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
		delete data;
		width=theWidth;
		height=theHeight;
		return txtnumber;
	};

	DECLARE_EVENT_TABLE()
};

enum
{
	onOpenButtonPressReferenceImage=234,
	onOKButtonPressReferenceImage,
	onCancelButtonPressReferenceImage
};