#pragma once

#include <omni/core.hpp>
#include <omni/win/incl_windows.h>
#include <omni/win/window_resources.hpp>
#include <omni/win/window_event.hpp>

namespace Omni::Win
{
    class OMNI_API Window
    {
    public:
        OMNI_CONSTEXPR Window() noexcept = default;
        OMNI_CONSTEXPR ~Window() noexcept = default;

        static LRESULT CALLBACK proc_wnd_msgs(HWND hwnd, UINT msg_type, WPARAM w_param, 
            LPARAM l_param);

        static LRESULT CALLBACK proc_wnd_creation_msgs(HWND hwnd, UINT msg_type, WPARAM w_param, 
            LPARAM l_param);

        static bool poll_msg();

        void create(WindowResources& wnd_resources, 
            const char* title = "Omni Window", 
            int x = CW_USEDEFAULT, 
            int y = CW_USEDEFAULT, 
            int width = 720,
            int height = 480, 
            DWORD style = DefaultStyle);
        void destroy();

        void add_callback();

        static constexpr DWORD DefaultStyle = WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;

    private:
        HWND m_hwnd;
    };
}