#ifndef _INFO_HPP_
#define _INFO_HPP_

#include <wx/wx.h>
#include "imgfile.hpp"

class InfoPanel : public wxPanel
{
public:
	InfoPanel(wxWindow *parent, OpenSavePanel *openPanel);
	
	void UpdateGUI();
	
	void DisableGUI();
	
private:
	wxTextCtrl *scheme;
	wxTextCtrl *totsize;
	wxTextCtrl *partition[4];
	
	OpenSavePanel *openPanel;
	
	wxPanel *CreateTotSizePanel(wxWindow *parent);
	wxPanel *CreateSchemePanel(wxWindow *parent);
	wxPanel *CreatePartPanel(wxWindow *parent, int index);
	
	wxString GetTypeName(uint8_t type);
};

#endif

