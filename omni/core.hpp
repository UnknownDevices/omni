#pragma once

#ifdef OMNI_BUILD_DLL
    #define OMNI_API __declspec(dllexport)
#else
    #define OMNI_API __declspec(dllimport)
#endif

namespace Omni
{
    typedef int8_t    int8;
    typedef int16_t   int16;
    typedef int32_t   int32;
    typedef int64_t   int64;
    typedef uint8_t   uint8;
    typedef uint16_t  uint16;
    typedef uint32_t  uint32;
    typedef uint64_t  uint64;

    typedef int_fast8_t    int8_fast;
    typedef int_fast16_t   int16_fast;
    typedef int_fast32_t   int32_fast;
    typedef int_fast64_t   int64_fast;
    typedef uint_fast8_t   uint8_fast;
    typedef uint_fast16_t  uint16_fast;
    typedef uint_fast32_t  uint32_fast;
    typedef uint_fast64_t  uint64_fast;

    typedef int_least8_t    int8_least;
    typedef int_least16_t   int16_least;
    typedef int_least32_t   int32_least;
    typedef int_least64_t   int64_least;
    typedef uint_least8_t   uint8_least;
    typedef uint_least16_t  uint16_least;
    typedef uint_least32_t  uint32_least;
    typedef uint_least64_t  uint64_least;

    inline HINSTANCE s_exe_h_inst;
	inline HINSTANCE s_omni_h_inst;

    inline constexpr auto
    bit_flag(size_t x) { return 1 << x; }

    int16_fast OMNI_API
    init();
    
    HMODULE OMNI_API
    get_this_module_handle();
}