#ifndef MAINFRAME_H
#define MAINFRAME_H
#include "wxcrafter.h"

class Notebook;
class MainFrame : public MainFrameBaseClass
{
    Notebook* m_book;
public:
    MainFrame(wxWindow* parent);
    virtual ~MainFrame();

    void OnExit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);

    void OnPageChanged(wxBookCtrlEvent& event);
    void OnPageChanging(wxBookCtrlEvent& event);
    void OnPageClosed(wxBookCtrlEvent& event);
    void OnPageClosing(wxBookCtrlEvent& event);
protected:
    virtual void OnMouseMiddleCloses(wxCommandEvent& event);
    virtual void OnDeleteAllPages(wxCommandEvent& event);
    virtual void OnAllowTabMove(wxCommandEvent& event);
    virtual void OnShowCloseButton(wxCommandEvent& event);
    virtual void OnTabStyle(wxCommandEvent& event);
    virtual void OnClose(wxCommandEvent& event);
};
#endif // MAINFRAME_H
