#include <omni/win/window.hpp>

#include <omni/resource.h>
#include <omni/logger.hpp>
#include <omni/utility/types_pack.hpp>

namespace Omni
{
Window& Window::get_user_data(HWND hwnd)
{
    return *reinterpret_cast<Window*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
}

LRESULT CALLBACK Window::proc_wnd_msgs(HWND hwnd, UINT msg_type, WPARAM wparam, LPARAM lparam)
{
    auto& u_wparam = *reinterpret_cast<uint64*>(&wparam);
    auto& u_lparam = *reinterpret_cast<uint64*>(&lparam);

    switch (msg_type)
    {
        case WM_DESTROY:
        {
            get_user_data(hwnd).destroy();
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
            pv_proc_button_down_msg(hwnd, wparam, lparam);
            return 0;
        }
        case WM_RBUTTONDOWN:
        {
            pv_proc_button_down_msg(hwnd, wparam, lparam);
            return 0;
        }
        case WM_MBUTTONDOWN:
        {
            pv_proc_button_down_msg(hwnd, wparam, lparam);
            return 0;
        }
        case WM_XBUTTONDOWN:
        {
            pv_proc_button_down_msg(hwnd, wparam, lparam);
            return 0;
        }
        case WM_LBUTTONUP:
        {
            pv_proc_button_up_msg(hwnd, wparam, lparam);
            return 0;
        }
        case WM_RBUTTONUP:
        {
            pv_proc_button_up_msg(hwnd, wparam, lparam);
            return 0;
        }
        case WM_MBUTTONUP:
        {
            pv_proc_button_up_msg(hwnd, wparam, lparam);
            return 0;
        }
        case WM_XBUTTONUP:
        {
            pv_proc_button_up_msg(hwnd, wparam, lparam);
            return 0;
        }
        case WM_KEYDOWN:
        {
            auto key_down_event = KeyDownEvent(
                static_cast<uint16>(u_wparam),
                static_cast<uint16>(get_bit_range(u_lparam, 16, 24)),
                static_cast<uint16>(get_bit_range(u_lparam, 0, 16)),
                static_cast<uint8> (get_bit_range(u_lparam, 30, 31)),
                static_cast<uint8> (get_bit_range(u_lparam, 24, 25)));

            get_user_data(hwnd).key_down_callbacks_(&key_down_event, &pv_handle_callback_ret);
            return 0;
        }
        case WM_KEYUP:
        {
            auto key_up_event = KeyUpEvent(
                static_cast<uint16>(u_wparam),
                static_cast<uint16>(get_bit_range(u_lparam, 16, 24)),
                static_cast<uint8> (get_bit_range(u_lparam, 24, 25)));

            get_user_data(hwnd).key_up_callbacks_(&key_up_event, &pv_handle_callback_ret);
            return 0;
        }
        case WM_CHAR:
        {
            auto char_event = CharEvent(
                static_cast<char> (wparam),
                static_cast<uint8>(get_bit_range(u_lparam, 30, 31)));

            get_user_data(hwnd).char_callbacks_(&char_event, &pv_handle_callback_ret);
            return 0;
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

bool Window::peek_msg()
{
    thread_local MSG msg;
    if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
    {
        if (msg.message == WM_QUIT)
        {
            quit = true;
            return true;
        }

        TranslateMessage(&msg);
        DispatchMessage(&msg);
        return true;
    }
    else
    {
        return false;
    }
}

void Window::make(WindowResources& wnd_resources, const char* title, int x, int y,
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

    is_null_ = false;
}

void Window::destroy()
{
    bool success = DestroyWindow(hwnd_);
    omni_assert_win32_call(success, DestroyWindow);

    is_null_ = true;
}

bool Window::pv_handle_callback_ret(size_t, bool e_handled) noexcept
{
    return !e_handled;
}

void Window::pv_proc_button_down_msg(HWND hwnd, WPARAM wparam, LPARAM lparam)
{
    auto& u_wparam = *reinterpret_cast<uint64*>(&wparam);

    auto button_down_event = ButtonDownEvent(
        *(reinterpret_cast<int16*>(&lparam)),
        *(reinterpret_cast<int16*>(&lparam) + 1),
        *(reinterpret_cast<uint8*>(&u_wparam)));

    get_user_data(hwnd).button_down_callbacks_(&button_down_event, &pv_handle_callback_ret);
}

void Window::pv_proc_button_up_msg(HWND hwnd, WPARAM wparam, LPARAM lparam)
{
    auto& u_wparam = *reinterpret_cast<uint64*>(&wparam);

    auto button_up_event = ButtonUpEvent(
        *(reinterpret_cast<int16*>(&lparam)),
        *(reinterpret_cast<int16*>(&lparam) + 1),
        *(reinterpret_cast<uint8*>(&u_wparam)));

    get_user_data(hwnd).button_up_callbacks_(&button_up_event, &pv_handle_callback_ret);
}
}
