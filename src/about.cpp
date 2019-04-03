#include "about.hpp"
#include "../resources/about.xpm"
#include "../resources/accept.xpm"
#include "../resources/deny.xpm"
#include "../resources/dogtag.xpm"
#include "../resources/cancel.xpm"

const static wxString appVersion(wxT("0.12"));

const static wxColour white(0xff, 0xff, 0xff);

// Icon made by Chanut from www.flaticon.com

void AboutPanel::DisableGUI()
{	
	cancelButton->Enable(false);
	aboutButton->Enable(false);
}

void AboutPanel::UpdateGUI()
{
	bool valid, loaded;
	
	aboutButton->Enable(true);
	
	if (openPanel->IsValidImage(valid) < 0) {
		return;
	}
	
	if (openPanel->IsLoaded(loaded) < 0) {
		return;
	}
	
	if (valid) {
		filevalid->SetBitmap(acceptBitmap);
	}
	else {
		filevalid->SetBitmap(denyBitmap);
	}
	
	if (loaded) {
		cancelButton->Enable(true);
	}
	else {
		cancelButton->Enable(false);
	}
}

void AboutPanel::OnCancel(wxCommandEvent& event)
{
	openPanel->CloseImage();
}

AboutPanel::AboutPanel(wxWindow *parent, OpenSavePanel *openPanel)
	: wxPanel(parent)
{
	this->openPanel = openPanel;
	
	acceptBitmap = wxBitmap(accept_xpm);
	denyBitmap   = wxBitmap(deny_xpm);
	aboutBitmap  = wxBitmap(about_xpm);
	dogtagBitmap = wxBitmap(dogtag_xpm);
	cancelBitmap = wxBitmap(cancel_xpm);

  	wxPanel *midPanel = new wxPanel(this);
  	
  	wxStaticBitmap *dogtag = new wxStaticBitmap(this, wxID_ANY, dogtagBitmap);
  	
	filevalid = new wxStaticBitmap(this, wxID_ANY, denyBitmap);
	
	wxString str = wxT("Version: ");
	
	wxStaticText *text = new wxStaticText(this, wxID_ANY, str + appVersion);
	
	cancelButton = new wxBitmapButton(this, wxID_ANY, cancelBitmap);
  	
  	aboutButton = new wxBitmapButton(this, wxID_ANY, aboutBitmap);
  	
  	wxBoxSizer *sizer = new wxBoxSizer(wxHORIZONTAL);
  	
  	sizer->Add(dogtag, 0, wxALL, 5);
  	sizer->Add(text, 0, wxALL | wxALIGN_BOTTOM, 5);
  	sizer->Add(midPanel, 1, wxALL, 5);
  	sizer->Add(filevalid, 0, wxALL | wxALIGN_CENTER, 5);
  	sizer->Add(aboutButton, 0, wxALL | wxALIGN_CENTER, 5);
  	sizer->Add(cancelButton, 0, wxALL | wxALIGN_CENTER, 5);
  	
  	this->SetSizer(sizer);
  	this->SetBackgroundColour(white);
  	midPanel->SetBackgroundColour(white);
  	
  	cancelButton->Enable(false);
  	
  	Bind(wxEVT_BUTTON, &AboutPanel::OnCancel, this, cancelButton->GetId());
}

