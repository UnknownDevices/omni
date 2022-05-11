#pragma once

#include <omni/logger.hpp>
#include <omni/win/wnd_rscs.hpp>
#include <omni/win/wnd.hpp>

std::string stream_as_decimals(const std::string& str)
{
	std::string ret = {};

	for (auto itr = str.begin(); itr < str.end(); ++itr)
		ret.append(std::format("[{:03d}] ", *itr));

	return ret;
}

int wmain(/**int argc, wchar_t** argv**/)
{
	Omni::init();

	Omni::Win::WndRscs wnd_rscs;
	wnd_rscs.start();
	Omni::Win::Wnd wnd;
	wnd.start(wnd_rscs);

	while (Omni::Win::Wnd::poll_msg());

	wnd_rscs.stop();

	Omni::trace_log("Exited succesfully.");
	return 0;
}