#pragma once

#pragma warning( push )
#pragma warning( disable : 4251 )

#include <omni/core.hpp>
#include <omni/win/windows.h>

namespace Omni::Win
{
    class OMNI_API WndRscs
    {
    public:
        OMNI_CONSTEXPR WndRscs() noexcept = default;
        OMNI_CONSTEXPR ~WndRscs() noexcept = default;

        static void increment(std::string& str, size_t begin_i = 0, bool allow_resize = true);

        void start();
        void stop();

        ATOM get_atom() const
        {
            return m_atom;
        }

    private:
        constexpr static size_t s_begin = 5;
        inline static std::string s_class_name = "omni-";
        ATOM m_atom;
    };
}

#pragma warning( pop )
 