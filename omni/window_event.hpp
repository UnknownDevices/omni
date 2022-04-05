#pragma once

#include <omni/core.h>

namespace Omni
{
	struct Window_Event
	{
		enum class Type_Flag
		{
			mouse_move_event      = BIT_FLAG(0),
			mouse_scroll_event    = BIT_FLAG(1),
			mouse_button_event    = BIT_FLAG(2),
			key_event             = BIT_FLAG(3),
			char_event            = BIT_FLAG(4),
			mouse_enter_event     = BIT_FLAG(5),
			window_focus_event    = BIT_FLAG(6),
			window_move_event     = BIT_FLAG(7),
			window_resize_event   = BIT_FLAG(8),
			window_iconify_event  = BIT_FLAG(9),
			window_maximise_event = BIT_FLAG(10),
			window_close_event    = BIT_FLAG(11),
			window_refresh_event  = BIT_FLAG(12),
		};

		Type_Flag type_flag;

	protected:
		constexpr Window_Event(Type_Flag type_flag) noexcept
			: type_flag(type_flag) {}

		~Window_Event() noexcept = default;
	};

	struct Mouse_Move_Event : public Window_Event
	{
		constexpr Mouse_Move_Event(double x_pox, double y_pos) noexcept
			: Window_Event(Type_Flag::mouse_move_event), x_pos(x_pox), y_pos(y_pos) {}

		double x_pos, y_pos;
	};

	struct Mouse_Scroll_Event : public Window_Event
	{
		constexpr Mouse_Scroll_Event(double x_offset, double y_offset) noexcept
			: Window_Event(Type_Flag::mouse_scroll_event), x_offset(x_offset), y_offset(y_offset) {}

		double x_offset, y_offset;
	};

	struct Mouse_Button_Event : public Window_Event
	{
		enum class Action
		{
			release = 0,
			press = 1
		};

		constexpr Mouse_Button_Event(int16_fast button, int16_fast mods, Action action) noexcept
			: Window_Event(Type_Flag::mouse_button_event), button(button), mods(mods), action(action) {}

		int16_fast button, mods;
		Action action;
	};

	struct Key_Event : public Window_Event
	{
		enum class Action
		{
			release = 0,
			press = 1,
			repeat = 2
		};

		constexpr Key_Event(int16_fast key, int16_fast scancode, int16_fast mods, Action action) 
			noexcept : Window_Event(Type_Flag::key_event), key(key), scancode(scancode), mods(mods),
			  action(action) {}

		int16_fast key, scancode, mods;
		Action action;
	};

	struct Char_Event : public Window_Event
	{
		constexpr Char_Event(uint16_fast codepoint) noexcept
			: Window_Event(Type_Flag::char_event), codepoint(codepoint) {}

		uint16_fast codepoint;
	};

	struct Mouse_Enter_Event : public Window_Event
	{
		constexpr Mouse_Enter_Event(bool entered) noexcept
			: Window_Event(Type_Flag::mouse_enter_event), entered(entered) {}

		bool entered;
	};

	struct Window_Focus_Event : public Window_Event
	{
		constexpr Window_Focus_Event(bool focused) noexcept
			: Window_Event(Type_Flag::window_focus_event), focused(focused) {}

		bool focused;
	};

	struct Window_Move_Event : public Window_Event
	{
		constexpr Window_Move_Event(int32_fast x_pos, int32_fast y_pos) noexcept
			: Window_Event(Type_Flag::window_move_event), x_pos(x_pos), y_pos(y_pos) {}

		double x_pos, y_pos;
	};

	struct Window_Resize_Event : public Window_Event
	{
		constexpr Window_Resize_Event(int32_fast width, int32_fast height) noexcept
			: Window_Event(Type_Flag::window_resize_event), width(width), height(height) {}

		double width, height;
	};

	struct Window_Iconify_Event : public Window_Event
	{
		constexpr Window_Iconify_Event(bool iconified) noexcept
			: Window_Event(Type_Flag::window_iconify_event), iconified(iconified) {}

		bool iconified;
	};

	struct Window_Maximise_Event : public Window_Event
	{
		constexpr Window_Maximise_Event(bool maximised) noexcept
			: Window_Event(Type_Flag::window_maximise_event), maximised(maximised) {}

		bool maximised;
	};

	struct Window_Close_Event : public Window_Event
	{
		constexpr Window_Close_Event() noexcept
			: Window_Event(Type_Flag::window_close_event) {}
	};

	struct Window_Refresh_Event : public Window_Event
	{
		constexpr Window_Refresh_Event() noexcept
			: Window_Event(Type_Flag::window_refresh_event) {}
	};
}
