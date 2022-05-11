#pragma once

#include <omni/core.hpp>
#include <omni/win/windows.h>
#include <omni/win/wnd_rscs.hpp>

namespace Omni::Win
{
    class OMNI_API Wnd
    {
    public:
        OMNI_CONSTEXPR Wnd() noexcept = default;
        OMNI_CONSTEXPR ~Wnd() noexcept = default;

        static LRESULT CALLBACK wnd_proc(HWND hwnd, UINT msg_type, WPARAM w_param, 
            LPARAM l_param); 

        static bool poll_msg();

        void start(WndRscs& wnd_rscs);
        void stop();

    private:
        HWND m_hwnd;
    };
}