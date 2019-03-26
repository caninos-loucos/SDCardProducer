#include "imgfile.hpp"
#include "myframe.hpp"
#include "../resources/open.xpm"
#include "../resources/save.xpm"
#include "../resources/fstage.h"
#include "../resources/sstage.h"
#include "../resources/uboot.h"
#include <cstring>

#define BLOCKSZ (0x100000) // 1MB

#define MAX_PARTITIONS (4)

#define BROM_DEV_SDCARD (0)
#define BROM_DEV_EMMC   (2)
#define BROM_DEV_NAND   (3)
#define BROM_DEV_SPINOR (4)

#define FIRST_STAGE_OFF  (0x200) // LBA=1
#define FIRST_STAGE_SIZE (0x200)

#define SECOND_STAGE_OFF  (0x400) // LBA=2
#define SECOND_STAGE_SIZE (0x8000)

#define UBOOT_OFF  (0x8400) // LBA=66
#define UBOOT_SIZE (0xF7C00)

#define FWINFO_CUR_BOOT_DEV_OFF (0x300)
#define FWINFO_DEF_BOOT_DEV_OFF (0x304)
#define FWINFO_KERN_VERSION_OFF (0x308)
#define FWINFO_EXEC_MODE_OFF    (0x30C)
#define FWINFO_MEM_TYPE_OFF     (0x310)
#define FWINFO_MEM_CAP_OFF      (0x314)
#define FWINFO_MEM_BANK_OFF     (0x318)
#define FWINFO_MEM_WIDTH_OFF    (0x31C)
#define FWINFO_MEM_VOLT_OFF     (0x320)
#define FWINFO_MEM_FREQ_OFF     (0x324)

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
	
	uint32_t dwchecksum(char * buf, int start, int length);
	int AddChecksum(uint8_t *buffer, int offset);
	
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
	case 0x06:
	case 0x07:
	case 0x0B:
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
	FILE *stream = NULL;
	wxString filename;
	
	parent->workercs.Enter();
	
	filename = parent->filename;
	
	parent->workercs.Leave();
	
	stream = fopen(filename.c_str(), "rb+");
	
	if (!stream)
	{
		parent->workercs.Enter();
		
		parent->error = true;
		
		parent->workercs.Leave();
		return;
	}
	
	uint8_t *buffer = new uint8_t[BLOCKSZ];
	
	if (fread(buffer, 1, BLOCKSZ, stream) != BLOCKSZ)
	{
		fclose(stream);
		
		delete buffer;
		
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
		fclose(parent->stream);
	}
	
	parent->stream = stream;
	parent->valid  = !invalidate;
	parent->loaded = true;
	parent->error  = false;
	
	if (parent->firstFileBlock) {
		delete parent->firstFileBlock;
	}
	
	parent->firstFileBlock = buffer;
	
	if (parent->partitions) {
		delete parent->partitions;
	}
	
	parent->partitions = partitions;
	
	parent->workercs.Leave();
}

uint32_t WorkThread::dwchecksum(char * buf, int start, int length)
{
    uint32_t sum = 0;
    uint32_t * tmp_buf = (uint32_t *)buf;
    int tmp_start = start / 4;
    int tmp_length = length / 4;
    int i;

    for(i = tmp_start; i < tmp_length; i++) {
        sum = sum + *(tmp_buf + i);
    }
    
    return sum;
}

int WorkThread::AddChecksum(uint8_t *buffer, int offset)
{
    const int size = FIRST_STAGE_SIZE;
    char *buf = (char*)(buffer + offset);
    uint32_t sum;
    
    sprintf(buf + size - 12, "ActBrm%c%c", 0xaa, 0x55);
    
    sum = dwchecksum(buf, 0, size - 4);
    sum += 0x1234;
    
    *((uint32_t*)(buf + size - 4)) = sum;
    
	return 0;
}

void WorkThread::DoSave()
{
	parent->workercs.Enter();
	
	uint8_t *buffer = parent->firstFileBlock;
	
	AddChecksum(buffer, FIRST_STAGE_OFF);
	
	memset(buffer + SECOND_STAGE_OFF, 0, SECOND_STAGE_SIZE);
	memcpy(buffer + SECOND_STAGE_OFF, hex_sstage, sizeof(hex_sstage));
	
	memset(buffer + UBOOT_OFF, 0, UBOOT_SIZE);
	memcpy(buffer + UBOOT_OFF, hex_uboot, sizeof(hex_uboot));
	
	rewind(parent->stream);
	
	if (fwrite(buffer, 1, BLOCKSZ, parent->stream) != BLOCKSZ)
	{
		parent->error = true;
		
		parent->workercs.Leave();
		
		return;
	}
	
	parent->error = false;
	
	parent->workercs.Leave();
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
	
	MyFrame *parent = dynamic_cast<MyFrame*>(GetParent());
	
	parent->UpdateGUI();
}

int OpenSavePanel::CloseImage()
{
	if (IsWorkerRunning()) {
		return -1;
	}
	
	if (stream)
	{
		fclose(stream);
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
	
	firstFileBlock = NULL;
	
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
		fclose(stream);
	}
	
	if (partitions) {
		delete partitions;
	}
	
	if (firstFileBlock) {
		delete firstFileBlock;
	}
}

void OpenSavePanel::DisableGUI()
{
	openButton->Enable(false);
  	saveButton->Enable(false);
  	
  	filenameCtrl->Enable(false);
  	
  	MyFrame *parent = dynamic_cast<MyFrame*>(GetParent());
	
	parent->DisableGUI();
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
	
	DisableGUI();
  	
    worker = new WorkThread(this, THREAD_CMD_OPEN);
  	
  	if(worker->Run() != wxTHREAD_NO_ERROR)
	{
		delete worker;
  		worker = NULL;
  		
  		UpdateGUI();
  		
  		wxMessageBox(createThreadErrStr, errorStr, wxOK | wxICON_ERROR);
	}
}

void OpenSavePanel::FillBlock()
{
	uint8_t *buffer = firstFileBlock;
	
	MyFrame *parent = dynamic_cast<MyFrame*>(GetParent());
	
	ConfigPanel *cfgPanel = parent->cfgPanel;
	
	memset(buffer + FIRST_STAGE_OFF, 0, FIRST_STAGE_SIZE);
	
	memcpy(buffer + FIRST_STAGE_OFF, hex_fstage, sizeof(hex_fstage));
	
	uint32_t *aux;
	
	aux = (uint32_t*)(buffer + FWINFO_CUR_BOOT_DEV_OFF);
	
	(*aux) = BROM_DEV_SDCARD;
	
	aux = (uint32_t*)(buffer + FWINFO_DEF_BOOT_DEV_OFF);
	
	int selection = cfgPanel->bootcfgChoice[BOOTCFG_BOOTDISK]->GetSelection();
	
	if (selection == DISK_SDCARD) {
		(*aux) = BROM_DEV_SDCARD;
	}
	else {
		(*aux) = BROM_DEV_EMMC;
	}
	
	aux = (uint32_t*)(buffer + FWINFO_KERN_VERSION_OFF);
	
	(*aux) = cfgPanel->bootcfgChoice[BOOTCFG_KVERSION]->GetSelection();
	
	aux = (uint32_t*)(buffer + FWINFO_EXEC_MODE_OFF);
	
	(*aux) = cfgPanel->bootcfgChoice[BOOTCFG_EXECMODE]->GetSelection();
	
	aux = (uint32_t*)(buffer + FWINFO_MEM_TYPE_OFF);
	
	(*aux) = cfgPanel->memcfgChoice[MEMCFG_TYPE]->GetSelection();
	
	aux = (uint32_t*)(buffer + FWINFO_MEM_CAP_OFF);
	
	(*aux) = cfgPanel->memcfgChoice[MEMCFG_CAP]->GetSelection();
	
	aux = (uint32_t*)(buffer + FWINFO_MEM_BANK_OFF);
	
	(*aux) = cfgPanel->memcfgChoice[MEMCFG_BANK]->GetSelection();
	
	aux = (uint32_t*)(buffer + FWINFO_MEM_WIDTH_OFF);
	
	(*aux) = cfgPanel->memcfgChoice[MEMCFG_BIT]->GetSelection();
	
	aux = (uint32_t*)(buffer + FWINFO_MEM_VOLT_OFF);
	
	(*aux) = cfgPanel->memcfgChoice[MEMCFG_VOLT]->GetSelection();
	
	aux = (uint32_t*)(buffer + FWINFO_MEM_FREQ_OFF);
	
	(*aux) = cfgPanel->memcfgChoice[MEMCFG_FREQ]->GetSelection();
}

void OpenSavePanel::OnSave(wxCommandEvent& event)
{
	if (IsWorkerRunning()) {
		return;
	}
	
	DisableGUI();
	
	FillBlock();
	
	worker = new WorkThread(this, THREAD_CMD_SAVE);
  	
  	if(worker->Run() != wxTHREAD_NO_ERROR)
	{
		delete worker;
  		worker = NULL;
  		
  		UpdateGUI();
  		
  		wxMessageBox(createThreadErrStr, errorStr, wxOK | wxICON_ERROR);
	}
}

