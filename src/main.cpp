#include "myframe.hpp"

class MyApp : public wxApp
{
public:
	virtual bool OnInit();
};

wxIMPLEMENT_APP(MyApp);

bool MyApp::OnInit(void)
{
    MyFrame *frame = new MyFrame();
    frame->Show(true);
    return true;
}

