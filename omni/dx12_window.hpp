#pragma once

#include <Windows.h>
#include <omni/core.hpp>
#include <omni/window_interface.hpp>
#include <omni/math.hpp>

namespace Omni
{
    class OMNI_API Dx12Window : public WindowInterface<Dx12Window>
    {
    public:
        constexpr
            Dx12Window() noexcept = default;

        constexpr
            Dx12Window(Vector2D<int> pos, Vector2D<int> size) noexcept :
            m_class_name("Omni Engine Class"), m_title("Omni Engine"), m_pos(pos), m_size(size)
        {}

        constexpr
            ~Dx12Window() noexcept = default;

        static Vector2D<int>
            get_pos_at_monitor_center(Vector2D<int> size);

        static Dx12Window&
            get_app_state(HWND hwnd);

        static LRESULT CALLBACK
            windows_proc(HWND hwnd, UINT msg_type, WPARAM w_param, LPARAM l_param);

        void
            create_sys_window();

    private:
        LPCSTR m_class_name, m_title;
        HWND   m_hwnd;
        Vector2D<int> m_size, m_pos;
    };
}
