#include "config.hpp"

const static wxColour white(0xff, 0xff, 0xff);

ConfigPanel::ConfigPanel(wxWindow *parent) : wxPanel(parent)
{
	leftPanel = new wxPanel(this);
	rightPanel = new wxPanel(this);
	
	wxBoxSizer *leftSizer = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer *rightSizer = new wxBoxSizer(wxVERTICAL);
	
	leftPanel->SetSizer(leftSizer);
	rightPanel->SetSizer(rightSizer);
	
	leftPanel->SetBackgroundColour(white);
	rightPanel->SetBackgroundColour(white);
	
	int maxSize = 0;
	
	presetPanel = CreatePresetPanel(leftPanel, maxSize);
	
	leftSizer->Add(presetPanel, 1, wxEXPAND | wxALL, 5);
	
	for (auto i = 0; i < BOOTCFG_COUNT; i++)
	{
		bootcfgPanel[i] = CreateBootConfigPanel(leftPanel, maxSize, i);
		
		leftSizer->Add(bootcfgPanel[i], 1, wxEXPAND | wxALL, 5);
	}
	
	memcfgPanel[0] = CreateMemConfigPanel(leftPanel, maxSize, 0);
	
	leftSizer->Add(memcfgPanel[0], 1, wxEXPAND | wxALL, 5);
	
	for (auto i = 1; i < MEMCFG_COUNT; i++)
	{
		memcfgPanel[i] = CreateMemConfigPanel(rightPanel, maxSize, i);
		
		rightSizer->Add(memcfgPanel[i], 1, wxEXPAND | wxALL, 5);
	}
	
	presetText->SetMinSize(wxSize(maxSize, -1));
	
	for (auto i = 0; i < BOOTCFG_COUNT; i++)
	{
		bootcfgText[i]->SetMinSize(wxSize(maxSize, -1));
	}
	
	for (auto i = 0; i < MEMCFG_COUNT; i++)
	{
		memcfgText[i]->SetMinSize(wxSize(maxSize, -1));
	}
	
	wxBoxSizer *mainSizer = new wxBoxSizer(wxHORIZONTAL);
  	
  	mainSizer->Add(leftPanel, 1, wxEXPAND);
  	mainSizer->Add(rightPanel, 1, wxEXPAND);
  	
	this->SetBackgroundColour(white);
	this->SetSizer(mainSizer);
}

wxPanel *ConfigPanel::CreatePresetPanel(wxWindow *parent, int &maxSize)
{
	wxString choices[PRESET_COUNT];
	
	wxPanel *panel = new wxPanel(parent);
	
	wxBoxSizer *sizer = new wxBoxSizer(wxHORIZONTAL);
	
	wxString str = wxT("Configuration Preset:");
	
	for (int i = 0; i < PRESET_COUNT; i++) {
		choices[i] = mypresets[i].name;
	}
	
	presetText = new wxStaticText(panel, wxID_ANY, str);
	
	if (presetText->GetSize().GetWidth() > maxSize) {
		maxSize = presetText->GetSize().GetWidth();
	}
	
	presetChoice = 
		new wxChoice(panel, wxID_ANY, wxDefaultPosition, 
		wxDefaultSize, PRESET_COUNT, choices);
	
	sizer->Add(presetText, 0, wxALIGN_CENTER);
  	sizer->Add(presetChoice, 1, wxLEFT | wxALIGN_CENTER, 5);
  	
  	panel->SetBackgroundColour(white);
	panel->SetSizer(sizer);
	
	return panel;
}

wxPanel *ConfigPanel::CreateBootConfigPanel(wxWindow *parent, int &maxSize, int index)
{	
	wxString str(bootcfg[index].text);
	
	int nchoices = bootcfg[index].nchoices;
	
	wxString choices[nchoices];
	
	for (auto i = 0; i < nchoices; i++) {
		choices[i] = bootcfg[index].data[i].name;
	}
	
	wxPanel *panel = new wxPanel(parent);

	wxBoxSizer *sizer = new wxBoxSizer(wxHORIZONTAL);
	
	bootcfgText[index] = new wxStaticText(panel, wxID_ANY, str + wxT(":"));
	
	if (bootcfgText[index]->GetSize().GetWidth() > maxSize) {
		maxSize = bootcfgText[index]->GetSize().GetWidth();
	}
	
	bootcfgChoice[index] =
		new wxChoice(panel, wxID_ANY, wxDefaultPosition,
		wxDefaultSize, nchoices, choices);
	
	sizer->Add(bootcfgText[index], 0, wxALIGN_CENTER);
  	sizer->Add(bootcfgChoice[index], 1, wxLEFT | wxALIGN_CENTER, 5);
  	
  	panel->SetBackgroundColour(white);
	panel->SetSizer(sizer);
	
	return panel;
}

wxPanel *ConfigPanel::CreateMemConfigPanel(wxWindow *parent, int &maxSize, int index)
{
	wxString str(memcfg[index].text);
	
	int nchoices = memcfg[index].nchoices;
	
	wxString choices[nchoices];
	
	for (auto i = 0; i < nchoices; i++) {
		choices[i] = memcfg[index].data[i].name;
	}
	
	wxPanel *panel = new wxPanel(parent);
	
	wxBoxSizer *sizer = new wxBoxSizer(wxHORIZONTAL);
	
	memcfgText[index] = new wxStaticText(panel, wxID_ANY, str + wxT(":"));
	
	if (memcfgText[index]->GetSize().GetWidth() > maxSize) {
		maxSize = memcfgText[index]->GetSize().GetWidth();
	}
	
	memcfgChoice[index] = 
		new wxChoice(panel, wxID_ANY, wxDefaultPosition,
		wxDefaultSize, nchoices, choices);
	
	sizer->Add(memcfgText[index], 0, wxALIGN_CENTER);
  	sizer->Add(memcfgChoice[index], 1, wxLEFT | wxALIGN_CENTER, 5);
  	
  	panel->SetBackgroundColour(white);
	panel->SetSizer(sizer);
	
	return panel;
}

void ConfigPanel::UpdateAccordingPreset(int preset)
{
	const struct Preset *data = &mypresets[preset];
	
	this->presetChoice->SetSelection(preset);
	
	memcfgChoice[MEMCFG_TYPE]->SetSelection(data->memcfg[MEMCFG_TYPE]);
	memcfgChoice[MEMCFG_CAP ]->SetSelection(data->memcfg[MEMCFG_CAP ]);
	memcfgChoice[MEMCFG_BANK]->SetSelection(data->memcfg[MEMCFG_BANK]);
	memcfgChoice[MEMCFG_BIT ]->SetSelection(data->memcfg[MEMCFG_BIT ]);
	memcfgChoice[MEMCFG_VOLT]->SetSelection(data->memcfg[MEMCFG_VOLT]);
	memcfgChoice[MEMCFG_FREQ]->SetSelection(data->memcfg[MEMCFG_FREQ]);
	
	bootcfgChoice[BOOTCFG_KVERSION]->SetSelection(data->bootcfg[BOOTCFG_KVERSION]);
	bootcfgChoice[BOOTCFG_BOOTDISK]->SetSelection(data->bootcfg[BOOTCFG_BOOTDISK]);
	bootcfgChoice[BOOTCFG_EXECMODE]->SetSelection(data->bootcfg[BOOTCFG_EXECMODE]);
}

