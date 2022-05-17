#pragma once

#include <magic_enum.hpp>
#include <omni/core.hpp>
#include <omni/logger.hpp>

namespace Omni::Win
{
    using WindowEventId = uint32;

    class WindowEvent
    {
    public:
        OMNI_NODISCARD OMNI_CONSTEXPR WindowEventId get_id() const noexcept
        {
            return id_;
        }

    protected:
        OMNI_CONSTEXPR WindowEvent(WindowEventId id) noexcept : id_(id)
        {}

        OMNI_CONSTEXPR ~WindowEvent() noexcept = default;

        WindowEventId id_;
    };

    class ButtonDownEvent : public WindowEvent
    {
    public:
        static constexpr WindowEventId Id = make_bit_flag(1);

        constexpr ButtonDownEvent(int16 x, int16 y, uint8 mods) noexcept :
            WindowEvent(Id), 
            x_(x), 
            y_(y), 
            mods_(mods)
        {}

        OMNI_NODISCARD OMNI_CONSTEXPR int16 get_x() const noexcept
        {
            return x_;
        }

        OMNI_NODISCARD OMNI_CONSTEXPR int16 get_y() const noexcept
        {
            return y_;
        }

        OMNI_NODISCARD OMNI_CONSTEXPR uint8 get_mods() const noexcept
        {
            return mods_;
        }

    private:
        int16 x_, y_;
        uint8 mods_;
    };

    class ButtonUpEvent : public WindowEvent
    {
    public:
        static constexpr WindowEventId Id = make_bit_flag(2);

        constexpr ButtonUpEvent(int16 x, int16 y, uint8 mods) noexcept :
            WindowEvent(Id), 
            x_(x), 
            y_(y), 
            mods_(mods)
        {}

        OMNI_NODISCARD OMNI_CONSTEXPR int16 get_x() const noexcept
        {
            return x_;
        }

        OMNI_NODISCARD OMNI_CONSTEXPR int16 get_y() const noexcept
        {
            return y_;
        }

        OMNI_NODISCARD OMNI_CONSTEXPR uint8 get_mods() const noexcept
        {
            return mods_;
        }

    private:
        int16 x_, y_;
        uint8 mods_;
    };

    class KeyDownEvent : public WindowEvent
    {
    public:
        static constexpr WindowEventId Id = make_bit_flag(3);

        constexpr KeyDownEvent(uint16 key_code, uint16 scan_code, uint16 repeat_count, 
            uint8 is_hold, uint8 is_extended_key) noexcept :
            WindowEvent(Id), 
            key_code_(key_code), 
            scan_code_(scan_code),
            repeat_count_(repeat_count),
            is_hold_(is_hold),
            is_extended_key_(is_extended_key)
        {}

        OMNI_NODISCARD OMNI_CONSTEXPR uint16 get_key_code() const noexcept
        {
            return key_code_;
        }

        OMNI_NODISCARD OMNI_CONSTEXPR uint16 get_scan_code() const noexcept
        {
            return scan_code_;
        }

        OMNI_NODISCARD OMNI_CONSTEXPR uint16 get_repeat_count() const noexcept
        {
            return repeat_count_;
        }

        OMNI_NODISCARD OMNI_CONSTEXPR bool is_hold() const noexcept
        {
            return is_hold_;
        }

        OMNI_NODISCARD OMNI_CONSTEXPR bool is_extended_key() const noexcept
        {
            return is_extended_key_;
        }

    private:
        uint16 key_code_, scan_code_, repeat_count_;
        uint8 is_hold_, is_extended_key_;
    };

    class KeyUpEvent : public WindowEvent
    {
    public:
        static constexpr WindowEventId Id = make_bit_flag(4);

        constexpr KeyUpEvent(uint16 key_code, uint16 scan_code, uint8 is_extended_key) noexcept :
            WindowEvent(Id), 
            key_code_(key_code), 
            scan_code_(scan_code),
            is_extended_key_(is_extended_key)
        {}
        
        OMNI_NODISCARD OMNI_CONSTEXPR uint16 get_key_code() const noexcept
        {
            return key_code_;
        }

        OMNI_NODISCARD OMNI_CONSTEXPR uint16 get_scan_code() const noexcept
        {
            return scan_code_;
        }

        OMNI_NODISCARD OMNI_CONSTEXPR bool is_extended_key() const noexcept
        {
            return is_extended_key_;
        }

    private:
        uint16 key_code_, scan_code_;
        uint8 is_extended_key_;
    };

    class CharEvent : public WindowEvent
    {
    public:
        static constexpr WindowEventId Id = make_bit_flag(5);

        constexpr CharEvent(char character_code, uint8 is_hold) noexcept :
            WindowEvent(Id), 
            character_code_(character_code),
            is_hold_(is_hold)
        {}

        OMNI_NODISCARD OMNI_CONSTEXPR char get_character_code() const noexcept
        {
            return character_code_;
        }

        OMNI_NODISCARD OMNI_CONSTEXPR bool is_hold() const noexcept
        {
            return is_hold_;
        }

    private:
        char character_code_;
        uint8 is_hold_;
    };
}

template<>
struct fmt::formatter<Omni::Win::ButtonDownEvent>
{
    template<typename ParseContext>
    constexpr auto parse(ParseContext& ctx)
    {
        return ctx.begin();
    }

    template<typename FormatContext>
    auto format(const Omni::Win::ButtonDownEvent& button_down, FormatContext& ctx)
    { 
        return fmt::format_to(ctx.out(),
            "x: [{:d}] "
            "y: [{:d}] "
            "mods: [{:08b}]",
            button_down.get_x(), 
            button_down.get_y(), 
            button_down.get_mods());
    }
};

template<>
struct fmt::formatter<Omni::Win::ButtonUpEvent>
{
    template<typename ParseContext>
    constexpr auto parse(ParseContext& ctx)
    {
        return ctx.begin();
    }

    template<typename FormatContext>
    auto format(const Omni::Win::ButtonUpEvent& button_up, FormatContext& ctx)
    { 
        return fmt::format_to(ctx.out(),
            "x: [{:d}] "
            "y: [{:d}] "
            "mods: [{:08b}]",
            button_up.get_x(), 
            button_up.get_y(), 
            button_up.get_mods());
    }
};

template<>
struct fmt::formatter<Omni::Win::KeyDownEvent>
{
    template<typename ParseContext>
    constexpr auto parse(ParseContext& ctx)
    {
        return ctx.begin();
    }

    template<typename FormatContext>
    auto format(const Omni::Win::KeyDownEvent& key_down, FormatContext& ctx)
    { 
        return fmt::format_to(ctx.out(), 
            "key_code: [{:03d}] "
            "scan_code: [{:03d}] "
            "repeat_count: [{:d}] "
            "is_hold: [{:d}] "
            "is_extended_key: [{:d}]",
            key_down.get_key_code(), 
            key_down.get_scan_code(), 
            key_down.get_repeat_count(),
            key_down.is_hold(),
            key_down.is_extended_key());
    }
};

template<>
struct fmt::formatter<Omni::Win::KeyUpEvent>
{
    template<typename ParseContext>
    constexpr auto parse(ParseContext& ctx)
    {
        return ctx.begin();
    }

    template<typename FormatContext>
    auto format(const Omni::Win::KeyUpEvent& key_up, FormatContext& ctx)
    { 
        return fmt::format_to(ctx.out(), 
            "key_code: [{:03d}] "
            "scan_code: [{:03d}] "
            "is_extended_key : [{:d}]", 
            key_up.get_key_code(), 
            key_up.get_scan_code(), 
            key_up.is_extended_key());
    }
};

template<>
struct fmt::formatter<Omni::Win::CharEvent>
{
    template<typename ParseContext>
    constexpr auto parse(ParseContext& ctx)
    {
        return ctx.begin();
    }

    template<typename FormatContext>
    auto format(const Omni::Win::CharEvent& key_up, FormatContext& ctx)
    { 
        return fmt::format_to(ctx.out(), 
            "character_code: [{:03d}] "
            "is_hold: [{:d}]", 
            key_up.get_character_code(),
            key_up.is_hold());
    }
};
