#pragma once

#include <omni/logger.hpp>
#include <omni/app.hpp>
#include <omni/target_platform_window.hpp>

int main(int argc,char* argv[])
{
    using namespace Omni;

    srand(static_cast<unsigned>(time(nullptr)));

    Logger::init();
	Target_Platform_Window::init_renderer(); 
    App::init(*new App());

    App::get_app().run();

    Logger::spdlog_logger->trace("Exiting Omni Engine");
    return 0;
}