#include "info.hpp"

const static wxColour white(0xff, 0xff, 0xff);

const static wxString noneStr(wxT("(none)"));

InfoPanel::InfoPanel(wxWindow *parent, OpenSavePanel *openPanel) 
	: wxPanel(parent)
{
	this->openPanel = openPanel;

	wxPanel *lPanel = new wxPanel(this);
	wxPanel *rPanel = new wxPanel(this);
	
	wxPanel *sch    = CreateSchemePanel(lPanel);
	wxPanel *part1  = CreatePartPanel(lPanel, 0);
	wxPanel *part2  = CreatePartPanel(lPanel, 1);
	wxPanel *tot    = CreateTotSizePanel(rPanel);
	wxPanel *part3  = CreatePartPanel(rPanel, 2);
	wxPanel *part4  = CreatePartPanel(rPanel, 3);
	
	wxBoxSizer *sizer  = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer *lSizer = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer *rSizer = new wxBoxSizer(wxVERTICAL);
	
	lSizer->Add(sch  , 1, wxEXPAND | wxALL, 5);
  	lSizer->Add(part1, 1, wxEXPAND | wxALL, 5);
  	lSizer->Add(part2, 1, wxEXPAND | wxALL, 5);
  	rSizer->Add(tot  , 1, wxEXPAND | wxALL, 5);
  	rSizer->Add(part3, 1, wxEXPAND | wxALL, 5);
  	rSizer->Add(part4, 1, wxEXPAND | wxALL, 5);
  	sizer->Add(lPanel, 1, wxEXPAND);
  	sizer->Add(rPanel, 1, wxEXPAND);
  	
	lPanel->SetBackgroundColour(white);
	rPanel->SetBackgroundColour(white);
	
	lPanel->SetSizer(lSizer);
	rPanel->SetSizer(rSizer);
	
	this->SetSizer(sizer);
	this->SetBackgroundColour(white);
}

void InfoPanel::OnThreadCompleted()
{
	UpdateGUI();
}

wxString InfoPanel::GetTypeName(uint8_t type)
{
	wxString str;
	
	switch(type)
	{
	case 0x00:
		str = wxT("FREE");
		break;
	case 0x01:
		str = wxT("FAT12");
		break;
	case 0x04:
		str = wxT("FAT16");
		break;
	case 0x07:
		str = wxT("NTFS");
		break;
	case 0x0C:
		str = wxT("FAT32");
		break;
	case 0x0E:
		str = wxT("FAT16B");
		break;
	case 0x82:
		str = wxT("Swap");
		break;
	case 0x83:
		str = wxT("Linux");
		break;
	default:
		str = wxT("??");
		break;
	}
	
	return str;
}

void InfoPanel::UpdateGUI()
{
	bool valid, loaded;
	
	if (openPanel->IsValidImage(valid) < 0) {
		return;
	}
	
	if (openPanel->IsLoaded(loaded) < 0) {
		return;
	}
	
	int count = openPanel->GetPartitionCount();
	
	if (count < 0) {
		return;
	}
	
	if (!loaded)
	{
		scheme->SetValue(noneStr);
		scheme->Enable(false);
		
		totsize->SetValue(noneStr);
		totsize->Enable(false);
	
		for(auto i = 0; i < 4; i++)
		{
			partition[i]->SetValue(noneStr);
			partition[i]->Enable(false);
		}
	}
	else
	{
		scheme->Enable(true);
		totsize->Enable(true);
		
		struct OpenSavePanel::Partition data;
		wxString str;
		
		openPanel->GetPartition(0, data);
		
		if (data.isMBR) {
			str = wxT("MBR");
		}
		else if (data.isGPT) {
			str = wxT("GPT");
		}
		else {
			str = wxT("Unknown");
		}
		
		if (valid) {
			str += wxT(" (Valid) ");
		}
		else {
			str += wxT(" (Invalid) ");
		}
		
		scheme->SetValue(str);
		
		for(auto i = 0; i < 4; i++)
		{
			openPanel->GetPartition(i, data);
			
			if (data.isMBR)
			{
				str = GetTypeName(data.type);
			
				str += wxString::Format(wxT(" - S: %u"), data.lba);
				str += wxString::Format(wxT(" L: %u"), data.sectors);
			
				if (data.active) {
					str += wxT(" (A)");
				}
			}
			else
			{
				str = wxT("Unknown");
			}
			
			partition[i]->SetValue(str);
			partition[i]->Enable(true);
		}
	}
}

wxPanel *InfoPanel::CreatePartPanel(wxWindow *parent, int index)
{
	wxPanel *panel = new wxPanel(parent);
	
	wxBoxSizer *sizer = new wxBoxSizer(wxHORIZONTAL);
	
	wxString str = wxString::Format(wxT("Partition %d:"), index);
	
	wxStaticText *text = new wxStaticText(panel, wxID_ANY, str,
	                                      wxDefaultPosition, 
	                                      wxSize(80, -1));
	
	wxTextCtrl * part = new wxTextCtrl(panel, wxID_ANY, noneStr,
	                                   wxDefaultPosition, wxDefaultSize,
	                                   wxTE_READONLY);
	partition[index] = part;
	
	partition[index]->Enable(false);
	
	sizer->Add(text, 0, wxALIGN_CENTER);
  	sizer->Add(part, 1, wxLEFT | wxALIGN_CENTER, 5);
  	
  	panel->SetBackgroundColour(white);
	panel->SetSizer(sizer);
	
	return panel;
}

wxPanel *InfoPanel::CreateSchemePanel(wxWindow *parent)
{
	wxPanel *panel = new wxPanel(parent);
	
	wxBoxSizer *sizer = new wxBoxSizer(wxHORIZONTAL);
	
	wxString str = wxT("Scheme:");
	
	wxStaticText *text = new wxStaticText(panel, wxID_ANY, str, 
	                                      wxDefaultPosition, 
	                                      wxSize(80, -1));
	
	scheme = new wxTextCtrl(panel, wxID_ANY, noneStr,
	                        wxDefaultPosition, wxDefaultSize,
	                        wxTE_READONLY);
	                        
	scheme->Enable(false);
	
	sizer->Add(text  , 0, wxALIGN_CENTER);
  	sizer->Add(scheme, 1, wxLEFT | wxALIGN_CENTER, 5);
  	
  	panel->SetBackgroundColour(white);
	panel->SetSizer(sizer);
	
	return panel;
}

wxPanel *InfoPanel::CreateTotSizePanel(wxWindow *parent)
{
	wxPanel *panel = new wxPanel(parent);
	
	wxBoxSizer *sizer = new wxBoxSizer(wxHORIZONTAL);
	
	wxString str = wxT("Total Size:");
	
	wxStaticText *text = new wxStaticText(panel, wxID_ANY, str, 
	                                      wxDefaultPosition, 
	                                      wxSize(80, -1));
	
	totsize = new wxTextCtrl(panel, wxID_ANY, noneStr,
	                        wxDefaultPosition, wxDefaultSize,
	                        wxTE_READONLY);
	                        
	totsize->Enable(false);
	
	sizer->Add(text   , 0, wxALIGN_CENTER);
  	sizer->Add(totsize, 1, wxLEFT | wxALIGN_CENTER, 5);
  	
  	panel->SetBackgroundColour(white);
	panel->SetSizer(sizer);
	
	return panel;
}

