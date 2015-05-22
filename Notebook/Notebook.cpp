#include "Notebook.h"
#include <wx/sizer.h>
#include <wx/dcbuffer.h>
#include <wx/dcgraph.h>
#include <wx/wupdlock.h>
#include <algorithm>
#include <wx/image.h>
#include <wx/xrc/xmlres.h>
#include <wx/xrc/xh_bmp.h>

wxDEFINE_EVENT(wxEVT_BOOK_PAGE_CHANGING, wxBookCtrlEvent);
wxDEFINE_EVENT(wxEVT_BOOK_PAGE_CHANGED, wxBookCtrlEvent);
wxDEFINE_EVENT(wxEVT_BOOK_PAGE_CLOSING, wxBookCtrlEvent);
wxDEFINE_EVENT(wxEVT_BOOK_PAGE_CLOSED, wxBookCtrlEvent);

extern void Notebook_Init_Bitmaps();

Notebook::Notebook(wxWindow* parent,
                   wxWindowID id,
                   const wxPoint& pos,
                   const wxSize& size,
                   long style,
                   const wxString& name)
    : wxPanel(parent, id, pos, size, style, name)
{
    static bool once = false;
    if(!once) {
        // Add PNG and Bitmap handler
        wxImage::AddHandler(new wxPNGHandler);
        wxXmlResource::Get()->AddHandler(new wxBitmapXmlHandler);
        Notebook_Init_Bitmaps();
        once = true;
    }

    wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
    SetSizer(sizer);

    m_header = new NotebookTabArea(this, style);
    sizer->Add(m_header, 0, wxEXPAND);
    m_book = new wxSimplebook(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0);
    sizer->Add(m_book, 1, wxEXPAND);
    Layout();
}

Notebook::~Notebook() {}

void Notebook::AddPage(wxWindow* page, const wxString& label, bool selected, const wxBitmap& bmp)
{
    NotebookTab::Ptr_t tab(new NotebookTab(GetStyle(), page, label, bmp));
    tab->SetActive(selected, GetStyle());
    m_header->AddPage(tab);
}

void Notebook::DoChangeSelection(wxWindow* page)
{
    for(size_t i = 0; i < m_book->GetPageCount(); ++i) {
        if(m_book->GetPage(i) == page) {
            m_book->ChangeSelection(i);
            break;
        }
    }
}

bool Notebook::InsertPage(size_t index, wxWindow* page, const wxString& label, bool selected, const wxBitmap& bmp)
{
    NotebookTab::Ptr_t tab(new NotebookTab(GetStyle(), page, label, bmp));
    tab->SetActive(selected, GetStyle());
    return m_header->InsertPage(index, tab);
}

void Notebook::SetStyle(size_t style)
{
    m_header->SetStyle(style);
    Refresh();
}

wxWindow* Notebook::GetCurrentPage() const
{
    if(m_header->GetSelection() == wxNOT_FOUND) return NULL;
    return m_header->GetPage(m_header->GetSelection());
}

int Notebook::FindPage(wxWindow* page) const { return m_header->FindPage(page); }

bool Notebook::RemovePage(size_t page) { return m_header->RemovePage(page, false, false); }
bool Notebook::DeletePage(size_t page) { return m_header->RemovePage(page, true, true); }

//----------------------------------------------------------
// Tab label
//----------------------------------------------------------
void NotebookTab::Draw(wxDC& dc, const NotebookTab::Colours& colours, size_t style)
{
    const int TOP_SMALL_HEIGHT = 2;
    wxColour bgColour(IsActive() ? colours.activeTabBgColour : colours.inactiveTabBgColour);
    wxColour penColour(IsActive() ? colours.activeTabPenColour : colours.inactiveTabPenColour);
    {
        wxPoint points[6];
        points[0] = m_rect.GetBottomLeft();

        points[1].x = points[0].x + MAJOR_CURVE_WIDTH;
        points[1].y = m_rect.GetLeftTop().y + TOP_SMALL_HEIGHT;

        points[2].x = points[1].x + SMALL_CURVE_WIDTH;
        points[2].y = points[1].y - TOP_SMALL_HEIGHT;

        points[3].x = points[0].x + (m_rect.GetWidth() - (MAJOR_CURVE_WIDTH + SMALL_CURVE_WIDTH));
        points[3].y = points[2].y;

        points[4].x = points[3].x + SMALL_CURVE_WIDTH;
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

        points[1].x = points[0].x + MAJOR_CURVE_WIDTH;
        points[1].y = m_rect.GetLeftTop().y + TOP_SMALL_HEIGHT + 1;

        points[2].x = points[1].x + SMALL_CURVE_WIDTH;
        points[2].y = points[1].y - TOP_SMALL_HEIGHT;

        points[3].x = points[0].x + (m_rect.GetWidth() - 2 - (MAJOR_CURVE_WIDTH + SMALL_CURVE_WIDTH));
        points[3].y = points[2].y;

        points[4].x = points[3].x + SMALL_CURVE_WIDTH;
        points[4].y = points[3].y + TOP_SMALL_HEIGHT;

        points[5] = m_rect.GetBottomRight();
        points[5].x -= 2;

        dc.SetPen(IsActive() ? colours.activeTabInnerPenColour : colours.inactiveTabInnerPenColour);
        dc.SetBrush(bgColour);
        dc.DrawPolygon(6, points);
    }

    // Draw bitmap
    if(m_bitmap.IsOk()) {
        dc.DrawBitmap(m_bitmap, m_bmpX + m_rect.GetX(), m_bmpY);
    }

    // Draw the text
    dc.SetTextForeground(IsActive() ? colours.activeTabTextColour : colours.inactiveTabTextColour);
    wxFont font = wxSystemSettings::GetFont(wxSYS_DEFAULT_GUI_FONT);
#ifdef __WXOSX__
    font.SetPointSize(10);
#else
    font.SetPointSize(9);
#endif

    dc.SetFont(font);
    dc.DrawText(m_label, m_textX + m_rect.GetX(), m_textY);

    if(IsActive() && (style & kNotebook_CloseButtonOnActiveTab)) {
        dc.DrawBitmap(colours.closeButton, m_bmpCloseX + m_rect.GetX(), m_bmpCloseY);
    }
}

void NotebookTab::CalculateOffsets(size_t style)
{
    wxBitmap b(1, 1);
    wxMemoryDC memDC(b);
    m_bmpCloseX = wxNOT_FOUND;
    m_bmpCloseY = wxNOT_FOUND;

    wxFont font = wxSystemSettings::GetFont(wxSYS_DEFAULT_GUI_FONT);
    font.SetPointSize(10);
    memDC.SetFont(font);
    wxSize sz = memDC.GetTextExtent(m_label);
    m_height = TAB_HEIGHT;

    m_width = 0;
    m_width += MAJOR_CURVE_WIDTH;
    m_width += SMALL_CURVE_WIDTH;
    m_width += X_SPACER;

    // bitmap
    m_bmpX = wxNOT_FOUND;
    m_bmpY = wxNOT_FOUND;
    if(m_bitmap.IsOk()) {
        m_bmpX = m_width;
        m_width += m_bitmap.GetWidth();
        m_width += X_SPACER;
        m_bmpY = ((m_height - m_bitmap.GetHeight()) / 2);
    }

    // Text
    m_textX = m_width;
    m_textY = ((m_height - sz.y) / 2);
    m_width += sz.x;

    // x button
    if((style & kNotebook_CloseButtonOnActiveTab)) {
        m_width += X_SPACER;
        m_bmpCloseX = m_width;
        m_bmpCloseY = ((m_height - 12) / 2);
        m_width += 12; // X button is 10 pixels in size
    }
    m_width += X_SPACER;
    m_width += SMALL_CURVE_WIDTH;
    m_width += MAJOR_CURVE_WIDTH;

    // Update the rect width
    m_rect.SetWidth(m_width);
}

void NotebookTab::SetBitmap(const wxBitmap& bitmap, size_t style)
{
    this->m_bitmap = bitmap;
    CalculateOffsets(style);
}

void NotebookTab::SetLabel(const wxString& label, size_t style)
{
    this->m_label = label;
    CalculateOffsets(style);
}

void NotebookTab::SetActive(bool active, size_t style)
{
    this->m_active = active;
    CalculateOffsets(style);
}

bool NotebookTabArea::ShiftRight(NotebookTab::Vec_t& tabs)
{
    // Move the first tab from the list and adjust the remainder
    // of the tabs x coordiate
    if(!tabs.empty()) {
        NotebookTab::Ptr_t t = tabs.at(0);
        int width = t->GetWidth();
        tabs.erase(tabs.begin() + 0);

        for(size_t i = 0; i < tabs.size(); ++i) {
            NotebookTab::Ptr_t t = tabs.at(i);
            t->GetRect().SetX(t->GetRect().x - width + NotebookTab::MAJOR_CURVE_WIDTH);
        }
        return true;
    }
    return false;
}

bool NotebookTabArea::IsActiveTabInList(const NotebookTab::Vec_t& tabs) const
{
    for(size_t i = 0; i < tabs.size(); ++i) {
        if(tabs.at(i)->IsActive()) return true;
    }
    return false;
}

bool NotebookTabArea::IsActiveTabVisible(const NotebookTab::Vec_t& tabs) const
{
    wxRect clientRect(GetClientRect());
    for(size_t i = 0; i < tabs.size(); ++i) {
        NotebookTab::Ptr_t t = tabs.at(i);
        if(t->IsActive() && clientRect.Intersects(t->GetRect())) return true;
    }
    return false;
}

//----------------------------------------------------------
// Notebook header
//----------------------------------------------------------
NotebookTabArea::NotebookTabArea(wxWindow* notebook, size_t style)
    : wxPanel(notebook)
    , m_height(NotebookTab::TAB_HEIGHT)
    , m_style(style)
{
    SetSizeHints(wxSize(-1, m_height));
    SetSize(-1, m_height);
    Bind(wxEVT_PAINT, &NotebookTabArea::OnPaint, this);
    Bind(wxEVT_ERASE_BACKGROUND, &NotebookTabArea::OnEraseBG, this);
    Bind(wxEVT_SIZE, &NotebookTabArea::OnSize, this);
    Bind(wxEVT_LEFT_DOWN, &NotebookTabArea::OnLeftDown, this);
    Bind(wxEVT_LEFT_UP, &NotebookTabArea::OnLeftUp, this);
    Bind(wxEVT_MOTION, &NotebookTabArea::OnMouseMotion, this);
    if(m_style & kNotebook_DarkTabs) {
        m_colours.InitDarkColours();
    } else {
        m_colours.InitLightColours();
    }
}

NotebookTabArea::~NotebookTabArea()
{
    Unbind(wxEVT_PAINT, &NotebookTabArea::OnPaint, this);
    Unbind(wxEVT_ERASE_BACKGROUND, &NotebookTabArea::OnEraseBG, this);
    Unbind(wxEVT_SIZE, &NotebookTabArea::OnSize, this);
    Unbind(wxEVT_LEFT_DOWN, &NotebookTabArea::OnLeftDown, this);
    Unbind(wxEVT_LEFT_UP, &NotebookTabArea::OnLeftUp, this);
    Unbind(wxEVT_MOTION, &NotebookTabArea::OnMouseMotion, this);
}

void NotebookTabArea::OnSize(wxSizeEvent& event)
{
    event.Skip();
    m_visibleTabs.clear();
    Refresh();
}

void NotebookTabArea::OnEraseBG(wxEraseEvent& e) { wxUnusedVar(e); }

void NotebookTabArea::OnPaint(wxPaintEvent& e)
{
    wxBufferedPaintDC dc(this);
    wxRect clientRect(GetClientRect());
    if(clientRect.width <= 3) return;

    wxRect rect(GetClientRect());
    rect.Deflate(0);

    // Reudce the length of the tabs bitmap by 16 pixles (we will draw there the drop down
    // button)
    rect.SetWidth(rect.GetWidth() - 16);

    // Draw background
    dc.SetPen(m_colours.tabAreaColour);
    dc.SetBrush(m_colours.tabAreaColour);
    dc.DrawRectangle(GetClientRect());

    for(size_t i = 0; i < m_tabs.size(); ++i) {
        m_tabs.at(i)->CalculateOffsets(GetStyle());
    }

    if(rect.GetSize().x > 0) {
        wxBitmap bmpTabs(rect.GetSize());
        wxMemoryDC memDC(bmpTabs);
        wxGCDC gcdc(memDC);

        gcdc.SetPen(m_colours.tabAreaColour);
        gcdc.SetBrush(m_colours.tabAreaColour);
        gcdc.DrawRectangle(rect.GetSize());

        UpdateVisibleTabs();

        int activeTabInex = wxNOT_FOUND;
        for(int i = (m_visibleTabs.size() - 1); i >= 0; --i) {
            NotebookTab::Ptr_t tab = m_visibleTabs.at(i);
            if(tab->IsActive()) {
                activeTabInex = i;
            }
            tab->Draw(gcdc, m_colours, m_style);
        }

        // Redraw the active tab
        if(activeTabInex != wxNOT_FOUND) {
            m_visibleTabs.at(activeTabInex)->Draw(gcdc, m_colours, m_style);
        }

        memDC.SelectObject(wxNullBitmap);
        dc.DrawBitmap(bmpTabs, 0, 0);

        if(activeTabInex != wxNOT_FOUND) {
            NotebookTab::Ptr_t activeTab = m_visibleTabs.at(activeTabInex);
            // Draw 3 lines at the bottom
            dc.SetPen(m_colours.activeTabPenColour);
            dc.SetBrush(m_colours.activeTabBgColour);
            wxPoint topLeft = clientRect.GetBottomLeft();
            wxSize rectSize(clientRect.width, NotebookTab::BOTTOM_AREA_HEIGHT);
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
            from = activeTab->GetRect().GetBottomLeft();
            to = activeTab->GetRect().GetBottomRight();
            from.y -= NotebookTab::BOTTOM_AREA_HEIGHT - 1;
            from.x += 2;
            to.y -= NotebookTab::BOTTOM_AREA_HEIGHT - 1;
            to.x -= 1;

            // from.y = bottomRect.GetTopLeft().y;
            // to.y = from.y;
            // from.x += NotebookTab::ANGLE_WIDTH_SMALL + 1;
            // to.x += 2;

            dc.SetPen(m_colours.activeTabBgColour);
            dc.DrawLine(from, to);
#ifdef __WXOSX__
            dc.DrawLine(from, to);
            dc.DrawLine(from, to);
            dc.DrawLine(from, to);
#endif
        }
    } else {
        m_visibleTabs.clear();
    }
}

void NotebookTabArea::DoUpdateCoordiantes(NotebookTab::Vec_t& tabs)
{
    int xx = 5;
    for(size_t i = 0; i < tabs.size(); ++i) {
        NotebookTab::Ptr_t tab = tabs.at(i);
        tab->GetRect().SetX(xx);
        tab->GetRect().SetY(0);
        tab->GetRect().SetWidth(tab->GetWidth());
        tab->GetRect().SetHeight(tab->GetHeight());
        xx += tab->GetWidth() - NotebookTab::MAJOR_CURVE_WIDTH;
    }
}

void NotebookTabArea::UpdateVisibleTabs()
{
    // dont update the list if we dont need to
    if(IsActiveTabInList(m_visibleTabs) && IsActiveTabVisible(m_visibleTabs)) return;

    // set the physical coords for each tab (we do this for all the tabs)
    DoUpdateCoordiantes(m_tabs);

    // Start shiffting right tabs until the active tab is visible
    m_visibleTabs = m_tabs;
    while(!IsActiveTabVisible(m_visibleTabs)) {
        if(!ShiftRight(m_visibleTabs)) break;
    }
}

void NotebookTabArea::OnLeftDown(wxMouseEvent& event)
{
    event.Skip();

    // Get list of visible tabs
    bool clickWasOnActiveTab = false;
    for(size_t i = 0; i < m_visibleTabs.size(); ++i) {
        NotebookTab::Ptr_t t = m_visibleTabs.at(i);
        if(t->GetRect().Contains(event.GetPosition()) && !t->IsActive()) {
            SetSelection(DoGetPageIndex(t->GetWindow()));
            return;

        } else if(t->GetRect().Contains(event.GetPosition())) {
            clickWasOnActiveTab = true;
            break;
        }
    }

    // We clicked on the active tab, start DnD operation
    if((m_style & kNotebook_AllowDnD) && clickWasOnActiveTab) {
    }
}

void NotebookTabArea::ChangeSelection(size_t tabIdx)
{
    wxWindowUpdateLocker locker(GetParent());
    if(!IsIndexValid(tabIdx)) return;

    for(size_t i = 0; i < m_tabs.size(); ++i) {
        NotebookTab::Ptr_t tab = m_tabs.at(i);
        tab->SetActive((i == tabIdx), GetStyle());
    }

    NotebookTab::Ptr_t activeTab = GetActiveTabInfo();
    if(activeTab) {
        static_cast<Notebook*>(GetParent())->DoChangeSelection(activeTab->GetWindow());
    }
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
        NotebookTab::Ptr_t tab = m_tabs.at(i);
        if(tab->IsActive()) return i;
    }
    return wxNOT_FOUND;
}

NotebookTab::Ptr_t NotebookTabArea::GetTabInfo(size_t index)
{
    if(!IsIndexValid(index)) return NotebookTab::Ptr_t(NULL);
    return m_tabs.at(index);
}

NotebookTab::Ptr_t NotebookTabArea::GetTabInfo(size_t index) const
{
    if(!IsIndexValid(index)) return NotebookTab::Ptr_t(NULL);
    return m_tabs.at(index);
}

NotebookTab::Ptr_t NotebookTabArea::GetTabInfo(wxWindow* page)
{
    for(size_t i = 0; i < m_tabs.size(); ++i) {
        NotebookTab::Ptr_t tab = m_tabs.at(i);
        if(tab->GetWindow() == page) return tab;
    }
    return NotebookTab::Ptr_t(NULL);
}

bool NotebookTabArea::SetPageText(size_t page, const wxString& text)
{
    NotebookTab::Ptr_t tab = GetTabInfo(page);
    if(!tab) return false;
    tab->SetLabel(text, GetStyle());
    Refresh();
    return true;
}

NotebookTab::Ptr_t NotebookTabArea::GetActiveTabInfo()
{
    for(size_t i = 0; i < m_tabs.size(); ++i) {
        if(m_tabs.at(i)->IsActive()) {
            return m_tabs.at(i);
        }
    }
    return NotebookTab::Ptr_t(NULL);
}

void NotebookTabArea::AddPage(NotebookTab::Ptr_t tab) { InsertPage(m_tabs.size(), tab); }

wxSimplebook* NotebookTabArea::GetBook() { return reinterpret_cast<Notebook*>(GetParent())->m_book; }

bool NotebookTabArea::InsertPage(size_t index, NotebookTab::Ptr_t tab)
{
    if(index > m_tabs.size()) return false;
    m_tabs.insert(m_tabs.begin() + index, tab);

    int tabIndex = index;
    tab->GetWindow()->Reparent(GetBook());
    GetBook()->InsertPage(index, tab->GetWindow(), tab->GetLabel(), tab->IsActive());
    NotebookTab::Ptr_t activeTab = GetActiveTabInfo();
    if(!activeTab) {
        // No active tab??
        ChangeSelection(0);
    }
    if(tab->IsActive()) {
        ChangeSelection(tabIndex);
    }
    Refresh();
    return true;
}

wxString NotebookTabArea::GetPageText(size_t page) const
{
    NotebookTab::Ptr_t tab = GetTabInfo(page);
    if(tab) return tab->GetLabel();
    return "";
}

wxBitmap NotebookTabArea::GetPageImage(size_t index) const
{
    NotebookTab::Ptr_t tab = GetTabInfo(index);
    if(tab) return tab->GetBitmap();
    return wxNullBitmap;
}

void NotebookTabArea::SetPageImage(size_t index, const wxBitmap& bmp)
{
    NotebookTab::Ptr_t tab = GetTabInfo(index);
    if(tab) {
        tab->SetBitmap(bmp, GetStyle());
        Refresh();
    }
}

void NotebookTabArea::OnLeftUp(wxMouseEvent& event)
{
    event.Skip();

    int tabHit, realPos;
    TestPoint(event.GetPosition(), realPos, tabHit);
    if(tabHit != wxNOT_FOUND) {
        if((GetStyle() & kNotebook_CloseButtonOnActiveTab) && m_visibleTabs.at(tabHit)->IsActive()) {
            // we clicked on the selected index
            NotebookTab::Ptr_t t = m_visibleTabs.at(tabHit);
            wxRect xRect(t->GetRect().x + t->GetBmpCloseX(), t->GetRect().y + t->GetBmpCloseY(), 16, 16);
            if(xRect.Contains(event.GetPosition())) {
                CallAfter(&NotebookTabArea::DoDeletePage, realPos);
            }
        }
    }
}

void NotebookTabArea::OnMouseMotion(wxMouseEvent& event) { event.Skip(); }

void NotebookTabArea::TestPoint(const wxPoint& pt, int& realPosition, int& tabHit)
{
    realPosition = wxNOT_FOUND;
    tabHit = wxNOT_FOUND;

    if(m_visibleTabs.empty()) return;

    for(size_t i = 0; i < m_visibleTabs.size(); ++i) {
        NotebookTab::Ptr_t tab = m_visibleTabs.at(i);
        if(tab->GetRect().Contains(pt)) {
            tabHit = i;
            realPosition = DoGetPageIndex(tab->GetWindow());
            return;
        }
    }
}

void NotebookTabArea::SetStyle(size_t style)
{
    this->m_style = style;
    if(style & kNotebook_DarkTabs) {
        m_colours.InitDarkColours();
    } else {
        m_colours.InitLightColours();
    }

    for(size_t i = 0; i < m_tabs.size(); ++i) {
        m_tabs.at(i)->CalculateOffsets(GetStyle());
    }
    m_visibleTabs.clear();
    Refresh();
}

wxWindow* NotebookTabArea::GetPage(size_t index) const
{
    NotebookTab::Ptr_t tab = GetTabInfo(index);
    if(tab) return tab->GetWindow();
    return NULL;
}

bool NotebookTabArea::IsIndexValid(size_t index) const { return (index < m_tabs.size()); }

NotebookTab::Colours::Colours() { InitDarkColours(); }

void NotebookTab::Colours::InitDarkColours()
{
    activeTabTextColour = "WHITE";
    activeTabBgColour = wxColour("#211e1e");
    activeTabPenColour = wxColour("#0e0d0d");
    activeTabInnerPenColour = wxColour("#343131");

    inactiveTabTextColour = wxColour("rgb(200, 200, 200)");
    inactiveTabBgColour = wxColour("#393838");
    inactiveTabPenColour = wxColour("#100f0f");
    inactiveTabInnerPenColour = wxColour("#535252");

    tabAreaColour = wxColour("#131111");
    // 12x12 bitmap
    closeButton = wxXmlResource::Get()->LoadBitmap("notebook-dark-x");
}

void NotebookTab::Colours::InitLightColours()
{
    activeTabTextColour = "#444444";
    activeTabBgColour = "#f0f0f0";
    activeTabPenColour = "#b9b9b9";
    activeTabInnerPenColour = "#ffffff";

    inactiveTabTextColour = "#444444";
    inactiveTabBgColour = "#e5e5e5";
    inactiveTabPenColour = "#b9b9b9";
    inactiveTabInnerPenColour = "#ffffff";

    tabAreaColour = "#dcdcdc"; // wxColour("rgb(64, 64, 64)");
    // 12x12 bitmap
    closeButton = wxXmlResource::Get()->LoadBitmap("notebook-light-x");
}

int NotebookTabArea::FindPage(wxWindow* page) const
{
    for(size_t i = 0; i < m_tabs.size(); ++i) {
        if(m_tabs.at(i)->GetWindow() == page) {
            return i;
        }
    }
    return wxNOT_FOUND;
}

bool NotebookTabArea::RemovePage(size_t page, bool notify, bool deletePage)
{
    int nextSelection = wxNOT_FOUND;
    if(!IsIndexValid(page)) return false;

    if(notify) {
        wxBookCtrlEvent event(wxEVT_BOOK_PAGE_CLOSING);
        event.SetEventObject(this);
        event.SetSelection(GetSelection());
        GetParent()->GetEventHandler()->ProcessEvent(event);
        if(!event.IsAllowed()) {
            // Vetoed
            return false;
        }
    }

    NotebookTab::Ptr_t tab = m_tabs.at(page);
    m_tabs.erase(m_tabs.begin() + page);
    m_visibleTabs.clear();

    // Choose a new selection
    nextSelection = page;
    if(!m_tabs.empty()) {
        if(nextSelection >= (int)m_tabs.size()) {
            --nextSelection;
        }

        // Ensure that the next selection is always valid
        if(nextSelection >= (int)m_tabs.size()) {
            nextSelection = 0;
        }

    } else {
        nextSelection = wxNOT_FOUND;
    }

    // Now remove the page from the notebook
    int where = GetBook()->FindPage(tab->GetWindow());
    if(where != wxNOT_FOUND) {
        GetBook()->RemovePage(where);
    }

    if(deletePage) {
        // Destory the page
        tab->GetWindow()->Destroy();
    }

    if(notify) {
        wxBookCtrlEvent event(wxEVT_BOOK_PAGE_CLOSED);
        event.SetEventObject(this);
        event.SetSelection(GetSelection());
        GetParent()->GetEventHandler()->ProcessEvent(event);
        if(!event.IsAllowed()) {
            // Vetoed
            return false;
        }
    }

    // Choose the next page
    if(nextSelection != wxNOT_FOUND) {
        ChangeSelection(nextSelection);
        if(notify) {
            wxBookCtrlEvent event(wxEVT_BOOK_PAGE_CHANGED);
            event.SetEventObject(this);
            event.SetSelection(GetSelection());
            GetParent()->GetEventHandler()->AddPendingEvent(event);
        }
    } else {
        Refresh();
    }
    return true;
}

int NotebookTabArea::DoGetPageIndex(wxWindow* win) const
{
    for(size_t i = 0; i < m_tabs.size(); ++i) {
        if(m_tabs.at(i)->GetWindow() == win) return i;
    }
    return wxNOT_FOUND;
}
