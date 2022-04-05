#pragma once 
 
#include <imgui_impl_glfw.h>
#include <omni/window_event.hpp>
#include <omni/app.hpp>

// TODO: Implement glfw as glfw_impl
namespace Omni
{
	auto forward_window_event_to_imgui(const Window_Event& event) -> bool
	{ 
        switch(event.type_flag)
		{
			case Window_Event::Type_Flag::mouse_move_event:
			{
				auto& mouse_move_event = static_cast<const Mouse_Move_Event&>(event);

				ImGui_ImplGlfw_CursorPosCallback(
					&App::get_app().get_target_platform_window().get_glfw_window(),
					 mouse_move_event.x_pos, mouse_move_event.y_pos);

				return false;
			}
			case Window_Event::Type_Flag::mouse_button_event:
			{
				if(!ImGui::GetIO().WantCaptureMouse)
					return false;

				auto& mouse_button_event = static_cast<const Mouse_Button_Event&>(event);
				
				ImGui_ImplGlfw_MouseButtonCallback(
					&App::get_app().get_target_platform_window().get_glfw_window(),
					 mouse_button_event.button, static_cast<int>(mouse_button_event.action), 
					 mouse_button_event.mods);

				return true;
			}
			case Window_Event::Type_Flag::mouse_scroll_event:
			{
				if(!ImGui::GetIO().WantCaptureMouse)
					return false;
					
				auto& mouse_scroll_event = static_cast<const Mouse_Scroll_Event&>(event);

				ImGui_ImplGlfw_ScrollCallback(
					&App::get_app().get_target_platform_window().get_glfw_window(),
					 mouse_scroll_event.x_offset, mouse_scroll_event.y_offset);

				return true;
			}
			case Window_Event::Type_Flag::mouse_enter_event:
			{
				auto& mouse_enter_event = static_cast<const Mouse_Enter_Event&>(event);

				ImGui_ImplGlfw_CursorEnterCallback(
					&App::get_app().get_target_platform_window().get_glfw_window(), 
					static_cast<int>(mouse_enter_event.entered));

				return false;
			}
			case Window_Event::Type_Flag::key_event:
			{
				if(!ImGui::GetIO().WantCaptureKeyboard)
					return false;

				auto& key_event = static_cast<const Key_Event&>(event);

				ImGui_ImplGlfw_KeyCallback(
					&App::get_app().get_target_platform_window().get_glfw_window(), 
					key_event.key, key_event.scancode, static_cast<int>(key_event.action),
					key_event.mods);

				return true;
			}
			case Window_Event::Type_Flag::char_event:
			{
				if(!ImGui::GetIO().WantTextInput)
					return false;

				auto& char_event = static_cast<const Char_Event&>(event);
				ImGui_ImplGlfw_CharCallback(
					&App::get_app().get_target_platform_window().get_glfw_window(), 
					char_event.codepoint);

				return true;
			}
			case Window_Event::Type_Flag::window_focus_event:
			{
				auto& window_focus_event = static_cast<const Window_Focus_Event&>(event);
				
				ImGui_ImplGlfw_WindowFocusCallback(
					&App::get_app().get_target_platform_window().get_glfw_window(), 
					static_cast<int>(window_focus_event.focused));

				return false;
			}
			default:
				return false;
    	}
	}
}