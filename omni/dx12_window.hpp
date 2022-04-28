#pragma once

#include <Windows.h>
#include <omni/core.hpp>
#include <omni/window_interface.hpp>

namespace Omni
{
    class OMNI_API Dx12Window : public WindowInterface<Dx12Window>
    {
    public:
        constexpr
        Dx12Window() noexcept;
        ~Dx12Window() noexcept;

    	static LRESULT CALLBACK 
	    WindowProc(HWND h_wnd, UINT msg_type, WPARAM w_param, LPARAM l_param);

        void
        run();

    private:
        LPCSTR m_class_name;
	    LPCSTR m_title;
	    INT	   m_width;
	    INT    m_height;
    };
}
