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
    NotebookTab tab(GetStyle(), page, label, bmp);
    tab.SetActive(selected, GetStyle());
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
    NotebookTab tab(GetStyle(), page, label, bmp);
    tab.SetActive(selected, GetStyle());
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

//----------------------------------------------------------
// Tab label
//----------------------------------------------------------
void NotebookTab::Draw(wxDC& dc, const NotebookTab::Colours& colours, size_t style)
{
    const int TOP_SMALL_HEIGHT = 2;
    m_xRect = wxRect();
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
        m_xRect = wxRect(m_bmpCloseX + m_rect.GetX(), m_bmpCloseY, 12, 12);
        dc.DrawBitmap(colours.closeButton, m_bmpCloseX + m_rect.GetX(), m_bmpCloseY);
    }
}

void NotebookTab::CalculateOffsets(size_t style)
{
    wxBitmap b(1, 1);
    wxMemoryDC memDC(b);
    wxFont font = wxSystemSettings::GetFont(wxSYS_DEFAULT_GUI_FONT);
    font.SetPointSize(10);
    memDC.SetFont(font);
    wxSize sz = memDC.GetTextExtent(m_label);
    m_rect.SetHeight(TAB_HEIGHT);

    int width = 0;
    width += MAJOR_CURVE_WIDTH;
    width += SMALL_CURVE_WIDTH;
    width += X_SPACER;

    // bitmap
    m_bmpX = wxNOT_FOUND;
    m_bmpY = wxNOT_FOUND;
    if(m_bitmap.IsOk()) {
        m_bmpX = width;
        width += m_bitmap.GetWidth();
        width += X_SPACER;
        m_bmpY = ((m_rect.GetHeight() - m_bitmap.GetHeight()) / 2);
    }

    // Text
    m_textX = width;
    m_textY = ((m_rect.GetHeight() - sz.y) / 2);
    width += sz.x;

    // x button
    if(IsActive() && (style & kNotebook_CloseButtonOnActiveTab)) {
        width += X_SPACER;
        m_bmpCloseX = width;
        m_bmpCloseY = ((m_rect.GetHeight() - 12) / 2);
        width += 12; // X button is 10 pixels in size
    }

    width += X_SPACER;
    width += SMALL_CURVE_WIDTH;
    width += MAJOR_CURVE_WIDTH;
    m_rect.SetWidth(width);
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

//----------------------------------------------------------
// Notebook header
//----------------------------------------------------------
NotebookTabArea::NotebookTabArea(wxWindow* notebook, size_t style)
    : wxPanel(notebook)
    , m_height(NotebookTab::TAB_HEIGHT)
    , m_dragging(false)
    , m_draggingIndex(wxNOT_FOUND)
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
    if(HasCapture()) {
        ReleaseMouse();
    }

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

    if(rect.GetSize().x > 0) {
        wxBitmap bmpTabs(rect.GetSize());
        wxMemoryDC memDC(bmpTabs);
        wxGCDC gcdc(memDC);

        gcdc.SetPen(m_colours.tabAreaColour);
        gcdc.SetBrush(m_colours.tabAreaColour);
        gcdc.DrawRectangle(rect.GetSize());

        // Loop over the tabs and set their cooridnates
        DoUpdateCoordiantes(m_tabs);
        // Get list of tabs that can fit the screen
        NotebookTab::Vec_t tabs;
        int offset;
        GetVisibleTabs(tabs, offset);

        // Update the tabs coordinates
        DoUpdateCoordiantes(tabs);
        int activeTabInex = wxNOT_FOUND;
        for(int i = (tabs.size() - 1); i >= 0; --i) {
            NotebookTab& tab = tabs.at(i);
            if(tab.IsActive()) {
                activeTabInex = i;
            }
            tab.Draw(gcdc, m_colours, m_style);
        }

        // Redraw the active tab
        if(activeTabInex != wxNOT_FOUND) {
            tabs.at(activeTabInex).Draw(gcdc, m_colours, m_style);
        }

        memDC.SelectObject(wxNullBitmap);
        dc.DrawBitmap(bmpTabs, 0, 0);

        if(activeTabInex != wxNOT_FOUND) {
            const NotebookTab& activeTab = tabs.at(activeTabInex);
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
            from = activeTab.GetRect().GetBottomLeft();
            to = activeTab.GetRect().GetBottomRight();
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
    }
}

void NotebookTabArea::DoUpdateCoordiantes(NotebookTab::Vec_t& tabs)
{
    int xx = 5;
    for(size_t i = 0; i < tabs.size(); ++i) {
        NotebookTab& tab = tabs.at(i);
        tab.GetRect().SetX(xx);
        tab.GetRect().SetY(0);
        xx += tab.GetRect().GetWidth() - NotebookTab::MAJOR_CURVE_WIDTH;
    }
}

void NotebookTabArea::GetVisibleTabs(NotebookTab::Vec_t& tabs, int& offset)
{
    wxRect clientRect(GetClientRect());
    tabs.clear();
    offset = wxNOT_FOUND;
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
                tabs.erase(tabs.begin());
            }
            const NotebookTab& tab = m_tabs.at(i);
            tabs.push_back(tab);
            if(tab.IsActive()) {
                break;
            }
        }
    }

    // set the offset
    if(!tabs.empty()) {
        const NotebookTab& t = tabs.at(0);
        for(size_t i = 0; i < m_tabs.size(); ++i) {
            if(m_tabs.at(i).GetWindow() == t.GetWindow()) {
                offset = i;
                break;
            }
        }
    }
}

void NotebookTabArea::OnLeftDown(wxMouseEvent& event)
{
    event.Skip();
    m_dragging = false;
    m_draggingIndex = wxNOT_FOUND;

    if(!HasCapture()) {
        // Capture the mouse, this is needed for DnD
        CaptureMouse();
    }

    // Get list of visible tabs
    NotebookTab::Vec_t tabs;
    int offset;
    GetVisibleTabs(tabs, offset);

    // Update the tabs coordinates
    DoUpdateCoordiantes(tabs);

    wxPoint pt = event.GetPosition();
    int activeTabIndex = wxNOT_FOUND;
    for(size_t i = 0; i < tabs.size(); ++i) {
        NotebookTab& tab = tabs.at(i);
        if(tab.IsActive()) {
            activeTabIndex = i;
        }
        if(tab.GetRect().Contains(pt) && !tab.IsActive()) {
            // Changing selection
            SetSelection(i);
            return;
        }
    }

    // We clicked on the active tab, start DnD operation
    if((m_style & kNotebook_AllowDnD) && (activeTabIndex != wxNOT_FOUND)) {
        m_dragging = true;
        m_draggingIndex = activeTabIndex;
    }
}

void NotebookTabArea::ChangeSelection(size_t tabIdx)
{
    wxWindowUpdateLocker locker(GetParent());
    if(!IsIndexValid(tabIdx)) return;

    for(size_t i = 0; i < m_tabs.size(); ++i) {
        NotebookTab& tab = m_tabs.at(i);
        tab.SetActive((i == tabIdx), GetStyle());
    }

    NotebookTab& activeTab = GetActiveTabInfo();
    if(activeTab.IsOk()) {
        static_cast<Notebook*>(GetParent())->DoChangeSelection(activeTab.GetWindow());
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
        const NotebookTab& tab = m_tabs.at(i);
        if(tab.IsActive()) return i;
    }
    return wxNOT_FOUND;
}

NotebookTab& NotebookTabArea::GetTabInfo(size_t index)
{
    static NotebookTab NullTab;
    if(!IsIndexValid(index)) return NullTab;
    return m_tabs.at(index);
}

const NotebookTab& NotebookTabArea::GetTabInfo(size_t index) const
{
    static NotebookTab NullTab;
    if(!IsIndexValid(index)) return NullTab;
    return m_tabs.at(index);
}

NotebookTab& NotebookTabArea::GetTabInfo(wxWindow* page)
{
    static NotebookTab NullTab;
    for(size_t i = 0; i < m_tabs.size(); ++i) {
        NotebookTab& tab = m_tabs.at(i);
        if(tab.GetWindow() == page) return tab;
    }
    return NullTab;
}

bool NotebookTabArea::SetPageText(size_t page, const wxString& text)
{
    NotebookTab& tab = GetTabInfo(page);
    if(!tab.IsOk()) return false;
    tab.SetLabel(text, GetStyle());
    Refresh();
    return true;
}

NotebookTab& NotebookTabArea::GetActiveTabInfo()
{
    static NotebookTab NullTab;
    for(size_t i = 0; i < m_tabs.size(); ++i) {
        if(m_tabs.at(i).IsActive()) {
            return m_tabs.at(i);
        }
    }
    return NullTab;
}

void NotebookTabArea::AddPage(NotebookTab tab) { InsertPage(m_tabs.size(), tab); }

wxSimplebook* NotebookTabArea::GetBook() { return reinterpret_cast<Notebook*>(GetParent())->m_book; }

bool NotebookTabArea::InsertPage(size_t index, NotebookTab tab)
{
    if(index > m_tabs.size()) return false;
    m_tabs.insert(m_tabs.begin() + index, tab);

    int tabIndex = index;
    tab.GetWindow()->Reparent(GetBook());
    GetBook()->InsertPage(index, tab.GetWindow(), tab.GetLabel(), tab.IsActive());
    NotebookTab& activeTab = GetActiveTabInfo();
    if(!activeTab.IsOk()) {
        // No active tab??
        ChangeSelection(0);
    }
    if(tab.IsActive()) {
        ChangeSelection(tabIndex);
    }
    Refresh();
    return true;
}

wxString NotebookTabArea::GetPageText(size_t page) const
{
    const NotebookTab& tab = GetTabInfo(page);
    if(tab.IsOk()) return tab.GetLabel();
    return "";
}

wxBitmap NotebookTabArea::GetPageImage(size_t index) const
{
    const NotebookTab& tab = GetTabInfo(index);
    if(tab.IsOk()) return tab.GetBitmap();
    return wxNullBitmap;
}

void NotebookTabArea::SetPageImage(size_t index, const wxBitmap& bmp)
{
    NotebookTab& tab = GetTabInfo(index);
    if(tab.IsOk()) {
        tab.SetBitmap(bmp, GetStyle());
        Refresh();
    }
}

void NotebookTabArea::OnLeftUp(wxMouseEvent& event)
{
    event.Skip();
    m_dragging = false;
    m_draggingIndex = wxNOT_FOUND;
    if(HasCapture()) {
        ReleaseMouse();
    }
}

void NotebookTabArea::OnMouseMotion(wxMouseEvent& event)
{
    event.Skip();
    if(m_dragging && (m_draggingIndex != wxNOT_FOUND)) {
        NotebookTab::Vec_t visibleTabs;
        int selectedIndex = wxNOT_FOUND;
        int offset = wxNOT_FOUND;
        TestPoint(event.GetPosition(), visibleTabs, offset, selectedIndex);
        if((selectedIndex != wxNOT_FOUND) && (selectedIndex != m_draggingIndex)) {
            // swap between the two values
            std::iter_swap(m_tabs.begin() + offset + selectedIndex, m_tabs.begin() + offset + m_draggingIndex);
            // update the dragging index
            m_draggingIndex = selectedIndex;
            Refresh();
        }
    } else {
        // Show tooltip?
    }
}

void NotebookTabArea::TestPoint(const wxPoint& pt, NotebookTab::Vec_t& visibleTabs, int& offset, int& selectedIndex)
{
    offset = wxNOT_FOUND;
    GetVisibleTabs(visibleTabs, offset);
    if(visibleTabs.empty()) return;

    // Update the tabs coordinates
    DoUpdateCoordiantes(visibleTabs);

    selectedIndex = wxNOT_FOUND;
    for(size_t i = 0; i < visibleTabs.size(); ++i) {
        NotebookTab& tab = visibleTabs.at(i);
        if(tab.GetRect().Contains(pt)) {
            // Changing selection
            selectedIndex = i;
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
        m_tabs.at(i).CalculateOffsets(GetStyle());
    }
    Refresh();
}

wxWindow* NotebookTabArea::GetPage(size_t index) const
{
    const NotebookTab& tab = GetTabInfo(index);
    return tab.GetWindow();
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
    for(size_t i=0; i<m_tabs.size(); ++i) {
        if(m_tabs.at(i).GetWindow() == page) {
            return i;
        }
    }
    return wxNOT_FOUND;
}
