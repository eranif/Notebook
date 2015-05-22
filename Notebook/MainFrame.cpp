#include "MainFrame.h"
#include <wx/aboutdlg.h>
#include "Notebook.h"
#include <wx/msgdlg.h>
#include <wx/stdpaths.h>

MainFrame::MainFrame(wxWindow* parent)
    : MainFrameBaseClass(parent)
{
    DemoAppImages images;
    Notebook* book = new Notebook(m_mainPanel);
    m_mainPanel->GetSizer()->Insert(0, book, 1, wxEXPAND | wxALL, 0);
    m_mainPanel->GetSizer()->Layout();
    
    wxBitmap settings = images.Bitmap("settings");
    wxBitmap blocks = images.Bitmap("blocks");
    
    book->AddPage(new wxTextCtrl(book, wxID_ANY, "Page One", wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE),
                  "Page One",
                  false);
    book->AddPage(new wxTextCtrl(book, wxID_ANY, "Page Two", wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE),
                  "Page Two",
                  false);
    book->AddPage(new wxTextCtrl(book, wxID_ANY, "Page Three", wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE),
                  "Page Three",
                  false);
    book->AddPage(new wxPanel(book), "wxPanel", false);
    book->AddPage(new wxTextCtrl(book, wxID_ANY, "Page Five", wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE),
                  "Page Five",
                  false);
    book->AddPage(new wxTextCtrl(book, wxID_ANY, "Page Six", wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE),
                  "Page Six",
                  true);
    book->SetPageImage(0, blocks);
    book->SetPageText(0, "Colourful Blocks!");
    book->SetPageImage(4, settings);
    
    book->Bind(wxEVT_BOOK_PAGE_CHANGED, &MainFrame::OnPageChanged, this);
    book->Bind(wxEVT_BOOK_PAGE_CHANGING, &MainFrame::OnPageChanging, this);
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
}

void MainFrame::OnPageChanging(wxBookCtrlEvent& event)
{
    event.Skip();
    wxString message;
    message << "Page changing event. Selection=" << event.GetSelection();
    m_log->AppendText(message + "\n");
    //if(wxMessageBox("Allow?", "You can veto", wxYES_NO | wxCENTER | wxICON_QUESTION) != wxYES) {
    //    event.Veto();
    //}
}
