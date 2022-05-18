#pragma once

#include <omni/core.hpp>
#include <omni/win/incl_windows.h>
#include <omni/win/window_resources.hpp>
#include <omni/win/window_event.hpp>
#include <omni/utility/delegate.hpp>

// TODO: EventDispatcher 

namespace Omni::Win
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

        void create(WindowResources& wnd_resources,
            const char* title = "Omni Window",
            int x = CW_USEDEFAULT,
            int y = CW_USEDEFAULT,
            int width = 720,
            int height = 480,
            DWORD style = DefaultStyle);
        void destroy();

        template <auto Invokable, typename TOwner>
        void add_callback(TOwner* owner)
        {
            using WindowEventSubtype = ArgsOf<decltype(Invokable)>::Params::template Unpack<0>;
            auto& callbacks = pv_deduce_callbacks<WindowEventSubtype>();

            using Callback = std::remove_reference<decltype(callbacks)>::type::value_type;
            callbacks.push_back(Callback::template create<TOwner, Invokable>(owner));
        }

        template <auto Invokable, typename TOwner>
        void add_callback(const TOwner* owner)
        {
            using WindowEventSubtype = ArgsOf<decltype(Invokable)>::Params::template Unpack<0>;
            auto& callbacks = pv_deduce_callbacks<WindowEventSubtype>();

            using Callback = std::remove_reference<decltype(callbacks)>::type::value_type;
            callbacks.push_back(Callback::template create<TOwner, Invokable>(owner));
        }

        template <auto Invokable>
        void add_callback()
        {
            using WindowEventSubtype = ArgsOf<decltype(Invokable)>::Params::template Unpack<0>;
            auto& callbacks = pv_deduce_callbacks<WindowEventSubtype>();

            using Callback = std::remove_reference<decltype(callbacks)>::type::value_type;
            callbacks.push_back(Callback::template create<Invokable>());
        }

        template <typename TFunctor>
        void add_callback(const TFunctor* functor)
        {
            add_callback<&TFunctor::operator()>(functor);
        }

    private:
        static bool pv_proc_button_down_msg(HWND hwnd, WPARAM wparam, LPARAM lparam);
        static bool pv_proc_button_up_msg(HWND hwnd, WPARAM wparam, LPARAM lparam);
        static bool pv_proc_key_down_msg(HWND hwnd, WPARAM wparam, LPARAM lparam);
        static bool pv_proc_key_up_msg(HWND hwnd, WPARAM wparam, LPARAM lparam);
        static bool pv_proc_char_msg(HWND hwnd, WPARAM wparam, LPARAM lparam);
        
            template <typename TWindowEventSubtype>
        OMNI_NODISCARD OMNI_CONSTEXPR auto& pv_deduce_callbacks()
        {
            if constexpr (std::is_same<TWindowEventSubtype, ButtonDownEvent*>::value)
                return button_down_callbacks_;

            else if constexpr (std::is_same<TWindowEventSubtype, ButtonUpEvent*>::value)
                return button_up_callbacks_;

            else if constexpr (std::is_same<TWindowEventSubtype, KeyDownEvent*>::value)
                return key_down_callbacks_;

            else if constexpr (std::is_same<TWindowEventSubtype, KeyUpEvent*>::value)
                return key_up_callbacks_;

            else if constexpr (std::is_same<TWindowEventSubtype, CharEvent*>::value)
                return char_callbacks_;
        }

        HWND hwnd_;
        std::vector<ButtonDownCallback> button_down_callbacks_;
        std::vector<ButtonUpCallback> button_up_callbacks_;
        std::vector<KeyDownCallback> key_down_callbacks_;
        std::vector<KeyUpCallback> key_up_callbacks_;
        std::vector<CharCallback> char_callbacks_;
    };
}
