#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <omni/window.hpp>
#include <omni/core.h>
#include <omni/window_event.hpp>

namespace Omni
{
    class OMNI_API Windows_Window : public Window<Windows_Window>
    {
    public:
        Windows_Window(bool vsync = true, int32_fast width = 720, int32_fast height = 480,
            std::string &&title = "Omni Engine",
            Callback_Vector &&callbacks = Callback_Vector(),
            Rendering_Layer_Vector &&renderering_layers = Rendering_Layer_Vector()) noexcept;

        ~Windows_Window() noexcept;

        [[nodiscard]] static auto is_renderer_init() -> bool { return s_renderer_init; }

        static auto on_glfw_error(int error, const char *description) -> void;

        static auto init_renderer() -> void;

        [[nodiscard]] auto is_vsync() const -> bool { return m_vsync; }

        [[nodiscard]] auto get_height() const -> int32_fast { return m_height; }

        [[nodiscard]] auto get_width() const -> int32_fast { return m_width; }

        [[nodiscard]] auto get_title() const -> std::string_view { return m_title; }

        [[nodiscard]] auto get_callbacks() -> Callback_Vector & { return m_callbacks; }

        [[nodiscard]] auto get_rendering_layers() -> Rendering_Layer_Vector &
        {
            return m_rendering_layers;
        }

        [[nodiscard]] auto get_glfw_window() -> GLFWwindow & { return *m_glfw_window; }

        [[nodiscard]] auto get_latest_event() -> Window_Event & { return m_latest_key_event; }

        auto start_rendering_thread(bool vsync = true) -> void;

        auto set_rendering_thread_to_stop() -> void;

        auto pause_rendering() -> void;
        
        auto resume_rendering() -> void;

        auto join_rendering_thread() -> void;

    private:
        auto pv_set_vsync(bool enable) -> void;

        auto pv_set_glfw_callbacks() -> void;

        auto pv_dispatch_event(Window_Event &event) -> void;

        auto pv_render_asynchronous(ImGuiContext* const imgui_context, bool vsync) -> void;

        static bool s_renderer_init;

        bool m_vsync, m_should_stop_rendering;
        int32_fast m_height, m_width;
        std::string m_title;
        std::thread *m_rendering_thread;
        GLFWwindow *m_glfw_window;
        union // TODO: Is there a more elegant solution to this??
        {
            Mouse_Move_Event m_latest_mouse_move_event;
            Mouse_Scroll_Event m_latest_mouse_scroll_event;
            Mouse_Button_Event m_latest_mouse_button_event;
            Key_Event m_latest_key_event;
            Char_Event m_latest_char_event;
            Mouse_Enter_Event m_latest_mouse_enter_event;
            Window_Focus_Event m_latest_window_focus_event;
            Window_Move_Event m_latest_window_move_event;
            Window_Resize_Event m_latest_window_resize_event;
            Window_Iconify_Event m_latest_window_iconify_event;
            Window_Maximise_Event m_latest_window_maximise_event;
            Window_Close_Event m_latest_window_close_event;
            Window_Refresh_Event m_latest_window_refresh_event;
        };
        Callback_Vector m_callbacks;
        Rendering_Layer_Vector m_rendering_layers;


        bool m_should_pause_rendering = false;
    };
}
