#ifdef __WXMSW__
#ifdef WXMAKINGDLL_SDK
#define WXDLLIMPEXP_SDK __declspec(dllexport)
#elif defined(WXUSINGDLL_SDK)
#define WXDLLIMPEXP_SDK __declspec(dllimport)
#else // not making nor using DLL
#define WXDLLIMPEXP_SDK
#endif
#else
#define WXDLLIMPEXP_SDK
#endif