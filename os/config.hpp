#if !defined(BOOST_NUMERIC_UBLAS_OS_CONFIG_HPP)
#define BOOST_NUMERIC_UBLAS_OS_CONFIG_HPP

#include <boost/predef.h>
#include <array>
#include <string>

#if BOOST_ARCH_ARM
#   define BOOST_NUMERIC_UBLAS_ARM
#endif

#if BOOST_ARCH_X86
#   define BOOST_NUMERIC_UBLAS_x86
#endif

#if BOOST_OS_LINUX
#   define BOOST_NUMERIC_UBLAS_LINUX
#endif

#if BOOST_OS_MACOS
#   define BOOST_NUMERIC_UBLAS_MACOS
#endif

#if BOOST_OS_WINDOWS
#   define BOOST_NUMERIC_UBLAS_WINDOWS
#endif

#ifndef BOOST_HW_SIMD_AVAILABLE
#   define BOOST_NUMERIC_UBLAS_SIMD_WIDTH 1ul
#endif

#if BOOST_HW_SIMD_X86 > BOOST_HW_SIMD_X86_SSE_VERSION
#   if BOOST_HW_SIMD_X86 > BOOST_HW_SIMD_X86_SSE4_2_VERSION
#       if BOOST_HW_SIMD_X86 > BOOST_HW_SIMD_X86_AVX2_VERSION
#           define BOOST_NUMERIC_UBLAS_SIMD_WIDTH 512ul
#       else
#           define BOOST_NUMERIC_UBLAS_SIMD_WIDTH 256ul
#       endif

#   else
#       define BOOST_NUMERIC_UBLAS_SIMD_WIDTH 128ul
#   endif
#else
#   define BOOST_NUMERIC_UBLAS_SIMD_WIDTH 64ul
#endif

#ifdef __ARM_FEATURE_SVE_BITS
#   define BOOST_NUMERIC_UBLAS_SIMD_WIDTH __ARM_FEATURE_SVE_BITS
#elif !defined(BOOST_NUMERIC_UBLAS_x86)
#   define BOOST_NUMERIC_UBLAS_SIMD_WIDTH 128ul
#endif

namespace boost::numeric::ublas::detail{

    template<typename ValueType>
        requires std::is_arithmetic_v<ValueType>
    inline static constexpr std::size_t simd_register_width = std::min(
        BOOST_NUMERIC_UBLAS_SIMD_WIDTH, 
        (BOOST_NUMERIC_UBLAS_SIMD_WIDTH / (CHAR_BIT * sizeof(ValueType)))
    );

/**
 * Format to provide a default level 'x' cache:
 * BOOST_NUMERIC_UBLAS_CACHE_L* = "LineSize, Associativity, Sets"
 * NOTE: The default values if not given are adjusted according to Apple
 * hardware because MacOS does not provides all the information.
 */

#ifndef BOOST_NUMERIC_UBLAS_CACHE_L1

#   if defined(BOOST_NUMERIC_UBLAS_x86)
        inline static constexpr std::array default_l1_cache = { 64ul, 8ul, 64ul };
#   else
        inline static constexpr std::array default_l1_cache = { 128ul, 4ul, 128ul };
#   endif

#else
    inline static constexpr std::array default_l1_cache = { BOOST_NUMERIC_UBLAS_CACHE_L1 };
    static_assert(default_l1_cache.size() == 3ul, "invalid BOOST_NUMERIC_UBLAS_CACHE_L1 arity");
#endif
    

#ifndef BOOST_NUMERIC_UBLAS_CACHE_L2

#   if defined(BOOST_NUMERIC_UBLAS_x86)
        inline static constexpr std::array default_l2_cache = { 64ul, 4ul, 1024ul };
#   else
        inline static constexpr std::array default_l2_cache = { 128ul, 16ul, 2048ul };
#   endif

#else
    inline static constexpr std::array default_l2_cache = { BOOST_NUMERIC_UBLAS_CACHE_L2 };
    static_assert(default_l2_cache.size() == 3ul, "invalid BOOST_NUMERIC_UBLAS_CACHE_L2 arity");
#endif

#ifndef BOOST_NUMERIC_UBLAS_CACHE_L3
    inline static constexpr std::array default_l3_cache = { 64ul, 16ul, 16384ul };
#else
    inline static constexpr std::array default_l3_cache = { BOOST_NUMERIC_UBLAS_CACHE_L3 };
    static_assert(default_l3_cache.size() == 3ul, "invalid BOOST_NUMERIC_UBLAS_CACHE_L3 arity");
#endif
    
    static_assert(std::is_integral_v<decltype(default_l1_cache)::value_type>);
    static_assert(std::is_integral_v<decltype(default_l2_cache)::value_type>);
    static_assert(std::is_integral_v<decltype(default_l3_cache)::value_type>);

} // namespace boost::numeric::ublas::detail

#undef BOOST_NUMERIC_UBLAS_SIMD_WIDTH

#endif // BOOST_NUMERIC_UBLAS_OS_CONFIG_HPP
