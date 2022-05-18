#pragma once

#include <omni/core.hpp>
#include <omni/win/window_event.hpp>

namespace Omni
{
    struct App
    {
        bool on_button_down(Win::ButtonDownEvent* button_down_event)
        {
            debug_log("button_down_event from member fn: [{}]", *button_down_event);
            return false;
        }

        bool on_button_up(Win::ButtonUpEvent* button_up_event)
        {
            debug_log("button_up_event from member fn: [{}]", *button_up_event);
            return false;
        }
    };
}