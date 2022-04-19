#include <omni/core.hpp>

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

        trace_log("Initialized Successfully...");
        return 0;
    }

    HMODULE 
    get_this_module_handle()
    {
        HMODULE h_module;
        ::GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | 
            GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT, 
            (LPCTSTR)get_this_module_handle, &h_module);

        return h_module;
    }
}