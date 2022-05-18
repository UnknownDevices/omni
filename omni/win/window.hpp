#pragma once

#include <omni/core.hpp>
#include <omni/win/incl_windows.h>
#include <omni/win/window_resources.hpp>
#include <omni/win/window_event.hpp>
#include <omni/utility/delegate.hpp>

namespace Omni
{
    class OMNI_API Window
    {
    public:
        using ButtonDownDelegate = Delegate<bool(ButtonDownEvent*)>;
        using ButtonUpDelegate   = Delegate<bool(ButtonUpEvent*)>;
        using KeyDownDelegate    = Delegate<bool(KeyDownEvent*)>;
        using KeyUpDelegate      = Delegate<bool(KeyUpEvent*)>;
        using CharDelegate       = Delegate<bool(CharEvent*)>;

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

        template <typename TDelegate>
        void add_callback(TDelegate callback)
        {
            using WindowEventSubtype = UnpackArg<TDelegate::Params, 0>;
            auto& callbacks = pv_deduce_callbacks<WindowEventSubtype>();

            callbacks.push_back(callback);
        }

    private:
        static void pv_proc_button_down_msg(HWND hwnd, WPARAM wparam, LPARAM lparam);
        static void pv_proc_button_up_msg(HWND hwnd, WPARAM wparam, LPARAM lparam);
        
        template <typename TWindowEventSubtype>
        OMNI_CONSTEXPR auto& pv_deduce_callbacks()
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
        std::vector<ButtonDownDelegate> button_down_callbacks_;
        std::vector<ButtonUpDelegate> button_up_callbacks_;
        std::vector<KeyDownDelegate> key_down_callbacks_;
        std::vector<KeyUpDelegate> key_up_callbacks_;
        std::vector<CharDelegate> char_callbacks_;
    };
}
