#pragma once

#include <omni/win/windows.h>
#include <omni/config.h>

#if OMNI_STATIC
#define OMNI_API
#else
#if OMNI_EXPORT_DLL
#define OMNI_API __declspec(dllexport)
#else
#define OMNI_API __declspec(dllimport)
#endif 
#endif

#if OMNI_DISABLE_LOGGER
#define OMNI_CONSTEXPR_EXCEPT_LOGS constexpr
#else
#define OMNI_CONSTEXPR_EXCEPT_LOGS
#endif

#if OMNI_DISABLE_LOGGER
#define OMNI_CONSTEVAL_EXCEPT_LOGS consteval
#else
#define OMNI_CONSTEVAL_EXCEPT_LOGS
#endif

#define OMNI_CONSTEXPR constexpr
#define OMNI_CONSTEVAL consteval
#define OMNI_INLINE inline

#if OMNI_DISABLE_FORCEINLINE
#define OMNI_FORCEINLINE inline
#else
#define OMNI_FORCEINLINE __forceinline
#endif

namespace Omni
{
    typedef int8_t   int8;
    typedef int16_t  int16;
    typedef int32_t  int32;
    typedef int64_t  int64;
    typedef uint8_t  uint8;
    typedef uint16_t uint16;
    typedef uint32_t uint32;
    typedef uint64_t uint64;

    typedef int_fast8_t   int8_fast;
    typedef int_fast16_t  int16_fast;
    typedef int_fast32_t  int32_fast;
    typedef int_fast64_t  int64_fast;
    typedef uint_fast8_t  uint8_fast;
    typedef uint_fast16_t uint16_fast;
    typedef uint_fast32_t uint32_fast;
    typedef uint_fast64_t uint64_fast;

    typedef int_least8_t   int8_least;
    typedef int_least16_t  int16_least;
    typedef int_least32_t  int32_least;
    typedef int_least64_t  int64_least;
    typedef uint_least8_t  uint8_least;
    typedef uint_least16_t uint16_least;
    typedef uint_least32_t uint32_least;
    typedef uint_least64_t uint64_least;

#if OMNI_STATIC
    inline HINSTANCE s_exe_h_inst;
    inline HINSTANCE& s_omni_h_inst = s_exe_h_inst;
#else
    inline HINSTANCE s_exe_h_inst;
    inline HINSTANCE s_omni_h_inst;
#endif

    constexpr auto make_bit_flag(auto x)
    {
        return 1U << x >> 1U;
    }

    void OMNI_API init();

    HMODULE OMNI_API get_this_module_handle();
}