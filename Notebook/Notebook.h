#ifndef NOTEBOOK_H
#define NOTEBOOK_H

#include <wx/panel.h>
#include <wx/simplebook.h>
#include <vector>
#include <wx/settings.h>
#include <wx/dcmemory.h>
#include <wx/sharedptr.h>
#include <wx/bookctrl.h>

#define WXDLLIMPEXP_SDK

enum NotebookStyle {
    /// Use the builtin light tab colours
    kNotebook_LightTabs = (1 << 0),
    /// Use the builtin dark tab colours
    kNotebook_DarkTabs = (1 << 2),
    /// Allow tabs to move using DnD
    kNotebook_AllowDnD = (1 << 3),
    /// Draw X button on the active tab
    kNotebook_CloseButtonOnActiveTab = (1 << 4),
};
/**
 * @class NotebookTab
 * @author Eran Ifrah
 * @brief contains information (mainly for drawing purposes) about a single tab label
 */
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
    int m_bmpCloseX;
    int m_bmpCloseY;
    int m_width;
    int m_height;

public:
    struct WXDLLIMPEXP_SDK Colours {
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

        // close button bitmaps (MUST be 12x12)
        wxBitmap closeButton;

        Colours();

        void InitDarkColours();
        void InitLightColours();
    };

public:
    // Geometry
    static const int Y_SPACER = 5;
    static const int X_SPACER = 5;
    static const int BOTTOM_AREA_HEIGHT = 4;
    static const int MAJOR_CURVE_WIDTH = 15;
    static const int SMALL_CURVE_WIDTH = 3;
    static const int TAB_HEIGHT = 35;

public:
    void CalculateOffsets(size_t style);

public:
    typedef wxSharedPtr<NotebookTab> Ptr_t;
    typedef std::vector<NotebookTab::Ptr_t> Vec_t;

    NotebookTab()
        : m_window(NULL)
        , m_active(false)
        , m_textX(wxNOT_FOUND)
        , m_textY(wxNOT_FOUND)
        , m_bmpX(wxNOT_FOUND)
        , m_bmpY(wxNOT_FOUND)
        , m_bmpCloseX(wxNOT_FOUND)
        , m_bmpCloseY(wxNOT_FOUND)
    {
        CalculateOffsets(0);
    }

    NotebookTab(size_t style, wxWindow* page, const wxString& text, const wxBitmap& bmp = wxNullBitmap)
        : m_label(text)
        , m_bitmap(bmp)
        , m_window(page)
        , m_active(false)
    {
        CalculateOffsets(style);
    }

    virtual ~NotebookTab() {}

    bool IsValid() const { return m_window != NULL; }

    /**
     * @brief render the using the provided wxDC
     */
    void Draw(wxDC& dc, const NotebookTab::Colours& colours, size_t style);
    void SetBitmap(const wxBitmap& bitmap, size_t style);
    void SetLabel(const wxString& label, size_t style);
    void SetActive(bool active, size_t style);
    void SetRect(const wxRect& rect) { this->m_rect = rect; }
    const wxBitmap& GetBitmap() const { return m_bitmap; }
    const wxString& GetLabel() const { return m_label; }
    const wxRect& GetRect() const { return m_rect; }
    wxRect& GetRect() { return m_rect; }
    wxWindow* GetWindow() { return m_window; }
    wxWindow* GetWindow() const { return m_window; }
    void SetWindow(wxWindow* window) { this->m_window = window; }
    bool IsActive() const { return m_active; }
    int GetBmpCloseX() const { return m_bmpCloseX; }
    int GetBmpCloseY() const { return m_bmpCloseY; }
    int GetHeight() const { return m_height; }
    int GetWidth() const { return m_width; }
};

class Notebook;
/**
 * @class NotebookTabArea
 * @author Eran Ifrah
 * @brief The Window that all the tabs are drawn on
 */
class WXDLLIMPEXP_SDK NotebookTabArea : public wxPanel
{
    int m_height;
    NotebookTab::Vec_t m_tabs;
    friend class Notebook;
    size_t m_style;
    NotebookTab::Colours m_colours;
    NotebookTab::Vec_t m_visibleTabs;
    int m_closeButtonClickedIndex;
    
protected:
    void OnPaint(wxPaintEvent& e);
    void OnEraseBG(wxEraseEvent& e);
    void OnSize(wxSizeEvent& event);
    void OnLeftDown(wxMouseEvent& event);
    void OnLeftUp(wxMouseEvent& event);
    void OnMouseMotion(wxMouseEvent& event);
    int DoGetPageIndex(wxWindow* win) const;

    bool ShiftRight(NotebookTab::Vec_t& tabs);
    bool IsActiveTabInList(const NotebookTab::Vec_t& tabs) const;
    bool IsActiveTabVisible(const NotebookTab::Vec_t& tabs) const;

    /**
     * @brief loop over the tabs and set their coordiantes
     */
    void DoUpdateCoordiantes(NotebookTab::Vec_t& tabs);
    /**
     * @brief get list of tabs to draw. This call always returns the active tab as part of the list
     * It also ensures that we draw as much tabs as we can.
     * @param offset reset the 0 based index from m_tabs
     */
    void UpdateVisibleTabs();

    NotebookTab::Ptr_t GetTabInfo(size_t index);
    NotebookTab::Ptr_t GetTabInfo(size_t index) const;
    NotebookTab::Ptr_t GetTabInfo(wxWindow* page);
    NotebookTab::Ptr_t GetActiveTabInfo();

    /**
     * @brief test if pt is on one of the visible tabs return its index
     * @param pt mouse click position
     * @param realPosition the index position in the m_tabs array
     * @param tabHit the inedx position in the m_visibleTabs array
     */
    void TestPoint(const wxPoint& pt, int& realPosition, int& tabHit);

    wxSimplebook* GetBook();

    void DoDeletePage(size_t page) { RemovePage(page, true, true); }

public:
    NotebookTabArea(wxWindow* notebook, size_t style);
    virtual ~NotebookTabArea();

    /**
     * @brief return true if index is in the tabs vector range
     */
    bool IsIndexValid(size_t index) const;

    void SetStyle(size_t style);
    size_t GetStyle() const { return m_style; }

    /**
     * @brief update the selected tab. This function also fires an event
     */
    int SetSelection(size_t tabIdx);
    /**
     * @brief update the selected tab. This function does not fire an event
     */
    int ChangeSelection(size_t tabIdx);

    /**
     * @brief return the current selection. return wxNOT_FOUND if non is selected
     */
    int GetSelection() const;

    /**
     * @brief Sets the text for the given page.
     */
    bool SetPageText(size_t page, const wxString& text);
    wxString GetPageText(size_t page) const;

    void AddPage(NotebookTab::Ptr_t tab);
    bool InsertPage(size_t index, NotebookTab::Ptr_t tab);

    void SetPageBitmap(size_t index, const wxBitmap& bmp);
    wxBitmap GetPageBitmap(size_t index) const;
    wxWindow* GetPage(size_t index) const;

    int FindPage(wxWindow* page) const;
    bool RemovePage(size_t page, bool notify, bool deletePage);
    bool DeleteAllPages();
};

/**
 * @class Notebook
 * @author Eran Ifrah
 * @brief A modern notebook (similar to the ones seen on Sublime Text and Atom editors
 * for wxWidgets. The class implementation uses wxSimplebook as the tab container and a
 * custom drawing tab area (see above the class NotebookTabArea)
 */
class WXDLLIMPEXP_SDK Notebook : public wxPanel
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
     * @brief set the notebook style. The style bits are kNotebook_* (you can set several
     * styles ORed)
     */
    void SetStyle(size_t style);

    /**
     * @brief return the book style
     */
    size_t GetStyle() const { return m_header->GetStyle(); }

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
     * @brief return the currently selected page or null
     */
    wxWindow* GetCurrentPage() const;

    /**
     * @brief Returns the index of the specified tab window or wxNOT_FOUND if not found
     */
    int FindPage(wxWindow* page) const;

    /**
     * @brief Deletes the specified page, without deleting the associated window
     */
    bool RemovePage(size_t page);

    /**
     * @brief Deletes the specified page and the associated window
     */
    bool DeletePage(size_t page);

    /**
     * @brief Deletes all pages
     */
    bool DeleteAllPages();

    /**
     * @brief set a new selection. This function fires an event that can be vetoed
     */
    int SetSelection(size_t selection) { return m_header->SetSelection(selection); }
    /**
     * @brief set new selection. No events are fired
     */
    int ChangeSelection(size_t selection) { return m_header->ChangeSelection(selection); }

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
    void SetPageBitmap(size_t index, const wxBitmap& bmp) { m_header->SetPageBitmap(index, bmp); }

    /**
     * @brief return bitmap for a given page. Return wxNullBitmap if invalid page
     */
    wxBitmap GetPageBitmap(size_t index) const { return m_header->GetPageBitmap(index); }

    // Base class members...
    virtual bool SetPageImage(size_t page, int image)
    {
        wxUnusedVar(page);
        wxUnusedVar(image);
        return false;
    }
    virtual int GetPageImage(size_t n) const { return wxNOT_FOUND; }

    /**
     * @brief Returns the number of pages in the control
     */
    size_t GetPagetCount() const { return m_book->GetPageCount(); }

    /**
     * @brief Returns the window at the given page position.
     */
    wxWindow* GetPage(size_t index) const { return m_header->GetPage(index); }
};

wxDECLARE_EXPORTED_EVENT(WXDLLIMPEXP_SDK, wxEVT_BOOK_PAGE_CHANGING, wxBookCtrlEvent);
wxDECLARE_EXPORTED_EVENT(WXDLLIMPEXP_SDK, wxEVT_BOOK_PAGE_CHANGED, wxBookCtrlEvent);
wxDECLARE_EXPORTED_EVENT(WXDLLIMPEXP_SDK, wxEVT_BOOK_PAGE_CLOSING, wxBookCtrlEvent);
wxDECLARE_EXPORTED_EVENT(WXDLLIMPEXP_SDK, wxEVT_BOOK_PAGE_CLOSED, wxBookCtrlEvent);

#endif // NOTEBOOK_H
