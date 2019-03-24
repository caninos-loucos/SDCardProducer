#ifndef _ABOUT_HPP_
#define _ABOUT_HPP_

#include <wx/wx.h>
#include "imgfile.hpp"

class AboutPanel : public wxPanel
{
public:
	AboutPanel(wxWindow *parent, OpenSavePanel *openPanel);
	
	void OnThreadCompleted();
	
private:
	wxBitmap aboutBitmap;
	wxBitmap acceptBitmap;
	wxBitmap denyBitmap;
	wxBitmap dogtagBitmap;
	
	OpenSavePanel *openPanel;
	
	wxStaticBitmap *filevalid;
};

#endif

