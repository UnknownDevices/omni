#include <omni/win/window.hpp>

#include <omni/resource.h>
#include <omni/logger.hpp>

namespace Omni::Win
{
    LRESULT CALLBACK Window::proc_wnd_msgs(HWND hwnd, UINT msg_type, WPARAM wparam, LPARAM lparam)
    {
        const auto u_lparam = *reinterpret_cast<uint64*>(&lparam);
        const auto u_wparam  = *reinterpret_cast<uint64*>(&wparam);
        
        switch (msg_type)
        {
            case WM_DESTROY:
            {
                auto& wnd = *reinterpret_cast<Window*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
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
            case WM_LBUTTONDOWN:
            {
                auto button_down = ButtonDownEvent(
                    *(reinterpret_cast<int16*>(&lparam)),
                    *(reinterpret_cast<int16*>(&lparam) + 1),
                    *(reinterpret_cast<uint8*>(&wparam)));

                debug_log("button_down: [{}]", button_down);
                break;
            }
            case WM_LBUTTONUP:
            {
                auto button_up = ButtonUpEvent(
                    *(reinterpret_cast<int16*>(&lparam)),
                    *(reinterpret_cast<int16*>(&lparam) + 1),
                    *(reinterpret_cast<uint8*>(&wparam)));

                debug_log("button_up: [{}]", button_up);
                break;
            }
            case WM_RBUTTONDOWN:
            {
                auto button_down = ButtonDownEvent(
                    static_cast<int16>(get_bit_range(lparam, 0, 16)),
                    static_cast<int16>(get_bit_range(lparam, 16, 32)),
                    static_cast<uint8>(u_wparam));

                debug_log("button_down: [{}]", button_down);
                break;
            }
            case WM_RBUTTONUP:
            {
                auto button_up = ButtonUpEvent(
                    static_cast<int16>(get_bit_range(lparam, 0, 16)),
                    static_cast<int16>(get_bit_range(lparam, 16, 32)),
                    static_cast<uint8>(u_wparam));

                debug_log("button_up: [{}]", button_up);
                break;
            }
            case WM_MBUTTONDOWN:
            {
                auto button_down = ButtonDownEvent(
                    static_cast<int16>(get_bit_range(lparam, 0, 16)),
                    static_cast<int16>(get_bit_range(lparam, 16, 32)),
                    static_cast<uint8>(u_wparam));

                debug_log("button_down: [{}]", button_down);
                break;
            }
            case WM_MBUTTONUP:
            {
                auto button_up = ButtonUpEvent(
                    static_cast<int16>(get_bit_range(lparam, 0, 16)),
                    static_cast<int16>(get_bit_range(lparam, 16, 32)),
                    static_cast<uint8>(u_wparam));

                debug_log("button_up: [{}]", button_up);
                break;
            }
            case WM_XBUTTONDOWN:
            {
                auto button_down = ButtonDownEvent(
                    static_cast<int16>(get_bit_range(lparam, 0, 16)),
                    static_cast<int16>(get_bit_range(lparam, 16, 32)),
                    static_cast<uint8>(u_wparam));

                debug_log("button_down: [{}]", button_down);
                break;
            }
            case WM_XBUTTONUP:
            {
                auto button_up = ButtonUpEvent(
                    static_cast<int16>(get_bit_range(lparam, 0, 16)),
                    static_cast<int16>(get_bit_range(lparam, 16, 32)),
                    static_cast<uint8>(u_wparam));

                debug_log("button_up: [{}]", button_up);
                break;
            }
            case WM_KEYDOWN:
            {
                auto key_down = KeyDownEvent(
                    static_cast<uint16>(u_wparam),
                    static_cast<uint16>(get_bit_range(u_lparam, 16, 24)),
                    static_cast<uint16>(get_bit_range(u_lparam, 0 , 16)),
                    static_cast<uint8> (get_bit_range(u_lparam, 30, 31)),
                    static_cast<uint8> (get_bit_range(u_lparam, 24, 25)));

                debug_log("key_down: [{}]", key_down);
                break;
            }
            case WM_KEYUP:
            {
                auto key_up = KeyUpEvent(
                    static_cast<uint16>(u_wparam),
                    static_cast<uint16>(get_bit_range(u_lparam, 16, 24)),
                    static_cast<uint8> (get_bit_range(u_lparam, 24, 25)));

                debug_log("key_up: [{}]", key_up);
                break;
            }
            case WM_CHAR:
            {
                auto char_event = CharEvent(
                    static_cast<char> (wparam),
                    static_cast<uint8>(get_bit_range(u_lparam, 30, 31)));

                debug_log("char_event: [{}]", char_event);
                break;
            }
            case WM_MENUCHAR:
            {
                return MNC_CLOSE << 16;
            }
        }

        return DefWindowProc(hwnd, msg_type, wparam, lparam);
    }

    LRESULT CALLBACK Window::proc_wnd_creation_msgs(HWND hwnd, UINT msg_type, WPARAM w_param,
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

    bool Window::poll_msg()
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

    void Window::create(WindowResources& wnd_resources, const char* title, int x, int y, 
        int width, int height, DWORD style)
    {
        RECT size;
        size.top = 0;
        size.bottom = height;
        size.left = 0;
        size.right = width;

        bool success = AdjustWindowRectEx(&size, style, false, 0);
        omni_assert_win32_call(success, AdjustWindowRectEx);

        hwnd_ = CreateWindowEx(0, (LPCTSTR)wnd_resources.get_atom(), title, style, x, y,
            size.right - size.left, size.bottom - size.top, nullptr, nullptr, omni_hinst, this);
        omni_assert_win32_call(hwnd_, CreateWindowEx);

        success = ShowWindowAsync(hwnd_, SW_SHOW);
        omni_assert_win32_call(success, ShowWindowAsync);
    }

    void Window::destroy()
    {
        bool success = DestroyWindow(hwnd_);
        omni_assert_win32_call(success, DestroyWindow);
        PostQuitMessage(0);
    }
}
