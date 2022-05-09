#include <omni/core.hpp>

#include <omni/logger.hpp>

namespace Omni
{
    void
        init()
    {
        Logger::init();
        
    #if OMNI_STATIC
        s_exe_h_inst  = GetModuleHandle(NULL); // TODO: Look into GetModuleHandleEx
    #else
        s_exe_h_inst  = GetModuleHandle(NULL);
        s_omni_h_inst = get_this_module_handle();
    #endif

        trace_log("Initialized successfully.");
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