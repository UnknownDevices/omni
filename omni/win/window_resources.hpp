#pragma once

#pragma warning( push )
#pragma warning( disable : 4251 )

#include <omni/core.hpp>
#include <omni/win/incl_windows.h>

namespace Omni
{
    class OMNI_API WindowResources
    {
    public:
        OMNI_CONSTEXPR WindowResources() noexcept = default;
        OMNI_CONSTEXPR ~WindowResources() noexcept = default;

        static void increment(std::string& str, size_t begin_i = 0, bool allow_resize = true);

        void start();
        void stop();

        ATOM get_atom() const
        {
            return atom_;
        }

    private:
        constexpr static size_t s_begin = 5;
        inline static std::string s_class_name = "omni-";
        ATOM atom_;
    };
}

#pragma warning( pop )
 