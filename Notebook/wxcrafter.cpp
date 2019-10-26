//////////////////////////////////////////////////////////////////////
// This file was auto-generated by codelite's wxCrafter Plugin
// wxCrafter project file: wxcrafter.wxcp
// Do not modify this file by hand!
//////////////////////////////////////////////////////////////////////

#include "wxcrafter.h"

// Declare the bitmap loading function
extern void wxC9ED9InitBitmapResources();

static bool bBitmapLoaded = false;

MainFrameBaseClass::MainFrameBaseClass(wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos,
                                       const wxSize& size, long style)
    : wxFrame(parent, id, title, pos, size, style)
{
    if(!bBitmapLoaded) {
        // We need to initialise the default bitmap handler
        wxXmlResource::Get()->AddHandler(new wxBitmapXmlHandler);
        wxC9ED9InitBitmapResources();
        bBitmapLoaded = true;
    }

    m_menuBar19 = new wxMenuBar(0);
    this->SetMenuBar(m_menuBar19);

    m_menu21 = new wxMenu();
    m_menuBar19->Append(m_menu21, _("File"));

    m_menu29 = new wxMenu();
    m_menu21->AppendSubMenu(m_menu29, _("Style"));

    m_menuItem55 = new wxMenuItem(m_menu29, ID_TOP_TABS, _("Top Tabs"), wxT(""), wxITEM_RADIO);
    m_menu29->Append(m_menuItem55);

    m_styleLeft = new wxMenuItem(m_menu29, ID_LEFT_SIZE_TABS, _("Left Side Tabs"), wxT(""), wxITEM_RADIO);
    m_menu29->Append(m_styleLeft);

    m_styleRight = new wxMenuItem(m_menu29, ID_RIGHT_SIZE_TABS, _("Right Side Tabs"), wxT(""), wxITEM_RADIO);
    m_menu29->Append(m_styleRight);

    m_menuItemBottomTabs = new wxMenuItem(m_menu29, ID_BOTTOM_TABS, _("Bottom Tabs"), wxT(""), wxITEM_RADIO);
    m_menu29->Append(m_menuItemBottomTabs);

    m_menu29->AppendSeparator();

    m_styleGTK3 = new wxMenuItem(m_menu29, ID_STYLE_GTK3, _("GTK3"), wxT(""), wxITEM_NORMAL);
    m_menu29->Append(m_styleGTK3);

    m_styleMINIMAL = new wxMenuItem(m_menu29, ID_STYLE_MINIMAL, _("MINIMAL"), wxT(""), wxITEM_NORMAL);
    m_menu29->Append(m_styleMINIMAL);

    m_styleTRAPEZ = new wxMenuItem(m_menu29, ID_STYLE_TRAPEZ, _("TRAPEZ"), wxT(""), wxITEM_NORMAL);
    m_menu29->Append(m_styleTRAPEZ);

    m_styleDEFAULT = new wxMenuItem(m_menu29, ID_STYLE_DEFAULT, _("DEFAULT"), wxT(""), wxITEM_NORMAL);
    m_menu29->Append(m_styleDEFAULT);

    m_menuItemMove = new wxMenuItem(m_menu21, ID_CLOSE_ALLOW_TAB_MOVE, _("Allow tabs to move"), wxT(""), wxITEM_CHECK);
    m_menu21->Append(m_menuItemMove);

    m_menuItemShowCloseButton =
        new wxMenuItem(m_menu21, ID_CLOSE_BUTTON, _("Show Close Button"), wxT(""), wxITEM_CHECK);
    m_menu21->Append(m_menuItemShowCloseButton);

    m_menuItemCloseAll = new wxMenuItem(m_menu21, ID_CLOSE_ALL, _("Delete all pages"), wxT(""), wxITEM_NORMAL);
    m_menu21->Append(m_menuItemCloseAll);

    m_menuItemMouseMiddleClickCLoses =
        new wxMenuItem(m_menu21, ID_MOUSE_MIDDLE_CLOSE, _("Mouse Middle Closes tab"), wxT(""), wxITEM_CHECK);
    m_menu21->Append(m_menuItemMouseMiddleClickCLoses);

    m_menuItemShowDropDownButton =
        new wxMenuItem(m_menu21, ID_SHOW_DROP_DOWNBUTTON, _("Show File List Button"), wxT(""), wxITEM_CHECK);
    m_menu21->Append(m_menuItemShowDropDownButton);

    m_menu21->AppendSeparator();

    Exit = new wxMenuItem(m_menu21, wxID_EXIT, _("Exit\tAlt-F4"), wxT(""), wxITEM_NORMAL);
    m_menu21->Append(Exit);

    wxBoxSizer* boxSizer1 = new wxBoxSizer(wxVERTICAL);
    this->SetSizer(boxSizer1);

    m_mainPanel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1, -1)), wxTAB_TRAVERSAL);

    boxSizer1->Add(m_mainPanel, 1, wxEXPAND, WXC_FROM_DIP(5));

    wxBoxSizer* boxSizer11 = new wxBoxSizer(wxVERTICAL);
    m_mainPanel->SetSizer(boxSizer11);

    m_log = new wxTextCtrl(m_mainPanel, wxID_ANY, wxT(""), wxDefaultPosition, wxDLG_UNIT(m_mainPanel, wxSize(-1, 150)),
                           wxTE_RICH | wxTE_MULTILINE);

    boxSizer11->Add(m_log, 0, wxALL | wxEXPAND, WXC_FROM_DIP(5));

    SetName(wxT("MainFrameBaseClass"));
    SetSize(wxDLG_UNIT(this, wxSize(-1, -1)));
    if(GetSizer()) { GetSizer()->Fit(this); }
    if(GetParent()) {
        CentreOnParent(wxBOTH);
    } else {
        CentreOnScreen(wxBOTH);
    }
#if wxVERSION_NUMBER >= 2900
    if(!wxPersistenceManager::Get().Find(this)) {
        wxPersistenceManager::Get().RegisterAndRestore(this);
    } else {
        wxPersistenceManager::Get().Restore(this);
    }
#endif
    // Connect events
    this->Connect(m_menuItem55->GetId(), wxEVT_COMMAND_MENU_SELECTED,
                  wxCommandEventHandler(MainFrameBaseClass::OnTopTabs), NULL, this);
    this->Connect(m_styleLeft->GetId(), wxEVT_COMMAND_MENU_SELECTED,
                  wxCommandEventHandler(MainFrameBaseClass::OnLeftTabs), NULL, this);
    this->Connect(m_styleRight->GetId(), wxEVT_COMMAND_MENU_SELECTED,
                  wxCommandEventHandler(MainFrameBaseClass::OnRightTabs), NULL, this);
    this->Connect(m_menuItemBottomTabs->GetId(), wxEVT_COMMAND_MENU_SELECTED,
                  wxCommandEventHandler(MainFrameBaseClass::OnBottomTabs), NULL, this);
    this->Connect(m_styleGTK3->GetId(), wxEVT_COMMAND_MENU_SELECTED,
                  wxCommandEventHandler(MainFrameBaseClass::OnTabStyle), NULL, this);
    this->Connect(m_styleMINIMAL->GetId(), wxEVT_COMMAND_MENU_SELECTED,
                  wxCommandEventHandler(MainFrameBaseClass::OnTabStyle), NULL, this);
    this->Connect(m_styleTRAPEZ->GetId(), wxEVT_COMMAND_MENU_SELECTED,
                  wxCommandEventHandler(MainFrameBaseClass::OnTabStyle), NULL, this);
    this->Connect(m_styleDEFAULT->GetId(), wxEVT_COMMAND_MENU_SELECTED,
                  wxCommandEventHandler(MainFrameBaseClass::OnTabStyle), NULL, this);
    this->Connect(m_menuItemMove->GetId(), wxEVT_COMMAND_MENU_SELECTED,
                  wxCommandEventHandler(MainFrameBaseClass::OnAllowTabMove), NULL, this);
    this->Connect(m_menuItemMove->GetId(), wxEVT_UPDATE_UI, wxUpdateUIEventHandler(MainFrameBaseClass::OnDnDUI), NULL,
                  this);
    this->Connect(m_menuItemShowCloseButton->GetId(), wxEVT_COMMAND_MENU_SELECTED,
                  wxCommandEventHandler(MainFrameBaseClass::OnShowCloseButton), NULL, this);
    this->Connect(m_menuItemCloseAll->GetId(), wxEVT_COMMAND_MENU_SELECTED,
                  wxCommandEventHandler(MainFrameBaseClass::OnDeleteAllPages), NULL, this);
    this->Connect(m_menuItemMouseMiddleClickCLoses->GetId(), wxEVT_COMMAND_MENU_SELECTED,
                  wxCommandEventHandler(MainFrameBaseClass::OnMouseMiddleCloses), NULL, this);
    this->Connect(m_menuItemShowDropDownButton->GetId(), wxEVT_COMMAND_MENU_SELECTED,
                  wxCommandEventHandler(MainFrameBaseClass::OnShowFileListButton), NULL, this);
    this->Connect(m_menuItemShowDropDownButton->GetId(), wxEVT_UPDATE_UI,
                  wxUpdateUIEventHandler(MainFrameBaseClass::OnShowFileListButtonUI), NULL, this);
    this->Connect(Exit->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrameBaseClass::OnClose), NULL,
                  this);
}

MainFrameBaseClass::~MainFrameBaseClass()
{
    this->Disconnect(m_menuItem55->GetId(), wxEVT_COMMAND_MENU_SELECTED,
                     wxCommandEventHandler(MainFrameBaseClass::OnTopTabs), NULL, this);
    this->Disconnect(m_styleLeft->GetId(), wxEVT_COMMAND_MENU_SELECTED,
                     wxCommandEventHandler(MainFrameBaseClass::OnLeftTabs), NULL, this);
    this->Disconnect(m_styleRight->GetId(), wxEVT_COMMAND_MENU_SELECTED,
                     wxCommandEventHandler(MainFrameBaseClass::OnRightTabs), NULL, this);
    this->Disconnect(m_menuItemBottomTabs->GetId(), wxEVT_COMMAND_MENU_SELECTED,
                     wxCommandEventHandler(MainFrameBaseClass::OnBottomTabs), NULL, this);
    this->Disconnect(m_styleGTK3->GetId(), wxEVT_COMMAND_MENU_SELECTED,
                     wxCommandEventHandler(MainFrameBaseClass::OnTabStyle), NULL, this);
    this->Disconnect(m_styleMINIMAL->GetId(), wxEVT_COMMAND_MENU_SELECTED,
                     wxCommandEventHandler(MainFrameBaseClass::OnTabStyle), NULL, this);
    this->Disconnect(m_styleTRAPEZ->GetId(), wxEVT_COMMAND_MENU_SELECTED,
                     wxCommandEventHandler(MainFrameBaseClass::OnTabStyle), NULL, this);
    this->Disconnect(m_styleDEFAULT->GetId(), wxEVT_COMMAND_MENU_SELECTED,
                     wxCommandEventHandler(MainFrameBaseClass::OnTabStyle), NULL, this);
    this->Disconnect(m_menuItemMove->GetId(), wxEVT_COMMAND_MENU_SELECTED,
                     wxCommandEventHandler(MainFrameBaseClass::OnAllowTabMove), NULL, this);
    this->Disconnect(m_menuItemMove->GetId(), wxEVT_UPDATE_UI, wxUpdateUIEventHandler(MainFrameBaseClass::OnDnDUI),
                     NULL, this);
    this->Disconnect(m_menuItemShowCloseButton->GetId(), wxEVT_COMMAND_MENU_SELECTED,
                     wxCommandEventHandler(MainFrameBaseClass::OnShowCloseButton), NULL, this);
    this->Disconnect(m_menuItemCloseAll->GetId(), wxEVT_COMMAND_MENU_SELECTED,
                     wxCommandEventHandler(MainFrameBaseClass::OnDeleteAllPages), NULL, this);
    this->Disconnect(m_menuItemMouseMiddleClickCLoses->GetId(), wxEVT_COMMAND_MENU_SELECTED,
                     wxCommandEventHandler(MainFrameBaseClass::OnMouseMiddleCloses), NULL, this);
    this->Disconnect(m_menuItemShowDropDownButton->GetId(), wxEVT_COMMAND_MENU_SELECTED,
                     wxCommandEventHandler(MainFrameBaseClass::OnShowFileListButton), NULL, this);
    this->Disconnect(m_menuItemShowDropDownButton->GetId(), wxEVT_UPDATE_UI,
                     wxUpdateUIEventHandler(MainFrameBaseClass::OnShowFileListButtonUI), NULL, this);
    this->Disconnect(Exit->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrameBaseClass::OnClose),
                     NULL, this);
}

DemoAppImages::DemoAppImages()
    : wxImageList(16, 16, true)
    , m_imagesWidth(16)
    , m_imagesHeight(16)
{
    if(!bBitmapLoaded) {
        // We need to initialise the default bitmap handler
        wxXmlResource::Get()->AddHandler(new wxBitmapXmlHandler);
        wxC9ED9InitBitmapResources();
        bBitmapLoaded = true;
    }

    {
        wxBitmap bmp;
        wxIcon icn;
        bmp = wxXmlResource::Get()->LoadBitmap(wxT("blocks"));
        if(bmp.IsOk()) {
            if((m_imagesWidth == bmp.GetWidth()) && (m_imagesHeight == bmp.GetHeight())) {
                icn.CopyFromBitmap(bmp);
                this->Add(icn);
            }
            m_bitmaps.insert(std::make_pair(wxT("blocks"), bmp));
        }
    }

    {
        wxBitmap bmp;
        wxIcon icn;
        bmp = wxXmlResource::Get()->LoadBitmap(wxT("settings"));
        if(bmp.IsOk()) {
            if((m_imagesWidth == bmp.GetWidth()) && (m_imagesHeight == bmp.GetHeight())) {
                icn.CopyFromBitmap(bmp);
                this->Add(icn);
            }
            m_bitmaps.insert(std::make_pair(wxT("settings"), bmp));
        }
    }
}

DemoAppImages::~DemoAppImages() {}
