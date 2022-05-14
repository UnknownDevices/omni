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
                break;
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

    void Wnd::create(WndRscs& wnd_rscs, const char* title, int x, int y, int width, 
        int height, DWORD style)
    {
        RECT size;
        size.top = 0;
        size.bottom = height;
        size.left = 0;
        size.right = width;

        bool success = AdjustWindowRectEx(&size, style, false, 0);
        omni_assert_win32_call(success, AdjustWindowRectEx);

        m_hwnd = CreateWindowEx(0, (LPCTSTR)wnd_rscs.get_atom(),title, style, x, y,
            size.right - size.left, size.bottom - size.top, nullptr, nullptr, s_omni_h_inst, this);
        omni_assert_win32_call(m_hwnd, CreateWindowEx);

        success = ShowWindowAsync(m_hwnd, SW_SHOW);
        omni_assert_win32_call(success, ShowWindowAsync);
    }

    void Wnd::destroy()
    {
        bool success = DestroyWindow(m_hwnd);
        omni_assert_win32_call(success, DestroyWindow);
        PostQuitMessage(0);
    }
}