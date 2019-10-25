//////////////////////////////////////////////////////////////////////
// This file was auto-generated by codelite's wxCrafter Plugin
// wxCrafter project file: wxcrafter.wxcp
// Do not modify this file by hand!
//////////////////////////////////////////////////////////////////////

#ifndef _NOTEBOOK_NOTEBOOK_WXCRAFTER_BASE_CLASSES_H
#define _NOTEBOOK_NOTEBOOK_WXCRAFTER_BASE_CLASSES_H

// clang-format off
#include <wx/settings.h>
#include <wx/xrc/xmlres.h>
#include <wx/xrc/xh_bmp.h>
#include <wx/frame.h>
#include <wx/iconbndl.h>
#include <wx/artprov.h>
#include <wx/sizer.h>
#include <wx/menu.h>
#include <wx/panel.h>
#include <wx/textctrl.h>
#include <wx/imaglist.h>
#include <wx/bitmap.h>
#include <map>
#include <wx/icon.h>
#if wxVERSION_NUMBER >= 2900
#include <wx/persist.h>
#include <wx/persist/toplevel.h>
#include <wx/persist/bookctrl.h>
#include <wx/persist/treebook.h>
#endif

#ifdef WXC_FROM_DIP
#undef WXC_FROM_DIP
#endif
#if wxVERSION_NUMBER >= 3100
#define WXC_FROM_DIP(x) wxWindow::FromDIP(x, NULL)
#else
#define WXC_FROM_DIP(x) x
#endif

// clang-format on

class MainFrameBaseClass : public wxFrame
{
public:
    enum {
        ID_SHOW_DROP_DOWNBUTTON = 1001,
        ID_MOUSE_MIDDLE_CLOSE = 1002,
        ID_CLOSE_ALL = 1003,
        ID_CLOSE_BUTTON = 1004,
        ID_CLOSE_ALLOW_TAB_MOVE = 1005,
        ID_STYLE_DEFAULT = 1006,
        ID_STYLE_TRAPEZ = 1007,
        ID_STYLE_MINIMAL = 1008,
        ID_STYLE_GTK3 = 1009,
        ID_BOTTOM_TABS = 1010,
        ID_RIGHT_SIZE_TABS = 1011,
        ID_LEFT_SIZE_TABS = 1012,
        ID_TOP_TABS = 1013,
    };

protected:
    wxMenuBar* m_menuBar19;
    wxMenu* m_menu21;
    wxMenu* m_menu29;
    wxMenuItem* m_menuItem55;
    wxMenuItem* m_styleLeft;
    wxMenuItem* m_styleRight;
    wxMenuItem* m_menuItemBottomTabs;
    wxMenuItem* m_menuItem63;
    wxMenuItem* m_styleGTK3;
    wxMenuItem* m_styleMINIMAL;
    wxMenuItem* m_styleTRAPEZ;
    wxMenuItem* m_styleDEFAULT;
    wxMenuItem* m_menuItemMove;
    wxMenuItem* m_menuItemShowCloseButton;
    wxMenuItem* m_menuItemCloseAll;
    wxMenuItem* m_menuItemMouseMiddleClickCLoses;
    wxMenuItem* m_menuItemShowDropDownButton;
    wxMenuItem* m_separator;
    wxMenuItem* Exit;
    wxPanel* m_mainPanel;
    wxTextCtrl* m_log;

protected:
    virtual void OnTopTabs(wxCommandEvent& event) { event.Skip(); }
    virtual void OnLeftTabs(wxCommandEvent& event) { event.Skip(); }
    virtual void OnRightTabs(wxCommandEvent& event) { event.Skip(); }
    virtual void OnBottomTabs(wxCommandEvent& event) { event.Skip(); }
    virtual void OnTabStyle(wxCommandEvent& event) { event.Skip(); }
    virtual void OnAllowTabMove(wxCommandEvent& event) { event.Skip(); }
    virtual void OnDnDUI(wxUpdateUIEvent& event) { event.Skip(); }
    virtual void OnShowCloseButton(wxCommandEvent& event) { event.Skip(); }
    virtual void OnDeleteAllPages(wxCommandEvent& event) { event.Skip(); }
    virtual void OnMouseMiddleCloses(wxCommandEvent& event) { event.Skip(); }
    virtual void OnShowFileListButton(wxCommandEvent& event) { event.Skip(); }
    virtual void OnShowFileListButtonUI(wxUpdateUIEvent& event) { event.Skip(); }
    virtual void OnClose(wxCommandEvent& event) { event.Skip(); }

public:
    wxMenuBar* GetMenuBar19() { return m_menuBar19; }
    wxTextCtrl* GetLog() { return m_log; }
    wxPanel* GetMainPanel() { return m_mainPanel; }
    MainFrameBaseClass(wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("My Frame"),
        const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize(-1, -1),
        long style = wxCAPTION | wxRESIZE_BORDER | wxMAXIMIZE_BOX | wxMINIMIZE_BOX | wxSYSTEM_MENU | wxCLOSE_BOX);
    virtual ~MainFrameBaseClass();
};

class DemoAppImages : public wxImageList
{
protected:
    // Maintain a map of all bitmaps representd by their name
    std::map<wxString, wxBitmap> m_bitmaps;
    // The requested image resolution (can be one of @2x, @1.5x, @1.25x or an empty string (the default)
    wxString m_resolution;
    int m_imagesWidth;
    int m_imagesHeight;

protected:
public:
    DemoAppImages();
    const wxBitmap& Bitmap(const wxString& name) const
    {
        if(!m_bitmaps.count(name + m_resolution)) return wxNullBitmap;
        return m_bitmaps.find(name + m_resolution)->second;
    }

    void SetBitmapResolution(const wxString& res = wxEmptyString) { m_resolution = res; }

    virtual ~DemoAppImages();
};

#endif
