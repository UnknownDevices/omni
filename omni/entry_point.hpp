#pragma once

#include <omni/window.hpp>
#include <omni/logger.hpp>

Omni::Window::WindowImplementer s_window_implementer;

Omni::Window&
get_window() noexcept
{
	return s_window_implementer.as_window_interface();
}

int
wmain(/**int argc, wchar_t** argv**/)
{
	using namespace Omni;
	init();

	auto pos = Window::get_pos_at_monitor_center({480, 720});
	s_window_implementer = {pos, {480, 720}};
	get_window().create_sys_window();

	trace_log("Exited succesfully.");
	return 0;
}