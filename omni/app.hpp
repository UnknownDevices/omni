#pragma once

#include <omni/core.h>
#include <omni/target_platform_window.hpp>
#include <omni/window.hpp>
#include <omni/window_event.hpp>

namespace Omni
{
    class OMNI_API App
    {
    public:
		App() noexcept ;
		virtual ~App() noexcept;

		[[nodiscard]] static auto get_app() -> App& { return *s_app; }

		static auto init(App& app) -> void;

        [[nodiscard]] auto get_window() -> Window<Target_Platform_Window>& 
        { 
            return m_platform_window; 
        }
            
        [[nodiscard]] auto get_target_platform_window() -> Target_Platform_Window& 
        { 
            return m_platform_window; 
        }

		auto run() -> void;
        
    private:
        static App* s_app;
        Target_Platform_Window m_platform_window;
    };
}