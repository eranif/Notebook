#ifndef MAINFRAME_H
#define MAINFRAME_H
#include "wxcrafter.h"

class MainFrame : public MainFrameBaseClass
{
public:
    MainFrame(wxWindow* parent);
    virtual ~MainFrame();

    void OnExit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);

    void OnPageChanged(wxBookCtrlEvent& event);
    void OnPageChanging(wxBookCtrlEvent& event);
};
#endif // MAINFRAME_H
