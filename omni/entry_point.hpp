#pragma once

#include <omni/logger.hpp>
#include <omni/app.hpp>
#include <omni/win/window_resources.hpp>
#include <omni/win/window.hpp>
#include <omni/utility/delegate.hpp>

bool on_key_up(Omni::Win::KeyUpEvent* key_up_event)
{
	Omni::debug_log("key_up_event from static fn: [{}]", *key_up_event);
	return false;
}

int wmain(/**int argc, wchar_t** argv**/)
{
	Omni::init();

	Omni::Win::WindowResources wnd_resources;
	wnd_resources.create();
	Omni::Win::Window wnd;
	wnd.create(wnd_resources);

//----TESTING--------------------------------------------------------------------------------------
	auto app = Omni::App();
	auto on_key_down = [](Omni::Win::KeyDownEvent* key_down_event) -> bool
	{
		Omni::debug_log("key_down_event from lambda: [{}]", *key_down_event);
		return false;
	};

	wnd.add_callback<&Omni::App::on_button_down>(&app);
	wnd.add_callback<&Omni::App::on_button_up>(&app);
	wnd.add_callback(&on_key_down);
	wnd.add_callback<&on_key_up>();
//-------------------------------------------------------------------------------------------------

	while (Omni::Win::Window::poll_msg());

	wnd_resources.destroy();

	int ret = 0;
	Omni::trace_log("Returning with code: [{}].", ret);
	return 0;
}