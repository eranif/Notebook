#ifndef NOTEBOOK_H
#define NOTEBOOK_H

#include <wx/panel.h>
#include <wx/simplebook.h>
#include <vector>
#include <wx/settings.h>
#include <wx/dcmemory.h>

#define WXDLLIMPEXP_SDK

class WXDLLIMPEXP_SDK NotebookTab
{
    wxString m_label;
    wxBitmap m_bitmap;
    wxWindow* m_window;
    wxRect m_rect;
    bool m_active;
    int m_textX;
    int m_textY;
    int m_bmpX;
    int m_bmpY;

public:
    struct Colours {
        // Active tab colours
        wxColour inactiveTabBgColour;
        wxColour inactiveTabPenColour;
        wxColour inactiveTabTextColour;
        wxColour inactiveTabInnerPenColour;

        // Inactive tab colours
        wxColour activeTabTextColour;
        wxColour activeTabBgColour;
        wxColour activeTabPenColour;
        wxColour activeTabInnerPenColour;

        // the tab area colours
        wxColour tabAreaColour;
        Colours();
        
        void InitDarkColours();
        void InitLightColours();
    };

public:
    static const int Y_SPACER = 5;
    static const int X_SPACER = 5;
    static const int BOTTOM_AREA_HEIGHT = 4;

    static const int ANGLE_WIDTH = 15;
    static const int ANGLE_WIDTH_SMALL = 3;

    static const int TAB_HEIGHT = 35;

protected:
    void CalculateOffsets();

public:
    typedef std::vector<NotebookTab> Vec_t;
    NotebookTab()
        : m_window(NULL)
        , m_active(false)
    {
    }
    NotebookTab(wxWindow* page, const wxString& text, const wxBitmap& bmp = wxNullBitmap)
        : m_label(text)
        , m_bitmap(bmp)
        , m_window(page)
        , m_active(false)
    {
        CalculateOffsets();
    }

    virtual ~NotebookTab() {}

    bool IsOk() const { return m_window != NULL; }
    void Draw(wxDC& dc);
    void SetBitmap(const wxBitmap& bitmap)
    {
        this->m_bitmap = bitmap;
        CalculateOffsets();
    }
    void SetLabel(const wxString& label)
    {
        this->m_label = label;
        CalculateOffsets();
    }
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

    NotebookTab& GetTabInfo(size_t index);
    const NotebookTab& GetTabInfo(size_t index) const;
    NotebookTab& GetTabInfo(wxWindow* page);
    NotebookTab& GetActiveTabInfo();

    wxSimplebook* GetBook();

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

    /**
     * @brief Sets the text for the given page.
     */
    bool SetPageText(size_t page, const wxString& text);
    wxString GetPageText(size_t page) const;

    void AddPage(NotebookTab tab);
    bool InsertPage(size_t index, NotebookTab tab);

    void SetPageImage(size_t index, const wxBitmap& bmp);
    wxBitmap GetPageImage(size_t index) const;
};

class Notebook : public wxPanel
{
    wxSimplebook* m_book;
    NotebookTabArea* m_header;
    friend class NotebookTabArea;

protected:
    void DoChangeSelection(wxWindow* page);

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
     * @brief insert page at a specified position
     */
    bool InsertPage(size_t index,
                    wxWindow* page,
                    const wxString& label,
                    bool selected = false,
                    const wxBitmap& bmp = wxNullBitmap);
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

    /**
     * @brief Sets the text for the given page.
     */
    bool SetPageText(size_t page, const wxString& text) { return m_header->SetPageText(page, text); }

    /**
     * @brief Returns the string for the given page
     */
    wxString GetPageText(size_t page) const { return m_header->GetPageText(page); }

    /**
     * @brief set the image for the given page
     */
    void SetPageImage(size_t index, const wxBitmap& bmp) { m_header->SetPageImage(index, bmp); }

    /**
     * @brief return bitmap for a given page. Return wxNullBitmap if invalid page
     */
    wxBitmap GetPageImage(size_t index) const { return m_header->GetPageImage(index); }
};

wxDECLARE_EXPORTED_EVENT(WXDLLIMPEXP_SDK, wxEVT_BOOK_PAGE_CHANGING, wxBookCtrlEvent);
wxDECLARE_EXPORTED_EVENT(WXDLLIMPEXP_SDK, wxEVT_BOOK_PAGE_CHANGED, wxBookCtrlEvent);

#endif // NOTEBOOK_H
