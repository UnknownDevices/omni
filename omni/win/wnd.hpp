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

        static LRESULT CALLBACK proc_wnd_msgs(HWND hwnd, UINT msg_type, WPARAM w_param, 
            LPARAM l_param);

        static LRESULT CALLBACK proc_wnd_creation_msgs(HWND hwnd, UINT msg_type, WPARAM w_param, 
            LPARAM l_param);

        static bool poll_msg();

        void create(WndRscs& wnd_rscs, int width = 720, int height = 480);
        void destroy();

    private:
        HWND m_hwnd;
    };
}