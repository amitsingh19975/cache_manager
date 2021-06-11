#if !defined(BOOST_NUMERIC_UBLAS_CACHE_MANAGER_HPP)
#define BOOST_NUMERIC_UBLAS_CACHE_MANAGER_HPP

#include "os/config.hpp"
#include <concepts>
#include <type_traits>
#include <optional>

namespace boost::numeric::ublas{
    
    struct cache_manager;

    struct cache_info{
        std::size_t line_size{};
        std::size_t associativity{};
        std::size_t sets{};
    };

    template<typename T>
    concept CacheManager = requires(T m){
        requires std::is_same_v<T, cache_manager>;
        
        typename T::value_type;
        typename T::base_type;
        typename T::size_type;
        typename T::const_reference;
        typename T::const_iterator;
        
        requires std::is_same_v<typename T::value_type, std::optional<cache_info>>;

        {m[0]} -> std::same_as<typename T::const_reference>;
        {m.at(0)} -> std::same_as<typename T::const_reference>;
        {m.begin()} -> std::same_as<typename T::const_iterator>;
        {m.end()} -> std::same_as<typename T::const_iterator>;
        {T::l1()} -> std::same_as<typename T::const_reference>;
        {T::l2()} -> std::same_as<typename T::const_reference>;
        {T::l3()} -> std::same_as<typename T::const_reference>;
        {T::is_valid(0)} -> std::same_as<bool>;
        {T::size(0)} -> std::same_as<std::optional<typename T::size_type>>;
        {T::line_size(0)} -> std::same_as<std::optional<typename T::size_type>>;
        {T::sets(0)} -> std::same_as<std::optional<typename T::size_type>>;
        {T::associativity(0)} -> std::same_as<std::optional<typename T::size_type>>;
    };


} // namespace boost::numeric::ublas

#ifndef BOOST_NUMERIC_UBLAS_DEFINE_NO_CACHE_MANAGER
#   if defined(BOOST_NUMERIC_UBLAS_MACOS)
        #include "os/mac/cache.hpp"
#   elif defined(BOOST_NUMERIC_UBLAS_LINUX)
        #include "os/linux/cache.hpp"
#   elif defined(BOOST_NUMERIC_UBLAS_WINDOWS)
        #include "os/windows/cache.hpp"
#   endif
#endif


#undef BOOST_NUMERIC_UBLAS_ARM
#undef BOOST_NUMERIC_UBLAS_LINUX
#undef BOOST_NUMERIC_UBLAS_MACOS
#undef BOOST_NUMERIC_UBLAS_WINDOWS

#endif // BOOST_NUMERIC_UBLAS_CACHE_MANAGER_HPP
