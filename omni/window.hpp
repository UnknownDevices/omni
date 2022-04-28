#pragma once

#include <omni/window_interface.hpp>
#include <omni/dx12_window.hpp>

namespace Omni
{
    using Window = WindowInterface<Dx12Window>;
}