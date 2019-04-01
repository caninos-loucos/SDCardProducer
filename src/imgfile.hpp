#ifndef _IMGFILE_HPP_
#define _IMGFILE_HPP_

#include <wx/wx.h>
#include <cstdio>
#include "enums.hpp"

class WorkThread;

class OpenSavePanel : public wxPanel
{
public:
	struct Partition
	{
		bool active;
		bool isMBR;
		bool isGPT;
		uint8_t type;
		uint32_t lba;
		uint32_t sectors;
	};
	
	OpenSavePanel(wxWindow *parent);
	~OpenSavePanel();
	
	int IsValidImage(bool &valid);
	int IsLoaded(bool &loaded);
	
	int GetPartition(int i, struct Partition &data);
	int GetPartitionCount();
	
	int GetDiskSectorCount(uint32_t &sectors);
	
	int CloseImage();
	
	void DisableGUI();
	
	void UpdateGUI();
	
private:
	wxBitmap openBitmap;
	wxBitmap saveBitmap;
	
	wxTextCtrl *filenameCtrl;
	
	void OnOpen(wxCommandEvent& event);
	void OnSave(wxCommandEvent& event);
	
	void OnThreadCompleted(wxThreadEvent& event);
	
	bool IsWorkerRunning();
	
	void FillBlock();
	
	friend class WorkThread;
	
	wxBitmapButton *openButton;
	wxBitmapButton *saveButton;
	
	WorkThread *worker;
	wxCriticalSection workercs;
	
	uint8_t *firstFileBlock;
	
	uint32_t diskSectorCount;
	
	struct Partition *partitions;
	FILE *stream;
	wxString filename;
	bool loaded;
	bool valid;
	bool error;
};

#endif

