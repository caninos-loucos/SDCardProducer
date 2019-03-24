#include "about.hpp"
#include "../resources/about.xpm"
#include "../resources/accept.xpm"
#include "../resources/deny.xpm"
#include "../resources/dogtag.xpm"

const static wxString appVersion(wxT("0.4"));

const static wxColour white(0xff, 0xff, 0xff);

void AboutPanel::OnThreadCompleted()
{
	bool valid;
	
	if (openPanel->IsValidImage(valid) < 0) {
		return;
	}
	
	if (valid) {
		filevalid->SetBitmap(acceptBitmap);
	}
	else {
		filevalid->SetBitmap(denyBitmap);
	}
}

AboutPanel::AboutPanel(wxWindow *parent, OpenSavePanel *openPanel)
	: wxPanel(parent)
{
	this->openPanel = openPanel;
	
	acceptBitmap = wxBitmap(accept_xpm);
	denyBitmap   = wxBitmap(deny_xpm);
	aboutBitmap  = wxBitmap(about_xpm);
	dogtagBitmap = wxBitmap(dogtag_xpm);

  	wxPanel *midPanel = new wxPanel(this);
  	
  	wxStaticBitmap *dogtag = new wxStaticBitmap(this, wxID_ANY, dogtagBitmap);
  	
	filevalid = new wxStaticBitmap(this, wxID_ANY, denyBitmap);
	
	wxString str = wxT("Version: ");
	
	wxStaticText *text = new wxStaticText(this, wxID_ANY, str + appVersion);
  	
  	wxBitmapButton *button = new wxBitmapButton(this, wxID_ANY, aboutBitmap);
  	
  	wxBoxSizer *sizer = new wxBoxSizer(wxHORIZONTAL);
  	
  	sizer->Add(dogtag   , 0, wxALL, 5);
  	sizer->Add(text     , 0, wxALL | wxALIGN_BOTTOM, 5);
  	sizer->Add(midPanel , 1, wxALL, 5);
  	sizer->Add(filevalid, 0, wxALL | wxALIGN_CENTER, 5);
  	sizer->Add(button   , 0, wxALL | wxALIGN_CENTER, 5);
  	
  	this->SetSizer(sizer);
  	this->SetBackgroundColour(white);
  	midPanel->SetBackgroundColour(white);
}

