#ifndef NOTEBOOK_H
#define NOTEBOOK_H

#include <wx/panel.h>
#include <wx/simplebook.h>
#include <vector>
#include <wx/settings.h>
#include <wx/dcmemory.h>

#define WXDLLIMPEXP_SDK

class NotebookTab
{
    wxString m_label;
    wxBitmap m_bitmap;
    wxWindow* m_window;
    wxRect m_rect;
    bool m_active;
    int m_textX;
    int m_textY;

public:
    struct Colours {
        wxColour inactiveTabBgColour;
        wxColour inactiveTabPenColour;
        wxColour inactiveTabTextColour;
        wxColour activeTabTextColour;
        wxColour activeTabBgColour;
        wxColour activeTabPenColour;
        wxColour tabAreaColour;
        Colours()
        {
            inactiveTabBgColour = wxColour("rgb(69, 69, 69)");
            inactiveTabPenColour = wxColour("rgb(93, 92, 92)");
            activeTabTextColour = *wxWHITE;
            inactiveTabTextColour = wxColour("rgb(200, 200, 200)");
            activeTabBgColour = wxColour("rgb(42, 39, 39)");
            activeTabPenColour = activeTabBgColour.ChangeLightness(120);
            tabAreaColour = wxColour("rgb(19, 18, 18)");
        }
    };

public:
    static const int Y_SPACER = 5;
    static const int X_SPACER = 5;

    static const int ANGLE_WIDTH = 15;
    static const int ANGLE_WIDTH_SMALL = 3;

public:
    typedef std::vector<NotebookTab> Vec_t;

    NotebookTab(wxWindow* page, const wxString& text, const wxBitmap& bmp = wxNullBitmap)
        : m_label(text)
        , m_bitmap(bmp)
        , m_window(page)
        , m_active(false)
    {
        wxBitmap b(1, 1);
        wxMemoryDC memDC(b);
        memDC.SetFont(wxSystemSettings::GetFont(wxSYS_DEFAULT_GUI_FONT));
        wxSize sz = memDC.GetTextExtent(text);
        m_rect.SetHeight(sz.y + (2 * Y_SPACER));

        int width = 0;
        width += ANGLE_WIDTH;
        width += ANGLE_WIDTH_SMALL;

        // bitmap
        if(m_bitmap.IsOk()) {
            width += m_bitmap.GetWidth();
            width += X_SPACER;
        }

        // Text
        m_textX = width;
        m_textY = (m_rect.GetHeight() - sz.y) / 2 -
                  1; // We remove 3 pixles because of the bottom rect that we draw on the tab area
        width += sz.x;

        if(m_active) {
            // x button
            width += X_SPACER;
            width += 10; // X button is 10 pixels in size
        }

        width += ANGLE_WIDTH_SMALL;
        width += ANGLE_WIDTH;
        m_rect.SetWidth(width);
    }

    virtual ~NotebookTab() {}
    void Draw(wxDC& dc);
    void SetBitmap(const wxBitmap& bitmap) { this->m_bitmap = bitmap; }
    void SetLabel(const wxString& label) { this->m_label = label; }
    void SetRect(const wxRect& rect) { this->m_rect = rect; }
    const wxBitmap& GetBitmap() const { return m_bitmap; }
    const wxString& GetLabel() const { return m_label; }
    const wxRect& GetRect() const { return m_rect; }
    wxRect& GetRect() { return m_rect; }
    wxWindow* GetWindow() { return m_window; }
    void SetActive(bool active) { this->m_active = active; }
    void SetWindow(wxWindow* window) { this->m_window = window; }
    bool IsActive() const { return m_active; }
};

class Notebook;
class NotebookTabArea : public wxPanel
{
    int m_height;
    NotebookTab::Vec_t m_tabs;
    friend class Notebook;

protected:
    void OnPaint(wxPaintEvent& e);
    void OnEraseBG(wxEraseEvent& e);
    void OnSize(wxSizeEvent& event);
    void OnLeftDown(wxMouseEvent& event);
    /**
     * @brief loop over the tabs and set their coordiantes
     */
    void DoUpdateCoordiantes(NotebookTab::Vec_t& tabs);
    /**
     * @brief get list of tabs to draw. This call always returns the active tab as part of the list
     * It also ensures that we draw as much tabs as we can
     */
    NotebookTab::Vec_t GetTabsToDraw();

public:
    NotebookTabArea(wxWindow* notebook);
    virtual ~NotebookTabArea();

    /**
     * @brief update the selected tab. This function also fires an event
     */
    void SetSelection(size_t tabIdx);
    /**
     * @brief update the selected tab. This function does not fire an event
     */
    void ChangeSelection(size_t tabIdx);

    /**
     * @brief return the current selection. return wxNOT_FOUND if non is selected
     */
    int GetSelection() const;
};

class Notebook : public wxPanel
{
    wxSimplebook* m_book;
    NotebookTabArea* m_header;
    friend class NotebookTabArea;

public:
    /**
     * Constructor
     */
    Notebook(wxWindow* parent,
             wxWindowID id = wxID_ANY,
             const wxPoint& pos = wxDefaultPosition,
             const wxSize& size = wxDefaultSize,
             long style = 0,
             const wxString& name = wxEmptyString);
    /**
     * destructor
     */
    virtual ~Notebook();

    /**
     * @brief append page to the notebook
     */
    void AddPage(wxWindow* page, const wxString& label, bool selected = false, const wxBitmap& bmp = wxNullBitmap);

    /**
     * @brief set new selection. This function fires an event that can be vetoed
     */
    void SetSelection(size_t selection) { m_header->SetSelection(selection); }
    /**
     * @brief set new selection. No events are fired
     */
    void ChangeSelection(size_t selection) { m_header->ChangeSelection(selection); }

    /**
     * @brief return the currently selected page, return wxNOT_FOUND if non found
     */
    int GetSelection() const { return m_header->GetSelection(); }
};

wxDECLARE_EXPORTED_EVENT(WXDLLIMPEXP_SDK, wxEVT_BOOK_PAGE_CHANGING, wxBookCtrlEvent);
wxDECLARE_EXPORTED_EVENT(WXDLLIMPEXP_SDK, wxEVT_BOOK_PAGE_CHANGED, wxBookCtrlEvent);

#endif // NOTEBOOK_H
