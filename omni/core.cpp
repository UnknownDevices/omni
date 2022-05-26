#include <omni/core.hpp>

#include <omni/win/window_resources.hpp>
#include <omni/win/window.hpp>
#include <omni/logger.hpp>

namespace Omni
{
void init()
{
    quit_is_requested = false;

#if OMNI_STATIC
    exe_hinst  = GetModuleHandle(NULL);
#else
    exe_hinst  = GetModuleHandle(NULL);
    omni_hinst = get_this_module_handle(); 
#endif

    Logger::init();
    trace_log("Initialized successfully.");
}

void run()
{
    auto wnd_resources = WindowResources();
    auto wnd           = Window();

    wnd_resources.make();
    wnd.make(wnd_resources);

    while (!wnd.is_null())
        Window::peek_msg();

    wnd_resources.destroy();
}

HMODULE get_this_module_handle()
{
    HMODULE hmodule;
    GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS |
        GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
        (LPCTSTR)get_this_module_handle, &hmodule);

    return hmodule;
}
}
