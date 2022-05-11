#include <omni/win/wnd.hpp>

#include <omni/resource.h>
#include <omni/logger.hpp>

namespace Omni::Win
{
    LRESULT CALLBACK Wnd::wnd_proc(HWND hwnd, UINT msg_type, WPARAM w_param, LPARAM l_param)
    {
        switch (msg_type)
        {
            case WM_DESTROY:
            {
                auto& wnd = *reinterpret_cast<Wnd*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
                wnd.stop();
                return 0;
            }
            case WM_CREATE:
            {
                CREATESTRUCT& p_create = *reinterpret_cast<CREATESTRUCT*>(l_param);
                SetWindowLongPtr(hwnd, GWLP_USERDATA,
                    reinterpret_cast<LONG_PTR>(p_create.lpCreateParams));
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

    void Wnd::start(WndRscs& wnd_rscs)
    {
        m_hwnd = CreateWindowEx(0, (LPCSTR)wnd_rscs.get_atom(), "Omni Window", WS_CAPTION |
            WS_SYSMENU | WS_MINIMIZEBOX, CW_USEDEFAULT, CW_USEDEFAULT, 720, 480, nullptr, nullptr,
            s_omni_h_inst, this);
        hard_assert(m_hwnd, "CreateWindowEx failed with code: {}!", GetLastError());

        bool succeded = ShowWindowAsync(m_hwnd, SW_SHOW);
        hard_assert(succeded, "ShowWindowAsync failed with code: {}!", GetLastError());
    }

    void Wnd::stop()
    {
        bool succeded = DestroyWindow(m_hwnd);
        hard_assert(succeded, "DestroyWindow failed with code: {}!", GetLastError());
        PostQuitMessage(0);
    }
}