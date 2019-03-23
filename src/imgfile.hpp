#ifndef _IMGFILE_HPP_
#define _IMGFILE_HPP_

#include <wx/wx.h>
#include <wx/wfstream.h>
#include "enums.hpp"

class OpenSavePanel : public wxPanel
{
public:
	OpenSavePanel(wxWindow *parent);
	~OpenSavePanel();
	
private:
	wxBitmap openBitmap;
	wxBitmap saveBitmap;
	
	wxTextCtrl *filenameCtrl;
	
	wxFileStream *fileStream;
	
	bool loaded;
	
	void OnOpen(wxCommandEvent& event);
	void OnSave(wxCommandEvent& event);
};

#endif

