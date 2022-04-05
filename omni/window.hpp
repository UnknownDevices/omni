#pragma once

#include <omni/utility/function.hpp>
#include <omni/window_event.hpp>
#include <omni/rendering_layer.hpp> 

namespace Omni
{
    template <typename Type>
    class Window;

    template <typename Window_Type>
    auto downcast_window(Window<Window_Type>& window) -> Window_Type&;

    template <typename Window_Type>
    auto downcast_window(const Window<Window_Type>& window) -> const Window_Type&;
 
    template <typename Type>
    class Window
    {
    public:
        using Callback = Function<bool, const Window_Event&>;
        using Callback_Vector = std::vector<Callback>; 
        using Rendering_Layer_Vector = std::vector<Rendering_Layer*>; 

        // TODO: Declare specifiers
        [[nodiscard]] static auto is_renderer_init() -> bool { return Type::is_renderer_init(); }

        static auto init_renderer() -> void { Type::init_renderer(); }

		[[nodiscard]] auto is_vsync() const -> bool { return downcast_window(*this).is_vsync(); } 

		[[nodiscard]] auto get_title() const -> std::string_view { return downcast_window(*this).get_title();; }

        [[nodiscard]] auto get_height() const -> int32_fast { return downcast_window(*this).get_height(); }

        [[nodiscard]] auto get_width() const -> int32_fast { return downcast_window(*this).get_width(); }

        [[nodiscard]] auto get_callbacks() -> Callback_Vector& 
        { 
            return downcast_window(*this).get_callbacks(); 
        }

        [[nodiscard]] auto get_rendering_layers() -> Rendering_Layer_Vector& 
        {
            return downcast_window(*this).get_rendering_layers(); 
        }

        auto start_rendering_thread(bool vsync = true) -> void 
        { 
            downcast_window(*this).start_rendering_thread(vsync); 
        }
		
        auto set_rendering_thread_to_stop() -> void 
        { 
            downcast_window(*this).set_rendering_thread_to_stop(); 
        }

        auto join_rendering_thread() -> void 
        { 
            downcast_window(*this).join_rendering_thread(); 
        }

    protected:
        constexpr Window() noexcept = default;
        
        constexpr ~Window() noexcept = default;     
    };

    template <typename Window_Type>
    auto downcast_window(Window<Window_Type>& window) -> Window_Type&
    {
        return static_cast<Window_Type&>(window);
    }

    template <typename Window_Type>
    auto downcast_window(const Window<Window_Type>& window) -> const Window_Type&
    {
        return static_cast<const Window_Type&>(window);
    }
}
