#include <omni/dx12_window.hpp>

#include <omni/resource.h>
#include <omni/logger.hpp>

namespace Omni
{
    Dx12Window::~Dx12Window() noexcept = default;

    Dx12Window&
        Dx12Window::get_app_state(HWND hwnd)
    {
        LONG_PTR ptr = GetWindowLongPtr(hwnd, GWLP_USERDATA);
        Dx12Window* p_state = reinterpret_cast<Dx12Window*>(ptr);
        return *p_state;
    }

    LRESULT CALLBACK
        Dx12Window::windows_proc(HWND hwnd, UINT msg_type, WPARAM w_param, LPARAM l_param)
    {
        switch (msg_type)
        {
            case WM_DESTROY:
            {
                PostQuitMessage(0);
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
            default:
            {
                return DefWindowProc(hwnd, msg_type, w_param, l_param);
            }
        }
    }

    void
        Dx12Window::create_sys_window()
    {
        WNDCLASSEX wcex;
        wcex.cbSize               = sizeof(WNDCLASSEX);
        wcex.style                = CS_OWNDC;
        wcex.cbClsExtra           = 0;
        wcex.cbWndExtra           = 0;
        wcex.hCursor              = nullptr;
        wcex.hbrBackground        = nullptr;
        wcex.hIcon = wcex.hIconSm = static_cast<HICON>(LoadImage(s_omni_h_inst,
            MAKEINTRESOURCE(ICON_MAIN), IMAGE_ICON, 0, 0, LR_DEFAULTCOLOR));
        wcex.lpszClassName        = m_class_name;
        wcex.lpszMenuName         = nullptr;
        wcex.hInstance            = s_omni_h_inst;
        wcex.lpfnWndProc          = windows_proc;

        RegisterClassEx(&wcex);

        m_hwnd = CreateWindowEx(0, m_class_name, m_title, WS_OVERLAPPEDWINDOW, m_pos.x,
            m_pos.y, m_size.x, m_size.y, nullptr, nullptr, s_omni_h_inst, this);
        hard_assert(m_hwnd, "CreateWindowEx failed with code: {}!", GetLastError());

        bool succeded = AnimateWindow(m_hwnd, 200, AW_SLIDE | AW_VER_POSITIVE);
        hard_assert(succeded, "AnimateWindow failed with code: {}!", GetLastError());

        MSG msg;
        while (true)
        {
            if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
            {
                if (msg.message == WM_QUIT)
                    break;

                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }
    }
}
