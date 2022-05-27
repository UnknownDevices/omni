#pragma once

#include <omni/logger.hpp>
#include <omni/core.hpp>

int wmain(/**int argc, wchar_t** argv**/)
{
	Omni::init();
	Omni::run();

	Omni::trace_log("Main returning normally...");
	return 0;
}
