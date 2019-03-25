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
	
	Bind(wxEVT_CHOICE, &ConfigPanel::OnChoice, this, wxID_ANY);
}

void ConfigPanel::OnChoice(wxCommandEvent& event)
{
	if (event.GetId() == presetChoice->GetId())
	{
		int selection = presetChoice->GetSelection();
		
		if (selection == PRESET_CUSTOM)
		{
			selection = MatchPreset();
			
			presetChoice->SetSelection(selection);
		}
		
		UpdateAccordingPreset(selection);
	}
	else
	{
		presetChoice->SetSelection(MatchPreset());
	}
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

int ConfigPanel::MatchPreset()
{
	for (auto i = 0; i < PRESET_COUNT; i++)
	{
		if (i != PRESET_CUSTOM)
		{
			const struct Preset *data = &mypresets[i];
			
			bool match = true;
			int selection;
			
			selection = memcfgChoice[MEMCFG_TYPE]->GetSelection();
			match &= (selection == data->memcfg[MEMCFG_TYPE]);
			
			selection = memcfgChoice[MEMCFG_CAP]->GetSelection();
			match &= (selection == data->memcfg[MEMCFG_CAP]);
			
			selection = memcfgChoice[MEMCFG_BANK]->GetSelection();
			match &= (selection == data->memcfg[MEMCFG_BANK]);
			
			selection = memcfgChoice[MEMCFG_BIT]->GetSelection();
			match &= (selection == data->memcfg[MEMCFG_BIT]);
			
			selection = memcfgChoice[MEMCFG_VOLT]->GetSelection();
			match &= (selection == data->memcfg[MEMCFG_VOLT]);
			
			selection = memcfgChoice[MEMCFG_FREQ]->GetSelection();
			match &= (selection == data->memcfg[MEMCFG_FREQ]);
	
			selection = bootcfgChoice[BOOTCFG_KVERSION]->GetSelection();
			match &= (selection == data->bootcfg[BOOTCFG_KVERSION]);
			
			selection = bootcfgChoice[BOOTCFG_BOOTDISK]->GetSelection();
			match &= (selection == data->bootcfg[BOOTCFG_BOOTDISK]);
			
			selection = bootcfgChoice[BOOTCFG_EXECMODE]->GetSelection();
			match &= (selection == data->bootcfg[BOOTCFG_EXECMODE]);
			
			if (match) {
				return i;
			}
		}
	}
	
	return PRESET_CUSTOM;
}

void ConfigPanel::UpdateGUI()
{
	presetChoice->Enable(true);
	
	memcfgChoice[MEMCFG_TYPE]->Enable(true);
	memcfgChoice[MEMCFG_CAP ]->Enable(true);
	memcfgChoice[MEMCFG_BANK]->Enable(true);
	memcfgChoice[MEMCFG_BIT ]->Enable(true);
	memcfgChoice[MEMCFG_VOLT]->Enable(true);
	memcfgChoice[MEMCFG_FREQ]->Enable(true);
	
	bootcfgChoice[BOOTCFG_KVERSION]->Enable(true);
	bootcfgChoice[BOOTCFG_BOOTDISK]->Enable(true);
	bootcfgChoice[BOOTCFG_EXECMODE]->Enable(true);
}
	
void ConfigPanel::DisableGUI()
{
	presetChoice->Enable(false);
	
	memcfgChoice[MEMCFG_TYPE]->Enable(false);
	memcfgChoice[MEMCFG_CAP ]->Enable(false);
	memcfgChoice[MEMCFG_BANK]->Enable(false);
	memcfgChoice[MEMCFG_BIT ]->Enable(false);
	memcfgChoice[MEMCFG_VOLT]->Enable(false);
	memcfgChoice[MEMCFG_FREQ]->Enable(false);
	
	bootcfgChoice[BOOTCFG_KVERSION]->Enable(false);
	bootcfgChoice[BOOTCFG_BOOTDISK]->Enable(false);
	bootcfgChoice[BOOTCFG_EXECMODE]->Enable(false);
}

void ConfigPanel::UpdateAccordingPreset(int preset)
{
	if (preset == PRESET_CUSTOM) {
		return;
	}

	const struct Preset *data = &mypresets[preset];
	
	presetChoice->SetSelection(preset);
	
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

