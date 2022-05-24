#pragma once

#include <omni/core.hpp>
#include <omni/win/incl_windows.h>
#include <omni/win/window_resources.hpp>
#include <omni/win/window_event.hpp>
#include <omni/utility/multicast_delegate.hpp>

namespace Omni
{
class OMNI_API Window
{
public:
	using ButtonDownCallback = Delegate<bool(ButtonDownEvent*)>;
	using ButtonUpCallback   = Delegate<bool(ButtonUpEvent*)>;
	using KeyDownCallback    = Delegate<bool(KeyDownEvent*)>;
	using KeyUpCallback      = Delegate<bool(KeyUpEvent*)>;
	using CharCallback       = Delegate<bool(CharEvent*)>;

	static constexpr DWORD DefaultStyle = WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;

	OMNI_CONSTEXPR Window() noexcept = default;
	OMNI_CONSTEXPR ~Window() noexcept = default;

	static Window& get_user_data(HWND hwnd);
	static LRESULT CALLBACK proc_wnd_msgs(HWND hwnd, UINT msg_type, WPARAM wparam,
		LPARAM lparam);
	static LRESULT CALLBACK proc_wnd_creation_msgs(HWND hwnd, UINT msg_type, WPARAM wparam,
		LPARAM lparam);

	static bool poll_msg();

	void start(WindowResources& wnd_resources,
		const char* title = "Omni Window",
		int x = CW_USEDEFAULT,
		int y = CW_USEDEFAULT,
		int width = 720,
		int height = 480,
		DWORD style = DefaultStyle);
	void stop();

	void add_button_down_callback(ButtonDownCallback callback)
	{
		button_down_callbacks_ += callback;
	}

	void add_button_up_callback(ButtonUpCallback callback)
	{
		button_up_callbacks_ += callback;
	}

	void add_key_down_callback(KeyDownCallback callback)
	{
		key_down_callbacks_ += callback;
	}

	void add_key_up_callback(KeyUpCallback callback)
	{
		key_up_callbacks_ += callback;
	}

	void add_char_callback(CharCallback callback)
	{
		char_callbacks_ += callback;
	}

	template <ButtonDownCallback::Function TFn>
	void emplace_button_down_callback()
	{
		add_button_down_callback(ButtonDownCallback::from<TFn>());
	}

	template <class TOwner, ButtonDownCallback::Method<TOwner> TMth>
	void emplace_button_down_callback(TOwner* owner)
	{
		add_button_down_callback(ButtonDownCallback::from<TOwner, TMth>(owner));
	}

	template <class TOwner, ButtonDownCallback::ConstMethod<TOwner> TMth>
	void emplace_button_down_callback(const TOwner* owner)
	{
		add_button_down_callback(ButtonDownCallback::from<TOwner, TMth>(owner));
	}

	template <typename TFunctor>
	void emplace_button_down_callback(const TFunctor* functor)
	{
		add_button_down_callback(ButtonDownCallback::from(functor));
	}

	template <ButtonUpCallback::Function TFn>
	void emplace_button_up_callback()
	{
		add_button_up_callback(ButtonUpCallback::from<TFn>());
	}

	template <class TOwner, ButtonUpCallback::Method<TOwner> TMth>
	void emplace_button_up_callback(TOwner* owner)
	{
		add_button_up_callback(ButtonUpCallback::from<TOwner, TMth>(owner));
	}

	template <class TOwner, ButtonUpCallback::ConstMethod<TOwner> TMth>
	void emplace_button_up_callback(const TOwner* owner)
	{
		add_button_up_callback(ButtonUpCallback::from<TOwner, TMth>(owner));
	}

	template <typename TFunctor>
	void emplace_button_up_callback(const TFunctor* functor)
	{
		add_button_up_callback(ButtonUpCallback::from(functor));
	}

	template <KeyDownCallback::Function TFn>
	void emplace_key_down_callback()
	{
		add_key_down_callback(KeyDownCallback::from<TFn>());
	}

	template <class TOwner, KeyDownCallback::Method<TOwner> TMth>
	void emplace_key_down_callback(TOwner* owner)
	{
		add_key_down_callback(KeyDownCallback::from<TOwner, TMth>(owner));
	}

	template <class TOwner, KeyDownCallback::ConstMethod<TOwner> TMth>
	void emplace_key_down_callback(const TOwner* owner)
	{
		add_key_down_callback(KeyDownCallback::from<TOwner, TMth>(owner));
	}

	template <typename TFunctor>
	void emplace_key_down_callback(const TFunctor* functor)
	{
		add_key_down_callback(KeyDownCallback::from(functor));
	}

	template <KeyUpCallback::Function TFn>
	void emplace_key_up_callback()
	{
		add_key_up_callback(KeyUpCallback::from<TFn>());
	}

	template <class TOwner, KeyUpCallback::Method<TOwner> TMth>
	void emplace_key_up_callback(TOwner* owner)
	{
		add_key_up_callback(KeyUpCallback::from<TOwner, TMth>(owner));
	}

	template <class TOwner, KeyUpCallback::ConstMethod<TOwner> TMth>
	void emplace_key_up_callback(const TOwner* owner)
	{
		add_key_up_callback(KeyUpCallback::from<TOwner, TMth>(owner));
	}

	template <CharCallback::Function TFn>
	void emplace_char_callback()
	{
		add_char_callback(CharCallback::from<TFn>());
	}

	template <class TOwner, CharCallback::Method<TOwner> TMth>
	void emplace_char_callback(TOwner* owner)
	{
		add_char_callback(CharCallback::from<TOwner, TMth>(owner));
	}

	template <class TOwner, CharCallback::ConstMethod<TOwner> TMth>
	void emplace_char_callback(const TOwner* owner)
	{
		add_char_callback(CharCallback::from<TOwner, TMth>(owner));
	}

	template <typename TFunctor>
	void emplace_char_callback(const TFunctor* functor)
	{
		add_char_callback(CharCallback::from(functor));
	}

private:
	static bool pv_handle_callback_ret(size_t, bool e_handled) noexcept;
	static void pv_proc_button_down_msg(HWND hwnd, WPARAM wparam, LPARAM lparam);
	static void pv_proc_button_up_msg(HWND hwnd, WPARAM wparam, LPARAM lparam);

	HWND hwnd_;
	MulticastDelegate<bool(ButtonDownEvent*)> button_down_callbacks_;
	MulticastDelegate<bool(ButtonUpEvent*)> button_up_callbacks_;
	MulticastDelegate<bool(KeyDownEvent*)> key_down_callbacks_;
	MulticastDelegate<bool(KeyUpEvent*)> key_up_callbacks_;
	MulticastDelegate<bool(CharEvent*)> char_callbacks_;
};
}
