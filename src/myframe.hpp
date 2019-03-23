#ifndef _MYFRAME_HPP_
#define _MYFRAME_HPP_

#include <wx/wx.h>
#include "config.hpp"
#include "imgfile.hpp"

class MyFrame : public wxFrame
{
public:
	MyFrame();
	
private:
	wxBitmap acceptBitmap;
	wxBitmap denyBitmap;
	
	wxBitmap aboutBitmap;
	wxBitmap dogtagBitmap;
	
	wxPanel *mainPanel;
	wxPanel *aboutPanel;
	
	wxPanel *savePanel;
	wxPanel *infoPanel;
	
	wxStaticBitmap *filevalid; 
	
	wxTextCtrl     *scheme;
	wxTextCtrl     *totsize;
	wxTextCtrl     *partition[4];
	
	ConfigPanel *cfgPanel;
	OpenSavePanel *openPanel;
	
	wxPanel *CreateAboutPanel     (wxPanel *parent);
	
	wxPanel *CreateInfoPanel      (wxPanel *parent);
	wxPanel *CreateTotSizePanel   (wxPanel *parent);
	wxPanel *CreateSchemePanel    (wxPanel *parent);
	wxPanel *CreatePartPanel      (wxPanel *parent, int index);
	
};

#endif

