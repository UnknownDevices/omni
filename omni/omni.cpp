#include <omni/omni.hpp>

namespace Omni
{
    int16_fast
    init()
    {
		s_exe_h_inst  = GetModuleHandle(NULL);
		s_omni_h_inst = GetThisModuleHandle();

        return 0;
    }

    HMODULE 
    GetThisModuleHandle()
    {
        HMODULE hModule;
        ::GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | 
            GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT, 
            (LPCTSTR)GetThisModuleHandle, &hModule);

        return hModule;
    }
}