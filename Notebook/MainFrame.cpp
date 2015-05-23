#include "MainFrame.h"
#include <wx/aboutdlg.h>
#include "Notebook.h"
#include <wx/msgdlg.h>
#include <wx/stdpaths.h>

MainFrame::MainFrame(wxWindow* parent)
    : MainFrameBaseClass(parent)
{
    DemoAppImages images;
    m_book = new Notebook(m_mainPanel, wxID_ANY, wxDefaultPosition, wxDefaultSize, kNotebook_LightTabs);
    m_mainPanel->GetSizer()->Insert(0, m_book, 1, wxEXPAND | wxALL, 0);
    m_mainPanel->GetSizer()->Layout();

    wxBitmap settings = images.Bitmap("settings");
    wxBitmap blocks = images.Bitmap("blocks");

    m_book->AddPage(new wxTextCtrl(m_book, wxID_ANY, "Page One", wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE),
                    "Page One",
                    false);
    m_book->AddPage(new wxTextCtrl(m_book, wxID_ANY, "Page Two", wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE),
                    "Page Two",
                    false);
    m_book->AddPage(new wxTextCtrl(m_book, wxID_ANY, "Page Three", wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE),
                    "Page Three",
                    false);
    m_book->AddPage(new wxPanel(m_book), "wxPanel", false);
    m_book->AddPage(new wxTextCtrl(m_book, wxID_ANY, "Page Five", wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE),
                    "Page Five",
                    false);
    m_book->AddPage(new wxTextCtrl(m_book, wxID_ANY, "Page Six", wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE),
                    "Page Six",
                    true);
    m_book->SetPageBitmap(0, blocks);
    m_book->SetPageText(0, "Colourful Blocks!");
    m_book->SetPageBitmap(4, settings);

    m_book->Bind(wxEVT_BOOK_PAGE_CHANGED, &MainFrame::OnPageChanged, this);
    m_book->Bind(wxEVT_BOOK_PAGE_CHANGING, &MainFrame::OnPageChanging, this);
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
        wxMessageBox("FATAL ERROR: current page does not match the selection!!",
                     "Notebook Demo",
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
    size_t style = m_book->GetStyle();
    if(event.GetId() == ID_STYLE_DARK) {
        style &= ~kNotebook_LightTabs;
        style |= kNotebook_DarkTabs;
    } else {
        style &= ~kNotebook_DarkTabs;
        style |= kNotebook_LightTabs;
    }
    m_book->SetStyle(style);
}

void MainFrame::OnShowCloseButton(wxCommandEvent& event)
{
    size_t style = m_book->GetStyle();
    if(event.IsChecked()) {
        style |= kNotebook_CloseButtonOnActiveTab;
    } else {
        style &= ~kNotebook_CloseButtonOnActiveTab;
    }
    m_book->SetStyle(style);
}

void MainFrame::OnAllowTabMove(wxCommandEvent& event)
{
    size_t style = m_book->GetStyle();
    if(event.IsChecked()) {
        style |= kNotebook_AllowDnD;
    } else {
        style &= ~kNotebook_AllowDnD;
    }
    m_book->SetStyle(style);
}
void MainFrame::OnDeleteAllPages(wxCommandEvent& event)
{
    m_book->DeleteAllPages();
}
