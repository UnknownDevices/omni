#pragma once

#define _WIN32_WINNT 0x0A00 
#define WINVER       0x0A00
#define NTDDI_WIN10  0x0A000000

#define WIN32_LEAN_AND_MEAN
#define NOCOMM
#include <Windows.h>

#define OMNI_WINAPI_MAX_STR 256

namespace Omni
{
inline const char* format_win32_error(DWORD error)
{
    thread_local char msg_buf[OMNI_WINAPI_MAX_STR];
    FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_MAX_WIDTH_MASK |
        FORMAT_MESSAGE_IGNORE_INSERTS, nullptr, HRESULT_FROM_WIN32(error),
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        reinterpret_cast<LPTSTR>(msg_buf), OMNI_WINAPI_MAX_STR, nullptr);

    return msg_buf;
}
}
