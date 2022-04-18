#include <omni/omni.hpp>

#include <omni/logger.hpp>

namespace Omni
{
    // TODO: Return should be custom type, ex. Omni::Error
    int16_fast 
    init()
    {
        Logger::init();

		s_exe_h_inst  = GetModuleHandle(NULL);
		s_omni_h_inst = get_this_module_handle();

        log(Logger::Level::trace, "Omni::init returning successfully...");
        return 0;
    }

    HMODULE 
    get_this_module_handle()
    {
        HMODULE hModule;
        ::GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | 
            GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT, 
            (LPCTSTR)get_this_module_handle, &hModule);

        return hModule;
    }
}