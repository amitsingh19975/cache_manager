#if !defined(BOOST_NUMERIC_UBLAS_CACHE_MANAGER_HPP)
#define BOOST_NUMERIC_UBLAS_CACHE_MANAGER_HPP

#include "os/config.hpp"
#include <concepts>
#include <type_traits>
#include <optional>

namespace boost::numeric::ublas{
    
    struct cache_manager_t;

    struct cache_info{
        std::size_t line_size{};
        std::size_t associativity{};
        std::size_t sets{};
    };

    /**
     * This function needs to be implemented by the user if
     * Operating System is not found. I choose this approach
     * because the function body can be defered but the object
     * body needs to defined before it could be used. Therefore,
     * if the OS is not found then the user has to define the body
     * before including the headers, but functions does not follow
     * this constraint. Other solution could be using templates, which
     * could be used as lazy evaluation, but it always requires the
     * cache manager to be a template parameter.
     */
    constexpr auto make_cache_manager() noexcept -> cache_manager_t;

    struct cache_manager_t{
        using value_type = std::optional<cache_info>;
        using base_type = std::array<value_type,3ul>;
        using size_type = typename base_type::size_type;
        using const_reference = typename base_type::const_reference;
        using const_iterator = typename base_type::const_iterator;

        explicit constexpr cache_manager_t(base_type data)
            : m_data(data)
        {}
        
        explicit constexpr cache_manager_t(value_type l1, value_type l2, value_type l3)
            : m_data{l1, l2, l3}
        {}

        constexpr cache_manager_t(cache_manager_t const&) = default;
        constexpr cache_manager_t(cache_manager_t &&) = default;
        constexpr cache_manager_t& operator=(cache_manager_t const&) = default;
        constexpr cache_manager_t& operator=(cache_manager_t &&) = default;
        ~cache_manager_t() = default;

        constexpr const_reference operator[](size_type k) const { return m_data[k]; }
        constexpr const_reference at(size_type k) const { return m_data.at(k); }

        constexpr const_iterator begin() const noexcept{ return m_data.begin(); }
        constexpr const_iterator end() const noexcept{ return m_data.end(); }

        constexpr const_reference l1() const noexcept{ return m_data[0ul]; }
        constexpr const_reference l2() const noexcept{ return m_data[1ul]; }
        constexpr const_reference l3() const noexcept{ return m_data[2ul]; }

        constexpr auto is_valid(std::size_t k) const noexcept 
            -> bool
        {
            return m_data[k].has_value();
        }

        constexpr auto size(size_type k) const
            -> std::optional<size_type>
        { 
            if(!is_valid(k)) return std::nullopt;
            auto const& cache = m_data[k];
            return cache->sets * cache->line_size * cache->associativity;
        }

        constexpr auto line_size(size_type k) const
            -> std::optional<size_type>
        { 
            if(!is_valid(k)) return std::nullopt;
            return m_data[k]->line_size;
        }

        constexpr auto sets(size_type k) const
            -> std::optional<size_type>
        { 
            if(!is_valid(k)) return std::nullopt;
            return m_data[k]->sets;
        }

        constexpr auto associativity(size_type k) const
            -> std::optional<size_type>
        { 
            if(!is_valid(k)) return std::nullopt;
            return m_data[k]->associativity;
        }

    private:
        base_type m_data;
    };
    
    // It avoids expensive system calls for every object construction.
    inline static cache_manager_t const cache_manager = make_cache_manager();

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
