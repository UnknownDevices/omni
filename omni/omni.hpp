#pragma once  

#include <Windows.h>
#include <omni/core.h>

namespace Omni
{
    inline HINSTANCE s_exe_h_inst;
	inline HINSTANCE s_omni_h_inst;

    int16_fast OMNI_API
    init();
    
    HMODULE OMNI_API
    get_this_module_handle();
}