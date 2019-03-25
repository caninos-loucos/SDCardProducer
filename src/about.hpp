#ifndef _ABOUT_HPP_
#define _ABOUT_HPP_

#include <wx/wx.h>
#include "imgfile.hpp"

class AboutPanel : public wxPanel
{
public:
	AboutPanel(wxWindow *parent, OpenSavePanel *openPanel);
	
	void UpdateGUI();
	
	void DisableGUI();
	
private:
	wxBitmap aboutBitmap;
	wxBitmap acceptBitmap;
	wxBitmap denyBitmap;
	wxBitmap dogtagBitmap;
	wxBitmap cancelBitmap;
	
	void OnCancel(wxCommandEvent& event);
	
	OpenSavePanel *openPanel;
	
	wxBitmapButton *cancelButton;
	wxBitmapButton *aboutButton;
	
	wxStaticBitmap *filevalid;
};

#endif

