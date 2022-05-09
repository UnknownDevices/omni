#pragma once

#include <omni/core.hpp>
#include <omni/math.hpp>

namespace Omni
{
    template<typename TImplementer>
    class WindowInterface
    {
    public:
        using WindowImplementer = TImplementer;

        ~WindowInterface() noexcept = default;

        constexpr WindowInterface<WindowImplementer>&
            as_window_interface() noexcept
        {
            return *this;
        }

        constexpr WindowImplementer&
            as_window_implementer() noexcept
        {
            return static_cast<WindowImplementer&>(*this);
        }

        static Vector2D<int>
            get_pos_at_monitor_center(Vector2D<int> size)
        {
            return WindowImplementer::get_pos_at_monitor_center(size);
        }

        void
            create_sys_window()
        {
            as_window_implementer().create_sys_window();
        }

    protected:
        constexpr
            WindowInterface() noexcept
        {};
    };
}