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
#   inline static constexpr std::size_t simd_width = 1ul;
#endif

namespace boost::numeric::ublas::detail{

#if BOOST_HW_SIMD_X86 > BOOST_HW_SIMD_X86_SSE_VERSION
#   if BOOST_HW_SIMD_X86 > BOOST_HW_SIMD_X86_SSE4_2_VERSION
#       if BOOST_HW_SIMD_X86 > BOOST_HW_SIMD_X86_AVX2_VERSION
            inline static constexpr std::size_t simd_width = 512ul;
#       else
            inline static constexpr std::size_t simd_width = 256ul;
#       endif

#   else
        inline static constexpr std::size_t simd_width = 128ul;
#   endif
#else
    inline static constexpr std::size_t simd_width = 64ul;
#endif

#ifdef __ARM_FEATURE_SVE_BITS
    inline static constexpr std::size_t simd_width = __ARM_FEATURE_SVE_BITS;
#elif !defined(BOOST_NUMERIC_UBLAS_x86)
    inline static constexpr std::size_t simd_width = 128ul;
#endif

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

} // namespace boost::numeric::ublas::detail



#endif // BOOST_NUMERIC_UBLAS_OS_CONFIG_HPP
