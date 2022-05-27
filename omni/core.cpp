#include <omni/core.hpp>

#include <omni/win/window_resources.hpp>
#include <omni/win/window.hpp>
#include <omni/win/graphics.hpp>
#include <omni/logger.hpp>

namespace Omni
{
void init()
{
    Logger::init();

    quit_is_requested = false;

#if OMNI_STATIC
    exe_hinst  = GetModuleHandle(NULL);
#else
    exe_hinst  = GetModuleHandle(NULL);
    omni_hinst = get_this_module_handle();
#endif

    trace_log("Initialized global variables.");
}

void run()
{
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    auto wnd_resources = WindowResources();
    auto wnd           = Window();
    auto gfx           = Graphics();

    wnd_resources.make();
    wnd.make(wnd_resources);

    Omni::trace_log("Started rendering thread.");
    auto render_thread = std::thread([&]()
    {
        gfx.make(wnd);

        Omni::trace_log("Entered rendering loop.");
        while (!wnd.is_null())
        {
            gfx.start_frame();

            float r = static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX + 1u);
            float g = static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX + 1u);
            float b = static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX + 1u);
            gfx.clear_buffer(r, g, b);

            gfx.end_frame();
        }
        Omni::trace_log("Exited rendering loop.");

        gfx.destroy();
    });

    Omni::trace_log("Entered message proc loop.");
    while (!wnd.is_null())
        Window::peek_msg();
    Omni::trace_log("Exited message proc loop.");

    wnd_resources.destroy();

    render_thread.join();
    Omni::trace_log("Joined rendering thread.");
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
