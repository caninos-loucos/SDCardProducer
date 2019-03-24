#ifndef _MYFRAME_HPP_
#define _MYFRAME_HPP_

#include <wx/wx.h>
#include "config.hpp"
#include "imgfile.hpp"
#include "about.hpp"
#include "info.hpp"

class MyFrame : public wxFrame
{
public:
	MyFrame();
	
	void OnThreadCompleted();
	
private:
	ConfigPanel *cfgPanel;
	OpenSavePanel *openPanel;
	AboutPanel *aboutPanel;
	InfoPanel *infoPanel;
};

#endif

