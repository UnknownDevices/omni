#pragma once

#include <omni/logger.hpp>
#include <omni/app.hpp>
#include <omni/win/window_resources.hpp>
#include <omni/win/window.hpp>
#include <omni/utility/delegate.hpp>

bool on_key_up(Omni::KeyUpEvent* key_up_event)
{
	debug_log("key_up_event from static fn: [{}]", *key_up_event);
	return false;
}

int wmain(/**int argc, wchar_t** argv**/)
{
	using namespace Omni;

	init();

	auto wnd_resources = WindowResources();
	wnd_resources.start();

	auto wnd = Window();	
	wnd.start(wnd_resources);

//----TESTING--------------------------------------------------------------------------------------
	const auto app = App();
	
	auto on_key_down = [](KeyDownEvent* key_down_event) -> bool
	{
		debug_log("key_down_event from lambda: [{}]", *key_down_event);
		return false;
	};

	wnd.add_callback(Window::ButtonDownDelegate::from<App, &App::on_button_down>(&app));
	wnd.add_callback(Window::ButtonDownDelegate::from<App, &App::on_button_down>(&app));
	wnd.add_callback(Window::ButtonUpDelegate::from<App, &App::on_button_up>(&app));
	wnd.add_callback(Window::KeyDownDelegate::from(&on_key_down));
	wnd.add_callback(Window::KeyUpDelegate::from<&on_key_up>());
//-------------------------------------------------------------------------------------------------

	while (Window::poll_msg());

	wnd_resources.stop();

	trace_log("Main return successfully.");
	return 0;
}