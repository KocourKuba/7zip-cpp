#pragma once

// Version of this library
#define SEVENZIP_VERSION "0.3.0-20180422"

#define SEVENZIP_LIBNAME "7zpp"

#ifdef _DEBUG
#define LIBDBG "d"
#else
#define LIBDBG

#endif
#ifdef _UNICODE
#define CHARSET "u"
#else
#define CHARSET "a"
#endif
#ifdef _AFXDLL
#define STATIC ""
#else
#define STATIC "s"
#endif
#ifdef _WIN64
#define BITNESS "64"
#else
#define BITNESS "32"
#endif

#define LIBFILENAME SEVENZIP_LIBNAME BITNESS CHARSET STATIC LIBDBG ".lib"

#ifdef _LIB
#pragma message (SEVENZIP_LIBNAME ": Build " LIBFILENAME)
#else
#pragma message("Automatically linking with " LIBFILENAME)
#pragma comment(lib, LIBFILENAME)
#endif

#undef LIBDBG
#undef CHARSET
#undef STATIC
#undef BITNESS
#undef SEVENZIP_LIBNAME
