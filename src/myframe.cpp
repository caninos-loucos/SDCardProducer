#include "myframe.hpp"
#include "enums.hpp"

const static wxString appName(wxT("SD Card Image Producer"));

const static wxColour darkgrey(0x4f, 0x50, 0x49);

const static int defaultPreset = PRESET_COREV2_FLASHING;

void MyFrame::DisableGUI()
{
	aboutPanel->DisableGUI();
	infoPanel->DisableGUI();
	cfgPanel->DisableGUI();
}

void MyFrame::UpdateGUI()
{	
	infoPanel->UpdateGUI();
	aboutPanel->UpdateGUI();
	cfgPanel->UpdateGUI();
}

MyFrame::MyFrame(void) : 
	wxFrame(NULL, wxID_ANY, appName, wxDefaultPosition, wxSize(740, 480),
	        wxDEFAULT_FRAME_STYLE & ~(wxRESIZE_BORDER | wxMAXIMIZE_BOX))
{
  	this->SetBackgroundColour(darkgrey);
  	
  	openPanel = new OpenSavePanel(this);
  	
  	infoPanel = new InfoPanel(this, openPanel);
  	cfgPanel = new ConfigPanel(this);
  	aboutPanel = new AboutPanel(this, openPanel);
  	
  	cfgPanel->UpdateAccordingPreset(defaultPreset);
  	
  	wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);
  	
    sizer->Add(aboutPanel, 0, wxEXPAND | wxALL, 2);
    sizer->Add(openPanel , 0, wxEXPAND | (wxALL & ~(wxTOP)), 2);
    sizer->Add(infoPanel , 1, wxEXPAND | (wxALL & ~(wxTOP)), 2);
    sizer->Add(cfgPanel  , 1, wxEXPAND | (wxALL & ~(wxTOP)), 2);
    
  	this->SetSizer(sizer);
}

