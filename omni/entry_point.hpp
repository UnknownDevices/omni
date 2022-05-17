#pragma once

#include <omni/logger.hpp>
#include <omni/win/window_resources.hpp>
#include <omni/win/window.hpp>

int wmain(/**int argc, wchar_t** argv**/)
{
	Omni::init();

	Omni::Win::WndRscs wnd_rscs;
	wnd_rscs.create();
	Omni::Win::Wnd wnd;
	wnd.create(wnd_rscs);

	while (Omni::Win::Wnd::poll_msg());

	wnd_rscs.destroy();

	int ret = 0;
	Omni::trace_log("Returning with code: [{}].", ret);
	return 0;
}