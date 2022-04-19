#pragma once

#include <Windows.h>
#include <omni/core.h>

namespace Omni
{
    class OMNI_API Dx12_Window
    {
    public:
        constexpr
        Dx12_Window() noexcept;
        ~Dx12_Window() noexcept;

    	static LRESULT CALLBACK 
	    WindowProc(HWND h_wnd, UINT msg_type, WPARAM w_param, LPARAM l_param);

        int16_fast
        run();

    private:
        LPCSTR m_class_name;
	    LPCSTR m_title;
	    INT	   m_width;
	    INT    m_height;
    };
}
