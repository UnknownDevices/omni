#pragma once

#include <omni/window.hpp>
#include <omni/logger.hpp>

Omni::Window::WindowImplementer s_window_implementer;

Omni::Window&
get_window() noexcept { return s_window_implementer.as_window_interface(); }

int
wmain(int argc, wchar_t** argv) {
	Omni::init();

	s_window_implementer = {};
	get_window().run();

	Omni::trace_log("Exiting Successfully...");
	return 0;
}