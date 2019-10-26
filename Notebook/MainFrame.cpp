#include "MainFrame.h"
#include "Notebook.h"
#include "clTabRendererClassic.h"
#include "clTabRendererCurved.h"
#include "clTabRendererGTK3.h"
#include "clTabRendererSquare.h"
#include <wx/aboutdlg.h>
#include <wx/msgdlg.h>
#include <wx/stdpaths.h>

MainFrame::MainFrame(wxWindow* parent)
    : MainFrameBaseClass(parent)
{
    DemoAppImages images;
    wxMenu* menu = new wxMenu;
    menu->Append(wxID_CLOSE);
    menu->AppendSeparator();
    menu->Append(wxID_COPY);
    menu->Append(wxID_PASTE);

    m_book = new Notebook(m_mainPanel, wxID_ANY, wxDefaultPosition, wxDefaultSize,
                          kNotebook_Default | kNotebook_UnderlineActiveTab);
    m_book->SetMenu(menu);

    m_mainPanel->GetSizer()->Insert(0, m_book, 1, wxEXPAND | wxALL, 0);
    m_mainPanel->GetSizer()->Layout();

    wxBitmap settings = images.Bitmap("settings");
    wxBitmap blocks = images.Bitmap("blocks");

    m_book->AddPage(
        new wxTextCtrl(m_book, wxID_ANY, "Colourful Blocks!", wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE),
        "Blocks", true);
    m_book->SetPageToolTip(0, _("What a colourful blocks!!"));

    m_book->AddPage(new wxTextCtrl(m_book, wxID_ANY, "Second Page", wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE),
                    "Second Page", true);
    m_book->AddPage(new wxTextCtrl(m_book, wxID_ANY, "Third Page", wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE),
                    "Third Page", true);
    m_book->AddPage(new wxPanel(m_book), "wxPanel", true);
    m_book->AddPage(new wxTextCtrl(m_book, wxID_ANY, "Fifth Page", wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE),
                    "Fifth Page", true);
    m_book->AddPage(new wxTextCtrl(m_book, wxID_ANY, "Sixth Page", wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE),
                    "Sixth Page", true);
    m_book->SetPageBitmap(0, blocks);
    m_book->SetPageBitmap(4, settings);

    m_book->Bind(wxEVT_BOOK_PAGE_CHANGED, &MainFrame::OnPageChanged, this);
    m_book->Bind(wxEVT_BOOK_PAGE_CHANGING, &MainFrame::OnPageChanging, this);
    m_book->Bind(wxEVT_BOOK_PAGE_CLOSING, &MainFrame::OnPageClosing, this);
    m_book->Bind(wxEVT_BOOK_PAGE_CLOSED, &MainFrame::OnPageClosed, this);
    m_book->Bind(wxEVT_BOOK_TABAREA_DCLICKED, &MainFrame::OnTabAreadDClicked, this);
    m_book->Bind(wxEVT_BOOK_TAB_DCLICKED, &MainFrame::OnTabDClicked, this);
    m_book->Bind(wxEVT_BOOK_FILELIST_BUTTON_CLICKED, &MainFrame::OnFileButtonClicked, this);
}

MainFrame::~MainFrame() {}

void MainFrame::OnExit(wxCommandEvent& event)
{
    wxUnusedVar(event);
    Close();
}

void MainFrame::OnAbout(wxCommandEvent& event)
{
    wxUnusedVar(event);
    wxAboutDialogInfo info;
    info.SetCopyright(_("My MainFrame"));
    info.SetLicence(_("GPL v2 or later"));
    info.SetDescription(_("Short description goes here"));
    ::wxAboutBox(info);
}

void MainFrame::OnPageChanged(wxBookCtrlEvent& event)
{
    event.Skip();
    wxString message;
    message << "Page changed event. Old=" << event.GetOldSelection() << ". New: =" << event.GetSelection();
    m_log->AppendText(message + "\n");
    wxWindow* curpage = m_book->GetCurrentPage();
    int sel = m_book->FindPage(curpage);
    if(sel != event.GetSelection()) {
        wxMessageBox("FATAL ERROR: current page does not match the selection!!", "Notebook Demo",
                     wxICON_ERROR | wxOK | wxCENTER);
    }
}

void MainFrame::OnPageChanging(wxBookCtrlEvent& event)
{
    event.Skip();
    wxString message;
    message << "Page changing event. Selection=" << event.GetSelection();
    m_log->AppendText(message + "\n");
    // if(wxMessageBox("Allow?", "You can veto", wxYES_NO | wxCENTER | wxICON_QUESTION) != wxYES) {
    //    event.Veto();
    //}
}
void MainFrame::OnClose(wxCommandEvent& event) { Close(); }
void MainFrame::OnTabStyle(wxCommandEvent& event)
{
    if(event.GetId() == ID_STYLE_DEFAULT) {
        m_book->SetArt(clTabRenderer::Ptr_t(new clTabRendererSquare()));
    } else if(event.GetId() == ID_STYLE_GTK3) {
        m_book->SetArt(clTabRenderer::Ptr_t(new clTabRendererGTK3()));
    } else if(event.GetId() == ID_STYLE_TRAPEZ) {
        m_book->SetArt(clTabRenderer::Ptr_t(new clTabRendererCurved()));
    } else if(event.GetId() == ID_STYLE_MINIMAL) {
        m_book->SetArt(clTabRenderer::Ptr_t(new clTabRendererClassic()));
    }
    m_book->SetStyle(m_book->GetStyle()); // trigger a layout
}

void MainFrame::OnShowCloseButton(wxCommandEvent& event)
{
    m_book->EnableStyle(kNotebook_CloseButtonOnActiveTab, event.IsChecked());
}

void MainFrame::OnAllowTabMove(wxCommandEvent& event) { m_book->EnableStyle(kNotebook_AllowDnD, event.IsChecked()); }
void MainFrame::OnDeleteAllPages(wxCommandEvent& event) { m_book->DeleteAllPages(); }

void MainFrame::OnPageClosed(wxBookCtrlEvent& event)
{
    m_log->AppendText(wxString() << "Page: " << event.GetSelection() << " closed"
                                 << "\n");
}

void MainFrame::OnPageClosing(wxBookCtrlEvent& event)
{
    m_log->AppendText(wxString() << "Page: " << event.GetSelection() << " is being closed"
                                 << "\n");
    // if(wxMessageBox("Allow?", "You can veto", wxYES_NO | wxCENTER | wxICON_QUESTION) != wxYES) {
    //    event.Veto();
    //}
}

void MainFrame::OnMouseMiddleCloses(wxCommandEvent& event)
{
    m_book->EnableStyle(kNotebook_MouseMiddleClickClosesTab, event.IsChecked());
}

void MainFrame::OnShowFileListButton(wxCommandEvent& event)
{
    m_book->EnableStyle(kNotebook_ShowFileListButton, event.IsChecked());
}

void MainFrame::OnNavigation(wxBookCtrlEvent& event)
{
    event.Skip();
    m_log->AppendText("Navigation event\n");
}

void MainFrame::OnTabAreadDClicked(wxBookCtrlEvent& event) { m_log->AppendText("Tab aread d-clicked!\n"); }

void MainFrame::OnTabDClicked(wxBookCtrlEvent& event)
{
    m_log->AppendText(wxString() << "Tab index=" << event.GetSelection() << " d-clicked!\n");
}
void MainFrame::OnDnDUI(wxUpdateUIEvent& event) { event.Check(m_book->GetStyle() & kNotebook_AllowDnD); }

void MainFrame::OnShowFileListButtonUI(wxUpdateUIEvent& event)
{
    event.Check(m_book->GetStyle() & kNotebook_ShowFileListButton);
}
void MainFrame::OnBottomTabs(wxCommandEvent& event) { m_book->SetTabDirection(wxBOTTOM); }
void MainFrame::OnTopTabs(wxCommandEvent& event) { m_book->SetTabDirection(wxTOP); }
void MainFrame::OnRightTabs(wxCommandEvent& event) { m_book->SetTabDirection(wxRIGHT); }
void MainFrame::OnLeftTabs(wxCommandEvent& event) { m_book->SetTabDirection(wxLEFT); }

void MainFrame::OnFileButtonClicked(wxContextMenuEvent& event)
{
    wxMenu menu;
    for(size_t i = 0; i < m_book->GetPageCount(); ++i) {
        menu.Append(wxID_ANY, m_book->GetPageText(i));
    }
    m_book->PopupMenu(&menu);
}
