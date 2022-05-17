#include <omni/core.hpp>

#include <omni/logger.hpp>

namespace Omni
{
    void init()
    {
        Logger::init();

    #if OMNI_STATIC
        // TODO: Look into GetModuleHandleEx
        exe_hinst  = GetModuleHandle(NULL);
    #else
        exe_hinst  = GetModuleHandle(NULL);
        omni_hinst = get_this_module_handle();
    #endif

        trace_log("Initialized successfully.");
    }

    HMODULE get_this_module_handle()
    {
        HMODULE hmodule;
        ::GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS |
            GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
            (LPCTSTR)get_this_module_handle, &hmodule);

        return hmodule;
    }
}