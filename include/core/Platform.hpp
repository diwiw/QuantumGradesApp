#pragma once

namespace qga {


// ---- Platform-wide sanity for Windows builds -------------------------------
#if defined(_WIN32) || defined(_WIN64)
  // Trim Windows headers to speed up compile and avoid extra macros
  #ifndef WIN32_LEAN_AND_MEAN
  #  define WIN32_LEAN_AND_MEAN
  #endif
  #ifndef NOMINMAX
  #  define NOMINMAX
  #endif

  // Some toolchains include <windows.h> indirectly (e.g., via graphics/logging libs)
  // If it already snuck in, neutralize problematic macros.
  #ifdef min
  #  undef min
  #endif
  #ifdef max
  #  undef max
  #endif
  #ifdef ERROR
  #  undef ERROR
  #endif
  #ifdef DELETE
  #  undef DELETE
  #endif
  #ifdef interface
  #  undef interface
  #endif
  // WinGDI also defines small macros like near/far.
  #ifdef near
  #  undef near
  #endif
  #ifdef far
  #  undef far
  #endif

  // Prefer standard secure CRT warnings off (optional; many projects use this)
  #ifndef _CRT_SECURE_NO_WARNINGS
  #  define _CRT_SECURE_NO_WARNINGS
  #endif

  // If you REALLY need windows.h somewhere, include it AFTER the defines above:
  // #  include <windows.h>
#endif

// ---- Common compiler tweaks ------------------------------------------------
#if defined(_MSC_VER)
// MSVC: silence benign warning about fopen, etc. if you don’t use *_s variants
#  pragma warning(disable: 4996)
#endif

// ---- End of Platform.hpp -----------------------------------------------------

} // namespace qga
