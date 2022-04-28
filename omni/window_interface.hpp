#pragma once

#include <omni/core.hpp>

namespace Omni
{
    template<typename TImplementer>
    class WindowInterface
    {
    public:
        using WindowImplementer = TImplementer;

        ~WindowInterface() noexcept = default;

        constexpr WindowInterface<WindowImplementer>&
        as_window_interface() noexcept {
            return *this;
        }

        constexpr WindowImplementer&
        as_window_implementer() noexcept {
            return static_cast<WindowImplementer&>(*this);
        }

        void
        run() {
            as_window_implementer().run();
        }

    protected:
        constexpr 
        WindowInterface() noexcept {};
    };
}