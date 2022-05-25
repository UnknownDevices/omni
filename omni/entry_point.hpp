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

void foo()
{}

int wmain(/**int argc, wchar_t** argv**/)
{
	using namespace Omni;

	init();

	auto wnd_resources = WindowResources();
	wnd_resources.start();

	auto wnd = Window();
	wnd.start(wnd_resources);

	//----TESTING----------------------------------------------------------------------------------
	auto mult_del1 = MulticastDelegate<void()>();
	auto mult_del2 = MulticastDelegate<void()>();

	auto del1 = Delegate<void()>::from<&foo>();
	auto del2 = Delegate<void()>::from<&foo>();

	mult_del1.emplace<&foo>();
	mult_del2.emplace<&foo>();

	auto app = App();
	auto button_down_event = ButtonDownEvent(1, 2, 3);

	auto on_key_down = [](KeyDownEvent* key_down_event) -> bool
	{
		debug_log("key_down_event from lambda: [{}]", *key_down_event);
		return false;
	};

	wnd.emplace_button_down_callback<App, &App::on_button_down>(&app);
	wnd.emplace_button_up_callback<App, &App::on_button_up>(&app);
	wnd.emplace_key_down_callback(&on_key_down);
	wnd.emplace_key_up_callback<&on_key_up>();
	//---------------------------------------------------------------------------------------------

	while (Window::poll_msg());

	wnd_resources.stop();

	trace_log("Main return successfully.");
	return 0;
}