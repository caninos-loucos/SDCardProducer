#include "imgfile.hpp"
#include "../resources/open.xpm"
#include "../resources/save.xpm"
#include "myframe.hpp"

#define SECTOR (512)
#define MAX_PARTITIONS (4)

wxDEFINE_EVENT(MYWORKER_READY, wxThreadEvent);

const static wxString emptyStr(wxT("(empty)"));
const static wxString createThreadErrStr(wxT("Could not create thread"));
const static wxString fileIOErrStr(wxT("File IO error"));
const static wxString errorStr(wxT("Error"));

const static wxColour white(0xff, 0xff, 0xff);

enum
{
	THREAD_CMD_OPEN = 0,
	THREAD_CMD_SAVE = 1,
};

class WorkThread : public wxThread
{
public:
	WorkThread(OpenSavePanel *parent, int cmd);
	~WorkThread();
	
protected:
	virtual ExitCode Entry();

private:
	OpenSavePanel *parent;
	bool IsValidType(uint8_t type);
	void DoOpen();
	void DoSave();
	int cmd;
};

WorkThread::~WorkThread()
{
	wxCriticalSectionLocker enter(parent->workercs);
	parent->worker = NULL;
}

WorkThread::WorkThread(OpenSavePanel *parent, int cmd) 
	: wxThread(wxTHREAD_DETACHED)
{
	this->parent = parent;
	this->cmd = cmd;
}

bool WorkThread::IsValidType(uint8_t type)
{
	bool valid = false;
	
	switch(type)
	{
	case 0x00:
	case 0x01:
	case 0x04:
	case 0x07:
	case 0x0C:
	case 0x0E:
	case 0x82:
	case 0x83:
		valid = true;
		break;
	}
	
	return valid;
}

void WorkThread::DoOpen()
{
	wxFileStream *stream;
	wxString filename;
	
	parent->workercs.Enter();
	
	filename = parent->filename;
	
	parent->workercs.Leave();
	
	stream = new wxFileStream(filename);
	
	if (!stream->IsOk())
	{
		delete stream;
		
		parent->workercs.Enter();
		
		parent->error = true;
		
		parent->workercs.Leave();
		return;
	}
	
	int blocklen = (2 * SECTOR);
	
	uint8_t buffer[blocklen];
	
	stream->Read(buffer, blocklen);
	
	if (stream->LastRead() != blocklen)
	{
		delete stream;
		
		parent->workercs.Enter();
		
		parent->error = true;
		
		parent->workercs.Leave();
		return;
	}
	
	struct OpenSavePanel::Partition *partitions;
	
	partitions = new struct OpenSavePanel::Partition[MAX_PARTITIONS];
	
	for (auto i = 0; i < MAX_PARTITIONS; i++)
	{
		partitions[i].active = false;
		partitions[i].isMBR = false;
		partitions[i].isGPT = false;
		partitions[i].type = 0;
		partitions[i].lba = 0;
		partitions[i].sectors = 0;
	}
	
	bool hasActivePartition = false;
	bool bootSig = true;
	bool invalidate = false;
	
	// check boot signature
	
	if (buffer[510] != 0x55 || buffer[511] != 0xAA)
	{
		invalidate = true;
		bootSig = false;
	}
	
	// is it a GPT table? check signature "EFI PART"
	
	if (buffer[512] == 0x45 && buffer[513] == 0x46 && 
		buffer[514] == 0x49 && buffer[515] == 0x20 &&
		buffer[516] == 0x50 && buffer[517] == 0x41 &&
		buffer[518] == 0x52 && buffer[519] == 0x54)
	{
		for (auto i = 0; i < MAX_PARTITIONS; i++) {
			partitions[i].isGPT = true;
		}
		
		invalidate = true;
	}
	
	uint32_t *aux = (uint32_t *)(buffer + 446);
	
	if (bootSig)
	{
		for (auto i = 0; i < MAX_PARTITIONS; i++)
		{
			if (*aux & 0x80)
			{
				if (hasActivePartition)
				{
					// must not have more than one bootable partition
					invalidate = true;
				}
			
				hasActivePartition = true;
				partitions[i].active = true;
			}
			else {
				partitions[i].active = false;
			}
		
			partitions[i].isMBR = true;
			aux++;
		
			partitions[i].type = *aux & 0xFF;
			aux++;
		
			partitions[i].lba = *aux;
			aux++;
		
			partitions[i].sectors = *aux;
			aux++;
		
			if (partitions[i].type != 0)
			{
				// no valid partition can be located before 1MB disk mark
			
				if (partitions[i].lba < 2048) {
					invalidate = true;
				}
			}
			
			if (!IsValidType(partitions[i].type)) {
				invalidate = true;
			}
		}
	}
	
	if (!hasActivePartition) {
		invalidate = true;
	}
	
	parent->workercs.Enter();
	
	if (parent->stream) {
		delete parent->stream;
	}
	
	parent->stream = stream;
	parent->valid  = !invalidate;
	parent->loaded = true;
	parent->error  = false;
	
	if (parent->partitions) {
		delete parent->partitions;
	}
	
	parent->partitions = partitions;
	
	parent->workercs.Leave();
}

void WorkThread::DoSave()
{
	//
}

wxThread::ExitCode WorkThread::Entry()
{
	switch (cmd)
	{
	case THREAD_CMD_OPEN:
		DoOpen();
		break;
		
	case THREAD_CMD_SAVE:
		DoSave();
		break;
	}
	
	wxQueueEvent(parent, new wxThreadEvent(MYWORKER_READY));
	
	return (wxThread::ExitCode)(0);
}

void OpenSavePanel::OnThreadCompleted(wxThreadEvent& event)
{
	while (IsWorkerRunning()) {
		wxThread::This()->Sleep(1);
	}
	
	UpdateGUI();
	
	if (error) {
		wxMessageBox(fileIOErrStr, errorStr, wxOK | wxICON_ERROR);
	}
	
	MyFrame *parent = dynamic_cast<MyFrame*>(GetParent());
	
	parent->OnThreadCompleted();
}

int OpenSavePanel::IsValidImage(bool &valid)
{
	if (IsWorkerRunning()) {
		return -1;
	}
	
	valid = this->valid;
	return 0;
}

int OpenSavePanel::IsLoaded(bool &loaded)
{
	if (IsWorkerRunning()) {
		return -1;
	}
	
	loaded = this->loaded;
	return 0;
}

void OpenSavePanel::UpdateGUI()
{
	if (loaded)
	{
		filenameCtrl->SetValue(filename);
		
		openButton->Enable(true);
		
		if (valid) {
			saveButton->Enable(true);
		}
		else {
			saveButton->Enable(false);
		}
		
		filenameCtrl->Enable(true);
	}
	else
	{
		filenameCtrl->SetValue(emptyStr);
		
		openButton->Enable(true);
		
		saveButton->Enable(false);
		
		filenameCtrl->Enable(false);
	}
}

int OpenSavePanel::CloseImage()
{
	if (IsWorkerRunning()) {
		return -1;
	}
	
	if (stream)
	{
		delete stream;
		stream = NULL;
	}
	
	if (partitions)
	{
		delete partitions;
		partitions = NULL;
	}
	
	valid  = false;
	loaded = false;
	error  = false;
	
	UpdateGUI();
	
	return 0;
}

int OpenSavePanel::GetPartition(int i, struct OpenSavePanel::Partition &data)
{
	if (IsWorkerRunning()) {
		return -1;
	}
	
	if ((partitions != NULL) && (i >= 0) && (i < MAX_PARTITIONS)) 
	{
		data = partitions[i];
		return 0;
	}
	
	return -1;
}

int OpenSavePanel::GetPartitionCount()
{
	if (IsWorkerRunning()) {
		return -1;
	}
	
	if (partitions) {
		return MAX_PARTITIONS;
	}
	
	return 0;
}

bool OpenSavePanel::IsWorkerRunning()
{
	wxCriticalSectionLocker enter(workercs);
	
	return (worker != NULL);
}

OpenSavePanel::OpenSavePanel(wxWindow *parent) : wxPanel(parent)
{
	openBitmap = wxBitmap(open_xpm);
	saveBitmap = wxBitmap(save_xpm);
	
	wxString str = wxT("Image filename:");
	
	wxStaticText *text = new wxStaticText(this, wxID_ANY, str);
	
	loaded = false;
	valid = false;
	error = false;
	stream = NULL;
	partitions = NULL;
	
  	filenameCtrl = 
  		new wxTextCtrl(this, wxID_ANY, emptyStr, wxDefaultPosition,
  		wxDefaultSize, wxTE_READONLY);
	
	openButton = new wxBitmapButton(this, wxID_ANY, openBitmap);
	saveButton = new wxBitmapButton(this, wxID_ANY, saveBitmap);
	
	wxBoxSizer *sizer = new wxBoxSizer(wxHORIZONTAL);
	
  	sizer->Add(text, 0, wxALL | wxALIGN_CENTER, 5);
  	sizer->Add(filenameCtrl, 1, wxALL | wxALIGN_CENTER, 5);
  	sizer->Add(openButton, 0, wxALL | wxALIGN_CENTER, 5);
  	sizer->Add(saveButton, 0, wxALL | wxALIGN_CENTER, 5);
  	
	this->SetBackgroundColour(white);
	this->SetSizer(sizer);
	
	saveButton->Enable(false);
	
	filenameCtrl->Enable(false);
	
	Bind(wxEVT_BUTTON, &OpenSavePanel::OnOpen, this, openButton->GetId());
	Bind(wxEVT_BUTTON, &OpenSavePanel::OnSave, this, saveButton->GetId());
	
	Bind(MYWORKER_READY, &OpenSavePanel::OnThreadCompleted, this, wxID_ANY);
}

OpenSavePanel::~OpenSavePanel()
{
	while (IsWorkerRunning()) {
		wxThread::This()->Sleep(1);
	}
	
	if (stream) {
		delete stream;
	}
	
	if (partitions) {
		delete partitions;
	}
}

void OpenSavePanel::OnOpen(wxCommandEvent& event)
{
	if (IsWorkerRunning()) {
		return;
	}
	
	wxFileDialog openFileDialog
		(this, "Open Disk Image", "", "", "Raw IMG files (*.img)|*.img",
		wxFD_OPEN | wxFD_FILE_MUST_EXIST);
    
    if (openFileDialog.ShowModal() == wxID_CANCEL) {
    	return;
    }
    
    filename = openFileDialog.GetPath();
    
    worker = new WorkThread(this, THREAD_CMD_OPEN);
  	
  	if(worker->Run() == wxTHREAD_NO_ERROR)
  	{
  		openButton->Enable(false);
  		saveButton->Enable(false);
	}
	else
	{
		delete worker;
  		worker = NULL;
  		
  		wxMessageBox(createThreadErrStr, errorStr, wxOK | wxICON_ERROR);
	}
}

void OpenSavePanel::OnSave(wxCommandEvent& event)
{
	if (IsWorkerRunning()) {
		return;
	}
}

