#if !defined(BOOST_NUMERIC_UBLAS_OS_LINUX_CACHE_HPP)
#define BOOST_NUMERIC_UBLAS_OS_LINUX_CACHE_HPP

#include "../config.hpp"
#include <optional>
#include <unistd.h>
#include <array>

namespace boost::numeric::ublas{

    namespace detail{
        
        template<typename IntType>
        constexpr std::optional<IntType> sysconf_helper(int name) noexcept{
            if( auto out = sysconf(name); out > 0 ){
                return static_cast<IntType>(out);
            }else{
                return std::nullopt;
            }
        }

        constexpr auto get_level_one() noexcept 
            -> std::optional<cache_info>
        {
            cache_info res{};

            res.line_size = sysconf_helper<std::size_t>(_SC_LEVEL1_DCACHE_LINESIZE).value_or(default_l1_cache[0]);
            res.associativity = sysconf_helper<std::size_t>(_SC_LEVEL1_DCACHE_ASSOC).value_or(default_l1_cache[1]);
            res.sets = default_l1_cache[2];

            if( auto const cache_size_or = sysconf_helper<std::size_t>(_SC_LEVEL1_DCACHE_SIZE); cache_size_or.has_value() ){
                auto const cache_size = cache_size_or.value();
                res.sets = cache_size / (res.associativity * res.line_size);
            }else{
                return std::nullopt;
            }

            return res;
        }

        constexpr auto get_level_two() noexcept 
            -> std::optional<cache_info>
        {
            cache_info res{};

            res.line_size = sysconf_helper<std::size_t>(_SC_LEVEL2_CACHE_LINESIZE).value_or(default_l2_cache[0]);
            res.associativity = sysconf_helper<std::size_t>(_SC_LEVEL2_CACHE_ASSOC).value_or(default_l2_cache[1]);
            res.sets = default_l2_cache[2];

            if( auto const cache_size_or = sysconf_helper<std::size_t>(_SC_LEVEL2_CACHE_SIZE); cache_size_or.has_value() ){
                auto const cache_size = cache_size_or.value();
                res.sets = cache_size / (res.associativity * res.line_size);
            }else{
                return std::nullopt;
            }

            return res;
        }

        constexpr auto get_level_three() noexcept 
            -> std::optional<cache_info>
        {
            #if defined(BOOST_NUMERIC_UBLAS_CACHE_L3) || defined(BOOST_NUMERIC_UBLAS_x86)
                cache_info res{};

                res.line_size = sysconf_helper<std::size_t>(_SC_LEVEL3_CACHE_LINESIZE).value_or(default_l3_cache[0]);
                res.associativity = sysconf_helper<std::size_t>(_SC_LEVEL3_CACHE_ASSOC).value_or(default_l3_cache[1]);
                res.sets = default_l3_cache[2];

                if( auto const cache_size_or = sysconf_helper<std::size_t>(_SC_LEVEL3_CACHE_SIZE); cache_size_or.has_value() ){
                    auto const cache_size = cache_size_or.value();
                    res.sets = cache_size / (res.associativity * res.line_size);
                }else{
                    return std::nullopt;
                }

                return res;
            #else
                return std::nullopt;
            #endif
        }

    } // namespace detal

    
    constexpr auto make_cache_manager() noexcept -> cache_manager_t{
        return cache_manager_t{detail::get_level_one(), detail::get_level_two(), detail::get_level_three()};
    }    

} // namespace boost::numeric::ublas

#endif // BOOST_NUMERIC_UBLAS_OS_LINUX_CACHE_HPP
