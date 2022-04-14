#pragma once

#include <Windows.h>
#include <omni/omni.hpp>
#include <omni/dx12_window.hpp>

int CALLBACK
WinMain(HINSTANCE, HINSTANCE, LPSTR, INT)
{
	Omni::init();
	Omni::Dx12_Window().run();

	return 0;
}