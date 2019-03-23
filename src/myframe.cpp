#include "myframe.hpp"
#include "../resources/about.xpm"
#include "../resources/accept.xpm"
#include "../resources/open.xpm"
#include "../resources/save.xpm"
#include "../resources/deny.xpm"
#include "../resources/dogtag.xpm"

const static wxString appName(wxT("SD Card Image Producer"));

const static wxString appVersion(wxT("0.2"));

const static wxColour white(0xff, 0xff, 0xff);

const static wxColour darkgrey(0x4f, 0x50, 0x49);

const static wxColour grey(0xd4, 0xd4, 0xd4);

const static int defaultPreset = PRESET_COREV2_FLASHING;

static struct Configuration memtypes[] =
{
	[MEM_TYPE_LPDDR2] = { .name = wxT("LPDDR2") },
	[MEM_TYPE_DDR3  ] = { .name = wxT("DDR3"  ) },
	[MEM_TYPE_LPDDR3] = { .name = wxT("LPDDR3") },
};

static struct Configuration memcaps[] =
{
	[MEM_CAP_128MB ] = { .name = wxT("128MB" ) },
	[MEM_CAP_256MB ] = { .name = wxT("256MB" ) },
	[MEM_CAP_512MB ] = { .name = wxT("512MB" ) },
	[MEM_CAP_1024MB] = { .name = wxT("1024MB") },
	[MEM_CAP_2048MB] = { .name = wxT("2048MB") },
};

static struct Configuration membanks[] =
{
	[MEM_BANK_SINGLE] = { .name = wxT("Single") },
	[MEM_BANK_DOUBLE] = { .name = wxT("Double") },
};

static struct Configuration memwidth[] =
{
	[MEM_WIDTH_16BITS] = { .name = wxT("16 bits") },
	[MEM_WIDTH_32BITS] = { .name = wxT("32 bits") },
};

static struct Configuration memvoltage[] =
{
	[MEM_VOLTAGE_1_00] = { .name = wxT("1.00V") },
	[MEM_VOLTAGE_1_05] = { .name = wxT("1.05V") },
	[MEM_VOLTAGE_1_10] = { .name = wxT("1.10V") },
	[MEM_VOLTAGE_1_15] = { .name = wxT("1.15V") },
	[MEM_VOLTAGE_1_20] = { .name = wxT("1.20V") },
	[MEM_VOLTAGE_1_25] = { .name = wxT("1.25V") },
	[MEM_VOLTAGE_1_30] = { .name = wxT("1.30V") },
	[MEM_VOLTAGE_1_35] = { .name = wxT("1.35V") },
	[MEM_VOLTAGE_1_40] = { .name = wxT("1.40V") },
	[MEM_VOLTAGE_1_45] = { .name = wxT("1.45V") },
	[MEM_VOLTAGE_1_50] = { .name = wxT("1.50V") },
	[MEM_VOLTAGE_1_55] = { .name = wxT("1.55V") },
	[MEM_VOLTAGE_1_60] = { .name = wxT("1.60V") },
	[MEM_VOLTAGE_1_65] = { .name = wxT("1.65V") },
	[MEM_VOLTAGE_1_70] = { .name = wxT("1.70V") },
	[MEM_VOLTAGE_1_75] = { .name = wxT("1.75V") },
	[MEM_VOLTAGE_1_80] = { .name = wxT("1.80V") },
	[MEM_VOLTAGE_1_85] = { .name = wxT("1.85V") },
	[MEM_VOLTAGE_1_90] = { .name = wxT("1.90V") },
	[MEM_VOLTAGE_1_95] = { .name = wxT("1.95V") },
	[MEM_VOLTAGE_2_00] = { .name = wxT("2.00V") },
};
	
static struct Configuration memfreq[] =
{
	[MEM_FREQ_156MHZ] = { .name = wxT("156MHz") },
	[MEM_FREQ_396MHZ] = { .name = wxT("396MHz") },
	[MEM_FREQ_528MHZ] = { .name = wxT("528MHz") },
	[MEM_FREQ_600MHZ] = { .name = wxT("600Mhz") },
};

static struct Configuration kversions[] =
{
	[KERN_VERSION3] = { .name = wxT("Kernel 3") },
	[KERN_VERSION4] = { .name = wxT("Kernel 4") },
};

static struct Configuration bootdisks[] =
{
	[DISK_SDCARD] = { .name = wxT("SD Card")       },
	[DISK_EMMC  ] = { .name = wxT("Internal EMMC") },
};

static struct Configuration execmodes[] =
{
	[MODE_NORMAL  ] = { .name = wxT("Normal")   },
	[MODE_FLASHING] = { .name = wxT("Flashing") },
};

static struct Preset mypresets[PRESET_COUNT] =
{
	[PRESET_COREV1_NORMAL] =
	{
		.name = wxT("CoreV1 Normal"),
		.memcfg =
		{
			[MEMCFG_TYPE ] = MEM_TYPE_DDR3,
			[MEMCFG_CAP  ] = MEM_CAP_2048MB,
			[MEMCFG_BANK ] = MEM_BANK_DOUBLE,
			[MEMCFG_BIT  ] = MEM_WIDTH_32BITS,
			[MEMCFG_VOLT ] = MEM_VOLTAGE_1_40,
			[MEMCFG_FREQ ] = MEM_FREQ_396MHZ,
		},
		.bootcfg =
		{
			[BOOTCFG_KVERSION] = KERN_VERSION3,
			[BOOTCFG_BOOTDISK] = DISK_SDCARD,
			[BOOTCFG_EXECMODE] = MODE_NORMAL,
		},
	},
	
	[PRESET_COREV1_FLASHING] =
	{
		.name = wxT("CoreV1 Flashing"),
		.memcfg =
		{
			[MEMCFG_TYPE ] = MEM_TYPE_DDR3,
			[MEMCFG_CAP  ] = MEM_CAP_2048MB,
			[MEMCFG_BANK ] = MEM_BANK_DOUBLE,
			[MEMCFG_BIT  ] = MEM_WIDTH_32BITS,
			[MEMCFG_VOLT ] = MEM_VOLTAGE_1_40,
			[MEMCFG_FREQ ] = MEM_FREQ_396MHZ,
		},
		.bootcfg =
		{
			[BOOTCFG_KVERSION] = KERN_VERSION3,
			[BOOTCFG_BOOTDISK] = DISK_SDCARD,
			[BOOTCFG_EXECMODE] = MODE_FLASHING,
		},
	},
	
	[PRESET_COREV2_NORMAL] =
	{
		.name = wxT("CoreV2 Normal"),
		.memcfg =
		{
			[MEMCFG_TYPE ] = MEM_TYPE_LPDDR3,
			[MEMCFG_CAP  ] = MEM_CAP_2048MB,
			[MEMCFG_BANK ] = MEM_BANK_DOUBLE,
			[MEMCFG_BIT  ] = MEM_WIDTH_32BITS,
			[MEMCFG_VOLT ] = MEM_VOLTAGE_1_25,
			[MEMCFG_FREQ ] = MEM_FREQ_528MHZ,
		},
		.bootcfg =
		{
			[BOOTCFG_KVERSION] = KERN_VERSION3,
			[BOOTCFG_BOOTDISK] = DISK_SDCARD,
			[BOOTCFG_EXECMODE] = MODE_NORMAL,
		},
	},
	
	[PRESET_COREV2_FLASHING] =
	{
		.name = wxT("CoreV2 Flashing"),
		.memcfg =
		{
			[MEMCFG_TYPE ] = MEM_TYPE_LPDDR3,
			[MEMCFG_CAP  ] = MEM_CAP_2048MB,
			[MEMCFG_BANK ] = MEM_BANK_DOUBLE,
			[MEMCFG_BIT  ] = MEM_WIDTH_32BITS,
			[MEMCFG_VOLT ] = MEM_VOLTAGE_1_25,
			[MEMCFG_FREQ ] = MEM_FREQ_528MHZ,
		},
		.bootcfg =
		{
			[BOOTCFG_KVERSION] = KERN_VERSION3,
			[BOOTCFG_BOOTDISK] = DISK_SDCARD,
			[BOOTCFG_EXECMODE] = MODE_FLASHING,
		},
	},
	
	[PRESET_CUSTOM] =
	{
		.name = wxT("Custom"),
	},
};

enum
{
    ID_ABOUT  = 1,
    ID_OPEN   = 2,
    ID_SAVE   = 3,
    ID_PRESET = 4,
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

wxPanel *MyFrame::CreateOpenPanel(wxPanel *parent)
{	
	wxPanel *panel = new wxPanel(parent);
	
	wxString str = wxT("Image filename:");
	
	wxStaticText *text = new wxStaticText(panel, wxID_ANY, str);
	
  	filename = new wxTextCtrl(panel, wxID_ANY, wxEmptyString,
  	                          wxDefaultPosition, wxDefaultSize,
  	                          wxTE_READONLY);
	         
	wxBitmapButton *button1 = new wxBitmapButton(panel, ID_OPEN, openBitmap);
	wxBitmapButton *button2 = new wxBitmapButton(panel, ID_SAVE, saveBitmap);

	wxBoxSizer *sizer = new wxBoxSizer(wxHORIZONTAL);
	
  	sizer->Add(text    , 0, wxALL | wxALIGN_CENTER, 5);
  	sizer->Add(filename, 1, wxALL | wxALIGN_CENTER, 5);
  	sizer->Add(button1 , 0, wxALL | wxALIGN_CENTER, 5);
  	sizer->Add(button2 , 0, wxALL | wxALIGN_CENTER, 5);
  	
	panel->SetBackgroundColour(white);
	panel->SetSizer(sizer);
	
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

wxPanel *MyFrame::CreatePresetPanel(wxPanel *parent)
{
	wxString choices[PRESET_COUNT];
	wxString str;
	
	wxPanel *panel = new wxPanel(parent);
	
	wxBoxSizer *sizer = new wxBoxSizer(wxHORIZONTAL);
	
	str = wxT("Configuration Preset:");
	
	for (int i = 0; i < PRESET_COUNT; i++) {
		choices[i] = mypresets[i].name;
	}
	
	wxStaticText *text = new wxStaticText(panel, wxID_ANY, str,
	                                      wxDefaultPosition, 
	                                      wxSize(155, -1));
	
	preset = new wxChoice(panel, wxID_ANY, wxDefaultPosition,
	                      wxDefaultSize, PRESET_COUNT, choices);
	
	sizer->Add(text  , 0, wxALIGN_CENTER);
  	sizer->Add(preset, 1, wxLEFT | wxALIGN_CENTER, 5);
  	
  	panel->SetBackgroundColour(white);
	panel->SetSizer(sizer);
	
	return panel;
}

wxPanel *MyFrame::CreateMemConfigPanel(wxPanel *parent, int index)
{
	wxString choices[21];
	wxString str;
	int n = 0;
	
	switch(index)
	{
	case MEMCFG_TYPE:
		str = wxT("Memory Type:");
		for (int i = 0; i < MEM_TYPE_COUNT; i++) {
			choices[i] = memtypes[i].name;
		}
		n = MEM_TYPE_COUNT;
		break;
		
	case MEMCFG_CAP:
		str = wxT("Memory Capacity:");
		for (int i = 0; i < MEM_CAP_COUNT; i++) {
			choices[i] = memcaps[i].name;
		}
		n = MEM_CAP_COUNT;
		break;
		
	case MEMCFG_BANK:
		str = wxT("Memory Bank:");
		for (int i = 0; i < MEM_BANK_COUNT; i++) {
			choices[i] = membanks[i].name;
		}
		n = MEM_BANK_COUNT;
		break;
		
	case MEMCFG_BIT:
		str = wxT("Memory Bus Width:");
		for (int i = 0; i < MEM_WIDTH_COUNT; i++) {
			choices[i] = memwidth[i].name;
		}
		n = MEM_WIDTH_COUNT;
		break;
		
	case MEMCFG_VOLT:
		str = wxT("Memory Voltage:");
		for (int i = 0; i < MEM_VOLTAGE_COUNT; i++) {
			choices[i] = memvoltage[i].name;
		}
		n = MEM_VOLTAGE_COUNT;
		break;
		
	case MEMCFG_FREQ:
		str = wxT("Memory Frequency:");
		for (int i = 0; i < MEM_FREQ_COUNT; i++) {
			choices[i] = memfreq[i].name;
		}
		n = MEM_FREQ_COUNT;
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
	sizer->Add(text, 0, wxALIGN_CENTER);
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
	case BOOTCFG_KVERSION:
		str = wxT("Kernel Version:");
		
		for (int i = 0; i < KERN_COUNT; i++) {
			choices[i] = kversions[i].name;
		}
		break;
		
	case BOOTCFG_BOOTDISK:
		str = wxT("Default Boot Disk:");
		
		for (int i = 0; i < DISK_COUNT; i++) {
			choices[i] = bootdisks[i].name;
		}
		break;
		
	case BOOTCFG_EXECMODE:
		str = wxT("Execution Mode:");
		
		for (int i = 0; i < MODE_COUNT; i++) {
			choices[i] = execmodes[i].name;
		}
		break;
		
	default:
		return NULL;
	}
	
	wxPanel *panel = new wxPanel(parent);

	wxBoxSizer *sizer = new wxBoxSizer(wxHORIZONTAL);
	
	wxStaticText *text = new wxStaticText(panel, wxID_ANY, str,
	                                      wxDefaultPosition, 
	                                      wxSize(155, -1));
	
	bootcfg[index] = new wxChoice(panel, wxID_ANY, wxDefaultPosition,
	                             wxDefaultSize, 2, choices);
	
	sizer->Add(text, 0, wxALIGN_CENTER);
  	sizer->Add(bootcfg[index], 1, wxLEFT | wxALIGN_CENTER, 5);
  	
  	panel->SetBackgroundColour(white);
	panel->SetSizer(sizer);
	
	return panel;
}

wxPanel *MyFrame::CreateConfigPanel(wxPanel *parent)
{	
	wxPanel *panel   = new wxPanel(parent);
	wxPanel *lPanel  = new wxPanel(panel);
	wxPanel *rPanel  = new wxPanel(panel);
	wxPanel *pPanel  = CreatePresetPanel(lPanel);
	
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

void MyFrame::UpdateGUIAccordingPreset(int preset)
{
	struct Preset *data = &mypresets[preset];
	
	this->preset->SetSelection(preset);
	
	memcfg[MEMCFG_TYPE]->SetSelection(data->memcfg[MEMCFG_TYPE]);
	memcfg[MEMCFG_CAP ]->SetSelection(data->memcfg[MEMCFG_CAP ]);
	memcfg[MEMCFG_BANK]->SetSelection(data->memcfg[MEMCFG_BANK]);
	memcfg[MEMCFG_BIT ]->SetSelection(data->memcfg[MEMCFG_BIT ]);
	memcfg[MEMCFG_VOLT]->SetSelection(data->memcfg[MEMCFG_VOLT]);
	memcfg[MEMCFG_FREQ]->SetSelection(data->memcfg[MEMCFG_FREQ]);
	
	bootcfg[BOOTCFG_KVERSION]->SetSelection(data->bootcfg[BOOTCFG_KVERSION]);
	bootcfg[BOOTCFG_BOOTDISK]->SetSelection(data->bootcfg[BOOTCFG_BOOTDISK]);
	bootcfg[BOOTCFG_EXECMODE]->SetSelection(data->bootcfg[BOOTCFG_EXECMODE]);
}

MyFrame::MyFrame(void) : 
	wxFrame(NULL, wxID_ANY, appName, wxDefaultPosition, wxSize(640, 480),
	        wxDEFAULT_FRAME_STYLE & ~(wxRESIZE_BORDER | wxMAXIMIZE_BOX))
{
	acceptBitmap = wxBitmap(accept_xpm);
	denyBitmap   = wxBitmap(deny_xpm);
	openBitmap   = wxBitmap(open_xpm);
	saveBitmap   = wxBitmap(save_xpm);
	aboutBitmap  = wxBitmap(about_xpm);
	dogtagBitmap = wxBitmap(dogtag_xpm);
    
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
  	
  	UpdateGUIAccordingPreset(defaultPreset);
}

