#include "imgfile.hpp"
#include "../resources/open.xpm"
#include "../resources/save.xpm"

const static wxColour white(0xff, 0xff, 0xff);

OpenSavePanel::OpenSavePanel(wxWindow *parent) : wxPanel(parent)
{
	openBitmap = wxBitmap(open_xpm);
	saveBitmap = wxBitmap(save_xpm);
	
	wxString str = wxT("Image filename:");
	
	wxStaticText *text = new wxStaticText(this, wxID_ANY, str);
	
	loaded = false;
	fileStream = NULL;
	
  	filenameCtrl = 
  		new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition,
  		wxDefaultSize, wxTE_READONLY);
	
	wxBitmapButton *button1 = new wxBitmapButton(this, wxID_ANY, openBitmap);
	wxBitmapButton *button2 = new wxBitmapButton(this, wxID_ANY, saveBitmap);
	
	wxBoxSizer *sizer = new wxBoxSizer(wxHORIZONTAL);
	
  	sizer->Add(text, 0, wxALL | wxALIGN_CENTER, 5);
  	sizer->Add(filenameCtrl, 1, wxALL | wxALIGN_CENTER, 5);
  	sizer->Add(button1, 0, wxALL | wxALIGN_CENTER, 5);
  	sizer->Add(button2, 0, wxALL | wxALIGN_CENTER, 5);
  	
	this->SetBackgroundColour(white);
	this->SetSizer(sizer);
	
	filenameCtrl->SetValue(wxT("(empty)"));
	
	Bind(wxEVT_BUTTON, &OpenSavePanel::OnOpen, this, button1->GetId());
	Bind(wxEVT_BUTTON, &OpenSavePanel::OnSave, this, button2->GetId());
}

OpenSavePanel::~OpenSavePanel()
{
	if (fileStream)
	{
		delete fileStream;
		fileStream = NULL;
	}
}

void OpenSavePanel::OnOpen(wxCommandEvent& event)
{
	wxFileDialog openFileDialog
		(this, "Open Disk Image", "", "", "Raw IMG files (*.img)|*.img",
		wxFD_OPEN | wxFD_FILE_MUST_EXIST);
    
    if (openFileDialog.ShowModal() == wxID_CANCEL) {
    	return;
    }
    
    wxString filepath = openFileDialog.GetPath();
    
    fileStream = new wxFileStream(filepath);
    
    if (fileStream->IsOk())
    {
    	loaded = true;
    	
    	filenameCtrl->SetValue(filepath);
    }
}

void OpenSavePanel::OnSave(wxCommandEvent& event)
{
	//
}

