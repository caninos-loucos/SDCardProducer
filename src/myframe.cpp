#include "myframe.hpp"

const static wxString appName(wxT("SD Card Image Producer"));

const static wxString appVersion(wxT("0.1"));

const static wxColour white(0xff, 0xff, 0xff);

const static wxColour darkgrey(0x4f, 0x50, 0x49);

const static wxColour grey(0xd4, 0xd4, 0xd4);

enum
{
    ID_ABOUT = 1,
    ID_OPEN  = 2,
    ID_SAVE  = 3,
};

// Icon made by Chanut from www.flaticon.com 

MyFrame::~MyFrame()
{
	///
}

wxPanel *MyFrame::CreateAboutPanel(wxPanel *parent)
{
	wxPanel *panel    = new wxPanel(parent);
  	wxPanel *midPanel = new wxPanel(panel);
  	
  	wxStaticBitmap *dogtag = new wxStaticBitmap(panel, wxID_ANY, dogtagBitmap);
	fileValid = new wxStaticBitmap(panel, wxID_ANY, denyBitmap);
	
	wxString str = wxT("Version: ");
	wxStaticText *text = new wxStaticText(panel, wxID_ANY, str + appVersion);
  	
  	// Create Button
  	
  	wxBitmapButton *button = new wxBitmapButton(panel, ID_ABOUT, aboutBitmap);
  	
  	// Create Sizer
  	
  	wxBoxSizer *sizer = new wxBoxSizer(wxHORIZONTAL);
  	
  	// Configure Sizer
  	
  	sizer->Add(dogtag   , 0, wxALL, 5);
  	sizer->Add(text     , 0, wxALL | wxALIGN_BOTTOM, 5);
  	sizer->Add(midPanel , 1, wxALL, 5);
  	sizer->Add(fileValid, 0, wxALL | wxALIGN_CENTER, 5);
  	sizer->Add(button   , 0, wxALL | wxALIGN_CENTER, 5);
  	
  	// Configure Panels
  	
  	panel->SetSizer(sizer);
  	panel->SetBackgroundColour(white);
  	midPanel->SetBackgroundColour(white);
  	
  	return panel;
}

wxPanel *MyFrame::CreateOpenPanel(wxPanel *parent)
{
	wxString str;
	
	// Create Panel
	wxPanel *panel = new wxPanel(parent);
	
	// Create Static
	str = wxT("Image filename:");
	wxStaticText *text = new wxStaticText(panel, wxID_ANY, str);
	
	// Create Text Control
  	inFilename = new wxTextCtrl(panel, wxID_ANY, wxEmptyString,
  	                            wxDefaultPosition, wxDefaultSize,
  	                            wxTE_READONLY);
	
  	// Create Button          
	wxBitmapButton *button1 = new wxBitmapButton(panel, ID_OPEN, openBitmap);
	wxBitmapButton *button2 = new wxBitmapButton(panel, ID_SAVE, saveBitmap);

	// Create Sizer
	wxBoxSizer *sizer = new wxBoxSizer(wxHORIZONTAL);
	
	// Configure Sizer
  	sizer->Add(text      , 0, wxALL | wxALIGN_CENTER, 5);
  	sizer->Add(inFilename, 1, wxALL | wxALIGN_CENTER, 5);
  	sizer->Add(button1   , 0, wxALL | wxALIGN_CENTER, 5);
  	sizer->Add(button2   , 0, wxALL | wxALIGN_CENTER, 5);
  	
	// Configure Panels
	panel->SetBackgroundColour(white);
	panel->SetSizer(sizer);
	
	return panel;
}

wxPanel *MyFrame::CreatePartPanel(wxPanel *parent, int index)
{
	wxString str;
	
	// Create Panel
	wxPanel *panel = new wxPanel(parent);
	
	// Create Sizers
	wxBoxSizer *sizer = new wxBoxSizer(wxHORIZONTAL);
	
	// Create Static
	str = wxString::Format(wxT("Partition %d:"), index);
	wxStaticText *text = new wxStaticText(panel, wxID_ANY, str,
	                                      wxDefaultPosition, 
	                                      wxSize(80, -1));
	
	// Create Text Control
	wxTextCtrl * part = new wxTextCtrl(panel, wxID_ANY, wxEmptyString,
	                                   wxDefaultPosition, wxDefaultSize,
	                                   wxTE_READONLY);
	partition[index] = part;
	
	// Configure Sizer
	sizer->Add(text, 0, wxALIGN_CENTER);
  	sizer->Add(part, 1, wxLEFT | wxALIGN_CENTER, 5);
  	
  	// Configure Panel
  	panel->SetBackgroundColour(white);
	panel->SetSizer(sizer);
	
	return panel;
}

wxPanel *MyFrame::CreateSchemePanel(wxPanel *parent)
{
	wxString str;
	
	// Create Panel
	wxPanel *panel = new wxPanel(parent);
	
	// Create Sizers
	wxBoxSizer *sizer = new wxBoxSizer(wxHORIZONTAL);
	
	// Create Static
	str = wxT("Scheme:");
	wxStaticText *text = new wxStaticText(panel, wxID_ANY, str, 
	                                      wxDefaultPosition, 
	                                      wxSize(80, -1));
	
	// Create Text Control
	scheme = new wxTextCtrl(panel, wxID_ANY, wxEmptyString,
	                        wxDefaultPosition, wxDefaultSize,
	                        wxTE_READONLY);
	
	// Configure Sizer
	sizer->Add(text  , 0, wxALIGN_CENTER);
  	sizer->Add(scheme, 1, wxLEFT | wxALIGN_CENTER, 5);
  	
  	// Configure Panel
  	panel->SetBackgroundColour(white);
	panel->SetSizer(sizer);
	
	return panel;
}

wxPanel *MyFrame::CreateTotSizePanel(wxPanel *parent)
{
	wxString str;
	
	// Create Panel
	wxPanel *panel = new wxPanel(parent);
	
	// Create Sizers
	wxBoxSizer *sizer = new wxBoxSizer(wxHORIZONTAL);
	
	// Create Static
	str = wxT("Total Size:");
	wxStaticText *text = new wxStaticText(panel, wxID_ANY, str, 
	                                      wxDefaultPosition, 
	                                      wxSize(80, -1));
	
	// Create Text Control
	totsize = new wxTextCtrl(panel, wxID_ANY, wxEmptyString,
	                        wxDefaultPosition, wxDefaultSize,
	                        wxTE_READONLY);
	
	// Configure Sizer
	sizer->Add(text   , 0, wxALIGN_CENTER);
  	sizer->Add(totsize, 1, wxLEFT | wxALIGN_CENTER, 5);
  	
  	// Configure Panel
  	panel->SetBackgroundColour(white);
	panel->SetSizer(sizer);
	
	return panel;
}

wxPanel *MyFrame::CreateInfoPanel(wxPanel *parent)
{
	wxString str;
	
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

wxPanel *MyFrame::CreatePresetPanel(wxPanel *parent)
{
	wxString choices[5];
	wxString str;
	
	wxPanel *panel = new wxPanel(parent);
	
	wxBoxSizer *sizer = new wxBoxSizer(wxHORIZONTAL);
	
	str = wxT("Configuration Preset:");
	
	choices[0] = wxT("CoreV1 Normal");
	choices[1] = wxT("CoreV1 Flashing");
	choices[2] = wxT("CoreV2 Normal");
	choices[3] = wxT("CoreV2 Flashing");
	choices[4] = wxT("Custom");
	
	wxStaticText *text = new wxStaticText(panel, wxID_ANY, str,
	                                      wxDefaultPosition, 
	                                      wxSize(155, -1));
	
	// Create Text Control
	preset = new wxChoice(panel, wxID_ANY, wxDefaultPosition,
	                      wxDefaultSize, 5, choices);
	
	// Configure Sizer
	sizer->Add(text  , 0, wxALIGN_CENTER);
  	sizer->Add(preset, 1, wxLEFT | wxALIGN_CENTER, 5);
  	
  	// Configure Panel
  	panel->SetBackgroundColour(white);
	panel->SetSizer(sizer);
	
	return panel;
}

wxPanel *MyFrame::CreateMemConfigPanel(wxPanel *parent, int index)
{
	wxString choices[16];
	wxString str;
	int n = 0;
	
	switch(index)
	{
	case MEMTYPE:
		str = wxT("Memory Type:");
		choices[0] = wxT("LPDDR2");
		choices[1] = wxT("DDR3");
		choices[2] = wxT("LPDDR3");
		n = 3;
		break;
		
	case MEMCAP:
		str = wxT("Memory Capacity:");
		choices[0] = wxT("128MB");
		choices[1] = wxT("256MB");
		choices[2] = wxT("512MB");
		choices[3] = wxT("756MB");
		choices[4] = wxT("1024MB");
		choices[5] = wxT("2048MB");
		n = 6;
		break;
		
	case MEMBANK:
		str = wxT("Memory Banks:");
		choices[0] = wxT("1 bank");
		choices[1] = wxT("2 banks");
		n = 2;
		break;
		
	case MEMBIT:
		str = wxT("Memory Bit Width:");
		choices[0] = wxT("16 bits");
		choices[1] = wxT("32 bits");
		n = 2;
		break;
		
	case MEMVOLT:
		str = wxT("Memory Voltage:");
		choices[0x0] = wxT("1.00V");
		choices[0x1] = wxT("1.05V");
		choices[0x2] = wxT("1.10V");
		choices[0x3] = wxT("1.15V");
		choices[0x4] = wxT("1.20V");
		choices[0x5] = wxT("1.25V");
		choices[0x6] = wxT("1.30V");
		choices[0x7] = wxT("1.35V");
		choices[0x8] = wxT("1.40V");
		choices[0x9] = wxT("1.45V");
		choices[0xA] = wxT("1.50V");
		choices[0xB] = wxT("1.55V");
		choices[0xC] = wxT("1.60V");
		choices[0xD] = wxT("1.65V");
		choices[0xE] = wxT("1.70V");
		choices[0xF] = wxT("1.75V");
		n = 16;
		break;
		
	case MEMFREQ:
		str = wxT("Memory Frequency:");
		choices[0x0] = wxT("156MHz");
		choices[0x1] = wxT("396MHz");
		choices[0x2] = wxT("528MHz");
		choices[0x3] = wxT("600Mhz");
		n = 4;
		break;
		
	default:
		return NULL;
	}
	
	wxPanel *panel = new wxPanel(parent);
	
	wxBoxSizer *sizer = new wxBoxSizer(wxHORIZONTAL);
	
	wxStaticText *text = new wxStaticText(panel, wxID_ANY, str,
	                                      wxDefaultPosition, 
	                                      wxSize(155, -1));
	
	memcfg[index] = new wxChoice(panel, wxID_ANY, wxDefaultPosition,
	                             wxDefaultSize, n, choices);
	
	// Configure Sizer
	sizer->Add(text   , 0, wxALIGN_CENTER);
  	sizer->Add(memcfg[index], 1, wxLEFT | wxALIGN_CENTER, 5);
  	
  	// Configure Panel
  	panel->SetBackgroundColour(white);
	panel->SetSizer(sizer);
	
	return panel;
}

wxPanel *MyFrame::CreateBootConfigPanel(wxPanel *parent, int index)
{
	wxString choices[2];
	wxString str;
	
	switch(index)
	{
	case KVERSION:
		str = wxT("Kernel Version:");
		choices[0] = wxT("Kernel 3");
		choices[1] = wxT("Kernel 4");
		break;
		
	case BOOTDISK:
		str = wxT("Default Boot Disk:");
		choices[0] = wxT("SD Card");
		choices[1] = wxT("Internal EMMC");
		break;
		
	case EXECMODE:
		str = wxT("Execution Mode:");
		choices[0] = wxT("Normal");
		choices[1] = wxT("Flashing");
		break;
		
	default:
		return NULL;
	}
	
	wxPanel *panel = new wxPanel(parent);

	wxBoxSizer *sizer = new wxBoxSizer(wxHORIZONTAL);
	
	wxStaticText *text = new wxStaticText(panel, wxID_ANY, str,
	                                      wxDefaultPosition, 
	                                      wxSize(155, -1));
	
	memcfg[index] = new wxChoice(panel, wxID_ANY, wxDefaultPosition,
	                             wxDefaultSize, 2, choices);
	
	sizer->Add(text, 0, wxALIGN_CENTER);
  	sizer->Add(memcfg[index], 1, wxLEFT | wxALIGN_CENTER, 5);
  	
  	panel->SetBackgroundColour(white);
	panel->SetSizer(sizer);
	
	return panel;
}

wxPanel *MyFrame::CreateConfigPanel(wxPanel *parent)
{
	wxString str;
	
	// Create Panels
	wxPanel *panel   = new wxPanel(parent);
	wxPanel *lPanel  = new wxPanel(panel);
	wxPanel *rPanel  = new wxPanel(panel);
	wxPanel *pPanel  = CreatePresetPanel(lPanel);
	
	// Create Sizers
	wxBoxSizer *sizer  = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer *lSizer = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer *rSizer = new wxBoxSizer(wxVERTICAL);
	
	// Configure Sizers
	lSizer->Add(pPanel , 1, wxEXPAND | wxALL, 5);
	
	for (int i = 0; i < 3; i++)
	{
		lSizer->Add(CreateBootConfigPanel(lPanel, i), 1, wxEXPAND | wxALL, 5);
	}
	
	lSizer->Add(CreateMemConfigPanel(lPanel, 0), 1, wxEXPAND | wxALL, 5);
	
	for (int i = 1; i < 6; i++)
	{
		rSizer->Add(CreateMemConfigPanel(rPanel, i), 1, wxEXPAND | wxALL, 5);
	}
  	
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
	acceptBitmap.LoadFile(wxT("accept.bmp"));
	denyBitmap.LoadFile(wxT("deny.bmp"));
	openBitmap.LoadFile(wxT("open.bmp"));
	saveBitmap.LoadFile(wxT("save.bmp"));
	aboutBitmap.LoadFile(wxT("about.bmp"));
	dogtagBitmap.LoadFile(wxT("dogtag.bmp"));
    
    //
    
	mainPanel = new wxPanel(this);
  	mainPanel->SetBackgroundColour(darkgrey);
  	
  	//
  	
  	aboutPanel = CreateAboutPanel (mainPanel);
  	openPanel  = CreateOpenPanel  (mainPanel);
  	infoPanel  = CreateInfoPanel  (mainPanel);
  	cfgPanel   = CreateConfigPanel(mainPanel);
  	
  	wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);
  	
    sizer->Add(aboutPanel, 0, wxEXPAND | wxALL, 2);
    sizer->Add(openPanel , 0, wxEXPAND | (wxALL & ~(wxTOP)), 2);
    sizer->Add(infoPanel , 1, wxEXPAND | (wxALL & ~(wxTOP)), 2);
    sizer->Add(cfgPanel  , 1, wxEXPAND | (wxALL & ~(wxTOP)), 2);
    
  	mainPanel->SetSizer(sizer);
}

