#ifndef _MYFRAME_HPP_
#define _MYFRAME_HPP_

#include <wx/wx.h>
#include <wx/wfstream.h>

enum
{
	MEMTYPE = 0,
	MEMCAP  = 1,
	MEMBANK = 2,
	MEMBIT  = 3,
	MEMVOLT = 4,
	MEMFREQ = 5,
};

enum
{
	KVERSION = 0,
	BOOTDISK = 1,
	EXECMODE = 2,
};

class MyFrame : public wxFrame
{
public:
	MyFrame();
	~MyFrame();
	
private:
	wxBitmap acceptBitmap;
	wxBitmap denyBitmap;
	wxBitmap openBitmap;
	wxBitmap saveBitmap; 
	wxBitmap aboutBitmap;
	wxBitmap dogtagBitmap;
	
	wxPanel *mainPanel;
	wxPanel *aboutPanel;
	wxPanel *openPanel;
	wxPanel *savePanel;
	wxPanel *infoPanel;
	wxPanel *cfgPanel;
	
	wxStaticBitmap *fileValid; 
	wxTextCtrl     *inFilename;
	wxTextCtrl     *scheme;
	wxTextCtrl     *totsize;
	wxTextCtrl     *partition[4];
	wxChoice       *preset;
	wxChoice       *memcfg[6];
	wxChoice       *bootcfg[3];
	
	wxPanel *CreateAboutPanel     (wxPanel *parent);
	wxPanel *CreateOpenPanel      (wxPanel *parent);
	wxPanel *CreateInfoPanel      (wxPanel *parent);
	wxPanel *CreateTotSizePanel   (wxPanel *parent);
	wxPanel *CreateSchemePanel    (wxPanel *parent);
	wxPanel *CreatePartPanel      (wxPanel *parent, int index);
	wxPanel *CreateConfigPanel    (wxPanel *parent);
	wxPanel *CreatePresetPanel    (wxPanel *parent);
	wxPanel *CreateMemConfigPanel (wxPanel *parent, int index);
	wxPanel *CreateBootConfigPanel(wxPanel *parent, int index);
};

#endif

