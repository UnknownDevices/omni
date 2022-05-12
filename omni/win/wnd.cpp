#include <omni/win/wnd.hpp>

#include <omni/resource.h>
#include <omni/logger.hpp>

namespace Omni::Win
{
    LRESULT CALLBACK Wnd::proc_wnd_msgs(HWND hwnd, UINT msg_type, WPARAM w_param, LPARAM l_param)
    {
        switch (msg_type)
        {
            case WM_DESTROY:
            {
                auto& wnd = *reinterpret_cast<Wnd*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
                wnd.destroy();
                return 0;
            }
            case WM_PAINT:
            {
                PAINTSTRUCT ps;
                BeginPaint(hwnd, &ps);

                EndPaint(hwnd, &ps);
                return 0;
            }
        }

        return DefWindowProc(hwnd, msg_type, w_param, l_param);
    }

    LRESULT CALLBACK Wnd::proc_wnd_creation_msgs(HWND hwnd, UINT msg_type, WPARAM w_param, 
        LPARAM l_param)
    {
        switch (msg_type)
        {
            case WM_NCCREATE:
            {
                CREATESTRUCT& p_create = *reinterpret_cast<CREATESTRUCT*>(l_param);
                SetWindowLongPtr(hwnd, GWLP_USERDATA,
                    reinterpret_cast<LONG_PTR>(p_create.lpCreateParams));
                SetWindowLongPtr(hwnd, GWLP_WNDPROC,
                    reinterpret_cast<LONG_PTR>(&proc_wnd_msgs));
                return 1;
            }
        }

        return DefWindowProc(hwnd, msg_type, w_param, l_param);
    }

    bool Wnd::poll_msg()
    {
        static MSG msg;
        if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_QUIT)
                return false;

            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        return true;
    }

    void Wnd::create(WndRscs& wnd_rscs, int width, int height)
    {
        RECT wnd_size;
        wnd_size.top = 0;
        wnd_size.bottom = height;
        wnd_size.left = 0;
        wnd_size.right = width;

        bool success = AdjustWindowRectEx(&wnd_size, WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX, 
            false, 0);
        hard_assert(success, "AdjustWindowRectEx failed with code: {}!", GetLastError());

        m_hwnd = CreateWindowEx(0, (LPCSTR)wnd_rscs.get_atom(), "Omni Window", WS_CAPTION |
            WS_SYSMENU | WS_MINIMIZEBOX, CW_USEDEFAULT, CW_USEDEFAULT,
            wnd_size.right - wnd_size.left, wnd_size.bottom - wnd_size.top, nullptr, nullptr,
            s_omni_h_inst, this);
        hard_assert(m_hwnd, "CreateWindowEx failed with code: {}!", GetLastError());

        success = ShowWindowAsync(m_hwnd, SW_SHOW);
        hard_assert(success, "ShowWindowAsync failed with code: {}!", GetLastError());
    }

    void Wnd::destroy()
    {
        bool success = DestroyWindow(m_hwnd);
        hard_assert(success, "DestroyWindow failed with code: {}!", GetLastError());
        PostQuitMessage(0);
    }
}