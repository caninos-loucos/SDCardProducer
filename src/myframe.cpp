#include "myframe.hpp"
#include "enums.hpp"
#include "../resources/about.xpm"
#include "../resources/accept.xpm"
#include "../resources/deny.xpm"
#include "../resources/dogtag.xpm"

const static wxString appName(wxT("SD Card Image Producer"));

const static wxString appVersion(wxT("0.3"));

const static wxColour white(0xff, 0xff, 0xff);

const static wxColour darkgrey(0x4f, 0x50, 0x49);

const static wxColour grey(0xd4, 0xd4, 0xd4);

const static int defaultPreset = PRESET_COREV2_FLASHING;

// Icon made by Chanut from www.flaticon.com 

wxPanel *MyFrame::CreateAboutPanel(wxPanel *parent)
{
	wxPanel *panel    = new wxPanel(parent);
  	wxPanel *midPanel = new wxPanel(panel);
  	
  	wxStaticBitmap *dogtag = new wxStaticBitmap(panel, wxID_ANY, dogtagBitmap);
	filevalid = new wxStaticBitmap(panel, wxID_ANY, denyBitmap);
	
	wxString str = wxT("Version: ");
	
	wxStaticText *text = new wxStaticText(panel, wxID_ANY, str + appVersion);
  	
  	wxBitmapButton *button = new wxBitmapButton(panel, ID_ABOUT, aboutBitmap);
  	
  	wxBoxSizer *sizer = new wxBoxSizer(wxHORIZONTAL);
  	
  	sizer->Add(dogtag   , 0, wxALL, 5);
  	sizer->Add(text     , 0, wxALL | wxALIGN_BOTTOM, 5);
  	sizer->Add(midPanel , 1, wxALL, 5);
  	sizer->Add(filevalid, 0, wxALL | wxALIGN_CENTER, 5);
  	sizer->Add(button   , 0, wxALL | wxALIGN_CENTER, 5);
  	
  	panel->SetSizer(sizer);
  	panel->SetBackgroundColour(white);
  	midPanel->SetBackgroundColour(white);
  	
  	return panel;
}

wxPanel *MyFrame::CreatePartPanel(wxPanel *parent, int index)
{
	wxPanel *panel = new wxPanel(parent);
	
	wxBoxSizer *sizer = new wxBoxSizer(wxHORIZONTAL);
	
	wxString str = wxString::Format(wxT("Partition %d:"), index);
	
	wxStaticText *text = new wxStaticText(panel, wxID_ANY, str,
	                                      wxDefaultPosition, 
	                                      wxSize(80, -1));
	
	wxTextCtrl * part = new wxTextCtrl(panel, wxID_ANY, wxEmptyString,
	                                   wxDefaultPosition, wxDefaultSize,
	                                   wxTE_READONLY);
	partition[index] = part;
	
	sizer->Add(text, 0, wxALIGN_CENTER);
  	sizer->Add(part, 1, wxLEFT | wxALIGN_CENTER, 5);
  	
  	panel->SetBackgroundColour(white);
	panel->SetSizer(sizer);
	
	return panel;
}

wxPanel *MyFrame::CreateSchemePanel(wxPanel *parent)
{
	wxPanel *panel = new wxPanel(parent);
	
	wxBoxSizer *sizer = new wxBoxSizer(wxHORIZONTAL);
	
	wxString str = wxT("Scheme:");
	
	wxStaticText *text = new wxStaticText(panel, wxID_ANY, str, 
	                                      wxDefaultPosition, 
	                                      wxSize(80, -1));
	
	scheme = new wxTextCtrl(panel, wxID_ANY, wxEmptyString,
	                        wxDefaultPosition, wxDefaultSize,
	                        wxTE_READONLY);
	
	sizer->Add(text  , 0, wxALIGN_CENTER);
  	sizer->Add(scheme, 1, wxLEFT | wxALIGN_CENTER, 5);
  	
  	panel->SetBackgroundColour(white);
	panel->SetSizer(sizer);
	
	return panel;
}

wxPanel *MyFrame::CreateTotSizePanel(wxPanel *parent)
{
	wxPanel *panel = new wxPanel(parent);
	
	wxBoxSizer *sizer = new wxBoxSizer(wxHORIZONTAL);
	
	wxString str = wxT("Total Size:");
	
	wxStaticText *text = new wxStaticText(panel, wxID_ANY, str, 
	                                      wxDefaultPosition, 
	                                      wxSize(80, -1));
	
	totsize = new wxTextCtrl(panel, wxID_ANY, wxEmptyString,
	                        wxDefaultPosition, wxDefaultSize,
	                        wxTE_READONLY);
	
	sizer->Add(text   , 0, wxALIGN_CENTER);
  	sizer->Add(totsize, 1, wxLEFT | wxALIGN_CENTER, 5);
  	
  	panel->SetBackgroundColour(white);
	panel->SetSizer(sizer);
	
	return panel;
}

wxPanel *MyFrame::CreateInfoPanel(wxPanel *parent)
{	
	// Create Panels
	wxPanel *panel  = new wxPanel(parent);
	wxPanel *lPanel = new wxPanel(panel);
	wxPanel *rPanel = new wxPanel(panel);
	wxPanel *sch    = CreateSchemePanel(lPanel);
	wxPanel *part1  = CreatePartPanel(lPanel, 0);
	wxPanel *part2  = CreatePartPanel(lPanel, 1);
	wxPanel *tot    = CreateTotSizePanel(rPanel);
	wxPanel *part3  = CreatePartPanel(rPanel, 2);
	wxPanel *part4  = CreatePartPanel(rPanel, 3);
	
	// Create Sizers
	wxBoxSizer *sizer  = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer *lSizer = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer *rSizer = new wxBoxSizer(wxVERTICAL);
	
	// Configure Sizers
	lSizer->Add(sch  , 1, wxEXPAND | wxALL, 5);
  	lSizer->Add(part1, 1, wxEXPAND | wxALL, 5);
  	lSizer->Add(part2, 1, wxEXPAND | wxALL, 5);
  	rSizer->Add(tot  , 1, wxEXPAND | wxALL, 5);
  	rSizer->Add(part3, 1, wxEXPAND | wxALL, 5);
  	rSizer->Add(part4, 1, wxEXPAND | wxALL, 5);
  	sizer->Add(lPanel, 1, wxEXPAND);
  	sizer->Add(rPanel, 1, wxEXPAND);
  	
	// Configure Panels
	panel->SetBackgroundColour(white);
	lPanel->SetBackgroundColour(white);
	rPanel->SetBackgroundColour(white);
	lPanel->SetSizer(lSizer);
	rPanel->SetSizer(rSizer);
	panel->SetSizer(sizer);
	
	return panel;
}

MyFrame::MyFrame(void) : 
	wxFrame(NULL, wxID_ANY, appName, wxDefaultPosition, wxSize(640, 480),
	        wxDEFAULT_FRAME_STYLE & ~(wxRESIZE_BORDER | wxMAXIMIZE_BOX))
{
	acceptBitmap = wxBitmap(accept_xpm);
	denyBitmap   = wxBitmap(deny_xpm);
	
	aboutBitmap  = wxBitmap(about_xpm);
	dogtagBitmap = wxBitmap(dogtag_xpm);
    
	mainPanel = new wxPanel(this);
  	mainPanel->SetBackgroundColour(darkgrey);
 
  	
  	aboutPanel = CreateAboutPanel (mainPanel);
  	infoPanel  = CreateInfoPanel  (mainPanel);
  	
  	
  	openPanel = new OpenSavePanel(mainPanel);
  	
  	cfgPanel = new ConfigPanel(mainPanel);
  	
  	cfgPanel->UpdateAccordingPreset(PRESET_COREV2_FLASHING);
  	
  	wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);
  	
    sizer->Add(aboutPanel, 0, wxEXPAND | wxALL, 2);
    sizer->Add(openPanel , 0, wxEXPAND | (wxALL & ~(wxTOP)), 2);
    sizer->Add(infoPanel , 1, wxEXPAND | (wxALL & ~(wxTOP)), 2);
    sizer->Add(cfgPanel  , 1, wxEXPAND | (wxALL & ~(wxTOP)), 2);
    
  	mainPanel->SetSizer(sizer);
}

