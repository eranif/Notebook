#ifndef WXSTRING_HASH
#define WXSTRING_HASH

#include <unordered_map>
#include <unordered_set>
#include <wx/version.h>

#if wxVERSION_NUMBER < 3100
#include <functional>
#include <wx/string.h>

namespace std
{
template <> struct hash<wxString> {
    std::size_t operator()(const wxString& s) const { return hash<std::wstring>{}(s.ToStdWstring()); }
};
} // namespace std
#endif
#endif // WXSTRING_HASH
