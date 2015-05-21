#include "Notebook.h"
#include <wx/sizer.h>
#include <wx/dcbuffer.h>
#include <wx/dcgraph.h>

wxDEFINE_EVENT(wxEVT_BOOK_PAGE_CHANGING, wxBookCtrlEvent);
wxDEFINE_EVENT(wxEVT_BOOK_PAGE_CHANGED, wxBookCtrlEvent);

Notebook::Notebook(wxWindow* parent,
                   wxWindowID id,
                   const wxPoint& pos,
                   const wxSize& size,
                   long style,
                   const wxString& name)
    : wxPanel(parent, id, pos, size, style, name)
{
    wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
    SetSizer(sizer);

    m_header = new NotebookTabArea(this);
    sizer->Add(m_header, 0, wxEXPAND);
    m_book = new wxSimplebook(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, style);
    sizer->Add(m_book, 1, wxEXPAND);
    Layout();
}

Notebook::~Notebook() {}

void Notebook::AddPage(wxWindow* page, const wxString& label, bool selected, const wxBitmap& bmp)
{
    NotebookTab tab(page, label, bmp);
    tab.SetActive(selected);
    m_header->m_tabs.push_back(tab);
    page->Reparent(m_book);
    m_book->AddPage(page, label, selected);
    Refresh();
}

//----------------------------------------------------------
// Tab label
//----------------------------------------------------------
void NotebookTab::Draw(wxDC& dc)
{
    Colours colours;
    const int TOP_SMALL_HEIGHT = 2;
    wxColour bgColour(IsActive() ? colours.activeTabBgColour : colours.inactiveTabBgColour);
    wxColour penColour(IsActive() ? colours.activeTabPenColour : colours.inactiveTabPenColour);
    {
        wxPoint points[6];
        points[0] = m_rect.GetBottomLeft();

        points[1].x = points[0].x + ANGLE_WIDTH;
        points[1].y = m_rect.GetLeftTop().y + TOP_SMALL_HEIGHT;

        points[2].x = points[1].x + ANGLE_WIDTH_SMALL;
        points[2].y = points[1].y - TOP_SMALL_HEIGHT;

        points[3].x = points[0].x + (m_rect.GetWidth() - (ANGLE_WIDTH + ANGLE_WIDTH_SMALL));
        points[3].y = points[2].y;

        points[4].x = points[3].x + ANGLE_WIDTH_SMALL;
        points[4].y = points[3].y + TOP_SMALL_HEIGHT;

        points[5] = m_rect.GetBottomRight();

        dc.SetPen(penColour);
        dc.SetBrush(bgColour);
        dc.DrawPolygon(6, points);
    }

    {
        wxPoint points[6];
        points[0] = m_rect.GetBottomLeft();
        points[0].x += 1;

        points[1].x = points[0].x + ANGLE_WIDTH;
        points[1].y = m_rect.GetLeftTop().y + TOP_SMALL_HEIGHT + 1;

        points[2].x = points[1].x + ANGLE_WIDTH_SMALL;
        points[2].y = points[1].y - TOP_SMALL_HEIGHT;

        points[3].x = points[0].x + (m_rect.GetWidth() - 2 - (ANGLE_WIDTH + ANGLE_WIDTH_SMALL));
        points[3].y = points[2].y;

        points[4].x = points[3].x + ANGLE_WIDTH_SMALL;
        points[4].y = points[3].y + TOP_SMALL_HEIGHT;

        points[5] = m_rect.GetBottomRight();
        points[5].x -= 2;

        dc.SetPen(IsActive() ? penColour.ChangeLightness(60) : penColour.ChangeLightness(80));
        dc.SetBrush(*wxTRANSPARENT_BRUSH);
        dc.DrawPolygon(6, points);
    }

    // Draw the text
    dc.SetTextForeground(IsActive() ? colours.activeTabTextColour : colours.inactiveTabTextColour);
    dc.DrawText(m_label, m_textX + m_rect.GetX(), m_textY);
}

//----------------------------------------------------------
// Notebook header
//----------------------------------------------------------
NotebookTabArea::NotebookTabArea(wxWindow* notebook)
    : wxPanel(notebook)
    , m_height(34)
{
    SetSizeHints(wxSize(-1, m_height));
    SetSize(-1, m_height);
    Bind(wxEVT_PAINT, &NotebookTabArea::OnPaint, this);
    Bind(wxEVT_ERASE_BACKGROUND, &NotebookTabArea::OnEraseBG, this);
    Bind(wxEVT_SIZE, &NotebookTabArea::OnSize, this);
    Bind(wxEVT_LEFT_DOWN, &NotebookTabArea::OnLeftDown, this);
}

NotebookTabArea::~NotebookTabArea()
{
    Unbind(wxEVT_PAINT, &NotebookTabArea::OnPaint, this);
    Unbind(wxEVT_ERASE_BACKGROUND, &NotebookTabArea::OnEraseBG, this);
    Unbind(wxEVT_SIZE, &NotebookTabArea::OnSize, this);
    Unbind(wxEVT_LEFT_DOWN, &NotebookTabArea::OnLeftDown, this);
}

void NotebookTabArea::OnSize(wxSizeEvent& event)
{
    event.Skip();
    Refresh();
}
void NotebookTabArea::OnEraseBG(wxEraseEvent& e) { wxUnusedVar(e); }

void NotebookTabArea::OnPaint(wxPaintEvent& e)
{
    wxBufferedPaintDC dc(this);
    wxRect clientRect(GetClientRect());
    wxRect rect(GetClientRect());
    rect.Deflate(2);

    // Draw background
    NotebookTab::Colours colours;
    dc.SetPen(colours.tabAreaColour);
    dc.SetBrush(colours.tabAreaColour);
    dc.DrawRectangle(GetClientRect());

    wxBitmap bmpTabs(rect.GetSize());
    wxMemoryDC memDC(bmpTabs);
    wxGCDC gcdc(memDC);

    gcdc.SetPen(colours.tabAreaColour);
    gcdc.SetBrush(colours.tabAreaColour);
    gcdc.DrawRectangle(rect.GetSize());

    // Loop over the tabs and set their cooridnates
    DoUpdateCoordiantes(m_tabs);
    // Get list of tabs that can fit the screen
    NotebookTab::Vec_t tabs = GetTabsToDraw();
    // Update the tabs coordinates
    DoUpdateCoordiantes(tabs);
    int activeTabInex = wxNOT_FOUND;
    for(int i = (tabs.size() - 1); i >= 0; --i) {
        NotebookTab& tab = tabs.at(i);
        if(tab.IsActive()) {
            activeTabInex = i;
        }
        tab.Draw(gcdc);
    }

    // Redraw the active tab
    if(activeTabInex != wxNOT_FOUND) {
        tabs.at(activeTabInex).Draw(gcdc);
    }

    memDC.SelectObject(wxNullBitmap);
    dc.DrawBitmap(bmpTabs, 2, 2);

    if(activeTabInex != wxNOT_FOUND) {
        const NotebookTab& activeTab = tabs.at(activeTabInex);
        // Draw 3 lines at the bottom
        dc.SetPen(colours.activeTabPenColour);
        dc.SetBrush(colours.activeTabBgColour);
        wxPoint topLeft = clientRect.GetBottomLeft();
        wxSize rectSize(clientRect.width, 6);
        topLeft.y -= rectSize.GetHeight() - 1;
        wxRect bottomRect(topLeft, rectSize);
        // We intentionally move the rect out of the client rect
        // so the left and right lines will be drawn out of screen
        bottomRect.x -= 1;
        bottomRect.width += 2;
        dc.DrawRectangle(bottomRect);

        // Draw a line under the active tab
        // that will erase the line drawn by the above rect
        wxPoint from, to;
        from = activeTab.GetRect().GetBottomLeft();
        to = activeTab.GetRect().GetBottomRight();
        from.y = bottomRect.GetTopLeft().y;
        to.y = from.y;
        from.x += NotebookTab::ANGLE_WIDTH_SMALL + 1;
        to.x += 2;

        dc.SetPen(colours.activeTabBgColour);
        dc.DrawLine(from, to);
    }
}

void NotebookTabArea::DoUpdateCoordiantes(NotebookTab::Vec_t& tabs)
{
    int xx = 5;
    for(size_t i = 0; i < tabs.size(); ++i) {
        NotebookTab& tab = tabs.at(i);
        tab.GetRect().SetX(xx);
        tab.GetRect().SetY(0);
        xx += tab.GetRect().GetWidth() - NotebookTab::ANGLE_WIDTH;
    }
}

NotebookTab::Vec_t NotebookTabArea::GetTabsToDraw()
{
    wxRect clientRect(GetClientRect());
    NotebookTab::Vec_t tabs;
    size_t i = 0;
    bool activeTabIndcluded = false;
    for(; i < m_tabs.size(); ++i) {
        const NotebookTab& tab = m_tabs.at(i);
        if(clientRect.Intersects(tab.GetRect())) {
            tabs.push_back(tab);
            if(!activeTabIndcluded) {
                activeTabIndcluded = tab.IsActive();
            }
        } else {
            break;
        }
    }

    if(!activeTabIndcluded) {
        // we got a list of tabs, but we don't have the active tab
        // start removing tabs from the start and push the tabs inside
        for(; i < m_tabs.size(); ++i) {
            if(!tabs.empty()) {
                const NotebookTab& firstTab = tabs.at(0);
                tabs.erase(tabs.begin());
            }
            const NotebookTab& tab = m_tabs.at(i);
            tabs.push_back(tab);
            if(tab.IsActive()) {
                break;
            }
        }
    }
    return tabs;
}

void NotebookTabArea::OnLeftDown(wxMouseEvent& event)
{
    event.Skip();

    // Get list of visible tabs
    NotebookTab::Vec_t tabs = GetTabsToDraw();
    // Update the tabs coordinates
    DoUpdateCoordiantes(tabs);

    wxPoint pt = event.GetPosition();
    for(size_t i = 0; i < tabs.size(); ++i) {
        if(tabs.at(i).GetRect().Contains(pt)) {
            SetSelection(i);
            break;
        }
    }
}

void NotebookTabArea::ChangeSelection(size_t tabIdx)
{
    if(tabIdx >= m_tabs.size()) return; // cant happen...
    for(size_t i = 0; i < m_tabs.size(); ++i) {
        NotebookTab& tab = m_tabs.at(i);
        tab.SetActive(i == tabIdx);
    }
    static_cast<Notebook*>(GetParent())->m_book->ChangeSelection(tabIdx);
    Refresh();
}

void NotebookTabArea::SetSelection(size_t tabIdx)
{
    int oldSelection = GetSelection();
    {
        wxBookCtrlEvent event(wxEVT_BOOK_PAGE_CHANGING);
        event.SetEventObject(this);
        event.SetSelection(oldSelection);
        event.SetOldSelection(wxNOT_FOUND);
        GetParent()->GetEventHandler()->ProcessEvent(event);

        if(!event.IsAllowed()) {
            return; // Vetoed by the user
        }
    }
    ChangeSelection(tabIdx);

    // Fire an event
    {
        wxBookCtrlEvent event(wxEVT_BOOK_PAGE_CHANGED);
        event.SetEventObject(this);
        event.SetSelection(GetSelection());
        event.SetOldSelection(oldSelection);
        GetParent()->GetEventHandler()->AddPendingEvent(event);
    }
}

int NotebookTabArea::GetSelection() const
{
    for(size_t i = 0; i < m_tabs.size(); ++i) {
        const NotebookTab& tab = m_tabs.at(i);
        if(tab.IsActive()) return i;
    }
    return wxNOT_FOUND;
}
