#include "MainFrame.h"
#include <wx/aboutdlg.h>
#include "Notebook.h"

MainFrame::MainFrame(wxWindow* parent)
    : MainFrameBaseClass(parent)
{
    Notebook* book = new Notebook(m_mainPanel);
    m_mainPanel->GetSizer()->Add(book, 1, wxEXPAND | wxALL, 0);
    m_mainPanel->GetSizer()->Layout();

    book->AddPage(new wxTextCtrl(book, wxID_ANY, "Page One", wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE),
                  "Page One",
                  false);
    book->AddPage(
        new wxTextCtrl(book, wxID_ANY, "Page Two", wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE), "Page Two", false);
    book->AddPage(new wxTextCtrl(book, wxID_ANY, "Page Three", wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE),
                  "Page Three",
                  false);
    book->AddPage(new wxPanel(book), "wxPanel", true);
    book->AddPage(new wxTextCtrl(book, wxID_ANY, "Page Three", wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE),
                  "Page Five",
                  false);
    book->AddPage(new wxTextCtrl(book, wxID_ANY, "Page Three", wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE),
                  "Page Six",
                  false);
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
