#pragma once

#include <omni/logger.hpp>
#include <omni/win/window_resources.hpp>
#include <omni/win/window.hpp>

int wmain(/**int argc, wchar_t** argv**/)
{
	Omni::init();

	Omni::Win::WindowResources wnd_resources;
	wnd_resources.create();
	Omni::Win::Window wnd;
	wnd.create(wnd_resources);

	while (Omni::Win::Window::poll_msg());

	wnd_resources.destroy();

	int ret = 0;
	Omni::trace_log("Returning with code: [{}].", ret);
	return 0;
}