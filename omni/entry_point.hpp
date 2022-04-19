#pragma once

#include <omni/omni.hpp>
#include <omni/dx12_window.hpp>
#include <omni/logger.hpp>

int
wmain(int argc, wchar_t** argv)
{
	Omni::init();
	Omni::Dx12_Window().run();

	Omni::trace_log("Omni Exiting Successfully...");
	return 0;
}